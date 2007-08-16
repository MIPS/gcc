/* Instruction scheduling pass.  This file contains definitions used
   internally in the scheduler.
   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef GCC_SEL_SCHED_IR_H
#define GCC_SEL_SCHED_IR_H

/* A define to use ENABLE_CHECKING logic inside conditionals.  */
#ifdef ENABLE_CHECKING
#define ENABLE_SEL_CHECKING 1
#else
#define ENABLE_SEL_CHECKING 0
#endif

/* For state_t.  */
#include "insn-attr.h"
/* For regset_head.  */
#include "basic-block.h"
/* For reg_note.  */
#include "rtl.h"
#include "ggc.h"
#include "bitmap.h"
#include "vecprim.h"
#include "sched-int.h"
#include "sched-rgn.h"
#include "cfgloop.h"

/* tc_t is a short for target context.  This is a state of the target
   backend.  */
typedef void *tc_t;

/* List data types used for av sets, fences, paths, and boundaries.  */

/* Forward declarations for types that are part of some list nodes.  */
struct _list_node;

/* List backend.  */
typedef struct _list_node *_list_t;
#define _LIST_NEXT(L) ((L)->next)

/* Instruction data that is part of vinsn type.  */
struct idata_def;
typedef struct idata_def *idata_t;

/* A virtual instruction, i.e. an instruction as seen by the scheduler.  */
struct vinsn_def;
typedef struct vinsn_def *vinsn_t;


/* RTX list.
   This type is the backend for ilist.  */
typedef _list_t _xlist_t;
#define _XLIST_X(L) ((L)->u.x)
#define _XLIST_NEXT(L) (_LIST_NEXT (L))

/* Instruction.  */
typedef rtx insn_t;

/* List of insns.  */
typedef _xlist_t ilist_t;
#define ILIST_INSN(L) (_XLIST_X (L))
#define ILIST_NEXT(L) (_XLIST_NEXT (L))

/* Right hand side information.  */
struct _expr
{
  /* Insn description.  */
  vinsn_t vinsn;

  /* SPEC is the degree of speculativeness.
     FIXME: now spec is increased when an rhs is moved through a
     conditional, thus showing only control speculativeness.  In the
     future we'd like to count data spec separately to allow a better
     control on scheduling.  */
  int spec;

  /* A priority of this expression.  */
  int priority;

  /* Number of times the insn was scheduled.  */
  int sched_times;

  /* A basic block index this was originated from.  Zero when there is 
     more than one originator.  */
  int orig_bb_index;

  /* Instruction should be of SPEC_DONE_DS type in order to be moved to this
     point.  */
  ds_t spec_done_ds;

  /* SPEC_TO_CHECK_DS hold speculation types that should be checked
     (used only during move_op ()).  */
  ds_t spec_to_check_ds;

  /* A vector of insn's hashes on which this expr was changed when 
     moving up.  We can't use bitmap here, because the recorded insn
     could be scheduled, and its bookkeeping copies should be checked 
     instead.  */
  VEC(unsigned, heap) *changed_on_insns;

  /* True when the expression was substituted.  Used for statistical 
     purposes.  */
  bool was_substituted;
};

typedef struct _expr expr_def;
typedef expr_def *expr_t;

/* Obsolete.  */
typedef expr_t rhs_t;

#define EXPR_VINSN(EXPR) ((EXPR)->vinsn)
#define EXPR_INSN_RTX(EXPR) (VINSN_INSN_RTX (EXPR_VINSN (EXPR)))
#define EXPR_PATTERN(EXPR) (VINSN_PATTERN (EXPR_VINSN (EXPR)))
#define EXPR_LHS(EXPR) (VINSN_LHS (EXPR_VINSN (EXPR)))
#define EXPR_RHS(EXPR) (VINSN_RHS (EXPR_VINSN (EXPR)))
#define EXPR_TYPE(EXPR) (VINSN_TYPE (EXPR_VINSN (EXPR)))
#define EXPR_SEPARABLE_P(EXPR) (VINSN_SEPARABLE_P (EXPR_VINSN (EXPR)))

#define EXPR_SPEC(EXPR) ((EXPR)->spec)
#define EXPR_PRIORITY(EXPR) ((EXPR)->priority)
#define EXPR_SCHED_TIMES(EXPR) ((EXPR)->sched_times)
#define EXPR_ORIG_BB_INDEX(EXPR) ((EXPR)->orig_bb_index)
#define EXPR_SPEC_DONE_DS(EXPR) ((EXPR)->spec_done_ds)
#define EXPR_SPEC_TO_CHECK_DS(EXPR) ((EXPR)->spec_to_check_ds)
#define EXPR_CHANGED_ON_INSNS(EXPR) ((EXPR)->changed_on_insns)
#define EXPR_WAS_SUBSTITUTED(EXPR) ((EXPR)->was_substituted)

/* Obsolete. */
#define RHS_VINSN(RHS) ((RHS)->vinsn)
#define RHS_INSN(RHS) (VINSN_INSN (RHS_VINSN (RHS)))
#define RHS_PATTERN(RHS) (VINSN_PATTERN (RHS_VINSN (RHS)))
#define RHS_SPEC(RHS) ((RHS)->spec)
#define RHS_PRIORITY(RHS) ((RHS)->priority)
#define RHS_DEST(RHS) (VINSN_LHS (RHS_VINSN (RHS))) 
#define RHS_SCHED_TIMES(RHS) ((RHS)->sched_times)

/* Insn definition for list of original insns in find_used_regs.  */
struct _def
{
  insn_t orig_insn;
  bool crosses_call;
  bool needs_spec_check_p;
};
typedef struct _def *def_t;


/* Availability sets arae sets of expressions we're scheduling.  */
typedef _list_t av_set_t;
#define _AV_SET_EXPR(L) (&(L)->u.expr)
#define _AV_SET_NEXT(L) (_LIST_NEXT (L))


/* Scheduling boundary.  This now corresponds to fence as we finish a cycle
   at the end of bb.  */
struct _bnd
{
  insn_t to;
  ilist_t ptr;
  av_set_t av;
  av_set_t av1;
  /* Deps Context at this boundary.  As long as we have one boundary per fence,
     this is just a pointer to the same deps context as in the corresponding
     fence.  */
  deps_t dc;
};
typedef struct _bnd *bnd_t;
#define BND_TO(B) ((B)->to)

