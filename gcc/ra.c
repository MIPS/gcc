/* Graph coloring register allocator
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Michael Matz <matzmich@cs.tu-berlin.de>
   and Daniel Berlin <dberlin@redhat.com>

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software
   Foundation; either version 2, or (at your option) any later version.

   GNU CC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License along
   with GNU CC; see the file COPYING.  If not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#define OLD_DF_INTERFACE
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
#include "toplev.h"

/* The algorithm used is currently Iterated Register Coalescing by
   L.A.George, and Appel.
*/

/* TODO
   * do lots of commenting
   * look through all XXX's and do something about them
   * handle REG_NO_CONLICTS blocks correctly (the current ad hoc approach
     might miss some conflicts due to insns which only seem to be in a 
     REG_NO_CONLICTS block)
   * we really _need_ to handle SUBREGs as only taking one hardreg.
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
   * don't insert hard-regs, but a limited set of pseudo-reg
     in emit_colors, and setup reg_renumber accordingly (done, but this
     needs reload, which I want to go away)
   * when spilling, update interference graph incrementally, which
     is possible, as we don't use global liveness
   * don't destroy coalescing information completely when spilling
   * use the constraints from asms
   * correctly handle SUBREG as being one hardreg on it's
     own, to handle such things:
     (set (subreg:SI (reg:DI 40) 0) (...))
     (set (reg:SI 41) (...)) 
     where it's clear from constraints, that 40 should go to
     0 and 41 to 1.  For now they conflict for the code below, although
     they don't in reality.
   * implement spill coalescing/propagation
   * implement optimistic coalescing
  */

/* Double linked list.  */
struct dlist
{
  struct dlist *prev;
  struct dlist *next;
  union
    {
      struct web *web;
      struct move *move;
    } value;
};
/* Simple helper macros for ease of misuse.  */
#define DLIST_WEB(l) ((l)->value.web)
#define DLIST_MOVE(l) ((l)->value.move)

/* Classification of a given node (I.E. What state it's in).  */
enum node_type
{
  INITIAL,
  PRECOLORED, SIMPLIFY, FREEZE, SPILL, SPILLED, COALESCED, COLORED, SELECT,
  LAST_NODE_TYPE
};

/* Such a structure is allocated initially for each def and use.
   In the process of building the interference graph web parts are
   connected together, if they have common instructions and reference the
   same register.  That way live ranges are build (by connecting defs and
   uses) and implicitely complete webs (by connecting web parts in common
   uses).  */
struct web_part
{
  /* The def or use for this web part.  */
  struct ref *ref;
  /* The uplink implementing the disjoint set.  */
  struct web_part *uplink;

  /* Here dynamic information associated with each def/use is saved.
     This all is only valid for root web parts (uplink==NULL).
     That's the information we need to merge, if web parts are unioned.  */
  /* Number of spanned insns.  It's 0 for defs.  */
  unsigned int spanned_insns;
  /* The IDs of conflicting root's of other web(part)s.  Only valid,
     if !uplink (part is root).  */
  bitmap conflicts;
};

/* Web structure used to store info about connected live ranges.  */
struct web
{
  unsigned int id; /* Unique web ID */
  unsigned int regno; /* Register number of the live range's variable */
  unsigned int weight; /* Weight used to determine, among other
                          things, how many colors we can block */
  unsigned int span_insns; /* How many insn's the live range spans */
  unsigned int spill_temp; /* Temporarily used to remember what webs
                              were spilled */
  unsigned int use_my_regs; /* Determine if we should use the
                               usable_regs for this web */
  unsigned int spill_cost; /* Cost of spilling */
  int was_spilled;
  /* We need to distinguish also webs which are targets of coalescing
     (all x with some y, so that x==alias(y)), but the alias field is
     only set for sources of coalescing.  This flag is set for all webs
     involved in coalescing in some way.  */
  int is_coalesced;
  int color; /* Color of the web */
  int add_hardregs; /* Additional hard registers needed to be
                       allocated to the web */
  int num_conflicts;  /* Number of conflicts currently */
  int num_uses; /* Number of uses this web spans */
  int num_defs; /* Number of defs this web spans. */
  int move_related; /* Whether the web is move related (IE involved
                       in a move) */
  enum node_type type; /* Current state of the node */
  rtx orig_x; /* The (reg:M a) or (subreg:M1 (reg:M2 a) x) rtx which this
		 web is based on.  This is used to distinguish subreg webs
		 from it's reg parents, and to get hold of the mode.  */
  struct web *subreg_next; /* If this web is a subreg, but not the last one of
			      another reg, then this points to the next
			      subreg.  If this is a reg, whose subreg's were
			      accessed, this points to the first subreg web.
			      If this is the last subreg web, this points up
			      to the containing reg web.  Ergo, this all forms
			      a circular list, so beware when traversing.  The
			      "first" list-elem is the one with
			      GET_CODE(orig_x) == REG.  For webs without
			      subregs subreg_next of course is 0.  */

  enum reg_class regclass; /* just used for debugging */
  /* might be too much to store a HARD_REG_SET here for machines with _many_
     registers.  Shouldn't hurt for now.  */
  HARD_REG_SET usable_regs;
  int num_freedom; /* Number of registers we could place this in */
  rtx reg_rtx; /* RTX for this register */
  rtx stack_slot; /* Stack slot for this register */
  struct ref **defs; /* [0..num_defs-1] */
  struct ref **uses; /* [0..num_uses-1] */
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
  WORKLIST, MV_COALESCED, CONSTRAINED, FROZEN, ACTIVE,
  LAST_MOVE_TYPE
};

/* Structure of a move we are considering coalescing.  */
struct move
{
  rtx insn;
  struct web *source_web;
  struct web *target_web;
  enum move_type type;
  struct dlist *dlink;
};

/* List of moves.  */
struct move_list
{
  struct move_list *next;
  struct move *move;
};

struct curr_use;

static struct web_part * find_web_part_1 PARAMS ((struct web_part *));
static struct web_part * union_web_part_roots
				PARAMS ((struct web_part *, struct web_part *));
static int regs_overlap_p PARAMS ((rtx, struct curr_use *));
static int live_out_1 PARAMS ((struct df *, struct curr_use *, rtx));
static int live_out PARAMS ((struct df *, struct curr_use *, rtx));
static void live_in PARAMS ((struct df *, struct curr_use *, rtx));
static void build_i_graph PARAMS ((struct df*));
static void debug_msg PARAMS ((int, const char *, ...)) ATTRIBUTE_PRINTF_2;
static int copy_insn_p PARAMS ((rtx, rtx *, rtx *));
static int hard_regs_intersect_p PARAMS ((HARD_REG_SET *, HARD_REG_SET *));
static void remember_move PARAMS ((rtx));
static void handle_asm_insn PARAMS ((struct df *, rtx));
static void prune_hardregs_for_mode PARAMS ((HARD_REG_SET *, enum machine_mode));
static void init_one_web PARAMS ((struct web *, rtx));
static struct web * find_subweb PARAMS ((struct web *, rtx));
static struct web * add_subweb PARAMS ((struct web *, rtx));
static void init_web_parts PARAMS ((struct df *));
static void record_conflict PARAMS ((struct web *, struct web *));
static void parts_to_webs PARAMS ((struct df *, struct web **));
static void conflicts_between_webs PARAMS ((struct df *, struct web **));
static void remember_web_was_spilled PARAMS ((struct web *));
static void detect_spill_temps PARAMS ((void));
static void make_webs PARAMS ((struct df *));
static void moves_to_webs PARAMS ((struct df *));
static void connect_rmw_web_parts PARAMS ((struct df *));
static void set_undefined PARAMS ((struct curr_use *));
static void build_web_parts_and_conflicts PARAMS ((struct df *));
static void dump_igraph PARAMS ((struct df *));
static void alloc_mem PARAMS ((struct df *));
static void free_mem PARAMS ((struct df *));
static void push_list PARAMS ((struct dlist *, struct dlist **));
static void remove_list PARAMS ((struct dlist *, struct dlist **));
static struct dlist * pop_list PARAMS ((struct dlist **));
static void free_dlist PARAMS ((struct dlist **));
static void free_all_lists PARAMS ((void));
static void put_web PARAMS ((struct web *, enum node_type));
static void put_move PARAMS ((struct move *, enum move_type));
static void build_worklists PARAMS ((struct df *));
static void enable_move PARAMS ((struct web *));
static void decrement_degree PARAMS ((struct web *, int));
static void simplify PARAMS ((void));
static void remove_move_1 PARAMS ((struct web *, struct move *));
static void remove_move PARAMS ((struct web *, struct move *));
static void merge_moves PARAMS ((struct web *, struct web *));
static void add_worklist PARAMS ((struct web *));
static int ok PARAMS ((struct web *, struct web *));
static int conservative PARAMS ((struct web *, struct web *));
static struct web * alias PARAMS ((struct web *));
static void combine PARAMS ((struct web *, struct web *));
static void coalesce PARAMS ((void));
static void freeze_moves PARAMS ((struct web *));
static void freeze PARAMS ((void));
static void select_spill PARAMS ((void));
static int get_free_reg PARAMS ((HARD_REG_SET, enum machine_mode));
static int count_long_blocks PARAMS ((HARD_REG_SET, int));
static void colorize_one_web PARAMS ((struct web *));
static void assign_colors PARAMS ((void));
static void allocate_spill_web PARAMS ((struct web *));
static void rewrite_program PARAMS ((struct df *));
static void emit_colors PARAMS ((struct df *));
static int one_pass PARAMS ((struct df *));
static void dump_ra PARAMS ((struct df *));
static void init_ra PARAMS ((void));
void reg_alloc PARAMS ((void));

