/* Graph coloring register allocator
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Michael Matz <matzmich@cs.tu-berlin.de>
   and Daniel Berlin <dberlin@redhat.com>
   
   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "function.h"
#include "regs.h"
#include "obstack.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "df.h"
#include "sbitmap.h"
#include "expr.h"
#include "output.h"
/* TODO
   * do lots of commenting 
   * create definitions of ever-life regs at the beginning of
     the insn chain
   * create webs for all hardregs, not just those actually defined
     (so we later can use that to implement every constraint)
   * insert only one spill per insn and use/def
   * insert loads as soon, stores as late as possile
   * insert spill insns as outward as possible (either looptree, or LCM)
   * reuse stack-slots
   * use the frame-pointer, when we can
   * delete coalesced insns
   * create webs dynamically instead of one large array
     web2def[] should by like use2web[]
   * don't insert hard-regs, but a limited set of pseudo-reg
     in emit_colors, and setup reg_renumber accordingly
   * when spilling, update interference graph incrementally, which
     is possible, as we don't use global liveness
   * don't destroy coalescing information completely when spilling
   * use the constrains from asms
   * correctly handle SUBREG as being one hardreg on it's
     own, to handle such things:
     (set (subreg:SI (reg:DI 40) 0) (...))
     (set (reg:SI 41) (...)) 
     where it's clear from constrains, that 40 should go to
     0 and 41 to 1.  For now they conflict for the code below, although
     the don't in reality.
   * implement spill coalescing/propagation
   * implement optimistic coalescing
  */
struct dlist
{
  struct dlist *prev;
  struct dlist *next;
  union
  {
    struct web *web;
    struct move *move;
  }
  value;
};
#define DLIST_WEB(l) ((l)->value.web)
#define DLIST_MOVE(l) ((l)->value.move)

enum type
{
  INITIAL,
  PRECOLORED, SIMPLIFY, FREEZE, SPILL, SPILLED, COALESCED, COLORED, SELECT
};

struct web
{
  unsigned int id;
  unsigned int regno;
  unsigned int weight;
  unsigned int span_insns;
  unsigned int spill_temp;
  unsigned int use_my_regs;
  int spill_cost;
  int color;
  int add_hardregs;
  int num_conflicts;
  int num_uses;
  int num_defs;
  int move_related;
  enum type type;
  enum reg_class regclass;	/* just used for debugging */
  /* might be too much to store a HARD_REG_SET here for machines with _many_
     registers.  Shouldn't hurt for now.  */
  HARD_REG_SET usable_regs;
  int num_freedom;
  rtx reg_rtx;
  rtx stack_slot;
  struct ref **defs;		/* [0..num_defs-1] */
  struct ref **uses;		/* [0..num_uses-1] */
  struct web *uplink;
  struct web *next;
  struct web *alias;
  struct move_list *moves;
  struct dlist *dlink;
};

struct web_link
{
  struct web_link *next;
  struct web *web;
};

enum move_type
{
  WORKLIST, COEALESCED, CONSTRAINED, FROZEN, ACTIVE
};

struct move
{
  rtx insn;
  struct web *source_web;
  struct web *target_web;
  enum move_type type;
  struct dlist *dlink;
};

struct move_list
{
  struct move_list *next;
  struct move *move;
};

static int live_out PARAMS((struct df *, int, rtx));
static void live_in PARAMS((struct df *, int, rtx));
static void build_i_graph PARAMS ((struct df *));
static void debug_msg PARAMS ((int, const char *, ...)) ATTRIBUTE_PRINTF_2;
#define igraph_index(i, j) ((MAX (i, j) * (MAX (i, j) - 1))/2 + MIN (i, j))
FILE *file;

sbitmap igraph;
sbitmap visited;
sbitmap move_handled;
struct web *def2web;
struct web **use2web;
struct web_link **conflicts;
struct move_list *wl_moves;
static struct ref **all_defs_for_web;
static struct ref **all_uses_for_web;

/* Used to detect spill instructions inserted by me.  */
static int orig_max_uid;

static HARD_REG_SET never_use_colors;
static HARD_REG_SET usable_regs[N_REG_CLASSES];
static unsigned int num_free_regs[N_REG_CLASSES];

#define NUM_REGS(W) (((W)->type == PRECOLORED) ? 1 : (W)->num_freedom)

static const char *const reg_class_names[] = REG_CLASS_NAMES;

static int debug_new_regalloc = 2;

/* Print a message to the dump file, if debug_new_regalloc the
   same or greater than the specified level. */

static void debug_msg VPARAMS ((int level, const char *format, ...))
{
#ifndef ANSI_PROTOTYPES
  int level;
  const char *format;
#endif
  va_list ap;
  if (debug_new_regalloc >= level && rtl_dump_file != NULL)
    {
      VA_START (ap, format);

#ifndef ANSI_PROTOTYPES
      format = va_arg (ap, const char *);
#endif
      
      vfprintf (rtl_dump_file, format, ap);
      va_end (ap);
    }
}

static int copy_insn_p PARAMS ((rtx, rtx *, rtx *));
static int hard_regs_intersect_p PARAMS((HARD_REG_SET *, HARD_REG_SET *));
static void record_conflict PARAMS((struct web *, struct web *));
static void queue_conflict PARAMS((struct web *));
static void apply_conflicts PARAMS((void));
static void relink_web PARAMS((struct web *, struct web *));
static void clean_conflicts PARAMS((struct df *));
static void remember_move PARAMS((rtx));
static int live_out PARAMS((struct df *, int, rtx));
static void live_in PARAMS((struct df *, int, rtx));
static void handle_asm_insn PARAMS((struct df *, rtx));
static void prune_hardregs_for_mode PARAMS((HARD_REG_SET *, enum machine_mode));
static void init_one_web PARAMS((struct web *, int));
static void init_webs PARAMS((struct df *));
static void remember_web_was_spilled PARAMS((struct web *));
static void fill_misc_web_info PARAMS((struct df *));
static void moves_to_webs PARAMS ((struct df *));
static void connect_rmw_webs PARAMS((struct df *));
static void build_webs_and_conflicts PARAMS((struct df *));
static void build_i_graph PARAMS((struct df *));
static void dump_igraph PARAMS((struct df *));
static void alloc_mem PARAMS((struct df *));
static void free_mem PARAMS((struct df *));
static void push_list PARAMS((struct dlist *, struct dlist **));
static void remove_list PARAMS((struct dlist *, struct dlist **));
static struct dlist * pop_list PARAMS((struct dlist **));
static void free_dlist PARAMS((struct dlist **));
static void free_all_lists PARAMS((void));
static void mark_simplify PARAMS((struct web *));
static void build_worklists PARAMS((struct df *));
static void enable_move PARAMS((struct web *));
static void decrement_degree PARAMS((struct web *, int));
static void simplify PARAMS((void));
static void remove_move_1 PARAMS((struct web *, struct move *));
static void remove_move PARAMS((struct web *, struct move *));
static void merge_moves PARAMS((struct web *, struct web *));
static void add_worklist PARAMS((struct web *));
static int ok PARAMS((struct web *, struct web *));
static int conservative PARAMS((struct web *, struct web *));
static struct web * alias PARAMS((struct web *));
static void combine PARAMS((struct web *, struct web *));
static void coalesce PARAMS((void));
static void freeze_moves PARAMS((struct web *));
static void freeze PARAMS((void));
static void select_spill PARAMS((void));
static int get_free_reg PARAMS((HARD_REG_SET, enum machine_mode));
static int count_long_blocks PARAMS((HARD_REG_SET, int));
static void colorize_one_web PARAMS((struct web *));
static void assign_colors PARAMS((void));
static void allocate_spill_web PARAMS((struct web *));
static void rewrite_program PARAMS((struct df *));
static void emit_colors PARAMS((struct df *));
static int one_pass PARAMS((struct df *));
static void dump_ra PARAMS((struct df *));
static void init_ra PARAMS((void));
void mm_reg_alloc PARAMS((FILE *));



static int
copy_insn_p (insn, source, target)
     rtx insn;
     rtx *source;
     rtx *target;
{
  rtx d, s;
  int subreg_seen = 0;
  insn = single_set (insn);
  if (!insn)
    return 0;
  d = SET_DEST (insn);
  s = SET_SRC (insn);
  while (GET_CODE (d) == STRICT_LOW_PART || GET_CODE (d) == SUBREG)
    {
      if (GET_CODE (d) == SUBREG)
	subreg_seen = 1;
      d = XEXP (d, 0);
    }
  if (GET_CODE (d) != REG)
    return 0;
  while (GET_CODE (s) == STRICT_LOW_PART
	 || GET_CODE (s) == ZERO_EXTRACT || GET_CODE (s) == SIGN_EXTRACT)
    s = XEXP (s, 0);
  /* (set (reg:SI a) (subreg:SI (reg:DI b) 0)) will be a noop
     if a and b become the same register, so this move is a candidate
     for coalescing.
     (set (reg:SI a) (subreg:SI (reg:DI b) 1)) is similar,
     if a becomes hardreg(b)+1, but this isn't really coalesing,
     so for now we ignore such a move.  */
  if (GET_CODE (s) == SUBREG)
    {
      return 0;			/* XXX */
      subreg_seen = 1;
      if (SUBREG_WORD (s) != 0)
	return 0;
      else
	s = XEXP (s, 0);
    }
  if (GET_CODE (s) != REG)
    return 0;

  /* XXX for now disallow copy insns involving a subreg of
     a hardreg.  Those usually produce more than one def/use, and some
     of the parts of the code can't handle this.  (It basically would
     be a parallel copy of more than one register).  */
  if (subreg_seen)
    return 0;

  /* Copies between hardregs are useless for us, as not coalesable anyway. */
  if (REGNO (s) < FIRST_PSEUDO_REGISTER && REGNO (d) < FIRST_PSEUDO_REGISTER)
    return 0;

  if (source)
    *source = s;
  if (target)
    *target = d;
  return 1;
}