/* PTR stands not for pointer as you might think, but as a Path To Root of the
   current instruction group from boundary B.  */
#define BND_PTR(B) ((B)->ptr)
#define BND_AV(B) ((B)->av)
#define BND_AV1(B) ((B)->av1)
#define BND_DC(B) ((B)->dc)

/* List of boundaries.  */
typedef _list_t blist_t;
#define BLIST_BND(L) (&(L)->u.bnd)
#define BLIST_NEXT(L) (_LIST_NEXT (L))


/* Fence information.  A fence represents current scheduling point and also
   blocks code motion through it when pipelining.  */
struct _fence
{
  insn_t insn;

  state_t state;

  /* Current cycle that is being scheduled on this fence.  */
  int cycle;
  /* Number of insns that were scheduled on the current cycle.
     This information has to be local to a fence.  */
  int cycle_issued_insns;

  /* At the end of fill_insns () this field holds the list of the instructions
     that are inner boundaries of the scheduled parallel group.  */
  ilist_t bnds;

  /* Deps Context at this fence.  It is used to model dependencies at the
     fence so that insn ticks can be properly evaluated.  */
  deps_t dc;

  /* Target context at this fence.  Used to save and load any local target
     scheduling information when changing fences.  */
  tc_t tc;

  /* Insn, which has been scheduled last on this fence.  */
  rtx last_scheduled_insn;

  /* If non-NULL force the next scheduled insn to be SCHED_NEXT.  */
  rtx sched_next;

  /* True if fill_insns processed this fence.  */
  BOOL_BITFIELD scheduled : 1;

  /* True if fill_insns actually scheduled something on this fence.  */
  BOOL_BITFIELD scheduled_something : 1;

  /* True when the next insn scheduled here would start a cycle.  */
  BOOL_BITFIELD starts_cycle_p : 1;

  /* True when the next insn scheduled here would be scheduled after a stall.  */
  BOOL_BITFIELD after_stall_p : 1;
};
typedef struct _fence *fence_t;

#define FENCE_INSN(F) ((F)->insn)
#define FENCE_STATE(F) ((F)->state)
#define FENCE_BNDS(F) ((F)->bnds)
#define FENCE_SCHEDULED(F) ((F)->scheduled)
#define FENCE_SCHEDULED_SOMETHING(F) ((F)->scheduled_something)
#define FENCE_ISSUED_INSNS(F) ((F)->cycle_issued_insns)
#define FENCE_CYCLE(F) ((F)->cycle)
#define FENCE_STARTS_CYCLE_P(F) ((F)->starts_cycle_p)
#define FENCE_AFTER_STALL_P(F) ((F)->after_stall_p)
#define FENCE_DC(F) ((F)->dc)
#define FENCE_TC(F) ((F)->tc)
#define FENCE_LAST_SCHEDULED_INSN(F) ((F)->last_scheduled_insn)
#define FENCE_SCHED_NEXT(F) ((F)->sched_next)

/* List of fences.  */
typedef _list_t flist_t;
#define FLIST_FENCE(L) (&(L)->u.fence)
#define FLIST_NEXT(L) (_LIST_NEXT (L))

/* List of fences with pointer to the tail node.  */
struct flist_tail_def
{
  flist_t head;
  flist_t *tailp;
};

typedef struct flist_tail_def *flist_tail_t;
#define FLIST_TAIL_HEAD(L) ((L)->head)
#define FLIST_TAIL_TAILP(L) ((L)->tailp)

/* List node information.  A list node can be any of the types above.  */
struct _list_node
{
  _list_t next;

  union
  {
    rtx x;
    struct _bnd bnd;
    expr_def expr;
    struct _fence fence;
    struct _def def;
    void *data;
  } u;
};


/* _list_t functions.
   All of _*list_* functions are used through accessor macros, thus
   we can't move them in sel-sched-ir.c.  */

static inline void
_list_add (_list_t *lp)
{
  _list_t l = xmalloc (sizeof (*l));

  _LIST_NEXT (l) = *lp;
  *lp = l;
}

static inline void
_list_remove (_list_t *lp)
{
  _list_t n = *lp;

  *lp = _LIST_NEXT (n);
  free (n);
}

static inline void
_list_clear (_list_t *l)
{
  while (*l)
    _list_remove (l);
}


/* List iterator backend.  */
typedef struct
{
  _list_t *lp;
  bool can_remove_p;
  bool removed_p;
} _list_iterator;

static inline void
_list_iter_start (_list_iterator *ip, _list_t *lp, bool can_remove_p)
{
  ip->lp = lp;
  ip->can_remove_p = can_remove_p;
  ip->removed_p = false;
}

static inline void
_list_iter_next (_list_iterator *ip)
{
  if (!ip->removed_p)
    ip->lp = &_LIST_NEXT (*ip->lp);
  else
    ip->removed_p = false;
}

static inline void
_list_iter_remove (_list_iterator *ip)
{
  gcc_assert (!ip->removed_p && ip->can_remove_p);
  _list_remove (ip->lp);
  ip->removed_p = true;
}

/* General macros to traverse a list.  FOR_EACH_* interfaces are
   implemented using these.  */
#define _FOR_EACH(TYPE, ELEM, I, L)				\
  for (_list_iter_start (&(I), &(L), false);			\
       _list_iter_cond_##TYPE (*(I).lp, &(ELEM));		\
       _list_iter_next (&(I)))

#define _FOR_EACH_1(TYPE, ELEM, I, LP)			\
  for (_list_iter_start (&(I), (LP), true);		\
       _list_iter_cond_##TYPE (*(I).lp, &(ELEM));	\
       _list_iter_next (&(I)))


/* _xlist_t functions.  */

static inline void
_xlist_add (_xlist_t *lp, rtx x)
{
  _list_add (lp);
  _XLIST_X (*lp) = x;
}

#define _xlist_remove(LP) (_list_remove (LP))
#define _xlist_clear(LP) (_list_clear (LP))

static inline bool
_xlist_is_in_p (_xlist_t l, rtx x)
{
  while (l)
    {
      if (_XLIST_X (l) == x)
        return true;
      l = _XLIST_NEXT (l);
    }

  return false;
}

