/* Graph coloring register allocator
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Michael Matz <matzmich@cs.tu-berlin.de>
   and Daniel Berlin <dan@cgsoftware.com>

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

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tm_p.h"
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
#include "flags.h"
#include "ggc.h"
#include "reload.h"

/* The algorithm used is currently Iterated Register Coalescing by
   L.A.George, and Appel.
*/

/* TODO
 
   * Lattice based rematerialization
   * Fix spill costs for rematerialization (if any part of a web is
   rematerializable, the spill cost becomes very small, even if the
   rest of the web isn't)
   * do lots of commenting
   * look through all XXX's and do something about them
   * handle REG_NO_CONFLICTS blocks correctly (the current ad hoc approach
     might miss some conflicts due to insns which only seem to be in a 
     REG_NO_CONLICTS block)
     -- Don't necessary anymore, I believe, because SUBREG tracking is
     implemented.
   * create definitions of ever-life regs at the beginning of
     the insn chain
   * create webs for all hardregs, not just those actually defined
     (so we later can use that to implement every constraint)
   * insert loads as soon, stores as late as possile
   * insert spill insns as outward as possible (either looptree, or LCM)
   * reuse stack-slots
   * use the frame-pointer, when we can
   * delete coalesced insns.  Partly done.  The rest can only go, when we get
     rid of reload.
   * don't insert hard-regs, but a limited set of pseudo-reg
     in emit_colors, and setup reg_renumber accordingly (done, but this
     needs reload, which I want to go away)
   * when spilling, update interference graph incrementally, which
     is possible, as we don't use global liveness
   * don't destroy coalescing information completely when spilling
   * use the constraints from asms
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

struct tagged_conflict
{
  struct tagged_conflict *next;
  bitmap conflicts;
  unsigned int size : 8;
  unsigned int word : 8;
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
  struct tagged_conflict *sub_conflicts;
  unsigned int crosses_call : 1;
};

/* Web structure used to store info about connected live ranges.  */
struct web
{
  unsigned int id; /* Unique web ID */
  unsigned int regno; /* Register number of the live range's variable */
  unsigned HOST_WIDE_INT weight; /* Weight used to determine, among other
                          things, how many colors we can block */
  unsigned int span_insns; /* How many insn's the live range spans */
  unsigned int spill_temp; /* Temporarily used to remember what webs
                              were spilled */
  unsigned int use_my_regs; /* Determine if we should use the
                               usable_regs for this web */
  unsigned HOST_WIDE_INT spill_cost; /* Cost of spilling */
  int was_spilled;
  /* We need to distinguish also webs which are targets of coalescing
     (all x with some y, so that x==alias(y)), but the alias field is
     only set for sources of coalescing.  This flag is set for all webs
     involved in coalescing in some way.  */
  int is_coalesced;
  int color; /* Color of the web */
  int add_hardregs; /* Additional hard registers needed to be
                       allocated to the web */
  int has_sub_conflicts; /* != 0 if this web conflicts with sub_webs */
  int artificial; /* != 0 : there is no rtl in the code which corresponds
		     to this web.  Happens e.g. with conflicts to a web,
		     of which only a part was still undefined at the point
		     of that conflict.  In this case we construct a subweb
		     representing these yet undefined bits to have a target
		     for the conflict.  Suppose e.g. this sequence:
		     (set (reg:DI x) ...)
		     (set (reg:SI y) ...)
		     (set (subreg:SI (reg:DI x) 0) ...)
		     (use (reg:DI x))
		     Here x only partly conflicts with y.  Namely only
		     (subreg:SI (reg:DI x) 1) conflicts with it, but this
		     rtx doesn't show up in the program.  For such things
		     an "artificial" subweb is built, and this flag is true
		     for them.  */
  int crosses_call;
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
			      GET_CODE(orig_x) == REG.  For webs x without
			      subregs subreg_next is x.  */

