/* Graph coloring register allocator
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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
#include "real.h"
#include "pre-reload.h"


#define NO_REMAT
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

/* The algorithm used is currently Iterated Register Coalescing by
   L.A.George, and Appel. XXX not true anymore
*/

/* TODO
 
   * Lattice based rematerialization
   * do lots of commenting
   * look through all XXX's and do something about them
   * handle REG_NO_CONFLICTS blocks correctly (the current ad hoc approach
     might miss some conflicts due to insns which only seem to be in a 
     REG_NO_CONLICTS block)
     -- Don't necessary anymore, I believe, because SUBREG tracking is
     implemented.
   * create definitions of ever-life regs at the beginning of
     the insn chain
   * insert loads as soon, stores as late as possile
   * insert spill insns as outward as possible (either looptree, or LCM)
   * reuse stack-slots
   * use the frame-pointer, when we can (possibly done)
   * delete coalesced insns.  Partly done.  The rest can only go, when we get
     rid of reload.
   * don't insert hard-regs, but a limited set of pseudo-reg
     in emit_colors, and setup reg_renumber accordingly (done, but this
     needs reload, which I want to go away)
   * don't destroy coalescing information completely when spilling
   * use the constraints from asms
   * implement spill coalescing/propagation
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
  INITIAL = 0, FREE,
  PRECOLORED,
  SIMPLIFY, SIMPLIFY_SPILL, SIMPLIFY_FAT, FREEZE, SPILL,
  SELECT,
  SPILLED, COALESCED, COLORED,
  LAST_NODE_TYPE
};

struct tagged_conflict
{
  struct tagged_conflict *next;
  bitmap conflicts;
  unsigned int size_word;  /* size | (word << 16) */
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
  unsigned int spanned_deaths;
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
  unsigned int span_deaths; /* How many insn's the live range spans */
  unsigned int spill_temp; /* Temporarily used to remember what webs
                              were spilled */
  unsigned int orig_spill_temp;
  unsigned HOST_WIDE_INT spill_cost; /* Cost of spilling */
  unsigned HOST_WIDE_INT orig_spill_cost; /* Before coalescing.  */
  unsigned int num_aliased; /* How many webs are aliased to us?.  */
  int color; /* Color of the web */
  int old_color; /* 1 + color of web in last iteration.  */
  unsigned int use_my_regs:1; /* Determine if we should use the
                                 usable_regs for this web */
  unsigned int was_spilled:1;
  /* We need to distinguish also webs which are targets of coalescing
     (all x with some y, so that x==alias(y)), but the alias field is
     only set for sources of coalescing.  This flag is set for all webs
     involved in coalescing in some way.  */
  unsigned int is_coalesced:1;
  unsigned int artificial:1;
  		    /* != 0 : there is no rtl in the code which corresponds
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
  unsigned int crosses_call:1;
  unsigned int move_related:1; /* Whether the web is move related (IE involved
                                  in a move) */
  unsigned int live_over_abnormal:1; /* 1 when this web (or parts thereof) are live
			       over an abnormal edge.  */
  unsigned int old_web:1; /* != 0, when this web stems from the last iteration
			     of the allocator, and still contains all info.  */
  unsigned int in_load:1; /* For rewrite_program2() to remember webs, which
			     are already marked for (re)loading.  */
  unsigned int one_load:1; /* If in_load, then this is non-zero if only one
			      use was seen since insertion in loadlist.
			      Zero if more uses currently need a reload.
			      Used to differentiate between inserting register
			      loads or directly substituting the stackref.  */
  unsigned int changed:1; /* When using rewrite_program2() this flag gets set
			     if some insns were inserted on behalf of this
			     web.  IR spilling might ignore some deaths up to
			     the def, so no code might be emitted and we need
			     not to spill such a web again.  */
  unsigned int target_of_spilled_move:1;
  unsigned int have_orig_conflicts:1;
  ENUM_BITFIELD(node_type) type:5; /* Current state of the node */
  ENUM_BITFIELD(reg_class) regclass:10; /* just used for debugging */
  int add_hardregs; /* Additional hard registers needed to be
                       allocated to the web */
  int num_conflicts;  /* Number of conflicts currently */
  unsigned int num_uses; /* Number of uses this web spans */
  unsigned int num_defs; /* Number of defs this web spans. */
  rtx orig_x; /* The (reg:M a) or (subreg:M1 (reg:M2 a) x) rtx which this
		 web is based on.  This is used to distinguish subreg webs
		 from it's reg parents, and to get hold of the mode.  */
  struct web *parent_web;
  struct web *subreg_next; /* If this web is a subreg, but not the last one of
			      another reg, then this points to the next
			      subreg.  */

  /* The set of webs (or subwebs), this web conflicts with.  */
  struct conflict_link *conflict_list;
  /* If have_orig_conflicts is set this contains a copy of conflict_list,
     as it was right after building the interference graph.
     It's used for incremental i-graph building and for breaking
     coalescings again.  */
  struct conflict_link *orig_conflict_list;
  bitmap useless_conflicts;
  /* might be too much to store a HARD_REG_SET here for machines with _many_
     registers.  Shouldn't hurt for now.  */
  HARD_REG_SET usable_regs;
  HARD_REG_SET orig_usable_regs;
  HARD_REG_SET bias_colors; /* Colors of neighbors.  */
  HARD_REG_SET prefer_colors; /* Colors of PRECOLORED webs this web is
				 connected to by a move.  */
  int num_freedom; /* Number of registers we could place this in */
  rtx reg_rtx; /* RTX for this register */
  rtx stack_slot; /* Stack slot for this register */
  rtx last_use_insn; /* Used in rewrite_program2() to remember the last using
			insn for webs needing (re)loads.  */
  rtx pattern; /* If != 0 web is rematerializable, and this contains the
		  RTL pattern usable as source for rematerialization.  */
  struct ref **defs; /* [0..num_defs-1] */
  struct ref **uses; /* [0..num_uses-1] */
  struct web *alias;
  struct move_list *moves;
  struct dlist *dlink;
  struct df_link *temp_refs;
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
struct rewrite_info;
struct rtx_list;

static struct obstack ra_obstack;
static inline void * ra_alloc PARAMS ((size_t));
static inline void * ra_calloc PARAMS ((size_t));
static void ra_print_rtx_1op PARAMS ((FILE *, rtx));
static void ra_print_rtx_2op PARAMS ((FILE *, rtx));
static void ra_print_rtx_3op PARAMS ((FILE *, rtx));
static void ra_print_rtx_object PARAMS ((FILE *, rtx));
static void ra_print_rtx PARAMS ((FILE *, rtx, int));
static void ra_print_rtx_top PARAMS ((FILE *, rtx, int));
static void ra_debug_rtx PARAMS ((rtx));
static void ra_debug_insns PARAMS ((rtx, int));
static void ra_debug_bbi PARAMS ((int));
static void ra_print_rtl_with_bb PARAMS ((FILE *, rtx));
static int hard_regs_count PARAMS ((HARD_REG_SET));
static unsigned int rtx_to_bits PARAMS ((rtx));
static unsigned HOST_WIDE_INT rtx_to_undefined PARAMS ((rtx));
static void create_insn_info PARAMS ((struct df *));
static void free_insn_info PARAMS ((void));
static bitmap find_sub_conflicts PARAMS ((struct web_part *, unsigned int));
static bitmap get_sub_conflicts PARAMS ((struct web_part *, unsigned int));
static unsigned int undef_to_size_word PARAMS ((unsigned HOST_WIDE_INT *));
static bitmap undef_to_bitmap PARAMS ((struct web_part *,
				       unsigned HOST_WIDE_INT *));
static struct web_part * find_web_part_1 PARAMS ((struct web_part *));
static struct web_part * union_web_part_roots
				PARAMS ((struct web_part *, struct web_part *));
static int defuse_overlap_p_1 PARAMS ((rtx, struct curr_use *));
static int live_out_1 PARAMS ((struct df *, struct curr_use *, rtx));
static int live_out PARAMS ((struct df *, struct curr_use *, rtx));
static rtx live_in_edge PARAMS (( struct df *, struct curr_use *, edge));
static void live_in PARAMS ((struct df *, struct curr_use *, rtx));
static void build_i_graph PARAMS ((struct df*));
static void debug_msg PARAMS ((unsigned int,
			       const char *, ...)) ATTRIBUTE_PRINTF_2;
static int copy_insn_p PARAMS ((rtx, rtx *, rtx *));
static int hard_regs_intersect_p PARAMS ((HARD_REG_SET *, HARD_REG_SET *));
static void remember_move PARAMS ((rtx));
static void handle_asm_insn PARAMS ((struct df *, rtx));
static void prune_hardregs_for_mode PARAMS ((HARD_REG_SET *, enum machine_mode));
static void init_one_web_common PARAMS ((struct web *, rtx));
static void init_one_web PARAMS ((struct web *, rtx));
static void reinit_one_web PARAMS ((struct web *, rtx));
static struct web * find_subweb PARAMS ((struct web *, rtx));
static struct web * find_subweb_2 PARAMS ((struct web *, unsigned int));
static struct web * add_subweb PARAMS ((struct web *, rtx));
static struct web * add_subweb_2 PARAMS ((struct web *, unsigned int));
static struct web * find_web_for_subweb_1 PARAMS ((struct web *));
static int regs_overlap_p PARAMS ((rtx, rtx));
static void init_web_parts PARAMS ((struct df *));
static void copy_conflict_list PARAMS ((struct web *));
static void add_conflict_edge PARAMS ((struct web *, struct web *));
static void record_conflict PARAMS ((struct web *, struct web *));
static void build_inverse_webs PARAMS ((struct web *));
static void copy_web PARAMS ((struct web *, struct web_link **));
static void compare_and_free_webs PARAMS ((struct web_link **));
static void init_webs_defs_uses PARAMS ((struct df *df));
static unsigned int parts_to_webs_1 PARAMS ((struct df *, struct web_link **,
				    struct df_link *));
static void parts_to_webs PARAMS ((struct df *));
static void reset_conflicts PARAMS ((void));
static void check_conflict_numbers PARAMS ((void));
static void conflicts_between_webs PARAMS ((struct df *));
static void remember_web_was_spilled PARAMS ((struct web *));
static void detect_spill_temps PARAMS ((void));
static int memref_is_stack_slot PARAMS ((rtx));
static int contains_pseudo PARAMS ((rtx));
static int want_to_remat PARAMS ((rtx x));
static void detect_remat_webs PARAMS ((void));
static void determine_web_costs PARAMS ((void));
static void make_webs PARAMS ((struct df *));
static void moves_to_webs PARAMS ((struct df *));
static void connect_rmw_web_parts PARAMS ((struct df *));
static void update_regnos_mentioned PARAMS ((void));
static void livethrough_conflicts_bb PARAMS ((basic_block));
static void init_bb_info PARAMS ((void));
static void free_bb_info PARAMS ((void));
static void build_web_parts_and_conflicts PARAMS ((struct df *));
static void dump_igraph PARAMS ((struct df *));
static void dump_igraph_machine PARAMS ((void));
static void realloc_web_parts PARAMS ((struct df *));
static void alloc_mem PARAMS ((struct df *));
static void free_mem PARAMS ((struct df *));
static void free_all_mem PARAMS ((struct df *df));
static void push_list PARAMS ((struct dlist *, struct dlist **));
static void push_list_end PARAMS ((struct dlist *, struct dlist **));
static void remove_list PARAMS ((struct dlist *, struct dlist **));
static struct dlist * pop_list PARAMS ((struct dlist **));
static void free_dlist PARAMS ((struct dlist **));
static void reset_lists PARAMS ((void));
static inline void put_web PARAMS ((struct web *, enum node_type));
static void put_web_at_end PARAMS ((struct web *, enum node_type));
static void remove_web_from_list PARAMS ((struct web *));
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
static inline unsigned int simplify_p PARAMS ((enum node_type));
static void combine PARAMS ((struct web *, struct web *));
static void coalesce PARAMS ((void));
static void freeze_moves PARAMS ((struct web *));
static void freeze PARAMS ((void));
static void select_spill PARAMS ((void));
static int color_usable_p PARAMS ((int, HARD_REG_SET, HARD_REG_SET,
				   enum machine_mode));
int get_free_reg PARAMS ((HARD_REG_SET, HARD_REG_SET, enum machine_mode));
static int get_biased_reg PARAMS ((HARD_REG_SET, HARD_REG_SET, HARD_REG_SET,
				   HARD_REG_SET, enum machine_mode));
static int count_long_blocks PARAMS ((HARD_REG_SET, int));
static char * hardregset_to_string PARAMS ((HARD_REG_SET));
static void calculate_dont_begin PARAMS ((struct web *, HARD_REG_SET *));
static void colorize_one_web PARAMS ((struct web *, int));
static void assign_colors PARAMS ((void));
static void try_recolor_web PARAMS ((struct web *));
static void insert_coalesced_conflicts PARAMS ((void));
static int comp_webs_maxcost PARAMS ((const void *, const void *));
static void recolor_spills PARAMS ((void));
static void restore_conflicts_from_coalesce PARAMS ((struct web *));
static void unalias_web PARAMS ((struct web *));
static void spill_coalescing PARAMS ((sbitmap, sbitmap));
static unsigned HOST_WIDE_INT spill_prop_savings PARAMS ((struct web *,
							  sbitmap));
static void spill_prop_insert PARAMS ((struct web *, sbitmap, sbitmap));
static int spill_propagation PARAMS ((sbitmap, sbitmap, sbitmap));
static void spill_coalprop PARAMS ((void));
static void allocate_spill_web PARAMS ((struct web *));
static void rewrite_program PARAMS ((bitmap));
static void remember_slot PARAMS ((struct rtx_list **, rtx));
static int slots_overlap_p PARAMS ((rtx, rtx));
static void delete_overlapping_slots PARAMS ((struct rtx_list **, rtx));
static int slot_member_p PARAMS ((struct rtx_list *, rtx));
static void insert_stores PARAMS ((bitmap));
static int spill_same_color_p PARAMS ((struct web *, struct web *)); 
static unsigned int is_partly_live_1 PARAMS ((sbitmap, struct web *));
static void update_spill_colors PARAMS ((HARD_REG_SET *, struct web *, int));
static int spill_is_free PARAMS ((HARD_REG_SET *, struct web *));
static void emit_loads PARAMS ((struct rewrite_info *, int, rtx));
static void detect_bbs_for_rewrite PARAMS ((sbitmap));
static void detect_deaths_in_bb PARAMS ((basic_block, sbitmap, bitmap));
static void reloads_to_loads PARAMS ((struct rewrite_info *, struct ref **,
				      unsigned int, struct web **));
static void rewrite_program2 PARAMS ((bitmap));
static void mark_refs_for_checking PARAMS ((struct web *, bitmap));
static void detect_web_parts_to_rebuild PARAMS ((void));
static void delete_useless_defs PARAMS ((void));
static void detect_non_changed_webs PARAMS ((void));
static void reset_changed_flag PARAMS ((void));
static void actual_spill PARAMS ((void));
static void emit_colors PARAMS ((struct df *));
static void delete_moves PARAMS ((void));
static void init_web_pairs PARAMS ((void));
static void add_web_pair_cost PARAMS ((struct web *, struct web *,
			               unsigned HOST_WIDE_INT, unsigned int));
static int comp_web_pairs PARAMS ((const void *, const void *));
static void sort_and_combine_web_pairs PARAMS ((int));
static void aggressive_coalesce PARAMS ((void));
static void aggressive_coalesce_2 PARAMS ((void));
static void extended_coalesce PARAMS ((void));
static void extended_coalesce_2 PARAMS ((void));
static void check_uncoalesced_moves PARAMS ((void));
static int one_pass PARAMS ((struct df *, int));
static void dump_constraints PARAMS ((void));
static void dump_cost PARAMS ((unsigned int));
static void dump_graph_cost PARAMS ((unsigned int, const char *));
static void dump_ra PARAMS ((struct df *));
static void init_ra PARAMS ((void));
static bool find_splits PARAMS ((unsigned int, int *));
static void splits_init PARAMS ((void));
static void setup_renumber PARAMS ((int));
static void check_df PARAMS ((struct df *));
static void remove_suspicious_death_notes PARAMS ((void));
static void dump_web_conflicts PARAMS ((struct web *));
static void dump_web_insns PARAMS ((struct web*));
void reg_alloc PARAMS ((void));

/* XXX use Daniels compressed bitmaps here.  */
#define igraph_index(i, j) ((i) < (j) ? ((j)*((j)-1)/2)+(i) : ((i)*((i)-1)/2)+(j))
static sbitmap igraph;
static sbitmap live_over_abnormal;

/* Note the insns not inserted by the allocator, where we detected any
   deaths of pseudos.  It is used to detect closeness of defs and uses.
   In the first pass this is empty (we could initialize it from REG_DEAD
   notes), in the other passes it is left from the pass before.  */
static sbitmap insns_with_deaths;
static unsigned int death_insns_max_uid;

/* Uhhuuhu.  Don't the hell use two sbitmaps! XXX
   (for now I need the sup_igraph to note if there is any conflict between
   parts of webs at all.  I can't use igraph for this, as there only the real
   conflicts are noted.)  This is only used to prevent coalescing two
   conflicting webs, were only parts of them are in conflict.  */
static sbitmap sup_igraph;