/* Used through _FOR_EACH.  */
static inline bool
_list_iter_cond_x (_xlist_t l, rtx *xp)
{
  if (l)
    {
      *xp = _XLIST_X (l);
      return true;
    }

  return false;
}

#define _xlist_iter_remove(IP) (_list_iter_remove (IP))

typedef _list_iterator _xlist_iterator;
#define _FOR_EACH_X(X, I, L) _FOR_EACH (x, (X), (I), (L))
#define _FOR_EACH_X_1(X, I, LP) _FOR_EACH_1 (x, (X), (I), (LP))


/* ilist_t functions.  Instruction lists are simply RTX lists.  */

#define ilist_add(LP, INSN) (_xlist_add ((LP), (INSN)))
#define ilist_remove(LP) (_xlist_remove (LP))
#define ilist_clear(LP) (_xlist_clear (LP))
#define ilist_is_in_p(L, INSN) (_xlist_is_in_p ((L), (INSN)))
#define ilist_iter_remove(IP) (_xlist_iter_remove (IP))

typedef _xlist_iterator ilist_iterator;
#define FOR_EACH_INSN(INSN, I, L) _FOR_EACH_X (INSN, I, L)
#define FOR_EACH_INSN_1(INSN, I, LP) _FOR_EACH_X_1 (INSN, I, LP)


/* Av set iterators.  */
typedef _list_iterator av_set_iterator;
#define FOR_EACH_RHS(RHS, I, AV) _FOR_EACH (rhs, (RHS), (I), (AV))
#define FOR_EACH_RHS_1(RHS, I, AV) _FOR_EACH_1 (rhs, (RHS), (I), (AV))

static bool
_list_iter_cond_rhs (av_set_t av, rhs_t *rhsp)
{
  if (av)
    {
      *rhsp = _AV_SET_EXPR (av);
      return true;
    }

  return false;
}


/* Def list iterators.  */
typedef _list_t def_list_t;
typedef _list_iterator def_list_iterator;

#define DEF_LIST_NEXT(L) (_LIST_NEXT (L))
#define DEF_LIST_DEF(L) (&(L)->u.def)

#define FOR_EACH_DEF(DEF, I, DEF_LIST) _FOR_EACH (def, (DEF), (I), (DEF_LIST))

static inline bool
_list_iter_cond_def (def_list_t def_list, def_t *def)
{
  if (def_list)
    {
      *def = DEF_LIST_DEF (def_list);
      return true;
    }

  return false;
}


/* InstructionData.  Contains information about insn pattern.  */
struct idata_def
{
  /* Type of the insn.
     o CALL_INSN - Call insn
     o JUMP_INSN - Jump insn
     o INSN - INSN that cannot be cloned
     o USE - INSN that can be cloned
     o SET - INSN that can be cloned and separable into lhs and rhs
     o PC - simplejump.  Insns that simply redirect control flow should not
     have any dependencies.  Sched-deps.c, though, might consider them as
     producers or consumers of certain registers.  To avoid that we handle
     dependency for simple jumps ourselves.  */
  int type;

  /* If insn is a SET, this is its left hand side.  */
  rtx lhs;

  /* If insn is a SET, this is its right hand side.  */
  rtx rhs;

  /* Registers that are set/used by this insn.  This info is now gathered
     via sched-deps.c.  The downside of this is that we also use live info
     from flow that is accumulated in the basic blocks.  These two infos
     can be slightly inconsistent, hence in the beginning we make a pass
     through CFG and calculating the conservative solution for the info in
     basic blocks.  When this scheduler will be switched to use dataflow,
     this can be unified as df gives us both per basic block and per
     instruction info.  Actually, we don't do that pass and just hope
     for the best.  */
  regset reg_sets;

  regset reg_uses;

  /* I hope that our renaming infrastructure handles this.  */
  /* bool has_internal_dep; */
};

#define IDATA_TYPE(ID) ((ID)->type)
#define IDATA_LHS(ID) ((ID)->lhs)
#define IDATA_RHS(ID) ((ID)->rhs)
#define IDATA_REG_SETS(ID) ((ID)->reg_sets)
#define IDATA_REG_USES(ID) ((ID)->reg_uses)

/* Type to represent all needed info to emit an insn.
   This is a virtual equivalent of the insn.

   vinsn can be either normal or unique.
   * Normal vinsn is the one, that can be cloned multiple times and typically
   corresponds to normal instruction.

   * Unique vinsn derivates from CALL, ASM, JUMP (for a while) and other
   unusual stuff.  Such a vinsn is described by its INSN field, which is a
   reference to the original instruction.  */
struct vinsn_def
{
  /* Associated insn.  */
  rtx insn_rtx;

  /* Its description.  */
  idata_t id;

  /* Hash of vinsn.  It is computed either from pattern or from rhs using
     hash_rtx.  It is not placed in ID for faster compares.  */
  unsigned hash;

  /* Smart pointer counter.  */
  int count;

  /* Cached cost of the vinsn.  To access it please use vinsn_cost ().  */
  int cost;

  /* Mark insns that may trap so we don't move them through jumps.  */
  bool may_trap_p;
};

#define VINSN_INSN_RTX(VI) ((VI)->insn_rtx)
#define VINSN_PATTERN(VI) (PATTERN (VINSN_INSN_RTX (VI)))
/* Obsolete.  */
#define VINSN_INSN(VI) (VINSN_INSN_RTX (VI))

#define VINSN_ID(VI) ((VI)->id)
#define VINSN_HASH(VI) ((VI)->hash)
#define VINSN_TYPE(VI) (IDATA_TYPE (VINSN_ID (VI)))
#define VINSN_SEPARABLE_P(VI) (VINSN_TYPE (VI) == SET)
#define VINSN_CLONABLE_P(VI) (VINSN_SEPARABLE_P (VI) || VINSN_TYPE (VI) == USE)
#define VINSN_UNIQUE_P(VI) (!VINSN_CLONABLE_P (VI))
#define VINSN_LHS(VI) (IDATA_LHS (VINSN_ID (VI)))
#define VINSN_RHS(VI) (IDATA_RHS (VINSN_ID (VI)))
#define VINSN_REG_SETS(VI) (IDATA_REG_SETS (VINSN_ID (VI)))
#define VINSN_REG_USES(VI) (IDATA_REG_USES (VINSN_ID (VI)))