/* XXX use Daniels compressed bitmaps here.  */
#define igraph_index(i, j) ((i) < (j) ? (j)*((j)-1)/2+(i) : (i)*((i)-1)/2+(j))
static sbitmap igraph;

/* XXX use Briggs sparse bitset, or eliminate visited alltogether (by 
   marking only block ends; this would work, as we also use
   visit_trace[] for a similar thing.  */
static unsigned int visited_pass;
static unsigned int *visited;
static sbitmap move_handled;

static struct web_part *web_parts;
static struct web_part **visit_trace;  /* Indexed by UID.  */
static unsigned int num_webs;
static struct web **id2web; /* Indexed by web id (0..num_webs-1).  */
static struct web **def2web;
static struct web **use2web;
static struct web_link **conflicts; /* XXX move that to struct web.  */
static struct move_list *wl_moves;
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

/* Print a message to the dump file, if debug_new_regalloc is the
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

struct copy_p_cache
{
  int seen;
  rtx source;
  rtx target;
};

/* On demand cache, for if insns are copy insns, and if yes, what
   source/target they have.  */
static struct copy_p_cache * copy_cache;
static int *number_seen;

static int
copy_insn_p (insn, source, target)
     rtx insn;
     rtx *source;
     rtx *target;
{
  rtx d, s;
  int subreg_seen = 0;
  int uid = INSN_UID (insn);

  if (copy_cache[uid].seen)
    {
      if (copy_cache[uid].seen == 1)
	{
	  if (source)
	    *source = copy_cache[uid].source;
	  if (target)
	    *target = copy_cache[uid].target;
	  return 1;
	}
      return 0;
    }

  /* Mark it as seen, but not being a copy insn.  */
  copy_cache[uid].seen = 2;
  insn = single_set (insn);
  if (!insn)
    return 0;
  d = SET_DEST (insn);
  s = SET_SRC (insn);
  while (GET_CODE (d) == STRICT_LOW_PART
	 || GET_CODE (d) == SUBREG)
    {
      if (GET_CODE (d) == SUBREG)
	subreg_seen = 1;
      d = XEXP (d, 0);
    }
  if (GET_CODE (d) != REG)
    return 0;
  while (GET_CODE (s) == STRICT_LOW_PART
	 || GET_CODE (s) == ZERO_EXTRACT
	 || GET_CODE (s) == SIGN_EXTRACT)
    s = XEXP (s, 0);
  /* (set (reg:SI a) (subreg:SI (reg:DI b) 0)) will be a noop
     if a and b become the same register, so this move is a candidate
     for coalescing.
     (set (reg:SI a) (subreg:SI (reg:DI b) 1)) is similar,
     if a becomes hardreg(b)+1, but this isn't really coalesing,
     so for now we ignore such a move.  */
  if (GET_CODE (s) == SUBREG)
    {
      return 0; /* XXX */
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
  if (REGNO (s) < FIRST_PSEUDO_REGISTER
      && REGNO (d) < FIRST_PSEUDO_REGISTER)
    return 0;

  if (source)
    *source = s;
  if (target)
    *target = d;
  copy_cache[uid].seen = 1;
  copy_cache[uid].source = s;
  copy_cache[uid].target = d;
  return 1;
}

/* We build webs, as we process the conflicts.  For each use we go upward
   the insn stream, noting any defs as potentially conflicting with the
   current use.  We stop at defs of the current regno.  The conflicts are only
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

/* Returns the root of the web part P is a member of.  Additionally
   it compresses the path.  P may not be NULL.  */
static struct web_part *
find_web_part_1 (p)
     struct web_part *p;
{
  struct web_part *r = p;
  struct web_part *p_next;
  while (r->uplink)
    r = r->uplink;
  for (; p != r; p = p_next)
    {
      p_next = p->uplink;
      p->uplink = r;
    }
  return r;
}

#define find_web_part(wp) ((! (wp)->uplink) ? (wp) \
  : (! (wp)->uplink->uplink) ? (wp)->uplink : find_web_part_1 (wp))

/* Unions together the parts R1 resp. R2 is a root of.
   All dynamic information associated with the parts (number of spanned insns
   and so on) is also merged.
   The root of the resulting (possibly larger) web part is returned.  */
static struct web_part *
union_web_part_roots (r1, r2)
     struct web_part *r1, *r2;
{
  if (r1 != r2)
    {
      rtx x1, x2;
      /* The new root is the smaller (pointerwise) of both.  This is crucial
         to make the construction of webs from web parts work (so, when
	 scanning all parts, we see the roots before all it's childs).  
         Additionally this ensures, that if the web has a def at all, than
         the root is a def (because all def parts are before use parts in the
	 web_parts[] array), or put another way, as soon, as the root of a
         web part is not a def, this is an uninitialized web part.  The
         way we construct the I-graph ensures, that if a web is initialized,
         then the first part we find (besides trivial 1 item parts) has a
         def.  */
      if (r1 > r2)
	{
	  struct web_part *h = r1;
	  r1 = r2;
	  r2 = h;
	}
      r2->uplink = r1;
      num_webs--;

      /* Now we merge the dynamic information of R1 and R2.  */
      r1->spanned_insns += r2->spanned_insns;

      /* The conflicts can only be merged, if the mode and subreg_word
	 of both web parts are exactly the same.  Otherwise we loose
	 information.  E.g. Think about a DEF originally only conflicting with
	 a (subreg:SI (reg:DI a) 0) which is now going to be merged with it's
	 setter (a (set (reg:DI a))).  If we now simply OR together the
	 conflicts it would look like that DEF would conflict with the whole
	 DImode pseudo.  For the case, that both are REGs, we don't need to
         test the mode, as a pseudo has only one, and we are given also
	 hardregs wordwise.  */
      x1 = DF_REF_REG (r1->ref);
      x2 = DF_REF_REG (r2->ref);
      if (GET_CODE (x1) == GET_CODE (x2)
	  && (GET_CODE (x1) == REG
	      || (GET_CODE (x1) == SUBREG && GET_MODE (x1) == GET_MODE (x2)
		  && SUBREG_WORD (x1) == SUBREG_WORD (x2))))
	{
	  bitmap_operation (r1->conflicts, r1->conflicts, r2->conflicts,
			    BITMAP_IOR);
	  BITMAP_XFREE (r2->conflicts);
	}
      else if (r2->conflicts->first == 0)
	/* No need to hold memory.  */
	BITMAP_XFREE (r2->conflicts);
    }
  return r1;
}

/* Convenience macro, that is cabable of unioning also non-roots.  */
#define union_web_parts(p1, p2) \
  ((p1 == p2) ? find_web_part (p1) \
      : union_web_part_roots (find_web_part (p1), find_web_part (p2)))

/* Remember that we've handled a given move, so we don't reprocess
   it.  */
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

/* This describes the USE currently looked at in the main-loop in
   build_web_parts_and_conflicts().  */
struct curr_use {
  struct web_part *wp;
  /* This has a 1-bit for each byte in the USE, which is still undefined.  */
  unsigned HOST_WIDE_INT undefined;
  /* For easy access.  */
  unsigned int regno;
  rtx x;
};

/* Returns nonzero iff rtx DEF and USE have bits in common (but see below).
   It is only called with DEF and USE being (reg:M a) or (subreg:M1 (reg:M2 a)
   x) rtx's.  Furthermore if it's a subreg rtx M1 is at least one word wide,
   and a is a multi-word pseudo.  If DEF or USE are hardregs, they are in
   wordmode, so we don't need to check for further hardregs which would result
   from wider references.  We are never called with paradoxical subregs.
 
   This returns:
   0 for no common bits,
   1 if DEF and USE exactly cover the same bytes,
   2 if the DEF only covers a part of the bits of USE
   3 if the DEF covers more than the bits of the USE, and
   4 if both are SUBREG's of different size, but have bytes in common.
   -1 is a special case, for when DEF and USE refer to the same regno, but
      have for other reasons no bits in common (can only happen with
      subregs refering to different words, or to words which already were
      defined for this USE).
   Furthermore it modifies use->undefined to clear the bits which get defined
   by DEF (only for cases with partial overlap).
   I.e. if bit 1 is set for the result != -1, the USE was completely covered,
   otherwise a test is needed to track the already defined bytes.  */