  /* The set of webs (or subwebs), this web conflicts with.  Note, that
     in this list only webs are noted, which really conflict with this web.
     I.e. If web A conflicts only with subweb B,1, then A is _not_ in B's
     conflict_list, but instead really in B,1's.  */
  struct conflict_link *conflict_list;
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

/* This represents an edge in the conflict graph.
   XXX: the structure of that graph isn't satisfactional at all.
   As long as we didn't have subreg's, we simply linked all webs, which a
   web A conflicts with into A's conflict list in structures 'web_link'.
   But with subreg, we collect all conflicts of a web _and it's subwebs_
   into the parent web in a linked list (as before), whose entries point
   to the webs it conflicts with (also to subwebs), so the "target"
   direction of one such edge is precise.  The problem is, that sometimes
   in the algorithms below we also need the precise "source" of the conflict
   (we iterate often over all conflicts for a parent web, but then need to
   know precisely what subweb of that web is the one which conflicts).
   For now we do this by simply storing not only the target of the edge,
   but also the source into this structure.  What is unsatisfactory about
   this is, that the "source" is constant in the way, that all "sources"
   in one such list belong to the same parent web, which even is the initial
   point of this list.  We'll do this later in a better way.  */
struct sub_conflict
{
  struct sub_conflict *next;
  struct web *s;
  struct web *t;
};

struct conflict_link
{
  struct conflict_link *next;
  struct web *t;
  struct sub_conflict *sub;
};

struct conflict
{
  struct conflict *next; /* Next conflict set, whose source is the same super
			    web is this set.  */
  struct web *s;  /* The source of the set of these conflicts.  */
  struct web_link *t; /* The targets of conflicts for this source.  */
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

static int rtx_to_bits PARAMS ((rtx));
static bitmap find_sub_conflicts PARAMS ((struct web_part *, int, int));
static bitmap get_sub_conflicts PARAMS ((struct web_part *, int, int));
static bitmap undef_to_bitmap PARAMS ((struct web_part *,
				       unsigned HOST_WIDE_INT *));
static struct web_part * find_web_part_1 PARAMS ((struct web_part *));
static struct web_part * union_web_part_roots
				PARAMS ((struct web_part *, struct web_part *));
static int defuse_overlap_p PARAMS ((rtx, struct curr_use *));
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
static struct web * find_subweb_2 PARAMS ((struct web *, unsigned int,
					   unsigned int));
static struct web * add_subweb PARAMS ((struct web *, rtx));
static struct web * add_subweb_2 PARAMS ((struct web *, unsigned int,
					  unsigned int));
static struct web * find_web_for_subweb PARAMS ((struct web *));
static int regs_overlap_p PARAMS ((rtx, rtx));
static void init_web_parts PARAMS ((struct df *));
static void add_conflict_edge PARAMS ((struct web *, struct web *));
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
int get_free_reg PARAMS ((HARD_REG_SET, HARD_REG_SET,
			  enum machine_mode));
static int count_long_blocks PARAMS ((HARD_REG_SET, int));
static char * hardregset_to_string PARAMS ((HARD_REG_SET));
static void colorize_one_web PARAMS ((struct web *, int));
static void assign_colors PARAMS ((void));
static void spill_coalescing PARAMS ((sbitmap, sbitmap));
static void allocate_spill_web PARAMS ((struct web *));
static void rewrite_program PARAMS ((void));
static void actual_spill PARAMS ((void));
static void emit_colors PARAMS ((struct df *));
static void delete_moves PARAMS ((void));
static int one_pass PARAMS ((struct df *));
static void dump_constraints PARAMS ((void));
static void dump_ra PARAMS ((struct df *));
static void init_ra PARAMS ((void));
static int rematerializable PARAMS ((struct ref *));
static bool find_splits PARAMS ((unsigned int, int *));
static void splits_init PARAMS ((void));
static int only_one_reaching_def PARAMS ((struct df_link *));
void reg_alloc PARAMS ((void));

/* XXX use Daniels compressed bitmaps here.  */
#define igraph_index(i, j) ((i) < (j) ? ((j)*((j)-1)/2)+(i) : ((i)*((i)-1)/2)+(j))
static bitmap unbrokengraph;
static sbitmap igraph;
/* Uhhuuhu.  Don't the hell use two sbitmaps! XXX
   (for now I need the sup_igraph to note if there is any conflict between
   parts of webs at all.  I can't use igraph for this, as there only the real
   conflicts are noted.)  This is only used to prevent coalescing two
   conflicting webs, were only parts of them are in conflict.  */
static sbitmap sup_igraph;

struct bb_begin_info
{
  unsigned int pass;
  unsigned HOST_WIDE_INT undefined;
  void *old_aux;
};
static unsigned int visited_pass;
static sbitmap move_handled;

static struct web_part *web_parts;
struct visit_trace
{
  struct web_part *wp;
  unsigned HOST_WIDE_INT undefined;
};
static struct visit_trace *visit_trace;  /* Indexed by UID.  */
static unsigned int num_webs;
static unsigned int num_subwebs;
static struct web **id2web; /* Indexed by web id (0..num_webs-1).  */
static struct web *hardreg2web[FIRST_PSEUDO_REGISTER];
static struct web **def2web;
static struct web **use2web;
static struct move_list *wl_moves;
static struct ref **all_defs_for_web;
static struct ref **all_uses_for_web;
struct df *df;

/* Used to detect spill instructions inserted by me.  */
static int orig_max_uid;

static HARD_REG_SET never_use_colors;
static HARD_REG_SET usable_regs[N_REG_CLASSES];
static unsigned int num_free_regs[N_REG_CLASSES];

#define NUM_REGS(W) (((W)->type == PRECOLORED) ? 1 : (W)->num_freedom)
#define SUBWEB_P(W) (GET_CODE ((W)->orig_x) == SUBREG)

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

#define BYTE_BEGIN(i) ((unsigned int)(i) & 0xFFFF)
#define BYTE_LENGTH(i) (((unsigned int)(i) >> 16) & 0xFFFF)
static int
rtx_to_bits (x)
     rtx x;
{
  unsigned int len, beg = 0;
  len = GET_MODE_SIZE (GET_MODE (x));
  if (GET_CODE (x) == SUBREG)
    {
      beg = SUBREG_BYTE (x);
    }
  return (((len & 0xFFFF) << 16) | (beg & 0xFFFF));
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

  /* We recognize moves between subreg's as copy insns.  This is used to avoid
     conflicts of those subwebs.  But they are currently _not_ used for
     coalescing (the check for this is in remember_move() below).  */
  while (GET_CODE (d) == STRICT_LOW_PART)
    d = XEXP (d, 0);
  if (GET_CODE (d) != REG
      && (GET_CODE (d) != SUBREG || GET_CODE (SUBREG_REG (d)) != REG))
    return 0;
  while (GET_CODE (s) == STRICT_LOW_PART)
    s = XEXP (s, 0);
  if (GET_CODE (s) != REG
      && (GET_CODE (s) != SUBREG || GET_CODE (SUBREG_REG (s)) != REG))
    return 0;

  /* Copies between hardregs are useless for us, as not coalesable anyway. */
  if (REGNO (GET_CODE (s) == SUBREG ? SUBREG_REG (s) : s) 
       < FIRST_PSEUDO_REGISTER
      && REGNO (GET_CODE (d) == SUBREG ? SUBREG_REG (d) : d) 
         < FIRST_PSEUDO_REGISTER)
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

static bitmap
find_sub_conflicts (wp, size, word)
     struct web_part *wp;
     int size;
     int word;
{
  struct tagged_conflict *cl;
  cl = wp->sub_conflicts;
  for (; cl; cl = cl->next)
    if (cl->size == size && cl->word == word)
      return cl->conflicts;
  return NULL;
}

static bitmap
get_sub_conflicts (wp, size, word)
     struct web_part *wp;
     int size;
     int word;
{
  bitmap b = find_sub_conflicts (wp, size, word);
  if (!b)
    {
      struct tagged_conflict *cl =
	(struct tagged_conflict *) xmalloc (sizeof *cl);
      cl->conflicts = BITMAP_XMALLOC ();
      cl->size = size;
      cl->word = word;
      cl->next = wp->sub_conflicts;
      wp->sub_conflicts = cl;
      b = cl->conflicts;
    }
  return b;
}

/* XXX we should make WORD actually be at least byte-offset (or bit offset)
   for subreg's.  Otherwise we track bytes and shorts incorrectly.  For now
   we denote them with different words.  */
struct undef_table_s {
  int new_undef;
  int size;
  int word;
} undef_table [] = {
  { 0, 0, 0}, /* 0 */
  { 0, 1, 0},
  { 0, 1, 1},
  { 0, 2, 0},
  { 0, 1, 2}, /* 4 */
  { 1, 1, 2},
  { 2, 1, 2},
  { 3, 1, 2},
  { 0, 1, 3}, /* 8 */
  { 1, 1, 3},
  { 2, 1, 3},
  { 3, 1, 3},
  { 0, 2, 2}, /* 12 */
  { 1, 2, 2},
  { 2, 2, 2}};

static bitmap
undef_to_bitmap (wp, undefined)
     struct web_part *wp;
     unsigned HOST_WIDE_INT *undefined;
{
  if (*undefined < 15)
    {
      struct undef_table_s u;
      u = undef_table[*undefined];
      *undefined = u.new_undef;
      return get_sub_conflicts (wp, u.size, u.word);
    }
  switch (*undefined)
    {
      case 0x000f : *undefined = 0; return get_sub_conflicts (wp, 4, 0);
      case 0x00f0 : *undefined = 0; return get_sub_conflicts (wp, 4, 4);
      case 0x00ff : *undefined = 0; return get_sub_conflicts (wp, 8, 0);
      case 0x0f00 : *undefined = 0; return get_sub_conflicts (wp, 4, 8);
      case 0x0ff0 : *undefined = 0xf0; return get_sub_conflicts (wp, 4, 8);
      case 0x0fff : *undefined = 0xff; return get_sub_conflicts (wp, 4, 8);
      case 0xf000 : *undefined = 0; return get_sub_conflicts (wp, 4, 12);
      case 0xff00 : *undefined = 0; return get_sub_conflicts (wp, 8, 8);
      case 0xfff0 : *undefined = 0xf0; return get_sub_conflicts (wp, 8, 8);
      case 0xffff : *undefined = 0; return get_sub_conflicts (wp, 16, 0);
      default :
	{
	  unsigned HOST_WIDE_INT u = *undefined;
	  struct undef_table_s u2;
	  int i;
	  
	  int word;
	  int size;
	  
	  for (word = 0; ! (u & 1); ++word)
	    u >>= 1;
	  for (size = 0; u & 1; size++)
	    u >>= 1;
	  if (u)
	    abort ();
	  *undefined = u;
	  return get_sub_conflicts (wp, size, word);
	}
	break;
    }
  /* XXX */
  abort ();
}

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

      if (!r1->sub_conflicts)
	r1->sub_conflicts = r2->sub_conflicts;
      else if (r2->sub_conflicts)
	/* We need to merge the conflict bitmaps from R2 into R1.  */
	{
	  struct tagged_conflict *cl1, *cl2;
	  /* First those from R2, which are also contained in R1.
	     We union the bitmaps, and free those from R2, resetting them
	     to 0.  */
	  for (cl1 = r1->sub_conflicts; cl1; cl1 = cl1->next)
	    for (cl2 = r2->sub_conflicts; cl2; cl2 = cl2->next)
	      if (cl1->size == cl2->size && cl1->word == cl2->word)
		{
		  bitmap_operation (cl1->conflicts, cl1->conflicts,
				    cl2->conflicts, BITMAP_IOR);
		  BITMAP_XFREE (cl2->conflicts);
		  cl2->conflicts = NULL;
		}
	  /* Now the conflict lists from R2 which weren't in R1.
	     We simply copy the entries from R2 into R1' list.  */
	  for (cl2 = r2->sub_conflicts; cl2;)
	    {
	      struct tagged_conflict *cl_next = cl2->next;
	      if (cl2->conflicts)
		{
		  cl2->next = r1->sub_conflicts;
		  r1->sub_conflicts = cl2;
		}
	      else
		free (cl2);
	      cl2 = cl_next;
	    }
	}
      r2->sub_conflicts = NULL;
      r1->crosses_call |= r2->crosses_call;
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
      rtx s, d;
      SET_BIT (move_handled, INSN_UID (insn));
      copy_insn_p (insn, &s, &d);
      /* XXX for now we don't remember move insns involving any subregs.
	 Those would be difficult to coalesce (we would need to implement
	 handling of all the subwebs in the allocator, including that such
	 subwebs could be source and target of coalesing).  */
      if (GET_CODE (s) == REG && GET_CODE (d) == REG)
	{
	  struct move *m = (struct move *) xcalloc (1, sizeof (struct move));
	  struct move_list *ml;
	  m->insn = insn;
	  ml = (struct move_list *) xmalloc (sizeof (struct move_list));
	  ml->move = m;
	  ml->next = wl_moves;
	  wl_moves = ml;
	}
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
defuse_overlap_p (def, use)
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
	    unsigned int bl = rtx_to_bits (def);
	    unsigned HOST_WIDE_INT old_u = use->undefined;
	    b = BYTE_BEGIN (bl);
	    e = b + BYTE_LENGTH (bl);
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
	  /* If the size of both things is the same, the subreg's overlap
	     if they refer to the same word.  */
	  if (SUBREG_BYTE (def) == SUBREG_BYTE (use->x))
	    return 1;
	/* Now the more difficult part: the same regno is refered, but the
	   sizes of the references or the words differ.  E.g.
           (subreg:SI (reg:CDI a) 0) and (subreg:DI (reg:CDI a) 2) do not
	   overlap, wereas the latter overlaps with (subreg:SI (reg:CDI a) 3).
	   */
	{
	  unsigned HOST_WIDE_INT old_u;
	  int b1, e1, b2, e2;
	  unsigned int bl1, bl2;
	  bl1 = rtx_to_bits (def);
	  bl2 = rtx_to_bits (use->x);
	  b1 = BYTE_BEGIN (bl1);
	  b2 = BYTE_BEGIN (bl2);
	  e1 = b1 + BYTE_LENGTH (bl1) - 1;
	  e2 = b2 + BYTE_LENGTH (bl2) - 1;
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
  visit_trace[uid].wp = wp;
  visit_trace[uid].undefined = use->undefined;

  if (INSN_P (insn))
    {
      struct df_link *link;
      unsigned int source_regno = ~0;
      unsigned int regno = use->regno;
      unsigned HOST_WIDE_INT orig_undef = use->undefined;
      unsigned HOST_WIDE_INT final_undef = use->undefined;
      rtx s = NULL, t;
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
	  source_regno = REGNO (GET_CODE (s) == SUBREG ? SUBREG_REG (s) : s);
	  remember_move (insn);
	}
      if (GET_CODE (insn) == CALL_INSN)
	wp->crosses_call = 1;
      for (link = DF_INSN_DEFS (df, insn); link; link = link->next)
        if (link->ref)
	  {
	    int lap;
	    use->undefined = orig_undef;
	    if ((lap = defuse_overlap_p (DF_REF_REG (link->ref), use)) != 0)
	      {
		if (lap == -1)
		  /* Same regnos but non-overlapping or already defined bits,
		     so ignore this DEF, or better said make the yet undefined
		     part and this DEF conflicting.  */
		  {
		    unsigned HOST_WIDE_INT undef;
		    undef = use->undefined;
		    while (undef)
		      bitmap_set_bit (undef_to_bitmap (wp, &undef),
				      DF_REF_ID (link->ref));
		    continue;
		  }
		if ((lap & 1) != 0)
		  /* The current DEF completely covers the USE, so we can
		     stop traversing the code looking for further DEFs.  */
	          defined = 1;
		else
		  /* We have a partial overlap.  */
		  {
		    final_undef &= use->undefined;
		    if (final_undef == 0)
		      /* Now the USE is completely defined, which means, that
			 we can stop looking for former DEFs.  */
		      defined = 1;
		    /* If this is a partial overlap, which left some bits
		       in USE undefined, we normally would need to create
		       conflicts between that undefined part and the part of
		       this DEF which overlapped with some of the formerly
		       undefined bits.  We don't need to do this, because both
		       parts of this DEF (that which overlaps, and that which
		       doesn't) are written together in this one DEF, and can
		       not be colored in a way which would conflict with
		       the USE.  This is only true for partial overlap,
		       because only then the DEF and USE have bits in common,
		       which makes the DEF move, if the USE moves, making them
		       aligned.
		       If they have no bits in common (lap == -1), they are
		       really independent.  Therefore we there make a
		       conflict.  */
		  }
		/* This is at least a partial overlap, so we need to union
		   the web parts.  */
		wp = union_web_parts (wp, &web_parts[DF_REF_ID (link->ref)]);
	      }
	    else 
	      {
		unsigned HOST_WIDE_INT undef = use->undefined;
		if (regno == source_regno)
		  /* This triggers only, when this was a copy insn and the
		     source is at least a part of the USE currently looked at.
		     In this case only the bits of the USE conflict with the
		     DEF, which are not covered by the source of this copy
		     insn, and which are still undefined.  I.e. in the best
		     case (the whole reg being the source), _no_ conflicts
		     between that USE and this DEF (the target of the move)
		     are created by this insn (though they might be by
		     others).  This is a super case of the normal copy insn
		     only between full regs.  */
		  {
		    unsigned int bl = rtx_to_bits (s);
		    int b = BYTE_BEGIN (bl);
		    int e = b + BYTE_LENGTH (bl);		    
		    /*FIXME: Complete BS hack to make x86 at least
		     *compile*, until someone fixes it (set (reg:QI)
		     (subreg:QI (reg:DI)) causes us to get a word of
		     1, size of 7, which later causes us to abort,
		     since this is wrong. */
		    if (e == 1 && b == 0)
		      {
			undef = 1;
		      }
		    else
		      for (; b < e; b++)
			{
			  undef &= ~((unsigned HOST_WIDE_INT)1 << b);
			}
		  }
		if (undef)
		  {
		    /*struct web_part *cwp;
		    cwp = find_web_part (&web_parts[DF_REF_ID
		       (link->ref)]);*/

		    /* TODO: somehow instead of noting the ID of the LINK
		       use the an ID nearer to the root webpart of that LINK.
		       We can't use the root itself, because we later use the
		       ID to look at the form (reg or subreg, and if yes,
		       which subreg) of this conflict.  This means, that we
		       need to remember in the root an ID for each form, and
		       maintaining this, when merging web parts.  This makes
		       the bitmaps smaller.  */
		    do
		      bitmap_set_bit (undef_to_bitmap (wp, &undef),
				      DF_REF_ID (link->ref));
		    while (undef);
		  }
	      }
	  }
      if (defined)
	use->undefined = 0;
      else
	use->undefined = final_undef;
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
  if (visit_trace[uid].wp
      && DF_REF_REGNO (visit_trace[uid].wp->ref) == use->regno
      && (use->undefined & ~visit_trace[uid].undefined) == 0)
    {
      (void) union_web_parts (visit_trace[uid].wp, use->wp);
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

  /* Note, that, even _if_ we are called with use->wp a root-part, this might
     become non-root in the for() loop below (due to live_out() unioning
     it).  So beware, not to change use->wp in a way, for which only root-webs
     are allowed.  */
  while (1)
    {
      int uid = INSN_UID (insn);
      basic_block bb = BLOCK_FOR_INSN (insn);
      rtx p;
      number_seen[uid]++;

      p = prev_real_insn (insn);
      if (!p)
	return;
      if (bb != BLOCK_FOR_INSN (p))
	{
	  edge e;
	  unsigned HOST_WIDE_INT undef = use->undefined;
	  struct bb_begin_info *info = (struct bb_begin_info *)bb->aux;
	  if ((e = bb->pred) == NULL)
	    return;
	  /* We now check, if we already traversed the predecessors of this
	     block for the current pass and the current set of undefined
	     bits.  If yes, we don't need to check the predecessors again.
	     I.e. conceptually this information is tagged to the first
	     insn of a basic block.  */
	  if (info->pass == loc_vpass && (undef & ~info->undefined) == 0)
	    return;
	  info->pass = loc_vpass;
	  info->undefined = undef;
	  /* All but the last predecessor are handled recursively.  */
	  for (; e->pred_next; e = e->pred_next)
	    {
	      if (live_out (df, use, e->src->end))
	        live_in (df, use, e->src->end);
	      use->undefined = undef;
	    }
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
  unsigned int bl;
  int b, e;
  use->undefined = (unsigned HOST_WIDE_INT) 0;
  bl = rtx_to_bits (use->x);
  b = BYTE_BEGIN (bl);
  e = b + BYTE_LENGTH (bl);
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
  int b;

  /* Setup copy cache, for copy_insn_p ().  */
  copy_cache = (struct copy_p_cache *)
    xcalloc (get_max_uid (), sizeof (copy_cache[0]));
  number_seen = (int *) xcalloc (get_max_uid (), sizeof (int));
  visit_trace = (struct visit_trace *) xcalloc (get_max_uid (),
					      sizeof (visit_trace[0]));

  for (b = 0; b < n_basic_blocks + 2; b++)
    {
      basic_block bb = (b == n_basic_blocks) ? ENTRY_BLOCK_PTR :
	  (b == n_basic_blocks + 1) ? EXIT_BLOCK_PTR :
	  BASIC_BLOCK (b);
      struct bb_begin_info *info = (struct bb_begin_info *) xmalloc (sizeof
								     *info);
      info->old_aux = bb->aux;
      bb->aux = (void *)info;
    }
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
  for (b = 0; b < n_basic_blocks + 2; b++)
    {
      basic_block bb = (b == n_basic_blocks) ? ENTRY_BLOCK_PTR :
	  (b == n_basic_blocks + 1) ? EXIT_BLOCK_PTR :
	  BASIC_BLOCK (b);
      struct bb_begin_info *info = (struct bb_begin_info *) bb->aux;
      bb->aux = info->old_aux;
      free (info);
    }
  free (visit_trace);
  free (number_seen);
  free (copy_cache);
  /* Catch prohibited uses of copy_insn_p () early.  */
  copy_cache = NULL;
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

      /* Look, if the constraints apply to a pseudo reg, and not to
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
	  while (link && link->ref && DF_REF_REAL_REG (link->ref) != reg)
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
      reg = DF_REF_REG (link->ref);
      if (GET_CODE (reg) == SUBREG)
	web = find_subweb (web, reg);

      /* Find the constraints, noting the allowed hardregs in allowed.  */
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
	 are not allowed by the constraints.  */
      if (nothing_allowed)
	{
	  /* If we had no real constraints nothing was explicitely
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
	  /* We can't yet establish these conflicts.  Reload must go first
	     (or better said, we must implement some functionality of reload).
	     E.g. if some operands must match, and they need the same color
	     we don't see yet, that they do not conflict (because they match).
	     For us it looks like two normal references with different DEFs,
	     so they conflict, and as they both need the same color, the
	     graph becomes uncolorable.  */
#if 0
	  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
	    if (TEST_HARD_REG_BIT (conflict, c))
	      record_conflict (web, hardreg2web[c]);
#endif
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
  web->spill_cost = 0;
  web->was_spilled = -1;
  web->is_coalesced = 0;
  web->has_sub_conflicts = 0;
  web->artificial = 0;
  web->num_defs = 0;
  web->num_uses = 0;
  web->orig_x = reg;
  web->subreg_next = web;
  web->conflict_list = NULL;
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
      if (!web->num_freedom)
	abort();
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
	&& SUBREG_BYTE (w->orig_x) == SUBREG_BYTE (reg))
      return w;
  return NULL;
}

static struct web *
find_subweb_2 (web, size, word)
     struct web *web;
     unsigned int size, word;
{
  struct web *w = web;
  if (size == GET_MODE_SIZE (GET_MODE (web->orig_x)))
    {
      if (word != 0)
	abort ();
      return web;
    }
  for (w = web->subreg_next; w != web; w = w->subreg_next)
    if (size == GET_MODE_SIZE (GET_MODE (w->orig_x))
        && word == BYTE_BEGIN (rtx_to_bits (w->orig_x)))
      return w;
  return NULL;
}

static struct web *
find_web_for_subweb (subweb)
     struct web *subweb;
{
  for (; GET_CODE (subweb->orig_x) != REG; subweb = subweb->subreg_next);
  return subweb;
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

static struct web *
add_subweb_2 (web, size, word)
     struct web *web;
     unsigned int size, word;
{
  /* To get a correct mode for the to be produced subreg, we don't want to
     simply do a mode_for_size() for the mode_class of the whole web.  
     Suppose we deal with a CDImode web, but search for a 8 byte part.  
     Now mode_for_size() would only search in the class MODE_COMPLEX_INT
     and would find CSImode which probably is not what we want.  Instead
     we want DImode, which is in a completely other class.  For this to work
     we instead first search the already existing subwebs, and take
     _their_ modeclasses as base for a search for ourself.  */
  struct web *w = web->subreg_next;
  enum machine_mode mode = mode_for_size (size * BITS_PER_UNIT, GET_MODE_CLASS
					  (GET_MODE (w->orig_x)), 0);
  if (mode == BLKmode)
    mode = mode_for_size (size * BITS_PER_UNIT, MODE_INT, 0);
  if (mode == BLKmode)
    abort ();
  w = add_subweb (web, gen_rtx_SUBREG (mode, web->orig_x, word));
  w->artificial = 1;
  return w;
}

/* Initialize all the web parts we are going to need.  */
static void
init_web_parts (df)
     struct df *df;
{
  int regno;
  unsigned int no;
  num_webs = 0;
  for (no = 0; no < df->def_id; no++)
    {
      if (df->defs[no])
	{
	  web_parts[no].ref = df->defs[no];
	  num_webs++;
	}
    }
  for (no = 0; no < df->use_id; no++)
    {
      if (df->uses[no])
	{
	  web_parts[no + df->def_id].ref = df->uses[no];
	  num_webs++;
	}      
    }

  /* We want to have only one web for each precolored register.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    {
      struct web_part *r1 = NULL;
      struct df_link *link;
      /* Here once was a test, if there is any DEF at all, and only then to
	 merge all the parts.  This was incorrect, we really also want to have
	 only one web-part for hardregs, even if there is no explicit DEF.  */
      /* Link together all defs...  */
      for (link = df->regs[regno].defs; link; link = link->next)
        if (link->ref)
	  {
	    struct web_part *r2 = &web_parts[DF_REF_ID (link->ref)];
	    if (!r1)
	      r1 = r2;
	    else
	      r1 = union_web_parts (r1, r2);
	  }
      /* ... and all uses.  */
      for (link = df->regs[regno].uses; link; link = link->next)
	if (link->ref)
	  {
	    struct web_part *r2 = &web_parts[df->def_id
		                             + DF_REF_ID (link->ref)];
	    if (!r1)
	      r1 = r2;
	    else
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

/* Called only when REG1 and REG2 refer to the same regno.
   REG1 and REG2 are REG's or SUBREG's.  This returns nonzero,
   when REG1 completely covers REG2.  */
static int
regs_overlap_p (reg1, reg2)
     rtx reg1, reg2;
{
  if (GET_CODE (reg1) == SUBREG)
    {
      if (GET_CODE (reg2) == REG)
	/* A SUBREG can't cover a complete REG (at least not in the
	   reg-allocator, as we don't allow paradoxical subregs).  */
	return 0;
    }
  else
    /* reg1 is REG, reg2 SUBREG or REG, so reg1 must cover reg2.  */
    return 1;
  /* We come here, when reg1 and reg2 are both SUBREGs.  */
  if (GET_MODE_SIZE (GET_MODE (reg1)) < GET_MODE_SIZE (GET_MODE (reg2)))
    return 0;
  /* Note, that the check below also would detect partial overlap, but all
     mode sizes should add up, so in effect this is a complete overlap (or
     none at all).  */
  {
    unsigned int bl1, bl2;
    int b1, b2, e1, e2;
    bl1 = rtx_to_bits (reg1);
    bl2 = rtx_to_bits (reg2);
    b1 = BYTE_BEGIN (bl1);
    b2 = BYTE_BEGIN (bl2);
    e1 = b1 + BYTE_LENGTH (bl1) - 1;
    e2 = b2 + BYTE_LENGTH (bl2) - 1;
    if (b1 > e2 || b2 > e1)
      return 0;
    else
      return 1;
  }
}

/* Possibly add an edge from web FROM to TO marking a conflict between
   those two.  This is one half of marking a complete conflict, which notes
   in FROM, that TO is a conflict.  Adding TO to FROM's conflicts might
   make other conflicts superflous, because the current TO overlaps some web
   already being in conflict with FROM.  In this case the smaller webs are
   deleted from the conflict list.  Likewise if TO is overlapped by a web
   already in the list, it isn't added at all.  Note, that this can only
   happen, if SUBREG webs are involved.  */
static void
add_conflict_edge (from, to)
     struct web *from, *to;
{
  if (from->type != PRECOLORED)
    {
      struct web *pfrom = find_web_for_subweb (from);
      struct web *pto = find_web_for_subweb (to);
      struct sub_conflict *sl;
      struct conflict_link *cl = pfrom->conflict_list;
      int may_delete = 1;
      
      /* This can happen when subwebs of one web conflict with each
	 other.  In live_out_1() we created such conflicts between yet
	 undefined webparts and defs of parts which didn't overlap with the
	 undefined bits.  Then later they nevertheless could have merged into
	 one web, and then we land here.  */
      if (pfrom == pto)
	return;
      if (!TEST_BIT (sup_igraph, (pfrom->id*num_webs + pto->id)))
	{
	  cl = (struct conflict_link *) xmalloc (sizeof (*cl));
	  cl->t = pto;
	  cl->sub = NULL;
	  cl->next = pfrom->conflict_list;
	  pfrom->conflict_list = cl;
	  pfrom->num_conflicts += 1 + pto->add_hardregs;
          SET_BIT (sup_igraph, (pfrom->id * num_webs + pto->id));
	  may_delete = 0;
	}
      /* We don't need to test for cl==NULL, because at this point
	 a cl with cl->t==pto is guaranteed to exist.  */
      while (cl->t != pto)
	cl = cl->next;
      if (pfrom != from || pto != to)
	{
	  /* This is a subconflict which should be added.
	     If we inserted cl in this incocation, we really need to add this
	     subconflict.  If we did _not_ add it here, we only add the
	     subconflict, if cl already had subconclicts, because otherwise
	     this indicated, that the whole webs already conflict, which
	     means we are not interested in this subconflict.  */
	  if (!may_delete || cl->sub != NULL)
	    {
	      sl = (struct sub_conflict *) xmalloc (sizeof (*sl));
	      sl->s = from;
	      sl->t = to;
	      sl->next = cl->sub;
	      cl->sub = sl;
	    }
	}
      else
	{
	  /* pfrom == from && pto == to means, that we are not interested
	     anymore in the subconflict list for this pair, because anyway
	     the whole webs conflict.  */
	  struct sub_conflict *sl_next;
	  for (sl = cl->sub; sl; sl = sl_next)
	    {
	      sl_next = sl->next;
	      free (sl);
	    }
	  cl->sub = NULL;
	}
    }
}

/* Record a conflict between two webs, if we haven't recorded it
   already.  */
static void
record_conflict (web1, web2)
     struct web *web1, *web2;
{
  unsigned int id1 = web1->id, id2 = web2->id;
  unsigned int index = igraph_index (id1, id2);
  /* Trivial non-conflict or already recorded conflict.  */
  if (web1 == web2 || TEST_BIT (igraph, index))
    return;
  if (id1 == id2)
    abort ();
  /* As fixed_regs are no targets for allocation, conflicts with them
     are pointless.  */
  if ((web1->regno < FIRST_PSEUDO_REGISTER && fixed_regs[web1->regno])
      || (web2->regno < FIRST_PSEUDO_REGISTER && fixed_regs[web2->regno]))
    return;
  /* Conflicts with hardregs, which are not even a candidate
     for this pseudo are also pointless.  The same if the set of allowed
     hardregs for two pseudos have no hardreg in common.  */
  if (! ((web1->type == PRECOLORED 
	  && TEST_HARD_REG_BIT (web2->usable_regs, web1->regno))
	 || (web2->type == PRECOLORED
	     && TEST_HARD_REG_BIT (web1->usable_regs, web2->regno))
	 || hard_regs_intersect_p (&web1->usable_regs, &web2->usable_regs)))
    return;
  SET_BIT (igraph, index);
  add_conflict_edge (web1, web2);
  add_conflict_edge (web2, web1);
  
}

/* This builds full webs out of web parts, without relating them to each
   other (i.e. without creating the conflict edges).  */
static void
parts_to_webs (df, part2web)
     struct df *df;
     struct web **part2web;
{
  unsigned int i;
  int j;
  unsigned int webnum;
  struct ref **ref_use, **ref_def;
  struct web_part *wp_first_use = &web_parts[df->def_id];
  struct web_link *all_subwebs = NULL;
  struct web_link *wl;
  num_subwebs = 0;

  memset (hardreg2web, 0, sizeof (hardreg2web));
  /* For each root web part: create and initialize a new web,
     setup def2web[] and use2web[] for all defs and uses, and
     id2web for all new webs.  */
  id2web = (struct web **) xcalloc (num_webs + FIRST_PSEUDO_REGISTER,
				    sizeof (struct web *));

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
	  web->crosses_call = wp->crosses_call;
	  id2web[webnum] = web;
	  webnum++;
	  if (web->regno < FIRST_PSEUDO_REGISTER)
	    hardreg2web[web->regno] = web;
	}
      else
	{
	  int j = DF_REF_ID (rwp->ref);
	  if (rwp < wp_first_use)
	    web = part2web[j];
	  else
	    web = part2web[j + df->def_id];
	}
      part2web[i] = web;
      if (GET_CODE (reg) == SUBREG && !find_subweb (web, reg))
	{
	  struct web *subweb = add_subweb (web, reg);
	  struct web_link *wl;
	  subweb->id = num_webs + num_subwebs;
	  wl = (struct web_link *) xmalloc (sizeof (struct web_link));
	  wl->web = subweb;
	  wl->next = all_subwebs;
	  all_subwebs = wl;
	  num_subwebs++;
	}
      web->weight += DF_REF_BB (ref)->frequency;
      web->weight += web->span_insns;
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

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (!hardreg2web[i])
      {
	struct web *web = (struct web *) xcalloc (1, sizeof (struct web));
	init_one_web (web, gen_rtx_REG (reg_raw_mode[i], i));
	web->id = webnum;
	id2web[webnum] = web;
	webnum++;
	hardreg2web[web->regno] = web;
      }
  num_webs = webnum;

  /* Now create all remaining artificial subwebs, i.e. those, which do
     not correspond to a real subreg in the current function's RTL, but
     which nevertheless is a target of a conflict.
     XXX we need to merge this loop with the one above, which means, we need
     a way to later override the artificiality.  Beware: currently
     add_subweb_2() relies on the existence of normal subwebs for deducing
     a sane mode to use for the artificial subwebs.  */
  for (i = 0; i < df->def_id + df->use_id; i++)
    {
      struct web_part *wp = &web_parts[i];
      struct tagged_conflict *cl = wp->sub_conflicts;
      struct web *web = part2web[i];
      if (wp->uplink)
	{
	  if (wp->sub_conflicts)
	    abort ();
	  continue;
	}
      for (; cl; cl = cl->next)
        if (!find_subweb_2 (web, cl->size, cl->word))
	  {
	    struct web *subweb = add_subweb_2 (web, cl->size, cl->word);
	    struct web_link *wl;
	    wl = (struct web_link *) xmalloc (sizeof (struct web_link));
	    wl->web = subweb;
	    wl->next = all_subwebs;
	    all_subwebs = wl;
	    num_subwebs++;
	  }
    }

  id2web = (struct web **) xrealloc (id2web, (num_webs + num_subwebs) *
				     sizeof (struct web *));
  for (wl = all_subwebs; wl;)
    {
      id2web[webnum] = wl->web;
      wl->web->id = webnum;
      webnum++;
      wl = wl->next;
      free (all_subwebs);
      all_subwebs = wl;
    }
  num_webs += num_subwebs;
  igraph = sbitmap_alloc (num_webs * num_webs / 2);
  sup_igraph = sbitmap_alloc (num_webs * num_webs);
  unbrokengraph = BITMAP_XMALLOC ();
  sbitmap_zero (igraph);
  sbitmap_zero (sup_igraph);
  bitmap_zero (unbrokengraph);

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
      if (df->defs[i])
	{
	  def2web[i]->defs[0] = df->defs[i];
	  def2web[i]->defs++;
	}
    }
  for (i = 0; i < df->use_id; i++)
    {
      if (df->uses[i])
	{
	  use2web[i]->uses[0] = df->uses[i];
	  use2web[i]->uses++;
	}
    }
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = id2web[i];
      web->defs -= web->num_defs;
      web->uses -= web->num_uses;
      web->weight *= (1 + web->add_hardregs);
      for (j = 0; j < web->num_uses; j++)
	{
	  if (rematerializable (web->uses[j]))
	    web->spill_cost += rtx_cost (DF_REF_INSN (web->uses[j]), 0);
	  else
	    web->spill_cost += MEMORY_MOVE_COST (GET_MODE (*DF_REF_LOC (web->uses[j])), 
						   web->regclass, 1);

	}
      for (j = 0; j < web->num_defs; j++)
	web->spill_cost += MEMORY_MOVE_COST (GET_MODE (*DF_REF_LOC (web->defs[j])), 
					     web->regclass, 0);
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
  struct tagged_conflict *cl;

  /* Now record all conflicts between webs.  */
  for (i = 0; i < df->def_id + df->use_id; i++)
    for (cl = web_parts[i].sub_conflicts; cl;)
      {
	struct tagged_conflict *cl_next = cl->next;
	if (cl->conflicts)
	  {
	    /* Without SUBREGs only web roots have .conflicts != 0, because we
	       were able to merge all conflicts when unioning the parts.  So
	       in that case the find_web_part() wouldn't be necessary, but
	       unfortunately with SUBREGs also non-roots can still have
	       conflicts so we need to test them all.  OTOH we need the root
	       to check, if we aren't dealing with an uninitialized web (which
	       is the case, if the root is itself a USE instead of a DEF).  */
	    struct web_part *rwp = find_web_part (&web_parts[i]);
	    int j;

	    if (rwp != &web_parts[i])
	      abort ();

	    if (rwp < wp_first_use)
	      {
		struct web *web1 = part2web[i];
		/* XXX I don't like the find_subweb() here.  It's just needed,
		   because parts2web[] notes the web (and not the subweb, if
		   that part really only means this), because in
		   parts_to_webs() part2web[] is used for merging information,
		   and therefore needs to resolve to the overall web.
		   May be use another array?  */
		web1 = find_subweb_2 (web1, cl->size, cl->word);
		/* Note, that there are only defs in the conflicts bitset.  */
		EXECUTE_IF_SET_IN_BITMAP (
		  cl->conflicts, 0, j,
		  {
		    struct web *web2 = part2web[j];
		    rtx reg2 = DF_REF_REG (web_parts[j].ref);
		    if (GET_CODE (reg2) == SUBREG)
		      web2 = find_subweb (web2, reg2);
		    record_conflict (web1, web2);
		    if (!rematerializable (web_parts[j].ref) && (DF_REF_REGNO (web_parts[j].ref) >= FIRST_PSEUDO_REGISTER))
		      {
			if (web1->id != web2->id)
			  bitmap_set_bit (unbrokengraph, (web1->id * num_webs) + web2->id);
		      }
		  });
	      }
	    BITMAP_XFREE (cl->conflicts);
	  }
	free (cl);
	cl = cl_next;
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
     many other things, but at some time we want to integrate that
     functionality into the allocator.  */
  COPY_HARD_REG_SET (web->usable_regs, reg_class_contents[(int) ALL_REGS]);
  AND_COMPL_HARD_REG_SET (web->usable_regs, never_use_colors);
  prune_hardregs_for_mode (&web->usable_regs, PSEUDO_REGNO_MODE (web->regno));
  web->num_freedom = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (TEST_HARD_REG_BIT (web->usable_regs, i))
      web->num_freedom++;
  if (!web->num_freedom)
      abort();
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
      struct conflict_link *wl;
      web->num_conflicts -= adjust;
      for (wl = web->conflict_list; wl; wl = wl->next)
	wl->t->num_conflicts -= adjust;
    }
}

static void
detect_spill_temps (void)
{
  unsigned int no;

  /* Detect webs used for spill temporaries.  */
  for (no = 0; no < num_webs - num_subwebs; no++)
    {
      struct web *web = id2web[no];

      /* Below only the detection of spill temporaries.  We never spill
         precolored webs, so those can't be spill temporaries.  The code above
         (remember_web_was_spilled) can't currently cope with hardregs
         anyway.  */
      if (web->regno < FIRST_PSEUDO_REGISTER)
	continue;

      /* A spill temporary has one def, one or more uses, all uses
	 are in one insn, and either the def or use insn was inserted
	 by the allocator.  */
      /* XXX not correct currently.  There might also be spill temps
	 involving more than one def.  Usually that's an additional
	 clobber in the using instruction.  We might also constrain
	 ourself to that, instead of like currently marking all
	 webs involving any spill insns at all.  */
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
	  /* Delete this move.  */
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
      reg = DF_REF_REAL_REG (wp1->ref);
      link = DF_INSN_DEFS (df, DF_REF_INSN (wp1->ref));
      for (; link; link = link->next)
        if (reg == DF_REF_REAL_REG (link->ref))
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
        if (def1 != def2 && TEST_BIT (igraph, igraph_index (def1, def2)))
	  {
	    if (num1 == num)
	      {
	        if (SUBWEB_P (id2web[def1]))
		  debug_msg (0, "%d (SUBREG %d, %d) with ", def1,
			     id2web[def1]->regno,
			     SUBREG_BYTE (id2web[def1]->orig_x));
	        else
	          debug_msg (0, "%d (REG %d) with ", def1,
			     id2web[def1]->regno);
	      }
	    if ((num2 % 9) == 8)
	      debug_msg (0, "\n              ");
	    num++;
	    num2++;
	    if (SUBWEB_P (id2web[def2]))
	      debug_msg (0, "%d(%d,%d) ", def2, id2web[def2]->regno,
			 SUBREG_BYTE (id2web[def2]->orig_x));
	    else
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
  struct conflict_link *wl, *wl_next;
  struct move_list *ml, *ml_next;
  unsigned int i;
  
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = id2web[i];
      for (wl = web->conflict_list; wl; wl = wl_next)
	{
	  struct sub_conflict *sl, *sl_next;
	  for (sl = wl->sub; sl; sl = sl_next)
	    {
	      sl_next = sl->next;
              free (sl);
	    }
	  wl_next = wl->next;
	  free (wl);
	}
      for (ml = web->moves; ml; ml = ml_next)
	{
	  ml_next = ml->next;
	  free (ml);
	}
      web->conflict_list = NULL;
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
  free (use2web);
  free (def2web);
  free (id2web);
  free (web_parts);
  web_parts = NULL;
  free (move_handled);
  sbitmap_free (sup_igraph);
  sbitmap_free (igraph);
  BITMAP_XFREE (unbrokengraph);
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
static struct dlist *select_stack, *simplify_fat_wl;

static struct dlist *mv_worklist, *mv_coalesced, *mv_constrained;
static struct dlist *mv_frozen, *mv_active;

static void
free_all_lists (void)
{
  free_dlist (&simplify_wl);
  free_dlist (&simplify_spilled_wl);
  free_dlist (&simplify_fat_wl);
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
	else if (web->add_hardregs)
	  push_list (web->dlink, &simplify_fat_wl);
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
  for (i = 0; i < num_webs - num_subwebs; i++)
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

/* Enable the active moves, in which WEB takes part, to be processed.  */
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
      struct conflict_link *a;
      enable_move (web);
      for (a = web->conflict_list; a; a = a->next)
	{
	  struct web *aweb = a->t;
	  if (aweb->type != SELECT && aweb->type != COALESCED)
	    enable_move (aweb);
	}
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
  struct conflict_link *wl;
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
      else if (simplify_fat_wl)
	d = pop_list (&simplify_fat_wl);
      else
	d = pop_list (&simplify_spilled_wl);
      if (!d)
	break;
      web = DLIST_WEB (d);
      debug_msg (0, " simplifying web %3d, conflicts = %d\n", web->id,
		 web->num_conflicts);
      put_web (web, SELECT);
      for (wl = web->conflict_list; wl; wl = wl->next)
	{
	  struct web *pweb = wl->t;
	  if (pweb->type != SELECT && pweb->type != COALESCED)
	    {
	      decrement_degree (pweb, 1 + web->add_hardregs);
	    }
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
  struct conflict_link *wl;
  int i;
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

  /* We can't coalesce target with the precolored registers which isn't in
     usable_regs.  */
  for (i = target->add_hardregs; i >= 0; --i)
    if (!TEST_HARD_REG_BIT (usable_regs[target->regclass], source->color + i))
      return 0;

  for (wl = target->conflict_list; wl; wl = wl->next)
    {
      struct web *pweb = wl->t;
      if (pweb->type == SELECT || pweb->type == COALESCED)
	continue;

      /* Coalescing target (T) and source (S) is o.k, if for
	 all conflicts C of T it is true, that:
	  1) C will be colored, or
	  2) C is a hardreg (precolored), or
	  3) C already conflicts with S too, or
	  4) a web which contains C conflicts already with S.
	 XXX: we handle here only the special case of 4), that C is
	 a subreg, and the containing thing is the reg itself, i.e.
	 we dont handle the situation, were T conflicts with
	 (subreg:SI x 1), and S conflicts with (subreg:DI x 0), which
	 would be allowed also, as the S-conflict overlaps
	 the T-conflict.
         So, we first test the whole web for any of these conditions, and
         continue with the next C, if 1, 2 or 3 is true.  */
      if (pweb->num_conflicts < NUM_REGS (pweb)
	  || pweb->type == PRECOLORED
	  || TEST_BIT (igraph, igraph_index (source->id, pweb->id)) )
	continue;

      /* This is reached, if not one of 1, 2 or 3 was true.  In the case C has
         no subwebs, 4 can't be true either, so we can't coalesce S and T.  */
      if (wl->sub == NULL)
        return 0;
      else
	{
	  /* The main webs do _not_ conflict, only some parts of both.  This
	     means, that 4 is possibly true, so we need to check this too.
	     For this we go thru all sub conflicts between T and C, and see if
	     the target part of C already conflicts with S.  When this is not
	     the case we disallow coalescing.  */
	  struct sub_conflict *sl;
	  for (sl = wl->sub; sl; sl = sl->next)
	    {
              if (!TEST_BIT (igraph, igraph_index (source->id, sl->t->id)))
	        return 0;
	    }
        }
    }
  return 1;
}

/* Non-precolored node coalescing heuristic.  */
static int
conservative (target, source)
     struct web *target, *source;
{
  unsigned int k;
  unsigned int loop;
  regset seen;
  struct conflict_link *wl;
  unsigned int num_regs = NUM_REGS (target); /* XXX */

  /* k counts the resulting conflict weight, if target and source
     would be merged, and all low-degree neighbors would be
     removed.  */
  k = MAX (target->add_hardregs, source->add_hardregs);
  seen = BITMAP_XMALLOC ();
  for (loop = 0; loop < 2; loop++)
    for (wl = ((loop == 0) ? target : source)->conflict_list;
	 wl; wl = wl->next)
      {
	struct web *pweb = wl->t;
	if (pweb->type != SELECT && pweb->type != COALESCED
	    && pweb->num_conflicts >= NUM_REGS (pweb)
	    && ! REGNO_REG_SET_P (seen, pweb->id))
	  {
	    SET_REGNO_REG_SET (seen, pweb->id);
	    k += 1 + pweb->add_hardregs;
	  }
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
  struct conflict_link *wl;
  if (v->type == FREEZE)
    remove_list (v->dlink, &freeze_wl);
  else
    remove_list (v->dlink, &spill_wl);
  put_web (v, COALESCED);
  v->alias = u;
  u->is_coalesced = 1;
  v->is_coalesced = 1;
  merge_moves (u, v);
  /* combine add_hardregs's of U and V.  */
  if (u->type == PRECOLORED)
    {
      int i;
      struct web *web;

      for (i = 0; i <= v->add_hardregs; ++i)
	{
	  web = hardreg2web [i + u->color];
	  for (wl = v->conflict_list; wl; wl = wl->next)
	    {
	      struct web *pweb = wl->t;
	      if (pweb->type != SELECT && pweb->type != COALESCED)
		{
		  if (wl->sub == NULL)
		    record_conflict (pweb, web);
		  else
		    {
		      struct sub_conflict *sl;
		      for (sl = wl->sub; sl; sl = sl->next)
			record_conflict (u, sl->t);
		    }
		}
	    }
	}

      for (wl = v->conflict_list; wl; wl = wl->next)
	{
	  struct web *pweb = wl->t;
	  if (pweb->type != SELECT && pweb->type != COALESCED)
	    decrement_degree (pweb, 1 + v->add_hardregs);
	}
    }
  else
    for (wl = v->conflict_list; wl; wl = wl->next)
      {
	struct web *pweb = wl->t;
	if (pweb->type != SELECT && pweb->type != COALESCED)
	  {
	    if (wl->sub == NULL)
	      record_conflict (u, pweb);
	    else
	      {
		struct sub_conflict *sl;
		/* So, between V and PWEB there are sub_conflicts.  We need
		   to relocate those conflicts to be between U and PWEB.
		   In the case only a part of V conflicted with (part of) PWEB
		   we nevertheless make the new conflict between the whole U
		   and the (part of) PWEB.  Later we might try to find in U the
		   correct subpart corresponding (by size and offset) to the
		   part of V (sl->s) which was the source of the conflict.  */
		for (sl = wl->sub; sl; sl = sl->next)
		  record_conflict (u, sl->t);
	      }
	    decrement_degree (pweb, 1 + v->add_hardregs);
	  }
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
	   || TEST_BIT (sup_igraph, source->id * num_webs + target->id)
	   || TEST_BIT (sup_igraph, target->id * num_webs + source->id))
    {
      remove_move (source, m);
      remove_move (target, m);
      put_move (m, CONSTRAINED);
      add_worklist (source);
      add_worklist (target);
    }
  else if ((source->type == PRECOLORED && ok (target, source))
	   || (source->type != PRECOLORED /*&& conservative (target, source) */))
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
  /*  freeze_moves (DLIST_WEB (d)); */
}

static unsigned HOST_WIDE_INT (*spill_heuristic) PARAMS ((struct web *));

static unsigned HOST_WIDE_INT default_spill_heuristic PARAMS ((struct web *));

static unsigned HOST_WIDE_INT
default_spill_heuristic (w)
     struct web *w;
{
  return (w->weight * w->spill_cost) / (w->num_conflicts + 1);
}

/* Select the cheapest spill to be potentially spilled (we don't
   *actually* spill until we need to).  */
static void
select_spill (void)
{
  unsigned HOST_WIDE_INT best = (unsigned HOST_WIDE_INT) -1;
  struct dlist *bestd = NULL;
  unsigned HOST_WIDE_INT best2 = (unsigned HOST_WIDE_INT) -1;
  struct dlist *bestd2 = NULL;
  struct dlist *d;
  for (d = spill_wl; d; d = d->next)
    {
      struct web *w = DLIST_WEB (d);
      unsigned HOST_WIDE_INT cost = spill_heuristic (w);
      if (!w->spill_temp && cost < best)
	{
	  best = cost;
	  bestd = d;
	}
      else if (w->spill_temp == 2 && cost < best2)
	{
	  best2 = cost;
	  bestd2 = d;
	}
    }
  if (!bestd)
    {
      bestd = bestd2;
      best = best2;
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
int
get_free_reg (dont_begin_colors, free_colors, mode)
     HARD_REG_SET dont_begin_colors, free_colors;
     enum machine_mode mode;
{
  int c;
  int last_resort_reg = -1;
  int pref_reg = -1;
  int pref_reg_order = INT_MAX;
  int last_resort_reg_order = INT_MAX;
  
  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
    if (!TEST_HARD_REG_BIT (dont_begin_colors, c)
	&& TEST_HARD_REG_BIT (free_colors, c)
	&& HARD_REGNO_MODE_OK (c, mode))
      {
	int i, size;
	size = HARD_REGNO_NREGS (c, mode);
	for (i = 1; i < size && TEST_HARD_REG_BIT (free_colors, c + i); i++);
	if (i != size)
 	  {
 	    c += i;
 	    continue;
 	  }
 	for (i = 1; i < size && HARD_REGNO_MODE_OK (c + i, mode); i++); 
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
static char *
hardregset_to_string (s)
     HARD_REG_SET s;
{
  static char string[/*FIRST_PSEUDO_REGISTER + 30*/1024];
#if FIRST_PSEUDO_REGISTER <= HOST_BITS_PER_WIDE_INT
  sprintf (string, "%x", s);
#else
  char *c = string;
  int i,j;
  c += sprintf (c, "{ ");
  for (i = 0;i < HARD_REG_SET_LONGS; i++)
    {
      for (j = 0; j < HOST_BITS_PER_WIDE_INT; j++)
	  c += sprintf (c, "%s", ( 1 << j) & s[i] ? "1" : "0");
      c += sprintf (c, "%s", i ? ", " : "");
    }
  c += sprintf (c, " }");
#endif
  return string;
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
colorize_one_web (web, hard)
     struct web *web;
     int hard;
{
  struct conflict_link *wl;
  HARD_REG_SET colors, conflict_colors;
  int c = -1;
  int bestc = -1;
  int neighbor_needs= 0;
  struct web *fat_neighbor = NULL;
  struct web *fats_parent = NULL;
  int num_fat = 0;
  int long_blocks = 0;
  int best_long_blocks = -1;
  HARD_REG_SET fat_colors;
  
  /* The bits set in conflict_colors correspond to the hardregs, at which
     WEB may not begin.  This differs from the set of _all_ hardregs which
     are taken by WEB's conflicts in the presence of wide webs, where only
     some parts conflict with others.  */
  CLEAR_HARD_REG_SET (conflict_colors);
  neighbor_needs = web->add_hardregs + 1;
  for (wl = web->conflict_list; wl; wl = wl->next)
    {
      struct web *w;
      struct web *ptarget = alias (wl->t);
      struct sub_conflict *sl = wl->sub;
      w = sl ? sl->t : wl->t;
      while (w)
	{
	  if (ptarget->type == COLORED || ptarget->type == PRECOLORED)
	    {
	      struct web *source = (sl) ? sl->s : web;
	      unsigned int tsize = HARD_REGNO_NREGS (ptarget->color,
						     GET_MODE (w->orig_x));
	      /* ssize is only a first guess for the size.  */
	      unsigned int ssize = HARD_REGNO_NREGS (ptarget->color, GET_MODE
					             (source->orig_x));
	      unsigned int tofs = 0;
	      unsigned int sofs = 0;
	      /* C1 and C2 can become negative, so unsigned
		 would be wrong.  */
	      int c1, c2;

	      if (SUBWEB_P (w)
		  && GET_MODE_SIZE (GET_MODE (w->orig_x)) >= UNITS_PER_WORD)
		tofs = (SUBREG_BYTE (w->orig_x) / UNITS_PER_WORD);
	      if (SUBWEB_P (source)
		  && GET_MODE_SIZE (GET_MODE (source->orig_x))
		     >= UNITS_PER_WORD)
		  sofs = (SUBREG_BYTE (source->orig_x) / UNITS_PER_WORD);
	      c1 = ptarget->color + tofs - sofs - ssize + 1;
	      c2 = ptarget->color + tofs + tsize - 1 - sofs;
	      if (c2 >= 0)
		{
		  if (c1 < 0)
		    c1 = 0;
		  /* Because ssize was only guessed above, which influenced our
		     begin color (c1), we need adjustment, if for that color
		     another size would be needed.  This is done by moving
		     c1 to a place, where the last of sources hardregs does not
		     overlap the first of targets colors.  */
		  while (c1 + sofs
			 + HARD_REGNO_NREGS (c1, GET_MODE (source->orig_x)) - 1
			 < ptarget->color + tofs)
		    c1++;
		  while (c1 > 0 && c1 + sofs
			 + HARD_REGNO_NREGS (c1, GET_MODE (source->orig_x)) - 1
			 > ptarget->color + tofs)
		    c1--;
		  for (; c1 <= c2; c1++)
		    SET_HARD_REG_BIT (conflict_colors, c1);
		}
	    }
	  else if (ptarget->was_spilled < 0
		   && find_web_for_subweb (w)->type != COALESCED
		   && w->add_hardregs >= neighbor_needs)
	    {
	      neighbor_needs = w->add_hardregs;
	      fat_neighbor = w;
	      fats_parent = ptarget;
	      num_fat++;
	    }
	  /* The next if() only gets true, if there was no wl->sub at all, in
	     which case we are only making one go thru this loop with W being
	     a whole web.  */
	  if (!sl)
	    break;
	  sl = sl->next;
	  w = sl ? sl->t : NULL;
	}
    }

  debug_msg (0, "colorize web %d [don't begin at %s]", web->id,
             hardregset_to_string (conflict_colors));
  if (num_fat)
    {
      if (fats_parent->use_my_regs)
        COPY_HARD_REG_SET (fat_colors, fats_parent->usable_regs);
      else
        {
          COPY_HARD_REG_SET (fat_colors, usable_regs
			     [reg_preferred_class (fats_parent->regno)]);
          IOR_HARD_REG_SET (fat_colors, usable_regs
			    [reg_alternate_class (fats_parent->regno)]);
        }
      long_blocks = count_long_blocks (fat_colors, neighbor_needs + 1);
    }

  while (1)
    {
      HARD_REG_SET call_clobbered;
	
      /* Here we choose a hard-reg for the current web.  For non spill
         temporaries we first search in the hardregs for it's prefered
	 class, then, if we found nothing appropriate, in those of the
	 alternate class.  For spill temporaries we only search in
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
      /*AND_COMPL_HARD_REG_SET (colors, conflict_colors);*/
      COPY_HARD_REG_SET (call_clobbered, colors);
      AND_HARD_REG_SET (call_clobbered, call_used_reg_set);

      c = get_free_reg (conflict_colors,
			call_clobbered, PSEUDO_REGNO_MODE (web->regno));
      if (c < 0)
	c = get_free_reg (conflict_colors,
			  colors, PSEUDO_REGNO_MODE (web->regno));
      
      if (!web->use_my_regs && c < 0)
	{
	  IOR_HARD_REG_SET (colors, usable_regs
			    [reg_alternate_class (web->regno)]);
	  /*AND_COMPL_HARD_REG_SET (colors, conflict_colors);*/
	  COPY_HARD_REG_SET (call_clobbered, colors);
	  AND_HARD_REG_SET (call_clobbered, call_used_reg_set);
	  
	  c = get_free_reg (conflict_colors,
			    call_clobbered, PSEUDO_REGNO_MODE (web->regno));
	  if (c < 0)
	    c = get_free_reg (conflict_colors,
			      colors, PSEUDO_REGNO_MODE (web->regno));
	}
      if (c < 0)
	break;
      if (bestc < 0)
        bestc = c;
      /* If one of the yet uncolored neighbors, which is not a potential
	 spill needs a block of hardregs be sure, not to destroy such a block
	 by coloring one reg in the middle.  */
      if (num_fat)
	{
	  int i;
	  int new_long;
	  HARD_REG_SET colors1;
	  COPY_HARD_REG_SET (colors1, fat_colors);
	  for (i = 0; i < 1 + web->add_hardregs; i++)
	    CLEAR_HARD_REG_BIT (colors1, c + i);
	  new_long = count_long_blocks (colors1, neighbor_needs + 1);
	  /* If we changed the number of long blocks, and it's now smaller
	     than needed, we try to avoid this color.  */
	  if (long_blocks != new_long && new_long < num_fat)
	    {
	      if (new_long > best_long_blocks)
		{
		  best_long_blocks = new_long;
		  bestc = c;
		}
	      SET_HARD_REG_BIT (conflict_colors, c);
	      debug_msg (0, " avoid %d", c);
	    }
	  else
	    /* We found a color which doesn't destroy a block.  */
	    break;
	}
      else
	break;
    }
  debug_msg (0, " --> got %d", c < 0 ? bestc : c);
  if (bestc >= 0 && c < 0 && web->was_spilled < 0)
    {
      /* This is a non-potential-spill web, which got a color, which did
	 destroy a hardreg block for one of it's neighbors.  We color
	 this web anyway and hope for the best for the neighbor, if we are
	 a spill temp.  */
      if (1 || web->spill_temp)
        c = bestc;
      debug_msg (0, " [constrains neighbors]");
    }
  debug_msg (0, "\n");

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
      if (hard && (web->was_spilled < 0 || web->spill_temp))
	{
	  unsigned int loop;
	  struct web *try = NULL;

	  debug_msg (0, "  *** %d spilled, although %s ***\n",
		     web->id, web->spill_temp ? "spilltemp" : "non-spill");
	  /* We make two passes over our conflicts, first trying to
	     spill those webs, which only got a color by chance, but
	     were potential spill ones, and if that isn't enough, in a second
	     pass also to spill normal colored webs.  */
	  for (loop = 0; (try == NULL && loop < 2); loop++)
	    for (wl = web->conflict_list; wl; wl = wl->next)
	      {
	        /* Normally we would have w=alias(wl->t), to get to all
		   conflicts.  But we can't simply spill webs which are
		   involved in coalescing anyway.  The premise for combining
		   webs was, that the final one will get a color.  One reason
		   is, that the code inserting the spill insns can't cope
		   with aliased webs (yet, may be, we should extend that).  */
		struct web *w = wl->t;
	        if (w->type == COLORED && !w->spill_temp && !w->is_coalesced
		    && (w->was_spilled > 0 || loop > 0)
		    /*&& w->add_hardregs >= web->add_hardregs
		    && w->span_insns > web->span_insns*/)
		  {
		    try = w;
		    break;
		  }
	      }
	  if (try)
	    {
	      int old_c = try->color;
	      remove_list (try->dlink, &colored_nodes);
	      put_web (try, SPILLED);
	      /* Now try to colorize us again.  Can recursively make other
		 webs also spill, until there are no more unspilled
		 neighbors.  */
	      debug_msg (0, "  trying to spill %d\n", try->id);
	      colorize_one_web (web, hard);
	      if (web->type != COLORED)
		{
		  /* We tried recursively to spill all already colored
		     neighbors, but we are still uncolorable.  So it made
		     no sense to spill those neighbors.  Recolor them.  */
		  remove_list (try->dlink, &spilled_nodes);
		  put_web (try, COLORED);
		  try->color = old_c;
		  debug_msg (0, "  spilling %d was useless\n", try->id);
		}
	      else
		{
		  debug_msg (0, "  to spill %d was a good idea\n", try->id);
		  remove_list (try->dlink, &spilled_nodes);
		  if (try->was_spilled > 0)
		    colorize_one_web (try, 0);
		  else
		    colorize_one_web (try, hard - 1);
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
      /* It's pointless to color webs that have no uses, and in fact,
	 we'll keep spilling them repeatedly, since they have almost
	 no cost to spill (since they have no uses)*/
      if (DLIST_WEB (d)->num_uses > 0)
	colorize_one_web (DLIST_WEB (d), 1);
    }

  for (d = coalesced_nodes; d; d = d->next)
    {
      struct web *a = alias (DLIST_WEB (d));
      DLIST_WEB (d)->color = a->color;
    }
}

static void
spill_coalescing (coalesce, spilled)
     sbitmap coalesce, spilled;
{
  struct move_list *ml;
  struct move *m;
  for (ml = wl_moves; ml; ml = ml->next)
    if ((m = ml->move) != NULL)
      {
	struct web *s = alias (m->source_web);
	struct web *t = alias (m->target_web);
	if ((TEST_BIT (spilled, s->id) && TEST_BIT (coalesce, t->id))
	    || (TEST_BIT (spilled, t->id) && TEST_BIT (coalesce, s->id)))
	  {
	    struct conflict_link *wl;
	    if (TEST_BIT (sup_igraph, s->id * num_webs + t->id)
		|| TEST_BIT (sup_igraph, t->id * num_webs + s->id))
	      continue;

	    PUT_CODE (m->insn, NOTE);
	    NOTE_LINE_NUMBER (m->insn) = NOTE_INSN_DELETED;

	    if (s == t)
	      /* May be, already coalesced due to a former move.  */
	      continue;
	    /* Merge the nodes S and T in the I-graph.  Beware: the merging
	       of conflicts relies on the fact, that in the conflict list
	       of T all of it's conflicts are noted.  This is currently not
	       the case if T would be the target of a coalesced web, because
	       then (in combine () above) only those conflicts were noted in
	       T from the web which was coalesced into T, which at the time
	       of combine() were not already on the SELECT stack or were
	       itself coalsced to something other.  */
	    if (t->type != SPILLED || s->type != SPILLED)
	      abort ();
	    remove_list (t->dlink, &spilled_nodes);
	    put_web (t, COALESCED);
	    t->alias = s;
	    s->is_coalesced = 1;
	    t->is_coalesced = 1;
	    merge_moves (s, t);
	    for (wl = t->conflict_list; wl; wl = wl->next)
	      {
		struct web *pweb = wl->t;
		if (wl->sub == NULL)
		  record_conflict (s, pweb);
		else
		  {
		    struct sub_conflict *sl;
		    for (sl = wl->sub; sl; sl = sl->next)
		      record_conflict (s, sl->t);
		  }
		/* No decrement_degree here, because we already have colored
		   the graph, and don't want to insert pweb into any other
		   list.  */
		pweb->num_conflicts -= 1 + t->add_hardregs;
	      }
	  }
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
    return;
  slot = assign_stack_local (PSEUDO_REGNO_MODE (regno), total_size,
			     inherent_size == total_size ? 0 : -1);
  RTX_UNCHANGING_P (slot) = RTX_UNCHANGING_P (regno_reg_rtx[regno]);
  MEM_ALIAS_SET (slot) = new_alias_set ();
  web->stack_slot = slot;
}

static unsigned int emitted_spill_loads;
static unsigned int emitted_spill_stores;
static unsigned int emitted_remat;

/* Rewrite the program to include the spill code.  */
static void
rewrite_program (void)
{
  unsigned int i;
  bitmap b = BITMAP_XMALLOC ();
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      struct web *web = id2web[i];
      struct web *aweb = alias (web);
      int j;
      rtx slot = 0;
      if (aweb->type != SPILLED/* || web->num_uses == 0*/)
	continue;
      
      for (j = 0; j < web->num_uses; j++)
	{
	  if (!rematerializable (web->uses[j]))
	    { 
	      allocate_spill_web (aweb);
	      slot = aweb->stack_slot;
	      break;

	    }
	} 
      bitmap_clear (b);
      if (slot)
      for (j = 0; j < web->num_defs; j++)
	{
	  rtx insns, source, dest;
	  rtx insn = DF_REF_INSN (web->defs[j]);
	  rtx following = NEXT_INSN (insn);
	  basic_block bb = BLOCK_FOR_INSN (insn);
/*	  if (!rematerializable (web->defs[j]))
	    { */
	      /* Happens when spill_coalescing() deletes move insns.  */
	      if (!INSN_P (insn))
		continue;
	      if (bitmap_bit_p (b, INSN_UID (insn)))
		continue;
	      bitmap_set_bit (b, INSN_UID (insn));
	      start_sequence ();
	      source = DF_REF_REG (web->defs[j]);
	      dest = slot;
	      if (GET_CODE (source) == SUBREG)
		{
		  dest = gen_rtx_MEM (GET_MODE (source),
				      plus_constant (XEXP (dest, 0),
						     SUBREG_BYTE (source)));
		  emit_insn (gen_move_insn (dest, source));
		}
	      else
		{
/*		  rtx reg = gen_reg_rtx (GET_MODE (source));
		  if (validate_change (insn, DF_REF_LOC (web->defs[j]),
				       reg, 0))
		    emit_insn (gen_move_insn (dest, reg));
		  else*/
		    emit_insn (gen_move_insn (dest, source));
		}
		
	      insns = get_insns ();
	      end_sequence ();
	      emit_insns_after (insns, insn);
	      if (bb->end == insn)
		bb->end = PREV_INSN (following);
	      for (; insn != following; insn = NEXT_INSN (insn))
		{
		  set_block_for_insn (insn, bb);
		  df_insn_modify (df, bb, insn);
		}
	      emitted_spill_stores++;
/*	    }
      
	  else
	    {
	      if (rtl_dump_file)
		  fprintf (rtl_dump_file, "We *should* rematerialize uses of def ID %d for web %d\n", DF_REF_ID (web->defs[j]), web->id);
	    } */
	}
      bitmap_clear (b);
      for (j = 0; j < web->num_uses; j++)
	{
	  rtx insns, target, source;
	  rtx insn = DF_REF_INSN (web->uses[j]);
	  rtx prev = PREV_INSN (insn);
	  basic_block bb = BLOCK_FOR_INSN (insn);
	  if (rematerializable (web->uses[j]))
	    {
	      rtx pat = PATTERN (DF_REF_INSN (DF_REF_CHAIN (web->uses[j])->ref));
	      if (!INSN_P (insn))
		continue;
	      if (bitmap_bit_p (b, INSN_UID (insn)))
		continue;
	      bitmap_set_bit (b, INSN_UID (insn));
	      if (!validate_change (insn, DF_REF_LOC (web->uses[j]),
				    SET_SRC (pat), 0))
		{
		  rtx reg = gen_reg_rtx (GET_MODE (SET_DEST (pat)));
		  start_sequence ();
		  if (validate_change (insn, DF_REF_LOC (web->uses[j]), reg, 0))
		      emit_insn (gen_move_insn (reg, SET_SRC (pat)));
		  else
		      emit_insn (gen_move_insn (SET_DEST (pat), SET_SRC (pat)));
		  /*	      emit_insn (PATTERN (DF_REF_INSN
			      (DF_REF_CHAIN (web->uses[j])->ref))); */
		  insns = get_insns ();
		  end_sequence ();
		  emit_insns_before (insns, insn);
		  if (bb->head == insn)
		      bb->head = NEXT_INSN (prev);
		  for (; insn != prev; insn = PREV_INSN (insn))
		    {
		      set_block_for_insn (insn, bb);
		      df_insn_modify (df, bb, insn);
		    }
		}
	      else
		  df_insn_modify (df, bb, insn);
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "Poof! We rematerialized use %d for web %d, associated with def %d\n", 
			 j, web->id, DF_REF_ID (DF_REF_CHAIN (web->uses[j])->ref));
	      emitted_remat++;
	    }
	  else	   
	    {
	      
	      /* Happens when spill_coalescing() deletes move insns.  */
	      if (!INSN_P (insn))
		continue;
	      if (bitmap_bit_p (b, INSN_UID (insn)))
		continue;
	      bitmap_set_bit (b, INSN_UID (insn));
	      start_sequence ();
	      target = DF_REF_REG (web->uses[j]);
	      source = slot;
	      if (GET_CODE (target) == SUBREG)
		{
		  source = gen_rtx_MEM (GET_MODE (target),
					plus_constant (XEXP (source, 0),
						       SUBREG_BYTE (target)));
		  emit_insn (gen_move_insn (target, source));
		}
	      else
		{
/*		  rtx reg = gen_reg_rtx (GET_MODE (target));

		  if (validate_change (insn, DF_REF_LOC (web->uses[j]),
				       reg, 0))
		    emit_insn (gen_move_insn (reg ,source));
		  else*/
		    emit_insn (gen_move_insn (target, source));
		}

	      insns = get_insns ();
	      end_sequence ();
	      emit_insns_before (insns, insn);
	      if (bb->head == insn)
		bb->head = NEXT_INSN (prev);
	      for (; insn != prev; insn = PREV_INSN (insn))
		{
		  set_block_for_insn (insn, bb);
		  df_insn_modify (df, bb, insn);
		}
		
	      emitted_spill_loads++;
	    }
	}

    }

  BITMAP_XFREE (b);

  /*if (! validate_change (insn, df->defs[i]->loc, web->stack_slot, 0)) */
}

static void
actual_spill (void)
{
  sbitmap spilled;
  struct dlist *d;
  spilled = sbitmap_alloc (num_webs);
  sbitmap_zero (spilled);
  for (d = spilled_nodes; d; d = d->next)
    SET_BIT (spilled, DLIST_WEB (d)->id);
  spill_coalescing (spilled, spilled);
  rewrite_program ();
  free (spilled);
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
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      web = id2web[i];
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      if (web->type == COALESCED && alias (web)->type == COLORED)
	continue;
      if (web->reg_rtx || web->regno < FIRST_PSEUDO_REGISTER)
	abort ();
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
     there get the same new pseudo-reg, so order would not matter.
     _However_ we did not connect webs, were the read cycle was an
     uninitialized read.  If we now would first replace the def reference
     and then the use ref, we would initialize it with a REG rtx, which
     gets never initialized, and yet more wrong, which would overwrite
     the definition of the other REG rtx.  So we must replace the defs last.
   */
  for (i = 0; i < df->use_id; i++)
    {     
      regset rs = DF_REF_BB (df->uses[i])->global_live_at_start;
      rtx regrtx;
      web = use2web[i];
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      regrtx = alias (web)->reg_rtx;
      if (!regrtx)
	regrtx = web->reg_rtx;
      *DF_REF_REAL_LOC (df->uses[i]) = regrtx;
      if (REGNO_REG_SET_P (rs, web->regno))
	{
          /*CLEAR_REGNO_REG_SET (rs, web->regno);*/
          SET_REGNO_REG_SET (rs, REGNO (regrtx));
	}
    }

  /* And all defs.  */
  for (i = 0; i < df->def_id; i++)
    {
      regset rs;
      rtx regrtx;
      if (!df->defs[i])
	continue;
      rs = DF_REF_BB (df->defs[i])->global_live_at_start;
      web = def2web[i];
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      regrtx = alias (web)->reg_rtx;
      if (!regrtx)
	regrtx = web->reg_rtx;
      *DF_REF_REAL_LOC (df->defs[i]) = regrtx;
      if (REGNO_REG_SET_P (rs, web->regno))
	{
	  /* Don't simply clear the current regno, as it might be
	     replaced by two webs.  */
          /*CLEAR_REGNO_REG_SET (rs, web->regno);*/
          SET_REGNO_REG_SET (rs, REGNO (regrtx));
	}
    }

  /* And now set up the reg_renumber array for reload with all the new
     pseudo-regs.  */
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      web = id2web[i];
      if (web->reg_rtx)
	{
	  int r = REGNO (web->reg_rtx);
          reg_renumber[r] = web->color;
          debug_msg (0, "Renumber pseudo %d (== web %d) to %d\n",
		     r, web->id, reg_renumber[r]);
	}
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
}


static unsigned int deleted_move_insns;

static void
delete_moves (void)
{
  struct move_list *ml;
  struct web *s, *t;
  /* XXX Beware: We normally would test here each copy insn, if
     source and target got the same color (either by coalescing or by pure
     luck), and then delete it.
     This will currently not work.  One problem is, that we don't color
     the regs ourself, but instead defer to reload.  So the colorization
     is only a kind of suggestion, which reload doesn't have to follow.
     For webs which are coalesced to a normal colored web, we only have one
     new pseudo, so in this case we indeed can delete copy insns involving
     those (because even if reload colors them different from our suggestion,
     it still has to color them the same, as only one pseudo exists).  But for
     webs coalesced to precolored ones, we have not a single pseudo, but
     instead one for each coalesced web.  This means, that we can't delete
     copy insns, where source and target are webs coalesced to precolored
     ones, because then the connection between both webs is destroyed.  Note
     that this not only means copy insns, where one side is the precolored one
     itself, but also those between webs which are coalesced to one color.
     Also because reload we can't delete copy insns which involve any
     precolored web at all.  These often have also special meaning (e.g.
     copying a return value of a call to a pseudo, or copying pseudo to the
     return register), and the deletion would confuse reload in thinkink the
     pseudo isn't needed.  One of those days reload will get away and we can
     do everything we want.
     In effect because of the later reload, we can't base our deletion on the
     colors itself, but instead need to base them on the newly created
     pseudos.  */
  for (ml = wl_moves; ml; ml = ml->next)
    /* The real condition we would ideally use is: s->color == t->color.
       Additionally: s->type != PRECOLORED && t->type != PRECOLORED, in case
       we want to prevent deletion of "special" copies.  */
    if (ml->move
       	&& (s = alias (ml->move->source_web))->reg_rtx
       	    == (t = alias (ml->move->target_web))->reg_rtx
	&& s->type != PRECOLORED && t->type != PRECOLORED)
      {
	df_insn_delete (df, BLOCK_FOR_INSN (ml->move->insn), ml->move->insn);
	deleted_move_insns++;
      }
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
  /*  splits_init (); */
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
  while (simplify_wl || simplify_fat_wl || simplify_spilled_wl || mv_worklist
	 || freeze_wl || spill_wl);
  assign_colors ();
  if (spilled_nodes)
    {
      actual_spill ();
      return 1;
    }
  return 0;
}

static void
dump_constraints (void)
{
  rtx insn;
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	int code;
	int uid = INSN_UID (insn);
	int o;
	/* Don't simply force rerecognition, as combine might left us
	   with some unrecongnizable ones, which later leads to aborts
	   in regclass, if we now destroy the remembered INSN_CODE().  */
	/*INSN_CODE (insn) = -1;*/
	code = recog_memoized (insn);
	if (code < 0)
	  {
	    debug_msg (0, "%d: asm insn or not recognizable.\n", uid);
	    continue;
	  }
	debug_msg (0, "%d: code %d {%s}, %d operands, constraints: ",
		   uid, code, insn_data[code].name, recog_data.n_operands);
        extract_insn (insn);
	/*preprocess_constraints ();*/
	for (o = 0; o < recog_data.n_operands; o++)
	  {
	    debug_msg (0, "%d:%s ", o, recog_data.constraints[o]);
	  }
	debug_msg (0, "\n");
      }
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

      debug_msg (0, "  %4d : regno %3d", i, web->regno);
      if (SUBWEB_P (web))
	{
	  debug_msg (0, " sub %d", SUBREG_BYTE (web->orig_x));
	  debug_msg (0, " par %d", find_web_for_subweb (web)->id);
	}
      debug_msg (0, " +%d (span %d, weight " HOST_WIDE_INT_PRINT_DEC ") (%s)%s",
	         web->add_hardregs, web->span_insns, web->weight,
	         reg_class_names[web->regclass],
	         web->spill_temp ? " (spilltemp)" : "");
      if (web->type == PRECOLORED)
        debug_msg (0, " (precolored, color=%d)", web->color);
      if (find_web_for_subweb (web)->num_uses == 0)
	debug_msg (0, " dead");
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
  debug_msg (0, "Added spill insns (overall): %d loads, %d stores, %d remats\n",
	     emitted_spill_loads, emitted_spill_stores, emitted_remat);
  if (deleted_move_insns)
    debug_msg (0, "Deleted %d move insns.\n", deleted_move_insns);
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
static 
int only_one_reaching_def (link)
     struct df_link *link;
{
  for (; link; link = link->next)
    {
      if (DF_REF_CHAIN (link->ref) == NULL || DF_REF_CHAIN (link->ref)->next != NULL)
	return 0;
    }
  return 1;
}
static
int rematerializable (ref)
     struct ref *ref;
{
  rtx set;
  if (DF_REF_REG_USE_P (ref))
    {
      if (DF_REF_CHAIN (ref) != NULL && DF_REF_CHAIN (ref)->next == NULL)
	{
	  set = single_set (DF_REF_INSN (DF_REF_CHAIN (ref)->ref));
	  if (!set)
	    return 0;
	  if (CONSTANT_P (SET_SRC (set)))
	    return 1;
	}
    }
  else if (DF_REF_REG_DEF_P (ref))
    {
      if (DF_REF_REGNO (ref) < FIRST_PSEUDO_REGISTER)
	return 0;
      if (DF_REF_CHAIN (ref) != NULL && only_one_reaching_def (DF_REF_CHAIN (ref)))
	{
	  set = single_set (DF_REF_INSN (ref));
	  if (!set)
	    return 0;
	  if (CONSTANT_P (SET_SRC (set)))
	    return 1;
	}
      else if (DF_REF_CHAIN (ref) == NULL)
	return 1;
    }
  return 0;
}

static basic_block dom_parent PARAMS ((basic_block));
static void find_nesting_depths PARAMS ((void));
static void calculate_pre_post PARAMS ((void));
static unsigned int walk_dom_tree PARAMS ((basic_block, unsigned int *,
					   unsigned int));
static bool sp_back_edge PARAMS ((basic_block, basic_block));
static void reach_under PARAMS ((basic_block, basic_block, sbitmap));
static void dfs PARAMS ((unsigned int));
static bool dominates PARAMS ((basic_block, basic_block));
static unsigned int DFS_DJ_graph PARAMS ((basic_block, unsigned int *));
static void visit_successors PARAMS ((unsigned int, unsigned int));
static struct dj_graph_info *dj_graph_info;
static unsigned int max_level = 0;
static struct linked_list **levels;
static struct scc_info *scc_info;
static unsigned int dfs_stack;
static unsigned int next_dfs_num;
static unsigned int max_depth;
static unsigned int *depths;
static int *pre;
static int *pre_inverse;
static int *post;
dominator_tree domtree;

static
void calculate_pre_post ()
{
  edge *stack;
  int i;
  int sp;
  int prenum = 0;
  int postnum = 0;
  sbitmap visited;

  /* Allocate the preorder and postorder number arrays.  */
  pre = (int *) xcalloc (n_basic_blocks+1, sizeof (int));
  post = (int *) xcalloc (n_basic_blocks+1, sizeof (int));
  pre_inverse = (int *) xcalloc (n_basic_blocks+1, sizeof (int));
  
  /* Allocate stack for back-tracking up CFG.  */
  stack = (edge *) xmalloc ((n_basic_blocks + 1) * sizeof (edge));
  sp = 0;

  /* Allocate bitmap to track nodes that have been visited.  */
  visited = sbitmap_alloc (n_basic_blocks);

  /* None of the nodes in the CFG have been visited yet.  */
  sbitmap_zero (visited);

  /* Push the first edge on to the stack.  */
  stack[sp++] = ENTRY_BLOCK_PTR->succ;

  while (sp)
    {
      edge e;
      basic_block src;
      basic_block dest;

      /* Look at the edge on the top of the stack.  */
      e = stack[sp - 1];
      src = e->src;
      dest = e->dest;

      /* Check if the edge destination has been visited yet.  */
      if (dest != EXIT_BLOCK_PTR && ! TEST_BIT (visited, dest->index))
	{
	  /* Mark that we have visited the destination.  */
	  SET_BIT (visited, dest->index);

	  pre[dest->index] = prenum++;

	  if (dest->succ)
	    {
	      /* Since the DEST node has been visited for the first
		 time, check its successors.  */
	      stack[sp++] = dest->succ;
	    }
	  else
	    post[dest->index] = postnum++;
	}
      else
	{
	  if (! e->succ_next && src != ENTRY_BLOCK_PTR)
	    post[src->index] = postnum++;

	  if (e->succ_next)
	    stack[sp - 1] = e->succ_next;
	  else
	    sp--;
	}
    }


  free (stack);
  sbitmap_free (visited);
  for (i = 0; i < n_basic_blocks; i++)
    pre_inverse[pre[i]] = i;
}
static basic_block
dom_parent (block)
     basic_block block;
{
  int fpred,dedge, index;
  fpred = DTREE_NODE (domtree, dom_node_for_block (domtree, block->index))->fpred;
  dedge = DTREE_EDGE (domtree, fpred)->from;
  index = DTREE_NODE (domtree, dedge)->index;
  if (index >= 0)
    return BASIC_BLOCK (index);
  if (index == ENTRY_BLOCK)
    return ENTRY_BLOCK_PTR;
  if (index == EXIT_BLOCK)
    return EXIT_BLOCK_PTR;
  abort();
}

  

static unsigned int
walk_dom_tree (block, index, level)
     basic_block block;
     unsigned int *index;
     unsigned int level;
{
  unsigned int size = 1;
  int preorder_index = pre [block->index];
  unsigned int dom_index = (*index)++;
  int child;
  dj_graph_info[preorder_index].dom_index = dom_index;
  dj_graph_info[preorder_index].dom_level = level;
  if (level > max_level) max_level = level;
  if (dom_node_for_block (domtree, block->index) < 0)
    child = -1;
  else
    child = DTREE_NODE (domtree,
			dom_node_for_block (domtree, block->index))->fsucc;
  while (child >= 0)
    {
      struct dom_edge *edge;
      basic_block block;
      edge = DTREE_EDGE (domtree, child);
      block = BASIC_BLOCK (DTREE_NODE (domtree, edge->to)->index);
      size += walk_dom_tree (block, index, level + 1);
      child =  edge->succ;
    }
  return dj_graph_info[preorder_index].dom_size = size;
}
static bool
sp_back_edge (parent, child)
     basic_block parent;
     basic_block child;
{
  unsigned int p_index = dj_graph_info[pre[parent->index]].dfs_index;
  unsigned int p_size = dj_graph_info[pre[parent->index]].dfs_size;
  unsigned int c_index = dj_graph_info[pre[child->index]].dfs_index;
  return p_index <= c_index && c_index < p_index + p_size;
}
static void
reach_under (block, head, loop)
     basic_block block;
     basic_block head;
     sbitmap loop;
{
  sbitmap worklist;
  edge edge;
  
  worklist = sbitmap_alloc (n_basic_blocks + 1);
  sbitmap_zero (worklist);
  SET_BIT (worklist, pre[block->index]);
  SET_BIT (worklist, pre[head->index]);
  for (edge = block->pred; edge; edge = edge->pred_next)
    {
      basic_block predblock = edge->src;
      int pred_index = pre[predblock->index];
      if (dominates (head, predblock))
	SET_BIT (worklist, pred_index);
    }
  while (sbitmap_first_set_bit (worklist) >= 0)
    {
      int index = sbitmap_first_set_bit (worklist);
      RESET_BIT (worklist, index);
      if (!TEST_BIT (loop, index))
	{
	  SET_BIT (loop, index);
	  for (edge = BASIC_BLOCK (pre_inverse[index])->pred; edge; edge = edge->pred_next)
	    {
	      basic_block predblock = edge->src;
	      int pred_index = pre[predblock->index];
	      if (dominates (head, predblock))
		SET_BIT (worklist, pred_index);
	    }
	}
    }
  sbitmap_free (worklist);
}

	      
      
static bool
dominates (parent, child)
     basic_block parent;
     basic_block child;
{
  unsigned int p_index = dj_graph_info[pre[parent->index]].dom_index;
  unsigned int p_size = dj_graph_info[pre[parent->index]].dom_size;
  unsigned int c_index = dj_graph_info[pre[child->index]].dom_index;
  return p_index <= c_index && c_index < p_index + p_size;
}
static void
dfs (i)
     unsigned int i;
{
  scc_info[i].visited = TRUE;
  scc_info[i].dfs_num = scc_info[i].low = next_dfs_num++;
  scc_info[i].in_stack = TRUE;
  scc_info[i].next = dfs_stack;
  dfs_stack = i;
  
  visit_successors (i, i);
  if (scc_info[i].low == scc_info[i].dfs_num)
    {
      unsigned int name = dfs_stack;
      if (i == name)
	{
	  unsigned int name = dfs_stack;
	  scc_info[name].in_stack = FALSE;
	  dfs_stack = scc_info[name].next;
	}
      else
	{
	  unsigned int scc = 0;
	  do
	    {
	      name = dfs_stack;
	      scc_info[name].in_stack = FALSE;
	      dfs_stack = scc_info[name].next;
	      scc_info[name].next = scc;
	      scc = name;
	    } while (name != i);
	  for (name = scc; name; name = scc_info[name].next)
	    {
	      unsigned int depth = ++(depths[i]);
	      if (depth > max_depth) max_depth = depth;
	      if (!dj_graph_info[name].loop_head || dj_graph_info[i].loop_head == name)
		{
		  dj_graph_info[name].loop_head = i;
		  if (name != i)
		    {
		      struct linked_list * node = (struct linked_list *) ggc_alloc (sizeof (struct linked_list));
		      node->name = name;
		      node->next = dj_graph_info[i].children;
		      dj_graph_info[i].children = node;
		    }		 
		}
	    }
	}
    }
}
static void
visit_successors PARAMS ((pred, i))
     unsigned int pred;
     unsigned int i;
{
  basic_block block;
  edge edge;
  struct linked_list *node;

  block = BASIC_BLOCK (pre_inverse[i]);
  for (edge = block->succ; edge; edge = edge->succ_next)
    {
      int succ_index = pre[edge->dest->index];
      if (!dj_graph_info[succ_index].loop_head)
	{
	  if (!scc_info[succ_index].visited)
	    {
	      dfs (succ_index);
	      scc_info[pred].low = MIN(scc_info[pred].low, scc_info[succ_index].low);
	    }
	  if (scc_info[succ_index].dfs_num < scc_info[pred].dfs_num
	      && scc_info[succ_index].in_stack)
	    scc_info[pred].low = MIN(scc_info[succ_index].dfs_num, scc_info[pred].low);
	}
    }
  for (node = dj_graph_info[i].children; node; node = node->next)
    visit_successors (pred, node->name);
}

static sbitmap visited;

static unsigned int
DFS_DJ_graph (block, index)
     basic_block block;
     unsigned int *index;
{
  unsigned int size = 1;
  int preorder_index = pre[block->index];
  edge succ;
  int child;
  SET_BIT (visited, block->index);
  dj_graph_info[preorder_index].dfs_index = (*index)++; 
  if (dom_node_for_block (domtree, block->index) < 0)
    child = -1;
  else
    child = DTREE_NODE (domtree,
			dom_node_for_block (domtree, block->index))->fsucc;
  while (child >= 0)
    {
      struct dom_edge *edge;
      basic_block childblock;
      edge = DTREE_EDGE (domtree, child);
      childblock = BASIC_BLOCK (DTREE_NODE (domtree, edge->to)->index);
      size += DFS_DJ_graph (childblock, index);
      child =  edge->succ;
    }
  for (succ = block->succ; succ; succ = succ->succ_next)
    {
      basic_block next_block = succ->dest;
      if (next_block->index >= 0)
	{
	  if (!TEST_BIT (visited, next_block->index))
	    size += DFS_DJ_graph (next_block, index);
	}
    }
  return dj_graph_info[preorder_index].dfs_size = size;
}
static void 
find_nesting_depths()
{
  unsigned int *depthtemp;
  unsigned int index = 1;
  unsigned int level = 0;
  int i;
  int *idom = (int *)alloca (n_basic_blocks * sizeof (int));
  sbitmap loop;
  
  memset (idom, -1, (size_t) n_basic_blocks * sizeof (int));
  calculate_pre_post ();
  depths = (unsigned int *)xcalloc (n_basic_blocks + 1, sizeof(unsigned int));
  depthtemp = (unsigned int *)xcalloc (n_basic_blocks + 1, sizeof (unsigned int));
  calculate_dominance_info (idom, NULL, CDI_DOMINATORS);
  domtree = dom_tree_from_idoms (idom);
  dj_graph_info = (struct dj_graph_info *) ggc_alloc_cleared
    ((n_basic_blocks + 1) * sizeof (struct dj_graph_info));
  walk_dom_tree (BASIC_BLOCK (0), &index,  level);    
  levels = (struct linked_list **) ggc_alloc_cleared ((max_level + 1) * sizeof (struct linked_list *));
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block block = BASIC_BLOCK (i);
      int block_num = pre[block->index];
      struct linked_list *node;

      level = dj_graph_info[block_num].dom_level;
      node = (struct linked_list *) ggc_alloc (sizeof (struct linked_list));
      node->name = block_num;
      node->next = levels[level];
      levels[level] = node;
    }
  index = 1;
  visited = sbitmap_alloc (n_basic_blocks);
  sbitmap_zero (visited);
  DFS_DJ_graph (BASIC_BLOCK (0), &index);
  loop = sbitmap_alloc (n_basic_blocks + 1);
  sbitmap_zero (loop);
  
  for (i = max_level; i >= 0; i--)
    {
      bool irreducible_loop = FALSE;
      struct linked_list *node;
      for (node = levels[i]; node; node = node->next)
	{
	  basic_block block = BASIC_BLOCK (pre_inverse[node->name]);
	  edge edge;
	  sbitmap_zero (loop);
	  for (edge = block->pred; edge; edge = edge->pred_next)
	    {
	      basic_block pred = edge->src;
	      if (pred != dom_parent (block))
		{
		  if (!dominates (block, pred) && sp_back_edge (block, pred))
		    irreducible_loop = TRUE;
		  if (dominates (block, pred))
		    reach_under (pred, block, loop);
		}
	    }
	  if (sbitmap_first_set_bit (loop) >= 0)
	    {
	      int block_index = pre [block->index];
	      unsigned int i;
	      EXECUTE_IF_SET_IN_SBITMAP (loop, 0, i, 
	      {
		unsigned int depth = ++(depths[i]);
		if (depth > max_depth) max_depth = depth;
		if (!dj_graph_info[i].loop_head || dj_graph_info[i].loop_head == i)
		  {
		    dj_graph_info[i].loop_head = block_index;
		    if ((int )i != block_index)
		      {
			struct linked_list *node = (struct linked_list *) ggc_alloc (sizeof (struct linked_list));
			node->name = i;
			node->next = dj_graph_info[block_index].children;
			dj_graph_info[block_index].children = node;
		      }
		  }
	      });
	    }
	  if (irreducible_loop)
	    {
	      unsigned int j;
	      scc_info = (struct scc_info *) ggc_alloc_cleared (sizeof (struct scc_info) * (n_basic_blocks + 1));
	      next_dfs_num = 0;
	      for (j = i; j <= max_depth; j++)
		{
		  struct linked_list *node;
		  for (node = levels[j]; node; node = node->next)
		    {
		      int index = node->name;
		      if (!dj_graph_info[index].loop_head && !scc_info[index].visited)
			dfs(i);
		    }
		}
	    }
	}
    }
  memcpy (depthtemp, depths, sizeof (unsigned int) * (n_basic_blocks));
  for (i = 0; i < n_basic_blocks; i ++)
	depths[i] = depthtemp[pre[i]];
  free (depthtemp);
  sbitmap_free (loop);
  sbitmap_free (visited);
  free (pre);
  free (post);
  free (pre_inverse);
  destroy_dominator_tree (domtree);
}
bool split_live_ranges = FALSE;
static bool any_splits_found;
struct split_range
{
  double loads;
  double stores;
};
static struct split_range *split_ranges;

static struct linked_list **split_around;
static struct linked_list **neighbors_with_color;
static inline double powraise PARAMS ((unsigned int));
static inline double 
powraise (power)
     unsigned int power;
{
  double i = 1.0;
  while (power--)
      i *= 10.0;
  return i;
}
static bool 
find_splits (name, colors)
     unsigned int name;
     int *colors;
{
  return FALSE;
}
static void 
splits_init ()
{
  if (split_live_ranges)
    {
      unsigned int i;
      split_ranges = (struct split_range *)ggc_alloc_cleared (num_webs * sizeof (struct split_range));
      split_around = (struct linked_list **)ggc_alloc_cleared (num_webs * sizeof (struct linked_list *));
      for (i = 0; i < num_webs; i++)
	{
	  int j;
	  struct web *web = id2web[i];
	  for (j = 0; j < web->num_defs; j++)
	    split_ranges[i].stores += powraise (depths[DF_REF_BB (web->defs[j])->index]);
	  for (j = 0; j < web->num_uses; j++)
	    split_ranges[i].loads += powraise (depths[DF_REF_BB (web->uses[j])->index]);
	}
      any_splits_found = FALSE;
    }
}


/* Main register allocator entry point.  */
void
reg_alloc (void)
{
  int changed;
  int pass = 0;
  split_live_ranges = FALSE;
  update_equiv_regs();
  init_ra ();
  /*  find_nesting_depths (); */
  pass = 0;
  no_new_pseudos = 0;
  /* We don't use those NOTEs, and as we anyway change all registers,
     they only make problems later.  */
  count_or_remove_death_notes (NULL, 1);
  emitted_spill_loads = 0;
  emitted_spill_stores = 0;
  emitted_remat = 0;
  deleted_move_insns = 0;
  df = df_init ();
  do
    {
      int j;
      debug_msg (0, "RegAlloc Pass %d\n\n", pass);
      if (pass++ > 40)
	internal_error ("Didn't find a coloring.\n");
      df_analyse (df, 0, DF_HARD_REGS | DF_RD_CHAIN | DF_RU_CHAIN | DF_DU_CHAIN | DF_UD_CHAIN );
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
	{
          emit_colors (df);
	  delete_moves ();
	  dump_constraints ();
	}
      else
	{
	  allocate_reg_info (max_reg_num (), FALSE, TRUE);
	  compute_bb_for_insn (get_max_uid ());
	  delete_trivially_dead_insns (get_insns (), max_reg_num (), 0);
	  reg_scan_update (get_insns (), BLOCK_END (n_basic_blocks - 1),
			   max_regno);
	  max_regno = max_reg_num ();
	  regclass (get_insns (), max_reg_num (), rtl_dump_file);
	}
      dump_ra (df);
      if (changed && rtl_dump_file)
	{
	  print_rtl_with_bb (rtl_dump_file, get_insns ());
	  fflush (rtl_dump_file);
	}
      free_all_lists ();
      free_mem (df);
    }
  while (changed);
  /*  free (depths); */
  df_finish (df);
  if (rtl_dump_file)
       print_rtl_with_bb (rtl_dump_file, get_insns ()); 
  no_new_pseudos = 0;
  allocate_reg_info (max_reg_num (), FALSE, TRUE);
  compute_bb_for_insn (get_max_uid ());
  store_motion ();
  no_new_pseudos = 1;
  compute_bb_for_insn (get_max_uid ());
  clear_log_links (get_insns ());
  cleanup_cfg (CLEANUP_EXPENSIVE);
  find_basic_blocks (get_insns (), max_reg_num (), rtl_dump_file);
  life_analysis (get_insns (), rtl_dump_file, 
		 PROP_DEATH_NOTES | PROP_LOG_LINKS  | PROP_REG_INFO); 
  recompute_reg_usage (get_insns (), TRUE);
  /* XXX: reg_scan screws up reg_renumber, and without reg_scan, we can't do
     regclass. */
  /*reg_scan (get_insns (), max_reg_num (), 1);
    regclass (get_insns (), max_reg_num (), rtl_dump_file); */
}

/*
vim:cinoptions={.5s,g0,p5,t0,(0,^-0.5s:tw=78:cindent:sw=4:
*/