#define VINSN_COUNT(VI) ((VI)->count)

#define VINSN_MAY_TRAP_P(VI) ((VI)->may_trap_p)



/* Indexed by INSN_LUID, the collection of all data associated with
   a single instruction that is in the stream.  */
struct _sel_insn_data
{
  /* Every insn in the stream has an associated vinsn.  This is used
     to reduce memory consumption and give use the fact that many insns
     (all of them at the moment) don't change through the scheduler.

     Much of the information, that reflect information about the insn itself
     (e.g. not about where it stands in CFG) is contained in the VINSN_ID
     field of this home VI.  */
  expr_def _expr;

  /* If (WS_LEVEL == GLOBAL_LEVEL) then AV is empty.  */
  int ws_level;

  int seqno;

  /* An INSN_LUID bit is set when deps analysis result is already known.  */
  bitmap analyzed_deps;

  /* An INSN_LUID bit is set when a hard dep was found, not set when 
     no dependence is found.  This is meaningful only when the analyzed_deps
     bitmap has its bit set.  */
  bitmap found_deps;

  /* Insn is an ASM.  */
  bool asm_p;

  /* This field is initialized at the beginning of scheduling and is used
     to handle sched group instructions.  If it is non-null, then it points
     to the instruction, which should be forced to schedule next.  Such
     instructions are unique.  */
  insn_t sched_next;

  /* Cycle at which insn was scheduled.  It is greater than zero if insn was
     scheduled.
     This is used for bundling.  */
  int sched_cycle;

  /* True when an insn is scheduled after we've determined that a stall is
     required.
     This is used when emulating the Haifa scheduler for bundling.  */
  BOOL_BITFIELD after_stall_p : 1;

  /* Speculations that are being checked by this insn.  */
  ds_t spec_checked_ds;
};

typedef struct _sel_insn_data sel_insn_data_def;
typedef sel_insn_data_def *sel_insn_data_t;

DEF_VEC_O (sel_insn_data_def);
DEF_VEC_ALLOC_O (sel_insn_data_def, heap);
extern VEC (sel_insn_data_def, heap) *s_i_d;

/* Accessor macros for s_i_d.  */
#define SID(INSN) (VEC_index (sel_insn_data_def, s_i_d,	INSN_LUID (INSN)))

extern sel_insn_data_def insn_sid (insn_t);

#define INSN_ASM_P(INSN) (SID (INSN)->asm_p)
#define INSN_SCHED_NEXT(INSN) (SID (INSN)->sched_next)
#define INSN_ANALYZED_DEPS(INSN) (SID (INSN)->analyzed_deps)
#define INSN_FOUND_DEPS(INSN) (SID (INSN)->found_deps) 

#define INSN_EXPR(INSN) (&SID (INSN)->_expr)
#define INSN_VINSN(INSN) (RHS_VINSN (INSN_EXPR (INSN)))
#define INSN_TYPE(INSN) (VINSN_TYPE (INSN_VINSN (INSN)))
#define INSN_SIMPLEJUMP_P(INSN) (INSN_TYPE (INSN) == PC)
#define INSN_LHS(INSN) (VINSN_LHS (INSN_VINSN (INSN)))
#define INSN_RHS(INSN) (VINSN_RHS (INSN_VINSN (INSN)))
#define INSN_REG_SETS(INSN) (VINSN_REG_SETS (INSN_VINSN (INSN)))
#define INSN_REG_USES(INSN) (VINSN_REG_USES (INSN_VINSN (INSN)))
#define INSN_SCHED_TIMES(INSN) (EXPR_SCHED_TIMES (INSN_EXPR (INSN)))
#define INSN_SEQNO(INSN) (SID (INSN)->seqno)
#define INSN_AFTER_STALL_P(INSN) (SID (INSN)->after_stall_p)
#define INSN_SCHED_CYCLE(INSN) (SID (INSN)->sched_cycle)
#define INSN_SPEC_CHECKED_DS(INSN) (SID (INSN)->spec_checked_ds)

/* A global level shows whether an insn is valid or not.  */
extern int global_level;

#define INSN_WS_LEVEL(INSN) (SID (INSN)->ws_level)

extern av_set_t get_av_set (insn_t);
extern int get_av_level (insn_t);

#define AV_SET(INSN) (get_av_set (INSN))
#define AV_LEVEL(INSN) (get_av_level (INSN))
#define AV_SET_VALID_P(INSN) (AV_LEVEL (INSN) == global_level)

/* A list of fences currently in the works.  */
extern flist_t fences;

extern void sel_register_rtl_hooks (void);
extern void sel_unregister_rtl_hooks (void);

/* A NOP pattern used as a placeholder for real insns.  */
extern rtx nop_pattern;

/* An insn that 'contained' in EXIT block.  */
extern rtx exit_insn;

/* Return true is INSN is a local NOP.  The nop is local in the sense that
   it was emitted by the scheduler as a temporary insn and will soon be
   deleted.  These nops are identified by their pattern.  */
#define INSN_NOP_P(INSN) (PATTERN (INSN) == nop_pattern)

/* Return true if INSN is linked into instruction stream.
   NB: It is impossible for INSN to have one field null and the other not
   null: gcc_assert ((PREV_INSN (INSN) == NULL_RTX)
   == (NEXT_INSN (INSN) == NULL_RTX)) is valid.  */
#define INSN_IN_STREAM_P(INSN) (PREV_INSN (INSN) && NEXT_INSN (INSN))

/* Return true in INSN is in current fence.  */
#define IN_CURRENT_FENCE_P(INSN) (flist_lookup (fences, INSN) != NULL)

/* Marks loop as being considered for pipelining.  */
#define MARK_LOOP_FOR_PIPELINING(LOOP) ((LOOP)->aux = (void *)(size_t)(1))
#define LOOP_MARKED_FOR_PIPELINING_P(LOOP) ((size_t)((LOOP)->aux))