static int
regs_overlap_p (def, use)
     rtx def;
     struct curr_use *use;
{
  int mode = 0;
  if (def == use->x)
    return 1;
  if (!def)
    return 0;
  if (GET_CODE (def) == SUBREG)
    mode |= 1;
  if (GET_CODE (use->x) == SUBREG)
    mode |= 2;
  switch (mode)
    {
      case 0: /* REG, REG */
	return (REGNO (def) == use->regno) ? 1 : 0;
      case 1: /* SUBREG, REG */
	if (REGNO (SUBREG_REG (def)) == use->regno)
	  {
	    int b, e;
	    unsigned HOST_WIDE_INT old_u = use->undefined;
	    b = GET_MODE_SIZE (GET_MODE (def)) * SUBREG_WORD (def);
	    e = b + GET_MODE_SIZE (GET_MODE (def));
	    for (; b != e; b++)
	      use->undefined &= ~((unsigned HOST_WIDE_INT)1 << b);
	    return (old_u != use->undefined) ? 2 : -1;
	  }
	else
	  return 0;
      case 2: /* REG, SUBREG */
	return (REGNO (def) == use->regno) ? 3 : 0;
      case 3: /* SUBREG, SUBREG */
	if (REGNO (SUBREG_REG (def)) != use->regno)
	  return 0;
	if (GET_MODE_SIZE (GET_MODE (def)) == GET_MODE_SIZE (GET_MODE (use->x)))
	  /* If the size of both things is the same, the subreg's only overlap
	     if they refer to the same word.  */
	  return (SUBREG_WORD (def) == SUBREG_WORD (use->x)) ? 1 : -1;
	/* Now the more difficult part: the same regno is refered, but the
	   sizes of the references differ.  E.g.
           (subreg:SI (reg:CDI a) 0) and (subreg:DI (reg:CDI a) 1) do not
	   overlap, wereas the latter overlaps with (subreg:SI (reg:CDI a) 2).
	   */
	{
	  unsigned HOST_WIDE_INT old_u;
	  int b1, e1, b2, e2;
	  b1 = GET_MODE_SIZE (GET_MODE (def)) * SUBREG_WORD (def);
	  b2 = GET_MODE_SIZE (GET_MODE (use->x)) * SUBREG_WORD (use->x);
	  e1 = b1 + GET_MODE_SIZE (GET_MODE (def)) - 1;
	  e2 = b2 + GET_MODE_SIZE (GET_MODE (use->x)) - 1;
	  if (b1 > e2 || b2 > e1)
	    return -1;
	  old_u = use->undefined;
	  for (; b1 <= e1; b1++)
	    use->undefined &= ~((unsigned HOST_WIDE_INT)1 << b1);
	  return (old_u != use->undefined) ? 4 : -1;
	}
      default:
        abort ();
    }
}

/* XXX gross hack to detect REG_NO_CONFLICT blocks, so we
   can ignore the conflict which would result from the initial clobber
   starting such a block.  */
static int in_no_conflict_block;

static int
live_out_1 (df, use, insn)
     struct df *df;
     struct curr_use *use;
     rtx insn;
{
  int defined = 0;
  unsigned int uid = INSN_UID (insn);
  struct web_part *wp = use->wp;

  /* Mark, that this insn needs this webpart live.  */
  visit_trace[uid] = wp;

  if (INSN_P (insn))
    {
      struct df_link *link;
      unsigned int source_regno = ~0;
      unsigned int regno = use->regno;
      rtx s,t;
      wp = find_web_part (wp);
      wp->spanned_insns++;
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
	  source_regno = REGNO (s);
	  remember_move (insn);
	}
      for (link = DF_INSN_DEFS (df, insn); link; link = link->next)
        if (link->ref)
	  {
	    int lap;
	    if ((lap = regs_overlap_p (DF_REF_REG (link->ref), use)) != 0)
	      {
		if (lap == -1)
		  /* Same regnos but non-overlapping or already defined bits,
		     so ignore this DEF.  */
		  continue;
		if ((lap & 1) != 0)
		  /* The current DEF completely covers the USE, so we can
		     stop traversing the code looking for further DEFs.  */
	          defined = 1;
		else
		  /* We have a partial overlap.  */
		  {
		    if (use->undefined == 0)
		      /* Now the USE is completely defined, which means, that
			 we can stop looking for former DEFs.  */
		      defined = 1;
		  }
		/* This is at least a partial overlap, so we need to union
		   the web parts.  */
		wp = union_web_parts (wp, &web_parts[DF_REF_ID (link->ref)]);
	      }
	    else if (regno != source_regno)
	      /* This triggers, when either this was no copy insn
		 (source_regno being ~0 then, which is != regno), or if it
		 was, but the source wasn't the current reg.  */
	      {
		struct web_part *cwp;

		if (find_regno_note (insn, REG_NO_CONFLICT, regno))
		  {
		    /* The current use don't conflict with the DEF in this
		       insn.  */
		    in_no_conflict_block = 1;
		    continue;
		  }
		/* We also don't conflict with the CLOBBER starting a
		   REG_NO_CONFLICT block.  */
		if (in_no_conflict_block
		    && GET_CODE (PATTERN (insn)) == CLOBBER
		    && find_reg_note (insn, REG_LIBCALL, NULL_RTX) != 0)
		  continue;

		/* XXX Beware, for SUBREG tracking we can't use the IDs of the
		   webroots unconditionally to identify conflicts.  We need a
		   webroot for each mode/subregword pair (at least
		   conceptionally) in addition to the real root for the reg
		   itself.  */
		cwp = find_web_part (&web_parts[DF_REF_ID (link->ref)]);
		/* Use the un-unioned web_part for remembering conflicts.  */
		bitmap_set_bit (/*use->*/wp->conflicts, DF_REF_ID (cwp->ref));
	      }
	  }
    }

  return !defined;
}

static inline int
live_out (df, use, insn)
     struct df *df;
     struct curr_use *use;
     rtx insn;
{
  unsigned int uid = INSN_UID (insn);
  if (visit_trace[uid] && DF_REF_REGNO (visit_trace[uid]->ref) == use->regno)
    {
      (void) union_web_parts (visit_trace[uid], use->wp);
      /* Don't search any further, as we already were here with this regno. */
      return 0;
    }
  else
    return live_out_1 (df, use, insn);
}

static void
live_in (df, use, insn)
     struct df *df;
     struct curr_use *use;
     rtx insn;
{
  unsigned int loc_vpass = visited_pass;
  unsigned int *loc_v = visited;

  /* Note, that, even _if_ we are called with use->wp a root-part, this might
     become non-root in the for() loop below (due to live_out() unioning
     it).  So beware, not to change use->wp in a way, for which only root-webs
     are allowed.  */
  while (1)
    {
      int uid = INSN_UID (insn);
      rtx p;
      if (loc_v[uid] == loc_vpass)
	return;
      loc_v[uid] = loc_vpass;
      number_seen[uid]++;

      p = prev_real_insn (insn);
      if (!p)
	return;
      if (BLOCK_FOR_INSN (insn) != BLOCK_FOR_INSN (p))
	{
	  edge e;
	  /* All but the last predecessor are handled recursively.  */
	  for (e = BLOCK_FOR_INSN (insn)->pred; e && e->pred_next;
	       e = e->pred_next)
	    if (live_out (df, use, e->src->end))
	      live_in (df, use, e->src->end);
	  if (!e)
	    return;
	  p = e->src->end;
	}
      if (live_out (df, use, p))
	insn = p;
      else
	return;
    }
}

static void
dump_number_seen (void)
{
  int num[17];
  int i;

  for (i = 0; i < 17; i++)
    num[i] = 0;
  for (i = 0; i < get_max_uid (); i++)
    if (number_seen[i] < 16)
      num[number_seen[i]]++;
    else
      num[16]++;
  for (i = 0; i < 16; i++)
    if (num[i])
      debug_msg (0, "%d insns seen %d times\n", num[i], i);
  if (num[16])
    debug_msg (0, "%d insn seen %d and more times\n", num[i], 16);
  debug_msg (0, "from overall %d insns\n", get_max_uid ());
}

static void
set_undefined (use)
     struct curr_use *use;
{
  int b = 0, e;
  use->undefined = (unsigned HOST_WIDE_INT) 0;
  if (GET_CODE (use->x) == SUBREG)
    b = GET_MODE_SIZE (GET_MODE (use->x)) * SUBREG_WORD (use->x);
  e = b + GET_MODE_SIZE (GET_MODE (use->x));
  for (; b < e; b++)
    use->undefined |= ((unsigned HOST_WIDE_INT)1 << b);
}

/* Connect web parts, thereby implicitely building webs, and
   their conflicts.  */
static void
build_web_parts_and_conflicts (df)
     struct df *df;
{
  struct df_link *link;
  struct curr_use use;

  /* Setup copy cache, for copy_insn_p ().  */
  copy_cache = (struct copy_p_cache *)
    xcalloc (get_max_uid (), sizeof (copy_cache[0]));
  number_seen = (int *) xcalloc (get_max_uid (), sizeof (int));
  visit_trace = (struct web_part **) xcalloc (get_max_uid (),
					      sizeof (visit_trace[0]));
  visited = (unsigned int *) xcalloc (get_max_uid (), sizeof (unsigned int));

  /* Here's the main loop.
     It goes through all insn's, connects web parts along the way, notes
     conflicts between webparts, and remembers move instructions.  */
  visited_pass = 0;
  for (use.regno = 0; use.regno < (unsigned int)max_regno; use.regno++)
    for (link = df->regs[use.regno].uses; link; link = link->next)
      if (link->ref)
	{
	  struct ref *ref = link->ref;
	  rtx insn = DF_REF_INSN (ref);
	  use.wp = &web_parts[df->def_id + DF_REF_ID (ref)];
	  use.x = DF_REF_REG (ref);
	  set_undefined (&use);
	  visited_pass++;
	  in_no_conflict_block = 0;
	  live_in (df, &use, insn);
	  if (copy_insn_p (insn, NULL, NULL))
	    remember_move (insn);
	}

  dump_number_seen ();
  free (visited);
  free (visit_trace);
  free (number_seen);
  free (copy_cache);
  /* Catch prohibited uses of copy_insn_p () early.  */
  copy_cache = NULL;
  visited = NULL;
}