static int
hard_regs_intersect_p (a, b)
     HARD_REG_SET *a, *b;
{
  HARD_REG_SET c;
  COPY_HARD_REG_SET (c, *a);
  AND_HARD_REG_SET (c, *b);
  GO_IF_HARD_REG_SUBSET (c, reg_class_contents[(int) NO_REGS], lose);
  return 1;
lose:
  return 0;
}

static void
record_conflict (web1, web2)
     struct web *web1, *web2;
{
  struct web_link *wl;
  unsigned int id1 = web1->id, id2 = web2->id;
  /* trivial non-conflict */
  if (web1 == web2 || TEST_BIT (igraph, igraph_index (id1, id2)))
    return;
  /* As fixed_regs are no targets for allocation, conflicts with them
     are pointless.  */
  if ((web1->regno < FIRST_PSEUDO_REGISTER && fixed_regs[web1->regno])
      || (web2->regno < FIRST_PSEUDO_REGISTER && fixed_regs[web2->regno]))
    return;
  /* Conflicts between two hardregs, which are not even a candidate
     for this pseudo are also pointless.  */
  if (!((web1->type == PRECOLORED
	 && TEST_HARD_REG_BIT (web2->usable_regs, web1->regno))
	|| (web2->type == PRECOLORED
	    && TEST_HARD_REG_BIT (web1->usable_regs, web2->regno))
	|| hard_regs_intersect_p (&web1->usable_regs, &web2->usable_regs)))
    return;
  SET_BIT (igraph, igraph_index (id1, id2));
  if (web2->type != PRECOLORED)
    {
      wl = (struct web_link *) xmalloc (sizeof (struct web_link));
      wl->web = web1;
      wl->next = conflicts[id2];
      conflicts[id2] = wl;
      web2->num_conflicts += 1 + web1->add_hardregs;
    }
  if (web1->type != PRECOLORED)
    {
      wl = (struct web_link *) xmalloc (sizeof (struct web_link));
      wl->web = web2;
      wl->next = conflicts[id1];
      conflicts[id1] = wl;
      web1->num_conflicts += 1 + web2->add_hardregs;
    }
}

/* We build webs, as we process the conflicts.  For each use we go upward
   the insn stream, noting any defs as potentially conflicting with the
   current use.  We stop at defs of the current regno. The conflicts are only
   potentially, because we may never reach a def, so this is an undefined use,
   which conflicts with nothing. We do this by queuing all conflicts, and
   applying them, if we are done with this use.  As soon as we reach a def of
   the current regno we have a webpart.  This might be the first found for the
   current use, in which case this is the new current_web, or we already have
   a current web.  In the latter case we link the newly found part to the
   current web.
   If we iterate over all uses in this way, we build the webs.
   Note, that the whole runtime of build_i_graph struct web doesn't refer
   to web, but rather webparts, as they are not constructed fully yet.  */

/* Invariant: !current_web || current_web->uplink == NULL */
static struct web *current_web;
static struct web_link *queued_conflicts;

static void
queue_conflict (web)
     struct web *web;
{
  struct web_link *wl;
  wl = (struct web_link *) xmalloc (sizeof (struct web_link));
  wl->web = web;
  wl->next = queued_conflicts;
  queued_conflicts = wl;
}

static void
apply_conflicts (void)
{
  struct web_link *wl = queued_conflicts;
  struct web_link *wl_next = NULL;
  if (current_web)
    {
      for (; wl; wl = wl_next)
	{
	  wl_next = wl->next;
	  record_conflict (current_web, wl->web);
	  free (wl);
	}
    }
  else
    {
      /* We had no definition for the current use, so there can be no
         conflict.  */
      for (; wl; wl = wl_next)
	{
	  wl_next = wl->next;
	  free (wl);
	}
    }
  queued_conflicts = NULL;
}

/* We want to have conflicts only between roots of webs, so if we relinked
   a web, we need to reassign the conflicts.  */
static void
relink_web (web, new_root)
     struct web *web, *new_root;
{
  struct web_link *c = conflicts[web->id];
  if (web == new_root)
    return;
  for (; c; c = c->next)
    {
      if (!c->web->uplink)
	record_conflict (new_root, c->web);
      /* TODO: delete the conflicts between c->web and web */
    }
  web->uplink = new_root;
  new_root->weight += web->weight;
  /* span_insn are not correctly tracked. It's only used to detect
     short webs coming from spills.  */
  new_root->span_insns += web->span_insns;
  new_root->num_defs += web->num_defs;
  new_root->num_uses += web->num_uses;

  /* Link the web into new_root's ->next chain.  */
  while (new_root->next)
    new_root = new_root->next;
  new_root->next = web;
}

/* Removes all conflicts involving non-root webs.  */
static void
clean_conflicts (df)
     struct df *df;
{
  unsigned int id;
  for (id = 0; id < df->def_id; id++)
    {
      struct web *web = &def2web[id];
      struct web_link *c, *c_next;
      struct web_link *prev = NULL;
      for (c = conflicts[id]; c; c = c_next)
	{
	  c_next = c->next;
	  if (web->uplink || c->web->uplink)
	    {
	      if (!prev)
		conflicts[id] = c_next;
	      else
		prev->next = c_next;
	      RESET_BIT (igraph, igraph_index (id, c->web->id));
	      web->num_conflicts -= 1 + c->web->add_hardregs;
	      free (c);
	    }
	  else
	    prev = (!prev) ? conflicts[id] : prev->next;
	}
    }
}

static void
remember_move (insn)
     rtx insn;
{
  if (!TEST_BIT (move_handled, INSN_UID (insn)))
    {
      struct move *m = (struct move *) xcalloc (1, sizeof (struct move));
      struct move_list *ml;
      SET_BIT (move_handled, INSN_UID (insn));
      m->insn = insn;
      ml = (struct move_list *) xmalloc (sizeof (struct move_list));
      ml->move = m;
      ml->next = wl_moves;
      wl_moves = ml;
    }
}

static unsigned int spanned_insns;

static int
live_out (df, regno, insn)
     struct df *df;
     int regno;
     rtx insn;
{
  int defined = 0;
  struct web *this_web;
  if (INSN_P (insn))
    {
      struct df_link *link;
      int is_copy = 0;
      int source_regno;
      rtx s, t;
      if (copy_insn_p (insn, &s, &t))
	{
	  /* Some sanity test for the copy insn. */
	  struct df_link *slink = DF_INSN_USES (df, insn);
	  link = DF_INSN_DEFS (df, insn);
	  if (!link || !link->ref || !slink || !slink->ref)
	    abort ();
	  /* The following (link->next != 0) happens when a hardreg
	     is used in wider mode (REG:DI %eax).  Then df.* creates
	     a def/use for each hardreg contained therein.  We only
	     allow hardregs here.  */
	  if (link->next
	      && DF_REF_REGNO (link->next->ref) >= FIRST_PSEUDO_REGISTER)
	    abort ();
	  /* This happens with a read/modify/write insn. E.g.
	     a (set (subreg:SI (reg:DI a) 1) (reg:SI b)) .
	     But it would be still vavorable, if we coalesce both.
	     if (slink->next
	     && DF_REF_REGNO (slink->next->ref) >= FIRST_PSEUDO_REGISTER)
	     abort (); */
	  is_copy = 1;
	  source_regno = REGNO (s);
	  remember_move (insn);
	}
      for (link = DF_INSN_DEFS (df, insn); link; link = link->next)
	if (link->ref)
	  {
	    this_web = &def2web[DF_REF_ID (link->ref)];
	    while (this_web->uplink)
	      this_web = this_web->uplink;
	    this_web->weight++;
	    if (DF_REF_REGNO (link->ref) == regno)
	      {
		defined = 1;
		if (!current_web)
		  current_web = this_web;
		else if (current_web != this_web)
		  relink_web (this_web, current_web);
	      }
	    else if (!is_copy || regno != source_regno)
	      queue_conflict (this_web);
	  }
      spanned_insns++;
    }

  return !defined;
}

static void
live_in (df, regno, insn)
     struct df *df;
     int regno;
     rtx insn;
{
  while (1)
    {
      int uid = INSN_UID (insn);
      rtx p;
      if (TEST_BIT (visited, uid))
	return;
      SET_BIT (visited, uid);

      p = prev_real_insn (insn);
      if (!p)
	return;
      if (BLOCK_FOR_INSN (insn) == 0 || BLOCK_FOR_INSN (p) == 0
	  || BLOCK_NUM (insn) != BLOCK_NUM (p))
	{
	  edge e;
	  /* All but the last predecessor are handled recursively.  */
	  for (e = BLOCK_FOR_INSN (insn)->pred; e && e->pred_next;
	       e = e->pred_next)
	    if (live_out (df, regno, e->src->end))
	      live_in (df, regno, e->src->end);
	  if (!e)
	    return;
	  p = e->src->end;
	}
      if (live_out (df, regno, p))
	insn = p;
      else
	return;
    }
}



