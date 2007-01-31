/* Instruction scheduling pass.
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


/* FIXME: check whether we need all these headers, and check the makefile.  */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "regs.h"
#include "function.h"
#include "flags.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "except.h"
#include "toplev.h"
#include "recog.h"
#include "cfglayout.h"
#include "params.h"
#include "target.h"
#include "timevar.h"
#include "tree-pass.h"
#include "sched-rgn.h"
#include "sched-int.h"
#include "sched-deps.h"
#include "cselib.h"
#include "ggc.h"
#include "tree.h"
#include "vec.h"
#include "langhooks.h"
#include "rtlhooks-def.h"

#ifdef INSN_SCHEDULING
#include "sel-sched-ir.h"
/* We don't have to use it except for sel_print_insn.  */
#include "sel-sched-dump.h"

/* A structure used to hold various parameters of insn initialization.  */
struct _insn_init insn_init;

/* Per insn data.  This array is indexed by INSN_UID, but could be by INSN_LUID.  */
struct sel_insn_data *s_i_d = NULL;

/* An array holding bb info.  */
sel_bb_info_t sel_bb_info = NULL;

/* The basic block that already has been processed by the sched_data_update (),
   but hasn't been in sel_add_or_remove_bb () yet.  */
basic_block last_added_block = NULL;

/* A maximum UID that is used as a size to per-insn arrays.  */
int sel_max_uid = 0;

/* For each bb header this array contains a set of live registers.
   For all other insns this array has a NULLs.

   NB: All the fields, gathered in the s_i_d are initialized only for
   insns that are in the current region and, therefore, can be indexed by
   LUID.  Except for this one.  We also need to know LV sets for the
   instructions, that are immediatly after the border of the region.  */
regset *lvs = NULL;

/* Size of LVS array.  */
static int lvs_size = 0;

/* A variable to track which part of rtx we are scanning in
   sched-deps.c: sched_analyze_insn ().  */
static enum deps_where_t deps_where = DEPS_IN_NOWHERE;

/* Is SEL_DEPS_HAS_DEP_P[DEPS_IN_X] is true, then X has a dependence.
   X is from { INSN, LHS, RHS }.  */
bool sel_deps_has_dep_p[DEPS_IN_NOWHERE];

/* A deps context which is used to determine if two insns are dependent.  */
static struct deps _sel_deps_di, *sel_deps_di = &_sel_deps_di;

/* Current producer and consumer insns for analyzing dependences.  */
static insn_t prod_of_dep, con_of_dep;

/* A placeholder for saving deps info structure for switching it.  */
static struct sched_deps_info_def *saved_deps_info = NULL;

/* This variable is used to ensure that no insns will be emitted by
   outer-world functions like redirect_edge_and_branch ().  */
static bool can_add_insns_p = true;

/* The same as the previous flag except that notes are allowed 
   to be emitted.  
   FIXME: avoid this dependency between files.  */
bool can_add_real_insns_p = true;

/* Redefine RTL hooks so we can catch the moment of creating an insn.  */
static void sel_rtl_insn_added (insn_t);
#undef RTL_HOOKS_INSN_ADDED
#define RTL_HOOKS_INSN_ADDED sel_rtl_insn_added
const struct rtl_hooks sel_rtl_hooks = RTL_HOOKS_INITIALIZER;


/* A regset pool structure.  */
static struct
{
  regset *v;
  int n;
  int s;

  /* In VV we accumulate all generated regsets so that, when destucting the
     pool, we can compare it with V and check that every regset was returned
     back to pool.  */
  regset *vv;
  int nn;
  int ss;

  int diff;
} regset_pool = { NULL, 0, 0, NULL, 0, 0, 0 };

/* This represents the nop pool.  */
static struct
{
  insn_t *v;
  int n;
  int s;  
} nop_pool = { NULL, 0, 0 };

/* A NOP pattern used to emit placeholder insns.  */
rtx nop_pattern = NULL_RTX;
/* A special instruction that resides in EXIT_BLOCK.
   EXIT_INSN is successor of the insns that lead to EXIT_BLOCK.  */
rtx exit_insn = NULL_RTX;


/* Forward static declarations.  */
static void fence_init (fence_t, insn_t, state_t, deps_t, void *,
                        rtx, rtx, int, int, bool, bool);
static void fence_clear (fence_t);
static int get_vinsn_type_for_insn (insn_t, bool);

static void save_deps_info (void);
static void restore_deps_info (void);
static void deps_init_id (idata_t, insn_t);

static void cfg_preds (basic_block, insn_t **, int *);




/* Various list functions.  */

/* Copy an instruction list L.  */
ilist_t
ilist_copy (ilist_t l)
{
  ilist_t head = NULL, *tailp = &head;

  while (l)
    {
      ilist_add (tailp, ILIST_INSN (l));
      tailp = &ILIST_NEXT (*tailp);
      l = ILIST_NEXT (l);
    }

  return head;
}

/* Invert an instruction list L.  */
ilist_t
ilist_invert (ilist_t l)
{
  ilist_t res = NULL;

  while (l)
    {
      ilist_add (&res, ILIST_INSN (l));
      l = ILIST_NEXT (l);
    }

  return res;
}

/* Add a new boundary to the LP list with parameters TO, PTR, and DC.  */
void
blist_add (blist_t *lp, insn_t to, ilist_t ptr, deps_t dc)
{
  bnd_t bnd;

  _list_add (lp);
  bnd = BLIST_BND (*lp);

  BND_TO (bnd) = to;
  BND_PTR (bnd) = ptr;
  BND_AV (bnd) = NULL;
  BND_AV1 (bnd) = NULL;
  BND_DC (bnd) = dc;
}

/* Remove the list note pointed to by LP.  */
void
blist_remove (blist_t *lp)
{
  bnd_t b = BLIST_BND (*lp);

  av_set_clear (&BND_AV (b));
  av_set_clear (&BND_AV1 (b));
  ilist_clear (&BND_PTR (b));

  _list_remove (lp);
}

/* Init a fence tail L.  */
void
flist_tail_init (flist_tail_t l)
{
  FLIST_TAIL_HEAD (l) = NULL;
  FLIST_TAIL_TAILP (l) = &FLIST_TAIL_HEAD (l);
}

/* Try to find fence corresponding to INSN in L.  */
fence_t
flist_lookup (flist_t l, insn_t insn)
{
  while (l)
    {
      if (FENCE_INSN (FLIST_FENCE (l)) == insn)
	return FLIST_FENCE (l);

      l = FLIST_NEXT (l);
    }

  return NULL;
}

/* Add new fence consisting of INSN and STATE to the list pointed to by LP.  */
void
flist_add (flist_t *lp, insn_t insn, state_t state, deps_t dc, void *tc, 
           insn_t last_scheduled_insn, insn_t sched_next, int cycle,
	   int cycle_issued_insns, bool starts_cycle_p, bool after_stall_p)
{
  _list_add (lp);
  fence_init (FLIST_FENCE (*lp), insn, state, dc, tc, last_scheduled_insn,
	      sched_next, cycle, cycle_issued_insns, starts_cycle_p,
	      after_stall_p);
}

/* Remove the head node of the list pointed to by LP.  */
static void
flist_remove (flist_t *lp)
{
  fence_clear (FLIST_FENCE (*lp));
  _list_remove (lp);
}

/* Clear the fence list pointed to by LP.  */
void
flist_clear (flist_t *lp)
{
  while (*lp)
    flist_remove (lp);
}

/* Add ORIGINAL_INSN the def list DL honoring CROSSES_CALL.  */
void
def_list_add (def_list_t *dl, insn_t original_insn, bool crosses_call)
{
  def_t d;
  _list_add (dl);
  d = DEF_LIST_DEF (*dl);

  d->orig_insn = original_insn;
  d->crosses_call = crosses_call;
}


/* Functions to work with target contexts.  */

/* Bulk target context.
   NB: It is convenient for debuging purposes to ensure that there are no
   uninitialized (null) target contexts.  */
static tc_t bulk_tc = (tc_t) 1;

/* Target hooks wrappers.
   Possibly it would be nice to provide some default implementations for
   them. */

/* Allocate a store for the target context.  */
static tc_t
alloc_target_context (void)
{
  return (targetm.sched.alloc_sched_context
	  ? targetm.sched.alloc_sched_context () : bulk_tc);
}

/* Init target context TC.
   If CLEAN_P is true, then make TC as it is beginning of the scheduler.
   Overwise, copy current backend context to TC.  */
static void
init_target_context (tc_t tc, bool clean_p)
{
  if (targetm.sched.init_sched_context)
    targetm.sched.init_sched_context (tc, clean_p);
}

/* Allocate and initialize a target context.  Meaning of CLEAN_P is the same as
   int init_target_context ().  */
tc_t
create_target_context (bool clean_p)
{
  tc_t tc = alloc_target_context ();

  init_target_context (tc, clean_p);
  return tc;
}

/* Copy TC to the current backend context.  */
void
set_target_context (tc_t tc)
{
  if (targetm.sched.set_sched_context)
    targetm.sched.set_sched_context (tc);
}

/* TC is about to be destroyed.  Free any internal data.  */
static void
clear_target_context (tc_t tc)
{
  if (targetm.sched.clear_sched_context)
    targetm.sched.clear_sched_context (tc);
}

/*  Clear and free it.  */
static void
delete_target_context (tc_t tc)
{
  clear_target_context (tc);

  if (targetm.sched.free_sched_context)
    targetm.sched.free_sched_context (tc);
}

/* Make a copy of FROM in TO.
   NB: May be this should be a hook.  */
static void
copy_target_context (tc_t to, tc_t from)
{
  tc_t tmp = create_target_context (false);

  set_target_context (from);
  init_target_context (to, false);

  set_target_context (tmp);
  delete_target_context (tmp);
}

/* Create a copy of TC.  */
static tc_t
create_copy_of_target_context (tc_t tc)
{
  tc_t copy = alloc_target_context ();

  copy_target_context (copy, tc);

  return copy;
}

/* Clear TC and initialize it according to CLEAN_P.  The meaning of CLEAN_P
   is the same as in init_target_context ().  */
void
reset_target_context (tc_t tc, bool clean_p)
{
  clear_target_context (tc);
  init_target_context (tc, clean_p);
}

/* Functions to work with dependence contexts. 
   Dc (aka deps context, aka deps_t, aka struct deps *) is short for dependence
   context.  It accumulates information about processed insns to decide if
   current insn is dependent on the processed ones.  */

/* Make a copy of FROM in TO.  */
static void
copy_deps_context (deps_t to, deps_t from)
{
  init_deps (to);
  deps_join (to, from);
}

/* Allocate store for dep context.  */
static deps_t
alloc_deps_context (void)
{
  return xmalloc (sizeof (struct deps));
}

/* Allocate and initialize dep context.  */
static deps_t
create_deps_context (void)
{
  deps_t dc = alloc_deps_context ();

  init_deps (dc);
  return dc;
}

/* Create a copy of FROM.  */
static deps_t
create_copy_of_deps_context (deps_t from)
{
  deps_t to = alloc_deps_context ();

  copy_deps_context (to, from);
  return to;
}

/* Clean up internal data of DC.  */
static void
clear_deps_context (deps_t dc)
{
  free_deps (dc);
}

/* Clear and free DC.  */
static void
delete_deps_context (deps_t dc)
{
  clear_deps_context (dc);
  free (dc);
}