/* Handle tricky asm insns.  */
static void
handle_asm_insn (df, insn)
     struct df *df;
     rtx insn;
{
  const char *constraints[MAX_RECOG_OPERANDS];
  enum machine_mode operand_mode[MAX_RECOG_OPERANDS];
  int i, noperands, in_output;
  HARD_REG_SET clobbered, allowed, conflict;
  rtx pat;
  if (! INSN_P (insn)
      || (noperands = asm_noperands (PATTERN (insn))) < 0)
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
	  while (link && link->ref && DF_REF_REG (link->ref) != reg)
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
	web = def2web[DF_REF_ID (link->ref)];
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
		do {
		    c = *p++;
		} while (c != '\0' && c != ',');
	      if (c == '\0')
	        break;
	      continue;
	    }

	  switch (c)
	    {
	      case '=': case '+': case '*': case '%': case '?': case '!':
	      case '0': case '1': case '2': case '3': case '4': case 'm':
	      case '<': case '>': case 'V': case 'o': case '&': case 'E':
	      case 'F': case 's': case 'i': case 'n': case 'X': case 'I':
	      case 'J': case 'K': case 'L': case 'M': case 'N': case 'O':
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
      if (rtl_dump_file)
	{
	  int c;
	  debug_msg (0, " ASM constrain Web %d conflicts with:", web->id);
	  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
	    if (TEST_HARD_REG_BIT (conflict, c))
	      debug_msg (0, " %d", c);
	  debug_msg (0, "\n");
	}
    }
}

/* Deletes all hardregs from *S which are not allowed for MODE.  */
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

/* Initialize a single web.  */
static void
init_one_web (web, reg)
     struct web *web;
     rtx reg;
{
  if (GET_CODE (reg) != REG)
    abort ();
  /* web->id isn't initialized here.  */
  web->regno = REGNO (reg);
  web->weight = 0;
  web->span_insns = 0;
  web->spill_temp = 0;
  web->use_my_regs = 0;
  web->spill_cost = reg_spill_cost (web->regno);
  web->was_spilled = -1;
  web->is_coalesced = 0;
  web->num_defs = 0;
  web->num_uses = 0;
  web->orig_x = reg;
  web->subreg_next = web;
  /* XXX
     the former (superunion) doesn't constrain the graph enough. E.g.
     on x86 QImode _requires_ QI_REGS, but as alternate class usually
     GENERAL_REGS is given.  So the graph is not constrained enough,
     thinking it has more freedom then it really has, which leads
     to repeated spill tryings.  OTOH the latter (only using preferred
     class) is too constrained, as normally (e.g. with all SImode
     pseudos), they can be allocated also in the alternate class.
     What we really want, are the _exact_ hard regs allowed, not
     just a class.  Later.  */
  /*web->regclass = reg_class_superunion
		    [reg_preferred_class (web->regno)]
		    [reg_alternate_class (web->regno)];*/
  /*web->regclass = reg_preferred_class (web->regno);*/
  web->regclass = reg_class_subunion
    [reg_preferred_class (web->regno)] [reg_alternate_class (web->regno)];
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
	 using a DFmode pseudo on x86 can result in class FLOAT_INT_REGS,
	 where, if it finally is allocated to GENERAL_REGS it needs two,
	 if allocated to FLOAT_REGS only one hardreg.  XXX */
      web->add_hardregs =
	CLASS_MAX_NREGS (web->regclass, PSEUDO_REGNO_MODE (web->regno)) - 1;
      web->num_conflicts = web->add_hardregs;
      COPY_HARD_REG_SET (web->usable_regs,
			reg_class_contents[reg_preferred_class (web->regno)]);
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
  web->alias = NULL;
  web->moves = NULL;
  web->dlink = NULL;
}

static struct web *
find_subweb (web, reg)
     struct web *web;
     rtx reg;
{
  struct web *w;
  if (GET_CODE (reg) != SUBREG)
    abort ();
  for (w = web->subreg_next; GET_CODE (w->orig_x) != REG; w = w->subreg_next)
    if (GET_MODE (w->orig_x) == GET_MODE (reg)
	&& SUBREG_WORD (w->orig_x) == SUBREG_WORD (reg))
      return w;
  return NULL;
}

static struct web *
add_subweb (web, reg)
     struct web *web;
     rtx reg;
{
  struct web *w;
  if (GET_CODE (reg) != SUBREG)
    abort ();
  w = (struct web *) xmalloc (sizeof (struct web));
  /* Copy most content from parent-web.  */
  *w = *web;
  w->orig_x = reg;
  w->add_hardregs = CLASS_MAX_NREGS (web->regclass, GET_MODE (reg)) - 1;
  w->num_conflicts = w->add_hardregs;
  w->num_defs = 0;
  w->num_uses = 0;
  w->subreg_next = web->subreg_next;
  web->subreg_next = w;
  return w;
}

/* Initialize all the web parts we are going to need.  */
static void
init_web_parts (df)
     struct df *df;
{
  int regno;
  unsigned int no;
  for (no = 0; no < df->def_id; no++)
    {
      web_parts[no].ref = df->defs[no];
      web_parts[no].conflicts = BITMAP_XMALLOC ();
    }
  for (no = 0; no < df->use_id; no++)
    {
      web_parts[no + df->def_id].ref = df->uses[no];
      web_parts[no + df->def_id].conflicts = BITMAP_XMALLOC ();
    }
  num_webs = df->def_id + df->use_id;

  /* We want to have only one web for each precolored register.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    {
      struct web_part *r1;
      struct df_link *link;
      for (link = df->regs[regno].defs; link && !link->ref; link =
	   link->next) ;
      if (!link)
	continue;
      r1 = &web_parts[DF_REF_ID (link->ref)];
      /* Link together all defs...  */
      for (link = link->next; link; link = link->next)
        if (link->ref)
	  {
	    struct web_part *r2 = &web_parts[DF_REF_ID (link->ref)];
	    r1 = union_web_parts (r1, r2);
	  }
      /* ... and all uses.  */
      for (link = df->regs[regno].uses; link; link = link->next)
	if (link->ref)
	  {
	    struct web_part *r2 = &web_parts[df->def_id
		                             + DF_REF_ID (link->ref)];
	    r1 = union_web_parts (r1, r2);
	  }
    }
}

/* Determine if two hard register sets intersect.
   Return 1 if they do.  */
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

/* Record a conflict between two webs, if we haven't recorded it
   already.  */
static void
record_conflict (web1, web2)
     struct web *web1, *web2;
{
  struct web_link *wl;
  unsigned int id1 = web1->id, id2 = web2->id;
  unsigned int index = igraph_index (id1, id2);
  /* trivial non-conflict */
  if (web1 == web2 || TEST_BIT (igraph, index))
    return;
  /* As fixed_regs are no targets for allocation, conflicts with them
     are pointless.  */
  if ((web1->regno < FIRST_PSEUDO_REGISTER && fixed_regs[web1->regno])
      || (web2->regno < FIRST_PSEUDO_REGISTER && fixed_regs[web2->regno]))
    return;
  /* Conflicts between two hardregs, which are not even a candidate
     for this pseudo are also pointless.  */
  if (! ((web1->type == PRECOLORED 
	  && TEST_HARD_REG_BIT (web2->usable_regs, web1->regno))
	 || (web2->type == PRECOLORED
	     && TEST_HARD_REG_BIT (web1->usable_regs, web2->regno))
	 || hard_regs_intersect_p (&web1->usable_regs, &web2->usable_regs)))
    return;
  SET_BIT (igraph, index);
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

/* This builds full webs out of web parts, without relating them to each
   other.  */
static void
parts_to_webs (df, part2web)
     struct df *df;
     struct web **part2web;
{
  unsigned int i;
  unsigned int webnum;
  struct ref **ref_use, **ref_def;
  struct web_part *wp_first_use = &web_parts[df->def_id];
  struct web_link *all_subwebs = NULL;
  struct web_link *wl;
  unsigned int num_subwebs = 0;

  /* For each root web part: create and initialize a new web,
     setup def2web[] and use2web[] for all defs and uses, and
     id2web for all new webs.  */
  id2web = (struct web **) xcalloc (num_webs, sizeof (struct web *));

  webnum = 0;
  for (i = 0; i < df->def_id + df->use_id; i++)
    {
      struct web *web;
      struct web_part *wp = &web_parts[i];
      struct ref *ref = wp->ref;
      struct web_part *rwp = find_web_part (wp);
      rtx reg = DF_REF_REG (ref);
      if (! wp->uplink)
	{
	  web = (struct web *) xcalloc (1, sizeof (struct web));
	  init_one_web (web, GET_CODE (reg) == SUBREG ? SUBREG_REG (reg) : reg);
	  web->id = webnum;
	  web->span_insns = wp->spanned_insns;
	  part2web[i] = web;
	  id2web[webnum] = web;
	  webnum++;
	}
      else
	{
	  int j = DF_REF_ID (rwp->ref);
	  if (rwp < wp_first_use)
	    web = part2web[j];
	  else
	    web = part2web[j + df->def_id];
	}
      if (GET_CODE (reg) == SUBREG && !find_subweb (web, reg))
	{
	  struct web *subweb = add_subweb (web, reg);
	  struct web_link *wl;
	  subweb->id = num_webs + num_subwebs;
	  wl = (struct web_link *) xmalloc (sizeof (struct web_link));
	  wl->web = subweb;
	  wl->next = all_subwebs;
	  num_subwebs++;
	}
      /* XXX make sure, web->weight doesn't wrap.  */
      if (DF_REF_BB (ref)->loop_depth <= 8)
        web->weight += 1 << (3 * DF_REF_BB (ref)->loop_depth);
      else
	web->weight += 1 << (3 * 8);
      if (i < df->def_id)
	{
	  web->num_defs++;
	  def2web[i] = web;
	}
      else
	{
	  web->num_uses++;
	  use2web[i - df->def_id] = web;
	}
    }
  if (webnum != num_webs)
    abort ();

