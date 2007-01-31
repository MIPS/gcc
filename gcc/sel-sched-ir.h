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
#include "sched-int.h"
#include "sched-rgn.h"

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

/* Expression macros -- to be removed.  */
#define expr_equal_p(A, B) (rtx_equal_p (A, B))
#define expr_copy(EXPR) (copy_rtx (EXPR))

/* Right hand side information.  */
struct _rhs
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
};
/* Right hand side.  */
typedef struct _rhs *rhs_t;

#define RHS_VINSN(RHS) ((RHS)->vinsn)
#define RHS_SPEC(RHS) ((RHS)->spec)
#define RHS_INSN(RHS) (VINSN_INSN (RHS_VINSN (RHS)))
#define RHS_PAT(RHS) (PATTERN (RHS_INSN (RHS)))
#define RHS_PRIORITY(RHS) ((RHS)->priority)
/* FIXME: rename it!!!  */
#define RHS_SCHEDULE_AS_RHS(RHS) (VINSN_SEPARABLE (RHS_VINSN (RHS)))
#define RHS_HAS_RHS(RHS) (VINSN_HAS_RHS (RHS_VINSN (RHS)))


/* Insn definition for list of original insns in find_used_regs.  */
struct _def
{
  insn_t orig_insn;
  bool crosses_call;
};
typedef struct _def *def_t;


/* Availability sets arae sets of expressions we're scheduling.  */
typedef _list_t av_set_t;
#define _AV_SET_RHS(L) (&(L)->u.rhs)
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
    struct _rhs rhs;
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
      *rhsp = _AV_SET_RHS (av);
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
  /* Type of the insn.  Can be INSN, CALL_INSN, JUMP_INSN, ... something
     else and SET.  If it is SET, then insn can be cloned.  */
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
};

#define IDATA_TYPE(ID) ((ID)->type)
#define IDATA_LHS(ID) ((ID)->lhs)
#define IDATA_RHS(ID) ((ID)->rhs)
#define IDATA_REG_SETS(ID) ((ID)->reg_sets)
#define IDATA_REG_USES(ID) ((ID)->reg_uses)
#define IDATA_SIMPLEJUMP_P(ID) ((ID)->simplejump_p)

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
  insn_t insn;

  /* Its description.  */
  idata_t id;

  /* Time of schedule.  It is greater than zero if insn was scheduled.  */
  int sched_cycle;

  /* Number of times the insn was scheduled.  */
  int sched_times;

  /* Smart pointer counter.  */
  int count;

  /* Whether we will separate insn and schedule it as RHS.  */
  bool separable;
};


#define VINSN_ID(VI) ((VI)->id)
#define VINSN_TYPE(VI) (IDATA_TYPE (VINSN_ID (VI)))
#define VINSN_INSN(VI) ((VI)->insn)
#define VINSN_LHS(VI) (IDATA_LHS (VINSN_ID (VI)))
#define VINSN_RHS(VI) (IDATA_RHS (VINSN_ID (VI)))
#define VINSN_REG_SETS(VI) (IDATA_REG_SETS (VINSN_ID (VI)))
#define VINSN_REG_USES(VI) (IDATA_REG_USES (VINSN_ID (VI)))
#define VINSN_SCHED_CYCLE(VI) ((VI)->sched_cycle)
#define VINSN_SCHED_TIMES(VI) ((VI)->sched_times)
#define VINSN_COUNT(VI) ((VI)->count)
#define VINSN_HAS_RHS(VI) (!!VINSN_RHS (VI))
#define VINSN_SEPARABLE(VI) ((VI)->separable)
#define VINSN_PATTERN(VI) (PATTERN (VINSN_INSN (VI)))
#define VINSN_UNIQUE_P(VI) (VINSN_TYPE (VI) != SET)



/* Indexed by INSN_UID, the collection of all data associated with
   a single instruction that is in the stream.
   Though this data is indexed by INSN_UID, it should be indexed by
   INSN_LUID.  */