/* Saved loop preheader to transfer when scheduling the loop.  */
#define LOOP_PREHEADER_BLOCKS(LOOP) ((size_t)((LOOP)->aux) == 1 \
                                     ? NULL \
                                     : ((VEC(basic_block, heap) *) (LOOP)->aux))
#define SET_LOOP_PREHEADER_BLOCKS(LOOP,BLOCKS) ((LOOP)->aux = BLOCKS)

/* When false, only notes may be added.  */
extern bool can_add_real_insns_p;




/* Types used when initializing insn data.  */

enum insn_init_what { INSN_INIT_WHAT_INSN, INSN_INIT_WHAT_INSN_RTX };

/* Provide a separate luid for the insn.  */
#define INSN_INIT_TODO_LUID (1)

/* Initialize s_s_i_d.  */
#define INSN_INIT_TODO_SSID (2)

/* Initialize data for simplejump.  */
#define INSN_INIT_TODO_SIMPLEJUMP (4)

/* A container to hold information about insn initialization.  */
struct _insn_init
{
  /* What is being initialized.  */
  enum insn_init_what what;

  /* This is a set of INSN_INIT_TODO_* flags.  */
  int todo;
};
extern struct _insn_init insn_init;


/* A variable to track which part of rtx we are scanning in
   sched-deps.c: sched_analyze_insn ().  */
enum _deps_where
  {
    DEPS_IN_INSN,
    DEPS_IN_LHS,
    DEPS_IN_RHS,
    DEPS_IN_NOWHERE
  };
typedef enum _deps_where deps_where_t;


/* Per basic block data for the whole CFG.  */
struct _sel_global_bb_info
{
  /* For each bb header this field contains a set of live registers.
     For all other insns this field has a NULL.
     We also need to know LV sets for the instructions, that are immediatly
     after the border of the region.  */
  regset lv_set;

  /* Status of LV_SET.
     true - block has usable LV_SET.
     false - block's LV_SET should be recomputed.  */
  bool lv_set_valid_p;
};

typedef struct _sel_global_bb_info sel_global_bb_info_def;
typedef sel_global_bb_info_def *sel_global_bb_info_t;

DEF_VEC_O (sel_global_bb_info_def);
DEF_VEC_ALLOC_O (sel_global_bb_info_def, heap);

/* Per basic block data.  This array is indexed by basic block index.  */
extern VEC (sel_global_bb_info_def, heap) *sel_global_bb_info;

extern void sel_extend_global_bb_info (void);
extern void sel_finish_global_bb_info (void);

/* Get data for BB.  */
#define SEL_GLOBAL_BB_INFO(BB)					\
  (VEC_index (sel_global_bb_info_def, sel_global_bb_info, (BB)->index))

/* Access macros.  */
#define BB_LV_SET(BB) (SEL_GLOBAL_BB_INFO (BB)->lv_set)
#define BB_LV_SET_VALID_P(BB) (SEL_GLOBAL_BB_INFO (BB)->lv_set_valid_p)

/* Per basic block data for the region.  */
struct _sel_region_bb_info
{
  /* This insn stream is constructed in such a way that it should be
     traversed by PREV_INSN field - (*not* NEXT_INSN).  */
  rtx note_list;

  /* Cached availability set at the beginning of a block.
     See also AV_LEVEL () for conditions when this av_set can be used.  */
  av_set_t av_set;

  /* If (AV_LEVEL == GLOBAL_LEVEL) then AV is valid.  */
  int av_level;
};

typedef struct _sel_region_bb_info sel_region_bb_info_def;
typedef sel_region_bb_info_def *sel_region_bb_info_t;

DEF_VEC_O (sel_region_bb_info_def);
DEF_VEC_ALLOC_O (sel_region_bb_info_def, heap);

/* Per basic block data.  This array is indexed by basic block index.  */
extern VEC (sel_region_bb_info_def, heap) *sel_region_bb_info;

/* Get data for BB.  */
#define SEL_REGION_BB_INFO(BB) (VEC_index (sel_region_bb_info_def,	\
					   sel_region_bb_info, (BB)->index))

/* Get BB's note_list.
   A note_list is a list of various notes that was scattered across BB
   before scheduling, and will be appended at the beginning of BB after
   scheduling is finished.  */
#define BB_NOTE_LIST(BB) (SEL_REGION_BB_INFO (BB)->note_list)

#define BB_AV_SET(BB) (SEL_REGION_BB_INFO (BB)->av_set)
#define BB_AV_LEVEL(BB) (SEL_REGION_BB_INFO (BB)->av_level)
#define BB_AV_SET_VALID_P(BB) (BB_AV_LEVEL (BB) == global_level)

/* Used in bb_in_ebb_p.  */
extern bitmap_head *forced_ebb_heads;

/* The loop nest being pipelined.  */
extern struct loop *current_loop_nest;

/* Saves pipelined blocks.  Bitmap is indexed by bb->index.  */
extern sbitmap bbs_pipelined;



extern bool enable_moveup_set_path_p;
extern bool enable_schedule_as_rhs_p;
extern bool pipelining_p;
extern bool bookkeeping_p;

/* Functions that are used in sel-sched.c.  */

/* List functions.  */
extern ilist_t ilist_copy (ilist_t);
extern ilist_t ilist_invert (ilist_t);
extern void blist_add (blist_t *, insn_t, ilist_t, deps_t);
extern void blist_remove (blist_t *);
extern void flist_tail_init (flist_tail_t);
extern void flist_add (flist_t *, insn_t, state_t, deps_t, void *, 
                       insn_t, insn_t, int, int, bool, bool);

extern fence_t flist_lookup (flist_t, insn_t);
extern void flist_clear (flist_t *);
extern void def_list_add (def_list_t *, insn_t, bool, bool);

/* Target context functions.  */
extern tc_t create_target_context (bool);
extern void set_target_context (tc_t);
extern void reset_target_context (tc_t, bool);

/* Deps context functions.  */
extern void advance_deps_context (deps_t, insn_t);

/* Fences functions.  */
extern void init_fences (insn_t);
extern void new_fences_add (flist_tail_t, insn_t, state_t, deps_t, void *, rtx,
                       rtx, int, int, bool, bool);
extern void new_fences_add_clean (flist_tail_t, insn_t, fence_t);
extern void new_fences_add_dirty (flist_tail_t, insn_t, fence_t);