/* Clear and init DC.  */
static void
reset_deps_context (deps_t dc)
{
  clear_deps_context (dc);
  init_deps (dc);
}

/* Process INSN and add its impact on DC.  */
void
advance_deps_context (deps_t dc, insn_t insn)
{
  insn_init.what = INSN_INIT_WHAT_INSN;
  insn_init.id = INSN_ID (insn);
  insn_init.todo = INSN_INIT_TODO_PREPARE_DEP;
  deps_analyze_insn (dc, insn);
}


/* Functions to work with DFA states.  */

/* Allocate store for a DFA state.  */
static state_t
state_alloc (void)
{
  return xmalloc (dfa_state_size);
}

/* Allocate and initialize DFA state.  */
static state_t
state_create (void)
{
  state_t state = state_alloc ();

  state_reset (state);
  return state;
}

/* Free DFA state.  */
static void
state_free (state_t state)
{
  free (state);
}

/* Make a copy of FROM in TO.  */
static void
state_copy (state_t to, state_t from)
{
  memcpy (to, from, dfa_state_size);
}

/* Create a copy of FROM.  */
static state_t
state_create_copy (state_t from)
{
  state_t to = state_alloc ();

  state_copy (to, from);
  return to;
}


/* Functions to work with fences.  */

/* Initialize the fence.  */
static void
fence_init (fence_t f, insn_t insn, state_t state, deps_t dc, void *tc,
	    rtx last_scheduled_insn, rtx sched_next, int cycle,
	    int cycle_issued_insns, bool starts_cycle_p, bool after_stall_p)
{
  FENCE_INSN (f) = insn;

  gcc_assert (state != NULL);
  FENCE_STATE (f) = state;

  FENCE_CYCLE (f) = cycle;
  FENCE_ISSUED_INSNS (f) = cycle_issued_insns;
  FENCE_STARTS_CYCLE_P (f) = starts_cycle_p;
  FENCE_AFTER_STALL_P (f) = after_stall_p;
  
  FENCE_BNDS (f) = NULL;
  FENCE_SCHEDULED (f) = false;
  FENCE_SCHEDULED_SOMETHING (f) = false;

  gcc_assert (dc != NULL);
  FENCE_DC (f) = dc;

  gcc_assert (tc != NULL || targetm.sched.alloc_sched_context == NULL);
  FENCE_TC (f) = tc;

  FENCE_LAST_SCHEDULED_INSN (f) = last_scheduled_insn;
  FENCE_SCHED_NEXT (f) = sched_next;
}

#if 0
/* Copy the FROM fence to TO.  */
static void
fence_copy (fence_t to, fence_t from)
{
  fence_init (to, FENCE_INSN (from), state_create_copy (FENCE_STATE (from)),
	      create_copy_of_deps_context (FENCE_DC (from)),
	      create_copy_of_target_context (FENCE_TC (from)),
	      FENCE_LAST_SCHEDULED_INSN (from), FENCE_SCHED_NEXT (from),
	      FENCE_CYCLE (from), FENCE_ISSUED_INSNS (from),
	      FENCE_STARTS_CYCLE_P (from), FENCE_AFTER_STALL_P (from));
}
#endif

/* Clear the fence.  */
static void
fence_clear (fence_t f)
{
  state_t s = FENCE_STATE (f);
  deps_t dc = FENCE_DC (f);
  void *tc = FENCE_TC (f);

  ilist_clear (&FENCE_BNDS (f));

  gcc_assert ((s != NULL && dc != NULL && tc != NULL)
	      || (s == NULL && dc == NULL && tc == NULL));

  if (s != NULL)
    free (s);

  if (dc != NULL)
    free_deps (dc);

  if (tc != NULL)
    delete_target_context (tc);
}

/* Init a list of fences with the head of BB.  */
void
init_fences (basic_block bb)
{
  flist_add (&fences, cfg_succ (bb_note (bb)),
	     state_create (),
	     create_deps_context () /* dc */,
	     create_target_context (true) /* tc */,
	     NULL_RTX /* last_scheduled_insn */, NULL_RTX /* sched_next */,
	     1 /* cycle */, 0 /* cycle_issued_insns */, 
	     1 /* starts_cycle_p */, 0 /* after_stall_p */);
}

/* Add a new fence to NEW_FENCES list, initializing it from all 
   other parameters.  */
void
new_fences_add (flist_tail_t new_fences, insn_t insn,
		state_t state, deps_t dc, void *tc, rtx last_scheduled_insn, 
		rtx sched_next, int cycle, int cycle_issued_insns, 
                bool starts_cycle_p, bool after_stall_p)
{
  fence_t f = flist_lookup (FLIST_TAIL_HEAD (new_fences), insn);

  if (!f)
    {
      flist_add (FLIST_TAIL_TAILP (new_fences), insn, state, dc, tc,
		 last_scheduled_insn, sched_next, cycle, cycle_issued_insns,
		 starts_cycle_p, after_stall_p);

      FLIST_TAIL_TAILP (new_fences)
	= &FLIST_NEXT (*FLIST_TAIL_TAILP (new_fences));
    }
  else
    /* Here we should somehow choose between two DFA states.
       Plain reset for now.  */
    {
      gcc_assert (bb_header_p (FENCE_INSN (f))
		  && !sched_next && !FENCE_SCHED_NEXT (f));

      state_reset (FENCE_STATE (f));
      state_free (state);

      reset_deps_context (FENCE_DC (f));
      delete_deps_context (dc);

      reset_target_context (FENCE_TC (f), true);
      delete_target_context (tc);

      if (cycle > FENCE_CYCLE (f))
        FENCE_CYCLE (f) = cycle;

      if (after_stall_p)
        FENCE_AFTER_STALL_P (f) = 1;

      FENCE_ISSUED_INSNS (f) = 0;
      FENCE_STARTS_CYCLE_P (f) = 1;
      FENCE_LAST_SCHEDULED_INSN (f) = NULL;
      FENCE_SCHED_NEXT (f) = NULL;
    }
}

/* Add a new fence to NEW_FENCES list and initialize most of its data 
   as a clean one.  */
void
new_fences_add_clean (flist_tail_t new_fences, insn_t succ, fence_t fence)
{
  new_fences_add (new_fences,
		  succ, state_create (), create_deps_context (),
		  create_target_context (true),
		  NULL_RTX, NULL_RTX, FENCE_CYCLE (fence) + 1,
		  0, 1, FENCE_AFTER_STALL_P (fence));
}

/* Add a new fence to NEW_FENCES list and initialize all of its data 
   from FENCE and SUCC.  */
void
new_fences_add_dirty (flist_tail_t new_fences, insn_t succ, fence_t fence)
{
  new_fences_add (new_fences,
		  succ, state_create_copy (FENCE_STATE (fence)),
		  create_copy_of_deps_context (FENCE_DC (fence)),
		  create_copy_of_target_context (FENCE_TC (fence)),
		  FENCE_LAST_SCHEDULED_INSN (fence), FENCE_SCHED_NEXT (fence),
		  FENCE_CYCLE (fence),
		  FENCE_ISSUED_INSNS (fence),
		  FENCE_STARTS_CYCLE_P (fence),
		  FENCE_AFTER_STALL_P (fence));
}


/* Functions to work with regset and nop pools.  */

regset
get_regset_from_pool (void)
{
  regset rs;

  if (regset_pool.n != 0)
    rs = regset_pool.v[--regset_pool.n];
  else
    /* We need to create the regset.  */
    {
      rs = ALLOC_REG_SET (&reg_obstack);

      if (regset_pool.nn == regset_pool.ss)
	regset_pool.vv = xrealloc (regset_pool.vv,
				   ((regset_pool.ss = 2 * regset_pool.ss + 1)
				    * sizeof (*regset_pool.vv)));

      regset_pool.vv[regset_pool.nn++] = rs;
    }

  regset_pool.diff++;

  return rs;
}

regset
get_clear_regset_from_pool (void)
{
  regset rs = get_regset_from_pool ();

  CLEAR_REG_SET (rs);
  return rs;
}

void
return_regset_to_pool (regset rs)
{
  regset_pool.diff--;

  if (regset_pool.n == regset_pool.s)
    regset_pool.v = xrealloc (regset_pool.v,
			      ((regset_pool.s = 2 * regset_pool.s + 1)
			       * sizeof (*regset_pool.v)));

  regset_pool.v[regset_pool.n++] = rs;
}

static int
cmp_v_in_regset_pool (const void *x, const void *xx)
{
  return *((const regset *) x) - *((const regset *) xx);
}

void
free_regset_pool (void)
{
  if (ENABLE_SEL_CHECKING)
    {
      regset *v = regset_pool.v;
      int i = 0;
      int n = regset_pool.n;

      regset *vv = regset_pool.vv;
      int ii = 0;
      int nn = regset_pool.nn;

      int diff = 0;

      gcc_assert (n <= nn);

      /* Sort both vectors so it will be possible to compare them.  */
      qsort (v, n, sizeof (*v), cmp_v_in_regset_pool);
      qsort (vv, nn, sizeof (*vv), cmp_v_in_regset_pool);

      while (ii < nn)
	{
	  if (v[i] == vv[ii])
	    i++;
	  else
	    /* VV[II] was lost.  */
	    diff++;

	  ii++;
	}

      gcc_assert (diff == regset_pool.diff);
    }

  /* If not true - we have a memory leak.  */
  gcc_assert (regset_pool.diff == 0);

  while (regset_pool.n)
    {
      --regset_pool.n;
      FREE_REG_SET (regset_pool.v[regset_pool.n]);
    }

  free (regset_pool.v);
  regset_pool.v = NULL;
  regset_pool.s = 0;
  
  free (regset_pool.vv);
  regset_pool.vv = NULL;
  regset_pool.nn = 0;
  regset_pool.ss = 0;

  regset_pool.diff = 0;
}

/* Functions to work with nop pools.  NOP insns are used as temporary 
   placeholders of the insns being scheduled to allow correct update of 
   the data sets.  When update is finished, NOPs are deleted.  */

/* Emit a nop before INSN, taking it from pool.  */
insn_t
get_nop_from_pool (insn_t insn)
{
  insn_t nop;

  if (nop_pool.n != 0)
    {
      nop = nop_pool.v[--nop_pool.n];
      insn_init.todo = INSN_INIT_TODO_NOTHING;
    }
  else
    {
      nop = nop_pattern;
      insn_init.todo = INSN_INIT_TODO_ALL & ~INSN_INIT_TODO_SEQNO;
    }

  insn_init.what = INSN_INIT_WHAT_INSN;
  nop = emit_insn_before (nop, insn);
  INSN_SEQNO (nop) = INSN_SEQNO (insn);

  vinsn_attach (INSN_VI (nop));

  return nop;
}

/* Remove NOP from the instruction stream and return it to the pool.  */
void
return_nop_to_pool (insn_t nop)
{
  sched_sel_remove_insn (nop);

  if (nop_pool.n == nop_pool.s)
    nop_pool.v = xrealloc (nop_pool.v, ((nop_pool.s = 2 * nop_pool.s + 1)
					* sizeof (*nop_pool.v)));

  nop_pool.v[nop_pool.n++] = nop;
}

/* Free the nop pool.  */
static void
free_nop_pool (void)
{
  while (nop_pool.n)
    vinsn_detach (INSN_VI (nop_pool.v[--nop_pool.n]));

  nop_pool.s = 0;
  free (nop_pool.v);
  nop_pool.v = NULL;
}