struct sel_insn_data
{
  /* Every insn in the stream has an associated vinsn.  This is used
     to reduce memory consumption and give use the fact that many insns
     (all of them at the moment) don't change through the scheduler.

     Much of the information, that reflect information about the insn itself
     (e.g. not about where it stands in CFG) is contained in the VINSN_ID
     field of this home VI.  */
  vinsn_t vi;

  int seqno;
  int av_level;
  int ws_level;
  av_set_t av;

  /* This field is initialized at the beginning of scheduling and is used
     to handle sched group instructions.  If it is non-null, then it points
     to the instruction, which should be forced to schedule next.  Such
     instructions are unique (i.e. can't be dublicated) as INSN_ASM_P is
     forced for them.  */
  insn_t sched_next;

  /* Insns that simply redirect control flow should not have any dependencies.
     sched-deps.c , though, might consider them as producers or consumers of
     certain registers.  To avoid that we handle dependency for simple jumps
     ourselves.  */
  BOOL_BITFIELD simplejump_p : 1;

  /* ??? This should be called unique_p as it might be set not for asms
     only.  It simply means that insn can't be cloned.  Such insns always
     have only one vinsn associated - INSN_VI field above.  */
  BOOL_BITFIELD asm_p : 1;

  /* True when an insn is scheduled after we've determined that a stall is required.
     This is used when emulating the Haifa scheduler for bundling.  */
  BOOL_BITFIELD after_stall_p : 1;

  /* Cycle at which insn was scheduled.  This is used for bundling.  */
  int final_sched_cycle;
};

extern struct sel_insn_data *s_i_d;
/* A global level shows whether an insn is valid or not.  */
extern int global_level;
/* A list of fences currently in the works.  */
extern flist_t fences;

/* Accessor macros for s_i_d.  */
#define SID(INSN) (s_i_d[INSN_UID (INSN)])

#define INSN_SEQNO(INSN) (SID (INSN).seqno)
#define AV_LEVEL(INSN)	(SID (INSN).av_level)
#define AV_SET(INSN)	(SID (INSN).av)
#define INSN_ASM_P(INSN) (SID (INSN).asm_p)

#define INSN_VI(INSN) (SID (INSN).vi)
#define INSN_ID(INSN) (VINSN_ID (INSN_VI (INSN)))
#define INSN_TYPE(INSN) (IDATA_TYPE (INSN_ID (INSN)))
#define INSN_LHS(INSN) (IDATA_LHS (INSN_ID (INSN)))
#define INSN_RHS(INSN) (IDATA_RHS (INSN_ID (INSN)))
#define INSN_REG_SETS(INSN) (IDATA_REG_SETS (INSN_ID (INSN)))
#define INSN_REG_USES(INSN) (IDATA_REG_USES (INSN_ID (INSN)))

#define INSN_SCHED_CYCLE(INSN) (VINSN_SCHED_CYCLE (INSN_VI (INSN)))
#define INSN_FINAL_SCHED_CYCLE(INSN) (SID (INSN).final_sched_cycle)

#define INSN_SCHED_NEXT(INSN) (SID (INSN).sched_next)
#define INSN_SIMPLEJUMP_P(INSN) (SID (INSN).simplejump_p)
#define INSN_AFTER_STALL_P(INSN) (SID (INSN).after_stall_p)

#define INSN_AV_VALID_P(INSN) (AV_LEVEL (INSN) == global_level)
#define INSN_UNIQUE_P(INSN) (INSN_TYPE (INSN) != SET)

/* Points to liveness sets.  */
extern regset * lvs;

/* Access macro.  */
#define LV_SET(INSN) (lvs[INSN_UID (INSN)])
/* !!! Replace all occurencies with (LV_SET (INSN) != NULL).  */
#define LV_SET_VALID_P(INSN) (lvs[INSN_UID (INSN)] != NULL)

/* Size of the per insn data structures.  */
extern int sel_max_uid;

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

/* When false, only notes may be added.  */
extern bool can_add_real_insns_p;

extern const struct rtl_hooks sel_rtl_hooks;
extern basic_block (*old_create_basic_block) (void *, void *, basic_block);



/* Types used when initializing insn data.  */