  id2web = (struct web **) xrealloc (id2web, (num_webs + num_subwebs) *
				     sizeof (struct web *));
  for (wl = all_subwebs; wl;)
    {
      id2web[wl->web->id] = wl->web;
      wl = wl->next;
      free (all_subwebs);
      all_subwebs = wl;
    }
  num_webs += num_subwebs;
  conflicts = (struct web_link **) xcalloc (num_webs, sizeof (conflicts[0]));
  igraph = sbitmap_alloc (num_webs * num_webs / 2);
  sbitmap_zero (igraph);

  /* Setup and fill uses[] and defs[] arrays of the webs.  */
  ref_def = all_defs_for_web;
  ref_use = all_uses_for_web;
  for (i = 0; i < num_webs; i++)
    {
      id2web[i]->defs = ref_def;
      id2web[i]->uses = ref_use;
      ref_def += id2web[i]->num_defs;
      ref_use += id2web[i]->num_uses;
    }
  for (i = 0; i < df->def_id; i++)
    {
      def2web[i]->defs[0] = df->defs[i];
      def2web[i]->defs++;
    }
  for (i = 0; i < df->use_id; i++)
    {
      use2web[i]->uses[0] = df->uses[i];
      use2web[i]->uses++;
    }
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = id2web[i];
      web->defs -= web->num_defs;
      web->uses -= web->num_uses;
      web->weight *= (1 + web->add_hardregs);
    }
}

/* Convert the conflicts between web parts to conflicts between full webs.  

   This can't be done in parts_to_webs(), because for recording conflicts
   between webs we need to know their final usable_regs set, which is used
   to discard non-conflicts (between webs having no hard reg in common).  
   But this is set for spill temporaries only after the webs itself are
   built.  Until then the usable_regs set is based on the pseudo regno used
   in this web, which may contain far less registers than later determined.  
   This would result in us loosing conflicts (due to record_conflict()
   thinking that a web can only be allocated to the current usable_regs,
   whereas later this is extended) leading to colorings, where some regs which
   in reality conflict get the same color.  
 
   This also deallocates the conflicts bitmaps for all web parts.  */
static void
conflicts_between_webs (df, part2web)
     struct df *df;
     struct web **part2web;
{
  unsigned int i;
  struct web_part *wp_first_use = &web_parts[df->def_id];

  /* Now record all conflicts between webs.  */
  for (i = 0; i < df->def_id + df->use_id; i++)
    if (web_parts[i].conflicts)
      {
	/* Without SUBREGs only web roots have .conflicts != 0, because we
	   were able to merge all conflicts when unioning the parts.  So in
	   that case the find_web_part() wouldn't be necessary, but
	   unfortunately with SUBREGs also non-roots can still have conflicts
	   so we need to test them all.  OTOH we need the root to check, if we
	   aren't dealing with an uninitialized web (which is the case, if the
	   root is itself a USE instead of a DEF), and because part2web[] only
	   is defined for roots (this anyway needs to change XXX).  */
	struct web_part *rwp = find_web_part (&web_parts[i]);
	int j;

	if (rwp < wp_first_use)
	  {
	    struct web *web1 = part2web[DF_REF_ID(rwp->ref)];
	    /* Note, that there are only defs in the conflicts bitset.  */
	    EXECUTE_IF_SET_IN_BITMAP (
	      web_parts[i].conflicts, 0, j,
	      {
		struct web_part *wp;
		struct web *web2;
		wp = find_web_part (&web_parts[j]);
		web2 = part2web[DF_REF_ID (wp->ref)];
		record_conflict (web1, web2);
	      });
	  }
	BITMAP_XFREE (web_parts[i].conflicts);
      }
}

/* Remember that a web was spilled.  */
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
     It's wrong sometimes to have the same constraints or
     preferences as the original pseudo, esp. if they were very narrow.
     (E.g. there once was a reg wanting class AREG (only one register)
     without alternative class.  As long, as also the spill-temps for
     this pseudo had the same constraints it was spilled over and over.
     Ideally we want some constraints also on spill-temps: Because they are
     not only loaded/stored, but also worked with, any constraints from insn
     alternatives needs applying.  Currently this is dealt with by reload, as
     many other things, but sometimes we want to integrate that functionality
     into the allocator.  */
  COPY_HARD_REG_SET (web->usable_regs, reg_class_contents[(int) ALL_REGS]);
  AND_COMPL_HARD_REG_SET (web->usable_regs, never_use_colors);
  prune_hardregs_for_mode (&web->usable_regs, PSEUDO_REGNO_MODE (web->regno));
  web->num_freedom = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (web->usable_regs, i))
      web->num_freedom++;

  /* Now look for a class, which is subset of our constraints, to
     setup add_hardregs, and regclass for debug output.  */
  web->regclass = NO_REGS;
  for (i = (int) ALL_REGS - 1; i > 0; i--)
    {
      unsigned int size;
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
detect_spill_temps (void)
{
  unsigned int no;

  /* Detect webs used for spill temporaries.  */
  for (no = 0; no < num_webs; no++)
    {
      struct web *web = id2web[no];

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
      /*if (web->num_defs == 1 && web->num_uses > 0)*/
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
	      || bigspill
	      || INSN_UID (ui) >= orig_max_uid)
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

/* Converts the connected web parts to full webs.  This means, it allocates
   all webs, and initializes all fields, including detecting spill
   temporaries.  It does not distribute moves to their corresponding webs,
   though.  */
static void
make_webs (df)
     struct df *df;
{
  /* Our mapping from part-IDs to webs for the process of converting them.  */
  struct web **part2web;

  part2web = (struct web **) xcalloc (df->def_id + df->use_id,
				      sizeof (struct web *));
  /* First build all the webs itself.  They are not related with
     others yet.  */
  parts_to_webs (df, part2web);
  /* Now detect spill temporaries to initialize their usable_regs set.  */
  detect_spill_temps ();
  /* And finally relate them to each other, meaning to record all possible
     conflicts between webs (see the comment there).  */
  conflicts_between_webs (df, part2web);

  free (part2web);
}

/* Distribute moves to the corresponding webs.  */
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
      /* Multiple defs/uses can happen in moves involving hard-regs in
	 a wider mode.  For those df.* creates use/def references for each
	 real hard-reg involved.  For coalescing we are interested in
	 the smallest numbered hard-reg.  */
      for (link = DF_INSN_DEFS (df, m->insn); link; link = link->next)
        if (link->ref)
	  {
	    web = def2web[DF_REF_ID (link->ref)];
	    if (!m->target_web || web->regno < m->target_web->regno)
	      m->target_web = web;
	  }
      for (link = DF_INSN_USES (df, m->insn); link; link = link->next)
        if (link->ref)
	  {
	    web = use2web[DF_REF_ID (link->ref)];
	    if (!m->source_web || web->regno < m->source_web->regno)
	      m->source_web = web;
	  }
      if (m->source_web && m->target_web
	  /* If the usable_regs don't intersect we can't coalesce the two
	     webs anyway, as this is no simple copy insn (it might even
	     need an intermediate stack temp to execute this "copy" insn). */
	  && hard_regs_intersect_p (&m->source_web->usable_regs,
				    &m->target_web->usable_regs))
	{
	  struct move_list *test = m->source_web->moves;
	  for (; test && test->move != m; test = test->next);
	  if (! test)
	    {
	      newml = (struct move_list*)
		xmalloc (sizeof (struct move_list));
	      newml->move = m;
	      newml->next = m->source_web->moves;
	      m->source_web->moves = newml;
	    }
	  test = m->target_web->moves;
	  for (; test && test->move != m; test = test->next);
	  if (! test)
	    {
	      newml = (struct move_list*)
		xmalloc (sizeof (struct move_list));
	      newml->move = m;
	      newml->next = m->target_web->moves;
	      m->target_web->moves = newml;
	    }
	}
      else
	{
	  /* Delete this move. At least one of the involved webs was
	     NULL.  XXX Shouldn't happen anymore.  */
	  free (m);
	  ml->move = NULL;
	}
    }
}

/* Here we look per insn, for DF references being in uses _and_ defs.
   This means, in the RTL a (REG xx) expression was seen as a
   read/modify/write, as happens for (set (subreg:SI (reg:DI xx)) (...))
   e.g.  Our code has created two webs for this, as it should.  Unfortunately,
   as the REG reference is only one time in the RTL we can't color
   both webs different (arguably this also would be wrong for a real
   read-mod-write instruction), so we must reconnect such webs.  */
static void
connect_rmw_web_parts (df)
     struct df *df;
{
  unsigned int i;

  for (i = 0; i < df->use_id; i++)
    {
      struct web_part *wp1 = &web_parts[df->def_id + i];
      rtx reg;
      struct df_link *link;

      /* If it's an uninitialized web, we don't want to connect it to others,
	 as the read cycle in read-mod-write had probably no effect.  */
      if (find_web_part (wp1) >= &web_parts[df->def_id])
	continue;
      reg = DF_REF_REG (wp1->ref);
      link = DF_INSN_DEFS (df, DF_REF_INSN (wp1->ref));
      for (; link; link = link->next)
        if (reg == DF_REF_REG (link->ref))
	  {
	    struct web_part *wp2 = &web_parts[DF_REF_ID (link->ref)];
	    union_web_parts (wp1, wp2);
	  }
    }
}

/* Build the interference graph (including all webs).  */
static void
build_i_graph (df)
     struct df *df;
{
  rtx insn;

  init_web_parts (df);

  sbitmap_zero (move_handled);
  wl_moves = NULL;

  build_web_parts_and_conflicts (df);

  /* For read-modify-write instructions we may have created two webs.
     Reconnect them here.  (s.a.)  */
  connect_rmw_web_parts (df);