struct ra_bb_info
{
  unsigned int pass;
  unsigned HOST_WIDE_INT undefined;
  bitmap regnos_mentioned, live_throughout;
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
static unsigned int num_allwebs;
static struct web **id2web;
static struct web *hardreg2web[FIRST_PSEUDO_REGISTER];
static struct web **def2web;
static struct web **use2web;
static struct move_list *wl_moves;
static int ra_max_regno;
static short *ra_reg_renumber;
static struct df *df;
static bitmap *live_at_end;
static int ra_pass;

/* The different lists on which a web can be (based on the type).  */
static struct dlist *web_lists[(int) LAST_NODE_TYPE];
#define WEBS(type) (web_lists[(int)(type)])

static unsigned int last_def_id;
static unsigned int last_use_id;
static unsigned int last_num_webs;
static unsigned int last_max_uid;
static sbitmap last_check_uses;
static unsigned int remember_conflicts;

/* Used to detect spill instructions inserted by me.  */
static int orig_max_uid;

static HARD_REG_SET never_use_colors;
static HARD_REG_SET usable_regs[N_REG_CLASSES];
static unsigned int num_free_regs[N_REG_CLASSES];
static HARD_REG_SET hardregs_for_mode[NUM_MACHINE_MODES];
static unsigned char byte2bitcount[256];

#define ID2WEB(I) id2web[I]
#define NUM_REGS(W) (((W)->type == PRECOLORED) ? 1 : (W)->num_freedom)
#define SUBWEB_P(W) (GET_CODE ((W)->orig_x) == SUBREG)

static const char *const reg_class_names[] = REG_CLASS_NAMES;

extern struct df2ra build_df2ra PARAMS ((struct df*, struct ra_info*));
static struct ra_info *ra_info;
struct df2ra df2ra;
static enum reg_class web_preferred_class PARAMS ((struct web *));
void web_class PARAMS ((void));

#define DUMP_COSTS	0x0001
#define DUMP_WEBS	0x0002
#define DUMP_IGRAPH	0x0004
#define DUMP_PROCESS	0x0008
#define DUMP_COLORIZE	0x0010
#define DUMP_ASM	0x0020
#define DUMP_CONSTRAINTS 0x0040
#define DUMP_RESULTS	0x0080
#define DUMP_DF		0x0100
#define DUMP_RTL	0x0200
#define DUMP_FINAL_RTL	0x0400
#define DUMP_REGCLASS	0x0800
#define DUMP_SM		0x1000
#define DUMP_LAST_FLOW	0x2000
#define DUMP_LAST_RTL	0x4000
#define DUMP_REBUILD	0x8000
#define DUMP_IGRAPH_M	0x10000

#define DUMP_EVER		((unsigned int)-1)
#define DUMP_NEARLY_EVER	(DUMP_EVER - DUMP_COSTS - DUMP_IGRAPH_M)
static unsigned int debug_new_regalloc = -1;
int flag_ra_dump_only_costs = 0;
int flag_ra_biased = 0;
int flag_ra_ir_spilling = 0;
int flag_ra_optimistic_coalescing = 0;
int flag_ra_break_aliases = 0;
int flag_ra_merge_spill_costs = 0;
int flag_ra_spill_every_use = 0;
int flag_ra_dump_notes = 0;

/* Print a message to the dump file, if debug_new_regalloc is the
   same or greater than the specified level. */

static void
debug_msg VPARAMS ((unsigned int level, const char *format, ...))
{
#ifndef ANSI_PROTOTYPES
  int level;
  const char *format;
#endif
  va_list ap;
  if ((debug_new_regalloc & level) != 0 && rtl_dump_file != NULL)
    {
      VA_START (ap, format);

#ifndef ANSI_PROTOTYPES
      format = va_arg (ap, const char *);
#endif
      
      vfprintf (rtl_dump_file, format, ap);
      va_end (ap);
    }
}

static inline void *
ra_alloc (size)
     size_t size;
{
  return obstack_alloc (&ra_obstack, size);
}

static inline void *
ra_calloc (size)
     size_t size;
{
  void *p = obstack_alloc (&ra_obstack, size);
  memset (p, 0, size);
  return p;
}

static void
ra_print_rtx_1op (file, x)
     FILE *file;
     rtx x;
{
  enum rtx_code code = GET_CODE (x);
  rtx op0 = XEXP (x, 0);
  switch (code)
    {
      case NEG:
      case NOT:
	  fputs ((code == NEG) ? "-(" : "~(", file);
	  ra_print_rtx (file, op0, 0);
	  fputs (")", file);
	  break;
      case HIGH:
	  fputs ("hi(", file);
	  ra_print_rtx (file, op0, 0);
	  fputs (")", file);
	  break;
      default:
	  fprintf (file, "%s", GET_RTX_NAME (code));
	  if (GET_MODE (x) != VOIDmode)
	    fprintf (file, ":%s(", GET_MODE_NAME (GET_MODE (x)));
	  else
	    fputs ("(", file);
	  ra_print_rtx (file, op0, 0);
	  fputs (")", file);
	  break;
    }
}

static void
ra_print_rtx_2op (file, x)
     FILE *file;
     rtx x;
{
  int infix = 1;
  const char *opname = "shitop";
  enum rtx_code code = GET_CODE (x);
  rtx op0 = XEXP (x, 0);
  rtx op1 = XEXP (x, 1);
  switch (code)
    {
      /* class '2' */
      case COMPARE: opname = "?"; break;
      case MINUS: opname = "-"; break;
      case DIV: opname = "/"; break;
      case UDIV: opname = "u/"; break;
      case MOD: opname = "%"; break;
      case UMOD: opname = "u%"; break;
      case ASHIFT: opname = "<<"; break;
      case ASHIFTRT: opname = "a>>"; break;
      case LSHIFTRT: opname = "l>>"; break;
      /* class 'c' */
      case PLUS: opname = "+"; break;
      case MULT: opname = "*"; break;
      case AND: opname = "&"; break;
      case IOR: opname = "|"; break;
      case XOR: opname = "^"; break;
      /* class '<' */
      case NE: opname = "!="; break;
      case EQ: opname = "=="; break;
      case GE: opname = "s>="; break;
      case GT: opname = "s>"; break;
      case LE: opname = "s<="; break;
      case LT: opname = "s<"; break;
      case GEU: opname = "u>="; break;
      case GTU: opname = "u>"; break;
      case LEU: opname = "u<="; break;
      case LTU: opname = "u<"; break;
      default:
		infix = 0;
		opname = GET_RTX_NAME (code);
		break;
    }
  if (infix)
    {
      fputs ("(", file);
      ra_print_rtx (file, op0, 0);
      fprintf (file, " %s ", opname);
      ra_print_rtx (file, op1, 0);
      fputs (")", file);
    }
  else
    {
      fprintf (file, "%s(", opname);
      ra_print_rtx (file, op0, 0);
      fputs (", ", file);
      ra_print_rtx (file, op1, 0);
      fputs (")", file);
    }
}

static void
ra_print_rtx_3op (file, x)
     FILE *file;
     rtx x;
{
  enum rtx_code code = GET_CODE (x);
  rtx op0 = XEXP (x, 0);
  rtx op1 = XEXP (x, 1);
  rtx op2 = XEXP (x, 2);
  if (code == IF_THEN_ELSE)
    {
      ra_print_rtx (file, op0, 0);
      fputs (" ? ", file);
      ra_print_rtx (file, op1, 0);
      fputs (" : ", file);
      ra_print_rtx (file, op2, 0);
    }
  else
    {
      /* Bitmap-operation */
      fprintf (file, "%s:%s(", GET_RTX_NAME (code),
	       GET_MODE_NAME (GET_MODE (x)));
      ra_print_rtx (file, op0, 0);
      fputs (", ", file);
      ra_print_rtx (file, op1, 0);
      fputs (", ", file);
      ra_print_rtx (file, op2, 0);
      fputs (")", file);
    }
}

static void
ra_print_rtx_object (file, x)
     FILE *file;
     rtx x;
{
  enum rtx_code code = GET_CODE (x);
  enum machine_mode mode = GET_MODE (x);
  switch (code)
    {
      case CONST_INT:
	  fprintf (file, HOST_WIDE_INT_PRINT_DEC, XWINT (x, 0));
	  break;
      case CONST_DOUBLE:
	    {
	      int i, num = 0;
	      const char *fmt = GET_RTX_FORMAT (code);
	      fputs ("dbl(", file);
	      for (i = 0; i < GET_RTX_LENGTH (code); i++)
		{
		  if (num)
		    fputs (", ", file);
		  if (fmt[i] == 'e' && XEXP (x, i))
		    /* The MEM or other stuff */
		    {
		      ra_print_rtx (file, XEXP (x, i), 0);
		      num++;
		    }
		  else if (fmt[i] == 'w')
		    {
		      fprintf (file, HOST_WIDE_INT_PRINT_HEX, XWINT (x, i));
		      num++;
		    }
		}
	      break;
	    }
      case CONST_STRING: fprintf (file, "\"%s\"", XSTR (x, 0)); break;
      case CONST: fputs ("const(", file);
		  ra_print_rtx (file, XEXP (x, 0), 0);
		  fputs (")", file);
		  break;
      case PC: fputs ("pc", file); break;
      case REG:
	       {
		 int regno = REGNO (x);
		 if (regno < FIRST_PSEUDO_REGISTER)
		   {
		     int i, nregs = HARD_REGNO_NREGS (regno, mode);
		     if (nregs > 1)
		       fputs ("[", file);
		     for (i = 0; i < nregs; i++)
		       {
			 if (i)
			   fputs (", ", file);
			 if (reg_names[regno+i] && *reg_names[regno + i])
			   fprintf (file, "%s", reg_names[regno + i]);
			 else
			   fprintf (file, "h%d", regno + i);
		       }
		     if (nregs > 1)
		       fputs ("]", file);
		   }
		 else
		   fprintf (file, "p%d", regno);
		 break;
	       }
      case SUBREG:
	       {
		 rtx sub = SUBREG_REG (x);
		 int ofs = SUBREG_BYTE (x);
		 if (GET_CODE (sub) == REG
		     && REGNO (sub) < FIRST_PSEUDO_REGISTER)
		   {
		     int regno = REGNO (sub);
		     int i, nregs = HARD_REGNO_NREGS (regno, mode);
		     regno += subreg_regno_offset (regno, GET_MODE (sub),
						   ofs, mode);
		     if (nregs > 1)
		       fputs ("[", file);
		     for (i = 0; i < nregs; i++)
		       {
			 if (i)
			   fputs (", ", file);
			 if (reg_names[regno+i])
			   fprintf (file, "%s", reg_names[regno + i]);
			 else
			   fprintf (file, "h%d", regno + i);
		       }
		     if (nregs > 1)
		       fputs ("]", file);
		   }
		 else
		   {
		     ra_print_rtx (file, sub, 0);
		     fprintf (file, ":[%s+%d]", GET_MODE_NAME (mode), ofs);
		   }
		 break;
	       }
      case SCRATCH: fputs ("scratch", file); break;
      case CONCAT: ra_print_rtx_2op (file, x); break;
      case HIGH: ra_print_rtx_1op (file, x); break;
      case LO_SUM:
		 fputs ("(", file);
		 ra_print_rtx (file, XEXP (x, 0), 0);
		 fputs (" + lo(", file);
		 ra_print_rtx (file, XEXP (x, 1), 0);
		 fputs ("))", file);
		 break;
      case MEM: fputs ("[", file);
		ra_print_rtx (file, XEXP (x, 0), 0);
		fprintf (file, "]:%s", GET_MODE_NAME (GET_MODE (x)));
		/* XXX print alias set too ?? */
		break;
      case LABEL_REF:
		  {
		    rtx sub = XEXP (x, 0);
		    if (GET_CODE (sub) == NOTE
			&& NOTE_LINE_NUMBER (sub) == NOTE_INSN_DELETED_LABEL)
		      fprintf (file, "(deleted uid=%d)", INSN_UID (sub));
		    else if (GET_CODE (sub) == CODE_LABEL)
		      fprintf (file, "L%d", CODE_LABEL_NUMBER (sub));
		    else
		      fprintf (file, "(nonlabel uid=%d)", INSN_UID (sub));
		  }
		break;
      case SYMBOL_REF:
		fprintf (file, "sym(\"%s\")", XSTR (x, 0)); break;
      case CC0: fputs ("cc0", file); break;
      default: print_inline_rtx (file, x, 0); break;
    }
}

static void
ra_print_rtx (file, x, with_pn)
     FILE *file;
     rtx x;
     int with_pn;
{
  enum rtx_code code;
  char class;
  int unhandled = 0;
  if (!x)
    return;
  code = GET_CODE (x);
  class = GET_RTX_CLASS (code);
  if (INSN_P (x) || code == NOTE || code == CODE_LABEL || code == BARRIER)
    {
      if (INSN_P (x))
	fputs ("  ", file);
      if (code == BARRIER)
	fputs ("; ", file);
      else if (code == NOTE)
	fprintf (file, "\t\t\t\t\t; ");
      else if (code == CODE_LABEL)
	{
	  fprintf (file, "L%d:\t; ", CODE_LABEL_NUMBER (x));
	  if (LABEL_NAME (x))
	    fprintf (file, "(%s) ", LABEL_NAME (x));
	  if (LABEL_ALTERNATE_NAME (x))
	    fprintf (file, "(alternate: %s) ", LABEL_ALTERNATE_NAME (x));
	  fprintf (file, " [%d uses] uid=(", LABEL_NUSES (x));
	}
      fprintf (file, "%d", INSN_UID (x));
      if (with_pn)
	fprintf (file, " %d %d", PREV_INSN (x) ? INSN_UID (PREV_INSN (x)) : 0,
		 NEXT_INSN (x) ? INSN_UID (NEXT_INSN (x)) : 0);
      if (code == BARRIER)
	fputs (" -------- barrier ---------", file);
      else if (code == CODE_LABEL)
	fputs (")", file);
      else if (code == NOTE)
	{
	  int ln = NOTE_LINE_NUMBER (x);
	  if (ln >= (int) NOTE_INSN_BIAS && ln < (int) NOTE_INSN_MAX)
	    fprintf (file, " %s", GET_NOTE_INSN_NAME (ln));
	  else
	    {
	      fprintf (file, " line %d", ln);
	      if (NOTE_SOURCE_FILE (x))
		fprintf (file, ":%s", NOTE_SOURCE_FILE (x));
	    }
	}
      else
	{
	  fprintf (file, "\t");
	  ra_print_rtx (file, PATTERN (x), 0);
	}
      return;
    }
  switch (code)
    {
      /* Top-level stuff.  */
      case PARALLEL:
	    {
	      int j;
	      for (j = 0; j < XVECLEN (x, 0); j++)
		{
		  if (j)
		    fputs ("\t;; ", file);
		  ra_print_rtx (file, XVECEXP (x, 0, j), 0);
		}
	      break;
	    }
      case UNSPEC: case UNSPEC_VOLATILE:
	    {
	      int j;
	      fprintf (file, "unspec%s(%d",
		       (code == UNSPEC) ? "" : "_vol", XINT (x, 1));
	      for (j = 0; j < XVECLEN (x, 0); j++)
		{
		  fputs (", ", file);
		  ra_print_rtx (file, XVECEXP (x, 0, j), 0);
		}
	      fputs (")", file);
	      break;
	    }
      case SET:
	  if (GET_CODE (SET_DEST (x)) == PC)
	    {
	      if (GET_CODE (SET_SRC (x)) == IF_THEN_ELSE
		  && GET_CODE (XEXP (SET_SRC(x), 2)) == PC)
		{
		  fputs ("if ", file);
		  ra_print_rtx (file, XEXP (SET_SRC (x), 0), 0);
		  fputs (" jump ", file);
		  ra_print_rtx (file, XEXP (SET_SRC (x), 1), 0);
		}
	      else
		{
		  fputs ("jump ", file);
		  ra_print_rtx (file, SET_SRC (x), 0);
		}
	    }
	  else
	    {
	      ra_print_rtx (file, SET_DEST (x), 0);
	      fputs (" <= ", file);
	      ra_print_rtx (file, SET_SRC (x), 0);
	    }
	  break;
      case USE:
	      fputs ("use <= ", file);
	      ra_print_rtx (file, XEXP (x, 0), 0);
	      break;
      case CLOBBER:
	      ra_print_rtx (file, XEXP (x, 0), 0);
	      fputs (" <= clobber", file);
	      break;
      case CALL:
	      fputs ("call ", file);
	      ra_print_rtx (file, XEXP (x, 0), 0); /* Address */
	      fputs (" numargs=", file);
	      ra_print_rtx (file, XEXP (x, 1), 0); /* Num arguments */
	      break;
      case RETURN:
	      fputs ("return", file);
	      break;
      case TRAP_IF:
	      fputs ("if (", file);
	      ra_print_rtx (file, XEXP (x, 0), 0);
	      fputs (") trap ", file);
	      ra_print_rtx (file, XEXP (x, 1), 0);
	      break;
      case RESX:
	      fprintf (file, "resx from region %d", XINT (x, 0));
	      break;

      /* Different things of class 'x' */
      case SUBREG: ra_print_rtx_object (file, x); break;
      case STRICT_LOW_PART: 
		   fputs ("low(", file);
		   ra_print_rtx (file, XEXP (x, 0), 0);
		   fputs (")", file);
		   break;
      default:
	unhandled = 1;
	break;
    }
  if (!unhandled)
    return;
  if (class == '1')
    ra_print_rtx_1op (file, x);
  else if (class == '2' || class == 'c' || class == '<')
    ra_print_rtx_2op (file, x);
  else if (class == '3' || class == 'b')
    ra_print_rtx_3op (file, x);
  else if (class == 'o')
    ra_print_rtx_object (file, x);
  else
    print_inline_rtx (file, x, 0);
}

static void
ra_print_rtx_top (file, x, with_pn)
     FILE *file;
     rtx x;
     int with_pn;
{
  ra_print_rtx (file, x, with_pn);
  fprintf (file, "\n");
}

static void
ra_debug_rtx (x)
     rtx x;
{
  ra_print_rtx_top (stderr, x, 1);
}

static void
ra_debug_bbi (bbi)
     int bbi;
{
  basic_block bb = BASIC_BLOCK (bbi);
  rtx insn;
  for (insn = bb->head; insn; insn = NEXT_INSN (insn))
    {
      ra_print_rtx_top (stderr, insn, (insn == bb->head || insn == bb->end));
      fprintf (stderr, "\n");
      if (insn == bb->end)
	break;
    }
}

static void
ra_debug_insns (insn, num)
     rtx insn;
     int num;
{
  int i, count = (num == 0 ? 1 : num < 0 ? -num : num);
  if (num < 0)
    for (i = count / 2; i > 0 && PREV_INSN (insn); i--)
      insn = PREV_INSN (insn);
  for (i = count; i > 0 && insn; insn = NEXT_INSN (insn), i--)
    {
      if (GET_CODE (insn) == CODE_LABEL)
	fprintf (stderr, "\n");
      ra_print_rtx_top (stderr, insn, (i == count || i == 1));
    }
}

static void
ra_print_rtl_with_bb (file, insn)
     FILE *file;
     rtx insn;
{
  basic_block last_bb, bb;
  unsigned int num = 0;
  if (!insn)
    fputs ("nil", file);
  last_bb = NULL;
  for (; insn; insn = NEXT_INSN (insn))
    {
      /* Bloody flow.c sometimes deletes the varray.  Bah.  */
      bb = (basic_block_for_insn) ? BLOCK_FOR_INSN (insn) : NULL;
      if (bb != last_bb)
	{
	  if (last_bb)
	    fprintf (file, ";; End of basic block %d\n", last_bb->index);
	  if (bb)
	    fprintf (file, ";; Begin of basic block %d\n", bb->index);
	  last_bb = bb;
	}
      if (GET_CODE (insn) == CODE_LABEL)
	fputc ('\n', file);
      if (GET_CODE (insn) == NOTE)
	{
	  /* Ignore basic block and maybe other notes not referencing
	     deleted things.  */
	  if (NOTE_LINE_NUMBER (insn) != NOTE_INSN_BASIC_BLOCK
	      && (flag_ra_dump_notes
		  || NOTE_LINE_NUMBER (insn) == NOTE_INSN_DELETED
		  || NOTE_LINE_NUMBER (insn) == NOTE_INSN_DELETED_LABEL))
	    {
	      ra_print_rtx_top (file, insn, (num == 0 || !NEXT_INSN (insn)));
	      num++;
	    }
	}
      else
	{
	  ra_print_rtx_top (file, insn, (num == 0 || !NEXT_INSN (insn)));
	  num++;
	}
    }
}

static int
hard_regs_count (rs)
     HARD_REG_SET rs;
{
  int count = 0;
#ifdef HARD_REG_SET
  while (rs)
    {
      unsigned char byte = rs & 0xFF;
      rs >>= 8;
      /* Avoid memory access, if nothing is set.  */
      if (byte)
        count += byte2bitcount[byte];
    }
#else
  unsigned int ofs;
  for (ofs = 0; ofs < HARD_REG_SET_LONGS; ofs++)
    {
      HARD_REG_ELT_TYPE elt = rs[ofs];
      while (elt)
	{
	  unsigned char byte = elt & 0xFF;
	  elt >>= 8;
	  if (byte)
	    count += byte2bitcount[byte];
	}
    }
#endif
  return count;
}

#define BL_TO_WORD(b, l) ((((b) & 0xFFFF) << 16) | ((l) & 0xFFFF))
#define BYTE_BEGIN(i) (((unsigned int)(i) >> 16) & 0xFFFF)
#define BYTE_LENGTH(i) ((unsigned int)(i) & 0xFFFF)
static unsigned int
rtx_to_bits (x)
     rtx x;
{
  unsigned int len, beg;
  len = GET_MODE_SIZE (GET_MODE (x));
  beg = (GET_CODE (x) == SUBREG) ? SUBREG_BYTE (x) : 0;
  return BL_TO_WORD (beg, len);
}

static unsigned HOST_WIDE_INT
rtx_to_undefined (x)
     rtx x;
{
  unsigned int len, beg;
  unsigned HOST_WIDE_INT ret;
  len = GET_MODE_SIZE (GET_MODE (x));
  beg = (GET_CODE (x) == SUBREG) ? SUBREG_BYTE (x) : 0;
  ret = ~ ((unsigned HOST_WIDE_INT) 0);
  ret = (~(ret << len)) << beg;
  return ret;
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

struct ra_insn_info
{
  unsigned int num_defs, num_uses;
  struct ref **defs, **uses;
};

static unsigned int insn_df_max_uid;
static struct ra_insn_info *insn_df;
static struct ref **refs_for_insn_df;

static void
create_insn_info (df)
     struct df *df;
{
  rtx insn;
  struct ref **act_refs;
  insn_df_max_uid = get_max_uid ();
  insn_df = xcalloc (insn_df_max_uid, sizeof (insn_df[0]));
  refs_for_insn_df = xcalloc (df->def_id + df->use_id, sizeof (struct ref *));
  act_refs = refs_for_insn_df;
  /* We create those things backwards to mimic the order in which
     the insns are visited in rewrite_program2() and live_in().  */
  for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
    {
      unsigned int uid = INSN_UID (insn);
      unsigned int n;
      struct df_link *link;
      if (!INSN_P (insn))
	continue;
      for (n = 0, link = DF_INSN_DEFS (df, insn); link; link = link->next)
        if (link->ref
	    && (DF_REF_REGNO (link->ref) >= FIRST_PSEUDO_REGISTER
		|| !TEST_HARD_REG_BIT (never_use_colors,
				       DF_REF_REGNO (link->ref))))
	  {
	    if (n == 0)
	      insn_df[uid].defs = act_refs;
	    insn_df[uid].defs[n++] = link->ref;
	  }
      act_refs += n;
      insn_df[uid].num_defs = n;
      for (n = 0, link = DF_INSN_USES (df, insn); link; link = link->next)
        if (link->ref
	    && (DF_REF_REGNO (link->ref) >= FIRST_PSEUDO_REGISTER
		|| !TEST_HARD_REG_BIT (never_use_colors,
				       DF_REF_REGNO (link->ref))))
	  {
	    if (n == 0)
	      insn_df[uid].uses = act_refs;
	    insn_df[uid].uses[n++] = link->ref;
	  }
      act_refs += n;
      insn_df[uid].num_uses = n;
    }
  if (refs_for_insn_df + (df->def_id + df->use_id) < act_refs)
    abort ();
}

static void
free_insn_info (void)
{
  free (refs_for_insn_df);
  refs_for_insn_df = NULL;
  free (insn_df);
  insn_df = NULL;
  insn_df_max_uid = 0;
}

static int
copy_insn_p (insn, source, target)
     rtx insn;
     rtx *source;
     rtx *target;
{
  rtx d, s;
  int uid = INSN_UID (insn);

  if (!INSN_P (insn))
    abort ();

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
find_sub_conflicts (wp, size_word)
     struct web_part *wp;
     unsigned int size_word;
{
  struct tagged_conflict *cl;
  cl = wp->sub_conflicts;
  for (; cl; cl = cl->next)
    if (cl->size_word == size_word)
      return cl->conflicts;
  return NULL;
}

static bitmap
get_sub_conflicts (wp, size_word)
     struct web_part *wp;
     unsigned int size_word;
{
  bitmap b = find_sub_conflicts (wp, size_word);
  if (!b)
    {
      struct tagged_conflict *cl =
	(struct tagged_conflict *) ra_alloc (sizeof *cl);
      cl->conflicts = BITMAP_XMALLOC ();
      cl->size_word = size_word;
      cl->next = wp->sub_conflicts;
      wp->sub_conflicts = cl;
      b = cl->conflicts;
    }
  return b;
}

/* Offsets and lengths are byte based.  */
struct undef_table_s {
  unsigned int new_undef;
  unsigned int size_word;      /* size | (word << 16) */
} undef_table [] = {
  { 0, BL_TO_WORD (0, 0)}, /* 0 */
  { 0, BL_TO_WORD (0, 1)},
  { 0, BL_TO_WORD (1, 1)},
  { 0, BL_TO_WORD (0, 2)},
  { 0, BL_TO_WORD (2, 1)}, /* 4 */
  { 1, BL_TO_WORD (2, 1)},
  { 2, BL_TO_WORD (2, 1)},
  { 3, BL_TO_WORD (2, 1)},
  { 0, BL_TO_WORD (3, 1)}, /* 8 */
  { 1, BL_TO_WORD (3, 1)},
  { 2, BL_TO_WORD (3, 1)},
  { 3, BL_TO_WORD (3, 1)},
  { 0, BL_TO_WORD (2, 2)}, /* 12 */
  { 1, BL_TO_WORD (2, 2)},
  { 2, BL_TO_WORD (2, 2)},
  { 0, BL_TO_WORD (0, 4)}};

static unsigned int
undef_to_size_word (undefined)
     unsigned HOST_WIDE_INT *undefined;
{
  if (*undefined <= 15)
    {
      struct undef_table_s u;
      u = undef_table[*undefined];
      *undefined = u.new_undef;
      return u.size_word;
    }
  switch (*undefined)
    {
      case 0x00f0 : *undefined = 0; return BL_TO_WORD (4, 4);
      case 0x00ff : *undefined = 0; return BL_TO_WORD (0, 8);
      case 0x0f00 : *undefined = 0; return BL_TO_WORD (8, 4);
      case 0x0ff0 : *undefined = 0xf0; return BL_TO_WORD (8, 4);
      case 0x0fff : *undefined = 0; return BL_TO_WORD (0, 12); /* XFmode */
      /* case 0x0fff : *undefined = 0xff; return BL_TO_WORD (8, 4); */
      case 0xf000 : *undefined = 0; return BL_TO_WORD (12, 4);
      case 0xff00 : *undefined = 0; return BL_TO_WORD (8, 8);
      case 0xfff0 : *undefined = 0xf0; return BL_TO_WORD (8, 8);
      case 0xffff : *undefined = 0; return BL_TO_WORD (0, 16);
      default :
	{
	  /* XXX For now unknown undefined bytes are converted to sequences
	     of maximal length 4 bytes.  We could make this larger if
	     necessary.  */
	  unsigned HOST_WIDE_INT u = *undefined;
	  int word;
	  struct undef_table_s tab;
	  for (word = 0; (u & 15) == 0; word += 4)
	    u >>= 4;
	  u = u & 15;
	  tab = undef_table[u];
	  u = tab.new_undef;
	  u = (*undefined & ~((unsigned HOST_WIDE_INT)15 << word))
	      | (u << word);
	  *undefined = u;
	  /* Size remains the same, only the begin is moved up move bytes.  */
	  return tab.size_word + BL_TO_WORD (word, 0);
	}
	break;
    }
}

static bitmap
undef_to_bitmap (wp, undefined)
     struct web_part *wp;
     unsigned HOST_WIDE_INT *undefined;
{
  unsigned int size_word = undef_to_size_word (undefined);
  return get_sub_conflicts (wp, size_word);
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
      r1->spanned_deaths += r2->spanned_deaths;

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
	      if (cl1->size_word == cl2->size_word)
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

/* Remember that we've handled a given move, so we don't reprocess it.  */
static void
remember_move (insn)
     rtx insn;
{
  if (!TEST_BIT (move_handled, INSN_UID (insn)))
    {
      rtx s, d;
      SET_BIT (move_handled, INSN_UID (insn));
      if (copy_insn_p (insn, &s, &d))
	{
	  /* Some sanity test for the copy insn. */
	  struct df_link *slink = DF_INSN_USES (df, insn);
	  struct df_link *link = DF_INSN_DEFS (df, insn);
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
	}
      else
	abort ();
      /* XXX for now we don't remember move insns involving any subregs.
	 Those would be difficult to coalesce (we would need to implement
	 handling of all the subwebs in the allocator, including that such
	 subwebs could be source and target of coalesing).  */
      if (GET_CODE (s) == REG && GET_CODE (d) == REG)
	{
	  struct move *m = (struct move *) ra_calloc (sizeof (struct move));
	  struct move_list *ml;
	  m->insn = insn;
	  ml = (struct move_list *) ra_alloc (sizeof (struct move_list));
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
  /* If some bits of this USE are live over an abnormal edge.  */
  unsigned int live_over_abnormal;
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
defuse_overlap_p_1 (def, use)
     rtx def;
     struct curr_use *use;
{
  int mode = 0;
  if (def == use->x)
    return 1;
  if (!def)
    return 0;
  if (GET_CODE (def) == SUBREG)
    {
      if (REGNO (SUBREG_REG (def)) != use->regno)
	return 0;
      mode |= 1;
    }
  else if (REGNO (def) != use->regno)
    return 0;
  if (GET_CODE (use->x) == SUBREG)
    mode |= 2;
  switch (mode)
    {
      case 0: /* REG, REG */
	return 1;
      case 1: /* SUBREG, REG */
	{
	  unsigned HOST_WIDE_INT old_u = use->undefined;
	  use->undefined &= ~ rtx_to_undefined (def);
	  return (old_u != use->undefined) ? 2 : -1;
	}
      case 2: /* REG, SUBREG */
	return 3;
      case 3: /* SUBREG, SUBREG */
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
	  use->undefined &= ~ rtx_to_undefined (def);
	  return (old_u != use->undefined) ? 4 : -1;
	}
      default:
        abort ();
    }
}

#define defuse_overlap_p(def, use) \
  ((def) == (use)->x ? 1 : \
     (REGNO (GET_CODE (def) == SUBREG \
	     ? SUBREG_REG (def) : def) != use->regno \
      ? 0 : defuse_overlap_p_1 (def, use)))

static int
live_out_1 (df, use, insn)
     struct df *df ATTRIBUTE_UNUSED;
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
      unsigned int source_regno = ~0;
      unsigned int regno = use->regno;
      unsigned HOST_WIDE_INT orig_undef = use->undefined;
      unsigned HOST_WIDE_INT final_undef = use->undefined;
      rtx s = NULL;
      unsigned int n, num_defs = insn_df[uid].num_defs;
      struct ref **defs = insn_df[uid].defs;

      wp = find_web_part (wp);
      if (GET_CODE (insn) == CALL_INSN)
	wp->crosses_call = 1;
      else if (copy_insn_p (insn, &s, NULL))
	source_regno = REGNO (GET_CODE (s) == SUBREG ? SUBREG_REG (s) : s);
      for (n = 0; n < num_defs; n++)
	{
	  struct ref *ref = defs[n];
	  int lap;
	  use->undefined = orig_undef;
	  if ((lap = defuse_overlap_p (DF_REF_REG (ref), use)) != 0)
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
				    DF_REF_ID (ref));
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
		     really independent.  Therefore we there made a
		     conflict above.  */
		}
	      /* This is at least a partial overlap, so we need to union
		 the web parts.  */
	      wp = union_web_parts (wp, &web_parts[DF_REF_ID (ref)]);
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
		  undef &= ~ rtx_to_undefined (s);
		}
	      if (undef)
		{
		  /*struct web_part *cwp;
		    cwp = find_web_part (&web_parts[DF_REF_ID
		    (ref)]);*/

		  /* TODO: somehow instead of noting the ID of the LINK
		     use an ID nearer to the root webpart of that LINK.
		     We can't use the root itself, because we later use the
		     ID to look at the form (reg or subreg, and if yes,
		     which subreg) of this conflict.  This means, that we
		     need to remember in the root an ID for each form, and
		     maintaining this, when merging web parts.  This makes
		     the bitmaps smaller.  */
		  do
		    bitmap_set_bit (undef_to_bitmap (wp, &undef),
				    DF_REF_ID (ref));
		  while (undef);
		}
	    }
	}
      if (defined)
	use->undefined = 0;
      else
	{
	  if (uid >= death_insns_max_uid)
	    abort ();
	  if (TEST_BIT (insns_with_deaths, uid))
	    wp->spanned_deaths++;
	  use->undefined = final_undef;
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

static rtx
live_in_edge (df, use, e)
     struct df *df;
     struct curr_use *use;
     edge e;
{
  struct ra_bb_info *info_pred;
  rtx next_insn;
  /* Call used hard regs die over an exception edge, ergo
     they don't reach the predecessor block, so ignore such
     uses.  And also don't set the live_over_abnormal flag
     for them.  */
  if ((e->flags & EDGE_EH) && use->regno < FIRST_PSEUDO_REGISTER
      && call_used_regs[use->regno])
    return NULL_RTX;
  if (e->flags & EDGE_ABNORMAL)
    use->live_over_abnormal = 1;
  bitmap_set_bit (live_at_end[e->src->index], DF_REF_ID (use->wp->ref));
  info_pred = (struct ra_bb_info *) e->src->aux;
  next_insn = e->src->end;
  if (live_out (df, use, next_insn))
    {
      /* If the current regno isn't mentioned anywhere in the whole block,
	 and the complete use is still undefined...  */
      if (!bitmap_bit_p (info_pred->regnos_mentioned, use->regno)
	  && (rtx_to_undefined (use->x) & ~use->undefined) == 0)
	{
	  /* ...we can hop over the whole block and defer conflict
	     creation to later.  */
	  bitmap_set_bit (info_pred->live_throughout,
			  DF_REF_ID (use->wp->ref));
	  next_insn = e->src->head;
	}
      return next_insn;
    }
  else
    return NULL_RTX;
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
      number_seen[uid]++;

      for (insn = PREV_INSN (insn); insn && !INSN_P (insn);
	   insn = PREV_INSN (insn))
	;
      if (!insn)
	return;
      if (bb != BLOCK_FOR_INSN (insn))
	{
	  edge e;
	  unsigned HOST_WIDE_INT undef = use->undefined;
	  struct ra_bb_info *info = (struct ra_bb_info *) bb->aux;
	  if ((e = bb->pred) == NULL)
	    return;
	  /* We now check, if we already traversed the predecessors of this
	     block for the current pass and the current set of undefined
	     bits.  If yes, we don't need to check the predecessors again.
	     So, conceptually this information is tagged to the first
	     insn of a basic block.  */
	  if (info->pass == loc_vpass && (undef & ~info->undefined) == 0)
	    return;
	  info->pass = loc_vpass;
	  info->undefined = undef;
	  /* All but the last predecessor are handled recursively.  */
	  for (; e->pred_next; e = e->pred_next)
	    {
	      insn = live_in_edge (df, use, e);
	      if (insn)
		live_in (df, use, insn);
	      use->undefined = undef;
	    }
	  insn = live_in_edge (df, use, e);
	  if (!insn)
	    return;
	}
      else if (!live_out (df, use, insn))
	return;
    }
}

static void
dump_number_seen (void)
{
#define N 17
  int num[N];
  int i;

  for (i = 0; i < N; i++)
    num[i] = 0;
  for (i = 0; i < get_max_uid (); i++)
    if (number_seen[i] < N - 1)
      num[number_seen[i]]++;
    else
      num[N - 1]++;
  for (i = 0; i < N - 1; i++)
    if (num[i])
      debug_msg (DUMP_PROCESS, "%d insns seen %d times\n", num[i], i);
  if (num[N - 1])
    debug_msg (DUMP_PROCESS, "%d insns seen %d and more times\n", num[i],
	       N - 1);
  debug_msg (DUMP_PROCESS, "from overall %d insns\n", get_max_uid ());
#undef N
}

static void
update_regnos_mentioned (void)
{
  int last_uid = last_max_uid;
  rtx insn;
  basic_block bb;
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	/* XXX We should also remember moves over iterations (we already
	   save the cache, but not the movelist).  */
	if (INSN_UID (insn) < last_uid)
	  {
	    if (copy_insn_p (insn, NULL, NULL))
	      remember_move (insn);
	  }
	else if ((bb = BLOCK_FOR_INSN (insn)) != NULL)
	  {
	    rtx source;
	    struct ra_bb_info *info = (struct ra_bb_info *) bb->aux;
	    bitmap mentioned = info->regnos_mentioned;
	    struct df_link *link;
	    if (copy_insn_p (insn, &source, NULL))
	      {
		remember_move (insn);
		bitmap_set_bit (mentioned,
				REGNO (GET_CODE (source) == SUBREG
				       ? SUBREG_REG (source) : source));
	      }
	    for (link = DF_INSN_DEFS (df, insn); link; link = link->next)
	      if (link->ref)
		bitmap_set_bit (mentioned, DF_REF_REGNO (link->ref));
	  }
      }
}

static void
livethrough_conflicts_bb (bb)
     basic_block bb;
{
  struct ra_bb_info *info = (struct ra_bb_info *) bb->aux;
  rtx insn;
  bitmap all_defs;
  int first, use_id;
  unsigned int deaths = 0;
  unsigned int contains_call = 0;

  if ((first = bitmap_first_set_bit (info->live_throughout)) < 0)
    return;

  all_defs = BITMAP_XMALLOC ();
  for (insn = bb->head; insn; insn = NEXT_INSN (insn))
    {
      if (INSN_P (insn))
	{
	  struct ra_insn_info info = insn_df[INSN_UID (insn)];
	  unsigned int n;
	  for (n = 0; n < info.num_defs; n++)
	    bitmap_set_bit (all_defs, DF_REF_ID (info.defs[n]));
	  if (TEST_BIT (insns_with_deaths, INSN_UID (insn)))
	    deaths++;
	  if (GET_CODE (insn) == CALL_INSN)
	    contains_call = 1;
	}
      if (insn == bb->end)
	break;
    }

  if (deaths > 0 || bitmap_first_set_bit (all_defs) >= 0)
    EXECUTE_IF_SET_IN_BITMAP (info->live_throughout, first, use_id,
      {
        struct web_part *wp = &web_parts[df->def_id + use_id];
        unsigned int bl = rtx_to_bits (DF_REF_REG (wp->ref));
        bitmap conflicts;
        wp = find_web_part (wp);
        wp->spanned_deaths += deaths;
	wp->crosses_call |= contains_call;
        conflicts = get_sub_conflicts (wp, bl);
        bitmap_operation (conflicts, conflicts, all_defs, BITMAP_IOR);
      });

  BITMAP_XFREE (all_defs);
}

static void
init_bb_info (void)
{
  int b;
  for (b = 0; b < n_basic_blocks + 2; b++)
    {
      basic_block bb = (b == n_basic_blocks) ? ENTRY_BLOCK_PTR :
	  (b == n_basic_blocks + 1) ? EXIT_BLOCK_PTR :
	  BASIC_BLOCK (b);
      struct ra_bb_info *info =
	(struct ra_bb_info *) xcalloc (1, sizeof *info);
      info->regnos_mentioned = BITMAP_XMALLOC ();
      info->live_throughout = BITMAP_XMALLOC ();
      info->old_aux = bb->aux;
      bb->aux = (void *) info;
    }
}

static void
free_bb_info (void)
{
  int b;
  for (b = 0; b < n_basic_blocks + 2; b++)
    {
      basic_block bb = (b == n_basic_blocks) ? ENTRY_BLOCK_PTR :
	  (b == n_basic_blocks + 1) ? EXIT_BLOCK_PTR :
	  BASIC_BLOCK (b);
      struct ra_bb_info *info = (struct ra_bb_info *) bb->aux;
      BITMAP_XFREE (info->regnos_mentioned);
      BITMAP_XFREE (info->live_throughout);
      bb->aux = info->old_aux;
      free (info);
    }
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

  number_seen = (int *) xcalloc (get_max_uid (), sizeof (int));
  visit_trace = (struct visit_trace *) xcalloc (get_max_uid (),
						sizeof (visit_trace[0]));
  update_regnos_mentioned ();

  /* Here's the main loop.
     It goes through all insn's, connects web parts along the way, notes
     conflicts between webparts, and remembers move instructions.  */
  visited_pass = 0;
  for (use.regno = 0; use.regno < (unsigned int)max_regno; use.regno++)
    if (use.regno >= FIRST_PSEUDO_REGISTER || !fixed_regs[use.regno])
      for (link = df->regs[use.regno].uses; link; link = link->next)
        if (link->ref)
	  {
	    struct ref *ref = link->ref;
	    rtx insn = DF_REF_INSN (ref);
	    /* Only recheck marked or new uses, or uses from hardregs.  */
	    if (use.regno >= FIRST_PSEUDO_REGISTER
		&& DF_REF_ID (ref) < last_use_id
		&& !TEST_BIT (last_check_uses, DF_REF_ID (ref)))
	      continue;
	    use.wp = &web_parts[df->def_id + DF_REF_ID (ref)];
	    use.x = DF_REF_REG (ref);
	    use.live_over_abnormal = 0;
	    use.undefined = rtx_to_undefined (use.x);
	    visited_pass++;
	    live_in (df, &use, insn);
	    if (use.live_over_abnormal)
	      SET_BIT (live_over_abnormal, DF_REF_ID (ref));
	  }

  dump_number_seen ();
  for (b = 0; b < n_basic_blocks + 2; b++)
    {
      basic_block bb = (b == n_basic_blocks) ? ENTRY_BLOCK_PTR :
	  (b == n_basic_blocks + 1) ? EXIT_BLOCK_PTR :
	  BASIC_BLOCK (b);
      struct ra_bb_info *info = (struct ra_bb_info *) bb->aux;
      livethrough_conflicts_bb (bb);
      bitmap_zero (info->live_throughout);
      info->pass = 0;
    }
  free (visit_trace);
  free (number_seen);
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
	  debug_msg (DUMP_ASM, " ASM constrain Web %d conflicts with:", web->id);
	  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
	    if (TEST_HARD_REG_BIT (conflict, c))
	      debug_msg (DUMP_ASM, " %d", c);
	  debug_msg (DUMP_ASM, "\n");
	}
    }
}

/* Deletes all hardregs from *S which are not allowed for MODE.  */
static void
prune_hardregs_for_mode (s, mode)
     HARD_REG_SET *s;
     enum machine_mode mode;
{
  /* We work by first noting _all_ hardregs for which MODE is OK (including
     it's consecutive regs), and later intersect that with *S.  
     We are not interested only in the beginning of a multi-reg, but in
     all the hardregs involved.  May be HARD_REGNO_MODE_OK() only ok's
     for beginnings.  */
  AND_HARD_REG_SET (*s, hardregs_for_mode[(int) mode]);
}

/* Initialize a single web.  */
static void
init_one_web_common (web, reg)
     struct web *web;
     rtx reg;
{
  if (GET_CODE (reg) != REG)
    abort ();
  /* web->id isn't initialized here.  */
  web->regno = REGNO (reg);
  web->orig_x = reg;
  if (!web->dlink)
    {
      web->dlink = (struct dlist *) ra_calloc (sizeof (struct dlist));
      DLIST_WEB (web->dlink) = web;
    }
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
      put_web (web, PRECOLORED);
      web->num_conflicts = INT_MAX;
      web->add_hardregs = 0;
      CLEAR_HARD_REG_SET (web->usable_regs);
      SET_HARD_REG_BIT (web->usable_regs, web->regno);
      web->num_freedom = 1;
    }
  else
    {
      HARD_REG_SET alternate;
      web->color = -1;
      put_web (web, INITIAL);
      /* add_hardregs is wrong in multi-length classes, e.g.
	 using a DFmode pseudo on x86 can result in class FLOAT_INT_REGS,
	 where, if it finally is allocated to GENERAL_REGS it needs two,
	 if allocated to FLOAT_REGS only one hardreg.  XXX */
      web->add_hardregs =
	CLASS_MAX_NREGS (web->regclass, PSEUDO_REGNO_MODE (web->regno)) - 1;
      web->num_conflicts = web->add_hardregs;
      COPY_HARD_REG_SET (web->usable_regs,
			reg_class_contents[reg_preferred_class (web->regno)]);
      COPY_HARD_REG_SET (alternate,
			reg_class_contents[reg_alternate_class (web->regno)]);
      IOR_HARD_REG_SET (web->usable_regs, alternate);
      /*IOR_HARD_REG_SET (web->usable_regs,
			reg_class_contents[reg_alternate_class
			(web->regno)]);*/
      AND_COMPL_HARD_REG_SET (web->usable_regs, never_use_colors);
      prune_hardregs_for_mode (&web->usable_regs,
			       PSEUDO_REGNO_MODE (web->regno));
      web->num_freedom = hard_regs_count (web->usable_regs);
      web->num_freedom -= web->add_hardregs;
      if (!web->num_freedom)
	abort();
    }
  COPY_HARD_REG_SET (web->orig_usable_regs, web->usable_regs);
}

static void
init_one_web (web, reg)
     struct web *web;
     rtx reg;
{
  memset (web, 0, sizeof (struct web));
  init_one_web_common (web, reg);
  web->useless_conflicts = BITMAP_XMALLOC ();
}

static void
reinit_one_web (web, reg)
     struct web *web;
     rtx reg;
{
  web->old_color = web->color + 1;
  init_one_web_common (web, reg);
  web->span_deaths = 0;
  web->spill_temp = 0;
  web->orig_spill_temp = 0;
  web->use_my_regs = 0;
  web->spill_cost = 0;
  web->was_spilled = 0;
  web->is_coalesced = 0;
  web->artificial = 0;
  web->live_over_abnormal = 0;
  web->move_related = 0;
  web->in_load = 0;
  web->target_of_spilled_move = 0;
  web->num_aliased = 0;
  if (web->type == PRECOLORED)
    {
      web->num_defs = 0;
      web->num_uses = 0;
      web->orig_spill_cost = 0;
      /*web->defs = NULL;
      web->uses = NULL;*/
    }
  CLEAR_HARD_REG_SET (web->bias_colors);
  CLEAR_HARD_REG_SET (web->prefer_colors);
  web->reg_rtx = NULL;
  web->stack_slot = NULL;
  web->pattern = NULL;
  web->alias = NULL;
  if (web->moves)
    abort ();
  if (!web->useless_conflicts)
    abort ();
}