static void
handle_asm_insn (df, insn)
     struct df *df;
     rtx insn;
{
  const char *constraints[MAX_RECOG_OPERANDS];
  enum machine_mode operand_mode[MAX_RECOG_OPERANDS];
  int i, noperands, in_output, c;
  HARD_REG_SET clobbered, allowed, conflict;
  rtx pat;
  if (!INSN_P (insn) || (noperands = asm_noperands (PATTERN (insn))) < 0)
    return;
  pat = PATTERN (insn);
  CLEAR_HARD_REG_SET (clobbered);

  if (GET_CODE (pat) == PARALLEL)
    for (i = 0; i < XVECLEN (pat, 0); i++)
      {
	rtx t = XVECEXP (pat, 0, i);
	if (GET_CODE (t) == CLOBBER && GET_CODE (XEXP (t, 0)) == REG
	    && REGNO (XEXP (t, 0)) < FIRST_PSEUDO_REGISTER)
	  SET_HARD_REG_BIT (clobbered, REGNO (XEXP (t, 0)));
      }

  decode_asm_operands (pat, recog_data.operand, recog_data.operand_loc,
		       constraints, operand_mode);
  in_output = 1;
  for (i = 0; i < noperands; i++)
    {
      const char *p = constraints[i];
      int cls = (int) NO_REGS;
      struct df_link *link;
      rtx reg;
      struct web *web;
      int nothing_allowed = 1;
      reg = recog_data.operand[i];

      /* Look, if the constrains apply to a pseudo reg, and not to
         e.g. a mem.  */
      while (GET_CODE (reg) == SUBREG
	     || GET_CODE (reg) == ZERO_EXTRACT
	     || GET_CODE (reg) == SIGN_EXTRACT
	     || GET_CODE (reg) == STRICT_LOW_PART)
	reg = XEXP (reg, 0);
      if (GET_CODE (reg) != REG || REGNO (reg) < FIRST_PSEUDO_REGISTER)
	continue;

      /* Search the web corresponding to this operand.  We depend on
         that decode_asm_operands() places the output operands
         before the input operands.  */
      while (1)
	{
	  if (in_output)
	    link = df->insns[INSN_UID (insn)].defs;
	  else
	    link = df->insns[INSN_UID (insn)].uses;
	  while (link && link->ref && link->ref->reg != reg)
	    link = link->next;
	  if (!link || !link->ref)
	    {
	      if (in_output)
		in_output = 0;
	      else
		abort ();
	    }
	  else
	    break;
	}
      if (in_output)
	{
	  web = &def2web[DF_REF_ID (link->ref)];
	  while (web->uplink)
	    web = web->uplink;
	}
      else
	web = use2web[DF_REF_ID (link->ref)];

      /* Find the constrains, noting the allowed hardregs in allowed.  */
      CLEAR_HARD_REG_SET (allowed);
      while (1)
	{
	  char c = *p++;

	  if (c == '\0' || c == ',' || c == '#')
	    {
	      /* End of one alternative - mark the regs in the current
	         class, and reset the class.
	       */
	      IOR_HARD_REG_SET (allowed, reg_class_contents[cls]);
	      if (cls != NO_REGS)
		nothing_allowed = 0;
	      cls = NO_REGS;
	      if (c == '#')
		do
		  {
		    c = *p++;
		  }
		while (c != '\0' && c != ',');
	      if (c == '\0')
		break;
	      continue;
	    }

	  switch (c)
	    {
	    case '=':
	    case '+':
	    case '*':
	    case '%':
	    case '?':
	    case '!':
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case 'm':
	    case '<':
	    case '>':
	    case 'V':
	    case 'o':
	    case '&':
	    case 'E':
	    case 'F':
	    case 's':
	    case 'i':
	    case 'n':
	    case 'X':
	    case 'I':
	    case 'J':
	    case 'K':
	    case 'L':
	    case 'M':
	    case 'N':
	    case 'O':
	    case 'P':
	      break;

	    case 'p':
	      cls = (int) reg_class_subunion[cls][(int) BASE_REG_CLASS];
	      nothing_allowed = 0;
	      break;

	    case 'g':
	    case 'r':
	      cls = (int) reg_class_subunion[cls][(int) GENERAL_REGS];
	      nothing_allowed = 0;
	      break;

	    default:
	      cls =
		(int) reg_class_subunion[cls][(int)
					      REG_CLASS_FROM_LETTER (c)];
	    }
	}

      /* Now make conflicts between this web, and all hardregs, which
         are not allowed by the constrains.  */
      if (nothing_allowed)
	{
	  /* If we had no real constrains nothing was explicitely
	     allowed, so we allow the whole class (i.e. we make no
	     additional conflicts).  */
	  CLEAR_HARD_REG_SET (conflict);
	}
      else
	{
	  COPY_HARD_REG_SET (conflict, usable_regs
			     [reg_preferred_class (web->regno)]);
	  IOR_HARD_REG_SET (conflict, usable_regs
			    [reg_alternate_class (web->regno)]);
	  AND_COMPL_HARD_REG_SET (conflict, allowed);
	}

      debug_msg (0, " ASM constrain Web %d conflicts with:", web->id);
      for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
	if (TEST_HARD_REG_BIT (conflict, c))
	  debug_msg (0, " %d", c);
      debug_msg (0, "\n");
    }
}

/* Deletes all hardregs from *S which are not allowed for mode.  */
static void
prune_hardregs_for_mode (s, mode)
     HARD_REG_SET *s;
     enum machine_mode mode;
{
  HARD_REG_SET all;
  int i;

  /* We work by first noting _all_ hardregs for which MODE is OK (including
     it's consecutive regs), and later intersect that with *S.  
     We are not interested only in the beginning of a multi-reg, but in
     all the hardregs involved.  May be HARD_REGNO_MODE_OK() only ok's
     for beginnings.  */
  CLEAR_HARD_REG_SET (all);
  for (i = 0; i < FIRST_PSEUDO_REGISTER;)
    if (HARD_REGNO_MODE_OK (i, mode))
      {
	int size = HARD_REGNO_NREGS (i, mode);
	while (size-- && i < FIRST_PSEUDO_REGISTER)
	  {
	    SET_HARD_REG_BIT (all, i);
	    i++;
	  }
      }
    else
      i++;
  AND_HARD_REG_SET (*s, all);
}

static void
init_one_web (web, regno)
     struct web *web;
     int regno;
{
  /* web->id isn't initialized here. */
  web->regno = regno;
  web->weight = 0;
  web->span_insns = 0;
  web->spill_temp = 0;
  web->use_my_regs = 0;
  web->spill_cost = -1;
  web->num_defs = 1;
  web->num_uses = 0;
  /* XXX
     the former (superunion) doesn't constrain the graph enough. E.g.
     on x86 QImode requires QI_REGS, but as alternate class usually
     GENERAL_REGS is given.  So the graph is not constrained enough,
     thinking it has more freedom then it really has, which leads
     to repeated spill tryings.  OTOH the latter (only using preferred
     class) is too constrained, as normally (e.g. with all :SI mode
     pseudos), they can be allocated also in the alternate class.
     What we really want, are the _exact_ hard regs allowed, not
     just a class.  Later.  */
  /*web->regclass = reg_class_superunion
     [reg_preferred_class (web->regno)]
     [reg_alternate_class (web->regno)]; */
  /*web->regclass = reg_preferred_class (web->regno); */
  web->regclass = reg_class_subunion
    [reg_preferred_class (web->regno)][reg_alternate_class (web->regno)];
  if (web->regno < FIRST_PSEUDO_REGISTER)
    {
      web->color = web->regno;
      web->type = PRECOLORED;
      web->num_conflicts = INT_MAX;
      web->add_hardregs = 0;
      CLEAR_HARD_REG_SET (web->usable_regs);
      SET_HARD_REG_BIT (web->usable_regs, web->regno);
      web->num_freedom = 1;
    }
  else
    {
      int i;
      web->color = -1;
      web->type = INITIAL;
      /* add_hardregs is wrong in multi-length classes, e.g.
         using a DFmode pseudo on x86 can result in class FLOAT_INT_REG,
         where, if it finally is allocated to GENERAL_REGS it needs two,
         if allocated to FLOAT_REGS only one hardreg.  */
      web->add_hardregs =
	CLASS_MAX_NREGS (web->regclass, PSEUDO_REGNO_MODE (web->regno)) - 1;
      web->num_conflicts = web->add_hardregs;
      COPY_HARD_REG_SET (web->usable_regs,
			 reg_class_contents[reg_preferred_class
					    (web->regno)]);
      IOR_HARD_REG_SET (web->usable_regs,
			reg_class_contents[reg_alternate_class (web->regno)]);
      AND_COMPL_HARD_REG_SET (web->usable_regs, never_use_colors);
      prune_hardregs_for_mode (&web->usable_regs,
			       PSEUDO_REGNO_MODE (web->regno));
      web->num_freedom = 0;
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (TEST_HARD_REG_BIT (web->usable_regs, i))
	  web->num_freedom++;
    }
  web->move_related = 0;
  web->defs = NULL;
  web->uses = NULL;
  web->reg_rtx = NULL;
  web->stack_slot = NULL;
  web->uplink = NULL;
  web->next = NULL;
  web->alias = NULL;
  web->moves = NULL;
  web->dlink = NULL;
}