  /* The webs are conceptually complete now, but still scattered around as
     connected web parts.  Collect all information and build the webs
     including all conflicts between webs (instead web parts).  */
  make_webs (df);
  moves_to_webs (df);

  /* Look for additional constraints given by asms.  */
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    handle_asm_insn (df, insn);
}

/* Dump the interference graph.  */
static void
dump_igraph (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  struct move_list *ml;
  unsigned int def1, def2;
  int num = 0;
  int num2;
  if (!rtl_dump_file || 0 > debug_new_regalloc)
    return;
  debug_msg (0, "conflicts:\n  ");
  for (def1 = 0; def1 < num_webs; def1++)
    {
      int num1 = num;
      for (num2=0, def2 = 0; def2 < num_webs; def2++)
        if (TEST_BIT (igraph, igraph_index (def1, def2)))
	  {
	    if (num1 == num)
	      debug_msg (0, "%d (REG %d) with ", def1, id2web[def1]->regno);
	    if ((num2 % 9) == 8)
	      debug_msg (0, "\n              ");
	    num++;
	    num2++;
	    debug_msg (0, "%d(%d) ", def2, id2web[def2]->regno);
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

/* Allocate the memory necessary for the register allocator.  */
static void
alloc_mem (df)
     struct df *df;
{
  move_handled = sbitmap_alloc (get_max_uid () );
  web_parts = (struct web_part *) xcalloc (df->def_id + df->use_id,
					   sizeof web_parts[0]);
  def2web = (struct web **) xcalloc (df->def_id, sizeof def2web[0]);
  use2web = (struct web **) xcalloc (df->use_id, sizeof use2web[0]);
  all_uses_for_web = (struct ref **) xcalloc (df->use_id,
					     sizeof all_uses_for_web[0]);
  all_defs_for_web = (struct ref **) xcalloc (df->def_id,
					     sizeof all_defs_for_web[0]);
}

/* Free the memory used by the register allocator.  */
static void
free_mem (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  struct web_link *wl, *wl_next;
  struct move_list *ml, *ml_next;
  unsigned int i;
  
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = id2web[i];
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
      free (web);
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
  free (web_parts);
  web_parts = NULL;
  free (move_handled);
  free (igraph);
}


/* Push a node onto the front of the list.  */
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

/* Remove a node from the list.  */
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

/* Pop the front of the list.  */
static struct dlist *
pop_list (list)
     struct dlist **list;
{
  struct dlist *r = *list;
  if (r)
    remove_list (r, list);
  return r;
}

/* Free the given double linked list.  */
static void
free_dlist (list)
     struct dlist **list;
{
  struct dlist *d;
  for (d = pop_list (list); d; d = pop_list (list))
    free (d);
  *list = NULL;
}


/* Here begins the real allocator code.  */

static struct dlist *simplify_wl, *freeze_wl, *spill_wl, *simplify_spilled_wl;
static struct dlist *coalesced_nodes, *colored_nodes, *spilled_nodes;
static struct dlist *select_stack;

static struct dlist *mv_worklist, *mv_coalesced, *mv_constrained;
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
  free_dlist (&mv_coalesced);
  free_dlist (&mv_constrained);
  free_dlist (&mv_frozen);
  free_dlist (&mv_active);
}

/* Inline, because it's called with constant TYPE every time.  */
static inline void
put_web (web, type)
     struct web *web;
     enum node_type type;
{
  switch (type)
    {
      case INITIAL:
      case PRECOLORED:
	abort ();
      case SIMPLIFY:
	if (web->spill_temp)
	  push_list (web->dlink, &simplify_spilled_wl);
	else
	  push_list (web->dlink, &simplify_wl);
	break;
      case FREEZE:
	push_list (web->dlink, &freeze_wl);
	break;
      case SPILL:
	push_list (web->dlink, &spill_wl);
	break;
      case SPILLED:
	push_list (web->dlink, &spilled_nodes);
	break;
      case COALESCED:
        push_list (web->dlink, &coalesced_nodes);
	break;
      case COLORED:
	push_list (web->dlink, &colored_nodes);
	break;
      case SELECT:
        push_list (web->dlink, &select_stack);
	break;
      default:
	abort ();
    }
  web->type = type;
}

static inline void
put_move (move, type)
     struct move *move;
     enum move_type type;
{
  switch (type)
    {
      case WORKLIST:
	push_list (move->dlink, &mv_worklist);
	break;
      case MV_COALESCED:
	push_list (move->dlink, &mv_coalesced);
	break;
      case CONSTRAINED:
	push_list (move->dlink, &mv_constrained);
	break;
      case FROZEN:
	push_list (move->dlink, &mv_frozen);
	break;
      case ACTIVE:
	push_list (move->dlink, &mv_active);
	break;
      default:
	abort ();
    }
  move->type = type;
}

/* Build the worklists we are going to process.  */
static void
build_worklists (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  unsigned int i;
  struct move_list *ml;
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = id2web[i];
      struct dlist *d = (struct dlist *) xcalloc (1, sizeof (struct dlist));
      DLIST_WEB (d) = web;
      web->dlink = d;
      if (web->type == PRECOLORED)
        continue;

      if (web->num_conflicts >= NUM_REGS (web))
	put_web (web, SPILL);
      else if (web->moves)
	put_web (web, FREEZE);
      else
	put_web (web, SIMPLIFY);
    }

  for (ml = wl_moves; ml; ml = ml->next)
    if (ml->move)
      {
        struct dlist *d = (struct dlist *) xcalloc (1, sizeof (struct dlist));
	struct move *m = ml->move;
        DLIST_MOVE (d) = m;
        m->dlink = d;
	put_move (m, WORKLIST);
      }
}

/* Enable a move to be processed.  */
static void
enable_move (web)
     struct web *web;
{
  struct move_list *ml;
  for (ml = web->moves; ml; ml = ml->next)
    if (ml->move->type == ACTIVE)
      {
	remove_list (ml->move->dlink, &mv_active);
	put_move (ml->move, WORKLIST);
      }
}

/* Decrement the degree of the node, and if necessary, it's
   neighbors.  */
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
	    put_web (web, FREEZE);
	  else
	    put_web (web, SIMPLIFY);
	}
    }
}

/* Simplify the nodes on the simplify worklist.  */
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
      put_web (web, SELECT);
      for (wl = conflicts[web->id]; wl; wl = wl->next)
        if (wl->web->type != SELECT && wl->web->type != COALESCED)
	  {
	    decrement_degree (wl->web, 1 + web->add_hardregs);
	  }
    }
}

/* Helper function to remove a move from the movelist of the web.  */
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
  for (; ml->next && ml->next->move != move; ml = ml->next) ;
  if (!ml->next)
    return;
  {
    struct move_list *old = ml->next;
    ml->next = ml->next->next;
    free (old);
  }
}

/* Remove a move from the movelist of the web.  Actually this is just a
   wrapper around remove_move_1(), making sure, the removed move really is
   not in the list anymore.  */
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

/* Merge the moves for the two webs into the first web's movelist.  */
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
      if (! REGNO_REG_SET_P (seen, INSN_UID (ml->move->insn)))
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

/* Add a web to the simplify worklist, from the freeze worklist.  */
static void
add_worklist (web)
     struct web *web;
{
  if (web->type != PRECOLORED && !web->moves
      && web->num_conflicts < NUM_REGS (web))
    {
      remove_list (web->dlink, &freeze_wl);
      put_web (web, SIMPLIFY);
    }
}

/* Precolored node coalescing heuristic.  */
static int
ok (target, source)
     struct web *target, *source;
{
  struct web_link *wl;

  /* Source is PRECOLORED.  We test here, if it isn't one of the fixed
     registers.  In this case we disallow the coalescing.
     XXX maybe not all fixed_regs[] have to be excluded.  The actual
     example I was looking at, was a copy (set (reg a) (reg 7 sp)) (the
     stackpointer).  It all worked till the assembler. The coalescing
     resulted in a (%eax, %esp) indexed address, which is invalid.
     May be we only need to exclude stackpointers.  Esp. the PIC register
     is interesting.  I guess we can anyway make it available for coloring,
     as long as it doesn't interfere with any current use.  */
  if (TEST_HARD_REG_BIT (never_use_colors, source->color))
    return 0;

  /* XXX we can't right now combine a web with a hard-reg to a web
     with a multi-word pseudo, cause we get the conflicts wrong (we forget
     to add the other hard-regs to the conflicts, and only see the first).  */
  if (source->add_hardregs != target->add_hardregs)
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

/* Non-precolored node coalescing heuristic.  */
static int
conservative (target, source)
     struct web *target, *source;
{
  unsigned int k;
  regset seen;
  struct web_link *wl;
  unsigned int num_regs = NUM_REGS (target); /* XXX */

  /* k counts the resulting conflict weight, if target and source
     would be merged, and all low-degree neightbors would be
     removed.  */
  k = MAX (target->add_hardregs, source->add_hardregs);
  seen = BITMAP_XMALLOC ();
  for (wl = conflicts[target->id]; wl; wl = wl->next)
    if (wl->web->type != SELECT && wl->web->type != COALESCED
	&& wl->web->num_conflicts >= NUM_REGS (wl->web)
	&& ! REGNO_REG_SET_P (seen, wl->web->id))
      {
	SET_REGNO_REG_SET (seen, wl->web->id);
        k += 1 + wl->web->add_hardregs;
      }
  for (wl = conflicts[source->id]; wl; wl = wl->next)
    if (wl->web->type != SELECT && wl->web->type != COALESCED
	&& wl->web->num_conflicts >= NUM_REGS (wl->web)
	&& ! REGNO_REG_SET_P (seen, wl->web->id))
      {
	SET_REGNO_REG_SET (seen, wl->web->id);
        k += 1 + wl->web->add_hardregs;
      }
  BITMAP_XFREE (seen);