static struct web *
find_subweb (web, reg)
     struct web *web;
     rtx reg;
{
  struct web *w;
  if (GET_CODE (reg) != SUBREG)
    abort ();
  for (w = web->subreg_next; w; w = w->subreg_next)
    if (GET_MODE (w->orig_x) == GET_MODE (reg)
	&& SUBREG_BYTE (w->orig_x) == SUBREG_BYTE (reg))
      return w;
  return NULL;
}

static struct web *
find_subweb_2 (web, size_word)
     struct web *web;
     unsigned int size_word;
{
  struct web *w = web;
  if (size_word == GET_MODE_SIZE (GET_MODE (web->orig_x)))
    /* size_word == size means BYTE_BEGIN(size_word) == 0.  */
    return web;
  for (w = web->subreg_next; w; w = w->subreg_next)
    {
      unsigned int bl = rtx_to_bits (w->orig_x);
      if (size_word == bl)
        return w;
    }
  return NULL;
}

static struct web *
find_web_for_subweb_1 (subweb)
     struct web *subweb;
{
  while (subweb->parent_web)
    subweb = subweb->parent_web;
  return subweb;
}

#define find_web_for_subweb(w) (((w)->parent_web) \
				? find_web_for_subweb_1 ((w)->parent_web) \
				: (w))

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
  w->dlink = NULL;
  w->parent_web = web;
  w->subreg_next = web->subreg_next;
  web->subreg_next = w;
  return w;
}

static struct web *
add_subweb_2 (web, size_word)
     struct web *web;
     unsigned int size_word;
{
  /* To get a correct mode for the to be produced subreg, we don't want to
     simply do a mode_for_size() for the mode_class of the whole web.  
     Suppose we deal with a CDImode web, but search for a 8 byte part.  
     Now mode_for_size() would only search in the class MODE_COMPLEX_INT
     and would find CSImode which probably is not what we want.  Instead
     we want DImode, which is in a completely other class.  For this to work
     we instead first search the already existing subwebs, and take
     _their_ modeclasses as base for a search for ourself.  */
  rtx ref_rtx = (web->subreg_next ? web->subreg_next : web)->orig_x;
  unsigned int size = BYTE_LENGTH (size_word) * BITS_PER_UNIT;
  enum machine_mode mode;
  mode = mode_for_size (size, GET_MODE_CLASS (GET_MODE (ref_rtx)), 0);
  if (mode == BLKmode)
    mode = mode_for_size (size, MODE_INT, 0);
  if (mode == BLKmode)
    abort ();
  web = add_subweb (web, gen_rtx_SUBREG (mode, web->orig_x,
					 BYTE_BEGIN (size_word)));
  web->artificial = 1;
  return web;
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
	  if (no < last_def_id && web_parts[no].ref != df->defs[no])
	    abort ();
	  web_parts[no].ref = df->defs[no];
	  /* Uplink might be set from the last iteration.  */
	  if (!web_parts[no].uplink)
	    num_webs++;
	}
      else
	/* The last iteration might have left .ref set, while df_analyse()
	   removed that ref (due to a removed copy insn) from the df->defs[]
	   array.  As we don't check for that in realloc_web_parts()
	   we do that here.  */
	web_parts[no].ref = NULL;
    }
  for (no = 0; no < df->use_id; no++)
    {
      if (df->uses[no])
	{
	  if (no < last_use_id
	      && web_parts[no + df->def_id].ref != df->uses[no])
	    abort ();
	  web_parts[no + df->def_id].ref = df->uses[no];
	  if (!web_parts[no + df->def_id].uplink)
	    num_webs++;
	}
      else
	web_parts[no + df->def_id].ref = NULL;
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

static void
copy_conflict_list (web)
     struct web *web;
{
  struct conflict_link *cl;
  if (web->orig_conflict_list || web->have_orig_conflicts)
    abort ();
  web->have_orig_conflicts = 1;
  for (cl = web->conflict_list; cl; cl = cl->next)
    {
      struct conflict_link *ncl;
      ncl = (struct conflict_link *) ra_alloc (sizeof *ncl);
      ncl->t = cl->t;
      ncl->sub = NULL;
      ncl->next = web->orig_conflict_list;
      web->orig_conflict_list = ncl;
      if (cl->sub)
	{
	  struct sub_conflict *sl, *nsl;
	  for (sl = cl->sub; sl; sl = sl->next)
	    {
	      nsl = (struct sub_conflict *) ra_alloc (sizeof *nsl);
	      nsl->s = sl->s;
	      nsl->t = sl->t;
	      nsl->next = ncl->sub;
	      ncl->sub = nsl;
	    }
	}
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
      if (remember_conflicts && !pfrom->have_orig_conflicts)
	copy_conflict_list (pfrom);
      if (!TEST_BIT (sup_igraph, (pfrom->id * num_webs + pto->id)))
	{
	  cl = (struct conflict_link *) ra_alloc (sizeof (*cl));
	  cl->t = pto;
	  cl->sub = NULL;
	  cl->next = pfrom->conflict_list;
	  pfrom->conflict_list = cl;
	  if (pto->type != SELECT && pto->type != COALESCED)
	    pfrom->num_conflicts += 1 + pto->add_hardregs;
          SET_BIT (sup_igraph, (pfrom->id * num_webs + pto->id));
	  may_delete = 0;
	}
      else
        /* We don't need to test for cl==NULL, because at this point
	   a cl with cl->t==pto is guaranteed to exist.  */
        while (cl->t != pto)
	  cl = cl->next;
      if (pfrom != from || pto != to)
	{
	  /* This is a subconflict which should be added.
	     If we inserted cl in this invocation, we really need to add this
	     subconflict.  If we did _not_ add it here, we only add the
	     subconflict, if cl already had subconflicts, because otherwise
	     this indicated, that the whole webs already conflict, which
	     means we are not interested in this subconflict.  */
	  if (!may_delete || cl->sub != NULL)
	    {
	      sl = (struct sub_conflict *) ra_alloc (sizeof (*sl));
	      sl->s = from;
	      sl->t = to;
	      sl->next = cl->sub;
	      cl->sub = sl;
	    }
	}
      else
	/* pfrom == from && pto == to means, that we are not interested
	   anymore in the subconflict list for this pair, because anyway
	   the whole webs conflict.  */
	cl->sub = NULL;
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
     for this pseudo are also pointless.  */
  if ((web1->type == PRECOLORED 
       && ! TEST_HARD_REG_BIT (web2->usable_regs, web1->regno))
      || (web2->type == PRECOLORED
	  && ! TEST_HARD_REG_BIT (web1->usable_regs, web2->regno)))
    return;
  /* Similar if the set of possible hardregs don't intersect.  This iteration
     those conflicts are useless (and would make num_conflicts wrong, because
     num_freedom is calculated from the set of possible hardregs).
     But in presence of spilling and incremental building of the graph we
     need to note all uses of webs conflicting with the spilled ones.
     Because the set of possible hardregs can change in the next round for
     spilled webs, we possibly have then conflicts with webs which would
     be excluded now (because then hardregs intersect).  But we actually
     need to check those uses, and to get hold of them, we need to remember
     also webs conflicting with this one, although not conflicting in this
     round because of non-intersecting hardregs.  */
  if (web1->type != PRECOLORED && web2->type != PRECOLORED
      && ! hard_regs_intersect_p (&web1->usable_regs, &web2->usable_regs))
    {
      struct web *p1 = find_web_for_subweb (web1);
      struct web *p2 = find_web_for_subweb (web2);
      /* We expect these to be rare enough to justify bitmaps.  And because
         we have only a special use for it, we not only the superwebs.  */
      bitmap_set_bit (p1->useless_conflicts, p2->id);
      bitmap_set_bit (p2->useless_conflicts, p1->id);
      return;
    }
  SET_BIT (igraph, index);
  add_conflict_edge (web1, web2);
  add_conflict_edge (web2, web1);
}

/* For each web W this produces the missing subwebs Wx, such that it's
   possible to exactly specify (W-Wy) for all already existing subwebs Wy.  */
static void
build_inverse_webs (web)
     struct web *web;
{
  struct web *sweb = web->subreg_next;
  unsigned HOST_WIDE_INT undef;

  undef = rtx_to_undefined (web->orig_x);
  for (; sweb; sweb = sweb->subreg_next)
    /* Only create inverses of non-artificial webs.  */
    if (!sweb->artificial)
      {
	unsigned HOST_WIDE_INT bits;
	bits = undef & ~ rtx_to_undefined (sweb->orig_x);
	while (bits)
	  {
	    unsigned int size_word = undef_to_size_word (&bits);
	    if (!find_subweb_2 (web, size_word))
	      {
		(void) add_subweb_2 (web, size_word);
	      }
	  }
      }
}

static void
copy_web (web, wl)
     struct web *web;
     struct web_link **wl;
{
  struct web *cweb = (struct web *) xmalloc (sizeof *cweb);
  struct web_link *link = (struct web_link *) ra_alloc (sizeof *link);
  link->next = *wl;
  *wl = link;
  link->web = cweb;
  *cweb = *web;
}

static void
compare_and_free_webs (link)
     struct web_link **link;
{
  struct web_link *wl;
  for (wl = *link; wl; wl = wl->next)
    {
      struct web *web1 = wl->web;
      struct web *web2 = ID2WEB (web1->id);
      if (web1->regno != web2->regno
	  || web1->crosses_call != web2->crosses_call
	  || web1->live_over_abnormal != web2->live_over_abnormal
	  || !rtx_equal_p (web1->orig_x, web2->orig_x)
	  || web1->type != web2->type
	  /* Only compare num_defs/num_uses with non-hardreg webs.
	     E.g. the number of uses of the framepointer changes due to
	     inserting spill code.  */
	  || (web1->type != PRECOLORED &&
	      (web1->num_uses != web2->num_uses
	       || web1->num_defs != web2->num_defs)))
	abort ();
      if (web1->type != PRECOLORED)
	{
	  unsigned int i;
	  for (i = 0; i < web1->num_defs; i++)
	    if (web1->defs[i] != web2->defs[i])
	      abort ();
	  for (i = 0; i < web1->num_uses; i++)
	    if (web1->uses[i] != web2->uses[i])
	      abort ();
	}
      if (web1->type == PRECOLORED)
	{
	  if (web1->defs)
	    free (web1->defs);
	  if (web1->uses)
	    free (web1->uses);
	}
      free (web1);
    }
  *link = NULL;
}

static void
init_webs_defs_uses (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  struct dlist *d;
  /* Setup and fill uses[] and defs[] arrays of the webs.  */
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      unsigned int def_i, use_i;
      struct df_link *link;
      if (web->old_web)
	continue;
      if (web->type == PRECOLORED)
	{
	  web->num_defs = web->num_uses = 0;
	  continue;
	}
      if (web->num_defs)
        web->defs = (struct ref **) xmalloc (web->num_defs *
					     sizeof (web->defs[0]));
      if (web->num_uses)
        web->uses = (struct ref **) xmalloc (web->num_uses *
					     sizeof (web->uses[0]));
      def_i = use_i = 0;
      for (link = web->temp_refs; link; link = link->next)
	{
	  if (DF_REF_REG_DEF_P (link->ref))
	    web->defs[def_i++] = link->ref;
	  else
	    web->uses[use_i++] = link->ref;
	}
      web->temp_refs = NULL;
      if (def_i != web->num_defs || use_i != web->num_uses)
	abort ();
    }
}

static unsigned int
parts_to_webs_1 (df, copy_webs, all_refs)
     struct df *df;
     struct web_link **copy_webs;
     struct df_link *all_refs;
{
  unsigned int i;
  unsigned int webnum;
  unsigned int def_id = df->def_id;
  unsigned int use_id = df->use_id;
  struct web_part *wp_first_use = &web_parts[def_id];

  /* For each root web part: create and initialize a new web,
     setup def2web[] and use2web[] for all defs and uses, and
     id2web for all new webs.  */

  webnum = 0;
  for (i = 0; i < def_id + use_id; i++)
    {
      struct web *web, *subweb;
      struct web_part *wp = &web_parts[i];
      struct ref *ref = wp->ref;
      unsigned int ref_id;
      rtx reg;
      if (!ref)
	continue;
      ref_id = i;
      if (i >= def_id)
	ref_id -= def_id;
      all_refs[i].ref = ref;
      reg = DF_REF_REG (ref);
      if (! wp->uplink)
	{
	  unsigned int newid = ~0U;
	  unsigned int old_web = 0;
	  if (ra_pass == 1)
	    {
	      web = (struct web *) xmalloc (sizeof (struct web));
	      newid = last_num_webs++;
	      init_one_web (web, GET_CODE (reg) == SUBREG
			         ? SUBREG_REG (reg) : reg);
	    }
	  else
	    {
	      /* Remember, that use2web == def2web + def_id.
		 Ergo is def2web[i] == use2web[i - def_id] for i >= def_id.
		 So we only need to look into def2web[] array.  */
	      web = def2web[i];
	      if (!web && DF_REF_REGNO (ref) < FIRST_PSEUDO_REGISTER)
		web = hardreg2web[DF_REF_REGNO (ref)];
	      if (web)
		{
		  web = find_web_for_subweb (web);
		  remove_list (web->dlink, &WEBS(INITIAL));
		  old_web = 1;
		  copy_web (web, copy_webs);
		}
	      else
		{
		  if (WEBS(FREE))
		    web = DLIST_WEB (pop_list (&WEBS(FREE)));
		  else
		    {
		      web = (struct web *) xmalloc (sizeof (struct web));
		      newid = last_num_webs++;
		    }
		}
	      /* The id is zeroed in init_one_web().  */
	      if (newid == ~0U)
		newid = web->id;
	      if (old_web)
		reinit_one_web (web, GET_CODE (reg) == SUBREG
				     ? SUBREG_REG (reg) : reg);
	      else
		init_one_web (web, GET_CODE (reg) == SUBREG
				   ? SUBREG_REG (reg) : reg);
	      web->old_web = (old_web && web->type != PRECOLORED) ? 1 : 0;
	    }
	  web->span_deaths = wp->spanned_deaths;
	  web->crosses_call = wp->crosses_call;
	  web->id = newid;
	  web->temp_refs = NULL;
	  webnum++;
	  if (web->regno < FIRST_PSEUDO_REGISTER && !hardreg2web[web->regno])
	    hardreg2web[web->regno] = web;
	  else if (web->regno < FIRST_PSEUDO_REGISTER
		   && hardreg2web[web->regno] != web)
	    abort ();
	}
      if (def2web[i] != NULL)
	{
	  web = def2web[i];
	  web = find_web_for_subweb (web);
	  if (i >= def_id
	      && TEST_BIT (live_over_abnormal, ref_id))
	    web->live_over_abnormal = 1;
	  if (!web->old_web || web->type == PRECOLORED)
	    abort ();
	  continue;
	}
      if (wp->uplink)
	{
	  struct web_part *rwp = find_web_part (wp);
	  int j = DF_REF_ID (rwp->ref);
	  if (rwp < wp_first_use)
	    web = def2web[j];
	  else
	    web = use2web[j];
	  web = find_web_for_subweb (web);
	}
      all_refs[i].next = web->temp_refs;
      web->temp_refs = &all_refs[i];
      if (web->old_web && web->type != PRECOLORED)
	abort ();
      if (GET_CODE (reg) == SUBREG)
	{
	  subweb = find_subweb (web, reg);
	  if (!subweb)
	    {
	      subweb = add_subweb (web, reg);
	      if (web->old_web)
		abort ();
	    }
	}
      else
	subweb = web;
      if (i < def_id)
	{
	  if (ra_pass > 1)
	    {
	      struct web *compare = def2web[i];
	      if (i < last_def_id)
		{
		  if (web->old_web && compare != subweb)
		    abort ();
		}
	      if (!web->old_web && compare)
		abort ();
	      if (compare && compare != subweb)
		abort ();
	    }
	  def2web[i] = subweb;
	  web->num_defs++;
	}
      else
	{
	  if (ra_pass > 1)
	    {
	      struct web *compare = use2web[ref_id];
	      if (ref_id < last_use_id)
		{
		  if (web->old_web && compare != subweb)
		    abort ();
		}
	      if (!web->old_web && compare)
		abort ();
	      if (compare && compare != subweb)
		abort ();
	    }
	  use2web[ref_id] = subweb;
	  web->num_uses++;
	  if (TEST_BIT (live_over_abnormal, ref_id))
	    web->live_over_abnormal = 1;
	}
    }
  if (webnum != num_webs)
    abort ();

  return webnum;
}

/* This builds full webs out of web parts, without relating them to each
   other (i.e. without creating the conflict edges).  */
static void
parts_to_webs (df)
     struct df *df;
{
  unsigned int i;
  unsigned int webnum;
  struct web_link *copy_webs = NULL;
  struct dlist *d;
  struct df_link *all_refs;
  num_subwebs = 0;

  all_refs = (struct df_link *) xcalloc (df->def_id + df->use_id,
					 sizeof (all_refs[0]));
  webnum = parts_to_webs_1 (df, &copy_webs, all_refs);

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (!hardreg2web[i])
      {
	struct web *web = (struct web *) xmalloc (sizeof (struct web));
	init_one_web (web, gen_rtx_REG (reg_raw_mode[i], i));
	web->id = last_num_webs++;
	hardreg2web[web->regno] = web;
      }
  num_webs = last_num_webs;

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
      struct tagged_conflict *cl;
      struct web *web;
      if (wp->uplink || !wp->ref)
	{
	  if (wp->sub_conflicts)
	    abort ();
	  continue;
	}
      web = def2web[i];
      web = find_web_for_subweb (web);
      for (cl = wp->sub_conflicts; cl; cl = cl->next)
        if (!find_subweb_2 (web, cl->size_word))
	  {
	    (void) add_subweb_2 (web, cl->size_word);
	  }
    }

  /* And now create artificial subwebs needed for representing the inverse
     of some subwebs.  This also gives IDs to all subwebs.  */
  webnum = last_num_webs;
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      if (web->subreg_next)
	{
	  struct web *sweb;
          build_inverse_webs (web);
	  for (sweb = web->subreg_next; sweb; sweb = sweb->subreg_next)
	    sweb->id = webnum++;
	}
    }

  id2web = (struct web **) xcalloc (webnum, sizeof (id2web[0]));
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      ID2WEB (web->id) = web;
      for (web = web->subreg_next; web; web = web->subreg_next)
        ID2WEB (web->id) = web;
    }
  num_subwebs = webnum - last_num_webs;
  num_allwebs = num_webs + num_subwebs;
  num_webs += num_subwebs;
  igraph = sbitmap_alloc (num_webs * num_webs / 2);
  sup_igraph = sbitmap_alloc (num_webs * num_webs);
  sbitmap_zero (igraph);
  sbitmap_zero (sup_igraph);

  init_webs_defs_uses (df);
  compare_and_free_webs (&copy_webs);
  free (all_refs);
}

/* This deletes all conflicts to and from webs which need to be renewed
   in this pass of the allocator, i.e. those which were spilled in the
   last pass.  Furthermore it also rebuilds the bitmaps for the remaining
   conflicts.  */
static void
reset_conflicts (void)
{
  unsigned int i;
  bitmap newwebs = BITMAP_XMALLOC ();
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      struct web *web = ID2WEB (i);
      /* Hardreg webs and non-old webs are new webs (which
	 need rebuilding).  */
      if (web->type == PRECOLORED || !web->old_web)
	bitmap_set_bit (newwebs, web->id);
    }
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      struct web *web = ID2WEB (i);
      struct conflict_link *cl;
      struct conflict_link **pcl;
      pcl = &(web->conflict_list);
      if (web->have_orig_conflicts)
	{
	  web->conflict_list = web->orig_conflict_list;
	  web->orig_conflict_list = NULL;
	}
      if (web->orig_conflict_list)
	abort ();
      if (web->type != PRECOLORED && !web->old_web)
 	{
	  *pcl = NULL;
 	  /* Useless conflicts will be rebuilt completely.  */
 	  if (bitmap_first_set_bit (web->useless_conflicts) >= 0)
 	    abort ();
 	}
      else
	{
 	  /* Useless conflicts with new webs will be rebuilt if they
 	     are still there.  */
 	  bitmap_operation (web->useless_conflicts, web->useless_conflicts,
 			    newwebs, BITMAP_AND_COMPL);
	  for (cl = web->conflict_list; cl; cl = cl->next)
	    {
	      if (cl->t->old_web || cl->t->type == PRECOLORED)
		{
		  *pcl = cl;
		  pcl = &(cl->next);
		  web->num_conflicts += 1 + cl->t->add_hardregs;
		  SET_BIT (sup_igraph, (web->id * num_webs + cl->t->id));
		  /* No subconflicts mean full webs conflict.  */
		  if (!cl->sub)
		    SET_BIT (igraph, igraph_index (web->id, cl->t->id));
		  else
		    /* Else only the parts in cl->sub must be in the
		       bitmap.  */
		    {
		      struct sub_conflict *sl;
		      for (sl = cl->sub; sl; sl = sl->next)
			SET_BIT (igraph, igraph_index (sl->s->id, sl->t->id));
		    }
		}
	    }
	  *pcl = NULL;
	}
      web->have_orig_conflicts = 0;
    }
  BITMAP_XFREE (newwebs);
}

static void
check_conflict_numbers (void)
{
  unsigned int i;
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = ID2WEB (i);
      int new_conf = web->add_hardregs;
      struct conflict_link *cl;
      for (cl = web->conflict_list; cl; cl = cl->next)
	if (cl->t->type != SELECT && cl->t->type != COALESCED)
	  new_conf += 1 + cl->t->add_hardregs;
      if (web->type != PRECOLORED && new_conf != web->num_conflicts)
	abort ();
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
   in reality conflict get the same color.  */
static void
conflicts_between_webs (df)
     struct df *df;
{
  unsigned int i;
  struct dlist *d;
  bitmap ignore_defs = BITMAP_XMALLOC ();
  unsigned int have_ignored;
  unsigned int *pass_cache = (unsigned int *) xcalloc (num_webs, sizeof (int));
  unsigned int pass = 0;

  if (ra_pass > 1)
    reset_conflicts ();

  /* It is possible, that in the conflict bitmaps still some defs I are noted,
     which have web_parts[I].ref being NULL.  This can happen, when from the
     last iteration the conflict bitmap for this part wasn't deleted, but a
     conflicting move insn was removed.  It's DEF is still in the conflict
     bitmap, but it doesn't exist anymore in df->defs.  To not have to check
     it in the tight loop below, we instead remember the ID's of them in a
     bitmap, and loop only over IDs which are not in it.  */
  for (i = 0; i < df->def_id; i++)
    if (web_parts[i].ref == NULL)
      bitmap_set_bit (ignore_defs, i);
  have_ignored = (bitmap_first_set_bit (ignore_defs) >= 0);

  /* Now record all conflicts between webs.  Note that we only check
     the conflict bitmaps of all defs.  Conflict bitmaps are only in
     webpart roots.  If they are in uses, those uses are roots, which
     means, that this is an uninitialized web, whose conflicts
     don't matter.  Nevertheless for hardregs we also need to check uses.
     E.g. hardregs used for argument passing have no DEF in the RTL,
     but if they have uses, they indeed conflict with all DEFs they
     overlap.  */
  for (i = 0; i < df->def_id + df->use_id; i++)
    {
      struct tagged_conflict *cl = web_parts[i].sub_conflicts;
      struct web *supweb1;
      if (!cl
	  || (i >= df->def_id
	      && DF_REF_REGNO (web_parts[i].ref) >= FIRST_PSEUDO_REGISTER))
	continue;
      supweb1 = def2web[i];
      supweb1 = find_web_for_subweb (supweb1);
      for (; cl; cl = cl->next)
        if (cl->conflicts)
	  {
	    int j;
	    struct web *web1 = find_subweb_2 (supweb1, cl->size_word);
	    if (have_ignored)
	      bitmap_operation (cl->conflicts, cl->conflicts, ignore_defs,
			        BITMAP_AND_COMPL);
	    /* We reduce the number of calls to record_conflict() with this
	       pass thing.  record_conflict() itself also has some early-out
	       optimizations, but here we can use the special properties of
	       the loop (constant web1) to reduce that even more.
	       We once used an sbitmap of already handled web indices,
	       but sbitmaps are slow to clear and bitmaps are slow to
	       set/test.  The current approach needs more memory, but
	       locality is large.  */
	    pass++;
	      
	    /* Note, that there are only defs in the conflicts bitset.  */
	    EXECUTE_IF_SET_IN_BITMAP (
	      cl->conflicts, 0, j,
	      {
		struct web *web2 = def2web[j];
		unsigned int id2 = web2->id;
		if (pass_cache[id2] != pass)
		  {
		    pass_cache[id2] = pass;
		    record_conflict (web1, web2);
		  }
	      });
	  }
    }

  free (pass_cache);
  BITMAP_XFREE (ignore_defs);

#ifdef STACK_REGS
  /* Pseudos can't go in stack regs if they are live at the beginning of
     a block that is reached by an abnormal edge.  */
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      int j;
      if (web->live_over_abnormal)
	for (j = FIRST_STACK_REG; j <= LAST_STACK_REG; j++)
	  record_conflict (web, hardreg2web[j]);
    }
#endif
}

/* Remember that a web was spilled.  */
static void
remember_web_was_spilled (web)
     struct web *web;
{
  int i;
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
  web->num_freedom = hard_regs_count (web->usable_regs);
  if (!web->num_freedom)
    abort();
  COPY_HARD_REG_SET (web->orig_usable_regs, web->usable_regs);
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
      size = hard_regs_count (test);
      if (found_size < size)
	{
          web->regclass = (enum reg_class) i;
	  found_size = size;
	}
    }

  adjust = web->add_hardregs;
  web->add_hardregs =
    CLASS_MAX_NREGS (web->regclass, PSEUDO_REGNO_MODE (web->regno)) - 1;
  web->num_freedom -= web->add_hardregs;
  if (!web->num_freedom)
    abort();
  adjust -= web->add_hardregs;
  web->num_conflicts -= adjust;
}

static void
detect_spill_temps (void)
{
  struct dlist *d;
  bitmap already = BITMAP_XMALLOC ();

  /* Detect webs used for spill temporaries.  */
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);

      /* Below only the detection of spill temporaries.  We never spill
         precolored webs, so those can't be spill temporaries.  The code above
         (remember_web_was_spilled) can't currently cope with hardregs
         anyway.  */
      if (web->regno < FIRST_PSEUDO_REGISTER)
	continue;
      /* Uninitialized webs can't be spill-temporaries.  */
      if (web->num_defs == 0)
	continue;

      /* A web with only defs and no uses can't be spilled.  Nevertheless
	 it must get a color, as it takes away an register from all webs
	 live at these defs.  So we make it a short web.  */
      if (web->num_uses == 0)
	web->spill_temp = 3;
      /* A web which was spilled last time, but for which no insns were
         emitted (can happen with IR spilling ignoring sometimes
	 all deaths).  */
      else if (web->changed)
	web->spill_temp = 1;
      /* A spill temporary has one def, one or more uses, all uses
	 are in one insn, and either the def or use insn was inserted
	 by the allocator.  */
      /* XXX not correct currently.  There might also be spill temps
	 involving more than one def.  Usually that's an additional
	 clobber in the using instruction.  We might also constrain
	 ourself to that, instead of like currently marking all
	 webs involving any spill insns at all.  */
      else
	{
	  unsigned int i;
	  int spill_involved = 0;
	  for (i = 0; i < web->num_uses && !spill_involved; i++)
	    if (DF_REF_INSN_UID (web->uses[i]) >= orig_max_uid)
	      spill_involved = 1;
	  for (i = 0; i < web->num_defs && !spill_involved; i++)
	    if (DF_REF_INSN_UID (web->defs[i]) >= orig_max_uid)
	      spill_involved = 1;

	  if (spill_involved/* && ra_pass > 2*/)
	    {
	      int num_deaths = web->span_deaths;
	      /* Mark webs involving at least one spill insn as
		 spill temps.  */
	      remember_web_was_spilled (web);
	      /* Search for insns which define and use the web in question
		 at the same time, i.e. look for rmw insns.  If these insns
		 are also deaths of other webs they might have been counted
		 as such into web->span_deaths.  But because of the rmw nature
		 of this insn it is no point where a load/reload could be
		 placed successfully (it would still conflict with the
		 dead web), so reduce the number of spanned deaths by those
		 insns.  Note that sometimes such deaths are _not_ counted,
	         so negative values can result.  */
	      bitmap_zero (already);
	      for (i = 0; i < web->num_defs; i++)
		{
		  rtx insn = web->defs[i]->insn;
		  if (TEST_BIT (insns_with_deaths, INSN_UID (insn))
		      && !bitmap_bit_p (already, INSN_UID (insn)))
		    {
		      unsigned int j;
		      bitmap_set_bit (already, INSN_UID (insn));
		      /* Only decrement it once for each insn.  */
		      for (j = 0; j < web->num_uses; j++)
			if (web->uses[j]->insn == insn)
			  {
			    num_deaths--;
			    break;
			  }
		    }
		}
	      /* But mark them specially if they could possibly be spilled,
		 either because they cross some deaths (without the above
		 mentioned ones) or calls.  */
	      if (web->crosses_call || num_deaths > 0)
		web->spill_temp = 1 * 2;
	    }
	  /* A web spanning no deaths can't be spilled either.  No loads
	     would be created for it, ergo no defs.  So the insns wouldn't
	     change making the graph not easier to color.  Make this also
	     a short web.  Don't do this if it crosses calls, as these are
	     also points of reloads.  */
	  else if (web->span_deaths == 0 && !web->crosses_call)
	    web->spill_temp = 3;
	}
      web->orig_spill_temp = web->spill_temp;
    }
  BITMAP_XFREE (already);
}

static int
memref_is_stack_slot (mem)
     rtx mem;
{
  rtx ad = XEXP (mem, 0);
  rtx x;
  if (GET_CODE (ad) != PLUS || GET_CODE (XEXP (ad, 1)) != CONST_INT)
    return 0;
  x = XEXP (ad, 0);
  if (x == frame_pointer_rtx || x == hard_frame_pointer_rtx
      || (x == arg_pointer_rtx && fixed_regs[ARG_POINTER_REGNUM])
      || x == stack_pointer_rtx)
    return 1;
  return 0;
}

static int
contains_pseudo (x)
     rtx x;
{
  const char *fmt;
  int i;
  if (GET_CODE (x) == SUBREG)
    x = SUBREG_REG (x);
  if (GET_CODE (x) == REG)
    {
      if (REGNO (x) >= FIRST_PSEUDO_REGISTER)
        return 1;
      else
	return 0;
    }

  fmt = GET_RTX_FORMAT (GET_CODE (x));
  for (i = GET_RTX_LENGTH (GET_CODE (x)) - 1; i >= 0; i--)
    if (fmt[i] == 'e')
      {
	if (contains_pseudo (XEXP (x, i)))
	  return 1;
      }
    else if (fmt[i] == 'E')
      {
	int j;
	for (j = 0; j < XVECLEN (x, i); j++)
	  if (contains_pseudo (XVECEXP (x, i, j)))
	    return 1;
      }
  return 0;
}

static int
want_to_remat (x)
     rtx x;
{
  static rtx test_insn = 0;
  int num_clobbers = 0;
  int icode;

  /* If this is a valid operand, we are OK.  If it's VOIDmode, we aren't.  */
  if (general_operand (x, GET_MODE (x)))
    return 1;

  /* Otherwise, check if we can make a valid insn from it.  First initialize
     our test insn if we haven't already.  */
  if (test_insn == 0)
    {
      test_insn
	= make_insn_raw (gen_rtx_SET (VOIDmode,
				      gen_rtx_REG (word_mode,
						   FIRST_PSEUDO_REGISTER * 2),
				      const0_rtx));
      NEXT_INSN (test_insn) = PREV_INSN (test_insn) = 0;
      ggc_add_rtx_root (&test_insn, 1);
    }

  /* Now make an insn like the one we would make when rematerializing
     the value X and see if valid.  */
  PUT_MODE (SET_DEST (PATTERN (test_insn)), GET_MODE (x));
  SET_SRC (PATTERN (test_insn)) = x;
  /* XXX For now we don't allow any clobbers to be added, not just no
     hardreg clobbers.  */
  return ((icode = recog (PATTERN (test_insn), test_insn, &num_clobbers)) >= 0
	  && (num_clobbers == 0 
	      /*|| ! added_clobbers_hard_reg_p (icode)*/));
}