/* Pool functions.  */
extern regset get_regset_from_pool (void);
extern regset get_clear_regset_from_pool (void);
extern void return_regset_to_pool (regset);
extern void free_regset_pool (void);

extern insn_t get_nop_from_pool (insn_t);
extern void return_nop_to_pool (insn_t);
extern void free_nop_pool (void);

/* Vinsns functions.  */
extern bool vinsn_separable_p (vinsn_t);
extern bool vinsn_cond_branch_p (vinsn_t);
extern void recompute_vinsn_lhs_rhs (vinsn_t);
extern int sel_vinsn_cost (vinsn_t);
extern insn_t sel_gen_insn_from_rtx_after (rtx, expr_t, int, insn_t);
extern insn_t sel_gen_recovery_insn_from_rtx_after (rtx, expr_t, int, insn_t);
extern insn_t sel_gen_insn_from_expr_after (expr_t, int, insn_t);

/* RHS functions.  */
extern bool vinsns_correlate_as_rhses_p (vinsn_t, vinsn_t);
extern void copy_expr (expr_t, expr_t);
extern void copy_expr_onside (expr_t, expr_t);
extern void merge_expr_data (expr_t, expr_t);
extern void merge_expr (expr_t, expr_t);
extern void clear_expr (expr_t);
extern int find_in_hash_vect (VEC(unsigned, heap) *, unsigned);
extern void insert_in_hash_vect (VEC(unsigned, heap) **, unsigned);

/* Av set functions.  */
extern void av_set_add (av_set_t *, rhs_t);
extern void av_set_iter_remove (av_set_iterator *);
extern rhs_t av_set_lookup (av_set_t, vinsn_t);
extern rhs_t av_set_lookup_other_equiv_rhs (av_set_t, vinsn_t);
extern bool av_set_is_in_p (av_set_t, vinsn_t);
extern av_set_t av_set_copy (av_set_t);
extern void av_set_union_and_clear (av_set_t *, av_set_t *);
extern void av_set_clear (av_set_t *);
extern void av_set_leave_one (av_set_t *);
extern rhs_t av_set_element (av_set_t, int);
extern void av_set_substract_cond_branches (av_set_t *);
extern void av_set_intersect (av_set_t *, av_set_t);

extern void sel_save_haifa_priorities (void);

extern void sel_init_global_and_expr (bb_vec_t);
extern void sel_finish_global_and_expr (void);

/* Dependence analysis functions.  */
extern void sel_clear_has_dependence (void);
extern ds_t has_dependence_p (rhs_t, insn_t, ds_t **);

extern bool tick_check_p (rhs_t, deps_t, fence_t);

/* Functions to work with insns.  */
extern bool lhs_of_insn_equals_to_dest_p (insn_t, rtx);
extern bool insn_rtx_valid (rtx);
extern bool insn_eligible_for_subst_p (insn_t);
extern void get_dest_and_mode (rtx, rtx *, enum machine_mode *);

extern void sel_init_new_insns (void);
extern void sel_finish_new_insns (void);

extern bool bookkeeping_can_be_created_if_moved_through_p (insn_t);
extern insn_t copy_insn_out_of_stream (vinsn_t);
extern insn_t copy_insn_and_insert_before (insn_t, insn_t);
extern void sel_remove_insn (insn_t);
extern int vinsn_dfa_cost (vinsn_t, fence_t);
extern bool bb_header_p (insn_t);
extern void sel_init_invalid_data_sets (insn_t);

/* Basic block and CFG functions.  */

extern insn_t sel_bb_head (basic_block);
extern bool sel_bb_head_p (insn_t);
extern insn_t sel_bb_end (basic_block);
extern bool sel_bb_end_p (insn_t);

extern bool sel_bb_empty_p (basic_block);

extern bool in_current_region_p (basic_block);

extern void sel_init_bbs (bb_vec_t, basic_block);
extern void sel_finish_bbs (void);

extern int cfg_succs_n (insn_t, int);
extern bool sel_insn_has_single_succ_p (insn_t, int);
extern void cfg_succs_1 (insn_t, int, insn_t **, int *);
extern void cfg_succs (insn_t, insn_t **, int *);
extern insn_t cfg_succ_1 (insn_t, int);
extern insn_t cfg_succ (insn_t);
extern bool sel_num_cfg_preds_gt_1 (insn_t);

extern bool is_ineligible_successor (insn_t, ilist_t);

extern bool bb_ends_ebb_p (basic_block);
extern bool in_same_ebb_p (insn_t, insn_t);

extern void free_bb_note_pool (void);

extern basic_block sel_create_basic_block_before (basic_block);
extern void sel_remove_empty_bb (basic_block, bool, bool);
extern basic_block sel_split_edge (edge);
extern basic_block sel_create_recovery_block (insn_t);
extern void sel_merge_blocks (basic_block, basic_block);
extern void sel_redirect_edge_and_branch (edge, basic_block);
extern void sel_redirect_edge_and_branch_force (edge, basic_block);
extern void pipeline_outer_loops (void);
extern void pipeline_outer_loops_init (void);
extern void pipeline_outer_loops_finish (void);
extern void sel_sched_region (int);
extern void sel_find_rgns (void);
extern loop_p get_loop_nest_for_rgn (unsigned int);
extern bool considered_for_pipelining_p (struct loop *);
extern void make_region_from_loop_preheader (VEC(basic_block, heap) **);
extern void sel_add_loop_preheader (void);
extern bool sel_is_loop_preheader_p (basic_block);
extern void clear_outdated_rtx_info (basic_block);

extern void sel_register_cfg_hooks (void);
extern void sel_unregister_cfg_hooks (void);

/* Expression transformation routines.  */
extern rtx create_insn_rtx_from_pattern (rtx, rtx);
extern vinsn_t create_vinsn_from_insn_rtx (rtx);
extern rtx create_copy_of_insn_rtx (rtx);
extern void change_vinsn_in_expr (expr_t, vinsn_t);

/* Various initialization functions.  */
extern void init_lv_sets (void);
extern void free_lv_sets (void);
extern void setup_nop_and_exit_insns (void);
extern void free_nop_and_exit_insns (void);
extern void setup_nop_vinsn (void);
extern void free_nop_vinsn (void);
extern void sel_setup_common_sched_info (void);
extern void sel_setup_sched_infos (void);