/* Functions to work with vinsns.  */
static bool
vinsn_equal_insn_p (vinsn_t vi, insn_t insn)
{
  if (VINSN_TYPE (vi) != get_vinsn_type_for_insn (insn, VINSN_UNIQUE_P (vi)))
    return false;

  return (VINSN_UNIQUE_P (vi)
	  ? VINSN_INSN (vi) == insn
	  : expr_equal_p (VINSN_PATTERN (vi), PATTERN (insn)));
}

static bool
vinsn_equal_p (vinsn_t vi1, vinsn_t vi2)
{
  if (VINSN_TYPE (vi1) != VINSN_TYPE (vi2))
    return false;

  return (VINSN_UNIQUE_P (vi1)
	  ? VINSN_INSN (vi1) == VINSN_INSN (vi2)
	  : expr_equal_p (VINSN_PATTERN (vi1), VINSN_PATTERN (vi2)));  
}

/* Returns whether we'll try to schedule VI's rhs and lhs separately.  */
bool
vinsn_separable_p (vinsn_t vi)
{
  if (!enable_schedule_as_rhs_p)
    return false;

  if (control_flow_insn_p (VINSN_INSN (vi)))
    return false;

  if (VINSN_UNIQUE_P (vi))
    return false;
  
  if (!VINSN_LHS (vi) || !VINSN_RHS (vi))
    return false;

  /* Do not schedule CONST and CONST_INT as rhs: no point to use reg, 
     where const can be used.  Moreover, scheduling const as rhs may lead
     to modes mismatch cause consts don't have modes but they could be merged
     from branches where the same const used in different modes.  */
  if (GET_CODE (VINSN_RHS (vi)) == CONST || GET_CODE (VINSN_RHS (vi)) == CONST_INT)
    return false;

  /* ??? Do not rename predicate registers to avoid ICEs in bundling.  */
  if (COMPARISON_P (VINSN_RHS (vi)))
      return false;

  /* Do not allow single REG to be an rhs.  */
  if (REG_P (VINSN_RHS (vi)))
    return false;

  /* See comment at find_used_regs_1 (*1) for explanation of this 
     restriction.  */
  /* FIXME: remove this later.  */
  if (MEM_P (VINSN_LHS (vi)))
    return false;

  /* This will filter all tricky things like ZERO_EXTRACT etc.
     For now we don't handle it.  */
  if (!REG_P (VINSN_LHS (vi)) && !MEM_P (VINSN_LHS (vi)))
    return false;

  /* Do not allow renaming of cheap insns.  See also PR #1.  */
  if (INSN_COST (VINSN_INSN (vi)) >= 0 && INSN_COST (VINSN_INSN (vi)) < 2)
    return false;

  return true;
}

/* Initialize vinsn VI for INSN.  Only for use from vinsn_create ().  */
static void
vinsn_init (vinsn_t vi, insn_t insn, bool unique_p)
{
  idata_t id = xcalloc (1, sizeof (*id));

  gcc_assert (insn_init.what != INSN_INIT_WHAT_INSN
	      || INSN_VI (insn) == NULL);

  VINSN_INSN (vi) = insn;
  VINSN_ID (vi) = id;

  IDATA_TYPE (id) = get_vinsn_type_for_insn (insn, unique_p);

  IDATA_REG_SETS (id) = get_clear_regset_from_pool ();
  IDATA_REG_USES (id) = get_clear_regset_from_pool ();

  if (!INSN_SIMPLEJUMP_P (insn))
    deps_init_id (id, insn);

  /* ??? We don't copy these fields when constructing the copy of vinsn.
     This can lead us to perfomance penalties or even to infinite loop or
     ICE.  */
  VINSN_SCHED_CYCLE (vi) = 0;
  VINSN_SCHED_TIMES (vi) = 0;

  VINSN_SEPARABLE (vi) = vinsn_separable_p (vi);
}

/* Indicate that VI has become the part of an rtx object.  */
void
vinsn_attach (vinsn_t vi)
{
  /* Assert that VI is not pending for deletion.  */
  gcc_assert (VINSN_INSN (vi));

  VINSN_COUNT (vi)++;
}

/* Create and init VI from the INSN.  Initialize VINSN_COUNT (VI) with COUNT 
   and use UNIQUE_P for determining the correct VINSN_TYPE (VI).  */
static vinsn_t
vinsn_create (insn_t insn, int count, bool unique_p)
{
  vinsn_t vi = xmalloc (sizeof (*vi));

  vinsn_init (vi, insn, unique_p);
  VINSN_COUNT (vi) = count;

  return vi;
}

/* Delete the VI vinsn and free its data.  */
static void
vinsn_delete (vinsn_t vi)
{
  insn_t insn = VINSN_INSN (vi);

  gcc_assert (VINSN_COUNT (vi) == 0);

  return_regset_to_pool (VINSN_REG_SETS (vi));
  return_regset_to_pool (VINSN_REG_USES (vi));

  free (VINSN_ID (vi));

  /* ??? This should be conditional on (INSN_HAS_LUID (insn)).  */
  AV_LEVEL (insn) = 0;
  av_set_clear (&AV_SET (insn));

  /* This insn should not be deleted as it may have shared parts.  */
  /*  if (!INSN_IN_STREAM_P (insn))  expr_clear (&insn); */

  free (vi);
}

/* Indicate that VI is no longer a part of some rtx object.  
   Remove VI if it is no longer needed.  */
void
vinsn_detach (vinsn_t vi)
{
  gcc_assert (VINSN_COUNT (vi) > 0);

  if (--VINSN_COUNT (vi) == 0)
    vinsn_delete (vi);
}

/* Returns TRUE if VI is a branch.  */
bool
vinsn_cond_branch_p (vinsn_t vi)
{
  insn_t insn;

  if (!VINSN_UNIQUE_P (vi))
    return false;

  insn = VINSN_INSN (vi);
  if (BB_END (BLOCK_FOR_INSN (insn)) != insn)
    return false;

  return control_flow_insn_p (insn);
}

/* This function is used after VINSN_INSN (RHS) has been altered, so
   VINSN_RHS and VINSN_LHS will become valid again.  */
void
recompute_vinsn_lhs_rhs (vinsn_t vi)
{
  if (VINSN_SEPARABLE (vi))
    {
      rtx pat = VINSN_PATTERN (vi);
      VINSN_LHS (vi) = SET_DEST (pat);
      VINSN_RHS (vi) = SET_SRC (pat);
    }

  VINSN_SEPARABLE (vi) = vinsn_separable_p (vi);
}

/* Functions to work with right-hand sides.  */

/* Return whether X and Y are equal rhs'es.
   Rhses should be compared as a trees, cause two rhses may become equal
   through different substitutions.  
   Rhses with different RHS_SCHEDULE_AS_RHS values are incomparable.  */
bool
rhs_equal_p (rhs_t x, rhs_t y)
{
  if (RHS_SCHEDULE_AS_RHS (x) != RHS_SCHEDULE_AS_RHS (y))
    {
#ifdef ENABLE_CHECKING
      /* Check for strange situations when comparing rhses like
	 RHS:(x+y) and WHOLE INSN:(z = x+y). */

      gcc_assert (!vinsn_equal_p (RHS_VINSN (x), RHS_VINSN (y)));
#endif
      return false;
    }

  if (RHS_SCHEDULE_AS_RHS (x)) 
    {
      /* Compare RHSes of VINSNs.  */
      gcc_assert (VINSN_RHS (RHS_VINSN (x)));
      gcc_assert (VINSN_RHS (RHS_VINSN (y)));

      return expr_equal_p (VINSN_RHS (RHS_VINSN (x)), 
			   VINSN_RHS (RHS_VINSN (y)));
    }
  else
    {
      /* Compare whole insns. */
      return vinsn_equal_p (RHS_VINSN (x), RHS_VINSN (y));
    }
}

/* Copy FROM to TO.  */
void
rhs_init (rhs_t rhs, vinsn_t vi, int spec, int priority)
{
  vinsn_attach (vi);
  RHS_VINSN (rhs) = vi;
  RHS_SPEC (rhs) = spec;
  RHS_PRIORITY (rhs) = priority;
}

/* Make a copy of the rhs FROM into the rhs TO.  */
void
rhs_copy (rhs_t to, rhs_t from)
{
  rhs_init (to, RHS_VINSN (from), RHS_SPEC (from), RHS_PRIORITY (from));
}

/* Merge bits of FROM rhs to TO rhs.  The two rhses should be equal.  */
static void
rhs_merge (rhs_t to, rhs_t from)
{
  gcc_assert (rhs_equal_p (to, from));

  /* For now, we just set the spec of resulting rhs to be minimum of the specs
     of merged rhses.  */
  if (RHS_SPEC (from) > RHS_SPEC (to))
    RHS_SPEC (to) = RHS_SPEC (from);

  if (RHS_PRIORITY (from) > RHS_PRIORITY (to))
    RHS_PRIORITY (to) = RHS_PRIORITY (from);
}

/* Clear the information of this RHS.  */
void
rhs_clear (rhs_t rhs)
{
  vinsn_detach (RHS_VINSN (rhs));
}

/* Returns whether R represents INSN (or it's right-hand side,
   if R has RHS_SCHEDULE_AS_RHS set to TRUE.  */
static bool
rhs_equals_insn_p (rhs_t r, insn_t insn)
{
  if (RHS_SCHEDULE_AS_RHS (r))
    {
      if (VINSN_SEPARABLE (INSN_VI (insn)))
	return expr_equal_p (VINSN_RHS (RHS_VINSN (r)), INSN_RHS (insn));
      else
	return false;
    }
  else
    {
      /* Just compare vinsns.  */
      return vinsn_equal_insn_p (RHS_VINSN (r), insn);
    }
}


/* Av set functions.  */

void
av_set_iter_remove (av_set_iterator *ip)
{
  rhs_clear (_AV_SET_RHS (*ip->lp));
  _list_iter_remove (ip);
}

/* Search for an rhs in SET, such that it's equivalent to SOUGHT_RHS in the
   sense of rhs_equal_p function. Return NULL if no such rhs is in SET was
   found.  */
rhs_t
av_set_lookup_rhs (av_set_t set, rhs_t sought_rhs)
{
  rhs_t r;
  av_set_iterator i;

  FOR_EACH_RHS (r, i, set)
    if (rhs_equal_p (r, sought_rhs))
      return r;

  return NULL;
}

bool
av_set_is_in_p (av_set_t set, rhs_t rhs)
{
  return av_set_lookup_rhs (set, rhs) != NULL;
}

rhs_t
av_set_add_vinsn (av_set_t *setp, vinsn_t vi, int spec, int priority)
{
  _list_add (setp);
  rhs_init (_AV_SET_RHS (*setp), vi, spec, priority);

  return _AV_SET_RHS (*setp);
}

av_set_t
av_set_copy (av_set_t set, int flags)
{
  rhs_t rhs;
  av_set_iterator i;
  av_set_t res = NULL;

  /* This flag is mandatory.  */
  gcc_assert (flags & UNIQUE_RHSES);
  gcc_assert (!(flags & UNIQUE_VINSNS));
  gcc_assert (!(flags & ~(UNIQUE_RHSES | UNIQUE_VINSNS)));

  FOR_EACH_RHS (rhs, i, set)
    {
      vinsn_t vi = RHS_VINSN (rhs);

      if (flags & UNIQUE_VINSNS)
	vi = vinsn_create (VINSN_INSN (vi), 0, VINSN_UNIQUE_P (vi));

      av_set_add_vinsn (&res, vi, RHS_SPEC (rhs), RHS_PRIORITY (rhs));
    }

  return res;
}