  if (k >= num_regs)
    return 0;
  return 1;
}

/* If the web is coalesced, return it's alias.  Otherwise, return what
   was passed in.  */
static struct web *
alias (web)
     struct web *web;
{
  while (web->type == COALESCED)
    web = web->alias;
  return web;
}

/* Actually combine two webs, when they can be coalesced.  */
static void
combine (u, v)
     struct web *u, *v;
{
  struct web_link *wl;
  if (v->type == FREEZE)
    remove_list (v->dlink, &freeze_wl);
  else
    remove_list (v->dlink, &spill_wl);
  put_web (v, COALESCED);
  v->alias = u;
  u->is_coalesced = 1;
  v->is_coalesced = 1;
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
      put_web (u, SPILL);
    }

  /* XXX combine the usable_regs together, and adjust the weight and
     conflict.  As this might constrain more, it might make us uncolorable.
     I think we should disallow this in conservative test, so only
     webs with similar constraints are coalesced.  */
  /* If we coalesce a spill temporary together with a normal node,
     the result is not any longer a spill temporary.  */
  if (u->spill_temp && ! v->spill_temp)
    u->spill_temp = 0;
}

/* Attempt to coalesce the first thing on the move worklist.  */
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
      put_move (m, MV_COALESCED);
      add_worklist (source);
    }
  else if (target->type == PRECOLORED
	   || TEST_BIT (igraph, igraph_index (source->id, target->id)))
    {
      remove_move (source, m);
      remove_move (target, m);
      put_move (m, CONSTRAINED);
      add_worklist (source);
      add_worklist (target);
    }
  else if ((source->type == PRECOLORED && ok (target, source))
	   || (source->type != PRECOLORED && conservative (target, source)))
    {
      remove_move (source, m);
      remove_move (target, m);
      put_move (m, MV_COALESCED);
      combine (source, target);
      add_worklist (source);
    }
  else
    put_move (m, ACTIVE);
}

/* Freeze the moves associated with the web.  */
static void
freeze_moves (web)
     struct web *web;
{
  struct move_list *ml, *ml_next;
  for (ml = web->moves; ml; ml = ml_next)
    {
      struct move *m = ml->move;
      struct web *src, *dest;
      ml_next = ml->next;
      if (m->type == ACTIVE)
	remove_list (m->dlink, &mv_active);
      else
	remove_list (m->dlink, &mv_worklist);
      put_move (m, FROZEN);
      remove_move (web, m);
      src = alias (m->source_web);
      dest = alias (m->target_web);
      src = (src == web) ? dest : src;
      remove_move (src, m);
      /* XXX GA use the original v, instead of alias(v) */
      if (!src->moves && src->num_conflicts < NUM_REGS (src))
	{
	  remove_list (src->dlink, &freeze_wl);
	  put_web (src, SIMPLIFY);
	}
    }
}

/* Freeze the first thing on the freeze worklist.  */
static void
freeze (void)
{
  struct dlist *d = pop_list (&freeze_wl);
  put_web (DLIST_WEB (d), SIMPLIFY);
  freeze_moves (DLIST_WEB (d));
}

static long (*spill_heuristic) PARAMS ((struct web *));

static long default_spill_heuristic PARAMS ((struct web *));

static long
default_spill_heuristic (w)
     struct web *w;
{
  return (w->weight * w->spill_cost) / w->num_conflicts;
}

/* Select the cheapest spill to be potentially spilled (we don't
   *actually* spill until we need to).  */
static void
select_spill (void)
{
  long best = INT_MAX;
  struct dlist *bestd = NULL;
  struct dlist *d;
  for (d = spill_wl; d; d = d->next)
    {
      struct web *w = DLIST_WEB (d);
      long cost = spill_heuristic (w);
      if (!w->spill_temp && cost < best)
	{
	  best = cost;
	  bestd = d;
	}
    }
  if (!bestd)
    abort ();

  remove_list (bestd, &spill_wl);
  DLIST_WEB (bestd)->was_spilled = best; /* Note the potential spill.  */
  put_web (DLIST_WEB (bestd), SIMPLIFY);
  freeze_moves (DLIST_WEB (bestd));
  debug_msg (0, " potential spill web %3d, conflicts = %d\n",
	     DLIST_WEB (bestd)->id, DLIST_WEB (bestd)->num_conflicts);
}

/* Searches in FREE_COLORS for a block of hardregs of the right length
   for MODE.  If it needs more than one hardreg it prefers blocks beginning
   at an even hardreg, and only gives an odd begin reg if no other
   block could be found.  */