static void
detect_remat_webs (void)
{
  struct dlist *d;
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      unsigned int i;
      rtx pat = NULL_RTX;
      /* Hardregs and useless webs aren't spilled -> no remat necessary.
	 Defless webs obviously also can't be rematerialized.  */
      if (web->regno < FIRST_PSEUDO_REGISTER || !web->num_defs
	  || !web->num_uses)
	continue;
      for (i = 0; i < web->num_defs; i++)
	{
	  rtx insn;
	  rtx set = single_set (insn = DF_REF_INSN (web->defs[i]));
	  rtx src;
	  if (!set)
	    break;
	  src = SET_SRC (set);
	  /* When only subregs of the web are set it isn't easily
	     rematerializable.  */
	  if (!rtx_equal_p (SET_DEST (set), web->orig_x))
	    break;
	  /* If we already have a pattern it must be equal to the current.  */
	  if (pat && !rtx_equal_p (pat, src))
	    break;
	  /* Don't do the expensive checks multiple times.  */
	  if (pat)
	    continue;
	  /* For now we allow only constant sources.  */
	  if (CONSTANT_P (src)
	      /* If the whole thing is stable already, it is a source for
		 remat, no matter how complicated (probably all needed
		 resources for it are live everywhere, and don't take
		 additional register resources).  */
	      /* XXX Currently we can't use patterns which contain
		 pseudos, _even_ if they are stable.
		 The code simply isn't prepared for that.  All those
		 operands can't be spilled (or the dependent remat webs are
		 not remat anymore), so they would be oldwebs in the next
		 iteration.  But currently oldwebs can't have their references
		 changed.  The incremental machinery barfs on that.  */
	      || (!rtx_unstable_p (src) && !contains_pseudo (src))
	      /* Additionally also memrefs to stack-slots are usefull, when
		 we created them ourself.  They might not have set their
		 unchanging flag set, but nevertheless they are stable
		 across the livetime in question.  */
	      || (GET_CODE (src) == MEM
		  && INSN_UID (insn) >= orig_max_uid
		  && memref_is_stack_slot (src)))
	    {
	      if (want_to_remat (src))
		pat = src;
	    }
	  else
	    break;
	}
      if (pat && i == web->num_defs)
	web->pattern = pat;
    }
}

static void
determine_web_costs (void)
{
  struct dlist *d;
  for (d = WEBS(INITIAL); d; d = d->next)
    {
      unsigned int i, num_loads;
      int load_cost, store_cost;
      unsigned HOST_WIDE_INT w;
      struct web *web = DLIST_WEB (d);
      if (web->type == PRECOLORED)
	continue;
      /* Get costs for one load/store.  Note that we offset them by 1,
	 because some patterns have a zero rtx_cost(), but we of course
	 still need the actual load/store insns.  With zero all those
	 webs would be the same, no matter how often and where
	 they are used.  */
      if (web->pattern)
	{
	  /* This web is rematerializable.  Beware, we set store_cost to
	     zero optimistically assuming, that we indeed don't emit any
	     stores in the spill-code addition.  This might be wrong if
	     at the point of the load not all needed resources are
	     available, in which case we emit a stack-based load, for
	     which we in turn need the according stores.  */
	  load_cost = 1 + rtx_cost (web->pattern, 0);
	  store_cost = 0;
	}
      else
	{
	  load_cost = 1 + MEMORY_MOVE_COST (GET_MODE (web->orig_x),
					    web->regclass, 1);
	  store_cost = 1 + MEMORY_MOVE_COST (GET_MODE (web->orig_x),
					     web->regclass, 0);
	}
      /* We create only loads at deaths, whose number is in span_deaths.  */
      num_loads = MIN (web->span_deaths, web->num_uses);
      for (w = 0, i = 0; i < web->num_uses; i++)
	w += DF_REF_BB (web->uses[i])->frequency + 1;
#if 0
      for (i = 0; i < web->num_defs; i++)
	w += DF_REF_BB (web->defs[i])->frequency /*+ 1*/;
      w += web->span_deaths;
      w *= web->num_uses * load_cost + web->num_defs * store_cost;
      web->spill_cost = w;
#else
      if (num_loads < web->num_uses)
	w = (w * num_loads + web->num_uses - 1) / web->num_uses;
      web->spill_cost = w * load_cost;
      if (store_cost)
	{
	  for (w = 0, i = 0; i < web->num_defs; i++)
	    w += DF_REF_BB (web->defs[i])->frequency + 1;
	  web->spill_cost += w * store_cost;
	}
#endif
      web->orig_spill_cost = web->spill_cost;
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
  /* First build all the webs itself.  They are not related with
     others yet.  */
  parts_to_webs (df);
  /* Now detect spill temporaries to initialize their usable_regs set.  */
  detect_spill_temps ();
  /* And finally relate them to each other, meaning to record all possible
     conflicts between webs (see the comment there).  */
  conflicts_between_webs (df);
  detect_remat_webs ();
  determine_web_costs ();
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
	    web = find_web_for_subweb (web);
	    if (!m->target_web || web->regno < m->target_web->regno)
	      m->target_web = web;
	  }
      for (link = DF_INSN_USES (df, m->insn); link; link = link->next)
        if (link->ref)
	  {
	    web = use2web[DF_REF_ID (link->ref)];
	    web = find_web_for_subweb (web);
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
	  if (!flag_ra_optimistic_coalescing)
	    {
	      struct move_list *test = m->source_web->moves;
	      for (; test && test->move != m; test = test->next);
	      if (! test)
		{
		  newml = (struct move_list*)
		    ra_alloc (sizeof (struct move_list));
		  newml->move = m;
		  newml->next = m->source_web->moves;
		  m->source_web->moves = newml;
		}
	      test = m->target_web->moves;
	      for (; test && test->move != m; test = test->next);
	      if (! test)
		{
		  newml = (struct move_list*)
		    ra_alloc (sizeof (struct move_list));
		  newml->move = m;
		  newml->next = m->target_web->moves;
		  m->target_web->moves = newml;
		}
	    }
	}
      else
	/* Delete this move.  */
	ml->move = NULL;
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
      if (!wp1->ref)
	continue;
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
  unsigned int i;
  if (!rtl_dump_file || (debug_new_regalloc & (DUMP_IGRAPH | DUMP_WEBS)) == 0)
    return;
  debug_msg (DUMP_IGRAPH, "conflicts:\n  ");
  for (def1 = 0; def1 < num_webs; def1++)
    {
      int num1 = num;
      for (num2=0, def2 = 0; def2 < num_webs; def2++)
        if (def1 != def2 && TEST_BIT (igraph, igraph_index (def1, def2)))
	  {
	    if (num1 == num)
	      {
	        if (SUBWEB_P (ID2WEB (def1)))
		  debug_msg (DUMP_IGRAPH, "%d (SUBREG %d, %d) with ", def1,
			     ID2WEB (def1)->regno,
			     SUBREG_BYTE (ID2WEB (def1)->orig_x));
	        else
	          debug_msg (DUMP_IGRAPH, "%d (REG %d) with ", def1,
			     ID2WEB (def1)->regno);
	      }
	    if ((num2 % 9) == 8)
	      debug_msg (DUMP_IGRAPH, "\n              ");
	    num++;
	    num2++;
	    if (SUBWEB_P (ID2WEB (def2)))
	      debug_msg (DUMP_IGRAPH, "%d(%d,%d) ", def2, ID2WEB (def2)->regno,
			 SUBREG_BYTE (ID2WEB (def2)->orig_x));
	    else
	      debug_msg (DUMP_IGRAPH, "%d(%d) ", def2, ID2WEB (def2)->regno);
	  }
      if (num1 != num)
	debug_msg (DUMP_IGRAPH, "\n  ");
    }
  debug_msg (DUMP_IGRAPH, "\n");
  for (ml = wl_moves; ml; ml = ml->next)
    if (ml->move)
      {
        debug_msg (DUMP_IGRAPH, "move: insn %d: Web %d <-- Web %d\n",
	         INSN_UID (ml->move->insn), ml->move->target_web->id,
	         ml->move->source_web->id);
      }
  debug_msg (DUMP_WEBS, "\nWebs:\n");
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = ID2WEB (i);

      debug_msg (DUMP_WEBS, "  %4d : regno %3d", i, web->regno);
      if (SUBWEB_P (web))
	{
	  debug_msg (DUMP_WEBS, " sub %d", SUBREG_BYTE (web->orig_x));
	  debug_msg (DUMP_WEBS, " par %d", find_web_for_subweb (web)->id);
	}
      debug_msg (DUMP_WEBS, " +%d (span %d, cost "
		 HOST_WIDE_INT_PRINT_DEC ") (%s)",
	         web->add_hardregs, web->span_deaths, web->spill_cost,
	         reg_class_names[web->regclass]);
      if (web->spill_temp == 1)
	debug_msg (DUMP_WEBS, " (spilltemp)");
      else if (web->spill_temp == 2)
	debug_msg (DUMP_WEBS, " (spilltem2)");
      else if (web->spill_temp == 3)
	debug_msg (DUMP_WEBS, " (short)");
      if (web->type == PRECOLORED)
        debug_msg (DUMP_WEBS, " (precolored, color=%d)", web->color);
      else if (find_web_for_subweb (web)->num_uses == 0)
	debug_msg (DUMP_WEBS, " dead");
      if (web->crosses_call)
	debug_msg (DUMP_WEBS, " xcall");
      debug_msg (DUMP_WEBS, "\n");
    }
}

static void
dump_igraph_machine (void)
{
  unsigned int i;
  
  if (!rtl_dump_file || (debug_new_regalloc & DUMP_IGRAPH_M) == 0)
    return;
  debug_msg (DUMP_IGRAPH_M, "g %d %d\n", num_webs - num_subwebs,
	     FIRST_PSEUDO_REGISTER);
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      struct web *web = ID2WEB (i);
      struct conflict_link *cl;
      int flags = 0;
      int numc = 0;
      int col = 0;
      flags = web->spill_temp & 0xF;
      flags |= ((web->type == PRECOLORED) ? 1 : 0) << 4;
      flags |= (web->add_hardregs & 0xF) << 5;
      for (cl = web->conflict_list; cl; cl = cl->next)
	if (cl->t->id < web->id)
	  numc++;
      debug_msg (DUMP_IGRAPH_M, "n %d %d %d %d %d %d %d\n",
		 web->id, web->color, flags,
		 (unsigned int)web->spill_cost, web->num_defs, web->num_uses,
		 numc);
      if (web->type != PRECOLORED)
	{
	  debug_msg (DUMP_IGRAPH_M, "s %d", web->id);
	  while (1)
	    {
	      unsigned int u = 0;
	      int n;
	      for (n = 0; n < 32 && col < FIRST_PSEUDO_REGISTER; n++, col++)
		if (TEST_HARD_REG_BIT (web->usable_regs, col))
		  u |= 1 << n;
	      debug_msg (DUMP_IGRAPH_M, " %u", u);
	      if (col >= FIRST_PSEUDO_REGISTER)
		break;
	    }
	  debug_msg (DUMP_IGRAPH_M, "\n");
	}
      if (numc)
	{
	  debug_msg (DUMP_IGRAPH_M, "c %d", web->id);
	  for (cl = web->conflict_list; cl; cl = cl->next)
	    {
	      if (cl->t->id < web->id)
		debug_msg (DUMP_IGRAPH_M, " %d", cl->t->id);
	    }
	  debug_msg (DUMP_IGRAPH_M, "\n");
	}
    }
  debug_msg (DUMP_IGRAPH_M, "e\n");
}

static void
realloc_web_parts (df)
     struct df *df;
{
  struct web_part *last_web_parts = web_parts;
  struct web **last_def2web = def2web;
  struct web **last_use2web = use2web;
  sbitmap last_live_over_abnormal = live_over_abnormal;
  unsigned int i;
  struct dlist *d;
  web_parts = (struct web_part *) xcalloc (df->def_id + df->use_id,
					   sizeof web_parts[0]);
  def2web = (struct web **) xcalloc (df->def_id + df->use_id,
				     sizeof def2web[0]);
  use2web = &def2web[df->def_id];
  live_over_abnormal = sbitmap_alloc (df->use_id);
  sbitmap_zero (live_over_abnormal);
  for (i = 0; i < last_def_id + last_use_id; i++)
    {
      struct web_part *dest = &web_parts[i < last_def_id
	  				 ? i : (df->def_id + i - last_def_id)];
      struct web_part *up;
      *dest = last_web_parts[i];
      up = dest->uplink;
      dest->uplink = NULL;
      if (up && up->ref)
	{
	  unsigned int id = DF_REF_ID (up->ref);
	  if (up < &last_web_parts[last_def_id])
	    {
	      if (df->defs[id])
	        dest->uplink = &web_parts[DF_REF_ID (up->ref)];
	    }
	  else if (df->uses[id])
	    dest->uplink = &web_parts[df->def_id + DF_REF_ID (up->ref)];
	}
    }
  for (i = 0; i < last_def_id; i++)
    {
      struct web *web = last_def2web[i];
      if (web)
	{
	  web = find_web_for_subweb (web);
	  if (web->type != FREE && web->type != PRECOLORED)
	    def2web[i] = last_def2web[i];
	}
    }
  for (i = 0; i < last_use_id; i++)
    {
      struct web *web = last_use2web[i];
      if (web)
	{
	  web = find_web_for_subweb (web);
	  if (web->type != FREE && web->type != PRECOLORED)
	    use2web[i] = last_use2web[i];
	}
      if (TEST_BIT (last_live_over_abnormal, i))
	SET_BIT (live_over_abnormal, i);
    }
  for (d = WEBS(FREE); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      struct web *wnext;
      for (web = web->subreg_next; web; web = wnext)
	{
	  wnext = web->subreg_next;
	  free (web);
	}
      DLIST_WEB (d)->subreg_next = NULL;
    }
  if (last_check_uses)
    sbitmap_difference (live_over_abnormal, live_over_abnormal,
		        last_check_uses);

  if (last_def_id || last_use_id)
    {
      sbitmap_free (last_live_over_abnormal);
      free (last_web_parts);
      free (last_def2web);
    }
  if (!last_max_uid)
    {
      /* Setup copy cache, for copy_insn_p ().  */
      copy_cache = (struct copy_p_cache *)
	xcalloc (get_max_uid (), sizeof (copy_cache[0]));
      init_bb_info ();
    }
  else
    {
      copy_cache = (struct copy_p_cache *)
	xrealloc (copy_cache, get_max_uid () * sizeof (copy_cache[0]));
      memset (&copy_cache[last_max_uid], 0,
	      (get_max_uid () - last_max_uid) * sizeof (copy_cache[0]));
    }
}

/* Allocate the memory necessary for the register allocator.  */
static void
alloc_mem (df)
     struct df *df;
{
  int i;
  move_handled = sbitmap_alloc (get_max_uid () );
/*  web_parts = (struct web_part *) xcalloc (df->def_id + df->use_id,
					   sizeof web_parts[0]);
  def2web = (struct web **) xcalloc (df->def_id + df->def_id,
  				     sizeof def2web[0]);
  use2web = &def2web[df->def_id];*/
  realloc_web_parts (df);
  if (!live_at_end)
    {
      live_at_end = (bitmap *) xmalloc ((n_basic_blocks + 2) * sizeof (bitmap));
      for (i = 0; i < n_basic_blocks + 2; i++)
	live_at_end[i] = BITMAP_XMALLOC ();
      live_at_end += 2;
    }
  create_insn_info (df);
}

/* Free the memory used by the register allocator.  */
static void
free_mem (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  struct dlist *d;
  unsigned int i;
  
  free_insn_info ();

  /* We must also cycle over subwebs.  */
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = ID2WEB (i);
      if (!web)
	abort ();
      if (i >= num_webs - num_subwebs
	  && (web->conflict_list || web->orig_conflict_list))
	abort ();
      web->moves = NULL;
    }
  for (d = WEBS(FREE); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      if (web->defs)
	free (web->defs);
      web->defs = NULL;
      if (web->uses)
	free (web->uses);
      web->uses = NULL;
      /* We can't free the subwebs here, as they are referenced from
	 def2web[], and possibly needed in the next realloc_web_parts().
	 We free them there (or in free_all_mem()).  */
    }

  for (i = 0; i < df->def_id + df->use_id; i++)
    {
      struct tagged_conflict *cl;
      for (cl = web_parts[i].sub_conflicts; cl; cl = cl->next)
	{
	  if (cl->conflicts)
	    BITMAP_XFREE (cl->conflicts);
	}
      web_parts[i].sub_conflicts = NULL;
    }

  wl_moves = NULL;
  
  free (id2web);
  free (move_handled);
  sbitmap_free (sup_igraph);
  sbitmap_free (igraph);
}

static void
free_all_mem (df)
     struct df *df;
{
  struct tagged_conflict *cl;
  unsigned int i;
  struct dlist *d;

  free_bb_info ();
  free (copy_cache);
  copy_cache = NULL;
  live_at_end -= 2;
  for (i = 0; i < (unsigned)n_basic_blocks + 2; i++)
    BITMAP_XFREE (live_at_end[i]);
  free (live_at_end);

  while ((d = pop_list (&WEBS(FREE))) != NULL)
    put_web (DLIST_WEB (d), INITIAL);
  while ((d = pop_list (&WEBS(INITIAL))) != NULL)
    {
      struct web *web =DLIST_WEB (d);
      struct web *wnext;
      web->orig_conflict_list = NULL;
      web->conflict_list = NULL;
      for (web = web->subreg_next; web; web = wnext)
	{
	  wnext = web->subreg_next;
	  free (web);
	}
      free (DLIST_WEB (d));
    }

  for (i = 0; i < df->def_id + df->use_id; i++)
    {
      for (cl = web_parts[i].sub_conflicts; cl; cl = cl->next)
	{
	  if (cl->conflicts)
	    BITMAP_XFREE (cl->conflicts);
	}
      web_parts[i].sub_conflicts = NULL;
    }
  sbitmap_free (live_over_abnormal);
  free (web_parts);
  web_parts = NULL;
  if (last_check_uses)
    sbitmap_free (last_check_uses);
  last_check_uses = NULL;
  free (def2web);
  use2web = NULL;
  def2web = NULL;
  obstack_free (&ra_obstack, NULL);
}

/* Push a node onto the front of the list.  */
static void
push_list (x, list)
     struct dlist *x;
     struct dlist **list;
{
  if (x->next || x->prev)
    abort ();
  x->next = *list;
  if (*list)
    (*list)->prev = x;
  *list = x;
}

static void
push_list_end (x, list)
     struct dlist *x;
     struct dlist **list;
{
  if (x->prev || x->next)
    abort ();
  if (!*list)
    {
      *list = x;
      return;
    }
  while ((*list)->next)
    list = &((*list)->next);
  x->prev = *list;
  (*list)->next = x;
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
  *list = NULL;
}


/* Here begins the real allocator code.  */

static struct dlist *mv_worklist, *mv_coalesced, *mv_constrained;
static struct dlist *mv_frozen, *mv_active;

static void
reset_lists (void)
{
  struct dlist *d;
  unsigned int i;
  if (WEBS(SIMPLIFY) || WEBS(SIMPLIFY_SPILL) || WEBS(SIMPLIFY_FAT)
      || WEBS(FREEZE) || WEBS(SPILL) || WEBS(SELECT))
    abort ();

  while ((d = pop_list (&WEBS(COALESCED))) != NULL)
    {
      struct web *web = DLIST_WEB (d);
      struct web *aweb = alias (web);
      /* Note, how alias() becomes invalid through the two put_web()'s
	 below.  It might set the type of a web to FREE (from COALESCED),
	 which itself is a target of aliasing (i.e. in the middle of
	 an alias chain).  We can handle this by checking also for
	 type == FREE.  Note nevertheless, that alias() is invalid
	 henceforth.  */
      if (aweb->type == SPILLED || aweb->type == FREE)
	put_web (web, FREE);
      else
	put_web (web, INITIAL);
    }
  while ((d = pop_list (&WEBS(SPILLED))) != NULL)
    put_web (DLIST_WEB (d), FREE);
  while ((d = pop_list (&WEBS(COLORED))) != NULL)
    put_web (DLIST_WEB (d), INITIAL);

  for (d = WEBS(FREE); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      BITMAP_XFREE (web->useless_conflicts);
      web->useless_conflicts = NULL;
    }

  for (i = 0; i < num_webs; i++)
    {
      struct web *web = ID2WEB (i);
      if (web->type != INITIAL && web->type != FREE && web->type != PRECOLORED)
	abort ();
    }
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
      case FREE:
      case FREEZE:
      case SPILL:
      case SPILLED:
      case COALESCED:
      case COLORED:
      case SELECT:
	push_list (web->dlink, &WEBS(type));
	break;
      case PRECOLORED:
	push_list (web->dlink, &WEBS(INITIAL));
	break;
      case SIMPLIFY:
	if (web->spill_temp)
	  push_list (web->dlink, &WEBS(type = SIMPLIFY_SPILL));
	else if (web->add_hardregs)
	  push_list (web->dlink, &WEBS(type = SIMPLIFY_FAT));
	else
	  push_list (web->dlink, &WEBS(SIMPLIFY));
	break;
      default:
	abort ();
    }
  web->type = type;
}

static void
put_web_at_end (web, type)
     struct web *web;
     enum node_type type;
{
  if (type == PRECOLORED)
    type = INITIAL;
  else if (type == SIMPLIFY)
    abort ();
  push_list_end (web->dlink, &WEBS(type));
  web->type = type;
}

static void
remove_web_from_list (web)
     struct web *web;
{
  if (web->type == PRECOLORED)
    remove_list (web->dlink, &WEBS(INITIAL));
  else
    remove_list (web->dlink, &WEBS(web->type));
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
  struct dlist *d, *d_next;
  struct move_list *ml;
  for (d = WEBS(INITIAL); d; d = d_next)
    {
      struct web *web = DLIST_WEB (d);
      d_next = d->next;
      if (web->type == PRECOLORED)
        continue;

      remove_list (d, &WEBS(INITIAL));
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
	struct move *m = ml->move;
        d = (struct dlist *) ra_calloc (sizeof (struct dlist));
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
	  remove_web_from_list (web);
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
      d = pop_list (&WEBS(SIMPLIFY));
      if (!d)
	d = pop_list (&WEBS(SIMPLIFY_FAT));
      if (!d)
	d = pop_list (&WEBS(SIMPLIFY_SPILL));
      if (!d)
	break;
      web = DLIST_WEB (d);
      debug_msg (DUMP_PROCESS, " simplifying web %3d, conflicts = %d\n",
		 web->id, web->num_conflicts);
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
      return;
    }
  for (; ml->next && ml->next->move != move; ml = ml->next) ;
  if (!ml->next)
    return;
  ml->next = ml->next->next;
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
  struct move_list *ml;
  
  seen = BITMAP_XMALLOC ();
  for (ml = u->moves; ml; ml = ml->next)
    bitmap_set_bit (seen, INSN_UID (ml->move->insn));
  for (ml = v->moves; ml; ml = ml->next)
    {
      if (! bitmap_bit_p (seen, INSN_UID (ml->move->insn)))
        {
	  ml->next = u->moves;
	  u->moves = ml;
	}
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
      remove_list (web->dlink, &WEBS(FREEZE));
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
  int color = source->color;
  int size;

  /* Normally one would think, the next test wouldn't be needed.
     We try to coalesce S and T, and S has already a color, and we checked
     when processing the insns, that both have the same mode.  So naively
     we could conclude, that of course that mode was valid for this color.
     Hah.  But there is sparc.  Before reload there are copy insns
     (e.g. the ones copying arguments to locals) which happily refer to
     colors in invalid modes.  We can't coalesce those things.  */
  if (! HARD_REGNO_MODE_OK (source->color, GET_MODE (target->orig_x)))
    return 0;

  /* Sanity for funny modes.  */
  size = HARD_REGNO_NREGS (color, GET_MODE (target->orig_x));
  if (!size)
    return 0;

  /* We can't coalesce target with a precolored register which isn't in
     usable_regs.  */
  for (i = size; i--;)
    if (TEST_HARD_REG_BIT (never_use_colors, color + i)
	|| !TEST_HARD_REG_BIT (target->usable_regs, color + i)
	/* Before usually calling ok() at all, we already test, if the
	   candidates conflict in sup_igraph.  But when wide webs are
	   coalesced to hardregs, we only test the hardweb coalesced into.
	   This is only the begin color.  When actually coalescing both,
	   it will also take the following size colors, i.e. their webs.
	   We nowhere checked if the candidate possibly conflicts with
	   one of _those_, which is possible with partial conflicts,
	   so we simply do it here (this does one bit-test more than
	   necessary, the first color).  Note, that if X is precolored
	   bit [X*num_webs + Y] can't be set (see add_conflict_edge()).  */
	|| TEST_BIT (sup_igraph,
		     target->id * num_webs + hardreg2web[color + i]->id))
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

static inline unsigned int
simplify_p (type)
     enum node_type type;
{
  return type == SIMPLIFY || type == SIMPLIFY_SPILL || type == SIMPLIFY_FAT;
}

/* Actually combine two webs, that can be coalesced.  */
static void
combine (u, v)
     struct web *u, *v;
{
  int i;
  struct conflict_link *wl;
  if (u == v || v->type == COALESCED)
    abort ();
  remove_web_from_list (v);
  put_web (v, COALESCED);
  v->alias = u;
  u->is_coalesced = 1;
  v->is_coalesced = 1;
  u->num_aliased += 1 + v->num_aliased;
  if (flag_ra_merge_spill_costs && u->type != PRECOLORED)
    u->spill_cost += v->spill_cost;
    /*u->spill_cost = MAX (u->spill_cost, v->spill_cost);*/
  merge_moves (u, v);
  /* combine add_hardregs's of U and V.  */

  for (wl = v->conflict_list; wl; wl = wl->next)
    {
      struct web *pweb = wl->t;
      if (1 /*pweb->type != SELECT && pweb->type != COALESCED*/)
	{
	  struct web *web = u;
	  int nregs = 1 + v->add_hardregs;
	  if (u->type == PRECOLORED)
	    nregs = HARD_REGNO_NREGS (u->color, GET_MODE (v->orig_x));

	  /* For precolored U's we need to make conflicts between V's
	     neighbors and as many hardregs from U as V needed if it gets
	     color U.  For now we approximate this by V->add_hardregs, which
	     could be too much in multi-length classes.  We should really
	     count how many hardregs are needed for V with color U.  When U
	     isn't precolored this loop breaks out after one iteration.  */
	  for (i = 0; i < nregs; i++)
	    {
	      if (u->type == PRECOLORED)
		web = hardreg2web[i + u->color];
	      if (wl->sub == NULL)
		record_conflict (web, pweb);
	      else
		{
		  struct sub_conflict *sl;
		  /* So, between V and PWEB there are sub_conflicts.  We
		     need to relocate those conflicts to be between WEB (==
		     U when it wasn't precolored) and PWEB.  In the case
		     only a part of V conflicted with (part of) PWEB we
		     nevertheless make the new conflict between the whole U
		     and the (part of) PWEB.  Later we might try to find in
		     U the correct subpart corresponding (by size and
		     offset) to the part of V (sl->s) which was the source
		     of the conflict.  */
		  for (sl = wl->sub; sl; sl = sl->next)
		    {
		      /* Beware: sl->s is no subweb of web (== U) but of V.
			 We try to search a corresponding subpart of U.
			 If we found none we let it conflict with the whole U.
			 Note that find_subweb() only looks for mode and
			 subreg_byte of the REG rtx but not for the pseudo
			 reg number (otherwise it would be guaranteed to
			 _not_ find any subpart).  */
		      struct web *sweb = NULL;
		      if (SUBWEB_P (sl->s))
			sweb = find_subweb (web, sl->s->orig_x);
		      if (!sweb)
			sweb = web;
		      record_conflict (sweb, sl->t);
		    }
		}
	      if (u->type != PRECOLORED)
		break;
	    }
	  if (pweb->type != SELECT && pweb->type != COALESCED)
	    decrement_degree (pweb, 1 + v->add_hardregs);
	}
    }
  
  /* Now merge the usable_regs together.  */
  /* XXX That merging might normally make it necessary to
     adjust add_hardregs, which also means to adjust neighbors.  This can
     result in making some more webs trivially colorable, (or the opposite,
     if this increases our add_hardregs).  Because we intersect the
     usable_regs it should only be possible to decrease add_hardregs.  So a
     conservative solution for now is to simply don't change it.  */
  u->use_my_regs = 1;
  AND_HARD_REG_SET (u->usable_regs, v->usable_regs);
  u->regclass = reg_class_subunion[u->regclass][v->regclass];
  /* Count number of possible hardregs.  This might make U a spillweb,
     but that could also happen, if U and V together had too many
     conflicts.  */
  u->num_freedom = hard_regs_count (u->usable_regs);
  u->num_freedom -= u->add_hardregs;
  /* The next would mean an invalid coalesced move (both webs have no
     possible hardreg in common), so abort.  */
  if (!u->num_freedom)
    abort();
  
  if (u->num_conflicts >= NUM_REGS (u)
      && (u->type == FREEZE || simplify_p (u->type)))
    {
      remove_web_from_list (u);
      put_web (u, SPILL);
    }

  /* We want the most relaxed combination of spill_temp state.
     I.e. if any was no spilltemp or a spilltemp2, the result is so too,
     otherwise if any is short, the result is too.  It remains, when both
     are normal spilltemps.  */
  if (v->spill_temp == 0)
    u->spill_temp = 0;
  else if (v->spill_temp == 2 && u->spill_temp != 0)
    u->spill_temp = 2;
  else if (v->spill_temp == 3 && u->spill_temp == 1)
    u->spill_temp = 3;
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
	  remove_list (src->dlink, &WEBS(FREEZE));
	  put_web (src, SIMPLIFY);
	}
    }
}

/* Freeze the first thing on the freeze worklist.  */
static void
freeze (void)
{
  struct dlist *d = pop_list (&WEBS(FREEZE));
  put_web (DLIST_WEB (d), SIMPLIFY);
  freeze_moves (DLIST_WEB (d));
}

static unsigned HOST_WIDE_INT (*spill_heuristic) PARAMS ((struct web *));

static unsigned HOST_WIDE_INT default_spill_heuristic PARAMS ((struct web *));