static void
init_webs (df)
     struct df *df;
{
  int regno;
  unsigned int defno;
  for (defno = 0; defno < df->def_id; defno++)
    {
      struct web *web = &def2web[defno];
      web->id = defno;
      init_one_web (web, DF_REF_REGNO (df->defs[defno]));
    }

  /* We want to have only one web for each precolored register.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    {
      struct web *root_web;
      struct df_link *link;
      for (link = df->regs[regno].defs; link && !link->ref; link =
	   link->next);
      if (!link)
	continue;
      root_web = &def2web[DF_REF_ID (link->ref)];
      for (link = link->next; link; link = link->next)
	if (link->ref)
	  {
	    struct web *web = &def2web[DF_REF_ID (link->ref)];
	    relink_web (web, root_web);
	  }
    }
}

static void
remember_web_was_spilled (web)
     struct web *web;
{
  int i, j;
  unsigned int found_size = 0;
  int adjust;
  web->spill_temp = 1;

  /* From now on don't use reg_pref/alt_class (regno) anymore for
     this web, but instead  usable_regs.  We can't use spill_temp for
     this, as it might get reset later, when we are coalesced to a
     non-spill-temp.  In that case we still want to use usable_regs.  */
  web->use_my_regs = 1;

  /* We don't constrain spill temporaries in any way for now.
     In every case it's wrong sometimes to have the same constrains or
     preferences, as the original pseudo, esp. if they were very narrow.
     (E.g. there once was a reg wanting class AREG (only one register)
     without alternative class.  As long, as also the spill-temps for
     this pseudo had the same constrains it was spilled over and over. */
  COPY_HARD_REG_SET (web->usable_regs, reg_class_contents[(int) ALL_REGS]);
  AND_COMPL_HARD_REG_SET (web->usable_regs, never_use_colors);
  prune_hardregs_for_mode (&web->usable_regs, PSEUDO_REGNO_MODE (web->regno));
  web->num_freedom = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (web->usable_regs, i))
      web->num_freedom++;

  /* Now look for a class, which is subset of our constrains, to
     setup add_hardregs, and regclass for debug output.  */
  web->regclass = NO_REGS;
  for (i = (int) ALL_REGS - 1; i > 0; i--)
    {
      int size;
      HARD_REG_SET test;
      COPY_HARD_REG_SET (test, reg_class_contents[i]);
      AND_COMPL_HARD_REG_SET (test, never_use_colors);
      GO_IF_HARD_REG_SUBSET (test, web->usable_regs, found);
      continue;
    found:
      /* Measure the actual number of bits which really are overlapping
         the target regset, not just the reg_class_size.  */
      size = 0;
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (test, j))
	  size++;
      if (found_size < size)
	{
	  web->regclass = (enum reg_class) i;
	  found_size = size;
	}
    }

  /* In case our weight changes, also all neighbors needs a change.  */
  adjust = web->add_hardregs;
  web->add_hardregs =
    CLASS_MAX_NREGS (web->regclass, PSEUDO_REGNO_MODE (web->regno)) - 1;
  adjust -= web->add_hardregs;
  if (adjust)
    {
      struct web_link *wl;
      web->num_conflicts -= adjust;
      for (wl = conflicts[web->id]; wl; wl = wl->next)
	wl->web->num_conflicts -= adjust;
    }
}

static void
fill_misc_web_info (df)
     struct df *df;
{
  int i;
  unsigned int no;
  struct ref **ref_use, **ref_def;
  /* Fill num_{uses,defs} and defs fields of the webs.  Set up also
     uses pointer.  */
  ref_use = all_uses_for_web;
  ref_def = all_defs_for_web;
  for (no = 0; no < df->def_id; no++)
    {
      struct web *web = &def2web[no];
      struct web *rweb;
      if (web->uplink)
	continue;
      web->defs = ref_def;
      web->uses = ref_use;
      ref_def += web->num_defs;
      ref_use += web->num_uses;
      rweb = web;
      for (i = web->num_defs - 1; i >= 0; i--)
	{
	  rweb->defs[i] = df->defs[web->id];
	  web = web->next;
	}
    }

  /* Fill uses array, and make sure use2web[] only contains webs, no
     webparts (web->uplink must be 0).  This look destroys the uses
     pointer, which is restored in the next loop.  */
  for (no = 0; no < df->use_id; no++)
    {
      struct web *web = use2web[no];
      if (web)
	{
	  while (web->uplink)
	    web = web->uplink;
	  /* We temporarily change the pointer, and adjust it back in the
	     next loop.  */
	  web->uses[0] = df->uses[no];
	  web->uses++;
	  use2web[no] = web;
	}
    }

  /* Restore uses pointer, and detect webs used for spill temporaries.  */
  for (no = 0; no < df->def_id; no++)
    {
      struct web *web = &def2web[no];
      if (web->uplink)
	continue;
      web->uses -= web->num_uses;

      /* Below only the detection of spill tempraries.  We never spill
         precolored webs, so those can't be spill temporaries.  The code above
         (remember_web_was_spilled) can't currently cope with hardregs
         anyway.  */
      if (web->regno < FIRST_PSEUDO_REGISTER)
	continue;

      /* A spill temporary has one def, one or more uses, all uses
         are in one insn, and either the def or use insn was inserted
         by me.  */
      /* XXX not correct currently. There might also be spill temps
         involving more than one def. Usually that's an additional
         clobber in the using instruction.  We might also constrain
         ourself to that, instead of like currently marking all
         webs involving spill insns at all.  */
      /*if (web->num_defs == 1 && web->num_uses > 0) */
      if (web->num_defs >= 1 && web->num_uses > 0)
	{
	  int i;
	  int one_insn = 1;
	  int bigspill = 0;
	  int spill_def = 0;
	  rtx ui = DF_REF_INSN (web->uses[0]);
	  for (i = 1; i < web->num_uses; i++)
	    if (ui != DF_REF_INSN (web->uses[i]))
	      {
		one_insn = 0;
		if (DF_REF_INSN_UID (web->uses[i]) >= orig_max_uid)
		  {
		    bigspill = 1;
		    break;
		  }
	      }
	  /* spill_def = (DF_REF_INSN_UID (web->defs[0]) >= orig_max_uid); */
	  for (i = 0; i < web->num_defs; i++)
	    if (DF_REF_INSN_UID (web->defs[i]) >= orig_max_uid)
	      {
		spill_def = 1;
		break;
	      }

	  if (DF_REF_INSN_UID (web->defs[0]) >= orig_max_uid
	      || bigspill || INSN_UID (ui) >= orig_max_uid)
	    {
	      remember_web_was_spilled (web);
	      /* Mark webs involving at least one spill insn as spill temps,
	         but mark them specially if they are not normal
	         load or store spills (have more than one using
	         instruction).  */
	      if (!one_insn || web->num_defs > 1)
		web->spill_temp = 2;
	    }
	}
    }
}

static void
moves_to_webs (df)
     struct df *df;
{
  struct df_link *link;
  struct move_list *ml;

  /* Distribute all moves to their corresponding webs, making sure,
     each move is in a web maximally one time (happens on some strange
     insns).  */
  for (ml = wl_moves; ml; ml = ml->next)
    {
      struct move *m = ml->move;
      struct web *web;
      struct move_list *newml;
      if (!m)
	continue;
      m->type = WORKLIST;
      m->dlink = NULL;
      link = DF_INSN_DEFS (df, m->insn);
      if (link->ref)
	{
	  web = &def2web[DF_REF_ID (link->ref)];
	  while (web->uplink)
	    web = web->uplink;
	  m->target_web = web;
	}
      link = DF_INSN_USES (df, m->insn);
      if (link->ref)
	{
	  web = use2web[DF_REF_ID (link->ref)];
	  m->source_web = web;
	}
      if (m->source_web && m->target_web)
	{
	  struct move_list *test = m->source_web->moves;
	  for (; test && test->move != m; test = test->next);
	  if (!test)
	    {
	      newml = (struct move_list *)
		xmalloc (sizeof (struct move_list));
	      newml->move = m;
	      newml->next = m->source_web->moves;
	      m->source_web->moves = newml;
	    }
	  test = m->target_web->moves;
	  for (; test && test->move != m; test = test->next);
	  if (!test)
	    {
	      newml = (struct move_list *)
		xmalloc (sizeof (struct move_list));
	      newml->move = m;
	      newml->next = m->target_web->moves;
	      m->target_web->moves = newml;
	    }
	}
      else
	{
	  /* Delete this move. At least one of the involved webs was
	     NULL.  */
	  free (m);
	  ml->move = NULL;
	}
    }
}

/* Here we look per insn, for DF references being in uses _and_ defs.
   This means, in the RTL a (REG xx) expression was seen as a
   read/modify/write, as happens for (set (subreg:SI (reg:DI xx)) (...))
   e.g.  Our code has created two webs for this, as it should.  Unfortunately,
   that the REG reference is only one time in the RTL we can't color
   both webs different (arguably this also would be wrong for a real
   read-mod-write instructions), so we must reconnect such webs.  */