/* !!! Think about this.  It would be better to have separate sets of hooks
   to initialize insns and vinsns.  */
enum insn_init_what { INSN_INIT_WHAT_INSN, INSN_INIT_WHAT_VINSN };

/* !!! Think about this.  It would be better to have separate sets of hooks
   to initialize (v)insns now or later.  */
enum insn_init_how { INSN_INIT_HOW_NOW, INSN_INIT_HOW_DEFERRED };

/* Do not initialize anything.  */
#define INSN_INIT_TODO_NOTHING (0)
/* Provide a separate luid for the insn.  */
#define INSN_INIT_TODO_LUID (1)
/* Initialize LHS.  */
#define INSN_INIT_TODO_LHS (2)
/* Initialize RHS.  */
#define INSN_INIT_TODO_RHS (4)
/* Initialize INSN_VI () field in s_i_d.  */
#define INSN_INIT_TODO_VINSN (8)
/* Initialize LV_SET for insn.  */
#define INSN_INIT_TODO_LV_SET (16)
/* Initialize all Insn Data.  */
#define INSN_INIT_TODO_INIT_ID (INSN_INIT_TODO_LHS | INSN_INIT_TODO_RHS \
				| INSN_INIT_TODO_VINSN)
/* Initialize fundamental instruction flags like SCHED_GROUP.  This is being
   run at the scheduler init.  */
#define INSN_INIT_TODO_GLOBAL (32)
/* Prepare for dependence analysis.  */
#define INSN_INIT_TODO_PREPARE_DEP (64)
/* Run dependence analysis.  */
#define INSN_INIT_TODO_HAS_DEP (128)
/* Provide a seqno for the insn.  */
#define INSN_INIT_TODO_SEQNO (256)
/* Init everything in s_i_d.  */
#define INSN_INIT_TODO_SID (512 | INSN_INIT_TODO_INIT_ID \
			    | INSN_INIT_TODO_SEQNO)
/* Init luid and s_i_d.  */
#define INSN_INIT_TODO_ALL (INSN_INIT_TODO_LUID | INSN_INIT_TODO_SID)
/* Move LV_SET to the insn if it is being added to the bb header.  */
#define INSN_INIT_TODO_MOVE_LV_SET_IF_BB_HEADER (1024)


/* A container to hold information about insn initialization.  */
struct _insn_init
{
  /* What is being initialized.  */
  enum insn_init_what what;

  /* How this is being initialized.  */
  enum insn_init_how how;

  /* This is a set of INSN_INIT_TODO_* flags.  */
  int todo;

  /* Insn Data of the insn being initialized.
     !!! Move it somewhere else.  */
  idata_t id;

  /* Data for global dependency analysis (to initialize CANT_MOVE and
     SCHED_GROUP_P).  */
  struct
  {
    /* Current insn.  */
    insn_t insn;

    /* Previous insn.  */
    insn_t prev_insn;

    /* Deps context.  */
    struct deps deps;
  } global;

  /* Data to support deferred instruction initialization.  */
  struct insn_init_how_deferred_def
  {
    /* Array of pending insns.  */
    insn_t *insns;

    /* Number of elements in the above array.  */
    int n;

    /* Size of the above array.  */
    int size;
  } how_deferred;
};
extern struct _insn_init insn_init;


/* A variable to track which part of rtx we are scanning in
   sched-deps.c: sched_analyze_insn ().  */
enum deps_where_t
  {
    DEPS_IN_INSN,
    DEPS_IN_LHS,
    DEPS_IN_RHS,
    DEPS_IN_NOWHERE
  };

/* Is SEL_DEPS_HAS_DEP_P[DEPS_IN_X] is true, then X has a dependence.
   X is from { INSN, LHS, RHS }.  */
extern bool sel_deps_has_dep_p[];


/* Per basic block data.  */
struct sel_bb_info_def
{
  /* This insn stream is constructed in such a way that it should be
     traversed by PREV_INSN field - (*not* NEXT_INSN).  */
  rtx note_list;
};
typedef struct sel_bb_info_def *sel_bb_info_t;