static unsigned HOST_WIDE_INT
default_spill_heuristic (web)
     struct web *web;
{
  unsigned HOST_WIDE_INT ret;
  unsigned int divisor = 1;
  /* Make coalesce targets cheaper to spill, because they will be broken
     up again into smaller parts.  */
  if (flag_ra_break_aliases)
    divisor += web->num_aliased;
  divisor += web->num_conflicts;
  ret = ((web->spill_cost << 8) + divisor - 1) / divisor;
  /* It is better to spill webs that span more insns (deaths in our
     case) than other webs with the otherwise same spill_cost.  So make
     them a little bit cheaper.  Remember that spill_cost is unsigned.  */
  if (web->span_deaths < ret)
    ret -= web->span_deaths;
  return ret;
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
  for (d = WEBS(SPILL); d; d = d->next)
    {
      struct web *w = DLIST_WEB (d);
      unsigned HOST_WIDE_INT cost = spill_heuristic (w);
      if ((!w->spill_temp) && cost < best)
	{
	  best = cost;
	  bestd = d;
	}
      /* Specially marked spill temps can be spilled.  Also coalesce
	 targets can.  Eventually they will be broken up later in the
	 colorizing process, so if we have nothing better take that.  */
      else if ((w->spill_temp == 2 || w->is_coalesced) && cost < best2)
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

  DLIST_WEB (bestd)->was_spilled = 1; /* Note the potential spill.  */
  remove_list (bestd, &WEBS(SPILL));
  put_web (DLIST_WEB (bestd), SIMPLIFY);
  freeze_moves (DLIST_WEB (bestd));
  debug_msg (DUMP_PROCESS, " potential spill web %3d, conflicts = %d\n",
	     DLIST_WEB (bestd)->id, DLIST_WEB (bestd)->num_conflicts);
}

static int
color_usable_p (c, dont_begin_colors, free_colors, mode)
     int c;
     HARD_REG_SET dont_begin_colors, free_colors;
     enum machine_mode mode;
{
  if (!TEST_HARD_REG_BIT (dont_begin_colors, c)
      && TEST_HARD_REG_BIT (free_colors, c)
      && HARD_REGNO_MODE_OK (c, mode))
    {
      int i, size;
      size = HARD_REGNO_NREGS (c, mode);
      for (i = 1; i < size && TEST_HARD_REG_BIT (free_colors, c + i); i++);
      if (i == size)
	return 1;
    }
  return 0;
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

static int
get_biased_reg (dont_begin_colors, bias, prefer_colors, free_colors, mode)
     HARD_REG_SET dont_begin_colors, bias, prefer_colors, free_colors;
     enum machine_mode mode;
{
  int c = -1;
  HARD_REG_SET s;
  if (flag_ra_biased)
    {
      COPY_HARD_REG_SET (s, dont_begin_colors);
      IOR_COMPL_HARD_REG_SET (s, bias);
      IOR_COMPL_HARD_REG_SET (s, prefer_colors);
      c = get_free_reg (s, free_colors, mode);
      if (c >= 0)
	return c;
      COPY_HARD_REG_SET (s, dont_begin_colors);
      IOR_COMPL_HARD_REG_SET (s, bias);
      c = get_free_reg (s, free_colors, mode);
      if (c >= 0)
	return c;
    }
  COPY_HARD_REG_SET (s, dont_begin_colors);
  IOR_COMPL_HARD_REG_SET (s, prefer_colors);
  c = get_free_reg (s, free_colors, mode);
  if (c >= 0)
      return c;
  c = get_free_reg (dont_begin_colors, free_colors, mode);
  return c;
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

static void
calculate_dont_begin (web, result)
     struct web *web;
     HARD_REG_SET *result;
{
  struct conflict_link *wl;
  HARD_REG_SET dont_begin;
  /* The bits set in dont_begin correspond to the hardregs, at which
     WEB may not begin.  This differs from the set of _all_ hardregs which
     are taken by WEB's conflicts in the presence of wide webs, where only
     some parts conflict with others.  */
  CLEAR_HARD_REG_SET (dont_begin);
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
		    SET_HARD_REG_BIT (dont_begin, c1);
		}
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
  COPY_HARD_REG_SET (*result, dont_begin);
}

/* This is a little bit hairy, as it tries very hard, to not constrain
   the uncolored non-spill neighbors, which need more hardregs than
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
  HARD_REG_SET colors, dont_begin;
  int c = -1;
  int bestc = -1;
  int neighbor_needs= 0;
  struct web *fat_neighbor = NULL;
  struct web *fats_parent = NULL;
  int num_fat = 0;
  int long_blocks = 0;
  int best_long_blocks = -1;
  HARD_REG_SET fat_colors;
  HARD_REG_SET bias;
  
  calculate_dont_begin (web, &dont_begin);
  CLEAR_HARD_REG_SET (bias);
  neighbor_needs = web->add_hardregs + 1;
  for (wl = web->conflict_list; wl; wl = wl->next)
    {
      struct web *w;
      struct web *ptarget = alias (wl->t);
      struct sub_conflict *sl = wl->sub;
      IOR_HARD_REG_SET (bias, ptarget->bias_colors);
      w = sl ? sl->t : wl->t;
      if (ptarget->type != COLORED && ptarget->type != PRECOLORED
	  && !ptarget->was_spilled)
        while (w)
	  {
	    if (find_web_for_subweb (w)->type != COALESCED
		&& w->add_hardregs >= neighbor_needs)
	      {
		neighbor_needs = w->add_hardregs;
		fat_neighbor = w;
		fats_parent = ptarget;
		num_fat++;
	      }
	    if (!sl)
	      break;
	    sl = sl->next;
	    w = sl ? sl->t : NULL;
	  }
    }

  debug_msg (DUMP_COLORIZE, "colorize web %d [don't begin at %s]", web->id,
             hardregset_to_string (dont_begin));
  if (num_fat)
    {
      COPY_HARD_REG_SET (fat_colors, fats_parent->usable_regs);
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
	{
	  COPY_HARD_REG_SET (colors, web->usable_regs);
	  AND_HARD_REG_SET (colors,
			    usable_regs[reg_preferred_class (web->regno)]);
	}
      else
	COPY_HARD_REG_SET (colors,
			   usable_regs[reg_preferred_class (web->regno)]);
      COPY_HARD_REG_SET (call_clobbered, colors);
      AND_HARD_REG_SET (call_clobbered, call_used_reg_set);

      if (web->old_color)
	{
	  c = web->old_color - 1;
	  if (!color_usable_p (c, dont_begin, colors,
			       PSEUDO_REGNO_MODE (web->regno)))
	    c = -1;
	}
      else
	c = -1;
      if (c < 0)
	c = get_biased_reg (dont_begin, bias, web->prefer_colors,
			    call_clobbered, PSEUDO_REGNO_MODE (web->regno));
      if (c < 0)
	c = get_biased_reg (dont_begin, bias, web->prefer_colors,
			  colors, PSEUDO_REGNO_MODE (web->regno));
      
      if (/*!web->use_my_regs &&*/ c < 0)
	{
	  if (web->use_my_regs)
	    IOR_HARD_REG_SET (colors, web->usable_regs);
	  else
	    IOR_HARD_REG_SET (colors, usable_regs
			      [reg_alternate_class (web->regno)]);
	  COPY_HARD_REG_SET (call_clobbered, colors);
	  AND_HARD_REG_SET (call_clobbered, call_used_reg_set);
	  
	  c = get_biased_reg (dont_begin, bias, web->prefer_colors,
			    call_clobbered, PSEUDO_REGNO_MODE (web->regno));
	  if (c < 0)
	    c = get_biased_reg (dont_begin, bias, web->prefer_colors,
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
	      SET_HARD_REG_BIT (dont_begin, c);
	      debug_msg (DUMP_COLORIZE, " avoid %d", c);
	    }
	  else
	    /* We found a color which doesn't destroy a block.  */
	    break;
	}
      else
	break;
    }
  debug_msg (DUMP_COLORIZE, " --> got %d", c < 0 ? bestc : c);
  if (bestc >= 0 && c < 0 && !web->was_spilled)
    {
      /* This is a non-potential-spill web, which got a color, which did
	 destroy a hardreg block for one of it's neighbors.  We color
	 this web anyway and hope for the best for the neighbor, if we are
	 a spill temp.  */
      if (1 || web->spill_temp)
        c = bestc;
      debug_msg (DUMP_COLORIZE, " [constrains neighbors]");
    }
  debug_msg (DUMP_COLORIZE, "\n");

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
	 
	 if (DLIST_WEB (d)->was_spilled < 0)
	 abort (); */
      if (hard && (!web->was_spilled || web->spill_temp))
	{
	  unsigned int loop;
	  struct web *try = NULL;
	  struct web *candidates[8];

	  debug_msg (DUMP_COLORIZE, "  *** %d spilled, although %s ***\n",
		     web->id, web->spill_temp ? "spilltemp" : "non-spill");
	  /* We make multiple passes over our conflicts, first trying to
	     spill those webs, which only got a color by chance, but
	     were potential spill ones, and if that isn't enough, in a second
	     pass also to spill normal colored webs.  If we still didn't find
	     a candidate, but we are a spill-temp, we make a third pass
	     and include also webs, which were targets for coalescing, and
	     spill those.  */
#if 0
	  for (loop = 0; (try == NULL && loop < 5); loop++)
	    for (wl = web->conflict_list; wl; wl = wl->next)
	      {
	        /* We check that it's indeed a colored web (this rules
		   out webs which are coalesced into others, but not targets
		   for coalescing).  If we are a spill-temp, and haven't
		   found a candidate in the first two passes, we also look
		   at targets of coalescing to spill.  This later probably
		   leads to more spill insertions than necessary (it would
		   be better to split the coalesce again), but spill-temps
		   _must_ get a color.  This is in difference to normal
		   non-spill webs, which we can also spill, that's why we
		   don't include coalesce targets also there.  */
		struct web *w = wl->t;
		/* If nothing worked we also check if some conflict was
		   coalesced into something other, and spill _that_.  This is
		   heavy, that's why only done at the fifth pass.  */
		if (loop > 3 && web->spill_temp)
		  w = alias (w);
	        if (w->type == COLORED 
		    && (!w->spill_temp || (loop > 2 && w->spill_temp == 2))
		    && (!w->is_coalesced || (loop > 1 && web->spill_temp))
		    && (w->was_spilled || loop > 0)
		    /*&& w->add_hardregs >= web->add_hardregs
		    && w->span_deaths > web->span_deaths*/)
		  {
		    try = w;
		    break;
		  }
	      }
#else
	  memset (candidates, 0, sizeof candidates);
#define set_cand(i, w) \
	  do { \
	      if (!candidates[(i)] \
		  || (candidates[(i)]->spill_cost < (w)->spill_cost)) \
		candidates[(i)] = (w); \
	  } while (0)
	  for (wl = web->conflict_list; wl; wl = wl->next)
	    {
	      struct web *w = wl->t;
	      struct web *aw = alias (w);
	      if (aw->type != COLORED)
		continue;
	      if (w->type == COLORED && !w->spill_temp && !w->is_coalesced
		  && w->was_spilled)
		{
		  if (w->spill_cost < web->spill_cost)
		    set_cand (0, w);
		  else if (web->spill_temp)
		    set_cand (1, w);
		}
	      if (w->type == COLORED && !w->spill_temp && !w->is_coalesced
		  && !w->was_spilled)
		{
		  if (w->spill_cost < web->spill_cost)
		    set_cand (2, w);
		  else if (web->spill_temp && web->spill_temp != 2)
		    set_cand (3, w);
		}
	      if (web->spill_temp)
		{
		  if (w->type == COLORED && w->spill_temp == 2
		      && !w->is_coalesced
		      && (w->spill_cost < web->spill_cost
			  || web->spill_temp != 2))
		    set_cand (4, w);
		  if (!aw->spill_temp)
		    set_cand (5, aw);
		  if (aw->spill_temp == 2
		      && (aw->spill_cost < web->spill_cost
			  || web->spill_temp != 2))
		    set_cand (6, aw);
		  /* For boehm-gc/misc.c.  If we are a difficult spilltemp,
		     also coalsced neighbors are a chance, _even_ if they
		     too are spilltemps.  At least their coalscing can be
		     broken up, which may be reset usable_regs, and makes
		     it easier colorable.  */
		  if (web->spill_temp != 2 && aw->is_coalesced
		      && flag_ra_optimistic_coalescing)
		    set_cand (7, aw);
		}
	    }
	  for (loop = 0; try == NULL && loop < 8; loop++)
	    if (candidates[loop])
	      try = candidates[loop];
#undef set_cand
#endif
	  if (try)
	    {
	      int old_c = try->color;
	      remove_list (try->dlink, &WEBS(COLORED));
	      put_web (try, SPILLED);
	      /* Now try to colorize us again.  Can recursively make other
		 webs also spill, until there are no more unspilled
		 neighbors.  */
	      debug_msg (DUMP_COLORIZE, "  trying to spill %d\n", try->id);
	      colorize_one_web (web, hard);
	      if (web->type != COLORED)
		{
		  /* We tried recursively to spill all already colored
		     neighbors, but we are still uncolorable.  So it made
		     no sense to spill those neighbors.  Recolor them.  */
		  remove_list (try->dlink, &WEBS(SPILLED));
		  put_web (try, COLORED);
		  try->color = old_c;
		  debug_msg (DUMP_COLORIZE, "  spilling %d was useless\n",
			     try->id);
		}
	      else
		{
		  debug_msg (DUMP_COLORIZE, "  to spill %d was a good idea\n",
			     try->id);
		  remove_list (try->dlink, &WEBS(SPILLED));
		  if (try->was_spilled)
		    colorize_one_web (try, 0);
		  else
		    colorize_one_web (try, hard - 1);
		}
	    }
	  else
	    /* No more chances to get a color, so give up hope and
	       spill us.  */
	    put_web (web, SPILLED);
	}
      else
        put_web (web, SPILLED);
    }
  else
    {
      put_web (web, COLORED);
      web->color = c;
      if (flag_ra_biased)
	{
	  int nregs = HARD_REGNO_NREGS (c, GET_MODE (web->orig_x));
	  for (wl = web->conflict_list; wl; wl = wl->next)
	    {
	      struct web *ptarget = alias (wl->t);
	      int i;
	      for (i = 0; i < nregs; i++)
		SET_HARD_REG_BIT (ptarget->bias_colors, c + i);
	    }
	}
    }
  if (web->type == SPILLED && flag_ra_optimistic_coalescing
      && web->is_coalesced)
    {
      debug_msg (DUMP_COLORIZE, "breaking aliases to web %d:", web->id);
      restore_conflicts_from_coalesce (web);
      unalias_web (web);
      insert_coalesced_conflicts ();
      debug_msg (DUMP_COLORIZE, "\n");
      remove_list (web->dlink, &WEBS(SPILLED));
      put_web (web, SELECT);
      web->color = -1;
    }
}

/* Assign the colors to the nodes on the select stack.  */
static void
assign_colors (void)
{
  struct dlist *d;
  while (WEBS(SELECT))
    {
      d = pop_list (&WEBS(SELECT));
      colorize_one_web (DLIST_WEB (d), 1);
    }

  for (d = WEBS(COALESCED); d; d = d->next)
    {
      struct web *a = alias (DLIST_WEB (d));
      DLIST_WEB (d)->color = a->color;
    }
}

static void
try_recolor_web (web)
     struct web *web;
{
  struct conflict_link *wl;
  unsigned HOST_WIDE_INT *cost_neighbors;
  unsigned int *min_color;
  int newcol, c;
  HARD_REG_SET precolored_neighbors, spill_temps;
  HARD_REG_SET possible_begin, wide_seen;
  cost_neighbors = (unsigned HOST_WIDE_INT *)
    xcalloc (FIRST_PSEUDO_REGISTER, sizeof (cost_neighbors[0]));
  /* For each hard-regs count the number of preceding hardregs, which
     would overlap this color, if used in WEB's mode.  */
  min_color = (unsigned int *) xcalloc (FIRST_PSEUDO_REGISTER, sizeof (int));
  CLEAR_HARD_REG_SET (possible_begin);
  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
    {
      int i, nregs;
      if (!HARD_REGNO_MODE_OK (c, GET_MODE (web->orig_x)))
	continue;
      nregs = HARD_REGNO_NREGS (c, GET_MODE (web->orig_x));
      for (i = 0; i < nregs; i++)
	if (!TEST_HARD_REG_BIT (web->usable_regs, c + i))
	  break;
      if (i < nregs || nregs == 0)
	continue;
      SET_HARD_REG_BIT (possible_begin, c);
      for (; nregs--;)
	if (!min_color[c + nregs])
	  min_color[c + nregs] = 1 + c;
    }
  CLEAR_HARD_REG_SET (precolored_neighbors);
  CLEAR_HARD_REG_SET (spill_temps);
  CLEAR_HARD_REG_SET (wide_seen);
  for (wl = web->conflict_list; wl; wl = wl->next)
    {
      HARD_REG_SET dont_begin;
      struct web *web2 = alias (wl->t);
      struct conflict_link *nn;
      int c1, c2;
      int wide_p = 0;
      if (wl->t->type == COALESCED || web2->type != COLORED)
	{
	  if (web2->type == PRECOLORED)
	    {
	      c1 = min_color[web2->color];
	      c1 = (c1 == 0) ? web2->color : (c1 - 1);
	      c2 = web2->color;
	      for (; c1 <= c2; c1++)
	        SET_HARD_REG_BIT (precolored_neighbors, c1);
	    }
	  continue;
	}
      /* Mark colors for which some wide webs are involved.  For
	 those the independent sets are not simply one-node graphs, so
	 they can't be recolored independ from their neighborhood.  This
	 means, that our cost calculation can be incorrect (assuming it
	 can avoid spilling a web because it thinks some colors are available,
	 although it's neighbors which itself need recoloring might take
	 away exactly those colors).  */
      if (web2->add_hardregs)
	wide_p = 1;
      for (nn = web2->conflict_list; nn && !wide_p; nn = nn->next)
	if (alias (nn->t)->add_hardregs)
	  wide_p = 1;
      calculate_dont_begin (web2, &dont_begin);
      c1 = min_color[web2->color];
      /* Note that min_color[] contains 1-based values (zero means
	 undef).  */
      c1 = c1 == 0 ? web2->color : (c1 - 1);
      c2 = web2->color + HARD_REGNO_NREGS (web2->color, GET_MODE
					   (web2->orig_x)) - 1;
      for (; c1 <= c2; c1++)
	if (TEST_HARD_REG_BIT (possible_begin, c1))
	  {
	    int nregs;
	    HARD_REG_SET colors;
	    nregs = HARD_REGNO_NREGS (c1, GET_MODE (web->orig_x));
	    COPY_HARD_REG_SET (colors, web2->usable_regs);
	    for (; nregs--;)
	      CLEAR_HARD_REG_BIT (colors, c1 + nregs);
	    if (wide_p)
	      SET_HARD_REG_BIT (wide_seen, c1);
	    if (get_free_reg (dont_begin, colors,
			      GET_MODE (web2->orig_x)) < 0)
	      {
		if (web2->spill_temp)
		  SET_HARD_REG_BIT (spill_temps, c1);
		else
		  cost_neighbors[c1] += web2->spill_cost;
	      }
	  }
    }
  newcol = -1;
  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
    if (TEST_HARD_REG_BIT (possible_begin, c)
	&& !TEST_HARD_REG_BIT (precolored_neighbors, c)
	&& !TEST_HARD_REG_BIT (spill_temps, c)
	&& (newcol == -1
	    || cost_neighbors[c] < cost_neighbors[newcol]))
      newcol = c;
  if (newcol >= 0 && cost_neighbors[newcol] < web->spill_cost)
    {
      int nregs = HARD_REGNO_NREGS (newcol, GET_MODE (web->orig_x));
      unsigned HOST_WIDE_INT cost = 0;
      int *old_colors;
      struct conflict_link *wl_next;
      debug_msg (DUMP_COLORIZE, "try to set web %d to color %d\n", web->id,
		 newcol);
      remove_list (web->dlink, &WEBS(SPILLED));
      put_web (web, COLORED);
      web->color = newcol;
      old_colors = (int *) xcalloc (num_webs, sizeof (int));
      for (wl = web->conflict_list; wl; wl = wl_next)
	{
	  struct web *web2 = alias (wl->t);
	  /* If web2 is a coalesce-target, and will become spilled
	     below in colorize_one_web(), and the current conflict wl
	     between web and web2 was only the result of that coalescing
	     this conflict will be deleted, making wl invalid.  So save
	     the next conflict right now.  Note that if web2 has indeed
	     such state, then wl->next can not be deleted in this
	     iteration.  */
	  wl_next = wl->next;
	  if (web2->type == COLORED)
	    {
	      int nregs2 = HARD_REGNO_NREGS (web2->color, GET_MODE
					     (web2->orig_x));
	      if (web->color >= web2->color + nregs2
		  || web2->color >= web->color + nregs)
		continue;
	      old_colors[web2->id] = web2->color + 1;
	      web2->color = -1;
	      remove_list (web2->dlink, &WEBS(COLORED));
	      web2->type = SELECT;
	      /* Allow webs to be spilled.  */
	      if (web2->spill_temp == 0 || web2->spill_temp == 2)
		web2->was_spilled = 1;
	      colorize_one_web (web2, 1);
	      if (web2->type == SPILLED)
		cost += web2->spill_cost;
	    }
	}
      /* The actual cost may be smaller than the guessed one, because
	 partial conflicts could result in some conflicting webs getting
	 a color, where we assumed it must be spilled.  See the comment
         above what happens, when wide webs are involved, and why in that
         case there might actually be some webs spilled although thought to
         be colorable.  */
      if (cost > cost_neighbors[newcol]
	  && !TEST_HARD_REG_BIT (wide_seen, newcol))
	abort ();
      /* But if the new spill-cost is higher than our own, then really loose.
	 Respill us and recolor neighbors as before.  */
      if (cost > web->spill_cost)
	{
	  debug_msg (DUMP_COLORIZE,
		     "reset coloring of web %d, too expensive\n", web->id);
	  remove_list (web->dlink, &WEBS(COLORED));
	  web->color = -1;
	  put_web (web, SPILLED);
	  for (wl = web->conflict_list; wl; wl = wl->next)
	    {
	      struct web *web2 = alias (wl->t);
	      if (old_colors[web2->id])
		{
		  if (web2->type == SPILLED)
		    {
		      remove_list (web2->dlink, &WEBS(SPILLED));
		      web2->color = old_colors[web2->id] - 1;
		      put_web (web2, COLORED);
		    }
		  else if (web2->type == COLORED)
		    web2->color = old_colors[web2->id] - 1;
		  else
		    abort ();
		}
	    }
	}
      free (old_colors);
    }
  free (min_color);
  free (cost_neighbors);
}

/* This ensures that all conflicts of coalesced webs are seen from
   the webs coalesced into.  combine() only adds the conflicts which
   at the time of combining were not already SELECTed or COALESCED
   to not destroy num_conflicts.  Here we add all remaining conflicts
   and thereby destroy num_conflicts.  This should be used when num_conflicts
   isn't used anymore, e.g. on a completely colored graph.  */
static void
insert_coalesced_conflicts (void)
{
  struct dlist *d;
  for (d = WEBS(COALESCED); 0 && d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      struct web *aweb = alias (web);
      struct conflict_link *wl;
      for (wl = web->conflict_list; wl; wl = wl->next)
	{
	  struct web *tweb = aweb;
	  int i;
	  int nregs = 1 + web->add_hardregs;
	  if (aweb->type == PRECOLORED)
	    nregs = HARD_REGNO_NREGS (aweb->color, GET_MODE (web->orig_x));
	  for (i = 0; i < nregs; i++)
	    {
	      if (aweb->type == PRECOLORED)
		tweb = hardreg2web[i + aweb->color];
	      /* There might be some conflict edges laying around
		 where the usable_regs don't intersect.  This can happen
		 when first some webs were coalesced and conflicts
		 propagated, then some combining narrowed usable_regs and
		 further coalescing ignored those conflicts.  Now there are
		 some edges to COALESCED webs but not to it's alias.
		 So abort only when they really should conflict.  */
	      if ((!(tweb->type == PRECOLORED
		     || TEST_BIT (sup_igraph, tweb->id * num_webs + wl->t->id))
		   || !(wl->t->type == PRECOLORED
		        || TEST_BIT (sup_igraph,
				     wl->t->id * num_webs + tweb->id)))
		  && hard_regs_intersect_p (&tweb->usable_regs,
					    &wl->t->usable_regs))
		abort ();
	      /*if (wl->sub == NULL)
		record_conflict (tweb, wl->t);
	      else
		{
		  struct sub_conflict *sl;
		  for (sl = wl->sub; sl; sl = sl->next)
		    record_conflict (tweb, sl->t);
		}*/
	      if (aweb->type != PRECOLORED)
		break;
	    }
	}
    }
}

static int
comp_webs_maxcost (w1, w2)
     const void *w1, *w2;
{
  struct web *web1 = *(struct web **)w1;
  struct web *web2 = *(struct web **)w2;
  if (web1->spill_cost > web2->spill_cost)
    return -1;
  else if (web1->spill_cost < web2->spill_cost)
    return 1;
  else
    return 0;
}

static void
recolor_spills (void)
{
  unsigned int i, num;
  struct web **order2web;
  num = num_webs - num_subwebs;
  order2web = (struct web **) xmalloc (num * sizeof (order2web[0]));
  for (i = 0; i < num; i++)
    {
      order2web[i] = id2web[i];
      /* If we aren't breaking aliases, combine() wasn't merging the
         spill_costs.  So do that here to have sane measures.  */
      if (!flag_ra_merge_spill_costs && id2web[i]->type == COALESCED)
	alias (id2web[i])->spill_cost += id2web[i]->spill_cost;
    }
  qsort (order2web, num, sizeof (order2web[0]), comp_webs_maxcost);
  insert_coalesced_conflicts ();
  dump_graph_cost (DUMP_COSTS, "before spill-recolor");
  for (i = 0; i < num; i++)
    {
      struct web *web = order2web[i];
      if (web->type == SPILLED)
	try_recolor_web (web);
    }
  /* It might have been decided in try_recolor_web() (in colorize_one_web())
     that a coalesced web should be spilled, so it was put on the
     select stack.  Those webs need recoloring again, and all remaining
     coalesced webs might need their color updated, so simply call
     assign_colors() again.  */
  assign_colors ();
  free (order2web);
}

static void
check_colors (void)
{
  unsigned int i;
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      struct web *web = id2web[i];
      struct web *aweb = alias (web);
      struct conflict_link *wl;
      int nregs, c;
      if (aweb->type == SPILLED)
	continue;
      else if (aweb->type == COLORED)
	nregs = HARD_REGNO_NREGS (aweb->color, GET_MODE (web->orig_x));
      else if (aweb->type == PRECOLORED)
	nregs = 1;
      else
	abort ();
      /* The color must be valid for the original usable_regs.  */
      for (c = 0; c < nregs; c++)
	if (!TEST_HARD_REG_BIT (web->usable_regs, aweb->color + c))
	  abort ();
      /* Search the original (pre-coalesce) conflict list.  In the current
	 one some inprecise conflicts may be noted (due to combine() or
	 insert_coalesced_conflicts() relocating partial conflicts) making
	 it look like some wide webs are in conflict and having the same
	 color.  */
      wl = (web->have_orig_conflicts ? web->orig_conflict_list
	    : web->conflict_list);
      for (; wl; wl = wl->next)
	if (!wl->sub)
	  {
	    struct web *web2 = alias (wl->t);
	    int nregs2;
	    if (web2->type == COLORED)
	      nregs2 = HARD_REGNO_NREGS (web2->color, GET_MODE (web2->orig_x));
	    else if (web2->type == PRECOLORED)
	      nregs2 = 1;
	    else
	      continue;
	    if (aweb->color >= web2->color + nregs2
	        || web2->color >= aweb->color + nregs)
	      continue;
	    abort ();
	  }
	else
	  {
	    struct sub_conflict *sl;
	    int scol = aweb->color;
	    int tcol = alias (wl->t)->color;
	    if (alias (wl->t)->type == SPILLED)
	      continue;
	    for (sl = wl->sub; sl; sl = sl->next)
	      {
		int ssize = HARD_REGNO_NREGS (scol, GET_MODE (sl->s->orig_x));
		int tsize = HARD_REGNO_NREGS (tcol, GET_MODE (sl->t->orig_x));
		int sofs = 0, tofs = 0;
	        if (SUBWEB_P (sl->t)
		    && GET_MODE_SIZE (GET_MODE (sl->t->orig_x)) >= UNITS_PER_WORD)
		  tofs = (SUBREG_BYTE (sl->t->orig_x) / UNITS_PER_WORD);
	        if (SUBWEB_P (sl->s)
		    && GET_MODE_SIZE (GET_MODE (sl->s->orig_x))
		       >= UNITS_PER_WORD)
		  sofs = (SUBREG_BYTE (sl->s->orig_x) / UNITS_PER_WORD);
		if ((tcol + tofs >= scol + sofs + ssize)
		    || (scol + sofs >= tcol + tofs + tsize))
		  continue;
		abort ();
	      }
	  }
    }
}

static void
unalias_web (web)
     struct web *web;
{
  struct dlist *d, *d_next;
  if (web->type != SPILLED)
    abort ();
  for (d = WEBS(COALESCED); d; d = d_next)
    {
      struct web *other = DLIST_WEB (d);
      d_next = d->next;
      /* Beware: Don't use alias() here.  We really want to check only
	 one level of aliasing, i.e. only break up webs directly
	 aliased to WEB, not also those aliased through other webs.  */
      if (other->alias == web)
	{
	  other->alias = NULL;
	  other->is_coalesced = 0;
	  other->color = -1;
	  /* Well, initially everything was spilled, so it isn't incorrect,
	     that also the individual parts can be spilled.
	     XXX this isn't entirely correct, as we also relaxed the
	     spill_temp flag in combine(), which might have made components
	     spill, although they were a short or spilltemp web.  */
	  other->was_spilled = 1;
	  remove_list (d, &WEBS(COALESCED));
	  /* Spilltemps must be colored right now (i.e. as early as possible),
	     other webs can be deferred to the end (the code building the
	     stack assumed that in this stage only one web was colored).  */
	  if (other->spill_temp && other->spill_temp != 2)
	    put_web (other, SELECT);
	  else
	    put_web_at_end (other, SELECT);
	  debug_msg (DUMP_COLORIZE, " %d", other->id);
	}
    }
  web->spill_temp = web->orig_spill_temp;
  web->spill_cost = web->orig_spill_cost;
  /* Beware: The following possibly widens usable_regs again.  While
     it was narrower there might have been some conflicts added which got
     ignored because of non-intersecting hardregsets.  All those conflicts
     would now matter again.  Fortunately we only add conflicts when
     coalescing, which is also the time of narrowing.  And we remove all
     those added conflicts again now that we unalias this web.
     Therefore this is safe to do.  */
  COPY_HARD_REG_SET (web->usable_regs, web->orig_usable_regs);
  web->is_coalesced = 0;
  web->num_aliased = 0;
  web->was_spilled = 1;
  /* Reset is_coalesced flag for webs which itself are target of coalescing.
     It was cleared above if it was coalesced to WEB.  */
  for (d = WEBS(COALESCED); d; d = d->next)
    DLIST_WEB (d)->alias->is_coalesced = 1;
}

/* WEB is a spilled web which was target for coalescing.
   Delete all interference edges which were added due to that coalescing,
   and break up the coalescing.  */
static void
restore_conflicts_from_coalesce (web)
     struct web *web;
{
  struct conflict_link **pcl;
  struct conflict_link *wl;
  pcl = &(web->conflict_list);
  /* No original conflict list means no conflict was added at all
     after building the graph.  So neither we nor any neighbors have
     conflicts due to this coalescing.  */
  if (!web->have_orig_conflicts)
    return;
  while (*pcl)
    {
      struct web *other = (*pcl)->t;
      for (wl = web->orig_conflict_list; wl; wl = wl->next)
	if (wl->t == other)
	  break;
      if (wl)
	{
	  /* We found this conflict also in the original list, so this
	     was no new conflict.  */
	  pcl = &((*pcl)->next);
	}
      else
	{
	  /* This is a new conflict, so delete it from us and
	     the neighbor.  */
	  struct conflict_link **opcl;
	  struct conflict_link *owl;
	  struct sub_conflict *sl;
	  wl = *pcl;
	  *pcl = wl->next;
	  if (!other->have_orig_conflicts && other->type != PRECOLORED)
	    abort ();
	  for (owl = other->orig_conflict_list; owl; owl = owl->next)
	    if (owl->t == web)
	      break;
	  if (owl)
	    abort ();
	  opcl = &(other->conflict_list);
	  while (*opcl)
	    {
	      if ((*opcl)->t == web)
		{
		  owl = *opcl;
		  *opcl = owl->next;
		  break;
		}
	      else
		{
		  opcl = &((*opcl)->next);
		}
	    }
	  if (!owl && other->type != PRECOLORED)
	    abort ();
	  /* wl and owl contain the edge data to be deleted.  */
	  RESET_BIT (sup_igraph, web->id * num_webs + other->id);
	  RESET_BIT (sup_igraph, other->id * num_webs + web->id);
	  RESET_BIT (igraph, igraph_index (web->id, other->id));
	  for (sl = wl->sub; sl; sl = sl->next)
	    RESET_BIT (igraph, igraph_index (sl->s->id, sl->t->id));
	  if (other->type != PRECOLORED)
	    {
	      for (sl = owl->sub; sl; sl = sl->next)
		RESET_BIT (igraph, igraph_index (sl->s->id, sl->t->id));
	    }
	}
    }

  /* We must restore usable_regs because record_conflict will use it.  */
  COPY_HARD_REG_SET (web->usable_regs, web->orig_usable_regs); 
  /* We might have deleted some conflicts above, which really are still
     there (diamond pattern coalescing).  This is because we don't reference
     count interference edges but some of them were the result of different
     coalesces.  */
  for (wl = web->conflict_list; wl; wl = wl->next)
    if (wl->t->type == COALESCED)
      {
	struct web *tweb;
	for (tweb = wl->t->alias; tweb; tweb = tweb->alias)
	  {
	    if (wl->sub == NULL)
	      record_conflict (web, tweb);
	    else
	      {
		struct sub_conflict *sl;
		for (sl = wl->sub; sl; sl = sl->next)
		  {
		    struct web *sweb = NULL;
		    if (SUBWEB_P (sl->t))
		      sweb = find_subweb (tweb, sl->t->orig_x);
		    if (!sweb)
		      sweb = tweb;
		    record_conflict (sl->s, sweb);
		  }
	      }
	    if (tweb->type != COALESCED)
	      break;
	  }
      }
}