static void
connect_rmw_webs (df)
     struct df *df;
{
  int i;
  for (i = 0; i < df->use_id; i++)
    {
      struct ref *ref;
      struct web *web = use2web[i];
      struct df_link *link;
      rtx reg;
      /* Not all uses have an associated web.  Those we can ignore.  */
      if (!web)
	continue;
      /* As we relink the webs in the loop, also use2web[] might have
         non-root webs.  */
      while (web->uplink)
	web = web->uplink;
      ref = df->uses[i];
      reg = DF_REF_REG (ref);
      for (link = DF_INSN_DEFS (df, DF_REF_INSN (ref)); link;
	   link = link->next)
	if (reg == DF_REF_REG (link->ref))
	  {
	    struct web *def_web = &def2web[DF_REF_ID (link->ref)];
	    while (def_web->uplink)
	      def_web = def_web->uplink;
	    relink_web (def_web, web);
	  }
    }
}

static void
build_webs_and_conflicts (df)
     struct df *df;
{
  unsigned int no;

  /* Here's the main loop.
     It goes through all insn's, builds web along the way, notes
     conflicts between webparts (not webs yet), and remembers move
     instructions.  */
  for (no = 0; no < df->use_id; no++)
    {
      rtx insn = DF_REF_INSN (df->uses[no]);
      int regno = DF_REF_REGNO (df->uses[no]);
      sbitmap_zero (visited);
      current_web = NULL;
      queued_conflicts = NULL;
      spanned_insns = 0;
      live_in (df, regno, insn);
      use2web[DF_REF_ID (df->uses[no])] = current_web;
      if (current_web)
	{
	  current_web->weight++;
	  current_web->span_insns += spanned_insns + 1;
	  current_web->num_uses++;
	}
      apply_conflicts ();
      if (copy_insn_p (insn, NULL, NULL))
	remember_move (insn);
    }
}

static void
build_i_graph (df)
     struct df *df;
{
  rtx insn;

  init_webs (df);

  sbitmap_zero (move_handled);
  wl_moves = NULL;

  build_webs_and_conflicts (df);

  /* For read-modify-write instructrions we may have created two webs.
     Reconnect them here.  (s.a.)  */
  connect_rmw_webs (df);

  /* The web are now complete, but in various arrays still references
     to the webparts are contained.  Clean this up, and fill some
     interesting info about the webs in them.  */

  clean_conflicts (df);
  fill_misc_web_info (df);
  moves_to_webs (df);

  /* Look for additional constrains given by asms.  */
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    handle_asm_insn (df, insn);
}

static void
dump_igraph (df)
     struct df *df;
{
  unsigned int mdef = df->def_id;
  struct move_list *ml;
  int def1, def2;
  int num = 0;
  int num2;
  if (rtl_dump_file == NULL)
    return;

  debug_msg (0, "conflicts:\n  ");
  for (def1 = 0; def1 < mdef; def1++)
    {
      int num1 = num;
      for (num2 = 0, def2 = 0; def2 < mdef; def2++)
	if (TEST_BIT (igraph, igraph_index (def1, def2)))
	  {
	    if (num1 == num)
	      debug_msg (0, "%d (REG %d) with ", def1, def2web[def1].regno);
	    if ((num2 % 9) == 8)
	      debug_msg (0, "\n              ");
	    num++;
	    num2++;
	    debug_msg (0, "%d(%d) ", def2, def2web[def2].regno);
	  }
      if (num1 != num)
	debug_msg (0, "\n  ");
    }
  debug_msg (0, "\n");
  for (ml = wl_moves; ml; ml = ml->next)
    if (ml->move)
      {
	debug_msg (0, "move: insn %d: Web %d <-- Web %d\n",
		   INSN_UID (ml->move->insn), ml->move->target_web->id,
		   ml->move->source_web->id);
      }
}

static void
alloc_mem (df)
     struct df *df;
{
  int mreg = df->def_id;
  igraph = sbitmap_alloc (mreg * mreg / 2);
  visited = sbitmap_alloc (get_max_uid ());
  move_handled = sbitmap_alloc (get_max_uid ());
  sbitmap_zero (igraph);
  sbitmap_zero (move_handled);
  sbitmap_zero (visited);
  def2web = (struct web *) xcalloc (mreg, sizeof def2web[0]);
  use2web = (struct web **) xcalloc (df->use_id, sizeof use2web[0]);
  conflicts = (struct web_link **) xcalloc (mreg, sizeof conflicts[0]);
  all_uses_for_web = (struct ref **) xcalloc (df->use_id,
					      sizeof all_uses_for_web[0]);
  all_defs_for_web = (struct ref **) xcalloc (df->def_id,
					      sizeof all_defs_for_web[0]);
}

static void
free_mem (df)
     struct df *df;
{
  struct web_link *wl, *wl_next;
  struct move_list *ml, *ml_next;
  unsigned int i;

  for (i = 0; i < df->def_id; i++)
    {
      struct web *web = &def2web[i];
      for (wl = conflicts[i]; wl; wl = wl_next)
	{
	  wl_next = wl->next;
	  free (wl);
	}
      for (ml = web->moves; ml; ml = ml_next)
	{
	  ml_next = ml->next;
	  free (ml);
	}
      conflicts[i] = NULL;
      web->moves = NULL;
    }
  for (ml = wl_moves; ml; ml = ml_next)
    {
      ml_next = ml->next;
      if (ml->move)
	free (ml->move);
      free (ml);
    }
  wl_moves = NULL;

  free (all_uses_for_web);
  free (all_defs_for_web);
  free (conflicts);
  free (use2web);
  free (def2web);

  free (move_handled);
  free (visited);
  free (igraph);
}

static void
push_list (x, list)
     struct dlist *x;
     struct dlist **list;
{
  x->next = *list;
  if (*list)
    (*list)->prev = x;
  *list = x;
}

static void
remove_list (x, list)
     struct dlist *x;
     struct dlist **list;
{
  struct dlist *y = x->prev;
  if (y)
    y->next = x->next;
  else
    *list = x->next;
  y = x->next;
  if (y)
    y->prev = x->prev;
  x->next = x->prev = NULL;
}

static struct dlist *
pop_list (list)
     struct dlist **list;
{
  struct dlist *r = *list;
  if (r)
    remove_list (r, list);
  return r;
}

static void
free_dlist (list)
     struct dlist **list;
{
  struct dlist *d;
  for (d = pop_list (list); d; d = pop_list (list))
    free (d);
  *list = NULL;
}


/*

   Here begins the real allocator. 

   */

static struct dlist *simplify_wl, *freeze_wl, *spill_wl, *spilled_nodes;
static struct dlist *simplify_spilled_wl;
static struct dlist *coalesced_nodes, *colored_nodes;
static struct dlist *select_stack;

static struct dlist *mv_worklist, *mv_coalsced, *mv_constrained;
static struct dlist *mv_frozen, *mv_active;

static void
free_all_lists (void)
{
  free_dlist (&simplify_wl);
  free_dlist (&simplify_spilled_wl);
  free_dlist (&freeze_wl);
  free_dlist (&spill_wl);
  free_dlist (&spilled_nodes);
  free_dlist (&coalesced_nodes);
  free_dlist (&colored_nodes);
  free_dlist (&select_stack);
  free_dlist (&mv_worklist);
  free_dlist (&mv_coalsced);
  free_dlist (&mv_constrained);
  free_dlist (&mv_frozen);
  free_dlist (&mv_active);
}

static void
mark_simplify (web)
     struct web *web;
{
  if (web->spill_temp)
    push_list (web->dlink, &simplify_spilled_wl);
  else
    push_list (web->dlink, &simplify_wl);
  web->type = SIMPLIFY;
}

static void
build_worklists (df)
     struct df *df;
{
  unsigned int i;
  struct move_list *ml;
  for (i = 0; i < df->def_id; i++)
    if (!def2web[i].uplink)
      {
	struct web *web = &def2web[i];
	struct dlist *d = (struct dlist *) xcalloc (1, sizeof (struct dlist));
	DLIST_WEB (d) = web;
	web->dlink = d;
	if (web->type == PRECOLORED)
	  continue;

	if (web->num_conflicts >= NUM_REGS (web))
	  {
	    push_list (d, &spill_wl);
	    web->type = SPILL;
	  }
	else if (web->moves)
	  {
	    push_list (d, &freeze_wl);
	    web->type = FREEZE;
	  }
	else
	  mark_simplify (web);
      }

  for (ml = wl_moves; ml; ml = ml->next)
    if (ml->move)
      {
	struct dlist *d = (struct dlist *) xcalloc (1, sizeof (struct dlist));
	DLIST_MOVE (d) = ml->move;
	ml->move->dlink = d;
	ml->move->type = WORKLIST;
	push_list (d, &mv_worklist);
      }
}

static void
enable_move (web)
     struct web *web;
{
  struct move_list *ml;
  for (ml = web->moves; ml; ml = ml->next)
    if (ml->move->type == ACTIVE)
      {
	remove_list (ml->move->dlink, &mv_active);
	push_list (ml->move->dlink, &mv_worklist);
	ml->move->type = WORKLIST;
      }
}