/* Makes set pointed to by TO to be the union of TO and FROM.  */
void
av_set_union_and_clear (av_set_t *top, av_set_t *fromp)
{
  rhs_t rhs1;
  av_set_iterator i;

  /* Delete from TOP all rhses, that present in FROMP.  */
  FOR_EACH_RHS_1 (rhs1, i, top)
    {
      rhs_t rhs2 = av_set_lookup_rhs (*fromp, rhs1);

      if (rhs2)
	{
          rhs_merge (rhs2, rhs1);
	  av_set_iter_remove (&i);
	}
    }

  /* Connect FROMP to the end of the TOP.  */
  *i.lp = *fromp;
}

void
av_set_clear (av_set_t *setp)
{
  rhs_t rhs;
  av_set_iterator i;

  FOR_EACH_RHS_1 (rhs, i, setp)
    av_set_iter_remove (&i);

  gcc_assert (*setp == NULL);
}

void
av_set_leave_one (av_set_t *setp)
{
  av_set_clear (&_AV_SET_NEXT (*setp));
}

rhs_t
av_set_element (av_set_t set, int n)
{
  rhs_t rhs;
  av_set_iterator i;

  FOR_EACH_RHS (rhs, i, set)
    if (n-- == 0)
      return rhs;

  gcc_unreachable ();
  return NULL;
}

/* Return RHS that corresponds to INSN.
   For those rhses that have RHS_SCHEDULE_AS_RHS attr set, only right-hand 
   sides are compared.  */
rhs_t
av_set_lookup_insn (av_set_t set, insn_t insn)
{
  rhs_t r;
  av_set_iterator i;

  FOR_EACH_RHS (r, i, set)
    if (rhs_equals_insn_p (r, insn))
      return r;

  return NULL;
}

/* Deletes all expressions from AVP that are conditional branches (IFs).  */
void
av_set_substract_cond_branches (av_set_t *avp)
{
  av_set_iterator i;
  rhs_t rhs;

  FOR_EACH_RHS_1 (rhs, i, avp)
    if (vinsn_cond_branch_p (RHS_VINSN (rhs)))
      av_set_iter_remove (&i);
}

/* Leave in AVP only those expressions, which are present in AV, and return it.  */
void
av_set_intersect (av_set_t *avp, av_set_t av)
{
  av_set_iterator i;
  rhs_t rhs;

  FOR_EACH_RHS_1 (rhs, i, avp)
    if (av_set_lookup_rhs (av, rhs) == NULL)
      av_set_iter_remove (&i);
}

/* Adds INSN to av_set AVP.  Helper function.
   It should be used in all places where insn is added to it's own av_set.  */
void
av_set_add_insn (av_set_t *avp, insn_t insn)
{
  rhs_t added = av_set_add_vinsn (avp, INSN_VI (insn), 0, INSN_PRIORITY (insn));

  if (enable_schedule_as_rhs_p)
    {
      /* If it's possible to extract rhs, set RHS_SCHEDULE_AS_RHS.
         We need this to be able to lift the same rhs from different
         branches and merge them.  */
      if (!RHS_SCHEDULE_AS_RHS (added) 
	  && vinsn_separable_p (RHS_VINSN (added)))
	    RHS_SCHEDULE_AS_RHS (added) = true;
    }
}



/* Dependence hooks to initialize insn data from the dependency analyzer.  */

/* Start initializing INSN data.  */
static void
deps_init_id_start_insn (insn_t insn)
{
  gcc_assert (deps_where == DEPS_IN_NOWHERE);

  IDATA_TYPE (insn_init.id) = get_vinsn_type_for_insn (insn, false);

  deps_where = DEPS_IN_INSN;
}

/* Finish initializing INSN data.  */
static void
deps_init_id_finish_insn (void)
{
  gcc_assert (deps_where == DEPS_IN_INSN);

  deps_where = DEPS_IN_NOWHERE;
}

/* Start initializing LHS data.  */
static void
deps_init_id_start_lhs (rtx lhs)
{
  gcc_assert (deps_where == DEPS_IN_INSN);

  gcc_assert (!IDATA_LHS (insn_init.id));

  if (IDATA_TYPE (insn_init.id) == SET)
    {
      IDATA_LHS (insn_init.id) = lhs;
      deps_where = DEPS_IN_LHS;
    }
}

/* Finish initializing LHS data.  */
static void
deps_init_id_finish_lhs (void)
{
  deps_where = DEPS_IN_INSN;
}

/* Start initializing RHS data.  */
static void
deps_init_id_start_rhs (rtx rhs)
{
  gcc_assert (deps_where == DEPS_IN_INSN);

  /* And there was no sel_deps_reset_to_insn ().  */
  if (IDATA_LHS (insn_init.id))
    {
      IDATA_RHS (insn_init.id) = rhs;
      deps_where = DEPS_IN_RHS;
    }
}

/* Finish initializing RHS data.  */
static void
deps_init_id_finish_rhs (void)
{
  gcc_assert (deps_where == DEPS_IN_RHS || deps_where == DEPS_IN_INSN);

  deps_where = DEPS_IN_INSN;
}

/* Reset deps data of currently analyzing insn.  */
static void
deps_init_id_reset_deps_to_insn (void)
{
  IDATA_LHS (insn_init.id) = NULL;
  IDATA_RHS (insn_init.id) = NULL;

  deps_where = DEPS_IN_INSN;
}

/* Note a set of REGNO.  */
static void
deps_init_id_note_reg_set (int regno)
{
  haifa_note_reg_set (regno);

  if (deps_where == DEPS_IN_RHS)
    deps_init_id_reset_deps_to_insn ();

  SET_REGNO_REG_SET (IDATA_REG_SETS (insn_init.id), regno);
}

/* Note a clobber of REGNO.  */
static void
deps_init_id_note_reg_clobber (int regno)
{
  haifa_note_reg_clobber (regno);

  if (deps_where == DEPS_IN_RHS)
    deps_init_id_reset_deps_to_insn ();

  SET_REGNO_REG_SET (IDATA_REG_SETS (insn_init.id), regno);
}

/* Note a use of REGNO.  */
static void
deps_init_id_note_reg_use (int regno)
{
  haifa_note_reg_use (regno);

  SET_REGNO_REG_SET (IDATA_REG_USES (insn_init.id), regno);
}

/* Note a dependence.  */
static void
deps_init_id_note_mem_dep (rtx mem ATTRIBUTE_UNUSED,
			   rtx pending_mem ATTRIBUTE_UNUSED,
			   insn_t pending_insn ATTRIBUTE_UNUSED,
			   ds_t ds ATTRIBUTE_UNUSED)
{
}

/* Note a memory dependence.  */
static void
deps_init_id_note_dep (insn_t pro ATTRIBUTE_UNUSED, ds_t ds ATTRIBUTE_UNUSED)
{
}

static struct sched_deps_info_def deps_init_id_info =
  {
    NULL,

    deps_init_id_start_insn,
    deps_init_id_finish_insn,
    NULL, /* start_x */
    NULL, /* finish_x */
    deps_init_id_start_lhs,
    deps_init_id_finish_lhs,
    deps_init_id_start_rhs,
    deps_init_id_finish_rhs,
    deps_init_id_note_reg_set,
    deps_init_id_note_reg_clobber,
    deps_init_id_note_reg_use,
    deps_init_id_note_mem_dep,
    deps_init_id_note_dep,

    0, 0, 0
  };

/* Initialize instruction data for INSN in ID.  */
static void
deps_init_id (idata_t id, insn_t insn)
{	 
  insn_init.id = id;

  save_deps_info ();
  sched_deps_info = &deps_init_id_info;

  init_deps (sel_deps_di);
  deps_analyze_insn (sel_deps_di, insn);
  free_deps (sel_deps_di);

  restore_deps_info ();
}



/* Implement hooks for dependence tracking of the scheduler.  */

/* ??? All these macros should be plain '==', not '&'.  
   And we need their names to be capitalized.  */
#define deps_todo_global_init_p (insn_init.todo & INSN_INIT_TODO_GLOBAL)
#define deps_todo_prepare_dep_p (insn_init.todo & INSN_INIT_TODO_PREPARE_DEP)
#define deps_todo_has_dep_p (insn_init.todo & INSN_INIT_TODO_HAS_DEP)

/* In the below hooks, we merely calculate whether or not a dependence 
   exists, and in what part of insn.  However, we will need more data 
   when we'll start caching dependence requests.  */

/* Start analyzing dependencies of INSN.  */
static void
sel_start_insn (insn_t insn)
{
  gcc_assert (deps_where == DEPS_IN_NOWHERE);

  if (deps_todo_has_dep_p)
    {
      int i;

      for (i = 0; i < DEPS_IN_NOWHERE; i++)
	sel_deps_has_dep_p[i] = false;
    }
  else if (deps_todo_global_init_p)
    insn_init.global.insn = insn;

  deps_where = DEPS_IN_INSN;
}

/* Finish analyzing dependencies of an insn.  */
static void
sel_finish_insn (void)
{
  gcc_assert (deps_where == DEPS_IN_INSN);

  if (deps_todo_global_init_p)
    {
      insn_t insn = insn_init.global.insn;

      if (SCHED_GROUP_P (insn))
	{
	  insn_t prev_insn = insn_init.global.prev_insn;

	  gcc_assert (CANT_MOVE (insn));

	  INSN_ASM_P (insn) = true;

	  if (prev_insn)
	    INSN_SCHED_NEXT (prev_insn) = insn;

	  insn_init.global.prev_insn = insn;
	}
      else
	{
	  if (CANT_MOVE (insn) || prologue_epilogue_contains (insn))
	    INSN_ASM_P (insn) = true;

	  insn_init.global.prev_insn = NULL_RTX;
	}
    }

  deps_where = DEPS_IN_NOWHERE;
}

/* Start analyzing dependencies of an rtx X, which is not a regular insn 
   or expression.  */
static void
sel_start_x (rtx x)
{
  if (deps_todo_global_init_p)
    {
      switch (GET_CODE (x))
	{
	case SCRATCH:
	case ASM_OPERANDS:
	  /* rtl.c: copy_rtx_1 () can't properly handle insns with SCRATCHes
	     and ASM_OPERANDS.  See emit-rtl.c: copy_insn ().  */
	  INSN_ASM_P (insn_init.global.insn) = true;
	default:
	  break;
	}
    }
}

/* Finish analyzing dependencies of an unknown rtx.  */
static void
sel_finish_x (void)
{
}

/* Start analyzing dependencies of LHS.  */
static void
sel_start_lhs (rtx lhs ATTRIBUTE_UNUSED)
{
  gcc_assert (deps_where == DEPS_IN_INSN);

  if (deps_todo_has_dep_p && IDATA_LHS (insn_init.id))
    deps_where = DEPS_IN_LHS;
}

/* Finish analyzing dependencies of an lhs.  */
static void
sel_finish_lhs (void)
{
  deps_where = DEPS_IN_INSN;
}