/* Successor iterator backend.  */
typedef struct
{
  bool bb_end;
  edge e1;
  edge e2;
  edge_iterator ei;
  basic_block bb;
  int flags;

  /* If skip to loop exits, save here information about loop exits.  */
  int cur_exit;
  VEC (edge, heap) *loop_exits;
} succ_iterator;


/* True when BB is a header of the inner loop.  */
static bool
inner_loop_header_p (basic_block bb)
{
  struct loop *inner_loop; 

  if (!flag_sel_sched_pipelining_outer_loops
      || !current_loop_nest)
    return false;

  if (bb == EXIT_BLOCK_PTR)
    return false;

  inner_loop = bb->loop_father;
  if (inner_loop == current_loop_nest)
    return false;

  /* If successor belongs to another loop.  */
  if (bb == inner_loop->header
      && flow_bb_inside_loop_p (current_loop_nest, bb))
    {
      /* Could be '=' here because of wrong loop depths.  */
      gcc_assert (loop_depth (inner_loop) >= loop_depth (current_loop_nest));
      return true;
    }

  return false;  
}

/* Return exit edges of LOOP, filtering out edges with the same dest bb.  */

static inline VEC (edge, heap) *
get_loop_exit_edges_unique_dests (const struct loop *loop)
{
  VEC (edge, heap) *edges = NULL;
  struct loop_exit *exit;

  gcc_assert (loop->latch != EXIT_BLOCK_PTR
              && current_loops->state & LOOPS_HAVE_RECORDED_EXITS);

  for (exit = loop->exits->next; exit->e; exit = exit->next)
    {
      int i;
      edge e;
      bool was_dest = false;
      
      for (i = 0; VEC_iterate (edge, edges, i, e); i++)
        if (e->dest == exit->e->dest)
          {
            was_dest = true;
            break;
          }

      if (!was_dest)
        VEC_safe_push (edge, heap, edges, exit->e);
    }
  return edges;
}


/* Collect all loop exits recursively, skipping empty BBs between them.  
   E.g. if BB is a loop header which has several loop exits,
   traverse all of them and if any of them turns out to be another loop header
   (after skipping empty BBs), add its loop exits to the resulting vector 
   as well.  */
static inline VEC(edge, heap) *
get_all_loop_exits (basic_block bb)
{
  VEC(edge, heap) *exits = NULL;

  /* If bb is empty, and we're skipping to loop exits, then
     consider bb as a possible gate to the inner loop now.  */
  while (sel_bb_empty_p (bb) 
	 && in_current_region_p (bb))
    bb = single_succ (bb);

  /* And now check whether we should skip over inner loop.  */
  if (inner_loop_header_p (bb))
    {
      struct loop *this_loop;
      int i;
      edge e;

      {
	struct loop *pred_loop = NULL;

	for (this_loop = bb->loop_father;
	     this_loop && this_loop != current_loop_nest;
	     this_loop = loop_outer (this_loop))
	  pred_loop = this_loop;

	this_loop = pred_loop;

	gcc_assert (this_loop != NULL);
      }

      exits = get_loop_exit_edges_unique_dests (this_loop);

      /* Traverse all loop headers.  */
      for (i = 0; VEC_iterate (edge, exits, i, e); i++)
	if (in_current_region_p (e->dest))
	  {
	    VEC(edge, heap) *next_exits = get_all_loop_exits (e->dest);
  
	    if (next_exits)
	      {
		int j;
		edge ne;
  
		/* Add all loop exits for the current edge into the
		   resulting vector.  */
		for (j = 0; VEC_iterate (edge, next_exits, j, ne); j++)
		  VEC_safe_push (edge, heap, exits, ne);
  
		/* Remove the original edge.  */
		VEC_ordered_remove (edge, exits, i);

		/*  Decrease the loop counter so we won't skip anything.  */
		i--;
		continue;
	      }
	  }
	else
	  {
	    gcc_assert (!inner_loop_header_p (e->dest));
	  }
    }

  return exits;
}


/* Flags to pass to cfg_succs () and FOR_EACH_SUCC ().
   NB: Any successor will fall into exactly one category.   */

/* Include normal successors.  */
#define SUCCS_NORMAL (1)
/* Include back-edge successors.  */
#define SUCCS_BACK (2)
/* Include successors that are outside of the current region.  */
#define SUCCS_OUT (4)
/* When pipelining of the outer loops is enabled, skip innermost loops 
   to their exits.  */
#define SUCCS_SKIP_TO_LOOP_EXITS (8)

/* Include all successors.  */
#define SUCCS_ALL (SUCCS_NORMAL | SUCCS_BACK | SUCCS_OUT)

/* We need to return a succ_iterator to avoid 'unitialized' warning
   during bootstrap.  */
static inline succ_iterator
_succ_iter_start (insn_t *succp, insn_t insn, int flags)
{
  succ_iterator i;

  basic_block bb = BLOCK_FOR_INSN (insn);

  gcc_assert (INSN_P (insn) || NOTE_INSN_BASIC_BLOCK_P (insn));

  i.flags = flags;

  /* Avoid 'uninitialized' warning.  */
  *succp = NULL;
  i.e1 = NULL;
  i.e2 = NULL;
  i.bb = bb;
  i.cur_exit = -1;
  i.loop_exits = NULL;

  if (bb != EXIT_BLOCK_PTR && BB_END (bb) != insn)
    {
      i.bb_end = false;

      /* Avoid 'uninitialized' warning.  */
      i.ei.index = 0;
      i.ei.container = NULL;
    }
  else
    {
      i.ei = ei_start (bb->succs);
      i.bb_end = true;
    }

  return i;
}