/* Per basic block data.  This array is indexed by basic block index.  */
extern sel_bb_info_t sel_bb_info;

/* Get data for BB.  */
#define SEL_BB_INFO(BB) (&sel_bb_info[(BB)->index])

/* Get BB's note_list.
   A note_list is a list of various notes that was scattered across BB
   before scheduling, and will be appended at the beginning of BB after
   scheduling is finished.  */
#define BB_NOTE_LIST(BB) (SEL_BB_INFO (BB)->note_list)

/* Used in bb_in_ebb_p.  */
extern bitmap_head *forced_ebb_heads;

/* Used in a sanity check in generate_bookkeeping_insn.  */
extern basic_block last_added_block;


extern bool enable_moveup_set_path_p;
extern bool enable_schedule_as_rhs_p;
extern bool pipelining_p;
extern bool bookkeeping_p;
/* !!! Possibly remove: UNIQUE_RHSES are always used and UNIQUE_VINSNS are
   never used.
   Flags to pass to av_set_copy ().  */
enum { UNIQUE_RHSES = 1, UNIQUE_VINSNS };

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
extern void def_list_add (def_list_t *, insn_t, bool);

/* Target context functions.  */
extern tc_t create_target_context (bool);
extern void set_target_context (tc_t);
extern void reset_target_context (tc_t, bool);

/* Deps context functions.  */
extern void advance_deps_context (deps_t, insn_t);

/* Fences functions.  */
extern void init_fences (basic_block);
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

/* Vinsns functions.  */
extern bool vinsn_separable_p (vinsn_t);
extern void vinsn_attach (vinsn_t);
extern void vinsn_detach (vinsn_t);
extern bool vinsn_cond_branch_p (vinsn_t);
extern void recompute_vinsn_lhs_rhs (vinsn_t);

/* RHS functions.  */
extern bool rhs_equal_p (rhs_t, rhs_t);
extern void rhs_init (rhs_t, vinsn_t, int, int);
extern void rhs_copy (rhs_t, rhs_t);
extern void rhs_clear (rhs_t);

/* Av set functions.  */
extern void av_set_iter_remove (av_set_iterator *);
extern rhs_t av_set_lookup_rhs (av_set_t, rhs_t);
extern bool av_set_is_in_p (av_set_t, rhs_t);
extern rhs_t av_set_add_vinsn (av_set_t *, vinsn_t, int, int);
extern av_set_t av_set_copy (av_set_t, int);
extern void av_set_union_and_clear (av_set_t *, av_set_t *);
extern void av_set_clear (av_set_t *);
extern void av_set_leave_one (av_set_t *);
extern rhs_t av_set_element (av_set_t, int);
extern rhs_t av_set_lookup_insn (av_set_t, insn_t);
extern void av_set_substract_cond_branches (av_set_t *);
extern void av_set_intersect (av_set_t *, av_set_t);
extern void av_set_add_insn (av_set_t *, insn_t);

/* Dependence analysis functions.  */
extern bool has_dependence_p (rhs_t, insn_t);
extern bool tick_check_p (vinsn_t, deps_t, fence_t);

/* Functions to work with insns.  */
extern bool lhs_equals_reg_p (insn_t, rtx);
extern bool bb_header_p (insn_t);
extern bool insn_valid_p (insn_t);
extern bool insn_eligible_for_subst_p (insn_t);
extern void get_dest_and_mode (rtx, rtx *, enum machine_mode *);
extern bool bookkeeping_can_be_created_if_moved_through_p (insn_t);
extern insn_t copy_insn_out_of_stream (vinsn_t);
extern insn_t copy_insn_and_insert_before (insn_t, insn_t);
extern void sched_sel_remove_insn (insn_t);
extern void transfer_data_sets (insn_t, insn_t);
extern int dfa_cost (insn_t, fence_t);
extern void release_lv_set_for_insn (rtx);
extern bool bb_header_p (insn_t);