/* Start analyzing dependencies of RHS.  */
static void
sel_start_rhs (rtx rhs ATTRIBUTE_UNUSED)
{
  gcc_assert (deps_where == DEPS_IN_INSN);

  if (deps_todo_has_dep_p && IDATA_RHS (insn_init.id))
    deps_where = DEPS_IN_RHS;
}

/* Start analyzing dependencies of an rhs.  */
static void
sel_finish_rhs (void)
{
  gcc_assert (deps_where == DEPS_IN_RHS || deps_where == DEPS_IN_INSN);

  deps_where = DEPS_IN_INSN;
}

/* Note a set of REGNO.  */
static void
sel_deps_note_reg_set (int regno)
{
  if (deps_todo_prepare_dep_p) 
    haifa_note_reg_set (regno);
  else if (deps_todo_has_dep_p)
    {
      struct deps_reg *reg_last = &sel_deps_di->reg_last[regno];

      if ((reg_last->sets || reg_last->clobbers || reg_last->uses)
          && !sched_insns_conditions_mutex_p (prod_of_dep, con_of_dep))
	sel_deps_has_dep_p[deps_where] = true;
    }
}

/* Note a clobber of REGNO.  */
static void
sel_deps_note_reg_clobber (int regno)
{
  if (deps_todo_prepare_dep_p)
    haifa_note_reg_clobber (regno);
  else if (deps_todo_has_dep_p)
    {
      struct deps_reg *reg_last = &sel_deps_di->reg_last[regno];

      if ((reg_last->sets || reg_last->uses)
          && !sched_insns_conditions_mutex_p (prod_of_dep, con_of_dep))
	sel_deps_has_dep_p[deps_where] = true;
    }
}

/* Note a use of REGNO.  */
static void
sel_deps_note_reg_use (int regno)
{
  if (deps_todo_prepare_dep_p)
    haifa_note_reg_use (regno);
  else if (deps_todo_has_dep_p)
    {
      struct deps_reg *reg_last = &sel_deps_di->reg_last[regno];

      if ((reg_last->sets || reg_last->clobbers)
          && !sched_insns_conditions_mutex_p (prod_of_dep, con_of_dep))
	sel_deps_has_dep_p[deps_where] = true;
    }
}

/* Note a memory dependence.  */
static void
sel_note_mem_dep (rtx mem ATTRIBUTE_UNUSED, rtx pending_mem ATTRIBUTE_UNUSED,
                  insn_t pending_insn ATTRIBUTE_UNUSED,
                  ds_t ds ATTRIBUTE_UNUSED)
{
  if (deps_todo_has_dep_p 
      && !sched_insns_conditions_mutex_p (prod_of_dep, con_of_dep))
    sel_deps_has_dep_p[deps_where] = true;
}

/* Note a dependence.  */
static void
sel_note_dep (insn_t pro ATTRIBUTE_UNUSED, ds_t ds ATTRIBUTE_UNUSED)
{
  if (deps_todo_has_dep_p
      && !sched_insns_conditions_mutex_p (prod_of_dep, con_of_dep))
    sel_deps_has_dep_p[deps_where] = true;
}

static struct sched_deps_info_def sel_sched_deps_info =
  {
    NULL,

    sel_start_insn,
    sel_finish_insn,
    sel_start_x,
    sel_finish_x,
    sel_start_lhs,
    sel_finish_lhs,
    sel_start_rhs,
    sel_finish_rhs,
    sel_deps_note_reg_set,
    sel_deps_note_reg_clobber,
    sel_deps_note_reg_use,
    sel_note_mem_dep,
    sel_note_dep,

    0, 0, 0
  };


/* Functions to support dependence analysis and handling.  */

/* Save deps info to allow its switching.  */
static void
save_deps_info (void)
{
  gcc_assert (saved_deps_info == NULL);
  saved_deps_info = sched_deps_info;
}

/* Restore previously saved deps info structure.  */
static void
restore_deps_info (void)
{
  gcc_assert (saved_deps_info != NULL);
  sched_deps_info = saved_deps_info;
  saved_deps_info = NULL;
}

/* Return nonzero if RHS has is dependent upon PRED.  */
bool
has_dependence_p (rhs_t rhs, insn_t pred)
{
  vinsn_t vi = RHS_VINSN (rhs);
  insn_t insn = VINSN_INSN (vi);
  int i;

  if (INSN_SIMPLEJUMP_P (pred))
    /* Unconditional jump is just a transfer of control flow.
       Ignore it.  */
    return false;

  init_deps (sel_deps_di);
  /* Initialize global variables for sel_dep_* hooks.  */
  prod_of_dep = pred;
  con_of_dep = insn;

  /* Initialize empty dep context with information about PRED.  */
  advance_deps_context (sel_deps_di, pred);

  insn_init.id = VINSN_ID (vi);
  insn_init.todo = INSN_INIT_TODO_HAS_DEP;

  /* Now catch all dependencies that would be generated between PRED and
     INSN.  */
  deps_analyze_insn (sel_deps_di, insn);

  free_deps (sel_deps_di);

  for (i = 0; i < DEPS_IN_NOWHERE; i++)
    if (sel_deps_has_dep_p[i])
      return true;

  return false;
}


/* Dependence hooks implementation that checks dependence latency constraints 
   on the insns being scheduled.  The entry point for these routines is 
   tick_check_p predicate.  */ 

/* An insn we are currently checking.  */
static insn_t tick_check_insn;
/* A minimal cycle for its scheduling.  */
static int tick_check_cycle;
/* Whether we have seen a true dependence while checking.  */
static int tick_check_seen_true_dep;

/* An implementation of note_dep hook.  Update minimal scheduling cycle 
   for tick_check_insn given that it depends on PRO with status DS.  */
static void
tick_check_note_dep (insn_t pro, ds_t ds)
{
  insn_t con;
  enum reg_note dt;
  int tick;

  con = tick_check_insn;

  if (con != pro)
    {
      if (/* PROducer was removed from above due to pipelining.  See PR8.  */
	  !INSN_IN_STREAM_P (pro)
	  /* Or PROducer was originally on the next iteration regarding the
	     CONsumer.  */
	  || (VINSN_SCHED_TIMES (INSN_VI (pro))
	      - VINSN_SCHED_TIMES (INSN_VI (con))) > 1)
	/* Don't count this dependence.  */
	{
	  gcc_assert (pipelining_p);

	  return;
	}

      gcc_assert (INSN_SCHED_CYCLE (pro) > 0);
      
      dt = ds_to_dt (ds);
      if (dt == REG_DEP_TRUE)
        tick_check_seen_true_dep = 1;

      tick = INSN_SCHED_CYCLE (pro) + dep_cost (pro, dt, con);
      /* When there are several kinds of dependencies between pro and con,
         only REG_DEP_TRUE should be taken into account.  */
      if (tick > tick_check_cycle && (dt == REG_DEP_TRUE 
                                      || !tick_check_seen_true_dep))
	tick_check_cycle = tick;
    }
}

/* An implementation of note_mem_dep hook.  */
static void
tick_check_note_mem_dep (rtx mem1 ATTRIBUTE_UNUSED, rtx mem2 ATTRIBUTE_UNUSED,
			 insn_t pro, ds_t ds)
{
  tick_check_note_dep (pro, ds);
}

static struct sched_deps_info_def tick_check_deps_info =
  {
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    haifa_note_reg_set,
    haifa_note_reg_clobber,
    haifa_note_reg_use,
    tick_check_note_mem_dep,
    tick_check_note_dep,

    0, 0, 0
  };

/* Returns true when VI's insn can be scheduled on the current cycle of 
   FENCE.  That is, all data from possible producers in DC_ORIG is ready.  */
bool
tick_check_p (vinsn_t vi, deps_t dc_orig, fence_t fence)
{
  struct deps _dc, *dc = &_dc;

  /* Initialize variables.  */
  tick_check_insn = VINSN_INSN (vi);
  tick_check_cycle = 0;
  tick_check_seen_true_dep = 0;

  /* Switch hooks.  */
  save_deps_info ();
  sched_deps_info = &tick_check_deps_info;

  /* Calculate TICK_CHECK_CYCLE.  */
  copy_deps_context (dc, dc_orig);
  deps_analyze_insn (dc, tick_check_insn);
  free_deps (dc);

  /* Restore hooks.  */
  restore_deps_info ();

  return FENCE_CYCLE (fence) >= tick_check_cycle;
}



/* Functions to work with insns.  */

/* Returns true if LHS of INSN is a register and it's the same register as R2.  */
bool
lhs_equals_reg_p (insn_t insn, rtx reg)
{
  rtx lhs = INSN_LHS (insn);
  
  if (!lhs || !reg)
    return false;
  return REG_P (lhs) && (REGNO (lhs) == REGNO (reg));
}

/* Determine whether INSN could be cloned and return appropriate vinsn type.
   All clonable VINSNS have type SET.  When UNIQUE_P is true, always require 
   a unique vinsn.  */
static int
get_vinsn_type_for_insn (insn_t insn, bool unique_p)
{
  int code = GET_CODE (insn), unique_vinsn_code = -1;

  /* If UNIQUE_P, the insn is considered unique in any case.  */
  if (unique_p)
    return unique_vinsn_code;
  /* Only regular insns could be cloned.  */
  if (code != INSN)
    return unique_vinsn_code;
  /* ASM and exception handling insns are always unique.  */
  if (INSN_ASM_P (insn)
      || (flag_non_call_exceptions && can_throw_internal (insn)))
    return unique_vinsn_code;
  
  /* Everything else is considered clonable and handled like SET.  */
  return SET;
}

/* True when INSN is a basic block header.  */
bool
bb_header_p (insn_t insn)
{
  gcc_assert (insn && INSN_P (insn));

  return insn == exit_insn || NOTE_INSN_BASIC_BLOCK_P (PREV_INSN (insn));
}

/* Returns whether insn is valid in terms of target architecture.  */
bool
insn_valid_p (insn_t insn)
{
  /* Reset the INSN_CODE.  After register replacement it might have became
     a different insn.  */
  INSN_CODE (insn) = -1;
  if (recog_memoized (insn) >= 0)
    {
      extract_insn (insn);
      return constrain_operands (reload_completed) ? true : false;
    }
  else
    return false;
}

/* Returns whether INSN is eligible for substitution, i.e. it's a copy
   operation x := y, and RHS that is moved up through this insn should be
   substituted.  */
bool
insn_eligible_for_subst_p (insn_t insn)
{
  /* Since we've got INSN_LHS and INSN_RHS it should be the SET insn,
     and it's RHS is free of side effects (like AUTO_INC), 
     so we just need to make sure the INSN_RHS consists of only one simple
     REG rtx.  */

  if (INSN_RHS (insn) && INSN_LHS (insn))
    {
      if (REG_P (INSN_RHS (insn)) && REG_P (INSN_LHS (insn)))
	{
	  gcc_assert (GET_MODE (INSN_LHS (insn)) 
		      == GET_MODE (INSN_RHS (insn)));
	}
      if ((REG_P (INSN_RHS (insn)) 
	   && (REG_P (INSN_LHS (insn)) 
	       || GET_CODE (INSN_RHS (insn)) == CONST_INT)))
	  return true;             
    }
  return false;
}

/* An implementation of RTL_HOOKS_INSN_ADDED hook.  The hook is used for 
   initializing per-insn data structures when new insn is emitted.  */