static void
break_coalesced_spills (void)
{
  int changed = 0;
  while (1)
    {
      struct dlist *d;
      struct web *web;
      for (d = WEBS(SPILLED); d; d = d->next)
	if (DLIST_WEB (d)->is_coalesced)
	  break;
      if (!d)
	break;
      changed = 1;
      web = DLIST_WEB (d);
      debug_msg (DUMP_COLORIZE, "breaking aliases to web %d:", web->id);
      restore_conflicts_from_coalesce (web);
      unalias_web (web);
      /* WEB was a spilled web and isn't anymore.  Everything coalesced
	 to WEB is now SELECTed and might potentially get a color.
	 If those other webs were itself targets of coalescing it might be
	 that there are still some conflicts from aliased webs missing,
	 because they were added in combine() right into the now
	 SELECTed web.  So we need to add those missing conflicts here.  */
      insert_coalesced_conflicts ();
      debug_msg (DUMP_COLORIZE, "\n");
      remove_list (d, &WEBS(SPILLED));
      put_web (web, SELECT);
      web->color = -1;
      while (WEBS(SELECT))
	{
	  d = pop_list (&WEBS(SELECT));
	  colorize_one_web (DLIST_WEB (d), 1);
	}
    }
  if (changed)
    {
      struct dlist *d;
      for (d = WEBS(COALESCED); d; d = d->next)
	{
	  struct web *a = alias (DLIST_WEB (d));
	  DLIST_WEB (d)->color = a->color;
	}
    }
  dump_graph_cost (DUMP_COSTS, "after alias-breaking");
}

static unsigned int deleted_move_insns;
static unsigned HOST_WIDE_INT deleted_move_cost;

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
		|| TEST_BIT (sup_igraph, t->id * num_webs + s->id)
		|| s->pattern || t->pattern)
	      continue;

	    deleted_move_insns++;
	    deleted_move_cost += BLOCK_FOR_INSN (m->insn)->frequency + 1;
	    PUT_CODE (m->insn, NOTE);
	    NOTE_LINE_NUMBER (m->insn) = NOTE_INSN_DELETED;
	    df_insn_modify (df, BLOCK_FOR_INSN (m->insn), m->insn);

	    m->target_web->target_of_spilled_move = 1;
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
	       itself coalesced to something other.  */
	    if (t->type != SPILLED || s->type != SPILLED)
	      abort ();
	    remove_list (t->dlink, &WEBS(SPILLED));
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
		      {
			struct web *sweb = NULL;
			if (SUBWEB_P (sl->s))
			  sweb = find_subweb (s, sl->s->orig_x);
			if (!sweb)
			  sweb = s;
			record_conflict (sweb, sl->t);
		      }
		  }
		/* No decrement_degree here, because we already have colored
		   the graph, and don't want to insert pweb into any other
		   list.  */
		pweb->num_conflicts -= 1 + t->add_hardregs;
	      }
	  }
      }
}

static unsigned HOST_WIDE_INT
spill_prop_savings (web, spilled)
     struct web *web;
     sbitmap spilled;
{
  unsigned HOST_WIDE_INT savings = 0;
  struct move_list *ml;
  struct move *m;
  unsigned int cost;
  if (web->pattern)
    return 0;
  cost = 1 + MEMORY_MOVE_COST (GET_MODE (web->orig_x), web->regclass, 1);
  cost += 1 + MEMORY_MOVE_COST (GET_MODE (web->orig_x), web->regclass, 0);
  for (ml = wl_moves; ml; ml = ml->next)
    if ((m = ml->move) != NULL)
      {
	struct web *s = alias (m->source_web);
	struct web *t = alias (m->target_web);
	if (s != web)
	  {
	    struct web *h = s;
	    s = t;
	    t = h;
	  }
	if (s != web || !TEST_BIT (spilled, t->id) || t->pattern
	    || TEST_BIT (sup_igraph, s->id * num_webs + t->id)
	    || TEST_BIT (sup_igraph, t->id * num_webs + s->id))
	  continue;
	savings += BLOCK_FOR_INSN (m->insn)->frequency * cost;
      }
  return savings;
}

static void
spill_prop_insert (web, list, processed)
     struct web *web;
     sbitmap list, processed;
{
  struct move_list *ml;
  struct move *m;
  for (ml = wl_moves; ml; ml = ml->next)
    if ((m = ml->move) != NULL)
      {
	struct web *s = alias (m->source_web);
	struct web *t = alias (m->target_web);
	if (s != web)
	  {
	    struct web *h = s;
	    s = t;
	    t = h;
	  }
	if (s != web || t->type != COLORED || TEST_BIT (processed, t->id))
	  continue;
	SET_BIT (list, t->id);
	SET_BIT (processed, t->id);
      }
}

static int
spill_propagation (to_prop, spilled, processed)
     sbitmap to_prop, spilled, processed;
{
  int id;
  int again = 0;
  sbitmap list = sbitmap_alloc (num_webs);
  sbitmap_zero (list);
  EXECUTE_IF_SET_IN_SBITMAP (to_prop, 0, id,
    {
      spill_prop_insert (ID2WEB (id), list, processed);
    });
  sbitmap_zero (to_prop);
  while ((id = sbitmap_first_set_bit (list)) >= 0)
    {
      struct web *web = ID2WEB (id);
      RESET_BIT (list, id);
      if (spill_prop_savings (web, spilled) >= web->spill_cost)
	{
	  remove_web_from_list (web);
	  web->color = -1;
	  put_web (web, SPILLED);
	  SET_BIT (spilled, id);
	  SET_BIT (to_prop, id);
	  spill_prop_insert (web, list, processed);
	  again = 1;
	}
    }
  sbitmap_free (list);
  return again;
}

static void
spill_coalprop (void)
{
  sbitmap spilled, processed, to_prop;
  struct dlist *d;
  int again;
  spilled = sbitmap_alloc (num_webs);
  processed = sbitmap_alloc (num_webs);
  to_prop = sbitmap_alloc (num_webs);
  sbitmap_zero (spilled);
  for (d = WEBS(SPILLED); d; d = d->next)
    SET_BIT (spilled, DLIST_WEB (d)->id);
  sbitmap_copy (to_prop, spilled);
  sbitmap_zero (processed);
  do
    {
      spill_coalescing (to_prop, spilled);
      /* XXX Currently (with optimistic coalescing) spill_propagation()
	 doesn't give better code, sometimes it gives worse (but not by much)
	 code.  I believe this is because of slightly wrong cost
	 measurements.  Anyway right now it isn't worth the time it takes,
	 so deactivate it for now.  */
      again = 0 && spill_propagation (to_prop, spilled, processed);
    }
  while (again);
  sbitmap_free (to_prop);
  sbitmap_free (processed);
  sbitmap_free (spilled);
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

/* This chooses a color for all SPILLED webs for interference region
   spilling.  The heuristic isn't good in any way.  */
static void
choose_spill_colors (void)
{
  struct dlist *d;
  unsigned HOST_WIDE_INT *costs = (unsigned HOST_WIDE_INT *)
    xmalloc (FIRST_PSEUDO_REGISTER * sizeof (costs[0]));
  for (d = WEBS(SPILLED); d; d = d->next)
    {
      struct web *web = DLIST_WEB (d);
      struct conflict_link *wl;
      int bestc, c;
      HARD_REG_SET avail;
      memset (costs, 0, FIRST_PSEUDO_REGISTER * sizeof (costs[0]));
      for (wl = web->conflict_list; wl; wl = wl->next)
	{
	  struct web *pweb = wl->t;
	  /*if (pweb->color >= 0)*/
	  if (pweb->type == COLORED || pweb->type == PRECOLORED)
	    costs[pweb->color] += pweb->spill_cost;
	}

      COPY_HARD_REG_SET (avail, web->usable_regs);
      if (web->crosses_call)
	{
	  /* Add an arbitrary constant cost to colors not usable by
	     call-crossing webs without saves/loads.  */
	  for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
	    if (TEST_HARD_REG_BIT (call_used_reg_set, c))
	      costs[c] += 1000;
	}
      bestc = -1;
      for (c = 0; c < FIRST_PSEUDO_REGISTER; c++)
	if ((bestc < 0 || costs[bestc] > costs[c])
            && TEST_HARD_REG_BIT (avail, c)
	    && HARD_REGNO_MODE_OK (c, PSEUDO_REGNO_MODE (web->regno)))
	  {
	    int i, size;
	    size = HARD_REGNO_NREGS (c, PSEUDO_REGNO_MODE (web->regno));
	    for (i = 1; i < size
		 && TEST_HARD_REG_BIT (avail, c + i); i++);
	    if (i == size)
	      bestc = c;
	  }
      web->color = bestc;
      debug_msg (DUMP_PROCESS, "choosing color %d for spilled web %d\n",
		 bestc, web->id);
    }

  free (costs);
}

static unsigned int emitted_spill_loads;
static unsigned int emitted_spill_stores;
static unsigned int emitted_remat;
static unsigned HOST_WIDE_INT spill_load_cost;
static unsigned HOST_WIDE_INT spill_store_cost;
static unsigned HOST_WIDE_INT spill_remat_cost;
static bitmap useless_defs;

/* Rewrite the program to include the spill code.  */
static void
rewrite_program (new_deaths)
     bitmap new_deaths;
{
  unsigned int i;
  struct dlist *d;
  bitmap b = BITMAP_XMALLOC ();
  for (i = 0; i < 2; i++)
    for (d = (i == 0) ? WEBS(SPILLED) : WEBS(COALESCED); d; d = d->next)
      {
	struct web *web = DLIST_WEB (d);
	struct web *aweb = alias (web);
	unsigned int j;
	rtx slot;
	if (aweb->type != SPILLED)
	  continue;
      
	if (flag_ra_spill_every_use)
	  {
	    bitmap_clear (b);
	    allocate_spill_web (aweb);
	    slot = aweb->stack_slot;
	    for (j = 0; j < web->num_uses; j++)
	      {
		rtx insns, target, source;
		rtx insn = DF_REF_INSN (web->uses[j]);
		rtx prev = PREV_INSN (insn);
		basic_block bb = BLOCK_FOR_INSN (insn);
		/* Happens when spill_coalescing() deletes move insns.  */
		if (!INSN_P (insn))
		  continue;
	        if (bitmap_bit_p (b, INSN_UID (insn)))
		  continue;
	        bitmap_set_bit (b, INSN_UID (insn));
	        target = DF_REF_REG (web->uses[j]);
	        source = slot;
		start_sequence ();
	        if (GET_CODE (target) == SUBREG)
		  source = adjust_address (source, GET_MODE (target),
					   SUBREG_BYTE (target));
		emit_move_insn (target, source);
		insns = get_insns ();
		end_sequence ();
		emit_insns_before (insns, insn);

	        if (bb->head == insn)
		  bb->head = NEXT_INSN (prev);
		for (insn = PREV_INSN (insn); insn != prev;
		     insn = PREV_INSN (insn))
		  {
		    set_block_for_insn (insn, bb);
		    df_insn_modify (df, bb, insn);
		  }
		
		emitted_spill_loads++;
		spill_load_cost += bb->frequency + 1;
	      }
	  }
	
	/* If any uses were loaded from stackslots (compared to
	   rematerialized or not reloaded due to IR spilling),
	   aweb->stack_slot will be set.  If not, we don't need to emit
	   any stack stores.  */
	slot = aweb->stack_slot;
	bitmap_clear (b);
	if (slot)
	  for (j = 0; j < web->num_defs; j++)
	    {
	      rtx insns, source, dest;
	      rtx insn = DF_REF_INSN (web->defs[j]);
	      rtx following = NEXT_INSN (insn);
	      basic_block bb = BLOCK_FOR_INSN (insn);
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
		  dest = adjust_address (dest, GET_MODE (source),
					 SUBREG_BYTE (source));
		  emit_move_insn (dest, source);
		}
	      else
		{
		  /*if (! bitmap_bit_p (useless_defs, DF_REF_ID (web->defs[j]))
		      || !validate_change (insn, DF_REF_LOC (web->defs[j]),
					   slot, 0))*/
/*		  rtx reg = gen_reg_rtx (GET_MODE (source));
		  if (validate_change (insn, DF_REF_LOC (web->defs[j]),
				       reg, 0))
		    emit_insn (gen_move_insn (dest, reg));
		  else*/
		    emit_move_insn (dest, source);
		}
		
	      insns = get_insns ();
	      end_sequence ();
	      if (insns)
		{
		  emit_insns_after (insns, insn);
		  if (bb->end == insn)
		    bb->end = PREV_INSN (following);
		  for (insn = insns; insn != following; insn = NEXT_INSN (insn))
		    {
		      set_block_for_insn (insn, bb);
		      df_insn_modify (df, bb, insn);
		    }
		}
	      else
		df_insn_modify (df, bb, insn);
	      emitted_spill_stores++;
	      spill_store_cost += bb->frequency + 1;
	      /* XXX we should set new_deaths for all inserted stores
		 whose pseudo dies here.
		 Note, that this isn't the case for _all_ stores.  */
	      /* I.e. the next is wrong, and might cause some spilltemps
		 to be categorized as spilltemp2's (i.e. live over a death),
		 although they aren't.  This might make them spill again,
		 which causes endlessness in the case, this insn is in fact
		 _no_ death.  */
	      bitmap_set_bit (new_deaths, INSN_UID (PREV_INSN (following)));
	    }
      }

  BITMAP_XFREE (b);

  /*if (! validate_change (insn, df->defs[i]->loc, web->stack_slot, 0)) */
}

struct rtx_list
{
  struct rtx_list *next;
  rtx x;
};

static void
remember_slot (list, x)
     struct rtx_list **list;
     rtx x;
{
  struct rtx_list *l;
  /* PRE: X is not already in LIST.  */
  l = (struct rtx_list *) ra_alloc (sizeof (*l));
  l->next = *list;
  l->x = x;
  *list = l;
}

static int
slots_overlap_p (s1, s2)
     rtx s1, s2;
{
  rtx base1, base2;
  HOST_WIDE_INT ofs1, ofs2;
  int size1 = GET_MODE_SIZE (GET_MODE (s1));
  int size2 = GET_MODE_SIZE (GET_MODE (s2));
  if (GET_CODE (s1) != MEM || GET_CODE (s2) != MEM)
    abort ();
  s1 = XEXP (s1, 0);
  s2 = XEXP (s2, 0);
  if (GET_CODE (s1) != PLUS || GET_CODE (XEXP (s1, 0)) != REG
      || GET_CODE (XEXP (s1, 1)) != CONST_INT)
    return 1;
  if (GET_CODE (s2) != PLUS || GET_CODE (XEXP (s2, 0)) != REG
      || GET_CODE (XEXP (s2, 1)) != CONST_INT)
    return 1;
  base1 = XEXP (s1, 0);
  base2 = XEXP (s2, 0);
  if (!rtx_equal_p (base1, base2))
    return 1;
  ofs1 = INTVAL (XEXP (s1, 1));
  ofs2 = INTVAL (XEXP (s2, 1));
  if (ofs1 >= ofs2 + size2 || ofs2 >= ofs1 + size1)
    return 0;
  return 1;
}

static void
delete_overlapping_slots (list, x)
     struct rtx_list **list;
     rtx x;
{
  while (*list)
    {
      if (slots_overlap_p ((*list)->x, x))
	*list = (*list)->next;
      else
	list = &((*list)->next);
    }
}

static int
slot_member_p (list, x)
     struct rtx_list *list;
     rtx x;
{
  for (;list; list = list->next)
    if (rtx_equal_p (list->x, x))
      return 1;
  return 0;
}

static void
insert_stores (new_deaths)
     bitmap new_deaths;
{
  rtx insn;
  rtx last_slot = NULL_RTX;
  struct rtx_list *slots = NULL;
  for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
    {
      unsigned int uid = INSN_UID (insn);
      if (/*GET_CODE (insn) == CODE_LABEL || */GET_CODE (insn) == BARRIER
	  || JUMP_P (insn))
	{
	  /* Clear any info about already emitted stores.  */
	  last_slot = NULL_RTX;
	  slots = NULL;
	}
      if (!INSN_P (insn))
	continue;
      if (uid < insn_df_max_uid)
	{
	  unsigned int n;
	  struct ra_insn_info info = insn_df[uid];
	  rtx following = NEXT_INSN (insn);
	  basic_block bb = BLOCK_FOR_INSN (insn);
	  for (n = 0; n < info.num_defs; n++)
	    {
	      struct web *web = def2web[DF_REF_ID (info.defs[n])];
	      struct web *aweb = alias (find_web_for_subweb (web));
	      rtx slot, source;
	      if (aweb->type != SPILLED || !aweb->stack_slot)
		continue;
	      slot = aweb->stack_slot;
	      source = DF_REF_REG (info.defs[n]);
	      /* adjust_address() might generate code.  */
	      start_sequence ();
	      if (GET_CODE (source) == SUBREG)
		slot = adjust_address (slot, GET_MODE (source),
				       SUBREG_BYTE (source));
	      if ((!last_slot || !rtx_equal_p (slot, last_slot))
		  && ! slot_member_p (slots, slot))
		{
		  rtx insns, ni;
		  last_slot = slot;
		  remember_slot (&slots, slot);
		  emit_move_insn (slot, source);
		  insns = get_insns ();
		  end_sequence ();
		  if (insns)
		    {
		      emit_insns_after (insns, insn);
		      if (bb->end == insn)
			bb->end = PREV_INSN (following);
		      for (ni = insns; ni != following; ni = NEXT_INSN (ni))
			{
			  set_block_for_insn (ni, bb);
			  df_insn_modify (df, bb, ni);
			}
		    }
		  else
		    df_insn_modify (df, bb, insn);
		  emitted_spill_stores++;
		  spill_store_cost += bb->frequency + 1;
		  bitmap_set_bit (new_deaths, INSN_UID (PREV_INSN (following)));
		}
	      else
		{
		  end_sequence ();
		  /* Ignore insns from adjust_address() above.  */
		}
	    }
	}
      /* Clear emitted stores if a new load is seen here.  */
      /* XXX If we emit the stack-ref directly into the using insn the
         following needs a change, because that is no new insn.  Preferably
	 we would add some notes to the insn, what stackslots are needed
	 for it.  */
      if (uid >= last_max_uid)
	{
	  rtx set = single_set (insn);
	  last_slot = NULL_RTX;
	  if (!set)
	    slots = NULL;
	  else
	    {
	      if (GET_CODE (SET_SRC (set)) == MEM)
	        delete_overlapping_slots (&slots, SET_SRC (set));
	    }
	}
    }
}

/* Returns 1 if both colored webs have some hardregs in common, even if
   they are not the same width.  */
static int
spill_same_color_p (web1, web2)
     struct web *web1, *web2;
{
  int c1, size1, c2, size2;
  if ((c1 = alias (web1)->color) < 0)
    return 0;
  if ((c2 = alias (web2)->color) < 0)
    return 0;

  size1 = web1->type == PRECOLORED
          ? 1 : HARD_REGNO_NREGS (c1, PSEUDO_REGNO_MODE (web1->regno));
  size2 = web2->type == PRECOLORED
          ? 1 : HARD_REGNO_NREGS (c2, PSEUDO_REGNO_MODE (web2->regno));
  if (c1 >= c2 + size2 || c2 >= c1 + size1)
    return 0;
  return 1;
}

static unsigned int
is_partly_live_1 (live, web)
     sbitmap live;
     struct web *web;
{
  do
    if (TEST_BIT (live, web->id))
      return 1;
  while ((web = web->subreg_next));
  return 0;
}

#define is_partly_live(live, web) ((!web->subreg_next)	\
				   ? TEST_BIT (live, web->id)	\
				   : is_partly_live_1 (live, web))

static void
update_spill_colors (in_use, web, add)
     HARD_REG_SET *in_use;
     struct web *web;
     int add;
{
  int c, size;
  if ((c = alias (find_web_for_subweb (web))->color) < 0)
    return;
  size = HARD_REGNO_NREGS (c, GET_MODE (web->orig_x));
  if (SUBWEB_P (web))
    {
      c += subreg_regno_offset (c, GET_MODE (SUBREG_REG (web->orig_x)),
				SUBREG_BYTE (web->orig_x),
				GET_MODE (web->orig_x));
    }
  else if (web->type == PRECOLORED)
    size = 1;
  if (add)
    for (; size--;)
      SET_HARD_REG_BIT (*in_use, c + size);
  else
    for (; size--;)
      CLEAR_HARD_REG_BIT (*in_use, c + size);
}

static int
spill_is_free (in_use, web)
     HARD_REG_SET *in_use;
     struct web *web;
{
  int c, size;
  if ((c = alias (web)->color) < 0)
    return -1;
  size = web->type == PRECOLORED
         ? 1 : HARD_REGNO_NREGS (c, PSEUDO_REGNO_MODE (web->regno));
  for (; size--;)
    if (TEST_HARD_REG_BIT (*in_use, c + size))
      return 0;
  return 1;
}

struct rewrite_info
{
  bitmap need_reload;
  bitmap scratch;
  sbitmap live;
  struct web **needed_loads;
  int nl_size;
  int num_reloads;
  HARD_REG_SET colors_in_use;
  int any_spilltemps_spilled;
  int need_load;
};

static void
emit_loads (ri, nl_first_reload, last_block_insn)
     struct rewrite_info *ri;
     int nl_first_reload;
     rtx last_block_insn;
{
  int j;
  for (j = ri->nl_size; j;)
    {
      struct web *web = ri->needed_loads[--j];
      struct web *supweb;
      struct web *aweb;
      rtx ni, slot, reg;
      rtx before = NULL_RTX, after = NULL_RTX;
      basic_block bb;
      /* When spilltemps were spilled for the last insns, their
	 loads already are emitted, which is noted by setting
	 needed_loads[] for it to 0.  */
      if (!web)
	continue;
      supweb = find_web_for_subweb (web);
      /* Check for web being a spilltemp, if we only want to
	 load spilltemps.  Also remember, that we emitted that
	 load, which we don't need to do when we have a death,
	 because then all of needed_loads[] is emptied.  */
      if (!ri->need_load)
	{
	  if (!supweb->spill_temp)
	    continue;
	  else
	    ri->needed_loads[j] = 0;
	}
      web->in_load = 0;
      /* The adding of reloads doesn't depend on liveness.  */
      if (j < nl_first_reload && !TEST_BIT (ri->live, web->id))
	continue;
      aweb = alias (supweb);
      aweb->changed = 1;
      start_sequence ();
      if (supweb->pattern)
	{
	  /* XXX If we later allow non-constant sources for rematerialization
	     we must also disallow coalescing _to_ rematerialized webs
	     (at least then disallow spilling them, which we already ensure
	     when flag_ra_break_aliases), or not take the pattern but a
	     stackslot.  */
	  if (aweb != supweb)
	    abort ();
	  slot = copy_rtx (supweb->pattern);
	  reg = copy_rtx (supweb->orig_x);
	  /* Sanity check.  orig_x should be a REG rtx, which should be
	     shared over all RTL, so copy_rtx should have no effect.  */
	  if (reg != supweb->orig_x)
	    abort ();
	}
      else
	{
	  allocate_spill_web (aweb);
	  slot = aweb->stack_slot;

	  /* If we don't copy the RTL there might be some SUBREG
	     rtx shared in the next iteration although being in
	     different webs, which leads to wrong code.  */
	  reg = copy_rtx (web->orig_x);
	  if (GET_CODE (reg) == SUBREG)
	    slot = adjust_address (slot, GET_MODE (reg), SUBREG_BYTE (reg));
	}
      emit_move_insn (reg, slot);
      ni = get_insns ();
      end_sequence ();
      before = web->last_use_insn;
      web->last_use_insn = NULL_RTX;
      if (!before)
	{
	  if (JUMP_P (last_block_insn))
	    before = last_block_insn;
	  else
	    after = last_block_insn;
	}
      if (after)
	{
	  rtx foll = NEXT_INSN (after);
	  bb = BLOCK_FOR_INSN (after);
	  emit_insns_after (ni, after);
	  if (bb->end == after)
	    bb->end = PREV_INSN (foll);
	  for (ni = NEXT_INSN (after); ni != foll; ni = NEXT_INSN (ni))
	    {
	      set_block_for_insn (ni, bb);
	      df_insn_modify (df, bb, ni);
	    }
	}
      else
	{
	  rtx prev = PREV_INSN (before);
	  bb = BLOCK_FOR_INSN (before);
	  emit_insns_before (ni, before);
	  if (bb->head == before)
	    bb->head = NEXT_INSN (prev);
	  for (; ni != before; ni = NEXT_INSN (ni))
	    {
	      set_block_for_insn (ni, bb);
	      df_insn_modify (df, bb, ni);
	    }
	}
      if (supweb->pattern)
	{
	  emitted_remat++;
	  spill_remat_cost += bb->frequency + 1;
	}
      else
	{
	  emitted_spill_loads++;
	  spill_load_cost += bb->frequency + 1;
	}
      RESET_BIT (ri->live, web->id);
      /* In the special case documented above only emit the reloads and
	 one load.  */
      if (ri->need_load == 2 && j < nl_first_reload)
	break;
    }
  if (ri->need_load)
    ri->nl_size = j;
}

static void
detect_bbs_for_rewrite (changed_bbs)
     sbitmap changed_bbs;
{
  int pass;
  struct dlist *d;
  for (pass = 0; pass < 2; pass++)
    for (d = (pass == 0) ? WEBS(SPILLED) : WEBS(COALESCED); d; d = d->next)
      {
        struct web *web = DLIST_WEB (d);
	unsigned int i;
	if (pass == 1 && alias (web)->type != SPILLED)
	  continue;
	for (i = 0; i < web->num_uses; i++)
	  SET_BIT (changed_bbs, 2 + DF_REF_BBNO (web->uses[i]));
      }
}

/* Fast version of rewrite_program2() for one basic block, where
   no spill code is necessary.  We detect here only insns with deaths.  */
static void
detect_deaths_in_bb (bb, live, new_deaths)
     basic_block bb;
     sbitmap live;
     bitmap new_deaths;
{
  rtx insn, head_prev;
  int j;

  insn = bb->end;
  if (!INSN_P (insn))
    insn = prev_real_insn (insn);
  /* Empty block?  */
  if (BLOCK_FOR_INSN (insn) != bb)
    return;

  head_prev = PREV_INSN (bb->head);
  sbitmap_zero (live);
  EXECUTE_IF_SET_IN_BITMAP (live_at_end[bb->index], 0, j,
    { 
      struct web *web = use2web[j];
      struct web *aweb = alias (find_web_for_subweb (web));
      /* See below in rewrite_program2() for a comment which webs we
	 consider live at end.  */
      if (aweb->type != SPILLED /*|| aweb->color >= 0*/)
	SET_BIT (live, web->id);
    });

  for (; insn != head_prev; insn = PREV_INSN (insn))
    {
      struct ra_insn_info info;
      unsigned int n;

      if (!INSN_P (insn))
	continue;

      info = insn_df[INSN_UID (insn)];
      for (n = 0; n < info.num_defs; n++)
	{
	  struct ref *ref = info.defs[n];
	  struct web *web = def2web[DF_REF_ID (ref)];
	  rtx reg = DF_REF_REG (ref);
	  int is_non_def = 0;
	  unsigned int n2;

	  web = find_web_for_subweb (web);
	  /* Detect rmw webs.  */
	  for (n2 = 0; n2 < info.num_uses; n2++)
	    {
	      struct web *web2 = use2web[DF_REF_ID (info.uses[n2])];
	      if (web == find_web_for_subweb (web2))
		{
		  is_non_def = 1;
		  break;
		}
	    }
	  if (is_non_def)
	    continue;

	  if (!is_partly_live (live, web))
	    bitmap_set_bit (useless_defs, DF_REF_ID (ref));

	  if (GET_CODE (reg) == SUBREG)
	    {
	      struct web *sweb;
	      sweb = find_subweb (web, reg);
	      RESET_BIT (live, sweb->id);
	    }
	  else
	    {
	      struct web *sweb;
	      RESET_BIT (live, web->id);
	      for (sweb = web->subreg_next; sweb;
		   sweb = sweb->subreg_next)
		RESET_BIT (live, sweb->id);
	    }
	}

      for (n = 0; n < info.num_uses; n++)
	{
	  struct web *web = use2web[DF_REF_ID (info.uses[n])];
	  struct web *supweb = find_web_for_subweb (web);
	  int is_death = !TEST_BIT (live, supweb->id);
	  is_death &= !TEST_BIT (live, web->id);
	  if (is_death)
	    {
	      bitmap_set_bit (new_deaths, INSN_UID (insn));
	      break;
	    }
	}
      
      for (n = 0; n < info.num_uses; n++)
	{
	  struct web *web = use2web[DF_REF_ID (info.uses[n])];
	  SET_BIT (live, web->id);
	}
    }
}

static void
reloads_to_loads (ri, refs, num_refs, ref2web)
     struct rewrite_info *ri;
     struct ref **refs;
     unsigned int num_refs;
     struct web **ref2web;
{
  unsigned int n;
  int num_reloads = ri->num_reloads;
  for (n = 0; n < num_refs && num_reloads; n++)
    {
      struct web *web = ref2web[DF_REF_ID (refs[n])];
      struct web *supweb = find_web_for_subweb (web);
      int is_death;
      int j;
      /* Only emit reloads when entering their interference
	 region.  A use of a spilled web never opens an
	 interference region, independent of it's color.  */
      if (alias (supweb)->type == SPILLED)
	continue;
      if (supweb->type == PRECOLORED
	  && TEST_HARD_REG_BIT (never_use_colors, supweb->color))
	continue;
      /* Note, that if web (and supweb) are DEFs, we already cleared
	 the corresponding bits in live.  I.e. is_death becomes true, which
	 is what we want.  */
      is_death = !TEST_BIT (ri->live, supweb->id);
      is_death &= !TEST_BIT (ri->live, web->id);
      if (is_death)
	{
	  int old_num_r = num_reloads;
	  bitmap_clear (ri->scratch);
	  EXECUTE_IF_SET_IN_BITMAP (ri->need_reload, 0, j,
	    {
	      struct web *web2 = ID2WEB (j);
	      struct web *aweb2 = alias (find_web_for_subweb (web2));
	      if (spill_is_free (&(ri->colors_in_use), aweb2) == 0)
		abort ();
	      if (spill_same_color_p (supweb, aweb2)
		  /* && interfere (web, web2) */)
		{
		  if (!web2->in_load)
		    {
		      ri->needed_loads[ri->nl_size++] = web2;
		      web2->in_load = 1;
		    }
		  bitmap_set_bit (ri->scratch, j);
		  num_reloads--;
		}
	    });
	  if (num_reloads != old_num_r)
	    bitmap_operation (ri->need_reload, ri->need_reload, ri->scratch,
			      BITMAP_AND_COMPL);
	}
    }
  ri->num_reloads = num_reloads;
}

#define NEW_SPILL