static void
decrement_degree (web, dec)
     struct web *web;
     int dec;
{
  int before = web->num_conflicts;
  web->num_conflicts -= dec;
  if (web->num_conflicts < NUM_REGS (web) && before >= NUM_REGS (web))
    {
      struct web_link *a;
      enable_move (web);
      for (a = conflicts[web->id]; a; a = a->next)
	if (a->web->type != SELECT && a->web->type != COALESCED)
	  enable_move (a->web);
      if (web->type != FREEZE)
	{
	  if (web->type != SPILL)
	    abort ();
	  remove_list (web->dlink, &spill_wl);
	  if (web->moves)
	    {
	      push_list (web->dlink, &freeze_wl);
	      web->type = FREEZE;
	    }
	  else
	    mark_simplify (web);
	}
    }
}

static void
simplify (void)
{
  struct dlist *d;
  struct web *web;
  struct web_link *wl;
  while (1)
    {
      /* We try hard to color all the webs resulting from spills first.
         Without that on register starved machines (x86 e.g) with some live
         DImode pseudos, -fPIC, and an asm requiring %edx, it might be, that
         we do rounds over rounds, because the conflict graph says, we can
         simplify those short webs, but later due to irregularities we can't
         color those pseudos.  So we have to spill them, which in later rounds
         leads to other spills.  */
      if (simplify_wl)
	d = pop_list (&simplify_wl);
      else
	d = pop_list (&simplify_spilled_wl);
      if (!d)
	break;
      web = DLIST_WEB (d);
      debug_msg (0, " simplifying web %3d, conflicts = %d\n", web->id,
		 web->num_conflicts);
      push_list (d, &select_stack);
      web->type = SELECT;
      for (wl = conflicts[web->id]; wl; wl = wl->next)
	if (wl->web->type != SELECT && wl->web->type != COALESCED)
	  {
	    decrement_degree (wl->web, 1 + web->add_hardregs);
	  }
    }
}

static void
remove_move_1 (web, move)
     struct web *web;
     struct move *move;
{
  struct move_list *ml = web->moves;
  if (!ml)
    return;
  if (ml->move == move)
    {
      web->moves = ml->next;
      free (ml);
      return;
    }
  for (; ml->next && ml->next->move != move; ml = ml->next);
  if (!ml->next)
    return;
  {
    struct move_list *old = ml->next;
    ml->next = ml->next->next;
    free (old);
  }
}

static void
remove_move (web, move)
     struct web *web;
     struct move *move;
{
  struct move_list *ml;
  remove_move_1 (web, move);
  for (ml = web->moves; ml; ml = ml->next)
    if (ml->move == move)
      abort ();
}

static void
merge_moves (u, v)
     struct web *u, *v;
{
  regset seen;
  struct move_list *ml, *ml_next;

  seen = BITMAP_XMALLOC ();
  for (ml = u->moves; ml; ml = ml->next)
    SET_REGNO_REG_SET (seen, INSN_UID (ml->move->insn));
  for (ml = v->moves; ml; ml = ml_next)
    {
      ml_next = ml->next;
      if (!REGNO_REG_SET_P (seen, INSN_UID (ml->move->insn)))
	{
	  ml->next = u->moves;
	  u->moves = ml;
	}
      else
	free (ml);
    }
  BITMAP_XFREE (seen);
  v->moves = NULL;
}

static void
add_worklist (web)
     struct web *web;
{
  if (web->type != PRECOLORED && !web->moves
      && web->num_conflicts < NUM_REGS (web))
    {
      remove_list (web->dlink, &freeze_wl);
      mark_simplify (web);
    }
}

static int
ok (target, source)
     struct web *target, *source;
{
  struct web_link *wl;

  /* Source is PRECOLORED.  We test here, if it isn't one of the fixed
     registers.  In this case we disallow the coalescing.
     XXX maybe not all fixe_regs[] have to be excluded.  The actual
     example I was looking at, was a copy (set (reg a) (reg 7 sp)) (the
     stackpointer).  It all worked till the assembler. The coalescing
     resulted in a (%eax, %esp) indexed address, which is invalid.
     May be we only need to exclude stackpointers.  Esp. the PIC register
     is interesting.  I guess we can anyway make it available for coloring,
     as long as it doesn't interfere with any current use.  */
  if (TEST_HARD_REG_BIT (never_use_colors, source->color))
    return 0;

  for (wl = conflicts[target->id]; wl; wl = wl->next)
    if (wl->web->type != SELECT && wl->web->type != COALESCED)
      {
	if (!(wl->web->num_conflicts < NUM_REGS (wl->web)
	      || wl->web->type == PRECOLORED
	      || TEST_BIT (igraph, igraph_index (source->id, wl->web->id))))
	  return 0;
      }
  return 1;
}

static int
conservative (target, source)
     struct web *target, *source;
{
  unsigned int k;
  regset seen;
  struct web_link *wl;
  unsigned int num_regs = NUM_REGS (target);	/* XXX */

  /* k counts the resulting conflict weight, if target and source
     would be merged, and all low-degree neightbors would be
     removed.  */
  k = MAX (target->add_hardregs, source->add_hardregs);
  seen = BITMAP_XMALLOC ();
  for (wl = conflicts[target->id]; wl; wl = wl->next)
    if (wl->web->type != SELECT && wl->web->type != COALESCED
	&& wl->web->num_conflicts >= NUM_REGS (wl->web)
	&& !REGNO_REG_SET_P (seen, wl->web->id))
      {
	SET_REGNO_REG_SET (seen, wl->web->id);
	k += 1 + wl->web->add_hardregs;
      }
  for (wl = conflicts[source->id]; wl; wl = wl->next)
    if (wl->web->type != SELECT && wl->web->type != COALESCED
	&& wl->web->num_conflicts >= NUM_REGS (wl->web)
	&& !REGNO_REG_SET_P (seen, wl->web->id))
      {
	SET_REGNO_REG_SET (seen, wl->web->id);
	k += 1 + wl->web->add_hardregs;
      }
  BITMAP_XFREE (seen);

  if (k >= num_regs)
    return 0;
  return 1;
}

static struct web *
alias (web)
     struct web *web;
{
  while (web->type == COALESCED)
    web = web->alias;
  return web;
}

static void
combine (u, v)
     struct web *u, *v;
{
  struct web_link *wl;
  if (v->type == FREEZE)
    remove_list (v->dlink, &freeze_wl);
  else
    remove_list (v->dlink, &spill_wl);
  push_list (v->dlink, &coalesced_nodes);
  v->type = COALESCED;
  v->alias = u;
  merge_moves (u, v);
  /* XXX combine add_hardregs's of U and V.  */
  for (wl = conflicts[v->id]; wl; wl = wl->next)
    if (wl->web->type != SELECT && wl->web->type != COALESCED)
      {
	record_conflict (u, wl->web);
	decrement_degree (wl->web, 1 + v->add_hardregs);
      }
  if (u->num_conflicts >= NUM_REGS (u) && u->type == FREEZE)
    {
      remove_list (u->dlink, &freeze_wl);
      push_list (u->dlink, &spill_wl);
      u->type = SPILL;
    }

  /* XXX combine the usable_regs together, and adjust the weight and
     conflict.  As this might constrain more, it might make us oncolorable.
     I think we should disallow this in conservative test, so only
     web with similar constrains are coalesced.  */
  /* If we coalesce a spill temporary together with a normal node,
     the result is not any longer a spill temporary.  */
  if (u->spill_temp && !v->spill_temp)
    u->spill_temp = 0;
}

static void
coalesce (void)
{
  struct dlist *d = pop_list (&mv_worklist);
  struct move *m = DLIST_MOVE (d);
  struct web *source = alias (m->source_web);
  struct web *target = alias (m->target_web);

  if (target->type == PRECOLORED)
    {
      struct web *h = source;
      source = target;
      target = h;
    }
  if (source == target)
    {
      remove_move (source, m);
      push_list (d, &mv_coalsced);
      m->type = COALESCED;
      add_worklist (source);
    }
  else if (target->type == PRECOLORED
	   || TEST_BIT (igraph, igraph_index (source->id, target->id)))
    {
      remove_move (source, m);
      remove_move (target, m);
      push_list (d, &mv_constrained);
      m->type = CONSTRAINED;
      add_worklist (source);
      add_worklist (target);
    }
  else if ((source->type == PRECOLORED && ok (target, source))
	   || (source->type != PRECOLORED && conservative (target, source)))
    {
      remove_move (source, m);
      remove_move (target, m);
      push_list (d, &mv_coalsced);
      m->type = COALESCED;
      combine (source, target);
      add_worklist (source);
    }
  else
    {
      push_list (d, &mv_active);
      m->type = ACTIVE;
    }
}

static void
freeze_moves (web)
     struct web *web;
{
  struct move_list *ml, *ml_next;
  for (ml = web->moves; ml; ml = ml_next)
    {
      struct move *m = ml->move;
      struct web *s, *d;
      ml_next = ml->next;
      if (m->type == ACTIVE)
	remove_list (m->dlink, &mv_active);
      else
	remove_list (m->dlink, &mv_worklist);
      push_list (m->dlink, &mv_frozen);
      m->type = FROZEN;
      remove_move (web, m);
      s = alias (m->source_web);
      d = alias (m->target_web);
      s = (s == web) ? d : s;
      remove_move (s, m);
      /* XXX GA use the original v, instead of alias(v) */
      if (!s->moves && s->num_conflicts < NUM_REGS (s))
	{
	  remove_list (s->dlink, &freeze_wl);
	  mark_simplify (s);
	}
    }
}