static void
sel_rtl_insn_added (insn_t insn)
{
  gcc_assert (can_add_insns_p);

  if (/* We have added an instruction to the instruction stream.  */
      insn_init.what == INSN_INIT_WHAT_INSN)
    {
      if (insn_init.how == INSN_INIT_HOW_NOW)
	{
	  if ((insn_init.todo & INSN_INIT_TODO_LUID)
	      && INSN_NEED_LUID_P (insn))
	    sched_data_update (NULL, NULL, insn);
	}
      else
	/* Initialize a bit later because something (e.g. CFG) is not
	   consistent yet.  This clause is used to handle insntruction that are
	   being added by CFG manipulation routines.  These insns will be
	   initialized when sel_insn_deferred_init () is called.  */
	{
	  struct insn_init_how_deferred_def *p = &insn_init.how_deferred;

	  gcc_assert (insn_init.how == INSN_INIT_HOW_DEFERRED);

	  if (p->n == p->size)
	    p->insns = xrealloc (p->insns, ((p->size = 2 * p->size + 1)
					    * sizeof (*p->insns)));

	  p->insns[p->n++] = insn;
	}
    }
}

/* Perform deferred initialization of insns.  This is used to process 
   a new jump that may be created by redirect_edge.  */
static void
sel_insn_deferred_init (void)
{
  int i;
  struct insn_init_how_deferred_def *p = &insn_init.how_deferred;

  gcc_assert (insn_init.how == INSN_INIT_HOW_DEFERRED);

  insn_init.how = INSN_INIT_HOW_NOW;

  /* NB: This can be optimized to a single call to
     sched_data_update (NULL, NULL, NULL, p->insns) .  */
  for (i = 0; i < p->n; i++)
    sel_rtl_insn_added (p->insns[i]);

  p->n = 0;
}

/* Extracts machine mode MODE and destination location DST_LOC 
   for given INSN.  */
void
get_dest_and_mode (rtx insn, rtx *dst_loc, enum machine_mode *mode)
{
  rtx pat = PATTERN (insn);

  gcc_assert (dst_loc);
  gcc_assert (GET_CODE (pat) == SET);

  *dst_loc = SET_DEST (pat);

  gcc_assert (*dst_loc);
  gcc_assert (MEM_P (*dst_loc) || REG_P (*dst_loc));

  if (mode)
    *mode = GET_MODE (*dst_loc);
}

/* Returns true when moving through JUMP will result in bookkeeping 
   creation.  */
bool
bookkeeping_can_be_created_if_moved_through_p (insn_t jump)
{
  insn_t succ;
  succ_iterator si;

  if (BB_END (BLOCK_FOR_INSN (jump)) != jump
      || !control_flow_insn_p (jump))
    /* Exit early.  */
    return false;

  FOR_EACH_SUCC (succ, si, jump)
    if (num_preds_gt_1 (succ))
      return true;

  return false;
}

/* Makes a copy of INSN with emit_insn call and returns it.  */
insn_t
copy_insn_out_of_stream (vinsn_t vi)
{
  insn_t new_insn;
  insn_t insn = PATTERN (VINSN_INSN (vi));

  insn_init.what = INSN_INIT_WHAT_INSN;
  insn_init.todo = INSN_INIT_TODO_LUID  | INSN_INIT_TODO_INIT_ID;
  start_sequence ();
  new_insn = emit_insn (expr_copy (insn));
  end_sequence ();

  /* Fill the only field in s_i_d to make vinsn_create work.  */
  INSN_TYPE (new_insn) = VINSN_TYPE (vi);

  /* Copy other fields.  */
  INSN_COST (new_insn) = INSN_COST (VINSN_INSN (vi));
  INSN_PRIORITY (new_insn) = INSN_PRIORITY (VINSN_INSN (vi));
  /* !!! Initialize INSN_SCHED_TIMES () and possibly other field.  */

  return new_insn;
}

/* Inserts a copy of INSN_TO_COPY before the insn INS_BEFORE via the call of 
   emit_insn_before.
   FIXME: is expr_copy necessary?  */
insn_t
copy_insn_and_insert_before (insn_t insn_to_copy, insn_t ins_before)
{
  insn_t new_insn;

  insn_init.what = INSN_INIT_WHAT_INSN;
  insn_init.todo = INSN_INIT_TODO_LUID  | INSN_INIT_TODO_INIT_ID;

  new_insn = emit_insn_before (expr_copy (PATTERN (insn_to_copy)), 
			       ins_before);

  /* Fill the only field in s_i_d to make vinsn_create work.  */
  INSN_TYPE (new_insn) = INSN_TYPE (insn_to_copy);

  return new_insn;
}

/* Rip-off INSN from the insn stream.  */
void
sched_sel_remove_insn (insn_t insn)
{
  gcc_assert (AV_SET (insn) == NULL && !INSN_AV_VALID_P (insn)
	      && !LV_SET_VALID_P (insn));

  if (INSN_IN_STREAM_P (insn))
    remove_insn (insn);

  /* It is necessary to null this fields before calling add_insn ().  */
  PREV_INSN (insn) = NULL_RTX;
  NEXT_INSN (insn) = NULL_RTX;

  vinsn_detach (INSN_VI (insn));
}

/* Transfer av and lv sets from FROM to TO.  */
void
transfer_data_sets (insn_t to, insn_t from)
{
  /* We used to assert !INSN_AV_VALID_P here, but this is wrong when 
     during previous compute_av_set the window size was reached 
     exactly at TO.  In this case, AV_SET (to) would be NULL.  */
  gcc_assert (AV_SET (to) == NULL && !LV_SET_VALID_P (to));

  AV_SET (to) = AV_SET (from);
  AV_SET (from) = NULL;

  AV_LEVEL (to) = AV_LEVEL (from);
  AV_LEVEL (from) = 0;

  LV_SET (to) = LV_SET (from);
  LV_SET (from) =  NULL;
}