static int
get_free_reg (free_colors, mode)
     HARD_REG_SET free_colors;
     enum machine_mode mode;
{
  int c;
  int last_resort_reg = -1;
  int pref_reg = -1;
  int pref_reg_order = INT_MAX;
  int last_resort_reg_order = INT_MAX;
  
  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
    if (TEST_HARD_REG_BIT (free_colors, c) && HARD_REGNO_MODE_OK (c, mode))
      {
	int i, size;
	size = HARD_REGNO_NREGS (c, mode);
	for (i = 1; i < size && TEST_HARD_REG_BIT (free_colors, c + i); i++);
	if (i == size)
	  {
	    if (size < 2 || (c & 1) == 0)
	      {
		if (inv_reg_alloc_order[c] < pref_reg_order)
		  {
		    pref_reg = c;
		    pref_reg_order = inv_reg_alloc_order[c];
		  }
	      }
	    else if (inv_reg_alloc_order[c] < last_resort_reg_order)
	      {
		last_resort_reg = c;
		last_resort_reg_order = inv_reg_alloc_order[c];
	      }
	  }
	else
	  c += i;
      }
  return pref_reg >= 0 ? pref_reg : last_resort_reg;
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
   There are 3 hardregs at all.  Now a simple minded method might choose 1
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
      else if (w->was_spilled < 0 && w->add_hardregs >= neighbor_needs)
	{
	  neighbor_needs = w->add_hardregs;
	  fat_neighbor = w;
	  num_fat++;
	}
    }

  while (1)
    {
      int long_blocks;
      HARD_REG_SET call_clobbered;
	
      /* Here we choose a hard-reg for the current web.  For non spill
         temporaries we first search in the hardregs for it's prefered
	 class, then, if we found nothing appropriate, in those of the
	 alternate class.  For spill tempraries we only search in
	 usable_regs of this web (which is probably larger than that of
	 the preferred or alternate class).  All searches first try to
	 find a non-call-clobbered hard-reg.  
         XXX this should be more finegraned... First look into preferred
         non-callclobbered hardregs, then _if_ the web crosses calls, in
         alternate non-cc hardregs, and only _then_ also in preferred cc
         hardregs (and alternate ones).  Currently we don't track the number
         of calls crossed for webs.  We should.  */
      if (web->use_my_regs)
	COPY_HARD_REG_SET (colors, web->usable_regs);
      else
        COPY_HARD_REG_SET (colors,
			   usable_regs[reg_preferred_class (web->regno)]);
      AND_COMPL_HARD_REG_SET (colors, conflict_colors);
      COPY_HARD_REG_SET (call_clobbered, colors);
      AND_HARD_REG_SET (call_clobbered, call_used_reg_set);

      c = get_free_reg (call_clobbered, PSEUDO_REGNO_MODE (web->regno));
      if (c < 0)
	c = get_free_reg (colors, PSEUDO_REGNO_MODE (web->regno));
      
      if (!web->use_my_regs && c < 0)
	{
	  IOR_HARD_REG_SET (colors, usable_regs
			    [reg_alternate_class (web->regno)]);
	  AND_COMPL_HARD_REG_SET (colors, conflict_colors);
	  COPY_HARD_REG_SET (call_clobbered, colors);
	  AND_HARD_REG_SET (call_clobbered, call_used_reg_set);
	  
	  c = get_free_reg (call_clobbered, PSEUDO_REGNO_MODE (web->regno));
	  if (c < 0)
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
  if (bestc >= 0 && c < 0 && web->was_spilled < 0)
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
	 are available in colors, but they are not consecutive.  This is a
	 very serious issue if this web is a short live one, because
	 even if we spill this one here, the situation won't become better
	 in the next iteration.  It probably will have the same conflicts,
	 those will have the same colors, and we would come here again, for
	 all parts, in which this one gets splitted by the spill.  This
	 can result in endless iteration spilling the same register again and
	 again.  That's why we try to find a neighbor, which spans more
	 instructions that ourself, and got a color, and try to spill _that_.
	 XXX try to make this better.  Also look into REG_NO_CONFLICT notes
	 to make no useless conflicts (I once had a case were using them
	 had prevented the above endless iteration even without this method.
	 
	 if (DLIST_WEB (d)->was_spilled < 0)
	 abort (); */
      if (web->was_spilled < 0)
        debug_msg (0,
		   "  *** Web %d spilled, although it was simplified "
		   "[free = %x, mode = %s]\n", web->id,
#if FIRST_PSEUDO_REGISTER <= HOST_BITS_PER_WIDE_INT
		   colors,
#else
		   /* Yes, yes, I know, that's not all hard-regs.  */
		   colors[0],
#endif
		   GET_MODE_NAME (PSEUDO_REGNO_MODE (web->regno)));
      if (web->spill_temp)
        debug_msg (0,
		   "  *** Web %d spilled, although it's a spilltemp\n",
		   web->id);
      if (web->was_spilled < 0 || web->spill_temp)
	{
	  for (wl = conflicts[web->id]; wl; wl = wl->next)
	    {
	      /* Normally we would have w=alias(wl->web), to get to all
		 conflicts.  But we can't simply spill webs which are
		 involved in coalescing anyway.  The premise for combining
		 webs, was that the final one will get a color.  One reason
		 is, that the code inserting the spill insns can't cope
		 with aliased webs (yet, may be, we should extend that).  */
	      struct web *w = wl->web;
	      if (w->type == COLORED && !w->spill_temp && !w->is_coalesced
		  && w->add_hardregs >= web->add_hardregs
		  && w->span_insns > web->span_insns)
		{
		  int old_c = w->color;
		  remove_list (w->dlink, &colored_nodes);
		  put_web (w, SPILLED);
		  /* Now try to colorize us again.  Can recursively make other
		     webs also spill, until there are no more unspilled
		     neighbors.  */
		  colorize_one_web (web);
		  if (web->type != COLORED)
		    {
		      /* We tried recursively to spill all already colored
			 neighbors, but we are still uncolorable.  So it made
			 no sense to spill those neighbors.  Recolor them.  */
		      remove_list (w->dlink, &spilled_nodes);
		      put_web (w, COLORED);
		      w->color = old_c;
		    }
		  break;
		}
	    }
	  /* Here we either have got a color for us (from recursive calls),
	     or not.  In the latter case we give up hope, and just spill
	     ourself.  */
	  if (web->type == SELECT)
	    put_web (web, SPILLED);
	}
      else
        put_web (web, SPILLED);
    }
  else
    {
      put_web (web, COLORED);
      web->color = c;
    }
}

/* Assign the colors to the nodes on the select stack.  */
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

/* Allocate a spill slot for a web.  */
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

/* Rewrite the program to include the spill code.  */
static void
rewrite_program (df)
     struct df *df;
{
  unsigned int i;
  struct web *web;
  for (i = 0; i < df->def_id; i++)
    {
      web = def2web[i];
      if (web->type == SPILLED)
	{
	  rtx insn = DF_REF_INSN (df->defs[i]);
	  if (!web->stack_slot)
	    allocate_spill_web (web);
	  /*if (! validate_change (insn, df->defs[i]->loc, web->stack_slot, 0))
	    */
	    {
	      rtx following = NEXT_INSN (insn);
	      basic_block bb = BLOCK_FOR_INSN (insn);
	      rtx insns;
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
    }
  for (i = 0; i < df->use_id; i++)
    {
      web = use2web[i];
      if (web->type == SPILLED)
	{
	  rtx insn = DF_REF_INSN (df->uses[i]);
	  /*if (! validate_change (insn, df->uses[i]->loc, web->stack_slot, 0))
	    */
	    {
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
}

/* Create new pseudos for each web we colored, and set up reg_renumber.  */
static void
emit_colors (df)
     struct df *df;
{
  unsigned int i;
  int si;
  struct web *web;
  int old_max_regno = max_reg_num ();
  regset old_regs;
  /* First create the (REG xx) rtx's for all webs, as we need to know
     the number, to make sure, flow has enough memory for them in the
     various tables.  */
  for (i = 0; i < num_webs; i++)
    {
      web = id2web[i];
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
     pseudoreg for each web.  All uses.  */
  /* XXX
     Beware: The order of replacements (first uses, then defs) matters only
     for read-mod-write insns, where the RTL expression for the REG is
     shared between def and use.  For normal rmw insns we connected all such
     webs, i.e. both the use and the def (which are the same memory)
     here get the same new pseudo-reg, so order would not matter.
     _However_ we did not connect webs, were the read cycle was an
     uninitialized read.  If we now would first replace the def reference
     and then the use ref, we would initialize it with a REG rtx, which
     gets never initialized, and yet more wrong, which would overwrite
     the definition of the other REG rtx.  So we must replace the defs last.
   */
  for (i = 0; i < df->use_id; i++)
    {
      regset rs = DF_REF_BB (df->uses[i])->global_live_at_start;
      web = use2web[i];
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      *DF_REF_LOC (df->uses[i]) = web->reg_rtx;
      if (REGNO_REG_SET_P (rs, web->regno))
	{
          //CLEAR_REGNO_REG_SET (rs, web->regno);
          SET_REGNO_REG_SET (rs, REGNO (web->reg_rtx));
	}
    }

  /* And all defs.  */
  for (i = 0; i < df->def_id; i++)
    {
      regset rs = DF_REF_BB (df->defs[i])->global_live_at_start;
      web = def2web[i];
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      *DF_REF_LOC (df->defs[i]) = web->reg_rtx;
      if (REGNO_REG_SET_P (rs, web->regno))
	{
	  /* Don't simply clear the current regno, as it might be
	     replaced by two webs.  */
          //CLEAR_REGNO_REG_SET (rs, web->regno);
          SET_REGNO_REG_SET (rs, REGNO (web->reg_rtx));
	}
    }

  /* And now set up the reg_renumber array for reload with all the new
     pseudo-regs.  */
  for (i = 0; i < num_webs; i++)
    {
      web = id2web[i];
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      reg_renumber[REGNO (web->reg_rtx)] = web->color;
    }

  old_regs = BITMAP_XMALLOC ();
  for (si = FIRST_PSEUDO_REGISTER; si < old_max_regno; si++)
    SET_REGNO_REG_SET (old_regs, si);
  for (si = 0; si < n_basic_blocks; si++)
    {
      AND_COMPL_REG_SET (BASIC_BLOCK (si)->global_live_at_start, old_regs);
      AND_COMPL_REG_SET (BASIC_BLOCK (si)->global_live_at_end, old_regs);
    }
  BITMAP_XFREE (old_regs);

  if (rtl_dump_file)
    for (i = FIRST_PSEUDO_REGISTER; i < (unsigned int) max_regno; i++)
      debug_msg (0, "Renumber pseudo %d to %d\n", i, reg_renumber[i]);
}

/* Perform one pass of iterated coalescing.  */
static int
one_pass (df)
     struct df *df;
{
  build_i_graph (df);
  if (rtl_dump_file)
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

/* Dump debugging info for the register allocator.  */
static void
dump_ra (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  struct web *web;
  struct dlist *d;
  unsigned int i;
  if (!rtl_dump_file || 0 > debug_new_regalloc)
    return;
  debug_msg (0, "Webs:\n");
  for (i = 0; i < num_webs; i++)
    {
      web = id2web[i];
      debug_msg (0, "  %4d : regno %3d +%d (span %d, weight %d) (%s)%s",
		 i, web->regno,
	         web->add_hardregs, web->span_insns, web->weight,
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

/* Initialize the register allocator.  */
static void
init_ra (void)
{
  int i;
  HARD_REG_SET rs;
  /* FIXME: Choose spill heuristic for platform if we have one */
  spill_heuristic = default_spill_heuristic;

  COPY_HARD_REG_SET (never_use_colors, fixed_reg_set);

#if HARD_FRAME_POINTER_REGNUM != FRAME_POINTER_REGNUM
  for (i = HARD_REGNO_NREGS (HARD_FRAME_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, HARD_FRAME_POINTER_REGNUM + i);
#endif

  for (i = HARD_REGNO_NREGS (FRAME_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, FRAME_POINTER_REGNUM + i);

  for (i = HARD_REGNO_NREGS (STACK_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, STACK_POINTER_REGNUM + i);

  for (i = HARD_REGNO_NREGS (ARG_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, ARG_POINTER_REGNUM + i);
	
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

/* Main register allocator entry point.  */
void
reg_alloc (void)
{
  struct df *df;
  int changed;
  int pass = 0;
  init_ra ();
  no_new_pseudos = 0;
  /* We don't use those NOTEs, and as we anyway change all registers,
     they only make problems later.  */
  count_or_remove_death_notes (NULL, 1);
  do
    {
      debug_msg (0, "RegAlloc Pass %d\n\n", pass);
      if (pass++ > 40)
	internal_error ("Didn't find a coloring.\n");
      df = df_init ();
      df_analyse (df, 0, DF_HARD_REGS | DF_RD_CHAIN | DF_RU_CHAIN);
      if (rtl_dump_file)
	{
	  rtx insn;
	  df_dump (df, DF_HARD_REGS, rtl_dump_file);
	  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
            if (INSN_P (insn))
	      {
	        df_insn_debug_regno (df, insn, rtl_dump_file);
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
  if (rtl_dump_file)
    print_rtl_with_bb (rtl_dump_file, get_insns ());

  no_new_pseudos = 1;
  compute_bb_for_insn (get_max_uid ());
  /*recompute_reg_usage (get_insns (), TRUE);
  regclass (get_insns (), max_reg_num (), rtl_dump_file);*/
  /*count_or_remove_death_notes (NULL, 1);
  allocate_reg_life_data ();
  update_life_info (NULL, UPDATE_LIFE_GLOBAL, PROP_REG_INFO
		    | PROP_DEATH_NOTES | PROP_SCAN_DEAD_CODE
		    | PROP_KILL_DEAD_CODE);*/
  find_basic_blocks (get_insns (), max_reg_num (), rtl_dump_file);
  life_analysis (get_insns (), rtl_dump_file, PROP_FINAL);
  recompute_reg_usage (get_insns (), TRUE);
  regclass (get_insns (), max_reg_num (), rtl_dump_file);
}

/*
vim:cinoptions={.5s,g0,p5,t0,(0,^-0.5s:tw=78:cindent:
*/