/* Basic block and CFG functions.  */
extern insn_t bb_head (basic_block);
extern bool in_current_region_p (basic_block);
extern bool bb_empty_p (basic_block);
extern int cfg_succs_n (insn_t, int);
extern void cfg_succs_1 (insn_t, int, insn_t **, int *);
extern void cfg_succs (insn_t, insn_t **, int *);
extern insn_t cfg_succ_1 (insn_t, int);
extern insn_t cfg_succ (insn_t);
extern bool num_preds_gt_1 (insn_t);
extern bool bb_ends_ebb_p (basic_block);
extern bool in_same_ebb_p (insn_t, insn_t);
extern bool is_ineligible_successor (insn_t, ilist_t);
extern void sel_add_or_remove_bb_1 (basic_block, bool);
extern void sel_add_or_remove_bb (basic_block, bool);
extern void sel_remove_empty_bb (basic_block, bool);
extern basic_block sel_split_block (basic_block);
extern basic_block sel_redirect_edge_force (edge, basic_block);
extern basic_block sel_create_basic_block (void *, void *, basic_block);
extern basic_block sel_create_basic_block_before (basic_block);

/* Various initialization functions.  */
extern void init_lv_sets (void);
extern void free_lv_sets (void);
extern void setup_nop_and_exit_insns (void);
extern void free_exit_insn_data (void);
extern void setup_sched_and_deps_infos (void);


/* Successor iterator backend.  */
typedef struct
{
  bool bb_end;
  edge e1;
  edge e2;
  edge_iterator ei;
  int flags;
} succ_iterator;

/* Flags to pass to cfg_succs () and FOR_EACH_SUCC ().
   NB: Any successor will fall into exactly one category.   */

/* Include normal successors.  */
#define SUCCS_NORMAL (1)
/* Include back-edge successors.  */
#define SUCCS_BACK (2)
/* Include successors that are outside of the current region.  */
#define SUCCS_OUT (4)
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

  /* Avoid 'uninitialized' warning.  */
  *succp = NULL;
  i.e1 = NULL;
  i.e2 = NULL;

  return i;
}

static inline bool
_succ_iter_cond (succ_iterator *ip, rtx *succp, rtx insn,
                 bool check (edge, int, edge *))
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
      while (ei_cond (ip->ei, &(ip->e1))
	     && !check (ip->e1, ip->flags, &ip->e2))
	ei_next (&(ip->ei));

      if (ip->e1)
	{
	  basic_block bb = ip->e2->dest;

	  if (bb == EXIT_BLOCK_PTR)
	    *succp = exit_insn;
	  else
	    {
	      *succp = next_nonnote_insn (bb_note (bb));

	      gcc_assert (ip->flags != SUCCS_NORMAL
			  || *succp == NEXT_INSN (bb_note (bb)));
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

  if (ip->bb_end && ip->e1)
    ei_next (&(ip->ei));
}

/* Returns true when E1 is an eligible successor edge, possibly skipping
   empty blocks.  When E2P is not null, the resulting edge is written there.
   FLAGS are used to specify whether back edges and out-of-region edges
   should be considered.  */
static inline bool
_eligible_successor_edge_p (edge e1, int flags, edge *e2p)
{
  edge e2 = e1;
  basic_block bb;

  gcc_assert (flags != 0);

  if (!in_current_region_p (e1->src))
    /* Any successor of the block that is outside current region is
       ineligible.
       Possibly this is impossible.  */
    {
      gcc_assert (flags & SUCCS_OUT);

      return false;
    }

  /* Skip empty blocks.  */
  do
    {
      bb = e2->dest;

      if (!bb_empty_p (bb))
	break;

      e2 = EDGE_SUCC (bb, 0);
    }
  while (1);

  if (e2p)
    *e2p = e2;

  if (in_current_region_p (bb))
    /* This is either normal or back edge.  */
    {
      if (/* During pipelining we ignore back edges.  */
	  pipelining_p
	  /* BLOCK_TO_BB set topological order of the region.  */
	  || BLOCK_TO_BB (e1->src->index) < BLOCK_TO_BB (bb->index))
	return !!(flags & SUCCS_NORMAL);
      else
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

#endif /* GCC_SEL_SCHED_IR_H */