static void
rewrite_program2 (new_deaths)
     bitmap new_deaths;
{
  int i;
  sbitmap changed_bbs = sbitmap_alloc (n_basic_blocks);
  int nl_first_reload;
  struct rewrite_info ri;
  rtx insn;
  ri.needed_loads = (struct web **) xmalloc (num_webs * sizeof (struct web *));
  ri.need_reload = BITMAP_XMALLOC ();
  ri.scratch = BITMAP_XMALLOC ();
  ri.live = sbitmap_alloc (num_webs);
  ri.nl_size = 0;
  ri.num_reloads = 0;
  sbitmap_zero (changed_bbs);
  detect_bbs_for_rewrite (changed_bbs);
#ifndef NEW_SPILL
  for (i = 0; i < n_basic_blocks + 2; i++)
#else
  for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
#endif
    {
      basic_block bb, last_bb = NULL;
      rtx last_block_insn;
      int j;
#ifndef NEW_SPILL
      if (i < 2)
	bb = (i == 0) ? EXIT_BLOCK_PTR : ENTRY_BLOCK_PTR;
      else
	bb = BASIC_BLOCK (i - 2);
      if (!bb->end)
	continue;
      if (!TEST_BIT (changed_bbs, i))
	{
	  detect_deaths_in_bb (bb, ri.live, new_deaths);
	  continue;
	}

      insn = bb->end;
      if (!INSN_P (insn))
        insn = prev_real_insn (insn);
      /* Empty block?  */
      if (BLOCK_FOR_INSN (insn) != bb)
	continue;
#else
      if (!INSN_P (insn))
	insn = prev_real_insn (insn);
      while (insn && !(bb = BLOCK_FOR_INSN (insn)))
	insn = prev_real_insn (insn);
      if (!insn)
	break;
      i = bb->index + 2;
#endif
      last_block_insn = insn;

      sbitmap_zero (ri.live);
      CLEAR_HARD_REG_SET (ri.colors_in_use);
      EXECUTE_IF_SET_IN_BITMAP (live_at_end[i - 2], 0, j,
	{ 
	  struct web *web = use2web[j];
	  struct web *aweb = alias (find_web_for_subweb (web));
	  /* A web is only live at end, if it isn't spilled.  If we wouldn't
	     check this, the last uses of spilled web per basic block
	     wouldn't be detected as deaths, although they are in the final
	     code.  This would lead to cumulating many loads without need,
	     only increasing register pressure.  */
	  /* XXX do add also spilled webs which got a color for IR spilling.
	     Remember to not add to colors_in_use in that case.  */
	  if (aweb->type != SPILLED /*|| aweb->color >= 0*/)
	    {
	      SET_BIT (ri.live, web->id);
	      if (aweb->type != SPILLED)
	        update_spill_colors (&(ri.colors_in_use), web, 1);
	    }
	});

      bitmap_clear (ri.need_reload);
      ri.num_reloads = 0;
      ri.any_spilltemps_spilled = 0;
      if (flag_ra_ir_spilling)
	{
	  struct dlist *d;
	  int pass;
	  /* XXX If we don't add spilled nodes into live above, the following
	     becomes an empty loop.  */
	  for (pass = 0; pass < 2; pass++)
	    for (d = (pass) ? WEBS(SPILLED) : WEBS(COALESCED); d; d = d->next)
	      {
	        struct web *web = DLIST_WEB (d);
		struct web *aweb = alias (web);
		if (aweb->type != SPILLED)
		  continue;
	        if (is_partly_live (ri.live, web)
		    && spill_is_free (&(ri.colors_in_use), web) > 0)
		  {
		    ri.num_reloads++;
	            bitmap_set_bit (ri.need_reload, web->id);
		    /* Last using insn is somewhere in another block.  */
		    web->last_use_insn = NULL_RTX;
		  }
	      }
	}

      last_bb = bb;
      for (; insn; insn = PREV_INSN (insn))
	{
	  struct ra_insn_info info;
	  unsigned int n;

#ifdef NEW_SPILL
	  if (INSN_P (insn) && BLOCK_FOR_INSN (insn) != last_bb)
	    {
	      int index = BLOCK_FOR_INSN (insn)->index + 2;
	      EXECUTE_IF_SET_IN_BITMAP (live_at_end[index - 2], 0, j,
		{ 
		  struct web *web = use2web[j];
		  struct web *aweb = alias (find_web_for_subweb (web));
		  if (aweb->type != SPILLED)
		    {
		      SET_BIT (ri.live, web->id);
		      update_spill_colors (&(ri.colors_in_use), web, 1);
		    }
		});
	      bitmap_clear (ri.scratch);
	      EXECUTE_IF_SET_IN_BITMAP (ri.need_reload, 0, j,
		{
		  struct web *web2 = ID2WEB (j);
		  struct web *supweb2 = find_web_for_subweb (web2);
		  struct web *aweb2 = alias (supweb2);
		  if (spill_is_free (&(ri.colors_in_use), aweb2) <= 0)
		    {
		      if (!web2->in_load)
			{
			  ri.needed_loads[ri.nl_size++] = web2;
			  web2->in_load = 1;
			}
		      bitmap_set_bit (ri.scratch, j);
		      ri.num_reloads--;
		    }
		});
	      bitmap_operation (ri.need_reload, ri.need_reload, ri.scratch,
				BITMAP_AND_COMPL);
	      last_bb = BLOCK_FOR_INSN (insn);
	      last_block_insn = insn;
	      if (!INSN_P (last_block_insn))
	        last_block_insn = prev_real_insn (last_block_insn);
	    }
#endif

	  ri.need_load = 0;
	  if (INSN_P (insn))
	    info = insn_df[INSN_UID (insn)];

	  if (INSN_P (insn))
	    for (n = 0; n < info.num_defs; n++)
	      {
		struct ref *ref = info.defs[n];
		struct web *web = def2web[DF_REF_ID (ref)];
		struct web *supweb = find_web_for_subweb (web);
		int is_non_def = 0;
		unsigned int n2;

		supweb = find_web_for_subweb (web);
		/* Webs which are defined here, but also used in the same insn
		   are rmw webs, or this use isn't a death because of looping
		   constructs.  In neither case makes this def available it's
		   resources.  Reloads for it are still needed, it's still
		   live and it's colors don't become free.  */
		for (n2 = 0; n2 < info.num_uses; n2++)
		  {
		    struct web *web2 = use2web[DF_REF_ID (info.uses[n2])];
		    if (supweb == find_web_for_subweb (web2))
		      {
			is_non_def = 1;
			break;
		      }
		  }
		if (is_non_def)
		  continue;

		if (!is_partly_live (ri.live, supweb))
		  bitmap_set_bit (useless_defs, DF_REF_ID (ref));

		RESET_BIT (ri.live, web->id);
		if (bitmap_bit_p (ri.need_reload, web->id))
		  {
		    ri.num_reloads--;
		    bitmap_clear_bit (ri.need_reload, web->id);
		  }
		if (web != supweb)
		  {
		    /* XXX subwebs aren't precisely tracked here.  We have
		       everything we need (inverse webs), but the code isn't
		       yet written.  We need to make all completely
		       overlapping web parts non-live here.  */
		    /* If by luck now the whole web isn't live anymore, no
		       reloads for it are needed.  */
		    if (!is_partly_live (ri.live, supweb)
			&& bitmap_bit_p (ri.need_reload, supweb->id))
		      {
			ri.num_reloads--;
			bitmap_clear_bit (ri.need_reload, supweb->id);
		      }
		  }
		else
		  {
		    struct web *sweb;
		    /* If the whole web is defined here, no parts of it are
		       live anymore and no reloads are needed for them.  */
		    for (sweb = supweb->subreg_next; sweb;
			 sweb = sweb->subreg_next)
		      {
		        RESET_BIT (ri.live, sweb->id);
			if (bitmap_bit_p (ri.need_reload, sweb->id))
			  {
		            ri.num_reloads--;
		            bitmap_clear_bit (ri.need_reload, sweb->id);
			  }
		      }
		  }
		if (alias (supweb)->type != SPILLED)
		  update_spill_colors (&(ri.colors_in_use), web, 0);
	      }

	  nl_first_reload = ri.nl_size;

	  /* CALL_INSNs are not really deaths, but still more registers
	     are free after a call, than before.
	     XXX Note, that sometimes reload barfs when we emit insns between
	     a call and the insn which copies the return register into a 
	     pseudo.  */
	  if (GET_CODE (insn) == CALL_INSN)
	    ri.need_load = 1;
	  else if (INSN_P (insn))
	    for (n = 0; n < info.num_uses; n++)
	      {
		struct web *web = use2web[DF_REF_ID (info.uses[n])];
		struct web *supweb = find_web_for_subweb (web);
		int is_death;
		if (supweb->type == PRECOLORED
		    && TEST_HARD_REG_BIT (never_use_colors, supweb->color))
		  continue;
		is_death = !TEST_BIT (ri.live, supweb->id);
		is_death &= !TEST_BIT (ri.live, web->id);
		if (is_death)
		  {
		    ri.need_load = 1;
		    bitmap_set_bit (new_deaths, INSN_UID (insn));
		    break;
		  }
	      }
	  
	  if (INSN_P (insn) && ri.num_reloads)
	    {
              int old_num_reloads = ri.num_reloads;
	      reloads_to_loads (&ri, info.uses, info.num_uses, use2web);

	      /* If this insn sets a pseudo, which isn't used later
		 (i.e. wasn't live before) it is a dead store.  We need
		 to emit all reloads which have the same color as this def.
		 We don't need to check for non-liveness here to detect
		 the deadness (it anyway is too late, as we already cleared
		 the liveness in the first loop over the defs), because if it
		 _would_ be live here, no reload could have that color, as
		 they would already have been converted to a load.  */
	      if (ri.num_reloads)
		reloads_to_loads (&ri, info.defs, info.num_defs, def2web);
	      if (ri.num_reloads != old_num_reloads && !ri.need_load)
		ri.need_load = 1;
	    }

	  if (ri.nl_size && (ri.need_load || ri.any_spilltemps_spilled))
	    emit_loads (&ri, nl_first_reload, last_block_insn);

	  if (INSN_P (insn) && flag_ra_ir_spilling)
	    for (n = 0; n < info.num_uses; n++)
	      {
		struct web *web = use2web[DF_REF_ID (info.uses[n])];
		struct web *aweb = alias (find_web_for_subweb (web));
		if (aweb->type != SPILLED)
		  update_spill_colors (&(ri.colors_in_use), web, 1);
	      }
	  
	  ri.any_spilltemps_spilled = 0;
	  if (INSN_P (insn))
	    for (n = 0; n < info.num_uses; n++)
	      {
		struct web *web = use2web[DF_REF_ID (info.uses[n])];
		struct web *supweb = find_web_for_subweb (web);
		struct web *aweb = alias (supweb);
		SET_BIT (ri.live, web->id);
		if (aweb->type != SPILLED)
		  continue;
		if (supweb->spill_temp)
		  ri.any_spilltemps_spilled = 1;
		web->last_use_insn = insn;
		if (!web->in_load)
		  {
		    if (spill_is_free (&(ri.colors_in_use), aweb) <= 0
			|| !flag_ra_ir_spilling)
		      {
			ri.needed_loads[ri.nl_size++] = web;
			web->in_load = 1;
			web->one_load = 1;
		      }
		    else if (!bitmap_bit_p (ri.need_reload, web->id))
		      {
		        bitmap_set_bit (ri.need_reload, web->id);
			ri.num_reloads++;
			web->one_load = 1;
		      }
		    else
		      web->one_load = 0;
		  }
		else
		  web->one_load = 0;
	      }

#ifndef NEW_SPILL
	  if (insn == bb->head)
#else
	  if (GET_CODE (insn) == CODE_LABEL)
#endif
	    break;
	}
      
      nl_first_reload = ri.nl_size;
      if (ri.num_reloads)
	{
	  int in_ir = 0;
	  edge e;
	  int num = 0;
	  HARD_REG_SET cum_colors, colors;
	  CLEAR_HARD_REG_SET (cum_colors);
	  for (e = bb->pred; e && num < 5; e = e->pred_next, num++)
	    {
	      int j;
	      CLEAR_HARD_REG_SET (colors);
	      EXECUTE_IF_SET_IN_BITMAP (live_at_end[e->src->index], 0, j,
		{ 
		  struct web *web = use2web[j];
		  struct web *aweb = alias (find_web_for_subweb (web));
		  if (aweb->type != SPILLED)
		    update_spill_colors (&colors, web, 1);
		});
	      IOR_HARD_REG_SET (cum_colors, colors);
	    }
	  if (num == 5)
	    in_ir = 1;
	  
	  bitmap_clear (ri.scratch);
	  EXECUTE_IF_SET_IN_BITMAP (ri.need_reload, 0, j,
	    {
	      struct web *web2 = ID2WEB (j);
	      struct web *supweb2 = find_web_for_subweb (web2);
	      struct web *aweb2 = alias (supweb2);
	      /* block entry is IR boundary for aweb2?  */
	      if (((ra_pass > 0 || supweb2->target_of_spilled_move)
		  && (1 || in_ir || spill_is_free (&cum_colors, aweb2) <= 0))
		  || (ra_pass == 1
		      && (in_ir
			  || spill_is_free (&cum_colors, aweb2) <= 0)))
		{
		  if (!web2->in_load)
		    {
		      ri.needed_loads[ri.nl_size++] = web2;
		      web2->in_load = 1;
		    }
		  bitmap_set_bit (ri.scratch, j);
		  ri.num_reloads--;
		}
	    });
	  bitmap_operation (ri.need_reload, ri.need_reload, ri.scratch,
			    BITMAP_AND_COMPL);
	}

      ri.need_load = 1;
      emit_loads (&ri, nl_first_reload, last_block_insn);
      if (ri.nl_size != 0 /*|| ri.num_reloads != 0*/)
	abort ();
#ifdef NEW_SPILL
      if (!insn)
	break;
#endif
    }
  free (ri.needed_loads);
  sbitmap_free (changed_bbs);
  sbitmap_free (ri.live);
  BITMAP_XFREE (ri.scratch);
  BITMAP_XFREE (ri.need_reload);
}

static void
mark_refs_for_checking (web, uses_as_bitmap)
     struct web *web;
     bitmap uses_as_bitmap;
{
  unsigned int i;
  for (i = 0; i < web->num_uses; i++)
    {
      unsigned int id = DF_REF_ID (web->uses[i]);
      SET_BIT (last_check_uses, id);
      bitmap_set_bit (uses_as_bitmap, id);
      web_parts[df->def_id + id].spanned_deaths = 0;
      web_parts[df->def_id + id].crosses_call = 0;
    }
  for (i = 0; i < web->num_defs; i++)
    {
      unsigned int id = DF_REF_ID (web->defs[i]);
      web_parts[id].spanned_deaths = 0;
      web_parts[id].crosses_call = 0;
    }
}

static void
detect_web_parts_to_rebuild (void)
{
  bitmap uses_as_bitmap;
  unsigned int i, pass;
  struct dlist *d;
  sbitmap already_webs = sbitmap_alloc (num_webs);

  uses_as_bitmap = BITMAP_XMALLOC ();
  if (last_check_uses)
    sbitmap_free (last_check_uses);
  last_check_uses = sbitmap_alloc (df->use_id);
  sbitmap_zero (last_check_uses);
  sbitmap_zero (already_webs);
  /* We need to recheck all uses of all webs involved in spilling (and the
     uses added by spill insns, but those are not analyzed yet).
     Those are the spilled webs themself, webs coalesced to spilled ones,
     and webs conflicting with any of them.  */
  for (pass = 0; pass < 2; pass++)
    for (d = (pass == 0) ? WEBS(SPILLED) : WEBS(COALESCED); d; d = d->next)
      {
        struct web *web = DLIST_WEB (d);
	struct conflict_link *wl;
	unsigned int j;
	/* This check is only needed for coalesced nodes, but hey.  */
	if (alias (web)->type != SPILLED)
	  continue;
        for (i = 0; i < web->num_uses; i++)
	  {
	    unsigned int id = DF_REF_ID (web->uses[i]);
	    SET_BIT (last_check_uses, id);
	    bitmap_set_bit (uses_as_bitmap, id);
	    web_parts[df->def_id + id].uplink = NULL;
	    web_parts[df->def_id + id].spanned_deaths = 0;
	    web_parts[df->def_id + id].crosses_call = 0;
	  }
	for (i = 0; i < web->num_defs; i++)
	  {
	    unsigned int id = DF_REF_ID (web->defs[i]);
	    web_parts[id].uplink = NULL;
	    web_parts[id].spanned_deaths = 0;
	    web_parts[id].crosses_call = 0;
	  }
	if (web->have_orig_conflicts)
	  wl = web->orig_conflict_list;
	else
	  wl = web->conflict_list;
	for (; wl; wl = wl->next)
	  {
	    if (TEST_BIT (already_webs, wl->t->id))
	      continue;
	    SET_BIT (already_webs, wl->t->id);
	    mark_refs_for_checking (wl->t, uses_as_bitmap);
	  }
	EXECUTE_IF_SET_IN_BITMAP (web->useless_conflicts, 0, j,
	  {
	    struct web *web2 = ID2WEB (j);
	    if (TEST_BIT (already_webs, web2->id))
	      continue;
	    SET_BIT (already_webs, web2->id);
	    mark_refs_for_checking (web2, uses_as_bitmap);
	  });
      }
  live_at_end -= 2;
  for (i = 0; i < (unsigned int) n_basic_blocks + 2; i++)
    bitmap_operation (live_at_end[i], live_at_end[i], uses_as_bitmap,
		      BITMAP_AND_COMPL);
  live_at_end += 2;

  if (rtl_dump_file && (debug_new_regalloc & DUMP_REBUILD) != 0)
    {
      debug_msg (DUMP_REBUILD, "need to check these uses:\n");
      dump_sbitmap_file (rtl_dump_file, last_check_uses);
    }
  sbitmap_free (already_webs);
  BITMAP_XFREE (uses_as_bitmap);
}

static unsigned int deleted_def_insns;
static unsigned HOST_WIDE_INT deleted_def_cost;

static void
delete_useless_defs (void)
{
  unsigned int i;
  /* If the insn only sets the def without any sideeffect (besides
     clobbers or uses), we can delete it.  single_set() also tests
     for INSN_P(insn).  */
  EXECUTE_IF_SET_IN_BITMAP (useless_defs, 0, i,
    {
      rtx insn = DF_REF_INSN (df->defs[i]);
      rtx set = single_set (insn);
      struct web *web = find_web_for_subweb (def2web[i]);
      if (set && web->type == SPILLED && web->stack_slot == NULL)
        {
	  deleted_def_insns++;
	  deleted_def_cost += BLOCK_FOR_INSN (insn)->frequency + 1;
	  PUT_CODE (insn, NOTE);
	  NOTE_LINE_NUMBER (insn) = NOTE_INSN_DELETED;
	  df_insn_modify (df, BLOCK_FOR_INSN (insn), insn);
	}
    });
}

static void
detect_non_changed_webs (void)
{
  struct dlist *d, *d_next;
  for (d = WEBS(SPILLED); d; d = d_next)
    {
      struct web *web = DLIST_WEB (d);
      d_next = d->next;
      if (!web->changed)
	{
	  debug_msg (DUMP_PROCESS, "no insns emitted for spilled web %d\n",
		     web->id);
	  remove_web_from_list (web);
	  put_web (web, COLORED);
	  web->changed = 1;
	}
      else
	web->changed = 0;
      /* From now on web->changed is used as the opposite flag.
	 I.e. colored webs, which have changed set were formerly
	 spilled webs for which no insns were emitted.  */
    }
}

static void
reset_changed_flag (void)
{
  struct dlist *d;
  for (d = WEBS(SPILLED); d; d = d->next)
    DLIST_WEB(d)->changed = 0;
}

static void
actual_spill (void)
{
  int i;
  bitmap new_deaths = BITMAP_XMALLOC ();
  reset_changed_flag ();
  spill_coalprop ();
  choose_spill_colors ();
  useless_defs = BITMAP_XMALLOC ();
  rewrite_program2 (new_deaths);
  /*rewrite_program (new_deaths);*/
  insert_stores (new_deaths);
  delete_useless_defs ();
  BITMAP_XFREE (useless_defs);
  sbitmap_free (insns_with_deaths);
  insns_with_deaths = sbitmap_alloc (get_max_uid ());
  death_insns_max_uid = get_max_uid ();
  sbitmap_zero (insns_with_deaths);
  EXECUTE_IF_SET_IN_BITMAP (new_deaths, 0, i,
    { SET_BIT (insns_with_deaths, i);});
  detect_non_changed_webs ();
  detect_web_parts_to_rebuild ();
  BITMAP_XFREE (new_deaths);
}

bitmap regnos_coalesced_to_hardregs;

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

  /* This bitmap is freed in remove_suspicious_death_notes(),
     which is also the user of it.  */
  regnos_coalesced_to_hardregs = BITMAP_XMALLOC ();
  /* First create the (REG xx) rtx's for all webs, as we need to know
     the number, to make sure, flow has enough memory for them in the
     various tables.  */
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      web = ID2WEB (i);
      if (web->type != COLORED && web->type != COALESCED)
	continue;
      if (web->type == COALESCED && alias (web)->type == COLORED)
	continue;
      if (web->reg_rtx || web->regno < FIRST_PSEUDO_REGISTER)
	abort ();

      /* Special case for i386 'fix_truncdi_nomemory' insn.
	 We must choose mode from insns not from PSEUDO_REGNO_MODE.
	 Actual only for clobbered register.  */
      if (web->num_uses == 0 && web->num_defs == 1)
	web->reg_rtx = gen_reg_rtx (GET_MODE (DF_REF_REG (web->defs[0])));
      else
	web->reg_rtx = gen_reg_rtx (PSEUDO_REGNO_MODE (web->regno));
      
      /* Remember the different parts directly coalesced to a hardreg.  */
      if (web->type == COALESCED)
	bitmap_set_bit (regnos_coalesced_to_hardregs, REGNO (web->reg_rtx));
    }
  ra_max_regno = max_regno = max_reg_num ();
  allocate_reg_info (max_regno, FALSE, FALSE);
  ra_reg_renumber = (short *) xmalloc (max_regno * sizeof (short));
  for (si = 0; si < max_regno; si++)
    ra_reg_renumber[si] = -1;

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
    if (df->uses[i])
      {
	regset rs = DF_REF_BB (df->uses[i])->global_live_at_start;
	rtx regrtx;
	web = use2web[i];
	web = find_web_for_subweb (web);
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
      web = find_web_for_subweb (web);
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

  /* And now set up the ra_reg_renumber array for reload with all the new
     pseudo-regs.  */
  for (i = 0; i < num_webs - num_subwebs; i++)
    {
      web = ID2WEB (i);
      if (web->reg_rtx)
	{
	  int r = REGNO (web->reg_rtx);
          ra_reg_renumber[r] = web->color;
          debug_msg (DUMP_COLORIZE, "Renumber pseudo %d (== web %d) to %d\n",
		     r, web->id, ra_reg_renumber[r]);
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
     return register), and the deletion would confuse reload in thinking the
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
	basic_block bb = BLOCK_FOR_INSN (ml->move->insn);
	df_insn_delete (df, bb, ml->move->insn);
	deleted_move_insns++;
	deleted_move_cost += bb->frequency + 1;
      }
}

struct web_pair
{
  struct web_pair *next_hash;
  struct web_pair *next_list;
  struct web *smaller;
  struct web *larger;
  unsigned int conflicts;
  unsigned HOST_WIDE_INT cost;
};

#define WEB_PAIR_HASH_SIZE 8192
static struct web_pair *web_pair_hash[WEB_PAIR_HASH_SIZE];
static struct web_pair *web_pair_list;
static unsigned int num_web_pairs;

static void
init_web_pairs (void)
{
  memset (web_pair_hash, 0, sizeof web_pair_hash);
  num_web_pairs = 0;
  web_pair_list = NULL;
}

static void
add_web_pair_cost (web1, web2, cost, conflicts)
     struct web *web1, *web2;
     unsigned HOST_WIDE_INT cost;
     unsigned int conflicts;
{
  unsigned int hash;
  struct web_pair *p;
  if (web1->id > web2->id)
    {
      struct web *h = web1;
      web1 = web2;
      web2 = h;
    }
  hash = (web1->id * num_webs + web2->id) % WEB_PAIR_HASH_SIZE;
  for (p = web_pair_hash[hash]; p; p = p->next_hash)
    if (p->smaller == web1 && p->larger == web2)
      {
	p->cost += cost;
	p->conflicts += conflicts;
	return;
      }
  p = (struct web_pair *) ra_alloc (sizeof *p);
  p->next_hash = web_pair_hash[hash];
  p->next_list = web_pair_list;
  p->smaller = web1;
  p->larger = web2;
  p->conflicts = conflicts;
  p->cost = cost;
  web_pair_hash[hash] = p;
  web_pair_list = p;
  num_web_pairs++;
}

static int
comp_web_pairs (w1, w2)
     const void *w1, *w2;
{
  struct web_pair *p1 = *(struct web_pair **)w1;
  struct web_pair *p2 = *(struct web_pair **)w2;
  if (p1->conflicts > p2->conflicts)
    return -1;
  else if (p1->conflicts < p2->conflicts)
    return 1;
  else if (p1->cost > p2->cost)
    return -1;
  else if (p1->cost < p2->cost)
    return 1;
  else
    return 0;
}

static void
sort_and_combine_web_pairs (for_move)
     int for_move;
{
  unsigned int i;
  struct web_pair **sorted;
  struct web_pair *p;
  if (!num_web_pairs)
    return;
  sorted = (struct web_pair **) xmalloc (num_web_pairs * sizeof (sorted[0]));
  for (p = web_pair_list, i = 0; p; p = p->next_list)
    sorted[i++] = p;
  if (i != num_web_pairs)
    abort ();
  qsort (sorted, num_web_pairs, sizeof (sorted[0]), comp_web_pairs);
  for (i = 0; i < num_web_pairs; i++)
    {
      struct web *w1, *w2;
      p = sorted[i];
      w1 = alias (p->smaller);
      w2 = alias (p->larger);
      if (!for_move && (w1->type == PRECOLORED || w2->type == PRECOLORED))
	continue;
      else if (w2->type == PRECOLORED)
	{
	  struct web *h = w1;
	  w1 = w2;
	  w2 = h;
	}
      if (w1 != w2
	  && !TEST_BIT (sup_igraph, w1->id * num_webs + w2->id)
	  && !TEST_BIT (sup_igraph, w2->id * num_webs + w1->id)
	  && w2->type != PRECOLORED
	  && hard_regs_intersect_p (&w1->usable_regs, &w2->usable_regs))
	  {
	    if (w1->type != PRECOLORED
		|| (w1->type == PRECOLORED && ok (w2, w1)))
	      combine (w1, w2);
	    else if (w1->type == PRECOLORED)
	      SET_HARD_REG_BIT (w2->prefer_colors, w1->color);
	  }
    }
  free (sorted);
}

static void
aggressive_coalesce (void)
{
  struct dlist *d;
  struct move *m;
  init_web_pairs ();
  while ((d = pop_list (&mv_worklist)) != NULL)
    if ((m = DLIST_MOVE (d)))
      {
	struct web *s = alias (m->source_web);
	struct web *t = alias (m->target_web);
	if (t->type == PRECOLORED)
	  {
	    struct web *h = s;
	    s = t;
	    t = h;
	  }
	if (s != t
	    && t->type != PRECOLORED
	    && !TEST_BIT (sup_igraph, s->id * num_webs + t->id)
	    && !TEST_BIT (sup_igraph, t->id * num_webs + s->id))
	  {
	    if ((s->type == PRECOLORED && ok (t, s))
		|| s->type != PRECOLORED)
	      {
	        put_move (m, MV_COALESCED);
		add_web_pair_cost (s, t, BLOCK_FOR_INSN (m->insn)->frequency,
				   0); 
	      }
	    else if (s->type == PRECOLORED)
	      /* It is !ok(t, s).  But later when coloring the graph it might
		 be possible to take that color.  So we remember the preferred
		 color to try that first.  */
	      {
		put_move (m, CONSTRAINED);
		SET_HARD_REG_BIT (t->prefer_colors, s->color);
	      }
	  }
	else
	  {
	    put_move (m, CONSTRAINED);
	  }
      }
  sort_and_combine_web_pairs (1);
}

static void
aggressive_coalesce_2 (void)
{
  while (mv_worklist)
    {
      struct dlist *d;
      struct move *m;
      struct dlist *bestd = mv_worklist;
      struct web *s, *t;

      for (d = bestd->next; d; d = d->next)
	if (BLOCK_FOR_INSN (DLIST_MOVE (d)->insn)->frequency
	    > BLOCK_FOR_INSN (DLIST_MOVE (bestd)->insn)->frequency)
	  bestd = d;
      m = DLIST_MOVE (bestd);
      if (m->type != WORKLIST)
        abort ();
      remove_list (bestd, &mv_worklist);
      s = alias (m->source_web);
      t = alias (m->target_web);
      if (t->type == PRECOLORED)
	{
	  struct web *h = s;
	  s = t;
	  t = h;
	}
      if (s != t
	  && t->type != PRECOLORED
	  && ((s->type == PRECOLORED && ok (t, s))
	      || s->type != PRECOLORED)
	  && !TEST_BIT (sup_igraph, s->id * num_webs + t->id)
	  && !TEST_BIT (sup_igraph, t->id * num_webs + s->id))
	{
	  put_move (m, MV_COALESCED);
	  combine (s, t);
	}
      else
	{
	  put_move (m, CONSTRAINED);
	}
    }
}

static void
extended_coalesce (void)
{
  rtx insn;
  bitmap defs = BITMAP_XMALLOC ();
  bitmap uses = BITMAP_XMALLOC ();
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
    {
      struct df_link *dlink;
      int num = 0;
      int i, j;
      bitmap_zero (defs);
      for (dlink = DF_INSN_DEFS (df, insn); dlink; dlink = dlink->next)
	{
	  struct web *dest = def2web[DF_REF_ID (dlink->ref)];
	  dest = alias (find_web_for_subweb (dest));
	  if (dest->type != PRECOLORED)
	    bitmap_set_bit (defs, dest->id), num++;
	}
      if (!num)
	continue;
      bitmap_zero (uses);
      num = 0;
      for (dlink = DF_INSN_USES (df, insn); dlink; dlink = dlink->next)
	{
	  struct web *source = use2web[DF_REF_ID (dlink->ref)];
	  source = alias (find_web_for_subweb (source));
	  if (source->type != PRECOLORED)
	    bitmap_set_bit (uses, source->id), num++;
	}
      if (!num)
	continue;

      EXECUTE_IF_SET_IN_BITMAP (defs, 0, i,
	{
	  struct web *dest = id2web[i];
	  EXECUTE_IF_SET_IN_BITMAP (uses, 0, j,
	    {
	      if (i != j
		  && !TEST_BIT (sup_igraph, i * num_webs + j)
		  && !TEST_BIT (sup_igraph, j * num_webs + i))
	        {
		  struct web *source = id2web[j];
		  if (GET_MODE (source->orig_x) == GET_MODE (dest->orig_x)
		      && hard_regs_intersect_p (&source->usable_regs,
						&dest->usable_regs))
		    {
		      combine (dest, source);
		      goto out;
		    }
		}
	    });
	});
out:
      /* ANSI C is broken.  It forbids labels at end of compound statements,
	 so fake a non-end.  */
      continue;
#if 0
      for (dlink = DF_INSN_DEFS (df, insn); dlink; dlink = dlink->next)
	{
	  struct web *dest = def2web[DF_REF_ID (dlink->ref)];
	  dest = alias (find_web_for_subweb (dest));
	  if (dest->type != PRECOLORED)
	    {
	      struct df_link *slink;
	      for (slink = DF_INSN_USES (df, insn); slink; slink =
		   slink->next)
		{
		  struct web *source = use2web[DF_REF_ID (slink->ref)];
		  source = alias (find_web_for_subweb (source));
		  if (source->type != PRECOLORED
		      && source != dest
		      /* Coalesced webs end up using the same REG rtx in
			 emit_colors().  So we can only coalesce something
			 of equal modes.  */
		      && GET_MODE (source->orig_x) == GET_MODE (dest->orig_x)
		      && !TEST_BIT (sup_igraph,
				    dest->id * num_webs + source->id)
		      && !TEST_BIT (sup_igraph,
				    source->id * num_webs + dest->id)
		      && hard_regs_intersect_p (&source->usable_regs,
						&dest->usable_regs))
		    {
		      combine (dest, source);
		    }
		}
	    }
	}
#endif
    }
  BITMAP_XFREE (uses);
  BITMAP_XFREE (defs);
}

static void
extended_coalesce_2 (void)
{
  rtx insn;
  struct ra_insn_info info;
  unsigned int n;
  init_web_pairs ();
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn) && (info = insn_df[INSN_UID (insn)]).num_defs)
      for (n = 0; n < info.num_defs; n++)
	{
	  struct web *dest = def2web[DF_REF_ID (info.defs[n])];
	  dest = alias (find_web_for_subweb (dest));
	  if (dest->type != PRECOLORED)
	    {
	      unsigned int n2;
	      for (n2 = 0; n2 < info.num_uses; n2++)
		{
		  struct web *source = use2web[DF_REF_ID (info.uses[n2])];
		  source = alias (find_web_for_subweb (source));
		  if (source->type != PRECOLORED
		      && source != dest
		      /* Coalesced webs end up using the same REG rtx in
			 emit_colors().  So we can only coalesce something
			 of equal modes.  */
		      && GET_MODE (source->orig_x) == GET_MODE (dest->orig_x)
		      && !TEST_BIT (sup_igraph,
				    dest->id * num_webs + source->id)
		      && !TEST_BIT (sup_igraph,
				    source->id * num_webs + dest->id)
		      && hard_regs_intersect_p (&source->usable_regs,
						&dest->usable_regs))
		    add_web_pair_cost (dest, source, 
				       BLOCK_FOR_INSN (insn)->frequency,
				       dest->num_conflicts
				       + source->num_conflicts);
		}
	    }
	}
  sort_and_combine_web_pairs (0);
}