/* Estimate number of the insns in BB.  */
static int
sched_sel_estimate_number_of_insns (basic_block bb)
{
  int res = 0;
  insn_t insn = NEXT_INSN (BB_HEAD (bb)), next_tail = NEXT_INSN (BB_END (bb));

  for (; insn != next_tail; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      res++;

  return res;
}

/* We don't need separate luids for notes or labels.  */
static int
sched_sel_which_luid (insn_t note)
{
  gcc_assert (NOTE_P (note) || LABEL_P (note));
  return -1;
}

/* Extend the per-insn data structures.  */
static void
sel_extend_insn_info (void)
{
  int lvs_size_1 = get_max_uid () + 1;

  lvs = xrecalloc (lvs, lvs_size_1, lvs_size, sizeof (*lvs));
  lvs_size = lvs_size_1;

  s_i_d = xrecalloc (s_i_d, sched_max_uid, sel_max_uid, sizeof (*s_i_d));
  sel_max_uid = sched_max_uid;
}

/* Init data for INSN.  */
static void
sel_init_insn_info (insn_t insn)
{
  int todo = insn_init.todo;

  if (INSN_P (insn) && todo)
    {
      INSN_COST (insn) = insn_cost (insn, 0, 0);
    }

  if (INSN_P (insn))
    gcc_assert (can_add_real_insns_p);

  if (INSN_P (insn) && (todo & INSN_INIT_TODO_GLOBAL))
    {
      if (bb_header_p (insn))
	{
	  gcc_assert (!CANT_MOVE (insn));

	  insn_init.global.prev_insn = NULL_RTX;

	  init_deps (&insn_init.global.deps);
	  deps_start_bb (&insn_init.global.deps, insn);
	}

      insn_init.todo = INSN_INIT_TODO_GLOBAL;

      /* This will initialize CANT_MOVE () and SCHED_GROUP_P () data.  */
      deps_analyze_insn (&insn_init.global.deps, insn);

      if (BB_END (BLOCK_FOR_INSN (insn)) == insn)
	free_deps (&insn_init.global.deps);
    }

  if (INSN_P (insn) && (todo & INSN_INIT_TODO_VINSN))
    {
      gcc_assert (insn_init.what == INSN_INIT_WHAT_INSN);
      
      INSN_SIMPLEJUMP_P (insn) = !!simplejump_p (insn);

      INSN_VI (insn) = vinsn_create (insn, 1, false);
    }

  if (INSN_P (insn) && (todo & INSN_INIT_TODO_LV_SET))
    /* This is used to initialize spurious jumps generated by
       sel_split_block () / sel_redirect_edge ().  */
    {
      rtx succ = cfg_succ_1 (insn, SUCCS_ALL);

      gcc_assert (insn_init.what == INSN_INIT_WHAT_INSN
		  && INSN_SIMPLEJUMP_P (insn)
		  && !LV_SET_VALID_P (insn));

      if (bb_header_p (insn))
	{
	  LV_SET (insn) = get_regset_from_pool ();
	  COPY_REG_SET (LV_SET (insn), LV_SET (succ));
	}
    }

  if (INSN_P (insn) && (todo & INSN_INIT_TODO_SEQNO))
    {
      int seqno;

      gcc_assert (INSN_SIMPLEJUMP_P (insn));

      if (!bb_header_p (insn))
	seqno = INSN_SEQNO (PREV_INSN (insn));
      else
	{
	  insn_t *preds;
	  int i, n;

	  cfg_preds (BLOCK_FOR_INSN (insn), &preds, &n);

	  seqno = INSN_SEQNO (preds[0]);

	  for (i = 1; i < n; i++)
	    {
	      gcc_unreachable ();
	      if (INSN_SEQNO (preds[i]) > seqno)
		seqno = INSN_SEQNO (preds[i]);
	    }

	  free (preds);
	}

      gcc_assert (seqno <= INSN_SEQNO (cfg_succ (insn)));

      INSN_SEQNO (insn) = seqno;
    }

  if (INSN_P (insn) && (todo & INSN_INIT_TODO_MOVE_LV_SET_IF_BB_HEADER))
    {
      if (bb_header_p (insn))
	{
	  insn_t next = NEXT_INSN (insn);

	  gcc_assert (BLOCK_FOR_INSN (next) == BLOCK_FOR_INSN (insn)
		      && LV_SET_VALID_P (next));

	  LV_SET (insn) = LV_SET (next);
	  LV_SET (next) = NULL;
	}
    }

  insn_init.todo = todo;
}

/* Free the per-insn data structures.  */
static void
sel_finish_insn_info (void)
{
  free_nop_pool ();

  free (s_i_d);
  s_i_d = NULL;
  sel_max_uid = 0;
}

/* Return the cost of INSN as estimated by DFA.  This function properly handles
   ASMs, USEs etc.  */
int
dfa_cost (insn_t insn, fence_t fence)
{
  if (recog_memoized (insn) < 0)
    {
      bool asm_p;

      asm_p = (GET_CODE (PATTERN (insn)) == ASM_INPUT
	       || asm_noperands (PATTERN (insn)) >= 0);

      if (!FENCE_STARTS_CYCLE_P (fence) && asm_p)
	/* This is asm insn which is tryed to be issued on the
	   cycle not first.  Issue it on the next cycle.  */
	return 1;
      else
	/* A USE insn, or something else we don't need to
	   understand.  We can't pass these directly to
	   state_transition because it will trigger a
	   fatal error for unrecognizable insns.  */
	return 0;
    }
  else
    {
      int cost;
      state_t temp_state = alloca (dfa_state_size);

      state_copy (temp_state, FENCE_STATE (fence));

      cost = state_transition (temp_state, insn);

      if (cost < 0)
	return 0;
      else if (cost == 0)
	return 1;

      return cost;
    }
}


/* Functions to init/finish work with lv sets.  */

/* Init LV_SET of INSN from a global_live_at_start set of BB.
   NOTE: We do need to detach register live info from bb because we
   use those regsets as LV_SETs.  */
static void
init_lv_set_for_insn (insn_t insn, basic_block bb)
{
  LV_SET (insn) = get_regset_from_pool ();
  COPY_REG_SET (LV_SET (insn), glat_start[bb->index]);
}

void
init_lv_sets (void)
{
  basic_block bb;

  lvs_size = get_max_uid () + 1;
  lvs = xcalloc (lvs_size, sizeof (*lvs));

  /* Initialization of the LV sets.  */
  FOR_EACH_BB (bb)
    {
      insn_t head;
      insn_t tail;

      get_ebb_head_tail (bb, bb, &head, &tail);

      if (/* BB has at least one insn.  */
	  INSN_P (head))
	init_lv_set_for_insn (head, bb);
    }

  /* Don't forget EXIT_INSN.  */
  init_lv_set_for_insn (exit_insn, EXIT_BLOCK_PTR);
}

void
release_lv_set_for_insn (rtx head)
{
  int uid = INSN_UID (head);
  
  if (uid < lvs_size && lvs[uid] != NULL)
    return_regset_to_pool (lvs[uid]);
}


void
free_lv_sets (void)
{
  free (lvs);
  lvs = NULL;
  lvs_size = 0;
}



/* Functions to work with control-flow graph.  */

/* Return first insn in BB.  BB must not be empty.  
   ??? BB_HEAD must be used instead of this.  */
insn_t
bb_head (basic_block bb)
{
  insn_t head;
  insn_t tail;

  get_ebb_head_tail (bb, bb, &head, &tail);

  return INSN_P (head) ? head : NULL_RTX;
}

/* True when BB belongs to the current scheduling region.  */
bool
in_current_region_p (basic_block bb)
{
  if (bb->index < NUM_FIXED_BLOCKS)
    return false;

  return CONTAINING_RGN (bb->index) == CONTAINING_RGN (BB_TO_BLOCK (0));
}

/* An implementation of create_basic_block hook, which additionally updates 
   per-bb data structures.  */
basic_block
sel_create_basic_block (void *headp, void *endp, basic_block after)
{
  gcc_assert (last_added_block == NULL);

  last_added_block = old_create_basic_block (headp, endp, after);

  sched_data_update (NULL, last_added_block, NULL);

  return last_added_block;
}

/* True when BB is empty.  A block containing only a label is considered 
   empty as well.  */
bool
bb_empty_p (basic_block bb)
{
  insn_t head, tail;

  if (bb == EXIT_BLOCK_PTR)
    return false;

  get_ebb_head_tail (bb, bb, &head, &tail);
  return head == tail && NOTE_P (head);
}

/* Extend per bb data structures.  */
static void
sel_extend_bb_info (void)
{
  sel_bb_info = xrecalloc (sel_bb_info, last_basic_block,
			   sched_last_basic_block, sizeof (*sel_bb_info));
}

/* Free per-bb data structures.  */
static void
sel_finish_bb_info (void)
{
  free (sel_bb_info);
  sel_bb_info = NULL;
}

/* Remove all notes from BB.  */
static void
sel_remove_notes (basic_block bb)
{
  remove_notes (bb_note (bb), BB_END (bb));
  BB_NOTE_LIST (bb) = note_list;
}

/* Return a number of INSN's successors honoring FLAGS.  */
int
cfg_succs_n (insn_t insn, int flags)
{
  int n = 0;
  succ_iterator si;
  insn_t succ;

  FOR_EACH_SUCC_1 (succ, si, insn, flags)
    n++;

  return n;
}

/* Return the successors of INSN in SUCCSP and their number in NP, 
   honoring FLAGS.  Empty blocks are skipped.  */
void
cfg_succs_1 (insn_t insn, int flags, insn_t **succsp, int *np)
{
  int n;
  succ_iterator si;
  insn_t succ;

  n = *np = cfg_succs_n (insn, flags);

  *succsp = xmalloc (n * sizeof (**succsp));

  FOR_EACH_SUCC_1 (succ, si, insn, flags)
    (*succsp)[--n] = succ;
}

/* Find all successors of INSN and record them in SUCCSP and their number 
   in NP.  Empty blocks are skipped, and only normal (forward in-region) 
   edges are processed.  */
void
cfg_succs (insn_t insn, insn_t **succsp, int *np)
{
  cfg_succs_1 (insn, SUCCS_NORMAL, succsp, np);
}

/* Return the only successor of INSN, honoring FLAGS.  */
insn_t
cfg_succ_1 (insn_t insn, int flags)
{
  insn_t succ;
  succ_iterator si;
  bool b = true;

  FOR_EACH_SUCC_1 (succ, si, insn, flags)
    {
      gcc_assert (b);
      b = false;
    }

  return succ;
}

/* Return the only successor of INSN.  Only normal edges are processed.  */
insn_t
cfg_succ (insn_t insn)
{
  return cfg_succ_1 (insn, SUCCS_NORMAL);
}

/* Return the predecessors of BB in PREDS and their number in N. 
   Empty blocks are skipped.  SIZE is used to allocate PREDS.  */
static void
cfg_preds_1 (basic_block bb, insn_t **preds, int *n, int *size)
{
  edge e;
  edge_iterator ei;

  gcc_assert (BLOCK_TO_BB (bb->index) != 0);

  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      basic_block pred_bb = e->src;
      insn_t bb_end = BB_END (pred_bb);

      if (bb_note (pred_bb) == bb_end)
	cfg_preds_1 (pred_bb, preds, n, size);
      else
	{
	  if (*n == *size)
	    *preds = xrealloc (*preds, ((*size = 2 * *size + 1)
					* sizeof (**preds)));

	  (*preds)[(*n)++] = bb_end;
	}
    }

  gcc_assert (*n != 0);
}

/* Find all predecessors of BB and record them in PREDS and their number 
   in N.  Empty blocks are skipped, and only normal (forward in-region) 
   edges are processed.  */
static void
cfg_preds (basic_block bb, insn_t **preds, int *n)
{
  int size = 0;

  *preds = NULL;
  *n = 0;
  cfg_preds_1 (bb, preds, n, &size);
}

/* Returns true if we are moving INSN through join point.
   !!! Rewrite me: this should use cfg_preds ().  */
bool
num_preds_gt_1 (insn_t insn)
{
  basic_block bb;

  if (!bb_header_p (insn) || INSN_BB (insn) == 0)
    return false;

  bb = BLOCK_FOR_INSN (insn);

  while (1)
    {
      if (EDGE_COUNT (bb->preds) > 1)
	{
	  if (ENABLE_SEL_CHECKING)
	    {
	      edge e;
	      edge_iterator ei;

	      FOR_EACH_EDGE (e, ei, bb->preds)
		{
		  basic_block pred = e->src;

		  gcc_assert (in_current_region_p (pred));
		}
	    }

	  return true;
	}

      gcc_assert (EDGE_PRED (bb, 0)->dest == bb);
      bb = EDGE_PRED (bb, 0)->src;

      if (bb_note (bb) != BB_END (bb))
	break;
    }

  return false;
}

/* True when BB consists of a single jump.  */
static bool
bb_jump_only_p (basic_block bb)
{
  rtx jump = BB_END (bb);

  return NEXT_INSN (bb_note (bb)) == jump && INSN_SIMPLEJUMP_P (jump);
}

/* Returns true when BB should be the end of an ebb.  Adapted from the 
   code in sched-ebb.c.  */
bool
bb_ends_ebb_p (basic_block bb)
{
  basic_block next_bb = bb->next_bb;
  edge e;
  edge_iterator ei;
  
  if (next_bb == EXIT_BLOCK_PTR
      || bitmap_bit_p (forced_ebb_heads, next_bb->index)
      || (LABEL_P (BB_HEAD (next_bb))
	  /* NB: LABEL_NUSES () is not maintained outside of jump.c .
	     Work around that.  */
	  && !single_pred_p (next_bb)))
    return true;

  if (!in_current_region_p (next_bb))
    return true;

  FOR_EACH_EDGE (e, ei, bb->succs)
    if ((e->flags & EDGE_FALLTHRU) != 0)
      {
	gcc_assert (e->dest == next_bb);

	return false;
      }

  return true;
}

/* Returns true when INSN and SUCC are in the same EBB, given that SUCC is a
   successor of INSN.  */
bool
in_same_ebb_p (insn_t insn, insn_t succ)
{
  basic_block ptr = BLOCK_FOR_INSN (insn);

  for(;;)
    {
      if (ptr == BLOCK_FOR_INSN (succ))
        return true;
    
      if (bb_ends_ebb_p (ptr))
        return false;

      ptr = ptr->next_bb;
    }

  gcc_unreachable ();
  return false;
}

/* Returns true if INSN is not a downward continuation of the given path P in 
   the current stage.  */
bool
is_ineligible_successor (insn_t insn, ilist_t p)
{
  insn_t prev_insn;

  /* Check if insn is not deleted.  */
  if (PREV_INSN (insn) && NEXT_INSN (PREV_INSN (insn)) != insn)
    gcc_unreachable ();
  else if (NEXT_INSN (insn) && PREV_INSN (NEXT_INSN (insn)) != insn)
    gcc_unreachable ();

  /* If it's the first insn visited, then the successor is ok.  */
  if (!p)
    return false;

  prev_insn = ILIST_INSN (p);

  if (/* a backward edge.  */
      INSN_SEQNO (insn) < INSN_SEQNO (prev_insn)
      /* is already visited.  */
      || (INSN_SEQNO (insn) == INSN_SEQNO (prev_insn)
	  && (ilist_is_in_p (p, insn)
              /* We can reach another fence here and still seqno of insn 
                 would be equal to seqno of prev_insn.  This is possible 
                 when prev_insn is a previously created bookkeeping copy.
                 In that case it'd get a seqno of insn.  Thus, check here
                 whether insn is in current fence too.  */
              || IN_CURRENT_FENCE_P (insn)))
      /* Was already scheduled on this round.  */
      || (INSN_SEQNO (insn) > INSN_SEQNO (prev_insn)
	  && IN_CURRENT_FENCE_P (insn))
      /* An insn from another fence could also be 
	 scheduled earlier even if this insn is not in 
	 a fence list right now.  Check INSN_SCHED_CYCLE instead.  */
      || (!pipelining_p && INSN_SCHED_CYCLE (insn) > 0))
    return true;
  else
    return false;
}

/* Add (or remove depending on ADD_P) BB to (from) the current region 
   and update sched-rgn.c data.  */