static void
freeze (void)
{
  struct dlist *d = pop_list (&freeze_wl);
  mark_simplify (DLIST_WEB (d));
  freeze_moves (DLIST_WEB (d));
}
static float (*spill_heuristic) PARAMS ((struct web *));

static float default_spill_heuristic PARAMS ((struct web *));

static float
default_spill_heuristic (w)
     struct web *w;
{
  return (float) ((float) w->weight * (float) reg_spill_cost (w->regno)) /
    (float) w->num_conflicts;
}

static void
select_spill (void)
{
  float best = INT_MAX;
  struct dlist *bestd = NULL;
  struct dlist *d;
  for (d = spill_wl; d; d = d->next)
    {
      struct web *w = DLIST_WEB (d);
      float weight = spill_heuristic (w);
      if (!w->spill_temp && weight < best)
	{
	  best = weight;
	  bestd = d;
	}
    }
  if (!bestd)
    abort ();

  remove_list (bestd, &spill_wl);
  DLIST_WEB (bestd)->spill_cost = best;	/* Note the potential spill.  */
  mark_simplify (DLIST_WEB (bestd));
  freeze_moves (DLIST_WEB (bestd));
  debug_msg (0, " potential spill web %3d, conflicts = %d\n",
	     DLIST_WEB (bestd)->id, DLIST_WEB (bestd)->num_conflicts);
}

/* Searches in FREE_COLORS for a block of hardregs of the right length
   for MODE.  If it needs more than one hardreg it prefers blocks beginning
   at an even hardreg, and only given an odd begin reg if no other
   block could be found.  */
static int
get_free_reg (free_colors, mode)
     HARD_REG_SET free_colors;
     enum machine_mode mode;
{
  int c;
  int last_resort = -1;
  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
    if (TEST_HARD_REG_BIT (free_colors, c) && HARD_REGNO_MODE_OK (c, mode))
      {
	int i, size;
	size = HARD_REGNO_NREGS (c, mode);
	for (i = 1; i < size && TEST_HARD_REG_BIT (free_colors, c + i); i++);
	if (i == size)
	  {
	    if (size < 2 || (c & 1) == 0)
	      return c;
	    else if (last_resort >= 0)
	      return last_resort;
	    else
	      last_resort = c;
	  }
	else
	  c += i;
      }
  return last_resort;
}

/* Counts the number of non-overlapping bitblocks of length LEN
   in FREE_COLORS.  */
static int
count_long_blocks (free_colors, len)
     HARD_REG_SET free_colors;
     int len;
{
  int i, j;
  int count = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (!TEST_HARD_REG_BIT (free_colors, i))
	continue;
      for (j = 1; j < len; j++)
	if (!TEST_HARD_REG_BIT (free_colors, i + j))
	  break;
      /* Bits [i .. i+j-1] are free.  */
      if (j == len)
	count++;
      i += j - 1;
    }
  return count;
}

/* This is a little bit hairy, as it tries very hard, to not constrain
   the uncolored non-spill neighbors, which need more hardregs then
   we.  Consider a situation, 2 hardregs free for us (0 and 1),
   and one of our neighbors needs 2 hardregs, and only conflicts with us.
   There are 3 hardregs at all.  Now a simple minded might choose 1
   as color for us.  Then our neighbor has two free colors (0 and 2) as it
   should, but they are not consecutive, so coloring it later would fail.
   This leads to nasty problems on register starved machines, so we try
   to avoid this.  */
static void
colorize_one_web (web)
     struct web *web;
{
  struct web_link *wl;
  int i;
  HARD_REG_SET colors, conflict_colors;
  int c = -1;
  int bestc = -1;
  int neighbor_needs = 0;
  struct web *fat_neighbor = NULL;
  int num_fat = 0;

  CLEAR_HARD_REG_SET (conflict_colors);
  neighbor_needs = web->add_hardregs + 1;
  for (wl = conflicts[web->id]; wl; wl = wl->next)
    {
      struct web *w = alias (wl->web);
      if (w->type == COLORED || w->type == PRECOLORED)
	{
	  for (i = 0; i < 1 + w->add_hardregs; i++)
	    SET_HARD_REG_BIT (conflict_colors, w->color + i);
	}
      else if (w->spill_cost < 0 && w->add_hardregs >= neighbor_needs)
	{
	  neighbor_needs = w->add_hardregs;
	  fat_neighbor = w;
	  num_fat++;
	}
    }

  while (1)
    {
      int long_blocks;
      if (web->use_my_regs)
	COPY_HARD_REG_SET (colors, web->usable_regs);
      else
	COPY_HARD_REG_SET (colors,
			   usable_regs[reg_preferred_class (web->regno)]);
      AND_COMPL_HARD_REG_SET (colors, conflict_colors);
      c = get_free_reg (colors, PSEUDO_REGNO_MODE (web->regno));
      if (!web->use_my_regs && c < 0)
	{
	  IOR_HARD_REG_SET (colors, usable_regs
			    [reg_alternate_class (web->regno)]);
	  AND_COMPL_HARD_REG_SET (colors, conflict_colors);
	  c = get_free_reg (colors, PSEUDO_REGNO_MODE (web->regno));
	}
      if (c < 0)
	break;
      bestc = c;
      /* If one of the yet uncolored neighbors, which is not a potential
         spill needs a block of hardregs be sure, not to destroy such a block
         by coloring one reg in the middle.  */
      if (num_fat)
	{
	  int i;
	  int new_long;
	  if (fat_neighbor->use_my_regs)
	    COPY_HARD_REG_SET (colors, fat_neighbor->usable_regs);
	  else
	    {
	      COPY_HARD_REG_SET (colors, usable_regs
				 [reg_preferred_class (fat_neighbor->regno)]);
	      IOR_HARD_REG_SET (colors, usable_regs
				[reg_alternate_class (fat_neighbor->regno)]);
	    }
	  long_blocks = count_long_blocks (colors, neighbor_needs + 1);
	  for (i = 0; i < 1 + web->add_hardregs; i++)
	    CLEAR_HARD_REG_BIT (colors, c + i);
	  new_long = count_long_blocks (colors, neighbor_needs + 1);
	  /* If we changed the number of long blocks, and it's now smaller
	     than needed, we try to avoid this color.  */
	  if (long_blocks != new_long && new_long < num_fat)
	    SET_HARD_REG_BIT (conflict_colors, c);
	  else
	    /* We found a color which doesn't destroy a block.  */
	    break;
	}
      else
	break;
    }
  if (bestc >= 0 && c < 0 && web->spill_cost < 0)
    {
      /* This is a non-potential-spill web, which got a color, which did
         destroy a hardreg block for one of it's neighbors.  We color
         this web anyway and hope for the best for the neighbor.  */
      //c = bestc;
      debug_msg (0, "  *** Non-spill web %d colored with %d, constrains"
		 " it's neighbors\n", web->id, c);
    }
  if (c < 0)
    {
      /* Guard against a simplified node being spilled.  */
      /* Don't abort.  This can happen, when e.g. enough registers
         are available in colors, but they are not consecutive.
         if (DLIST_WEB (d)->spill_cost < 0)
         abort (); */
      if (web->spill_cost < 0)
	{
	  debug_msg (0,
		     "  *** Web %d spilled, although it was simplified "
		     "[free = %x, mode = %s]\n", web->id, colors[0],
		     GET_MODE_NAME (PSEUDO_REGNO_MODE (web->regno)));
	}
      if (web->spill_temp)
	{
	  debug_msg (0,
		     "  *** Web %d spilled, although it's a spilltemp\n",
		     web->id);
	}
      push_list (web->dlink, &spilled_nodes);
      web->type = SPILLED;
    }
  else
    {
      push_list (web->dlink, &colored_nodes);
      web->type = COLORED;
      web->color = c;
    }
}

static void
assign_colors (void)
{
  struct dlist *d;
  while (select_stack)
    {
      d = pop_list (&select_stack);
      colorize_one_web (DLIST_WEB (d));
    }

  for (d = coalesced_nodes; d; d = d->next)
    {
      struct web *a = alias (DLIST_WEB (d));
      DLIST_WEB (d)->color = a->color;
    }
}

static void
allocate_spill_web (web)
     struct web *web;
{
  int regno = web->regno;
  rtx slot;
  unsigned int inherent_size = PSEUDO_REGNO_BYTES (regno);
  /* XXX
     unsigned int total_size = MAX (inherent_size, reg_max_ref_width[i]); */
  unsigned int total_size = MAX (inherent_size, 0);
  if (web->stack_slot)
    abort ();
  slot = assign_stack_local (PSEUDO_REGNO_MODE (regno), total_size,
			     inherent_size == total_size ? 0 : -1);
  RTX_UNCHANGING_P (slot) = RTX_UNCHANGING_P (regno_reg_rtx[regno]);
  MEM_ALIAS_SET (slot) = new_alias_set ();
  web->stack_slot = slot;
}