static inline bool
_succ_iter_cond (succ_iterator *ip, rtx *succp, rtx insn,
                 bool check (edge, basic_block, int, edge *))
{
  if (!ip->bb_end)
    {
      /* When we're in a middle of a basic block, return
         the next insn immediately, but only when SUCCS_NORMAL is set.  */
      if (*succp != NULL || (ip->flags & SUCCS_NORMAL) == 0)
        return false;

      *succp = NEXT_INSN (insn);
      return true;
    }
  else
    {
      while (1) 
        {
          edge e_tmp = NULL;

          /* First, try loop exits, if we have them.  */
          if (ip->loop_exits)
            {
              do
                {
                  VEC_iterate (edge, ip->loop_exits, ip->cur_exit, e_tmp);
                  ip->cur_exit++;
                }
	      while (e_tmp && !check (e_tmp, ip->bb, ip->flags, &ip->e2));
              
              if (!e_tmp)
                VEC_free (edge, heap, ip->loop_exits);
            }

          /* If we have found a successor, then great.  */
          if (e_tmp)
            {
              ip->e1 = e_tmp;
              break;
            }

          /* If not, then try the next edge.  */
          while (ei_cond (ip->ei, &(ip->e1)))
            {
              basic_block bb = ip->e1->dest;

              /* Consider bb as a possible loop header.  */
              if ((ip->flags & SUCCS_SKIP_TO_LOOP_EXITS)
                  && flag_sel_sched_pipelining_outer_loops
		  && (!in_current_region_p (bb) 
		      || BLOCK_TO_BB (ip->bb->index) 
			 < BLOCK_TO_BB (bb->index)))
                {
		  /* Get all loop exits recursively.  */
		  ip->loop_exits = get_all_loop_exits (bb);

		  if (ip->loop_exits)
		    {
  		      ip->cur_exit = 0;
		      /* Move the iterator now, because we won't do 
			 succ_iter_next until loop exits will end.  */
		      ei_next (&(ip->ei));
		      break;
		    }
                }

              /* bb is not a loop header, check as usual.  */
              if (check (ip->e1, ip->bb, ip->flags, &ip->e2))
                break;

              ei_next (&(ip->ei));
            }

          /* If loop_exits are non null, we have found an inner loop;
	     do one more iteration to fetch an edge from these exits.  */
          if (ip->loop_exits)
            continue;

          /* Otherwise, we've found an edge in a usual way.  Break now.  */
          break;
        }

      if (ip->e1)
	{
	  basic_block bb = ip->e2->dest;

	  if (bb == EXIT_BLOCK_PTR)
	    *succp = exit_insn;
	  else
	    {
              *succp = sel_bb_head (bb);

              gcc_assert (ip->flags != SUCCS_NORMAL
                          || *succp == NEXT_INSN (bb_note (bb)));

	      gcc_assert (BLOCK_FOR_INSN (*succp) == bb);
	    }

	  return true;
	}
      else
	return false;
    }
}

static inline void
_succ_iter_next (succ_iterator *ip)
{
  gcc_assert (!ip->e2 || ip->e1);

  if (ip->bb_end && ip->e1 && !ip->loop_exits)
    ei_next (&(ip->ei));
}

/* Returns true when E1 is an eligible successor edge, possibly skipping
   empty blocks.  When E2P is not null, the resulting edge is written there.
   FLAGS are used to specify whether back edges and out-of-region edges
   should be considered.  */
static inline bool
_eligible_successor_edge_p (edge e1, basic_block real_pred, int flags, edge *e2p)
{
  edge e2 = e1;
  basic_block bb;
  bool src_outside_rgn = !in_current_region_p (e1->src);

  gcc_assert (flags != 0);

  if (src_outside_rgn)
    {
      /* Any successor of the block that is outside current region is
         ineligible, except when we're skipping to loop exits.  */
      gcc_assert (flags & (SUCCS_OUT | SUCCS_SKIP_TO_LOOP_EXITS));

      if (flags & SUCCS_OUT)
	return false;
    }

  bb = e2->dest;

  /* Skip empty blocks, but be careful not to leave the region.  */
  while (1)
    {
      if (!sel_bb_empty_p (bb))
        break;
        
      if (!in_current_region_p (bb) 
          && !(flags & SUCCS_OUT))
        return false;

      e2 = EDGE_SUCC (bb, 0);
      bb = e2->dest;
    }
  
  if (e2p)
    *e2p = e2;

  if (in_current_region_p (bb))
    {
      /* BLOCK_TO_BB sets topological order of the region here.  
         It is important to use REAL_PRED here as we may well have 
         e1->src outside current region, when skipping to loop exits.  */
      bool succeeds_in_top_order = (BLOCK_TO_BB (real_pred->index)
				    < BLOCK_TO_BB (bb->index));

      /* We are advancing forward in the region, as usual.  */
      if (succeeds_in_top_order)
        {
          /* We are skipping to loop exits here.  */
          gcc_assert (!src_outside_rgn
                      || flag_sel_sched_pipelining_outer_loops);

          return !!(flags & SUCCS_NORMAL);
        }

      /* This is a back edge.  During pipelining we ignore back edges, 
         but only when it leads to the same loop.  It can lead to the header
         of the outer loop, which will also be the preheader of 
         the current loop.  */
      if (pipelining_p
           && (!flag_sel_sched_pipelining_outer_loops
               || e1->src->loop_father == bb->loop_father))
        return !!(flags & SUCCS_NORMAL);

      /* A back edge should be requested explicitly.  */
      return !!(flags & SUCCS_BACK);
    }

  return !!(flags & SUCCS_OUT);
}

#define FOR_EACH_SUCC_1(SUCC, ITER, INSN, FLAGS) \
  for ((ITER) = _succ_iter_start (&(SUCC), (INSN), (FLAGS)); \
       _succ_iter_cond (&(ITER), &(SUCC), (INSN), _eligible_successor_edge_p);\
       _succ_iter_next (&(ITER)))

#define FOR_EACH_SUCC(SUCC, ITER, INSN) \
FOR_EACH_SUCC_1 (SUCC, ITER, INSN, SUCCS_NORMAL)

extern regset sel_all_regs;

/* Return the next block of BB not running into inconsistencies.  */
static inline basic_block
bb_next_bb (basic_block bb)
{
  switch (EDGE_COUNT (bb->succs))
    {
    case 0:
      return bb->next_bb;

    case 1: 
      return single_succ (bb);

    case 2:
      return FALLTHRU_EDGE (bb)->dest;
      
    default:
      return bb->next_bb;
    }

  gcc_unreachable ();
}

#endif /* GCC_SEL_SCHED_IR_H */