static void
check_uncoalesced_moves (void)
{
  struct move_list *ml;
  struct move *m;
  for (ml = wl_moves; ml; ml = ml->next)
    if ((m = ml->move))
      {
	struct web *s = alias (m->source_web);
	struct web *t = alias (m->target_web);
	if (t->type == PRECOLORED)
	  {
	    struct web *h = s;
	    s = t;
	    t = h;
	  }
	if (s != t
	    && m->type != CONSTRAINED
	    /* Following can happen when a move was coalesced, but later
	       broken up again.  Then s!=t, but m is still MV_COALESCED.  */
	    && m->type != MV_COALESCED
	    && t->type != PRECOLORED
	    && ((s->type == PRECOLORED && ok (t, s))
		|| s->type != PRECOLORED)
	    && !TEST_BIT (sup_igraph, s->id * num_webs + t->id)
	    && !TEST_BIT (sup_igraph, t->id * num_webs + s->id))
	  abort ();
      }
}

static long ticks_build;
static long ticks_rebuild;

/* Perform one pass of iterated coalescing.  */
static int
one_pass (df, rebuild)
     struct df *df;
     int rebuild;
{
  long ticks = clock ();
  int something_spilled;
  remember_conflicts = 0;
  build_i_graph (df);
  remember_conflicts = 1;
  if (!rebuild)
    dump_igraph_machine ();
  if (rtl_dump_file)
    dump_igraph (df);
  build_worklists (df);
  if (flag_ra_optimistic_coalescing)
    {
      aggressive_coalesce ();
      /*aggressive_coalesce_2 ();*/
      extended_coalesce_2 ();
    }
  /*  splits_init (); */
  do
    {
      simplify ();
      if (mv_worklist)
	coalesce ();
      else if (WEBS(FREEZE))
	freeze ();
      else if (WEBS(SPILL))
	select_spill ();
    }
  while (WEBS(SIMPLIFY) || WEBS(SIMPLIFY_FAT) || WEBS(SIMPLIFY_SPILL)
	 || mv_worklist || WEBS(FREEZE) || WEBS(SPILL));
  if (flag_ra_optimistic_coalescing)
    check_uncoalesced_moves ();
  assign_colors ();
  check_colors ();
  dump_graph_cost (DUMP_COSTS, "initially");
  if (flag_ra_break_aliases)
    break_coalesced_spills ();
  check_colors ();
  recolor_spills ();
  dump_graph_cost (DUMP_COSTS, "after spill-recolor");
  check_colors ();
  last_max_uid = get_max_uid ();
  /* actual_spill() might change WEBS(SPILLED) and even empty it,
     so we need to remember it's state.  */
  something_spilled = !!WEBS(SPILLED);
  if (something_spilled)
    actual_spill ();
  ticks = clock () - ticks;
  if (rebuild)
    ticks_rebuild += ticks;
  else
    ticks_build += ticks;
  return something_spilled;
}

static void
dump_constraints (void)
{
  rtx insn;
  int i;
  if (!rtl_dump_file || (debug_new_regalloc & DUMP_CONSTRAINTS) == 0)
    return;
  for (i = FIRST_PSEUDO_REGISTER; i < ra_max_regno; i++)
    if (regno_reg_rtx[i] && GET_CODE (regno_reg_rtx[i]) == REG)
      REGNO (regno_reg_rtx[i])
	  = ra_reg_renumber[i] >= 0 ? ra_reg_renumber[i] : i;
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
	    debug_msg (DUMP_CONSTRAINTS,
		       "%d: asm insn or not recognizable.\n", uid);
	    continue;
	  }
	debug_msg (DUMP_CONSTRAINTS,
		   "%d: code %d {%s}, %d operands, constraints: ",
		   uid, code, insn_data[code].name, recog_data.n_operands);
        extract_insn (insn);
	/*preprocess_constraints ();*/
	for (o = 0; o < recog_data.n_operands; o++)
	  {
	    debug_msg (DUMP_CONSTRAINTS,
		       "%d:%s ", o, recog_data.constraints[o]);
	  }
	if (constrain_operands (1))
	  debug_msg (DUMP_CONSTRAINTS, "matches strictly alternative %d",
		     which_alternative);
	else
	  debug_msg (DUMP_CONSTRAINTS, "doesn't match strictly");
	debug_msg (DUMP_CONSTRAINTS, "\n");
      }
  for (i = FIRST_PSEUDO_REGISTER; i < ra_max_regno; i++)
    if (regno_reg_rtx[i] && GET_CODE (regno_reg_rtx[i]) == REG)
      REGNO (regno_reg_rtx[i]) = i;
}

static void
dump_cost (level)
     unsigned int level;
{
#define LU HOST_WIDE_INT_PRINT_UNSIGNED
  debug_msg (level, "Instructions for spilling\n added:\n");
  debug_msg (level, "  loads =%d cost=" LU "\n", emitted_spill_loads,
	     spill_load_cost);
  debug_msg (level, "  stores=%d cost=" LU "\n", emitted_spill_stores,
	     spill_store_cost);
  debug_msg (level, "  remat =%d cost=" LU "\n", emitted_remat,
	     spill_remat_cost);
  debug_msg (level, " removed:\n");
  debug_msg (level, "  moves =%d cost=" LU "\n", deleted_move_insns,
	     deleted_move_cost);
  debug_msg (level, "  others=%d cost=" LU "\n", deleted_def_insns,
	     deleted_def_cost);
#undef LU
}

static void
dump_graph_cost (level, msg)
     unsigned int level;
     const char *msg;
{
  unsigned int i;
  unsigned HOST_WIDE_INT cost;
#define LU HOST_WIDE_INT_PRINT_UNSIGNED
  if (!rtl_dump_file || (debug_new_regalloc & level) == 0)
    return;

  cost = 0;
  for (i = 0; i < num_webs; i++)
    {
      struct web *web = id2web[i];
      if (alias (web)->type == SPILLED)
	cost += web->orig_spill_cost;
    }
  debug_msg (level, " spill cost of graph (%s) = " LU "\n",
	     msg ? msg : "", cost);
#undef LU
}

/* Dump debugging info for the register allocator.  */
static void
dump_ra (df)
     struct df *df ATTRIBUTE_UNUSED;
{
  struct web *web;
  struct dlist *d;
  if (!rtl_dump_file || (debug_new_regalloc & DUMP_RESULTS) == 0)
    return;
    
  debug_msg (DUMP_RESULTS, "\nColored:\n");
  for (d = WEBS(COLORED); d; d = d->next)
    {
      web = DLIST_WEB (d);
      debug_msg (DUMP_RESULTS, "  %4d : color %d\n", web->id, web->color);
    }
  debug_msg (DUMP_RESULTS, "\nCoalesced:\n");
  for (d = WEBS(COALESCED); d; d = d->next)
    {
      web = DLIST_WEB (d);
      debug_msg (DUMP_RESULTS, "  %4d : to web %d, color %d\n", web->id,
	         alias (web)->id, web->color);
    }
  debug_msg (DUMP_RESULTS, "\nSpilled:\n");
  for (d = WEBS(SPILLED); d; d = d->next)
    {
      web = DLIST_WEB (d);
      debug_msg (DUMP_RESULTS, "  %4d\n", web->id);
    }
  debug_msg (DUMP_RESULTS, "\n");
  dump_cost (DUMP_RESULTS);
}

/* Initialize the register allocator.  */
static void
init_ra (void)
{
  int i;
  HARD_REG_SET rs;
#ifdef ELIMINABLE_REGS
  static struct {int from, to; } eliminables[] = ELIMINABLE_REGS;
  unsigned int j;
#endif
  int need_fp
    = (! flag_omit_frame_pointer
#ifdef EXIT_IGNORE_STACK
       || (current_function_calls_alloca && EXIT_IGNORE_STACK)
#endif
       || FRAME_POINTER_REQUIRED);
  /* FIXME: Choose spill heuristic for platform if we have one */
  spill_heuristic = default_spill_heuristic;

  COPY_HARD_REG_SET (never_use_colors, fixed_reg_set);

#ifdef ELIMINABLE_REGS
  for (j = 0; j < ARRAY_SIZE (eliminables); j++)
    {
      if (! CAN_ELIMINATE (eliminables[j].from, eliminables[j].to)
	  || (eliminables[j].to == STACK_POINTER_REGNUM && need_fp))
	for (i = HARD_REGNO_NREGS (eliminables[j].from, Pmode); i--;)
	  SET_HARD_REG_BIT (never_use_colors, eliminables[j].from + i);
    }
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
  if (need_fp)
    for (i = HARD_REGNO_NREGS (HARD_FRAME_POINTER_REGNUM, Pmode); i--;)
      SET_HARD_REG_BIT (never_use_colors, HARD_FRAME_POINTER_REGNUM + i);
#endif

#else
  if (need_fp)
    for (i = HARD_REGNO_NREGS (FRAME_POINTER_REGNUM, Pmode); i--;)
      SET_HARD_REG_BIT (never_use_colors, FRAME_POINTER_REGNUM + i);
#endif
/*
#if HARD_FRAME_POINTER_REGNUM != FRAME_POINTER_REGNUM
  for (i = HARD_REGNO_NREGS (HARD_FRAME_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, HARD_FRAME_POINTER_REGNUM + i);
#endif

  for (i = HARD_REGNO_NREGS (FRAME_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, FRAME_POINTER_REGNUM + i);
*/
  for (i = HARD_REGNO_NREGS (STACK_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, STACK_POINTER_REGNUM + i);

  for (i = HARD_REGNO_NREGS (ARG_POINTER_REGNUM, Pmode); i--;)
    SET_HARD_REG_BIT (never_use_colors, ARG_POINTER_REGNUM + i);
	
  for (i = 0; i < 256; i++)
    {
      unsigned char byte = ((unsigned) i) & 0xFF;
      unsigned char count = 0;
      while (byte)
	{
	  if (byte & 1)
	    count++;
	  byte >>= 1;
	}
      byte2bitcount[i] = count;
    }
  
  for (i = 0; i < N_REG_CLASSES; i++)
    {
      int size;
      COPY_HARD_REG_SET (rs, reg_class_contents[i]);
      AND_COMPL_HARD_REG_SET (rs, never_use_colors);
      size = hard_regs_count (rs);
      num_free_regs[i] = size;
      COPY_HARD_REG_SET (usable_regs[i], rs);
    }

  for (i = 0; i < NUM_MACHINE_MODES; i++)
    {
      int reg, size;
      CLEAR_HARD_REG_SET (rs);
      for (reg = 0; reg < FIRST_PSEUDO_REGISTER; reg++)
	if (HARD_REGNO_MODE_OK (reg, i)
	    /* Ignore VOIDmode and similar things.  */
	    && (size = HARD_REGNO_NREGS (reg, i)) != 0
	    && (reg + size) <= FIRST_PSEUDO_REGISTER)
	  {
	    while (size--)
	      SET_HARD_REG_BIT (rs, reg + size);
	  }
      COPY_HARD_REG_SET (hardregs_for_mode[i], rs);
    }
  
  orig_max_uid = get_max_uid ();
  compute_bb_for_insn (get_max_uid ());
  ra_reg_renumber = NULL;
  insns_with_deaths = sbitmap_alloc (orig_max_uid);
  death_insns_max_uid = orig_max_uid;
  sbitmap_ones (insns_with_deaths);
  gcc_obstack_init (&ra_obstack);
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
     unsigned int name ATTRIBUTE_UNUSED;
     int *colors ATTRIBUTE_UNUSED;
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
	  unsigned int j;
	  struct web *web = ID2WEB (i);
	  for (j = 0; j < web->num_defs; j++)
	    split_ranges[i].stores += powraise (depths[DF_REF_BB (web->defs[j])->index]);
	  for (j = 0; j < web->num_uses; j++)
	    split_ranges[i].loads += powraise (depths[DF_REF_BB (web->uses[j])->index]);
	}
      any_splits_found = FALSE;
    }
}

static void
setup_renumber (free_it)
     int free_it;
{
  int i;
  max_regno = max_reg_num ();
  allocate_reg_info (max_regno, FALSE, TRUE);
  for (i = 0; i < max_regno; i++)
    {
      reg_renumber[i] = (i < ra_max_regno) ? ra_reg_renumber[i] : -1;
    }
  if (free_it)
    {
      free (ra_reg_renumber);
      ra_reg_renumber = NULL;
      ra_max_regno = 0;
    }
}

/* Check the consistency of DF.  */
static void
check_df (df)
     struct df *df;
{
  struct df_link *link;
  rtx insn;
  int regno;
  unsigned int ui;
  bitmap b = BITMAP_XMALLOC ();
  bitmap empty_defs = BITMAP_XMALLOC ();
  bitmap empty_uses = BITMAP_XMALLOC ();

  for (ui = 0; ui < df->def_id; ui++)
    if (!df->defs[ui])
      bitmap_set_bit (empty_defs, ui);
  for (ui = 0; ui < df->use_id; ui++)
    if (!df->uses[ui])
      bitmap_set_bit (empty_uses, ui);
  
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	bitmap_clear (b);
	for (link = DF_INSN_DEFS (df, insn); link; link = link->next)
	  if (!link->ref || bitmap_bit_p (empty_defs, DF_REF_ID (link->ref))
	      || bitmap_bit_p (b, DF_REF_ID (link->ref)))
	    abort ();
	  else
	    bitmap_set_bit (b, DF_REF_ID (link->ref));
			    
	bitmap_clear (b);
	for (link = DF_INSN_USES (df, insn); link; link = link->next)
	  if (!link->ref || bitmap_bit_p (empty_uses, DF_REF_ID (link->ref))
	      || bitmap_bit_p (b, DF_REF_ID (link->ref)))
	    abort ();
	  else
	    bitmap_set_bit (b, DF_REF_ID (link->ref));
      }

  for (regno = 0; regno < max_reg_num (); regno++)
    {
      bitmap_clear (b);
      for (link = df->regs[regno].defs; link; link = link->next)
	if (!link->ref || bitmap_bit_p (empty_defs, DF_REF_ID (link->ref))
	    || bitmap_bit_p (b, DF_REF_ID (link->ref)))
	  abort ();
	else
	  bitmap_set_bit (b, DF_REF_ID (link->ref));

      bitmap_clear (b);
      for (link = df->regs[regno].uses; link; link = link->next)
	if (!link->ref || bitmap_bit_p (empty_uses, DF_REF_ID (link->ref))
	    || bitmap_bit_p (b, DF_REF_ID (link->ref)))
	  abort ();
	else
	  bitmap_set_bit (b, DF_REF_ID (link->ref));
    }
  
  BITMAP_XFREE (empty_uses);
  BITMAP_XFREE (empty_defs);
  BITMAP_XFREE (b);
}

/* Due to resons documented elsewhere we create different pseudos
   for all webs coalesced to hardregs.  For these parts life_analysis()
   might have added REG_DEAD notes without considering, that only this part
   but not the whole coalesced web dies.  The RTL is correct, there is no
   coalescing yet.  But if later reload's alter_reg() substitutes the
   hardreg into the REG rtx it looks like that particular hardreg dies here,
   although (due to coalescing) it still is live.  This might make different
   places of reload think, it can use that hardreg for reload regs,
   accidentally overwriting it.  So we need to remove those REG_DEAD notes.
   (Or better teach life_analysis() and reload about our coalescing, but
   that comes later) Bah.  */
static void
remove_suspicious_death_notes (void)
{
  rtx insn;
  for (insn = get_insns(); insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      {
	rtx *pnote = &REG_NOTES (insn);
	while (*pnote)
	  {
	    rtx note = *pnote;
	    if ((REG_NOTE_KIND (note) == REG_DEAD
		 || REG_NOTE_KIND (note) == REG_UNUSED)
		&& (GET_CODE (XEXP (note, 0)) == REG
		    && bitmap_bit_p (regnos_coalesced_to_hardregs,
				     REGNO (XEXP (note, 0)))))
	      *pnote = XEXP (note, 1);
	    else
	      pnote = &XEXP (*pnote, 1);
	  }
      }
  BITMAP_XFREE (regnos_coalesced_to_hardregs);
  regnos_coalesced_to_hardregs = NULL;
}

/* XXX see recog.c  */
extern int while_newra;

/* Main register allocator entry point.  */
void
reg_alloc (void)
{
  int changed;
  FILE *ra_dump_file = rtl_dump_file;

  switch (0)
    {
      case 0: debug_new_regalloc = DUMP_EVER; break;
      case 1: debug_new_regalloc = DUMP_COSTS; break;
      case 2: debug_new_regalloc = DUMP_IGRAPH_M; break;
      case 3: debug_new_regalloc = DUMP_COLORIZE + DUMP_COSTS; break;
      case 4: debug_new_regalloc = DUMP_COLORIZE + DUMP_COSTS + DUMP_WEBS;
	      break;
      case 5: debug_new_regalloc = DUMP_FINAL_RTL + DUMP_COSTS +
	      DUMP_CONSTRAINTS;
	      break;
    }
  if (!rtl_dump_file)
    debug_new_regalloc = 0;

  if ((debug_new_regalloc & DUMP_REGCLASS) == 0)
    rtl_dump_file = NULL;
  regclass (get_insns (), max_reg_num (), rtl_dump_file);
  rtl_dump_file = ra_dump_file;

/*  ra_info = ra_info_init (max_reg_num ());
  pre_reload (ra_info);
  {
    allocate_reg_info (max_reg_num (), FALSE, FALSE);
    compute_bb_for_insn (get_max_uid ());
    delete_trivially_dead_insns (get_insns (), max_reg_num (), 1);
    reg_scan_update (get_insns (), BLOCK_END (n_basic_blocks - 1),
		     max_regno);
    max_regno = max_reg_num ();
    regclass (get_insns (), max_reg_num (), rtl_dump_file);
  }
  ra_info_free (ra_info);
  free (ra_info);*/
  
  split_live_ranges = FALSE;
  /* XXX the REG_EQUIV notes currently are screwed up, when pseudos are
     coalesced, which have such notes.  In that case, the whole combined
     web gets that note too, which is wrong.  */
  /*update_equiv_regs();*/
  init_ra ();
  /*  find_nesting_depths (); */
  ra_pass = 0;
  no_new_pseudos = 0;
  /* We don't use those NOTEs, and as we anyway change all registers,
     they only make problems later.  */
  count_or_remove_death_notes (NULL, 1);
  emitted_spill_loads = 0;
  emitted_spill_stores = 0;
  emitted_remat = 0;
  spill_load_cost = 0;
  spill_store_cost = 0;
  spill_remat_cost = 0;
  deleted_move_insns = 0;
  deleted_move_cost = 0;
  deleted_def_insns = 0;
  deleted_def_cost = 0;
  last_def_id = 0;
  last_use_id = 0;
  last_num_webs = 0;
  last_max_uid = 0;
  last_check_uses = NULL;
  live_at_end = NULL;
  WEBS(INITIAL) = NULL;
  WEBS(FREE) = NULL;
  memset (hardreg2web, 0, sizeof (hardreg2web));
  ticks_build = ticks_rebuild = 0;
  flag_ra_biased = 0;
  flag_ra_spill_every_use = 0;
  flag_ra_ir_spilling = 1;
  flag_ra_break_aliases = 0;
  flag_ra_optimistic_coalescing = 1;
  flag_ra_merge_spill_costs = 1;
  if (flag_ra_optimistic_coalescing)
    flag_ra_break_aliases = 1;
  flag_ra_dump_notes = 0;
  df = df_init ();
  do
    {
      debug_msg (DUMP_NEARLY_EVER, "RegAlloc Pass %d\n\n", ra_pass);
      if (ra_pass++ > 40)
	internal_error ("Didn't find a coloring.\n");

      /* FIXME denisc@overta.ru
	 Example of usage ra_info ... routines */
#if 0
      ra_info = ra_info_init (max_reg_num ());
      pre_reload (ra_info);

      {
	allocate_reg_info (max_reg_num (), FALSE, FALSE);
	compute_bb_for_insn (get_max_uid ());
	delete_trivially_dead_insns (get_insns (), max_reg_num (), 1);
	reg_scan_update (get_insns (), BLOCK_END (n_basic_blocks - 1),
			 max_regno);
	max_regno = max_reg_num ();
	regclass (get_insns (), max_reg_num (), rtl_dump_file);
	orig_max_uid = get_max_uid ();
	death_insns_max_uid = orig_max_uid;
      }
#endif
      
      df_analyse (df, (ra_pass == 1) ? 0 : (bitmap) -1,
		  DF_HARD_REGS | DF_RD_CHAIN | DF_RU_CHAIN
#ifndef NO_REMAT
		  | DF_DU_CHAIN | DF_UD_CHAIN
#endif
		 );
      
      /* FIXME denisc@overta.ru
	 Example of usage ra_info ... routines */
#if 0
      df2ra = build_df2ra (df, ra_info);
#endif
      if ((debug_new_regalloc & DUMP_DF) != 0)
	{
	  rtx insn;
	  df_dump (df, DF_HARD_REGS, rtl_dump_file);
	  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
            if (INSN_P (insn))
	      {
	        df_insn_debug_regno (df, insn, rtl_dump_file);
	      }
	}
      check_df (df);
      alloc_mem (df);
      changed = one_pass (df, ra_pass > 1);

      /* FIXME denisc@overta.ru
	 Example of usage ra_info ... routines */
#if 0
      ra_info_free (ra_info);
      free (df2ra.def2def);
      free (df2ra.use2use);
      free (ra_info);
#endif 
      if (!changed)
	{
          emit_colors (df);
	  /* Already setup a preliminary reg_renumber[] array, but don't
	     free our own version.  reg_renumber[] will again be destroyed
	     later.  We right now need it in dump_constraints() for
	     constrain_operands(1) whose subproc sometimes reference
	     it (because we are cehcking strictly, i.e. as if
	     after reload).  */
	  setup_renumber (0);
	  delete_moves ();
	  dump_constraints ();
	}
      else
	{
	  if ((debug_new_regalloc & DUMP_REGCLASS) == 0)
	    rtl_dump_file = NULL;
	  allocate_reg_info (max_reg_num (), FALSE, FALSE);
	  compute_bb_for_insn (get_max_uid ());
	  delete_trivially_dead_insns (get_insns (), max_reg_num (), 1);
	  reg_scan_update (get_insns (), BLOCK_END (n_basic_blocks - 1),
			   max_regno);
	  max_regno = max_reg_num ();
	  regclass (get_insns (), max_reg_num (), rtl_dump_file);
	  rtl_dump_file = ra_dump_file;

	  last_def_id = df->def_id;
	  last_use_id = df->use_id;
	}
      dump_ra (df);
      if (changed && (debug_new_regalloc & DUMP_RTL) != 0)
	{
	  ra_print_rtl_with_bb (rtl_dump_file, get_insns ());
	  fflush (rtl_dump_file);
	}
      reset_lists ();
      free_mem (df);
    }
  while (changed);
  free_all_mem (df);
  /*  free (depths); */
  df_finish (df);
  if ((debug_new_regalloc & DUMP_RESULTS) == 0)
    dump_cost (DUMP_COSTS);
  /*debug_msg (DUMP_COSTS, "ticks for build-phase: %ld\n", ticks_build);
  debug_msg (DUMP_COSTS, "ticks for rebuild-phase: %ld\n", ticks_rebuild);*/
  if ((debug_new_regalloc & (DUMP_FINAL_RTL | DUMP_RTL)) != 0)
    ra_print_rtl_with_bb (rtl_dump_file, get_insns ()); 
  
  if ((debug_new_regalloc & DUMP_SM) == 0)
    rtl_dump_file = NULL;
  no_new_pseudos = 0;
  allocate_reg_info (max_reg_num (), FALSE, FALSE);
  compute_bb_for_insn (get_max_uid ());
  while_newra = 1;
  store_motion ();
  no_new_pseudos = 1;
  rtl_dump_file = ra_dump_file;

  if ((debug_new_regalloc & DUMP_LAST_FLOW) == 0)
    rtl_dump_file = NULL;
  find_basic_blocks (get_insns (), max_reg_num (), rtl_dump_file);
  compute_bb_for_insn (get_max_uid ());
  clear_log_links (get_insns ());
  life_analysis (get_insns (), rtl_dump_file, 
		 PROP_DEATH_NOTES | PROP_LOG_LINKS  | PROP_REG_INFO);
/*  recompute_reg_usage (get_insns (), TRUE);
  life_analysis (get_insns (), rtl_dump_file, 
		 PROP_SCAN_DEAD_CODE | PROP_KILL_DEAD_CODE); */
  cleanup_cfg (CLEANUP_EXPENSIVE);
  recompute_reg_usage (get_insns (), TRUE);
/*  delete_trivially_dead_insns (get_insns (), max_reg_num (), 1);*/
  if (rtl_dump_file)
    dump_flow_info (rtl_dump_file);
	  
  /* XXX: reg_scan screws up reg_renumber, and without reg_scan, we can't do
     regclass. */
  /*reg_scan (get_insns (), max_reg_num (), 1);
    regclass (get_insns (), max_reg_num (), rtl_dump_file); */
  rtl_dump_file = ra_dump_file;

  /* Also update_equiv_regs() can't be called after register allocation.
     It might delete some pseudos, and insert other insns setting
     up those pseudos in different places.  This of course screws up
     the allocation because that may destroy a hardreg for another
     pseudo.
     XXX we probably should do something like that on our own.  I.e.
     creating REG_EQUIV notes.  */
  /*update_equiv_regs ();*/
  /* We must maintain our own reg_renumber[] array, because life_analysis()
     destroys any prior set up reg_renumber[].  */
  while_newra = 0;
  setup_renumber (1);
  sbitmap_free (insns_with_deaths);

  remove_suspicious_death_notes ();
  if ((debug_new_regalloc & DUMP_LAST_RTL) != 0)
    ra_print_rtl_with_bb (rtl_dump_file, get_insns ()); 
}

static int web_conflicts_p PARAMS ((struct web *, struct web *));

static int
web_conflicts_p (web1, web2)
     struct web * web1;
     struct web * web2;
{
  if (web1->type == PRECOLORED && web2->type == PRECOLORED)
    return 0;
  
  if (web1->type == PRECOLORED)
    return TEST_HARD_REG_BIT (web2->usable_regs, web1->regno);

  if (web2->type == PRECOLORED)
    return TEST_HARD_REG_BIT (web1->usable_regs, web2->regno);
    
  return hard_regs_intersect_p (&web1->usable_regs, &web2->usable_regs);
}

/* Dump all insns from one web.  */
static void
dump_web_insns (web)
     struct web* web;
{
  unsigned int i;
  
  debug_msg (DUMP_EVER, "Web: %i(%i)+%i class: %s freedom: %i degree %i\n",
	     web->id, web->regno, web->add_hardregs,
	     reg_class_names[web->regclass],
	     web->num_freedom, web->num_conflicts);
  debug_msg (DUMP_EVER, "   def insns:");
  
  for (i = 0; i < web->num_defs; ++i)
    {
      debug_msg (DUMP_EVER, " %d ", INSN_UID (web->defs[i]->insn));
    }

  debug_msg (DUMP_EVER, "\n   use insns:");
  for (i = 0; i < web->num_uses; ++i)
    {
      debug_msg (DUMP_EVER, " %d ", INSN_UID (web->uses[i]->insn));
    }
  debug_msg (DUMP_EVER, "\n");
}


/* Dump conflicts for web WEB.  */
static void
dump_web_conflicts (web)
     struct web *web;
{
  int num = 0;
  unsigned int def2;

  debug_msg (DUMP_EVER, "Web: %i(%i)+%i class: %s freedom: %i degree %i\n",
	     web->id, web->regno, web->add_hardregs,
	     reg_class_names[web->regclass],
	     web->num_freedom, web->num_conflicts);
  
  for (def2 = 0; def2 < num_webs; def2++)
    if (TEST_BIT (igraph, igraph_index (web->id, def2)) && web->id != def2)
      {
	if ((num % 9) == 5)
	  debug_msg (DUMP_EVER, "\n             ");
	num++;
	
	debug_msg (DUMP_EVER, " %d(%d)", def2, id2web[def2]->regno);
	if (id2web[def2]->add_hardregs)
	  debug_msg (DUMP_EVER, "+%d", id2web[def2]->add_hardregs);

	if (web_conflicts_p (web, id2web[def2]))
	  debug_msg (DUMP_EVER, "/x");

	if (id2web[def2]->type == SELECT)
	  debug_msg (DUMP_EVER, "/s");
	  
	if (id2web[def2]->type == COALESCED)
	  debug_msg (DUMP_EVER,"/c/%d", alias (id2web[def2])->id);
      }
  debug_msg (DUMP_EVER, "\n");
  {
    struct conflict_link *wl;
    num = 0;
    debug_msg (DUMP_EVER, "By conflicts:     ");
    for (wl = web->conflict_list; wl; wl = wl->next)
      {
	struct web* w = wl->t;
	if ((num % 9) == 8)
	  debug_msg (DUMP_EVER, "\n              ");
	num++;
	debug_msg (DUMP_EVER, "%d(%d)%s ", w->id, w->regno,
		   web_conflicts_p (web, w) ? "+" : "");
      }
    debug_msg (DUMP_EVER, "\n");  
  }
}

void debug_hard_reg_set PARAMS ((HARD_REG_SET));
/* Output HARD_REG_SET to stderr.  */
void
debug_hard_reg_set (set)
     HARD_REG_SET set;
{
  int i;
  for (i=0; i < FIRST_PSEUDO_REGISTER; ++i)
    {
      if (TEST_HARD_REG_BIT (set, i))
	{
	  fprintf (stderr, "%s ", reg_names[i]);
	}
    }
  fprintf (stderr, "\n");
}

static enum reg_class *reg_class_of_web;

static enum reg_class
web_preferred_class (web)
     struct web *web;
{
  if (!reg_class_of_web)
    abort ();

  if (web->id >= num_webs)
    abort ();

  return reg_class_of_web[find_web_for_subweb(web)->id];
}

void
web_class ()
{
  unsigned int n;
  unsigned int i;
  char class[LIM_REG_CLASSES];
  ra_ref *rref;
  struct ref* dref;
  enum reg_class best;

  if (reg_class_of_web)
    free (reg_class_of_web);

  reg_class_of_web = xmalloc (sizeof (enum reg_class) * (num_webs
							 - num_subwebs));
  for (n = 0; n < num_webs - num_subwebs; ++n)
    {
      struct web *web = id2web[n];
      int founded = 0;

      reg_class_of_web[n] = NO_REGS;

      if (web->type == PRECOLORED)
	continue;
      
      for (i = 0; i < LIM_REG_CLASSES; ++i)
	class[i] = 0;

      for (i = 0; i < web->num_defs; ++i)
	{
	  dref = web->defs[i];
	  rref = DF2RA (df2ra, dref);
	  if (rref)
	    ++class[rref->class];
	}

      for (i = 0; i < web->num_uses; ++i)
	{
	  dref = web->uses[i];
	  rref = DF2RA (df2ra, dref);
	  if (rref)
	    ++class[rref->class];
	}

/*        fprintf (stderr, "Web: %d ", web->id); */
      best = ALL_REGS;
      for (i = 0; i < LIM_REG_CLASSES; ++i)
	if (class[i])
	  {
	    if (reg_class_subset_p (i, best))
	      {
		best = i;
		founded = 1;
	      }
	    else if (!reg_class_subset_p (best, i))
	      best = NO_REGS;
/*    	    fprintf (stderr, "%s: %d ", reg_class_names[i], class[i]);  */
	  }
/*        fprintf (stderr, " BEST: %s\n", reg_class_names[best]); */
      if (best == NO_REGS)
	{
    	  fprintf (stderr, "Web: %d (%d) NO_REGS\n", web->id, web->regno);
	  best = GENERAL_REGS;
	}
      reg_class_of_web[n] = best;
    }
}

/*
vim:cinoptions={.5s,g0,p5,t0,(0,^-0.5s,n-0.5s:tw=78:cindent:sw=4:
*/