static void
rewrite_program (df)
     struct df *df;
{
  int i;
  struct web *web;
  for (i = 0; i < df->def_id; i++)
    {
      web = &def2web[i];
      while (web->uplink)
	web = web->uplink;
      if (web->type == SPILLED)
	{
	  rtx insn = DF_REF_INSN (df->defs[i]);
	  rtx following = NEXT_INSN (insn);
	  basic_block bb = BLOCK_FOR_INSN (insn);
	  rtx insns;
	  if (!web->stack_slot)
	    allocate_spill_web (web);
	  start_sequence ();
	  emit_insn (gen_move_insn (web->stack_slot,
				    DF_REF_REG (df->defs[i])));
	  insns = get_insns ();
	  end_sequence ();
	  emit_insns_after (insns, insn);
	  if (bb->end == insn)
	    bb->end = PREV_INSN (following);
	  for (; insn != following; insn = NEXT_INSN (insn))
	    set_block_for_insn (insn, bb);
	}
    }
  for (i = 0; i < df->use_id; i++)
    {
      web = use2web[i];
      if (web && web->type == SPILLED)
	{
	  rtx insn = DF_REF_INSN (df->uses[i]);
	  rtx prev = PREV_INSN (insn);
	  basic_block bb = BLOCK_FOR_INSN (insn);
	  rtx insns;
	  start_sequence ();
	  emit_insn (gen_move_insn (DF_REF_REG (df->uses[i]),
				    web->stack_slot));
	  insns = get_insns ();
	  end_sequence ();
	  emit_insns_before (insns, insn);
	  if (bb->head == insn)
	    bb->head = NEXT_INSN (prev);
	  for (; insn != prev; insn = PREV_INSN (insn))
	    set_block_for_insn (insn, bb);
	}
    }
}

static void
emit_colors (df)
     struct df *df;
{
  /*
     struct dlist *d;
     while (d = pop_list (&colored_nodes))
     {
     struct web *web = DLIST_WEB (d);

     }
   */
  unsigned int i;
  struct web *web;
  /* First create the (REG xx) rtx's for all webs, as we need to know
     the number, to make sure, flow has enough memory for them in the
     various tables.  */
  for (i = 0; i < df->def_id; i++)
    {
      web = &def2web[i];
      if (web->uplink)
	continue;
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      if (web->reg_rtx || web->regno < FIRST_PSEUDO_REGISTER)
	abort ();
      //web->reg_rtx = gen_rtx_REG (PSEUDO_REGNO_MODE (web->regno), web->color);
      web->reg_rtx = gen_reg_rtx (PSEUDO_REGNO_MODE (web->regno));
    }
  max_regno = max_reg_num ();
  allocate_reg_info (max_regno, FALSE, TRUE);

  /* Then go through all references, and replace them by a new
     pseudoreg for each web.  All defs.  */
  for (i = 0; i < df->def_id; i++)
    {
      regset rs = DF_REF_BB (df->defs[i])->global_live_at_start;
      web = &def2web[i];
      /* For defs the web is not already the canonical one.  */
      while (web && web->uplink)
	web = web->uplink;
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      *df->defs[i]->loc = web->reg_rtx;
      if (REGNO_REG_SET_P (rs, web->regno))
	{
	  /* Don't simply clear the current regno, as it might be
	     replaced by two webs.  */
	  //CLEAR_REGNO_REG_SET (rs, web->regno);
	  SET_REGNO_REG_SET (rs, REGNO (web->reg_rtx));
	}
    }

  /* And all uses.  */
  for (i = 0; i < df->use_id; i++)
    {
      regset rs = DF_REF_BB (df->uses[i])->global_live_at_start;
      web = use2web[i];
      if (!web || (web->type != COLORED && web->type != COALESCED))
	continue;
      *df->uses[i]->loc = web->reg_rtx;
      if (REGNO_REG_SET_P (rs, web->regno))
	{
	  //CLEAR_REGNO_REG_SET (rs, web->regno);
	  SET_REGNO_REG_SET (rs, REGNO (web->reg_rtx));
	}
    }

  /* And now set up the reg_renumber array for reload with all the new
     pseudo-regs.  */
  for (i = 0; i < df->def_id; i++)
    {
      web = &def2web[i];
      if (web->uplink)
	continue;
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      reg_renumber[REGNO (web->reg_rtx)] = web->color;
    }
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    debug_msg (0, "Renumber pseudo %d to %d\n", i, reg_renumber[i]);
}

static int
one_pass (df)
     struct df *df;
{
  build_i_graph (df);
  dump_igraph (df);
  build_worklists (df);
  do
    {
      simplify ();
      if (mv_worklist)
	coalesce ();
      else if (freeze_wl)
	freeze ();
      else if (spill_wl)
	select_spill ();
    }
  while (simplify_wl || simplify_spilled_wl || mv_worklist
	 || freeze_wl || spill_wl);
  assign_colors ();
  if (spilled_nodes)
    {
      rewrite_program (df);
      return 1;
    }
  return 0;
}

static void
dump_ra (df)
     struct df *df;
{
  struct web *web;
  struct dlist *d;
  unsigned int i;
  debug_msg (0, "Webs:\n");
  for (i = 0; i < df->def_id; i++)
    if (!def2web[i].uplink)
      {
	web = &def2web[i];
	debug_msg (0, "  %4d : regno %3d +%d (span %d) (%s)%s", i, web->regno,
		   web->add_hardregs, web->span_insns,
		   reg_class_names[web->regclass],
		   web->spill_temp ? " (spilltemp)" : "");
	if (web->type == PRECOLORED)
	  debug_msg (0, " (precolored, color=%d)", web->color);
	debug_msg (0, "\n");
      }
  debug_msg (0, "\nColored:\n");
  for (d = colored_nodes; d; d = d->next)
    {
      web = DLIST_WEB (d);
      debug_msg (0, "  %4d : color %d\n", web->id, web->color);
    }
  debug_msg (0, "\nCoalesced:\n");
  for (d = coalesced_nodes; d; d = d->next)
    {
      web = DLIST_WEB (d);
      debug_msg (0, "  %4d : to web %d, color %d\n", web->id,
		 alias (web)->id, web->color);
    }
  debug_msg (0, "\nSpilled:\n");
  for (d = spilled_nodes; d; d = d->next)
    {
      web = DLIST_WEB (d);
      debug_msg (0, "  %4d\n", web->id);
    }
  debug_msg (0, "\n");
}

static void
init_ra (void)
{
  int i;
  HARD_REG_SET rs;
  /* FIXME: Choose spill heuristic for platform if we have one */
  spill_heuristic = default_spill_heuristic;

  CLEAR_HARD_REG_SET (never_use_colors);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (fixed_regs[i]
	|| i == HARD_FRAME_POINTER_REGNUM
	|| i == STACK_POINTER_REGNUM
	|| i == FRAME_POINTER_REGNUM || i == ARG_POINTER_REGNUM)
      SET_HARD_REG_BIT (never_use_colors, i);

  for (i = 0; i < N_REG_CLASSES; i++)
    {
      int size = 0, j;
      COPY_HARD_REG_SET (rs, reg_class_contents[i]);
      AND_COMPL_HARD_REG_SET (rs, never_use_colors);
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (rs, j))
	  size++;
      num_free_regs[i] = size;
      COPY_HARD_REG_SET (usable_regs[i], rs);
    }

  orig_max_uid = get_max_uid ();
  compute_bb_for_insn (get_max_uid ());
}

void
mm_reg_alloc (_file)
     FILE *_file;
{
  struct df *df;
  int changed;
  rtx insn;
  int pass = 0;
  file = _file;
  init_ra ();
  no_new_pseudos = 0;
  /* We don't use those NOTEs, and as we anyway change all registers,
     they only make problems later.  */
  count_or_remove_death_notes (NULL, 1);
  do
    {
      debug_msg (0, "RegAlloc Pass %d\n\n", pass);
      if (pass++ > 40)
	fatal ("Didn't find a coloring.\n");
      df = df_init ();
      df_analyse (df, 0, DF_HARD_REGS | DF_RD_CHAIN | DF_RU_CHAIN);
      if (file)
	{
	  df_dump (df, DF_HARD_REGS, rtl_dump_file);
	  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
	    if (INSN_P (insn))
	      {
		//  df_insn_debug_regno (df, insn, file);
	      }
	}
      alloc_mem (df);
      changed = one_pass (df);
      if (!changed)
	emit_colors (df);
      dump_ra (df);
      if (changed && rtl_dump_file)
	print_rtl_with_bb (rtl_dump_file, get_insns ());
      free_all_lists ();
      free_mem (df);
      df_finish (df);
    }
  while (changed);
  if (rtl_dump_file != NULL)
    print_rtl_with_bb (rtl_dump_file, get_insns ());

  no_new_pseudos = 1;
  compute_bb_for_insn (get_max_uid ());
  /*count_or_remove_death_notes (NULL, 1);
     allocate_reg_life_data ();
     update_life_info (NULL, UPDATE_LIFE_GLOBAL, PROP_REG_INFO
     | PROP_DEATH_NOTES | PROP_SCAN_DEAD_CODE
     | PROP_KILL_DEAD_CODE); */
  find_basic_blocks (get_insns (), max_reg_num (), rtl_dump_file);
  life_analysis (get_insns (), rtl_dump_file, PROP_FINAL);
  file = 0;
}

/*
vim:cinoptions={.5s,g0,p5,t0,(0,^-0.5s:tw=78:cindent:
*/