void
sel_add_or_remove_bb_1 (basic_block bb, bool add_p)
{
  int i, pos, bbi = -2, rgn;
  int step = add_p ? 1 : 0;

  if (step)
    {
      /* Calculate topological index of BB's predesessor.
	 (BBI + 1) will be the topological sort number of the BB.  */

      if (EDGE_COUNT (bb->preds) == 1)
	{
	  basic_block pred = EDGE_PRED (bb, 0)->src;

	  if (!in_current_region_p (pred))
	    /* This is the case when we generate an extra empty block
	       to serve as region head during pipelining.
	       BB will be the new head.  */
	    {
	      gcc_assert (EDGE_COUNT (bb->succs) == 1
			  && in_current_region_p (EDGE_SUCC (bb, 0)->dest)
			  && (BLOCK_TO_BB (EDGE_SUCC (bb, 0)->dest->index)
			      == 0));

	      bbi = -1;
	    }
	}

      if (bbi == -2)
	{
	  if (EDGE_COUNT (bb->succs) > 0)
	    {
	      int pred_bbi;

	      gcc_assert (EDGE_COUNT (bb->preds) == 1);

	      pred_bbi = EDGE_PRED (bb, 0)->src->index;

	      bbi = BLOCK_TO_BB (pred_bbi);
	    }
	  else
	    /* BB doesn't have no successors.  It is safe to put it in the
	       end.  */
	    bbi = current_nr_blocks - 1;
	}
    }
  else
    bbi = BLOCK_TO_BB (bb->index);

  rgn = CONTAINING_RGN (BB_TO_BLOCK (0));

  bbi += step;
  pos = RGN_BLOCKS (rgn) + bbi;

  gcc_assert (RGN_HAS_REAL_EBB (rgn) == 0
              && ebb_head[bbi] == pos);

  /* First of all, we free outdated info:
     Nothing to be done here.  */

  if (step)
    {
      /* Second, we make a place for the new block.  */
      extend_regions ();

      for (i = RGN_BLOCKS (rgn + 1) - 1; i >= pos; i--)
	/* We better not use EBB_HEAD here, as it has region-scope.  */
	BLOCK_TO_BB (rgn_bb_table[i])++;
    }
  else
    {
      for (i = RGN_BLOCKS (rgn + 1) - 1; i >= pos; i--)
	BLOCK_TO_BB (rgn_bb_table[i])--;
    }

  memmove (rgn_bb_table + pos + step,
           rgn_bb_table + pos + 1 - step,
           (RGN_BLOCKS (nr_regions) - pos) * sizeof (*rgn_bb_table));

  if (step)
    {
      /* Third, we initialize data for BB.  */
      rgn_bb_table[pos] = bb->index;
      BLOCK_TO_BB (bb->index) = bbi;
      CONTAINING_RGN (bb->index) = rgn;

      RGN_NR_BLOCKS (rgn)++;

      for (i = rgn + 1; i <= nr_regions; i++)
	RGN_BLOCKS (i)++;
    }
  else
    {
      RGN_NR_BLOCKS (rgn)--;
      for (i = rgn + 1; i <= nr_regions; i++)
	RGN_BLOCKS (i)--;

      delete_basic_block (bb);
    }
}

/* Add (remove depending on ADD_P) BB to (from) the current region 
   and update all data.  */
void
sel_add_or_remove_bb (basic_block bb, bool add_p)
{
  bool b;

  if (add_p)
    {
      gcc_assert (last_added_block != NULL);
      last_added_block = NULL;
    }
  else
    gcc_assert (BB_NOTE_LIST (bb) == NULL_RTX);

  sel_add_or_remove_bb_1 (bb, add_p);

  b = sched_rgn_local_init (CONTAINING_RGN (bb->index), true);
  gcc_assert (!b);
}

/* Remove an empty basic block EMPTY_BB.  When MERGE_UP_P is true, we put 
   EMPTY_BB's note lists into its predecessor instead of putting them 
   into the successor.  */
void
sel_remove_empty_bb (basic_block empty_bb, bool merge_up_p)
{
  basic_block merge_bb;

  if (merge_up_p)
    {
      merge_bb = empty_bb->prev_bb;

      gcc_assert (EDGE_COUNT (empty_bb->preds) == 1
		  && EDGE_PRED (empty_bb, 0)->src == merge_bb);
    }
  else
    {
      merge_bb = empty_bb->next_bb;

      gcc_assert (EDGE_COUNT (empty_bb->succs) == 1
		  && EDGE_SUCC (empty_bb, 0)->dest == merge_bb);
    }

  gcc_assert (in_current_region_p (merge_bb));

  concat_note_lists (BB_NOTE_LIST (empty_bb), 
		     &BB_NOTE_LIST (merge_bb));
  BB_NOTE_LIST (empty_bb) = NULL_RTX;

  /* Fixup CFG.  */

  gcc_assert (/* The BB contains just a bb note ...  */
	      BB_HEAD (empty_bb) == BB_END (empty_bb)
	      /* ... or an unused label.  */
	      || (LABEL_P (BB_HEAD (empty_bb))
		  /* This guarantees that the only pred edge is a fallthru
		     one.

		     NB: We can't use LABEL_NUSES because it is not maintained
		     outside jump.c .  We check that the only pred edge is
		     fallthru one below.  */
		  && true));

  /* If basic block has predecessors or successors, redirect them.  */
  if (EDGE_COUNT (empty_bb->preds) > 0
      || EDGE_COUNT (empty_bb->succs) > 0)
    {
      basic_block pred;
      basic_block succ;

      /* We need to init PRED and SUCC before redirecting edges.  */
      if (EDGE_COUNT (empty_bb->preds) > 0)
	{
	  edge e;

	  gcc_assert (EDGE_COUNT (empty_bb->preds) == 1);

	  e = EDGE_PRED (empty_bb, 0);

	  gcc_assert (e->src == empty_bb->prev_bb
		      && (e->flags & EDGE_FALLTHRU));

	  pred = empty_bb->prev_bb;
	}
      else
	pred = NULL;

      if (EDGE_COUNT (empty_bb->succs) > 0)
	{
	  edge e;

	  gcc_assert (EDGE_COUNT (empty_bb->succs) == 1);

	  e = EDGE_SUCC (empty_bb, 0);

	  gcc_assert (e->dest == empty_bb->next_bb
		      && (e->flags & EDGE_FALLTHRU));

	  succ = empty_bb->next_bb;
	}
      else
	succ = NULL;


      if (EDGE_COUNT (empty_bb->preds) > 0 && succ != NULL)
	redirect_edge_succ_nodup (EDGE_PRED (empty_bb, 0), succ);

      if (EDGE_COUNT (empty_bb->succs) > 0 && pred != NULL)
	{
	  edge e = EDGE_SUCC (empty_bb, 0);

	  if (find_edge (pred, e->dest) == NULL)
	    redirect_edge_pred (e, pred);
	}
    }

  /* Finish removing.  */
  sel_add_or_remove_bb (empty_bb, false);
}

/* Splits BB on two basic blocks, adding it to the region and extending 
   per-bb data structures.  Returns the newly created bb.  */
basic_block
sel_split_block (basic_block bb)
{
  basic_block new_bb;

  insn_init.what = INSN_INIT_WHAT_INSN;
  insn_init.todo = ((INSN_INIT_TODO_ALL & ~INSN_INIT_TODO_SEQNO)
		    | INSN_INIT_TODO_LV_SET);

  new_bb = split_block_after_labels (bb)->dest;

  sel_add_or_remove_bb (new_bb, true);

  gcc_assert (bb_empty_p (bb));

  return new_bb;
}

/* A wrapper for redirect_edge_and_branch_force, which also initializes
   data structures for possibly created bb and insns.  Returns the newly
   added bb or NULL, when a bb was not needed.  */
basic_block
sel_redirect_edge_force (edge e, basic_block to)
{
  basic_block jump_bb;

  gcc_assert (bb_note (e->src) != BB_END (e->src));

  insn_init.how = INSN_INIT_HOW_DEFERRED;
  jump_bb = redirect_edge_and_branch_force (e, to);

  gcc_assert (last_added_block == jump_bb);

  if (jump_bb)
    sel_add_or_remove_bb (jump_bb, true);

  /* Now the CFG has been updated, and we can init data for the newly 
     created insns.  */
  sel_insn_deferred_init ();

  /* Only after sel_insn_deferred_init () we can check for 
     INSN_SIMPLEJUMP_P ().  */
  gcc_assert (jump_bb == NULL || bb_jump_only_p (jump_bb));

  return jump_bb;
}

/* A wrapper for create_basic_block_before, which also extends per-bb 
   data structures.  Returns the newly created bb.  */
basic_block
sel_create_basic_block_before (basic_block before)
{
  basic_block prev_bb;
  basic_block bb;
  edge e;

  gcc_assert (in_current_region_p (before));

  prev_bb = before->prev_bb;

  e = find_fallthru_edge (prev_bb);
  gcc_assert (e != NULL);

  /* This code is taken from cfghooks.c: split_block ().  */
  bb = create_basic_block (BB_HEAD (before), NULL_RTX, prev_bb);
  bb->count = prev_bb->count;
  bb->frequency = prev_bb->frequency;
  bb->loop_depth = prev_bb->loop_depth;
  make_single_succ_edge (bb, before, EDGE_FALLTHRU);

  redirect_edge_succ (e, bb);

  sel_add_or_remove_bb (bb, true);

  return bb;
}


/* Helpers for global init.  */
/* Data structure to describe interaction with the generic scheduler utils.  */
static struct common_sched_info_def sel_common_sched_info;

/* This structure is used to be able to call existing bundling mechanism
   and calculate insn priorities.  */
static struct haifa_sched_info sched_sel_haifa_sched_info = 
{
  NULL, /* init_ready_list */
  NULL, /* can_schedule_ready_p */
  NULL, /* schedule_more_p */
  NULL, /* new_ready */
  NULL, /* rgn_rank */
  sel_print_insn, /* rgn_print_insn */
  contributes_to_priority,

  NULL, NULL,
  NULL, NULL,
  0, 0,

  NULL, /* add_remove_insn */
  NULL, /* begin_schedule_ready */
  NULL, /* advance_target_bb */
};

/* Setup special insns used in the scheduler.  */
void 
setup_nop_and_exit_insns (void)
{
  if (!nop_pattern)
    nop_pattern = gen_nop ();

  if (exit_insn == NULL_RTX)
    {
      start_sequence ();
      emit_insn (nop_pattern);
      exit_insn = get_insns ();
      end_sequence ();
    }
  set_block_for_insn (exit_insn, EXIT_BLOCK_PTR);
}

void
free_exit_insn_data (void)
{
  gcc_assert (LV_SET_VALID_P (exit_insn));
  return_regset_to_pool (LV_SET (exit_insn));

  exit_insn = NULL_RTX;

}

/* Setup pointers to global sched info structures.  */
void
setup_sched_and_deps_infos (void)
{
  memcpy (&sel_common_sched_info, &rgn_common_sched_info,
	  sizeof (sel_common_sched_info));
  sel_common_sched_info.sched_pass_id = SCHED_SEL_PASS;
  sel_common_sched_info.fix_recovery_cfg = NULL;
  sel_common_sched_info.add_block = NULL;
  sel_common_sched_info.estimate_number_of_insns
    = sched_sel_estimate_number_of_insns;
  sel_common_sched_info.which_luid = sched_sel_which_luid;
  sel_common_sched_info.remove_notes = sel_remove_notes;
  sel_common_sched_info.bb_extend = sel_extend_bb_info;
  sel_common_sched_info.bb_finish = sel_finish_bb_info;
  sel_common_sched_info.insn_extend = sel_extend_insn_info;
  sel_common_sched_info.insn_init = sel_init_insn_info;
  sel_common_sched_info.insn_finish = sel_finish_insn_info;

  common_sched_info = &sel_common_sched_info;
  current_sched_info = &sched_sel_haifa_sched_info;

  sched_deps_info = &sel_sched_deps_info;
}

#endif
