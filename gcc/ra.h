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
  unsigned int mode_changed:1; /* != 0 if this web is used in subregs where
				  the mode change was illegal for hardregs
				  in CLASS_CANNOT_CHANGE_MODE.  */
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

struct ra_insn_info
{
  unsigned int num_defs, num_uses;
  struct ref **defs, **uses;
};

int insn_df_max_uid;
struct ra_insn_info *insn_df;

#define igraph_index(i, j) ((i) < (j) ? ((j)*((j)-1)/2)+(i) : ((i)*((i)-1)/2)+(j))
extern sbitmap igraph;
extern sbitmap sup_igraph;

extern sbitmap insns_with_deaths;
extern int death_insns_max_uid;

extern struct web_part *web_parts;

extern unsigned int num_webs;
extern unsigned int num_subwebs;
extern unsigned int num_allwebs;
extern struct web **id2web;
extern struct web *hardreg2web[FIRST_PSEUDO_REGISTER];
extern struct web **def2web;
extern struct web **use2web;
extern struct move_list *wl_moves;
extern int ra_max_regno;
extern short *ra_reg_renumber;
extern struct df *df;
extern bitmap *live_at_end;
extern int ra_pass;
extern unsigned int max_normal_pseudo;
extern int an_unusable_color;

extern int *number_seen;

/* The different lists on which a web can be (based on the type).  */
extern struct dlist *web_lists[(int) LAST_NODE_TYPE];
#define WEBS(type) (web_lists[(int)(type)])

extern unsigned int last_def_id;
extern unsigned int last_use_id;
extern int last_max_uid;
extern unsigned int last_num_webs;
extern sbitmap last_check_uses;
extern unsigned int remember_conflicts;

/* Used to detect spill instructions inserted by me.  */
extern int orig_max_uid;

extern HARD_REG_SET never_use_colors;
extern HARD_REG_SET usable_regs[N_REG_CLASSES];
extern unsigned int num_free_regs[N_REG_CLASSES];
extern HARD_REG_SET hardregs_for_mode[NUM_MACHINE_MODES];
extern unsigned char byte2bitcount[256];

#define ID2WEB(I) id2web[I]
#define NUM_REGS(W) (((W)->type == PRECOLORED) ? 1 : (W)->num_freedom)
#define SUBWEB_P(W) (GET_CODE ((W)->orig_x) == SUBREG)

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
#define DUMP_VALIDIFY	0x20000
#define DUMP_EVER		((unsigned int)-1)
#define DUMP_NEARLY_EVER	(DUMP_EVER - DUMP_COSTS - DUMP_IGRAPH_M)

extern unsigned int debug_new_regalloc;

extern int flag_ra_dump_only_costs;
extern int flag_ra_biased;
extern int flag_ra_ir_spilling;
extern int flag_ra_optimistic_coalescing;
extern int flag_ra_break_aliases;
extern int flag_ra_merge_spill_costs;
extern int flag_ra_spill_every_use;
extern int flag_ra_dump_notes;

extern inline void * ra_alloc PARAMS ((size_t));
extern inline void * ra_calloc PARAMS ((size_t));
extern int hard_regs_count PARAMS ((HARD_REG_SET));
extern rtx ra_emit_move_insn PARAMS ((rtx, rtx));
extern void ra_debug_msg PARAMS ((unsigned int,
			          const char *, ...)) ATTRIBUTE_PRINTF_2;
extern int hard_regs_intersect_p PARAMS ((HARD_REG_SET *, HARD_REG_SET *));
extern unsigned int rtx_to_bits PARAMS ((rtx));
extern struct web * find_subweb PARAMS ((struct web *, rtx));
extern struct web * find_subweb_2 PARAMS ((struct web *, unsigned int));
extern struct web * find_web_for_subweb_1 PARAMS ((struct web *));

#define find_web_for_subweb(w) (((w)->parent_web) \
				? find_web_for_subweb_1 ((w)->parent_web) \
				: (w))

extern void ra_build_realloc PARAMS ((struct df *));
extern void ra_build_free PARAMS ((void));
extern void ra_build_free_all PARAMS ((struct df *));
extern void ra_colorize_init PARAMS ((void));
extern void ra_colorize_free_all PARAMS ((void));
extern void ra_rewrite_init PARAMS ((void));

extern void ra_print_rtx PARAMS ((FILE *, rtx, int));
extern void ra_print_rtx_top PARAMS ((FILE *, rtx, int));
extern void ra_debug_rtx PARAMS ((rtx));
extern void ra_debug_insns PARAMS ((rtx, int));
extern void ra_debug_bbi PARAMS ((int));
extern void ra_print_rtl_with_bb PARAMS ((FILE *, rtx));
extern void dump_igraph PARAMS ((struct df *));
extern void dump_igraph_machine PARAMS ((void));
extern void dump_constraints PARAMS ((void));
extern void dump_cost PARAMS ((unsigned int));
extern void dump_graph_cost PARAMS ((unsigned int, const char *));
extern void dump_ra PARAMS ((struct df *));
extern void dump_number_seen PARAMS ((void));
extern void dump_static_insn_cost PARAMS ((FILE *, const char *,
					   const char *));
extern void dump_web_conflicts PARAMS ((struct web *));
extern void dump_web_insns PARAMS ((struct web*));
extern int web_conflicts_p PARAMS ((struct web *, struct web *));

extern void remove_list PARAMS ((struct dlist *, struct dlist **));
extern struct dlist * pop_list PARAMS ((struct dlist **));
extern void record_conflict PARAMS ((struct web *, struct web *));
extern int memref_is_stack_slot PARAMS ((rtx));
extern void build_i_graph PARAMS ((struct df *));
extern void put_web PARAMS ((struct web *, enum node_type));
extern void remove_web_from_list PARAMS ((struct web *));
extern void reset_lists PARAMS ((void));
extern struct web * alias PARAMS ((struct web *));
extern void merge_moves PARAMS ((struct web *, struct web *));
extern void ra_colorize_graph PARAMS ((struct df *));

extern void actual_spill PARAMS ((void));
extern void emit_colors PARAMS ((struct df *));
extern void delete_moves PARAMS ((void));
extern void setup_renumber PARAMS ((int));
extern void remove_suspicious_death_notes PARAMS ((void));
