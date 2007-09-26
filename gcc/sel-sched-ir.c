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

/* A vector holding bb info for whole scheduling pass.  */
VEC(sel_global_bb_info_def, heap) *sel_global_bb_info = NULL;

/* A vector holding bb info.  */
VEC(sel_region_bb_info_def, heap) *sel_region_bb_info = NULL;

/* Data structure to describe interaction with the generic scheduler utils.  */
static struct common_sched_info_def sel_common_sched_info;

/* The loop nest being pipelined.  */
struct loop *current_loop_nest;

/* LOOP_NESTS is a vector containing the corresponding loop nest for
   each region.  */
static VEC(loop_p, heap) *loop_nests = NULL;

/* Saves blocks already in loop regions, indexed by bb->index.  */
static sbitmap bbs_in_loop_rgns = NULL;


/* Array containing reverse topological index of function basic blocks,
   indexed by BB->INDEX.  */
static int *rev_top_order_index = NULL;

/* Length of the above array.  */
static int rev_top_order_index_len = -1;

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

static void deps_init_id (idata_t, insn_t, bool);
static void cfg_preds (basic_block, insn_t **, int *);

static void sel_add_or_remove_bb (basic_block, int);
static void free_data_sets (basic_block);
static void move_bb_info (basic_block, basic_block);
static void remove_empty_bb (basic_block, bool);


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
def_list_add (def_list_t *dl, insn_t original_insn, bool crosses_call,
	      bool needs_spec_check_p)
{
  def_t d;
  _list_add (dl);
  d = DEF_LIST_DEF (*dl);

  d->orig_insn = original_insn;
  d->crosses_call = crosses_call;
  d->needs_spec_check_p = needs_spec_check_p;
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

static struct sched_deps_info_def _advance_deps_context_sched_deps_info =
  {
    NULL,

    NULL, /* start_insn */
    NULL, /* finish_insn */
    NULL, /* start_x */
    NULL, /* finish_x */
    NULL, /* start_lhs */
    NULL, /* finish_lhs */
    NULL, /* start_rhs */
    NULL, /* finish_rhs */
    haifa_note_reg_set,
    haifa_note_reg_clobber,
    haifa_note_reg_use,
    NULL, /* note_mem_dep */
    NULL, /* note_dep */

    0, 0, 0
  };

/* Process INSN and add its impact on DC.  */
void
advance_deps_context (deps_t dc, insn_t insn)
{
  sched_deps_info = &_advance_deps_context_sched_deps_info;
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
    delete_deps_context (dc);

  if (tc != NULL)
    delete_target_context (tc);
}

/* Init a list of fences with successors of OLD_FENCE.  */
void
init_fences (insn_t old_fence)
{
  int succs_num;
  insn_t *succs;
  int i;

  cfg_succs_1 (old_fence, SUCCS_NORMAL | SUCCS_SKIP_TO_LOOP_EXITS, 
	       &succs, &succs_num);

  gcc_assert (flag_sel_sched_pipelining_outer_loops
	      || succs_num == 1);

  for (i = 0; i < succs_num; i++)
    {
      flist_add (&fences, succs[i],
		 state_create (),
		 create_deps_context () /* dc */,
		 create_target_context (true) /* tc */,
		 NULL_RTX /* last_scheduled_insn */, NULL_RTX /* sched_next */,
		 1 /* cycle */, 0 /* cycle_issued_insns */, 
		 1 /* starts_cycle_p */, 0 /* after_stall_p */);  
    }
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
      gcc_assert (sel_bb_head_p (FENCE_INSN (f))
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

static void set_insn_init (expr_t, vinsn_t, int);
static void vinsn_attach (vinsn_t);
static void vinsn_detach (vinsn_t);

/* A vinsn that is used to represent a nop.  This vinsn is shared among all
   nops sel-sched generates.  */
static vinsn_t nop_vinsn = NULL;

/* Emit a nop before INSN, taking it from pool.  */
insn_t
get_nop_from_pool (insn_t insn)
{
  insn_t nop;
  bool old_p = nop_pool.n != 0;

  if (old_p)
    nop = nop_pool.v[--nop_pool.n];
  else
    nop = nop_pattern;

  insn_init.what = INSN_INIT_WHAT_INSN;
  nop = emit_insn_before (nop, insn);

  if (old_p)
    insn_init.todo = INSN_INIT_TODO_SSID;
  else
    insn_init.todo = INSN_INIT_TODO_LUID | INSN_INIT_TODO_SSID;

  set_insn_init (INSN_EXPR (insn), nop_vinsn, INSN_SEQNO (insn));
  sel_init_new_insns ();

  return nop;
}

/* Remove NOP from the instruction stream and return it to the pool.  */
void
return_nop_to_pool (insn_t nop)
{
  gcc_assert (INSN_IN_STREAM_P (nop));
  sel_remove_insn (nop);

  if (nop_pool.n == nop_pool.s)
    nop_pool.v = xrealloc (nop_pool.v, ((nop_pool.s = 2 * nop_pool.s + 1)
					* sizeof (*nop_pool.v)));

  nop_pool.v[nop_pool.n++] = nop;
}

/* Free the nop pool.  */
void
free_nop_pool (void)
{
  nop_pool.n = 0;
  nop_pool.s = 0;
  free (nop_pool.v);
  nop_pool.v = NULL;
}


/* Return 1 if X and Y are identical-looking rtx's.
   This is the Lisp function EQUAL for rtx arguments.
   Copied from rtl.c.  The only difference is support for ia64 speculation.  */
static int
sel_rtx_equal_p (rtx x, rtx y)
{
  int i;
  int j;
  enum rtx_code code;
  const char *fmt;

  if (x == y)
    return 1;
  if (x == 0 || y == 0)
    return 0;

  /* Support ia64 speculation.  */
  {
    if (GET_CODE (x) == UNSPEC
	&& (targetm.sched.skip_rtx_p == NULL
	    || targetm.sched.skip_rtx_p (x)))
      return sel_rtx_equal_p (XVECEXP (x, 0, 0), y);

    if (GET_CODE (y) == UNSPEC
	&& (targetm.sched.skip_rtx_p == NULL
	    || targetm.sched.skip_rtx_p (y)))
      return sel_rtx_equal_p (x, XVECEXP (y, 0, 0));
  }

  code = GET_CODE (x);
  /* Rtx's of different codes cannot be equal.  */
  if (code != GET_CODE (y))
    return 0;

  /* (MULT:SI x y) and (MULT:HI x y) are NOT equivalent.
     (REG:SI x) and (REG:HI x) are NOT equivalent.  */

  if (GET_MODE (x) != GET_MODE (y))
    return 0;

  /* Some RTL can be compared nonrecursively.  */
  switch (code)
    {
    case REG:
      return (REGNO (x) == REGNO (y));

    case LABEL_REF:
      return XEXP (x, 0) == XEXP (y, 0);

    case SYMBOL_REF:
      return XSTR (x, 0) == XSTR (y, 0);

    case SCRATCH:
    case CONST_DOUBLE:
    case CONST_INT:
      return 0;

    default:
      break;
    }

  /* Compare the elements.  If any pair of corresponding elements
     fail to match, return 0 for the whole thing.  */

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      switch (fmt[i])
	{
	case 'w':
	  if (XWINT (x, i) != XWINT (y, i))
	    return 0;
	  break;

	case 'n':
	case 'i':
	  if (XINT (x, i) != XINT (y, i))
	    return 0;
	  break;

	case 'V':
	case 'E':
	  /* Two vectors must have the same length.  */
	  if (XVECLEN (x, i) != XVECLEN (y, i))
	    return 0;

	  /* And the corresponding elements must match.  */
	  for (j = 0; j < XVECLEN (x, i); j++)
	    if (sel_rtx_equal_p (XVECEXP (x, i, j), XVECEXP (y, i, j)) == 0)
	      return 0;
	  break;

	case 'e':
	  if (sel_rtx_equal_p (XEXP (x, i), XEXP (y, i)) == 0)
	    return 0;
	  break;

	case 'S':
	case 's':
	  if ((XSTR (x, i) || XSTR (y, i))
	      && (! XSTR (x, i) || ! XSTR (y, i)
		  || strcmp (XSTR (x, i), XSTR (y, i))))
	    return 0;
	  break;

	case 'u':
	  /* These are just backpointers, so they don't matter.  */
	  break;

	case '0':
	case 't':
	  break;

	  /* It is believed that rtx's at this level will never
	     contain anything but integers and other rtx's,
	     except for within LABEL_REFs and SYMBOL_REFs.  */
	default:
	  gcc_unreachable ();
	}
    }
  return 1;
}

/* Hash an rtx X.  The difference from hash_rtx is that we try to hash as 
   much stuff as possible, not skipping volatile mems, calls, etc.  */

static unsigned
sel_hash_rtx (rtx x, enum machine_mode mode)
{
  int i, j;
  unsigned hash = 0;
  enum rtx_code code;
  const char *fmt;

  /* Used to turn recursion into iteration.  */
 repeat:
  if (x == 0)
    return hash;

  code = GET_CODE (x);
  switch (code)
    {
    case REG:
      {
	unsigned int regno = REGNO (x);

	hash += ((unsigned int) REG << 7);
        hash += regno;
	return hash;
      }

    case SUBREG:
      {
	if (REG_P (SUBREG_REG (x)))
	  {
	    hash += (((unsigned int) SUBREG << 7)
		     + REGNO (SUBREG_REG (x))
		     + (SUBREG_BYTE (x) / UNITS_PER_WORD));
	    return hash;
	  }
	break;
      }

    case CONST_INT:
      hash += (((unsigned int) CONST_INT << 7) + (unsigned int) mode
               + (unsigned int) INTVAL (x));
      return hash;

    case CONST_DOUBLE:
      hash += (unsigned int) code + (unsigned int) GET_MODE (x);
      if (GET_MODE (x) != VOIDmode)
	hash += real_hash (CONST_DOUBLE_REAL_VALUE (x));
      else
	hash += ((unsigned int) CONST_DOUBLE_LOW (x)
		 + (unsigned int) CONST_DOUBLE_HIGH (x));
      return hash;

    case CONST_VECTOR:
      {
	int units;
	rtx elt;

	units = CONST_VECTOR_NUNITS (x);

	for (i = 0; i < units; ++i)
	  {
	    elt = CONST_VECTOR_ELT (x, i);
	    hash += sel_hash_rtx (elt, GET_MODE (elt));
	  }

	return hash;
      }

      /* Assume there is only one rtx object for any given label.  */
    case LABEL_REF:
      /* We don't hash on the address of the CODE_LABEL to avoid bootstrap
	 differences and differences between each stage's debugging dumps.  */
	 hash += (((unsigned int) LABEL_REF << 7)
		  + CODE_LABEL_NUMBER (XEXP (x, 0)));
      return hash;

    case SYMBOL_REF:
      {
	/* Don't hash on the symbol's address to avoid bootstrap differences.
	   Different hash values may cause expressions to be recorded in
	   different orders and thus different registers to be used in the
	   final assembler.  This also avoids differences in the dump files
	   between various stages.  */
	unsigned int h = 0;
	const unsigned char *p = (const unsigned char *) XSTR (x, 0);

	while (*p)
	  h += (h << 7) + *p++; /* ??? revisit */

	hash += ((unsigned int) SYMBOL_REF << 7) + h;
	return hash;
      }

    case MEM:
      hash += (unsigned) MEM;
      x = XEXP (x, 0);
      goto repeat;

    case USE:
      if (MEM_P (XEXP (x, 0))
	  && ! MEM_VOLATILE_P (XEXP (x, 0)))
	{
	  hash += (unsigned) USE;
	  x = XEXP (x, 0);

	  hash += (unsigned) MEM;
	  x = XEXP (x, 0);
	  goto repeat;
	}
      break;

    case UNSPEC:
      /* Skip UNSPECs when we are so told.  */
      if (targetm.sched.skip_rtx_p && targetm.sched.skip_rtx_p (x))
        {
          hash += sel_hash_rtx (XVECEXP (x, 0, 0), 0);
          return hash;
        }
      break;
        
    case ASM_OPERANDS:
      /* We don't want to take the filename and line into account.  */
      hash += (unsigned) code + (unsigned) GET_MODE (x)
        + hash_rtx_string (ASM_OPERANDS_TEMPLATE (x))
        + hash_rtx_string (ASM_OPERANDS_OUTPUT_CONSTRAINT (x))
        + (unsigned) ASM_OPERANDS_OUTPUT_IDX (x);

      if (ASM_OPERANDS_INPUT_LENGTH (x))
        {
          for (i = 1; i < ASM_OPERANDS_INPUT_LENGTH (x); i++)
            {
              hash += (sel_hash_rtx (ASM_OPERANDS_INPUT (x, i),
                                 GET_MODE (ASM_OPERANDS_INPUT (x, i)))
                       + hash_rtx_string
                       (ASM_OPERANDS_INPUT_CONSTRAINT (x, i)));
            }

          hash += hash_rtx_string (ASM_OPERANDS_INPUT_CONSTRAINT (x, 0));
          x = ASM_OPERANDS_INPUT (x, 0);
          mode = GET_MODE (x);
          goto repeat;
        }

      return hash;
      
    default:
      break;
    }

  i = GET_RTX_LENGTH (code) - 1;
  hash += (unsigned) code + (unsigned) GET_MODE (x);
  fmt = GET_RTX_FORMAT (code);
  for (; i >= 0; i--)
    {
      switch (fmt[i])
	{
	case 'e':
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function  is called enough to be worth it.  */
	  if (i == 0)
	    {
	      x = XEXP (x, i);
	      goto repeat;
	    }

	  hash += sel_hash_rtx (XEXP (x, i), 0);
	  break;

	case 'E':
	  for (j = 0; j < XVECLEN (x, i); j++)
	    hash += sel_hash_rtx (XVECEXP (x, i, j), 0);
	  break;

	case 's':
	  hash += hash_rtx_string (XSTR (x, i));
	  break;

	case 'i':
	  hash += (unsigned int) XINT (x, i);
	  break;

	case '0': case 't':
	  /* Unused.  */
	  break;

	default:
	  gcc_unreachable ();
	}
    }

  return hash;
}

static bool
vinsn_equal_p (vinsn_t vi1, vinsn_t vi2)
{
  if (VINSN_TYPE (vi1) != VINSN_TYPE (vi2))
    return false;

  return (VINSN_UNIQUE_P (vi1)
	  ? VINSN_INSN (vi1) == VINSN_INSN (vi2)
	  : sel_rtx_equal_p (VINSN_PATTERN (vi1), VINSN_PATTERN (vi2)));
}

/* Returns LHS and RHS are ok to be scheduled separately.  */
static bool
lhs_and_rhs_separable_p (rtx lhs, rtx rhs)
{
  if (lhs == NULL || rhs == NULL)
    return false;

  /* Do not schedule CONST, CONST_INT and CONST_DOUBLE etc as rhs: no point 
     to use reg, if const can be used.  Moreover, scheduling const as rhs may 
     lead to mode mismatch cause consts don't have modes but they could be 
     merged from branches where the same const used in different modes.  */
  if (CONSTANT_P (rhs))
    return false;

  /* ??? Do not rename predicate registers to avoid ICEs in bundling.  */
  if (COMPARISON_P (rhs))
      return false;

  /* Do not allow single REG to be an rhs.  */
  if (REG_P (rhs))
    return false;

  /* See comment at find_used_regs_1 (*1) for explanation of this 
     restriction.  */
  /* FIXME: remove this later.  */
  if (MEM_P (lhs))
    return false;

  /* This will filter all tricky things like ZERO_EXTRACT etc.
     For now we don't handle it.  */
  if (!REG_P (lhs) && !MEM_P (lhs))
    return false;

  return true;
}

/* Initialize vinsn VI for INSN.  Only for use from vinsn_create ().  */
static void
vinsn_init (vinsn_t vi, insn_t insn, bool force_unique_p)
{
  idata_t id = xcalloc (1, sizeof (*id));

  VINSN_INSN (vi) = insn;

  vi->cost = -1;

  deps_init_id (id, insn, force_unique_p);
  VINSN_ID (vi) = id;
  
  /* Hash vinsn depending on whether it is separable or not.  */
  if (VINSN_SEPARABLE_P (vi))
    {
      rtx rhs = VINSN_RHS (vi);

      VINSN_HASH (vi) = sel_hash_rtx (rhs, GET_MODE (rhs));
    }
  else
    VINSN_HASH (vi) = sel_hash_rtx (VINSN_PATTERN (vi), VOIDmode);
    
  VINSN_COUNT (vi) = 0;

  {
    int class = haifa_classify_insn (insn);

    if (class >= 2
	&& (!targetm.sched.get_insn_spec_ds
	    || ((targetm.sched.get_insn_spec_ds (insn) & BEGIN_CONTROL)
		== 0)))
      VINSN_MAY_TRAP_P (vi) = true;
    else
      VINSN_MAY_TRAP_P (vi) = false;
  }
}

/* Indicate that VI has become the part of an rtx object.  */
static void
vinsn_attach (vinsn_t vi)
{
  /* Assert that VI is not pending for deletion.  */
  gcc_assert (VINSN_INSN (vi));

  VINSN_COUNT (vi)++;
}

/* Create and init VI from the INSN.  Initialize VINSN_COUNT (VI) with COUNT 
   and use UNIQUE_P for determining the correct VINSN_TYPE (VI).  */
static vinsn_t
vinsn_create (insn_t insn, bool force_unique_p)
{
  vinsn_t vi = xmalloc (sizeof (*vi));

  vinsn_init (vi, insn, force_unique_p);

  return vi;
}

/* Delete the VI vinsn and free its data.  */
static void
vinsn_delete (vinsn_t vi)
{
  gcc_assert (VINSN_COUNT (vi) == 0);

  return_regset_to_pool (VINSN_REG_SETS (vi));
  return_regset_to_pool (VINSN_REG_USES (vi));

  free (VINSN_ID (vi));

  /* This insn should not be deleted as it may have shared parts.  */
  /*  if (!INSN_IN_STREAM_P (insn))  expr_clear (&insn); */

  free (vi);
}

/* Indicate that VI is no longer a part of some rtx object.  
   Remove VI if it is no longer needed.  */
static void
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

/* Return latency of INSN.  */
static int
sel_insn_rtx_cost (rtx insn)
{
  int cost;

  /* A USE insn, or something else we don't need to
     understand.  We can't pass these directly to
     result_ready_cost or insn_default_latency because it will
     trigger a fatal error for unrecognizable insns.  */
  if (recog_memoized (insn) < 0)
    cost = 0;
  else
    {
      cost = insn_default_latency (insn);

      if (cost < 0)
	cost = 0;
    }

  return cost;
}

/* Return the cost of the VI.
   !!! FIXME: Unify with haifa-sched.c: insn_cost ().  */
int
sel_vinsn_cost (vinsn_t vi)
{
  int cost = vi->cost;

  if (cost < 0)
    {
      cost = sel_insn_rtx_cost (VINSN_INSN (vi));
      vi->cost = cost;
    }

  return cost;
}

static bool insn_is_the_only_one_in_bb_p (insn_t);
static void init_invalid_data_sets (basic_block);

/* Emit new insn after AFTER based on PATTERN and initialize its data from
   EXPR and SEQNO.  */
insn_t
sel_gen_insn_from_rtx_after (rtx pattern, expr_t expr, int seqno, insn_t after)
{
  insn_t new_insn;

  insn_init.what = INSN_INIT_WHAT_INSN;

  new_insn = emit_insn_after (pattern, after);

  insn_init.todo = INSN_INIT_TODO_LUID | INSN_INIT_TODO_SSID;
  set_insn_init (expr, NULL, seqno);
  sel_init_new_insns ();

  return new_insn;
}

/* Force newly generated vinsns to be unique.  */
static bool init_insn_force_unique_p = false;

/* Emit new speculation recovery insn after AFTER based on PATTERN and
   initialize its data from EXPR and SEQNO.  */
insn_t
sel_gen_recovery_insn_from_rtx_after (rtx pattern, expr_t expr, int seqno,
				      insn_t after)
{
  insn_t insn;

  gcc_assert (!init_insn_force_unique_p);

  init_insn_force_unique_p = true;
  insn = sel_gen_insn_from_rtx_after (pattern, expr, seqno, after);
  CANT_MOVE (insn) = 1;
  init_insn_force_unique_p = false;

  return insn;
}

/* Emit new insn after AFTER based on EXPR and SEQNO.  */
insn_t
sel_gen_insn_from_expr_after (rhs_t expr, int seqno, insn_t after)
{
  insn_t insn = RHS_INSN (expr);

  gcc_assert (!INSN_IN_STREAM_P (insn));

  insn_init.what = INSN_INIT_WHAT_INSN;
  add_insn_after (insn, after, BLOCK_FOR_INSN (insn));

  /* Do not simplify insn if it is not a final schedule.  */
  if (!pipelining_p)
    validate_simplify_insn (insn);

  insn_init.todo = INSN_INIT_TODO_SSID;
  set_insn_init (expr, EXPR_VINSN (expr), seqno);

  if (INSN_LUID (insn) == 0)
    insn_init.todo |= INSN_INIT_TODO_LUID;

  sel_init_new_insns ();

  return insn;
}

/* Functions to work with right-hand sides.  */

/* Search for a hash value HASH in a sorted vector VECT and return true
   when found.  Write to INDP the index on which the search has stopped,
   such that inserting HASH at INDP will retain VECT's sort order.  */
static bool
find_in_hash_vect_1 (VEC(unsigned, heap) *vect, unsigned hash, int *indp)
{
  unsigned *arr;
  int i, j, len = VEC_length (unsigned, vect);

  if (len == 0)
    {
      *indp = 0;
      return false;
    }

  arr = VEC_address (unsigned, vect);
  i = 0, j = len - 1;

  while (i <= j)
    {
#if 0
      int k = (i + j) / 2;

      if (arr[k] == hash)
        {
          *indp = k;
          return true;
        }

      if (arr[k] < hash)
        i = k + 1;
      else
        j = k - 1;
#else
      unsigned ahash = arr[i];

      if (ahash == hash)
        {
          *indp = i;
          return true;
        }
      else if (ahash > hash)
        break;
      i++;
#endif
    }

  *indp = i;
  return false;
}

/* Search for a hash value HASH in a sorted vector VECT.  Return 
   the position found or -1, if no such value is in vector.  */
int
find_in_hash_vect (VEC(unsigned, heap) *vect, unsigned hash)
{
  int ind;

  if (find_in_hash_vect_1 (vect, hash, &ind))
    return ind;
  
  return -1;
}

/* Insert HASH in a sorted vector pointed to by PVECT, if HASH is
   not there already.  */
void
insert_in_hash_vect (VEC (unsigned, heap) **pvect, unsigned hash)
{
  VEC(unsigned, heap) *vect = *pvect;
  int ind;

  if (! find_in_hash_vect_1 (vect, hash, &ind))
    {
      VEC_safe_insert (unsigned, heap, vect, ind, hash);
      *pvect = vect;
    }
}

/* Compare two vinsns as rhses if possible and as vinsns otherwise.  */
bool
vinsns_correlate_as_rhses_p (vinsn_t x, vinsn_t y)
{
  /* We should have checked earlier for (X == Y).  */
  gcc_assert (x != y);

  if (VINSN_TYPE (x) != VINSN_TYPE (y))
    return false;

  if (VINSN_HASH (x) != VINSN_HASH (y))
    return false;

  if (VINSN_SEPARABLE_P (x)) 
    {
      /* Compare RHSes of VINSNs.  */
      gcc_assert (VINSN_RHS (x));
      gcc_assert (VINSN_RHS (y));

      return sel_rtx_equal_p (VINSN_RHS (x), VINSN_RHS (y));
    }
  else
    /* Compare whole insns. */
    return vinsn_equal_p (x, y);
}

/* Initialize RHS.  */
static void
init_expr (expr_t expr, vinsn_t vi, int spec, int priority, int sched_times,
	   int orig_bb_index, ds_t spec_done_ds, ds_t spec_to_check_ds,
	   VEC(unsigned, heap) *changed_on, bool was_substituted)
{
  vinsn_attach (vi);

  EXPR_VINSN (expr) = vi;
  EXPR_SPEC (expr) = spec;
  EXPR_PRIORITY (expr) = priority;
  EXPR_SCHED_TIMES (expr) = sched_times;
  EXPR_ORIG_BB_INDEX (expr) = orig_bb_index;
  EXPR_SPEC_DONE_DS (expr) = spec_done_ds;
  EXPR_SPEC_TO_CHECK_DS (expr) = spec_to_check_ds;

  if (changed_on)
    EXPR_CHANGED_ON_INSNS (expr) = changed_on;
  else
    EXPR_CHANGED_ON_INSNS (expr) = NULL;

  EXPR_WAS_SUBSTITUTED (expr) = was_substituted;
}

/* Make a copy of the rhs FROM into the rhs TO.  */
void
copy_expr (expr_t to, expr_t from)
{
  VEC(unsigned, heap) *temp;

  temp = VEC_copy (unsigned, heap, EXPR_CHANGED_ON_INSNS (from));
  init_expr (to, EXPR_VINSN (from), EXPR_SPEC (from), EXPR_PRIORITY (from),
	     EXPR_SCHED_TIMES (from), EXPR_ORIG_BB_INDEX (from),
	     EXPR_SPEC_DONE_DS (from), EXPR_SPEC_TO_CHECK_DS (from), temp,
	     EXPR_WAS_SUBSTITUTED (from));
}

/* Same, but the final expr will not ever be in av sets, so don't copy 
   "uninteresting" data such as bitmap cache.  */
void
copy_expr_onside (expr_t to, expr_t from)
{
  init_expr (to, EXPR_VINSN (from), EXPR_SPEC (from), EXPR_PRIORITY (from),
	     EXPR_SCHED_TIMES (from), 0,
	     EXPR_SPEC_DONE_DS (from), EXPR_SPEC_TO_CHECK_DS (from), NULL,
	     EXPR_WAS_SUBSTITUTED (from));
}

/* Merge bits of FROM rhs to TO rhs.  */
void
merge_expr_data (expr_t to, expr_t from)
{
  int i;
  unsigned hash;

  /* For now, we just set the spec of resulting rhs to be minimum of the specs
     of merged rhses.  */
  if (RHS_SPEC (to) > RHS_SPEC (from))
    RHS_SPEC (to) = RHS_SPEC (from);

  if (RHS_PRIORITY (to) < RHS_PRIORITY (from))
    RHS_PRIORITY (to) = RHS_PRIORITY (from);

  if (RHS_SCHED_TIMES (to) > RHS_SCHED_TIMES (from))
    RHS_SCHED_TIMES (to) = RHS_SCHED_TIMES (from);

  if (EXPR_ORIG_BB_INDEX (to) != EXPR_ORIG_BB_INDEX (from))
    EXPR_ORIG_BB_INDEX (to) = 0;

  EXPR_SPEC_DONE_DS (to) = ds_max_merge (EXPR_SPEC_DONE_DS (to),
					 EXPR_SPEC_DONE_DS (from));

  EXPR_SPEC_TO_CHECK_DS (to) |= EXPR_SPEC_TO_CHECK_DS (from);
  
  /* We keep this vector sorted.  */
  for (i = 0; 
       VEC_iterate (unsigned, EXPR_CHANGED_ON_INSNS (from), i, hash);
       i++)
    insert_in_hash_vect (&EXPR_CHANGED_ON_INSNS (to), hash);

  EXPR_WAS_SUBSTITUTED (to) |= EXPR_WAS_SUBSTITUTED (from);
}

/* Merge bits of FROM rhs to TO rhs.  Vinsns in the rhses should correlate.  */
void
merge_expr (expr_t to, expr_t from)
{
  vinsn_t to_vi = EXPR_VINSN (to);
  vinsn_t from_vi = EXPR_VINSN (from);

  gcc_assert (to_vi == from_vi
	      || vinsns_correlate_as_rhses_p (to_vi, from_vi));

  /* Make sure that speculative pattern is propagated into exprs that
     have non-speculative one.  This will provide us with consistent
     speculative bits and speculative patterns inside expr.  */
  if (EXPR_SPEC_DONE_DS (to) == 0)
    change_vinsn_in_expr (to, EXPR_VINSN (from));

  merge_expr_data (to, from);
}

/* Clear the information of this RHS.  */
void
clear_expr (rhs_t rhs)
{
  vinsn_detach (RHS_VINSN (rhs));
  RHS_VINSN (rhs) = NULL;
  VEC_free (unsigned, heap, EXPR_CHANGED_ON_INSNS (rhs));
}


/* Av set functions.  */

/* Add EXPR to SETP.  */
void
av_set_add (av_set_t *setp, expr_t expr)
{
  _list_add (setp);
  copy_expr (_AV_SET_EXPR (*setp), expr);
}

/* Remove expr pointed to by IP from the av_set.  */
void
av_set_iter_remove (av_set_iterator *ip)
{
  clear_expr (_AV_SET_EXPR (*ip->lp));
  _list_iter_remove (ip);
}

/* Search for an rhs in SET, such that it's equivalent to SOUGHT_VINSN in the
   sense of vinsns_correlate_as_rhses_p function. Return NULL if no such rhs is
   in SET was found.  */
rhs_t
av_set_lookup (av_set_t set, vinsn_t sought_vinsn)
{
  rhs_t rhs;
  av_set_iterator i;

  FOR_EACH_RHS (rhs, i, set)
    {
      vinsn_t rhs_vinsn = RHS_VINSN (rhs);

      if (rhs_vinsn == sought_vinsn
	  || vinsns_correlate_as_rhses_p (rhs_vinsn, sought_vinsn))
	return rhs;
    }

  return NULL;
}

/* Search for an rhs in SET, such that it's equivalent to SOUGHT_VINSN in the
   sense of vinsns_correlate_as_rhses_p function, but not SOUGHT_VINSN itself.
   Returns NULL if no such rhs is in SET was found.  */
rhs_t
av_set_lookup_other_equiv_rhs (av_set_t set, vinsn_t sought_vinsn)
{
  rhs_t rhs;
  av_set_iterator i;

  FOR_EACH_RHS (rhs, i, set)
    {
      vinsn_t rhs_vinsn = RHS_VINSN (rhs);

      if (rhs_vinsn == sought_vinsn)
	continue;

      if (vinsns_correlate_as_rhses_p (rhs_vinsn, sought_vinsn))
	return rhs;
    }

  return NULL;
}

/* Return true if there is an expr that correlates to VI in SET.  */
bool
av_set_is_in_p (av_set_t set, vinsn_t vi)
{
  return av_set_lookup (set, vi) != NULL;
}

/* Return a copy of SET.  */
av_set_t
av_set_copy (av_set_t set)
{
  rhs_t rhs;
  av_set_iterator i;
  av_set_t res = NULL;

  FOR_EACH_RHS (rhs, i, set)
    av_set_add (&res, rhs);

  return res;
}

/* Makes set pointed to by TO to be the union of TO and FROM.  Clear av_set
   pointed to by FROMP afterwards.  */
void
av_set_union_and_clear (av_set_t *top, av_set_t *fromp)
{
  rhs_t rhs1;
  av_set_iterator i;

  /* Delete from TOP all rhses, that present in FROMP.  */
  FOR_EACH_RHS_1 (rhs1, i, top)
    {
      rhs_t rhs2 = av_set_lookup (*fromp, RHS_VINSN (rhs1));

      if (rhs2)
	{
          merge_expr (rhs2, rhs1);
	  av_set_iter_remove (&i);
	}
    }

  /* Connect FROMP to the end of the TOP.  */
  *i.lp = *fromp;
  *fromp = NULL;
}

/* Clear av_set pointed to by SETP.  */
void
av_set_clear (av_set_t *setp)
{
  rhs_t rhs;
  av_set_iterator i;

  FOR_EACH_RHS_1 (rhs, i, setp)
    av_set_iter_remove (&i);

  gcc_assert (*setp == NULL);
}

/* Remove all the elements of SETP except for the first one.  */
void
av_set_leave_one (av_set_t *setp)
{
  av_set_clear (&_AV_SET_NEXT (*setp));
}

/* Return the N'th element of the SET.  */
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

/* Leave in AVP only those expressions, which are present in AV,
   and return it.  */
void
av_set_intersect (av_set_t *avp, av_set_t av)
{
  av_set_iterator i;
  rhs_t rhs;

  FOR_EACH_RHS_1 (rhs, i, avp)
    if (av_set_lookup (av, RHS_VINSN (rhs)) == NULL)
      av_set_iter_remove (&i);
}



/* Dependence hooks to initialize insn data.  */

static struct
{
  deps_where_t where;
  idata_t id;
  bool force_unique_p;
} deps_init_id_data;

/* Start initializing insn data.  */
static void
deps_init_id_start_insn (insn_t insn)
{
  int type;
  idata_t id;

  gcc_assert (deps_init_id_data.where == DEPS_IN_NOWHERE);

  /* Determine whether INSN could be cloned and return appropriate vinsn type.
     That clonable insns which can be separated into lhs and rhs have type SET.
     Other clonable insns have type USE.  */
  type = GET_CODE (insn);

  /* Only regular insns could be cloned.  */
  if (type == INSN)
    {
      if (!deps_init_id_data.force_unique_p)
	{
	  type = USE;

	  if (enable_schedule_as_rhs_p)
	    type = SET;
	}
    }
  else if (type == JUMP_INSN)
    {
      if (simplejump_p (insn))
	type = PC;
    }

  id = deps_init_id_data.id;

  IDATA_TYPE (id) = type;

  IDATA_REG_SETS (id) = get_clear_regset_from_pool ();
  IDATA_REG_USES (id) = get_clear_regset_from_pool ();

  deps_init_id_data.where = DEPS_IN_INSN;
}

/* Start initializing lhs data.  */
static void
deps_init_id_start_lhs (rtx lhs)
{
  gcc_assert (deps_init_id_data.where == DEPS_IN_INSN);

  gcc_assert (IDATA_LHS (deps_init_id_data.id) == NULL);

  if (IDATA_TYPE (deps_init_id_data.id) == SET)
    {
      IDATA_LHS (deps_init_id_data.id) = lhs;
      deps_init_id_data.where = DEPS_IN_LHS;
    }
}

/* Finish initializing lhs data.  */
static void
deps_init_id_finish_lhs (void)
{
  deps_init_id_data.where = DEPS_IN_INSN;
}

/* Downgrade to USE.  */
static void
deps_init_id_downgrade_to_use (void)
{
  gcc_assert (IDATA_TYPE (deps_init_id_data.id) == SET);

  IDATA_TYPE (deps_init_id_data.id) = USE;

  IDATA_LHS (deps_init_id_data.id) = NULL;
  IDATA_RHS (deps_init_id_data.id) = NULL;

  deps_init_id_data.where = DEPS_IN_INSN;
}

/* Note a set of REGNO.  */
static void
deps_init_id_note_reg_set (int regno)
{
  haifa_note_reg_set (regno);

  if (deps_init_id_data.where == DEPS_IN_RHS)
    deps_init_id_downgrade_to_use ();

  if (IDATA_TYPE (deps_init_id_data.id) != PC)
    SET_REGNO_REG_SET (IDATA_REG_SETS (deps_init_id_data.id), regno);
}

/* Note a clobber of REGNO.  */
static void
deps_init_id_note_reg_clobber (int regno)
{
  haifa_note_reg_clobber (regno);

  if (deps_init_id_data.where == DEPS_IN_RHS)
    deps_init_id_downgrade_to_use ();

  if (IDATA_TYPE (deps_init_id_data.id) != PC)
    SET_REGNO_REG_SET (IDATA_REG_SETS (deps_init_id_data.id), regno);
}

/* Note a use of REGNO.  */
static void
deps_init_id_note_reg_use (int regno)
{
  haifa_note_reg_use (regno);

  if (IDATA_TYPE (deps_init_id_data.id) != PC)
    SET_REGNO_REG_SET (IDATA_REG_USES (deps_init_id_data.id), regno);
}

/* Start initializing rhs data.  */
static void
deps_init_id_start_rhs (rtx rhs)
{
  gcc_assert (deps_init_id_data.where == DEPS_IN_INSN);

  /* And there was no sel_deps_reset_to_insn ().  */
  if (IDATA_LHS (deps_init_id_data.id) != NULL)
    {
      IDATA_RHS (deps_init_id_data.id) = rhs;
      deps_init_id_data.where = DEPS_IN_RHS;
    }
}

/* Finish initializing rhs data.  */
static void
deps_init_id_finish_rhs (void)
{
  gcc_assert (deps_init_id_data.where == DEPS_IN_RHS
	      || deps_init_id_data.where == DEPS_IN_INSN);

  deps_init_id_data.where = DEPS_IN_INSN;
}

/* Finish initializing insn data.  */
static void
deps_init_id_finish_insn (void)
{
  gcc_assert (deps_init_id_data.where == DEPS_IN_INSN);

  if (IDATA_TYPE (deps_init_id_data.id) == SET)
    {
      rtx lhs = IDATA_LHS (deps_init_id_data.id);
      rtx rhs = IDATA_RHS (deps_init_id_data.id);

      if (lhs == NULL || rhs == NULL || !lhs_and_rhs_separable_p (lhs, rhs))
	{
          /* This should be a USE, as we don't want to schedule its RHS 
             separately.  However, we still want to have them recorded
             for the purposes of substitution.  That's why we don't 
             simply call downgrade_to_use () here.  */
	  gcc_assert (IDATA_TYPE (deps_init_id_data.id) == SET);
	  gcc_assert (!lhs == !rhs);

	  IDATA_TYPE (deps_init_id_data.id) = USE;
	}
    }

  deps_init_id_data.where = DEPS_IN_NOWHERE;
}

static const struct sched_deps_info_def const_deps_init_id_sched_deps_info =
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
    NULL, /* note_mem_dep */
    NULL, /* note_dep */

    0, /* use_cselib */
    0, /* use_deps_list */
    0 /* generate_spec_deps */
  };

static struct sched_deps_info_def deps_init_id_sched_deps_info;

/* Initialize instruction data for INSN in ID.  */
static void
deps_init_id (idata_t id, insn_t insn, bool force_unique_p)
{
  struct deps _dc, *dc = &_dc;

  deps_init_id_data.where = DEPS_IN_NOWHERE;
  deps_init_id_data.id = id;
  deps_init_id_data.force_unique_p = force_unique_p;

  init_deps (dc);

  memcpy (&deps_init_id_sched_deps_info,
	  &const_deps_init_id_sched_deps_info,
	  sizeof (deps_init_id_sched_deps_info));

  if (spec_info != NULL)
    deps_init_id_sched_deps_info.generate_spec_deps = 1;

  sched_deps_info = &deps_init_id_sched_deps_info;

  deps_analyze_insn (dc, insn);

  free_deps (dc);

  deps_init_id_data.id = NULL;
}



/* Implement hooks for collecting fundamental insn properties like if insn is
   an ASM or is within a SCHED_GROUP.  */

static void init_invalid_av_set (basic_block);

/* Initialize region-scope data structures for basic blocks.  */
static void
init_global_and_expr_for_bb (basic_block bb)
{
  if (sel_bb_empty_p (bb))
    return;

  init_invalid_av_set (bb);
}

/* Data for global dependency analysis (to initialize CANT_MOVE and
   SCHED_GROUP_P).  */
static struct
{
  /* Previous insn.  */
  insn_t prev_insn;
} init_global_data;

/* Determine if INSN is in the sched_group, is an asm or should not be
   cloned.  After that initialize its expr.  */
static void
init_global_and_expr_for_insn (insn_t insn)
{
  if (LABEL_P (insn))
    return;

  if (NOTE_INSN_BASIC_BLOCK_P (insn))
    {
      init_global_data.prev_insn = NULL_RTX;
      return;
    }

  gcc_assert (INSN_P (insn));

  if (SCHED_GROUP_P (insn))
    /* Setup a sched_group.  */
    {
      insn_t prev_insn = init_global_data.prev_insn;

      if (prev_insn)
	INSN_SCHED_NEXT (prev_insn) = insn;

      init_global_data.prev_insn = insn;
    }
  else
    init_global_data.prev_insn = NULL_RTX;

  if (GET_CODE (PATTERN (insn)) == ASM_INPUT
      || asm_noperands (PATTERN (insn)) >= 0)
    /* Mark INSN as an asm.  */
    INSN_ASM_P (insn) = true;

  {
    bool force_unique_p;
    ds_t spec_done_ds;

    /* Certain instructions cannot be cloned.  */
    if (CANT_MOVE (insn)
	|| INSN_ASM_P (insn)
	|| SCHED_GROUP_P (insn)
	|| prologue_epilogue_contains (insn) 
	/* Exception handling insns are always unique.  */
	|| (flag_non_call_exceptions && can_throw_internal (insn))
	/* TRAP_IF though have an INSN code is control_flow_insn_p ().  */
	|| control_flow_insn_p (insn))
      force_unique_p = true;
    else
      force_unique_p = false;

    if (targetm.sched.get_insn_spec_ds)
      {
	spec_done_ds = targetm.sched.get_insn_spec_ds (insn);
	spec_done_ds = ds_get_max_dep_weak (spec_done_ds);
      }
    else
      spec_done_ds = 0;

    /* Initialize INSN's expr.  */
    init_expr (INSN_EXPR (insn), vinsn_create (insn, force_unique_p), 0,
	       INSN_PRIORITY (insn), 0, BLOCK_NUM (insn), spec_done_ds, 0,
	       NULL, false);
  }

  INSN_ANALYZED_DEPS (insn) = BITMAP_ALLOC (NULL);
  INSN_FOUND_DEPS (insn) = BITMAP_ALLOC (NULL);
}

static void extend_insn (void);

/* Scan the region and initialize instruction data.  */
void
sel_init_global_and_expr (bb_vec_t bbs)
{
  {
    /* ??? It would be nice to implement push / pop scheme for sched_infos.  */
    const struct sched_scan_info_def ssi =
      {
	NULL, /* extend_bb */
	init_global_and_expr_for_bb, /* init_bb */
	extend_insn, /* extend_insn */
	init_global_and_expr_for_insn /* init_insn */
      };

    sched_scan (&ssi, bbs, NULL, NULL, NULL);
  }
}

/* Finalize region-scope data structures for basic blocks.  */
static void
finish_global_and_expr_for_bb (basic_block bb)
{
  av_set_clear (&BB_AV_SET (bb));
  BB_AV_LEVEL (bb) = 0;
}

/* Finalize INSN's data.  */
static void
finish_global_and_expr_insn (insn_t insn)
{
  if (LABEL_P (insn) || NOTE_INSN_BASIC_BLOCK_P (insn))
    return;

  gcc_assert (INSN_P (insn));

  if (INSN_LUID (insn) > 0)
    {
      BITMAP_FREE (INSN_ANALYZED_DEPS (insn));
      BITMAP_FREE (INSN_FOUND_DEPS (insn));

      INSN_WS_LEVEL (insn) = 0;

      gcc_assert (VINSN_COUNT (INSN_VINSN (insn)) == 1);

      clear_expr (INSN_EXPR (insn));
    }
}

static void finish_insns (void);

/* Finalize per instruction data for the whole region.  */
void
sel_finish_global_and_expr (void)
{
  {
    bb_vec_t bbs;
    int i;

    bbs = VEC_alloc (basic_block, heap, current_nr_blocks);

    for (i = 0; i < current_nr_blocks; i++)
      VEC_quick_push (basic_block, bbs, BASIC_BLOCK (BB_TO_BLOCK (i)));

    /* Clear AV_SETs and INSN_EXPRs.  */
    {
      const struct sched_scan_info_def ssi =
	{
	  NULL, /* extend_bb */
	  finish_global_and_expr_for_bb, /* init_bb */
	  NULL, /* extend_insn */
	  finish_global_and_expr_insn /* init_insn */
	};

      sched_scan (&ssi, bbs, NULL, NULL, NULL);
    }

    VEC_free (basic_block, heap, bbs);
  }

  finish_insns ();
}

/* In the below hooks, we merely calculate whether or not a dependence 
   exists, and in what part of insn.  However, we will need more data 
   when we'll start caching dependence requests.  */

/* Container to hold information for dependency analysis.  */
static struct _has_dependence_data
{
  deps_t dc;

  /* A variable to track which part of rtx we are scanning in
     sched-deps.c: sched_analyze_insn ().  */
  deps_where_t where;

  /* Current producer.  */
  insn_t pro;

  /* Current consumer.  */
  vinsn_t con;

  /* Is SEL_DEPS_HAS_DEP_P[DEPS_IN_X] is true, then X has a dependence.
     X is from { INSN, LHS, RHS }.  */
  ds_t has_dep_p[DEPS_IN_NOWHERE];
} has_dependence_data;

/* Start analyzing dependencies of INSN.  */
static void
has_dependence_start_insn (insn_t insn ATTRIBUTE_UNUSED)
{
  gcc_assert (has_dependence_data.where == DEPS_IN_NOWHERE);

  has_dependence_data.where = DEPS_IN_INSN;
}

/* Finish analyzing dependencies of an insn.  */
static void
has_dependence_finish_insn (void)
{
  gcc_assert (has_dependence_data.where == DEPS_IN_INSN);

  has_dependence_data.where = DEPS_IN_NOWHERE;
}

/* Start analyzing dependencies of LHS.  */
static void
has_dependence_start_lhs (rtx lhs ATTRIBUTE_UNUSED)
{
  gcc_assert (has_dependence_data.where == DEPS_IN_INSN);

  if (VINSN_LHS (has_dependence_data.con) != NULL)
    has_dependence_data.where = DEPS_IN_LHS;
}

/* Finish analyzing dependencies of an lhs.  */
static void
has_dependence_finish_lhs (void)
{
  has_dependence_data.where = DEPS_IN_INSN;
}

/* Start analyzing dependencies of RHS.  */
static void
has_dependence_start_rhs (rtx rhs ATTRIBUTE_UNUSED)
{
  gcc_assert (has_dependence_data.where == DEPS_IN_INSN);

  if (VINSN_RHS (has_dependence_data.con) != NULL)
    has_dependence_data.where = DEPS_IN_RHS;
}

/* Start analyzing dependencies of an rhs.  */
static void
has_dependence_finish_rhs (void)
{
  gcc_assert (has_dependence_data.where == DEPS_IN_RHS
	      || has_dependence_data.where == DEPS_IN_INSN);

  has_dependence_data.where = DEPS_IN_INSN;
}

/* Note a set of REGNO.  */
static void
has_dependence_note_reg_set (int regno)
{
  struct deps_reg *reg_last = &has_dependence_data.dc->reg_last[regno];

  if (!sched_insns_conditions_mutex_p (has_dependence_data.pro,
				       VINSN_INSN
				       (has_dependence_data.con)))
    {
      ds_t *dsp = &has_dependence_data.has_dep_p[has_dependence_data.where];

      if (reg_last->sets != NULL
	  || reg_last->clobbers != NULL)
	*dsp = (*dsp & ~SPECULATIVE) | DEP_OUTPUT;

      if (reg_last->uses)
	*dsp = (*dsp & ~SPECULATIVE) | DEP_ANTI;
    }
}

/* Note a clobber of REGNO.  */
static void
has_dependence_note_reg_clobber (int regno)
{
  struct deps_reg *reg_last = &has_dependence_data.dc->reg_last[regno];

  if (!sched_insns_conditions_mutex_p (has_dependence_data.pro,
				       VINSN_INSN
				       (has_dependence_data.con)))
    {
      ds_t *dsp = &has_dependence_data.has_dep_p[has_dependence_data.where];

      if (reg_last->sets)
	*dsp = (*dsp & ~SPECULATIVE) | DEP_OUTPUT;
	
      if (reg_last->uses)
	*dsp = (*dsp & ~SPECULATIVE) | DEP_ANTI;
    }
}

/* Note a use of REGNO.  */
static void
has_dependence_note_reg_use (int regno)
{
  struct deps_reg *reg_last = &has_dependence_data.dc->reg_last[regno];

  if (!sched_insns_conditions_mutex_p (has_dependence_data.pro,
				       VINSN_INSN
				       (has_dependence_data.con)))
    {
      ds_t *dsp = &has_dependence_data.has_dep_p[has_dependence_data.where];

      if (reg_last->sets)
	*dsp = (*dsp & ~SPECULATIVE) | DEP_TRUE;

      if (reg_last->clobbers)
	*dsp = (*dsp & ~SPECULATIVE) | DEP_ANTI;

      /* Handle BE_IN_SPEC.  */
      if (reg_last->uses)
	{
	  ds_t pro_spec_checked_ds;

	  pro_spec_checked_ds = INSN_SPEC_CHECKED_DS (has_dependence_data.pro);
	  pro_spec_checked_ds = ds_get_max_dep_weak (pro_spec_checked_ds);

	  if (pro_spec_checked_ds != 0)
	    /* Merge BE_IN_SPEC bits into *DSP.  */
	    *dsp = ds_full_merge (*dsp, pro_spec_checked_ds,
				  NULL_RTX, NULL_RTX);
	}
    }
}

/* Note a memory dependence.  */
static void
has_dependence_note_mem_dep (rtx mem ATTRIBUTE_UNUSED,
			     rtx pending_mem ATTRIBUTE_UNUSED,
			     insn_t pending_insn ATTRIBUTE_UNUSED,
			     ds_t ds ATTRIBUTE_UNUSED)
{
  if (!sched_insns_conditions_mutex_p (has_dependence_data.pro,
				       VINSN_INSN (has_dependence_data.con)))
    {
      ds_t *dsp = &has_dependence_data.has_dep_p[has_dependence_data.where];

      *dsp = ds_full_merge (ds, *dsp, pending_mem, mem);
    }
}

/* Note a dependence.  */
static void
has_dependence_note_dep (insn_t pro ATTRIBUTE_UNUSED,
			 ds_t ds ATTRIBUTE_UNUSED)
{
  if (!sched_insns_conditions_mutex_p (has_dependence_data.pro,
				       VINSN_INSN (has_dependence_data.con)))
    {
      ds_t *dsp = &has_dependence_data.has_dep_p[has_dependence_data.where];

      *dsp = ds_full_merge (ds, *dsp, NULL_RTX, NULL_RTX);
    }
}

/* Mark the insn as having a hard dependence that prevents speculation.  */
void
sel_mark_hard_insn (rtx insn)
{
  int i;

  /* Only work when we're in has_dependence_p mode.
     ??? This is a hack, this should actually be a hook.  */
  if (!has_dependence_data.dc || !has_dependence_data.pro)
    return;

  gcc_assert (insn == VINSN_INSN (has_dependence_data.con));
  gcc_assert (has_dependence_data.where == DEPS_IN_INSN);

  for (i = 0; i < DEPS_IN_NOWHERE; i++)
    has_dependence_data.has_dep_p[i] &= ~SPECULATIVE;
}

static const struct sched_deps_info_def const_has_dependence_sched_deps_info =
  {
    NULL,

    has_dependence_start_insn,
    has_dependence_finish_insn,
    NULL, /* start_x */
    NULL, /* finish_x */
    has_dependence_start_lhs,
    has_dependence_finish_lhs,
    has_dependence_start_rhs,
    has_dependence_finish_rhs,
    has_dependence_note_reg_set,
    has_dependence_note_reg_clobber,
    has_dependence_note_reg_use,
    has_dependence_note_mem_dep,
    has_dependence_note_dep,

    0, /* use_cselib */
    0, /* use_deps_list */
    0 /* generate_spec_deps */
  };

static struct sched_deps_info_def has_dependence_sched_deps_info;

static void
setup_has_dependence_sched_deps_info (void)
{
  memcpy (&has_dependence_sched_deps_info,
	  &const_has_dependence_sched_deps_info,
	  sizeof (has_dependence_sched_deps_info));

  if (spec_info != NULL)
    has_dependence_sched_deps_info.generate_spec_deps = 1;

  sched_deps_info = &has_dependence_sched_deps_info;
}

void
sel_clear_has_dependence (void)
{
  int i;

  for (i = 0; i < DEPS_IN_NOWHERE; i++)
    has_dependence_data.has_dep_p[i] = 0;
}

/* Return nonzero if RHS has is dependent upon PRED.  */
ds_t
has_dependence_p (rhs_t rhs, insn_t pred, ds_t **has_dep_pp)
{
  struct deps _dc;
  int i;
  ds_t ds;

  if (INSN_SIMPLEJUMP_P (pred))
    /* Unconditional jump is just a transfer of control flow.
       Ignore it.  */
    return false;

  has_dependence_data.dc = &_dc;
  has_dependence_data.pro = NULL;

  init_deps (has_dependence_data.dc);

  /* Initialize empty dep context with information about PRED.  */
  advance_deps_context (has_dependence_data.dc, pred);

  has_dependence_data.where = DEPS_IN_NOWHERE;

  has_dependence_data.pro = pred;

  has_dependence_data.con = RHS_VINSN (rhs);

  sel_clear_has_dependence ();

  /* Now catch all dependencies that would be generated between PRED and
     INSN.  */
  setup_has_dependence_sched_deps_info ();
  deps_analyze_insn (has_dependence_data.dc, RHS_INSN (rhs));

  free_deps (has_dependence_data.dc);
  has_dependence_data.dc = NULL;

  *has_dep_pp = has_dependence_data.has_dep_p;

  ds = 0;

  for (i = 0; i < DEPS_IN_NOWHERE; i++)
    ds = ds_full_merge (ds, has_dependence_data.has_dep_p[i],
			NULL_RTX, NULL_RTX);

  return ds;
}

/* Dependence hooks implementation that checks dependence latency constraints 
   on the insns being scheduled.  The entry point for these routines is 
   tick_check_p predicate.  */ 

static struct
{
  /* An rhs we are currently checking.  */
  rhs_t rhs;

  /* A minimal cycle for its scheduling.  */
  int cycle;

  /* Whether we have seen a true dependence while checking.  */
  bool seen_true_dep_p;
} tick_check_data;

/* Update minimal scheduling cycle for tick_check_insn given that it depends
   on PRO with status DS and weight DW.  */
static void
tick_check_dep_with_dw (insn_t pro_insn, ds_t ds, dw_t dw)
{
  rhs_t con_rhs = tick_check_data.rhs;
  insn_t con_insn = RHS_INSN (con_rhs);

  if (con_insn != pro_insn)
    {
      enum reg_note dt;
      int tick;

      if (/* PROducer was removed from above due to pipelining.  See PR8.  */
	  !INSN_IN_STREAM_P (pro_insn)
	  /* Or PROducer was originally on the next iteration regarding the
	     CONsumer.  */
	  || (INSN_SCHED_TIMES (pro_insn)
	      - RHS_SCHED_TIMES (con_rhs)) > 1)
	/* Don't count this dependence.  */
	{
	  /* ??? This assert fails on a large testcase.  It is not clear
	     to me if the assert is right so defer debugging until a smaller
	     testcase is avalailable.  */
	  gcc_assert (1 || pipelining_p);

	  return;
	}

      dt = ds_to_dt (ds);
      if (dt == REG_DEP_TRUE)
        tick_check_data.seen_true_dep_p = true;

      gcc_assert (INSN_SCHED_CYCLE (pro_insn) > 0);

      {
	dep_def _dep, *dep = &_dep;

	init_dep (dep, pro_insn, con_insn, dt);

	tick = INSN_SCHED_CYCLE (pro_insn) + dep_cost_1 (dep, dw);
      }

      /* When there are several kinds of dependencies between pro and con,
         only REG_DEP_TRUE should be taken into account.  */
      if (tick > tick_check_data.cycle
	  && (dt == REG_DEP_TRUE || !tick_check_data.seen_true_dep_p))
	tick_check_data.cycle = tick;
    }
}

/* An implementation of note_dep hook.  */
static void
tick_check_note_dep (insn_t pro, ds_t ds)
{
  tick_check_dep_with_dw (pro, ds, 0);
}

/* An implementation of note_mem_dep hook.  */
static void
tick_check_note_mem_dep (rtx mem1, rtx mem2, insn_t pro, ds_t ds)
{
  dw_t dw;

  dw = (ds_to_dt (ds) == REG_DEP_TRUE
        ? estimate_dep_weak (mem1, mem2)
        : 0);

  tick_check_dep_with_dw (pro, ds, dw);
}

static struct sched_deps_info_def _tick_check_sched_deps_info =
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
tick_check_p (rhs_t rhs, deps_t dc_orig, fence_t fence)
{
  struct deps _dc, *dc = &_dc;

  /* Initialize variables.  */
  tick_check_data.rhs = rhs;
  tick_check_data.cycle = 0;
  tick_check_data.seen_true_dep_p = false;

  /* Calculate TICK_CHECK_CYCLE.  */
  copy_deps_context (dc, dc_orig);

  sched_deps_info = &_tick_check_sched_deps_info;
  deps_analyze_insn (dc, RHS_INSN (rhs));

  free_deps (dc);

  return FENCE_CYCLE (fence) >= tick_check_data.cycle;
}


/* Functions to work with insns.  */

/* Returns true if LHS of INSN is the same as DEST of an insn
   being moved.  */
bool
lhs_of_insn_equals_to_dest_p (insn_t insn, rtx dest)
{
  rtx lhs = INSN_LHS (insn);

  if (lhs == NULL || dest == NULL)
    return false;
  
  return rtx_equal_p (lhs, dest);
}

/* Returns whether INSN_RTX is valid in terms of target architecture.
   Don't use this function inside gcc_assert () because it has side effects:
   e.g. it initializes INSN_CODE (INSN_RTX).  */
bool
insn_rtx_valid (rtx insn_rtx)
{
  /* Reset the INSN_CODE.  After register replacement it might have became
     a different insn.  */
  INSN_CODE (insn_rtx) = -1;

  if (recog_memoized (insn_rtx) >= 0)
    {
      extract_insn (insn_rtx);
      return constrain_operands (reload_completed) != 0;
    }
  else
    return false;
}

/* Return s_i_d entry of INSN.  Callable from debugger.  */
sel_insn_data_def
insn_sid (insn_t insn)
{
  return *SID (insn);
}

/* True when INSN is a speculative check.  We can tell this by looking
   at the data structures of the selective scheduler, not by examining
   the pattern.  */
bool
sel_insn_is_speculation_check (rtx insn)
{
  return s_i_d && !! INSN_SPEC_CHECKED_DS (insn);
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
      if ((REG_P (INSN_LHS (insn)) 
	   && (REG_P (INSN_RHS (insn)) 
               /* Not supported atm.  
	       || GET_CODE (INSN_RHS (insn)) == CONST_INT  */)))
	  return true;             
    }
  return false;
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
    if (sel_num_cfg_preds_gt_1 (succ))
      return true;

  return false;
}

/* Return 'true' if INSN is the only one in its basic block.  */
static bool
insn_is_the_only_one_in_bb_p (insn_t insn)
{
  return sel_bb_head_p (insn) && sel_bb_end_p (insn);
}

/* Rip-off INSN from the insn stream.  */
void
sel_remove_insn (insn_t insn)
{
  basic_block bb = BLOCK_FOR_INSN (insn);

  gcc_assert (INSN_IN_STREAM_P (insn));
  remove_insn (insn);

  /* It is necessary to null this fields before calling add_insn ().  */
  PREV_INSN (insn) = NULL_RTX;
  NEXT_INSN (insn) = NULL_RTX;

  clear_expr (INSN_EXPR (insn));

  if (sel_bb_empty_p (bb))
    /* Get rid of empty BB.  */
    {
      free_data_sets (bb);

      if (single_succ_p (bb))
	{
	  basic_block succ_bb;
	  bool rescan_p;
	  basic_block pred_bb;

	  succ_bb = single_succ (bb);
	  rescan_p = true;
	  pred_bb = NULL;

	  /* Redirect all non-fallthru edges to the next bb.  */
	  while (rescan_p)
	    {
	      edge e;
	      edge_iterator ei;

	      rescan_p = false;

	      FOR_EACH_EDGE (e, ei, bb->preds)
		{
		  pred_bb = e->src;

		  if (!(e->flags & EDGE_FALLTHRU))
		    {
		      sel_redirect_edge_and_branch (e, succ_bb);
		      rescan_p = true;
		      break;
		    }
		}
	    }

	  /* If it is possible - merge BB with its predecessor.  */
	  if (can_merge_blocks_p (bb->prev_bb, bb))
	    sel_merge_blocks (bb->prev_bb, bb);
	  else
	    /* Otherwise this is a block without fallthru predecessor.
	       Just delete it.  */
	    {
	      gcc_assert (pred_bb != NULL);

	      move_bb_info (pred_bb, bb);
	      remove_empty_bb (bb, true);
	    }
	}
      else
	/* Do not delete BB if it has more than one successor.
	   That can occur when we moving a jump.  */
	{
	  gcc_assert (can_merge_blocks_p (bb->prev_bb, bb));

	  sel_merge_blocks (bb->prev_bb, bb);
	}
    }
}

/* Estimate number of the insns in BB.  */
static int
sel_estimate_number_of_insns (basic_block bb)
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
sel_luid_for_non_insn (rtx x)
{
  gcc_assert (NOTE_P (x) || LABEL_P (x));

  return -1;
}

/* Return seqno of the only predecessor of INSN.  */
static int
get_seqno_of_a_pred (insn_t insn)
{
  int seqno;

  gcc_assert (INSN_SIMPLEJUMP_P (insn));

  if (!sel_bb_head_p (insn))
    seqno = INSN_SEQNO (PREV_INSN (insn));
  else
    {
      basic_block bb = BLOCK_FOR_INSN (insn);

      if (single_pred_p (bb)
	  && !in_current_region_p (single_pred (bb)))
	/* We can have preds outside a region when splitting edges
	   for pipelining of an outer loop.  Use succ instead.  */
	{
	  insn_t succ;

	  gcc_assert (flag_sel_sched_pipelining_outer_loops
		      && current_loop_nest);

	  succ = cfg_succ_1 (insn, (SUCCS_NORMAL | SUCCS_SKIP_TO_LOOP_EXITS));
	  gcc_assert (succ != NULL);

	  seqno = INSN_SEQNO (succ);
	}
      else
	{
	  insn_t *preds;
	  int n;

	  cfg_preds (BLOCK_FOR_INSN (insn), &preds, &n);
	  gcc_assert (n == 1);

	  seqno = INSN_SEQNO (preds[0]);
              
	  free (preds);
	}
    }

  return seqno;
}


/* Extend pass-scope data structures for basic blocks.  */
void
sel_extend_global_bb_info (void)
{
  VEC_safe_grow_cleared (sel_global_bb_info_def, heap, sel_global_bb_info,
			 last_basic_block);
}

/* Extend region-scope data structures for basic blocks.  */
static void
extend_region_bb_info (void)
{
  VEC_safe_grow_cleared (sel_region_bb_info_def, heap, sel_region_bb_info,
			 last_basic_block);
}

/* Extend all data structures to fit for all basic blocks.  */
static void
extend_bb_info (void)
{
  sel_extend_global_bb_info ();
  extend_region_bb_info ();
}

/* Finalize pass-scope data structures for basic blocks.  */
void
sel_finish_global_bb_info (void)
{
  VEC_free (sel_global_bb_info_def, heap, sel_global_bb_info);
}

/* Finalize region-scope data structures for basic blocks.  */
static void
finish_region_bb_info (void)
{
  VEC_free (sel_region_bb_info_def, heap, sel_region_bb_info);
}


/* Data for each insn in current region.  */
VEC (sel_insn_data_def, heap) *s_i_d = NULL;

static insn_vec_t new_insns = NULL;

/* This variable is used to ensure that no insns will be emitted by
   outer-world functions like redirect_edge_and_branch ().  */
static bool can_add_insns_p = true;

/* The same as the previous flag except that notes are allowed 
   to be emitted.  
   FIXME: avoid this dependency between files.  */
bool can_add_real_insns_p = true;

/* Extend data structures for insns from current region.  */
static void
extend_insn (void)
{
  sched_extend_target ();
  sched_deps_init (false);

  /* Extend data structures for insns from current region.  */
  VEC_safe_grow_cleared (sel_insn_data_def, heap, s_i_d,
			 sched_max_luid);
}

/* Finalize data structures for insns from current region.  */
static void
finish_insns (void)
{
  VEC_free (sel_insn_data_def, heap, s_i_d);
  deps_finish_d_i_d ();
}

/* An implementation of RTL_HOOKS_INSN_ADDED hook.  The hook is used for 
   initializing data structures when new insn is emitted.
   This hook remembers all relevant instuctions which can be initialized later
   with the call to sel_init_new_insns ().  */
static void
sel_rtl_insn_added (insn_t insn)
{
  gcc_assert (can_add_insns_p
	      && (!INSN_P (insn) || can_add_real_insns_p));

  if (INSN_P (insn)
      && INSN_IN_STREAM_P (insn)
      && insn_is_the_only_one_in_bb_p (insn))
    {
      extend_bb_info ();
      init_invalid_data_sets (BLOCK_FOR_INSN (insn));
    }

  if (!INSN_P (insn) || insn_init.what == INSN_INIT_WHAT_INSN_RTX)
    return;

  /* Initialize a bit later because something (e.g. CFG) is not
     consistent yet.  These insns will be initialized when
     sel_init_new_insns () is called.  */
  VEC_safe_push (rtx, heap, new_insns, insn);
}

/* Save original RTL hooks here.  */
static struct rtl_hooks orig_rtl_hooks;

/* Redefine RTL hooks so we can catch the moment of creating an insn.  */
#undef RTL_HOOKS_INSN_ADDED
#define RTL_HOOKS_INSN_ADDED sel_rtl_insn_added
static const struct rtl_hooks sel_rtl_hooks = RTL_HOOKS_INITIALIZER;

void
sel_register_rtl_hooks (void)
{
  orig_rtl_hooks = rtl_hooks;
  rtl_hooks = sel_rtl_hooks;
}

void
sel_unregister_rtl_hooks (void)
{
  rtl_hooks = orig_rtl_hooks;
}

/* A proxy to pass initialization data to init_insn ().  */
static sel_insn_data_def _insn_init_ssid;
static sel_insn_data_t insn_init_ssid = &_insn_init_ssid;

/* If true create a new vinsn.  Otherwise use the one from EXPR.  */
static bool insn_init_create_new_vinsn_p;

/* Set all necessary data for initialization of the new insn[s].  */
static void
set_insn_init (expr_t expr, vinsn_t vi, int seqno)
{
  expr_t x = &insn_init_ssid->_expr;

  copy_expr (x, expr);

  if (vi != NULL)
    {
      insn_init_create_new_vinsn_p = false;
      change_vinsn_in_expr (x, vi);
    }
  else
    insn_init_create_new_vinsn_p = true;

  insn_init_ssid->seqno = seqno;
}

/* Init data for INSN.  */
static void
init_insn (insn_t insn)
{
  expr_t expr;
  expr_t x;
  sel_insn_data_t ssid = insn_init_ssid;

  /* The fields mentioned below are special and hence are not being
     propagated to the new insns.  */
  gcc_assert (!ssid->asm_p && ssid->sched_next == NULL
	      && !ssid->after_stall_p && ssid->sched_cycle == 0);

  gcc_assert (INSN_P (insn) && INSN_LUID (insn) > 0);

  expr = INSN_EXPR (insn);
  x = &ssid->_expr;

  copy_expr (expr, x);

  EXPR_ORIG_BB_INDEX (expr) = BLOCK_NUM (insn);

  if (insn_init_create_new_vinsn_p)
    change_vinsn_in_expr (expr, vinsn_create (insn, init_insn_force_unique_p));

  INSN_SEQNO (insn) = ssid->seqno;

  INSN_ANALYZED_DEPS (insn) = BITMAP_ALLOC (NULL);
  INSN_FOUND_DEPS (insn) = BITMAP_ALLOC (NULL);
}

/* This is used to initialize spurious jumps generated by
   sel_redirect_edge ().  */
static void
init_simplejump (insn_t insn)
{
  init_expr (INSN_EXPR (insn), vinsn_create (insn, false), 0, 0, 0, 
             0, 0, 0, NULL, false);

  INSN_SEQNO (insn) = get_seqno_of_a_pred (insn);

  INSN_ANALYZED_DEPS (insn) = BITMAP_ALLOC (NULL);
  INSN_FOUND_DEPS (insn) = BITMAP_ALLOC (NULL);
}

/* Perform deferred initialization of insns.  This is used to process 
   a new jump that may be created by redirect_edge.  */
void
sel_init_new_insns (void)
{
  int todo = insn_init.todo;

  if (todo & INSN_INIT_TODO_LUID)
    sched_init_luids (NULL, NULL, new_insns, NULL);

  if (todo & INSN_INIT_TODO_SSID)
    {
      const struct sched_scan_info_def ssi =
	{
	  NULL, /* extend_bb */
	  NULL, /* init_bb */
	  extend_insn, /* extend_insn */
	  init_insn /* init_insn */
	};

      sched_scan (&ssi, NULL, NULL, new_insns, NULL);

      clear_expr (&insn_init_ssid->_expr);
    }

  if (todo & INSN_INIT_TODO_SIMPLEJUMP)
    {
      const struct sched_scan_info_def ssi =
	{
	  NULL, /* extend_bb */
	  NULL, /* init_bb */
	  extend_insn, /* extend_insn */
	  init_simplejump /* init_insn */
	};

      sched_scan (&ssi, NULL, NULL, new_insns, NULL);
    }

#ifdef ENABLE_CHECKING
  /* Check that all insns were emitted to the current_region.  */
  {
    unsigned i;
    insn_t insn;
    int current_region = CONTAINING_RGN (BB_TO_BLOCK (0));

    for (i = 0; VEC_iterate (rtx, new_insns, i, insn); i++)
      gcc_assert (CONTAINING_RGN (BLOCK_NUM (insn))
		  == current_region);
  }
#endif

  VEC_truncate (rtx, new_insns, 0);
}

/* Finalize new_insns data.  */
void
sel_finish_new_insns (void)
{
  gcc_assert (VEC_empty (rtx, new_insns));

  VEC_free (rtx, heap, new_insns);
}

/* Return the cost of VINSN as estimated by DFA.  This function properly
   handles ASMs, USEs etc.  */
int
vinsn_dfa_cost (vinsn_t vinsn, fence_t fence)
{
  rtx insn = VINSN_INSN (vinsn);

  if (recog_memoized (insn) < 0)
    {
      if (!FENCE_STARTS_CYCLE_P (fence) && VINSN_UNIQUE_P (vinsn)
	  && INSN_ASM_P (insn))
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

/* Init BB_LV_SET of BB from a global_live_at_start set of BB.
   NOTE: We do need to detach register live info from bb because we
   use those regsets as BB_LV_SETs.  */
static void
init_lv_set (basic_block bb)
{
  gcc_assert (!BB_LV_SET_VALID_P (bb));

  if (sel_bb_empty_p (bb))
    return;

  BB_LV_SET (bb) = get_regset_from_pool ();
  COPY_REG_SET (BB_LV_SET (bb), df_get_live_in (bb));
  BB_LV_SET_VALID_P (bb) = true;
}

/* Initialize lv set of all bb headers.  */
void
init_lv_sets (void)
{
  basic_block bb;

  /* Initialize of LV sets.  */
  FOR_EACH_BB (bb)
    init_lv_set (bb);

  /* Don't forget EXIT_BLOCK.  */
  init_lv_set (EXIT_BLOCK_PTR);
}

/* Release lv set of HEAD.  */
static void
free_lv_set (basic_block bb)
{
  gcc_assert (BB_LV_SET (bb) != NULL);

  return_regset_to_pool (BB_LV_SET (bb));
  BB_LV_SET (bb) = NULL;
  BB_LV_SET_VALID_P (bb) = false;
}

/* Finalize lv sets of all bb headers.  */
void
free_lv_sets (void)
{
  basic_block bb;

  /* Don't forget EXIT_BLOCK.  */
  free_lv_set (EXIT_BLOCK_PTR);

  /* Free LV sets.  */
  FOR_EACH_BB (bb)
    if (!sel_bb_empty_p (bb))
      free_lv_set (bb);
}

/* Initialize an invalid LV_SET for BB.
   This set will be updated next time compute_live () process BB.  */
static void
init_invalid_lv_set (basic_block bb)
{
  gcc_assert (BB_LV_SET (bb) == NULL
	      && BB_LV_SET_VALID_P (bb) == false);

  BB_LV_SET (bb) = get_regset_from_pool ();
}

/* Initialize an invalid AV_SET for BB.
   This set will be updated next time compute_av () process BB.  */
static void
init_invalid_av_set (basic_block bb)
{
  gcc_assert (BB_AV_LEVEL (bb) == 0
	      && BB_AV_SET (bb) == NULL);

  BB_AV_LEVEL (bb) = -1;
}

/* Initialize invalid data sets for INSN.
   These sets will be updated next time update_data_sets () process INSN.  */
static void
init_invalid_data_sets (basic_block bb)
{
  init_invalid_lv_set (bb);
  init_invalid_av_set (bb);
}

/* Free av set of BB.  */
static void
free_av_set (basic_block bb)
{
  av_set_clear (&BB_AV_SET (bb));
  BB_AV_LEVEL (bb) = 0;
}

/* Free data sets of BB.  */
static void
free_data_sets (basic_block bb)
{
  free_lv_set (bb);
  free_av_set (bb);
}

/* Exchange lv sets of TO and FROM.  */
static void
exchange_lv_sets (basic_block to, basic_block from)
{
  {
    regset to_lv_set = BB_LV_SET (to);

    BB_LV_SET (to) = BB_LV_SET (from);
    BB_LV_SET (from) = to_lv_set;
  }

  {
    bool to_lv_set_valid_p = BB_LV_SET_VALID_P (to);

    BB_LV_SET_VALID_P (to) = BB_LV_SET_VALID_P (from);
    BB_LV_SET_VALID_P (from) = to_lv_set_valid_p;
  }
}


/* Exchange av sets of TO and FROM.  */
static void
exchange_av_sets (basic_block to, basic_block from)
{
  {
    av_set_t to_av_set = BB_AV_SET (to);

    BB_AV_SET (to) = BB_AV_SET (from);
    BB_AV_SET (from) = to_av_set;
  }

  {
    int to_av_level = BB_AV_LEVEL (to);

    BB_AV_LEVEL (to) = BB_AV_LEVEL (from);
    BB_AV_LEVEL (from) = to_av_level;
  }
}

/* Exchange data sets of TO and FROM.  */
static void
exchange_data_sets (basic_block to, basic_block from)
{
  exchange_lv_sets (to, from);
  exchange_av_sets (to, from);
}

av_set_t
get_av_set (insn_t insn)
{
  av_set_t av_set;

  gcc_assert (AV_SET_VALID_P (insn));

  if (sel_bb_head_p (insn))
    av_set = BB_AV_SET (BLOCK_FOR_INSN (insn));
  else
    av_set = NULL;

  return av_set;
}

/* Implementation of AV_LEVEL () macro.  Return AV_LEVEL () of INSN.  */
int
get_av_level (insn_t insn)
{
  int av_level;

  gcc_assert (INSN_P (insn));

  if (sel_bb_head_p (insn))
    av_level = BB_AV_LEVEL (BLOCK_FOR_INSN (insn));
  else
    av_level = INSN_WS_LEVEL (insn);

  return av_level;
}



/* Variables to work with control-flow graph.  */

/* The basic block that already has been processed by the sched_data_update (),
   but hasn't been in sel_add_or_remove_bb () yet.  */
static VEC (basic_block, heap) *last_added_blocks = NULL;

/* Functions to work with control-flow graph.  */

/* Return basic block note of BB.  */
insn_t
sel_bb_head (basic_block bb)
{
  insn_t head;

  if (bb == EXIT_BLOCK_PTR)
    {
      gcc_assert (exit_insn != NULL_RTX);
      head = exit_insn;
    }
  else
    {
      insn_t note;

      note = bb_note (bb);
      head = next_nonnote_insn (note);

      if (head && BLOCK_FOR_INSN (head) != bb)
	head = NULL_RTX;
    }

  return head;
}

/* Return true if INSN is a basic block header.  */
bool
sel_bb_head_p (insn_t insn)
{
  return sel_bb_head (BLOCK_FOR_INSN (insn)) == insn;
}

/* Return last insn of BB.  */
insn_t
sel_bb_end (basic_block bb)
{
  if (sel_bb_empty_p (bb))
    return NULL_RTX;

  gcc_assert (bb != EXIT_BLOCK_PTR);

  return BB_END (bb);
}

/* Return true if INSN is the last insn in its basic block.  */
bool
sel_bb_end_p (insn_t insn)
{
  return insn == sel_bb_end (BLOCK_FOR_INSN (insn));
}

/* Return true if BB consist of single NOTE_INSN_BASIC_BLOCK.  */
bool
sel_bb_empty_p (basic_block bb)
{
  return sel_bb_head (bb) == NULL;
}

/* True when BB belongs to the current scheduling region.  */
bool
in_current_region_p (basic_block bb)
{
  if (bb->index < NUM_FIXED_BLOCKS)
    return false;

  return CONTAINING_RGN (bb->index) == CONTAINING_RGN (BB_TO_BLOCK (0));
}

/* Remove all notes from BB.  */
static void
init_bb (basic_block bb)
{
  remove_notes (bb_note (bb), BB_END (bb));
  BB_NOTE_LIST (bb) = note_list;
}

void
sel_init_bbs (bb_vec_t bbs, basic_block bb)
{
  const struct sched_scan_info_def ssi =
    {
      extend_bb_info, /* extend_bb */
      init_bb, /* init_bb */
      NULL, /* extend_insn */
      NULL /* init_insn */
    };

  sched_scan (&ssi, bbs, bb, new_insns, NULL);
}

/* Restore other notes for the whole region.  */
static void
sel_restore_other_notes (void)
{
  int bb;

  for (bb = 0; bb < current_nr_blocks; bb++)
    {
      basic_block first, last;

      first = EBB_FIRST_BB (bb);
      last = EBB_LAST_BB (bb)->next_bb;

      do
	{
	  note_list = BB_NOTE_LIST (first);
	  restore_other_notes (NULL, first);
	  BB_NOTE_LIST (first) = NULL_RTX;

          first = first->next_bb;
	}
      while (first != last);
    }
}

static void sel_remove_loop_preheader (void);

/* Free per-bb data structures.  */
void
sel_finish_bbs (void)
{
  sel_restore_other_notes ();

  /* Remove current loop preheader from this loop.  */
  if (flag_sel_sched_pipelining_outer_loops && current_loop_nest)
    sel_remove_loop_preheader ();

  finish_region_bb_info ();
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

/* Return true if INSN has a single successor of type FLAGS.  */
bool
sel_insn_has_single_succ_p (insn_t insn, int flags)
{
  insn_t succ;
  succ_iterator si;
  bool first_p = true;

  FOR_EACH_SUCC_1 (succ, si, insn, flags)
    {
      if (first_p)
	first_p = false;
      else
	return false;
    }

  return true;
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

      /* ??? This code is not supposed to walk out of a region.  */
      gcc_assert (in_current_region_p (pred_bb));

      if (sel_bb_empty_p (pred_bb))
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
sel_num_cfg_preds_gt_1 (insn_t insn)
{
  basic_block bb;

  if (!sel_bb_head_p (insn) || INSN_BB (insn) == 0)
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

      if (!sel_bb_empty_p (bb))
	break;
    }

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
      || (!pipelining_p && INSN_SCHED_TIMES (insn) > 0))
    return true;
  else
    return false;
}

/* Returns true when BB should be the end of an ebb.  Adapted from the 
   code in sched-ebb.c.  */
bool
bb_ends_ebb_p (basic_block bb)
{
  basic_block next_bb = bb_next_bb (bb);
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

      ptr = bb_next_bb (ptr);
    }

  gcc_unreachable ();
  return false;
}

/* Recomputes the reverse topological order for the function and
   saves it in REV_TOP_ORDER_INDEX.  REV_TOP_ORDER_INDEX_LEN is also
   modified appropriately.  */
static void
recompute_rev_top_order (void)
{
  int *postorder;
  int n_blocks, i;

  if (!rev_top_order_index || rev_top_order_index_len < last_basic_block)
    {
      rev_top_order_index_len = last_basic_block; 
      rev_top_order_index = XRESIZEVEC (int, rev_top_order_index,
                                        rev_top_order_index_len);
    }

  postorder = XNEWVEC (int, n_basic_blocks);

  n_blocks = post_order_compute (postorder, true, false);
  gcc_assert (n_basic_blocks == n_blocks);

  /* Build reverse function: for each basic block with BB->INDEX == K
     rev_top_order_index[K] is it's reverse topological sort number.  */
  for (i = 0; i < n_blocks; i++)
    {
      gcc_assert (postorder[i] < rev_top_order_index_len);
      rev_top_order_index[postorder[i]] = i;
    }

  free (postorder);
}

/* Clear all flags from insns in BB that could spoil its rescheduling.  */
void
clear_outdated_rtx_info (basic_block bb)
{
  rtx insn;

  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn) && SCHED_GROUP_P (insn))
      SCHED_GROUP_P (insn) = 0;
}

typedef VEC(rtx, heap) *rtx_vec_t;

static rtx_vec_t bb_note_pool;

/* Add BB_NOTE to the pool of available basic block notes.  */
static void
return_bb_to_pool (basic_block bb)
{
  rtx note = bb_note (bb);

  gcc_assert (NOTE_BASIC_BLOCK (note) == bb
	      && bb->aux == NULL);

  /* It turns out that current cfg infrastructure does not support
     reuse of basic blocks.  Don't bother for now.  */
  /*VEC_safe_push (rtx, heap, bb_note_pool, note);*/
}

/* Get a bb_note from pool or return NULL_RTX if pool is empty.  */
static rtx
get_bb_note_from_pool (void)
{
  if (VEC_empty (rtx, bb_note_pool))
    return NULL_RTX;
  else
    {
      rtx note = VEC_pop (rtx, bb_note_pool);

      PREV_INSN (note) = NULL_RTX;
      NEXT_INSN (note) = NULL_RTX;

      return note;
    }
}

/* Free bb_note_pool.  */
void
free_bb_note_pool (void)
{
  VEC_free (rtx, heap, bb_note_pool);
}

/* Returns a position in RGN where BB can be inserted retaining 
   topological order.  */
static int
find_place_to_insert_bb (basic_block bb, int rgn)
{
  int i, bbi = bb->index, cur_bbi;
  
  for (i = RGN_NR_BLOCKS (rgn) - 1; i >= 0; i--)
    {
      cur_bbi = BB_TO_BLOCK (i);
      if (rev_top_order_index[bbi] 
          < rev_top_order_index[cur_bbi])
        break;
    }
              
  /* We skipped the right block, so we increase i.  We accomodate
     it for increasing by step later, so we decrease i.  */
  return (i + 1) - 1;
}

/* Add (or remove depending on ADD) BB to (from) the current region 
   and update sched-rgn.c data.  */
static void
sel_add_or_remove_bb_1 (basic_block bb, int add)
{
  int i, pos, bbi = -2, rgn;
  int step = (add > 0) ? 1 : 0;

  rgn = CONTAINING_RGN (BB_TO_BLOCK (0));

  if (step)
    {
      bool has_preds_outside_rgn = false;
      edge e;
      edge_iterator ei;

      /* Find whether we have preds outside the region.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
        if (!in_current_region_p (e->src))
          {
            has_preds_outside_rgn = true;
            break;
          }

      /* Recompute the top order -- needed when we have > 1 pred
         and in case we don't have preds outside.  */
      if (flag_sel_sched_pipelining_outer_loops
          && (has_preds_outside_rgn || EDGE_COUNT (bb->preds) > 1))
        {
          recompute_rev_top_order ();
          bbi = find_place_to_insert_bb (bb, rgn);
        }
      else if (has_preds_outside_rgn)
        {
          /* This is the case when we generate an extra empty block
             to serve as region head during pipelining.  */
          e = EDGE_SUCC (bb, 0);
          gcc_assert (EDGE_COUNT (bb->succs) == 1
                      && in_current_region_p (EDGE_SUCC (bb, 0)->dest)
                      && (BLOCK_TO_BB (e->dest->index) == 0));
                  
          bbi = -1;
        }
      else
        {
          if (EDGE_COUNT (bb->succs) > 0)
	    /* We don't have preds outside the region.  We should have
	       the only pred, because the multiple preds case comes from
	       the pipelining of outer loops, and that is handled above.
	       Just take the bbi of this single pred.  */
            {
              int pred_bbi;

              gcc_assert (EDGE_COUNT (bb->preds) == 1);

              pred_bbi = EDGE_PRED (bb, 0)->src->index;
              bbi = BLOCK_TO_BB (pred_bbi);
            }
          else
            /* BB has no successors.  It is safe to put it in the end.  */
            bbi = current_nr_blocks - 1;
        }
    }
  else
    bbi = BLOCK_TO_BB (bb->index);
  
  /* Assert that we've found a proper place.  */
  gcc_assert (bbi != -2);

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
    }
}

/* Add (remove depending on ADD) BB to (from) the current region 
   and update all data.  If BB is NULL, add all blocks from 
   last_added_blocks vector.  */
static void
sel_add_or_remove_bb (basic_block bb, int add)
{
  if (add > 0)
    {
      /* Extend luids so that new notes will recieve zero luids.  */
      sched_init_luids (NULL, NULL, NULL, NULL);
      sched_init_bbs (last_added_blocks, NULL);
      sel_init_bbs (last_added_blocks, NULL);

      /* When bb is passed explicitly, the vector should contain 
         the only element that equals to bb; otherwise, the vector
         should not be NULL.  */
      gcc_assert (last_added_blocks != NULL);

      if (bb != NULL)
        {
          gcc_assert (VEC_length (basic_block, last_added_blocks) == 1
                      && VEC_index (basic_block, 
                                    last_added_blocks, 0) == bb);

	  /* Free the vector.  */
          VEC_free (basic_block, heap, last_added_blocks);
        }
    }
  else
    {
      gcc_assert (bb != NULL && BB_NOTE_LIST (bb) == NULL_RTX);
    }

  if (bb != NULL)
    {
      sel_add_or_remove_bb_1 (bb, add);

      if (add > 0 && !sel_bb_empty_p (bb)
	  && BB_LV_SET (bb) == NULL)
	/* ??? We associate creating/deleting data sets with the first insn
	   appearing / disappearing in the bb.  This is not a clean way to
	   implement infrastructure for handling data sets because we often
	   create new basic blocks with instructions already inside it.
	   That could be made cleaner in two ways:
	   1. Have the only primitive for basic block creation:
	   sel_create_basic_block () and then fill the new basic block with
	   move_insns_to_bb ().
	   2. Or associate data sets with bb notes.  */
	init_invalid_data_sets (bb);

      if (add <= 0)
	{
	  return_bb_to_pool (bb);

	  if (add < 0)
	    {
	      gcc_assert (sel_bb_empty_p (bb));

	      /* Can't assert av_set properties when (add == 0) because
		 we use sel_add_or_remove_bb (bb, 0) when removing loop
		 preheader from the region.  At the point of removing the
		 preheader we already have deallocated sel_region_bb_info.  */
	      gcc_assert (BB_LV_SET (bb) == NULL
			  && !BB_LV_SET_VALID_P (bb)
			  && BB_AV_LEVEL (bb) == 0
			  && BB_AV_SET (bb) == NULL);

	      delete_basic_block (bb);
	    }
	}
    }
  else
    /* BB is NULL - process LAST_ADDED_BLOCKS instead.  */
    {
      int i;
      basic_block temp_bb = NULL;

      gcc_assert (add > 0);

      for (i = 0; 
           VEC_iterate (basic_block, last_added_blocks, i, bb); i++)
        {
          sel_add_or_remove_bb_1 (bb, add);
          temp_bb = bb;
        }

      /* We need to fetch at least one bb so we know the region 
         to update.  */
      gcc_assert (temp_bb != NULL);
      bb = temp_bb;

      VEC_free (basic_block, heap, last_added_blocks);
    }

  rgn_setup_region (CONTAINING_RGN (bb->index));

#if defined ENABLE_CHECKING && 0
  /* This check is verifies that all jumps jump where they should.
     This code is adopted from flow.c: init_propagate_block_info ().  */
  {
    basic_block bb;

    FOR_EACH_BB (bb)
      {
	if (JUMP_P (BB_END (bb))
	    && any_condjump_p (BB_END (bb)))
	  {
	    if (!single_succ_p (bb))
	      gcc_assert (EDGE_SUCC (bb, 0)->flags & EDGE_FALLTHRU
			  || EDGE_SUCC (bb, 1)->flags & EDGE_FALLTHRU);
	    else
	      gcc_assert (JUMP_LABEL (BB_END (bb))
			  == BB_HEAD (EDGE_SUCC (bb, 0)->dest));
	  }
      }
  }
#endif
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

  sel_add_or_remove_bb (bb, 1);

  return bb;
}

/* Concatenate info of EMPTY_BB to info of MERGE_BB.  */
static void
move_bb_info (basic_block merge_bb, basic_block empty_bb)
{
  gcc_assert (in_current_region_p (merge_bb));

  concat_note_lists (BB_NOTE_LIST (empty_bb), 
		     &BB_NOTE_LIST (merge_bb));
  BB_NOTE_LIST (empty_bb) = NULL_RTX;

}

/* Remove an empty basic block EMPTY_BB.  When MERGE_UP_P is true, we put 
   EMPTY_BB's note lists into its predecessor instead of putting them 
   into the successor.  */
void
sel_remove_empty_bb (basic_block empty_bb, bool merge_up_p,
		     bool remove_from_cfg_p)
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
      merge_bb = bb_next_bb (empty_bb);

      gcc_assert (EDGE_COUNT (empty_bb->succs) == 1
		  && EDGE_SUCC (empty_bb, 0)->dest == merge_bb);
    }

  move_bb_info (merge_bb, empty_bb);

  remove_empty_bb (empty_bb, remove_from_cfg_p);
}

/* Remove EMPTY_BB.  If REMOVE_FROM_CFG_P is false, remove EMPTY_BB from
   region, but keep it in CFG.  */
static void
remove_empty_bb (basic_block empty_bb, bool remove_from_cfg_p)
{
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
  if (remove_from_cfg_p
      && (EDGE_COUNT (empty_bb->preds) > 0
	  || EDGE_COUNT (empty_bb->succs) > 0))
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

	  gcc_assert (e->flags & EDGE_FALLTHRU);

	  succ = e->dest;
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
  sel_add_or_remove_bb (empty_bb, remove_from_cfg_p ? -1 : 0);
}

static struct cfg_hooks orig_cfg_hooks;

/* An implementation of create_basic_block hook, which additionally updates 
   per-bb data structures.  */
static basic_block
sel_create_basic_block (void *headp, void *endp, basic_block after)
{
  basic_block new_bb;
  insn_t new_bb_note;
  
  gcc_assert (flag_sel_sched_pipelining_outer_loops 
              || last_added_blocks == NULL);

  new_bb_note = get_bb_note_from_pool ();

  if (new_bb_note == NULL_RTX)
    new_bb = orig_cfg_hooks.create_basic_block (headp, endp, after);
  else
    {
      new_bb = create_basic_block_structure (headp, endp,
					     new_bb_note, after);
      new_bb->aux = NULL;
    }

  VEC_safe_push (basic_block, heap, last_added_blocks, new_bb);

  return new_bb;
}

/* Implement sched_init_only_bb ().  */
static void
sel_init_only_bb (basic_block bb, basic_block after)
{
  gcc_assert (after == NULL);

  rgn_make_new_region_out_of_new_block (bb);
}

/* Update the latch when we've splitted or merged it.
   This should be checked for all outer loops, too.  */
static void
change_loops_latches (basic_block from, basic_block to)
{
  gcc_assert (from != to);

  if (flag_sel_sched_pipelining_outer_loops 
      && current_loop_nest)
    {
      struct loop *loop;

      for (loop = current_loop_nest; loop; loop = loop_outer (loop))
        if (considered_for_pipelining_p (loop) && loop->latch == from)
          {
            gcc_assert (loop == current_loop_nest);
            loop->latch = to;
            gcc_assert (loop_latch_edge (loop));
          }
    }
}

/* Splits BB on two basic blocks, adding it to the region and extending 
   per-bb data structures.  Returns the newly created bb.  */
static basic_block
sel_split_block (basic_block bb, rtx after)
{
  basic_block new_bb;
  insn_t insn;

  can_add_real_insns_p = false;
  new_bb = sched_split_block_1 (bb, after);
  can_add_real_insns_p = true;

  sel_add_or_remove_bb (new_bb, 1);

  /* This should be called after sel_add_or_remove_bb, because this uses
     CONTAINING_RGN for the new block, which is not yet initialized.  
     FIXME: this function may be a no-op now.  */
  change_loops_latches (bb, new_bb);

  /* Update ORIG_BB_INDEX for insns moved into the new block.  */
  FOR_BB_INSNS (new_bb, insn)
   if (INSN_P (insn))
     EXPR_ORIG_BB_INDEX (INSN_EXPR (insn)) = new_bb->index;

  if (sel_bb_empty_p (bb))
    {
      gcc_assert (!sel_bb_empty_p (new_bb));

      /* NEW_BB has data sets that need to be updated and BB holds
	 data sets that should be removed.  Exchange these data sets
	 so that we won't lose BB's valid data sets.  */
      exchange_data_sets (new_bb, bb);

      free_data_sets (bb);
    }

  return new_bb;
}

/* Splits E and adds the newly created basic block to the current region.
   Returns this basic block.  */
basic_block
sel_split_edge (edge e)
{
  basic_block new_bb;
  
  /* We don't need to split edges inside a region.  */
  gcc_assert (!in_current_region_p (e->src)
              && in_current_region_p (e->dest));

  insn_init.what = INSN_INIT_WHAT_INSN;

  new_bb = split_edge (e);

  if (flag_sel_sched_pipelining_outer_loops 
      && current_loop_nest)
    {
      int i;
      basic_block bb;

      /* Some of the basic blocks might not have been added to the loop.  
         Add them here, until this is fixed in force_fallthru.  */
      for (i = 0; 
           VEC_iterate (basic_block, last_added_blocks, i, bb); i++)
        if (!bb->loop_father)
          add_bb_to_loop (bb, e->dest->loop_father);
    }

  /* Add all last_added_blocks to the region.  */
  sel_add_or_remove_bb (NULL, 1);

  /* Now the CFG has been updated, and we can init data for the newly 
     created insns.  */
  insn_init.todo = (INSN_INIT_TODO_LUID | INSN_INIT_TODO_SIMPLEJUMP);
  sel_init_new_insns ();

  return new_bb;
}

/* Implement sched_create_empty_bb ().  */
static basic_block
sel_create_empty_bb (basic_block after)
{
  basic_block new_bb;

  can_add_real_insns_p = false;
  new_bb = sched_create_empty_bb_1 (after);
  can_add_real_insns_p = true;

  /* We'll explicitly initialize NEW_BB via sel_init_only_bb () a bit
     later.  */
  gcc_assert (VEC_length (basic_block, last_added_blocks) == 1
	      && VEC_index (basic_block, last_added_blocks, 0) == new_bb);

  VEC_free (basic_block, heap, last_added_blocks);

  return new_bb;
}

/* Implement sched_create_recovery_block ().  */
basic_block
sel_create_recovery_block (insn_t orig_insn)
{
  basic_block first_bb;
  basic_block second_bb;
  basic_block recovery_block;

  first_bb = BLOCK_FOR_INSN (orig_insn);
  second_bb = sched_split_block (first_bb, orig_insn);

  can_add_real_insns_p = false;
  recovery_block = sched_create_recovery_block ();
  can_add_real_insns_p = true;
  gcc_assert (sel_bb_empty_p (recovery_block));

  insn_init.what = INSN_INIT_WHAT_INSN;

  sched_create_recovery_edges (first_bb, recovery_block, second_bb);

  if (current_loops != NULL)
    add_bb_to_loop (recovery_block, first_bb->loop_father);

  sel_add_or_remove_bb (recovery_block, 1);

  /* Now the CFG has been updated, and we can init data for the newly 
     created insns.  */
  insn_init.todo = (INSN_INIT_TODO_LUID | INSN_INIT_TODO_SIMPLEJUMP);
  sel_init_new_insns ();

  return recovery_block;
}

/* Merge basic block B into basic block A.  */
void
sel_merge_blocks (basic_block a, basic_block b)
{
  gcc_assert (can_merge_blocks_p (a, b));

  sel_remove_empty_bb (b, true, false);
  merge_blocks (a, b);

  change_loops_latches (b, a);
}

/* A wrapper for redirect_edge_and_branch_force, which also initializes
   data structures for possibly created bb and insns.  Returns the newly
   added bb or NULL, when a bb was not needed.  */
void
sel_redirect_edge_and_branch_force (edge e, basic_block to)
{
  basic_block jump_bb;

  gcc_assert (!sel_bb_empty_p (e->src));

  insn_init.what = INSN_INIT_WHAT_INSN;

  jump_bb = redirect_edge_and_branch_force (e, to);

  if (jump_bb != NULL)
    sel_add_or_remove_bb (jump_bb, 1);

  /* This function could not be used to spoil the loop structure by now,
     thus we don't care to update anything.  But check it to be sure.  */
  if (flag_sel_sched_pipelining_outer_loops && current_loop_nest)
    gcc_assert (loop_latch_edge (current_loop_nest));

  /* Now the CFG has been updated, and we can init data for the newly 
     created insns.  */
  insn_init.todo = (INSN_INIT_TODO_LUID | INSN_INIT_TODO_SIMPLEJUMP);
  sel_init_new_insns ();
}

/* A wrapper for redirect_edge_and_branch.  */
void
sel_redirect_edge_and_branch (edge e, basic_block to)
{
  edge ee;
  bool latch_edge_p;

  latch_edge_p = (flag_sel_sched_pipelining_outer_loops 
                  && current_loop_nest
                  && e == loop_latch_edge (current_loop_nest));

  insn_init.what = INSN_INIT_WHAT_INSN;

  ee = redirect_edge_and_branch (e, to);

  /* When we've redirected a latch edge, update the header.  */
  if (latch_edge_p)
    {
      current_loop_nest->header = to;
      gcc_assert (loop_latch_edge (current_loop_nest));
    }

  gcc_assert (ee == e && last_added_blocks == NULL);

  /* Now the CFG has been updated, and we can init data for the newly 
     created insns.  */
  insn_init.todo = (INSN_INIT_TODO_LUID | INSN_INIT_TODO_SIMPLEJUMP);
  sel_init_new_insns ();
}

static struct cfg_hooks sel_cfg_hooks;

/* Register sel-sched cfg hooks.  */
void
sel_register_cfg_hooks (void)
{
  sched_split_block = sel_split_block;

  orig_cfg_hooks = get_cfg_hooks ();
  sel_cfg_hooks = orig_cfg_hooks;

  sel_cfg_hooks.create_basic_block = sel_create_basic_block;
  sel_cfg_hooks.delete_basic_block = rtl_delete_block_not_barriers;

  set_cfg_hooks (sel_cfg_hooks);

  sched_init_only_bb = sel_init_only_bb;
  sched_split_block = sel_split_block;
  sched_create_empty_bb = sel_create_empty_bb;
}

/* Unregister sel-sched cfg hooks.  */
void
sel_unregister_cfg_hooks (void)
{
  sched_create_empty_bb = NULL;
  sched_split_block = NULL;
  sched_init_only_bb = NULL;

  set_cfg_hooks (orig_cfg_hooks);
}



/* Emit an insn rtx based on PATTERN.  */
static rtx
create_insn_rtx_from_pattern_1 (rtx pattern, rtx label)
{
  rtx insn_rtx;

  gcc_assert (!INSN_P (pattern));

  start_sequence ();
  insn_init.what = INSN_INIT_WHAT_INSN_RTX;

  if (label == NULL_RTX)
    insn_rtx = emit_insn (pattern);
  else
    {
      insn_rtx = emit_jump_insn (pattern);
      JUMP_LABEL (insn_rtx) = label;
      ++LABEL_NUSES (label);
    }

  end_sequence ();

  sched_init_luids (NULL, NULL, NULL, NULL);

  sched_extend_target ();
  sched_deps_init (false);

  return insn_rtx;
}

/* Emit an insn rtx based on PATTERN and ICE if the result is not a valid
   insn.  */
rtx
create_insn_rtx_from_pattern (rtx pattern, rtx label)
{
  rtx insn_rtx = create_insn_rtx_from_pattern_1 (pattern, label);

  if (!insn_rtx_valid (insn_rtx))
    gcc_unreachable ();

  return insn_rtx;
}

/* Create a new vinsn for INSN_RTX.  */
vinsn_t
create_vinsn_from_insn_rtx (rtx insn_rtx)
{
  gcc_assert (INSN_P (insn_rtx) && !INSN_IN_STREAM_P (insn_rtx));

  return vinsn_create (insn_rtx, false);
}

/* Create a copy of INSN_RTX.  */
rtx
create_copy_of_insn_rtx (rtx insn_rtx)
{
  bool orig_is_valid_p;
  rtx res;

  gcc_assert (NONJUMP_INSN_P (insn_rtx));

  orig_is_valid_p = insn_rtx_valid (insn_rtx);

  res = create_insn_rtx_from_pattern_1 (copy_rtx (PATTERN (insn_rtx)),
					NULL_RTX);

  if (insn_rtx_valid (res))
    gcc_assert (orig_is_valid_p);
  else
    gcc_assert (!orig_is_valid_p);

  return res;
}

/* Change vinsn field of EXPR to hold NEW_VINSN.  */
void
change_vinsn_in_expr (expr_t expr, vinsn_t new_vinsn)
{
  vinsn_detach (EXPR_VINSN (expr));

  EXPR_VINSN (expr) = new_vinsn;
  vinsn_attach (new_vinsn);
}

/* Helpers for global init.  */
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
  SEL_SCHED | NEW_BBS
};

/* Setup special insns used in the scheduler.  */
void 
setup_nop_and_exit_insns (void)
{
  gcc_assert (nop_pattern == NULL_RTX
	      && exit_insn == NULL_RTX);

  nop_pattern = gen_nop ();

  {
    start_sequence ();
    insn_init.what = INSN_INIT_WHAT_INSN_RTX;
    emit_insn (nop_pattern);
    exit_insn = get_insns ();
    end_sequence ();
    set_block_for_insn (exit_insn, EXIT_BLOCK_PTR);
  }
}

/* Free special insns used in the scheduler.  */
void
free_nop_and_exit_insns (void)
{
  exit_insn = NULL_RTX;
  nop_pattern = NULL_RTX;
}

/* Setup a special vinsn used in new insns initialization.  */
void
setup_nop_vinsn (void)
{
  nop_vinsn = vinsn_create (exit_insn, false);
  vinsn_attach (nop_vinsn);
}

/* Free a special vinsn used in new insns initialization.  */
void
free_nop_vinsn (void)
{
  gcc_assert (VINSN_COUNT (nop_vinsn) == 1);
  vinsn_detach (nop_vinsn);
  nop_vinsn = NULL;
}

/* Call a set_sched_flags hook.  */
void
sel_set_sched_flags (void)
{
  /* ??? This means that set_sched_flags were called, and we decided to 
     support speculation.  However, set_sched_flags also modifies flags
     on current_sched_info, doing this only at global init.  And we 
     sometimes change c_s_i later.  So put the correct flags again.  */
  if (spec_info && targetm.sched.set_sched_flags)
    targetm.sched.set_sched_flags (spec_info);
}

/* Setup pointers to global sched info structures.  */
void
sel_setup_sched_infos (void)
{
  rgn_setup_common_sched_info ();

  memcpy (&sel_common_sched_info, common_sched_info,
	  sizeof (sel_common_sched_info));

  sel_common_sched_info.fix_recovery_cfg = NULL;
  sel_common_sched_info.add_block = NULL;
  sel_common_sched_info.estimate_number_of_insns
    = sel_estimate_number_of_insns;
  sel_common_sched_info.luid_for_non_insn = sel_luid_for_non_insn;
  sel_common_sched_info.sched_pass_id = SCHED_SEL_PASS;

  common_sched_info = &sel_common_sched_info;
  current_sched_info = &sched_sel_haifa_sched_info;
  
  sel_set_sched_flags ();
}

/* Adds basic block BB to region RGN at the position *BB_ORD_INDEX,
   *BB_ORD_INDEX after that is increased.  */
static void
sel_add_block_to_region (basic_block bb, int *bb_ord_index, int rgn)
{
  RGN_NR_BLOCKS (rgn) += 1;
  RGN_DONT_CALC_DEPS (rgn) = 0;
  RGN_HAS_REAL_EBB (rgn) = 0;
  RGN_HAS_RENAMING_P (nr_regions) = 0;
  RGN_WAS_PIPELINED_P (nr_regions) = 0;
  RGN_NEEDS_GLOBAL_LIVE_UPDATE (nr_regions) = 0;
  CONTAINING_RGN (bb->index) = rgn;
  BLOCK_TO_BB (bb->index) = *bb_ord_index;
  rgn_bb_table[RGN_BLOCKS (rgn) + *bb_ord_index] = bb->index;
  (*bb_ord_index)++;

  /* FIXME: it is true only when not scheduling ebbs.  */
  RGN_BLOCKS (rgn + 1) = RGN_BLOCKS (rgn) + RGN_NR_BLOCKS (rgn);
}

/* Functions to support pipelining of outer loops.  */

/* Creates a new empty region and returns it's number.  */
static int
sel_create_new_region (void)
{
  int new_rgn_number = nr_regions;

  RGN_NR_BLOCKS (new_rgn_number) = 0;

  /* FIXME: This will work only when EBBs are not created.  */
  if (new_rgn_number != 0)
    RGN_BLOCKS (new_rgn_number) = RGN_BLOCKS (new_rgn_number - 1) + 
      RGN_NR_BLOCKS (new_rgn_number - 1);
  else
    RGN_BLOCKS (new_rgn_number) = 0;

  /* Set the blocks of the next region so the other functions may
     calculate the number of blocks in the region.  */
  RGN_BLOCKS (new_rgn_number + 1) = RGN_BLOCKS (new_rgn_number) + 
    RGN_NR_BLOCKS (new_rgn_number);

  nr_regions++;

  return new_rgn_number;
}

/* If BB1 has a smaller topological sort number than BB2, returns -1;
   if greater, returns 1.  */
static int
bb_top_order_comparator (const void *x, const void *y)
{
  basic_block bb1 = *(const basic_block *) x;
  basic_block bb2 = *(const basic_block *) y;

  gcc_assert (bb1 == bb2 
	      || rev_top_order_index[bb1->index] 
		 != rev_top_order_index[bb2->index]);

  /* It's a reverse topological order in REV_TOP_ORDER_INDEX, so
     bbs with greater number should go earlier.  */
  if (rev_top_order_index[bb1->index] > rev_top_order_index[bb2->index])
    return -1;
  else
    return 1;
}

/* Create a region for LOOP and return its number.  If we don't want 
   to pipeline LOOP, return -1.  */
static int
make_region_from_loop (struct loop *loop)
{
  unsigned int i;
  int num_insns;
  int new_rgn_number = -1;
  struct loop *inner;

  /* Basic block index, to be assigned to BLOCK_TO_BB.  */
  int bb_ord_index = 0;
  basic_block *loop_blocks;
  basic_block preheader_block;

  if (loop->num_nodes 
      > (unsigned) PARAM_VALUE (PARAM_MAX_PIPELINE_REGION_BLOCKS))
    return -1;
  
  /* Don't pipeline loops whose latch belongs to some of its inner loops.  */
  for (inner = loop->inner; inner; inner = inner->inner)
    if (flow_bb_inside_loop_p (inner, loop->latch))
      return -1;

  num_insns = 0;
  loop_blocks = get_loop_body_in_custom_order (loop, bb_top_order_comparator);

  for (i = 0; i < loop->num_nodes; i++)
    {
      num_insns += (common_sched_info->estimate_number_of_insns
                    (loop_blocks [i]));

      if ((loop_blocks[i]->flags & BB_IRREDUCIBLE_LOOP)
          || num_insns > PARAM_VALUE (PARAM_MAX_PIPELINE_REGION_INSNS))
        {
          free (loop_blocks);
          return -1;
        }
      
    }

  preheader_block = loop_preheader_edge (loop)->src;
  gcc_assert (preheader_block);
  gcc_assert (loop_blocks[0] == loop->header);

  new_rgn_number = sel_create_new_region ();

  sel_add_block_to_region (preheader_block, &bb_ord_index, new_rgn_number);
  SET_BIT (bbs_in_loop_rgns, preheader_block->index);

  for (i = 0; i < loop->num_nodes; i++)
    {
      /* Add only those blocks that haven't been scheduled in the inner loop.
	 The exception is the basic blocks with bookkeeping code - they should
	 be added to the region (and they actually don't belong to the loop 
	 body, but to the region containing that loop body).  */

      gcc_assert (new_rgn_number >= 0);

      if (! TEST_BIT (bbs_in_loop_rgns, loop_blocks[i]->index))
	{
	  sel_add_block_to_region (loop_blocks[i], &bb_ord_index, 
                                   new_rgn_number);
	  SET_BIT (bbs_in_loop_rgns, loop_blocks[i]->index);
	}
    }

  free (loop_blocks);
  MARK_LOOP_FOR_PIPELINING (loop);

  return new_rgn_number;
}

/* Create a new region from preheader blocks LOOP_BLOCKS.  */
void
make_region_from_loop_preheader (VEC(basic_block, heap) **loop_blocks)
{
  unsigned int i;
  int new_rgn_number = -1;
  basic_block bb;

  /* Basic block index, to be assigned to BLOCK_TO_BB.  */
  int bb_ord_index = 0;

  new_rgn_number = sel_create_new_region ();

  for (i = 0; VEC_iterate (basic_block, *loop_blocks, i, bb); i++)
    {
      gcc_assert (new_rgn_number >= 0);

      sel_add_block_to_region (bb, &bb_ord_index, new_rgn_number);
    }

  VEC_free (basic_block, heap, *loop_blocks);
  gcc_assert (*loop_blocks == NULL);
}


/* Create region(s) from loop nest LOOP, such that inner loops will be
   pipelined before outer loops.  Returns true when a region for LOOP 
   is created.  */
static bool
make_regions_from_loop_nest (struct loop *loop)
{   
  struct loop *cur_loop;
  int rgn_number;

  /* Traverse all inner nodes of the loop.  */
  for (cur_loop = loop->inner; cur_loop; cur_loop = cur_loop->next)
    if (! TEST_BIT (bbs_in_loop_rgns, cur_loop->header->index)
        && ! make_regions_from_loop_nest (cur_loop))
      return false;

  /* At this moment all regular inner loops should have been pipelined.
     Try to create a region from this loop.  */
  rgn_number = make_region_from_loop (loop);

  if (rgn_number < 0)
    return false;

  VEC_safe_push (loop_p, heap, loop_nests, loop);
  return true;
}

/* Initalize data structures needed.  */
void
pipeline_outer_loops_init (void)
{
  /* Collect loop information to be used in outer loops pipelining.  */
  loop_optimizer_init (LOOPS_HAVE_PREHEADERS
                       | LOOPS_HAVE_FALLTHRU_PREHEADERS
		       | LOOPS_HAVE_RECORDED_EXITS
		       | LOOPS_HAVE_MARKED_IRREDUCIBLE_REGIONS);
  current_loop_nest = NULL;

  bbs_in_loop_rgns = sbitmap_alloc (last_basic_block);
  sbitmap_zero (bbs_in_loop_rgns);

  recompute_rev_top_order ();
}

/* Returns a struct loop for region RGN.  */
loop_p
get_loop_nest_for_rgn (unsigned int rgn)
{
  /* Regions created with extend_rgns don't have corresponding loop nests,
     because they don't represent loops.  */
  if (rgn < VEC_length (loop_p, loop_nests))
    return VEC_index (loop_p, loop_nests, rgn);
  else
    return NULL;
}

/* True when LOOP was included into pipelining regions.   */
bool
considered_for_pipelining_p (struct loop *loop)
{
  if (loop_depth (loop) == 0)
    return false;

  /* Now, the loop could be too large or irreducible.  Check whether its 
     region is in LOOP_NESTS.  
     We determine the region number of LOOP as the region number of its 
     latch.  We can't use header here, because this header could be 
     just removed preheader and it will give us the wrong region number.
     Latch can't be used because it could be in the inner loop too.  */
  if (LOOP_MARKED_FOR_PIPELINING_P (loop))
    {
      int rgn = CONTAINING_RGN (loop->latch->index);

      gcc_assert ((unsigned) rgn < VEC_length (loop_p, loop_nests));
      return true;
    }
  
  return false;
}

/* Makes regions from the rest of the blocks, after loops are chosen 
   for pipelining.  */
static void
make_regions_from_the_rest (void)
{
  int cur_rgn_blocks;
  int *loop_hdr;
  int i;

  basic_block bb;
  edge e;
  edge_iterator ei;
  int *degree;
  int new_regions;

  /* Index in rgn_bb_table where to start allocating new regions.  */
  cur_rgn_blocks = nr_regions ? RGN_BLOCKS (nr_regions) : 0;
  new_regions = nr_regions;

  /* Make regions from all the rest basic blocks - those that don't belong to 
     any loop or belong to irreducible loops.  Prepare the data structures
     for extend_rgns.  */

  /* LOOP_HDR[I] == -1 if I-th bb doesn't belong to any loop,
     LOOP_HDR[I] == LOOP_HDR[J] iff basic blocks I and J reside within the same
     loop.  */
  loop_hdr = XNEWVEC (int, last_basic_block);
  degree = XCNEWVEC (int, last_basic_block);


  /* For each basic block that belongs to some loop assign the number
     of innermost loop it belongs to.  */
  for (i = 0; i < last_basic_block; i++)
    loop_hdr[i] = -1;

  FOR_EACH_BB (bb)
    {
      if (bb->loop_father && !bb->loop_father->num == 0
	  && !(bb->flags & BB_IRREDUCIBLE_LOOP))
	loop_hdr[bb->index] = bb->loop_father->num;
    }

  /* For each basic block degree is calculated as the number of incoming 
     edges, that are going out of bbs that are not yet scheduled.
     The basic blocks that are scheduled have degree value of zero.  */
  FOR_EACH_BB (bb) 
    {
      degree[bb->index] = 0;

      if (!TEST_BIT (bbs_in_loop_rgns, bb->index))
	{
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    if (!TEST_BIT (bbs_in_loop_rgns, e->src->index))
	      degree[bb->index]++;
	}
      else
	degree[bb->index] = -1;
    }

  extend_rgns (degree, &cur_rgn_blocks, bbs_in_loop_rgns, loop_hdr);

  /* Any block that did not end up in a region is placed into a region
     by itself.  */
  FOR_EACH_BB (bb)
    if (degree[bb->index] >= 0)
      {
	rgn_bb_table[cur_rgn_blocks] = bb->index;
	RGN_NR_BLOCKS (nr_regions) = 1;
	RGN_BLOCKS (nr_regions) = cur_rgn_blocks++;
        RGN_DONT_CALC_DEPS (nr_regions) = 0;
	RGN_HAS_REAL_EBB (nr_regions) = 0;
	RGN_HAS_RENAMING_P (nr_regions) = 0;
	RGN_WAS_PIPELINED_P (nr_regions) = 0;
	RGN_NEEDS_GLOBAL_LIVE_UPDATE (nr_regions) = 0;
	CONTAINING_RGN (bb->index) = nr_regions++;
	BLOCK_TO_BB (bb->index) = 0;
      }

  free (degree);
  free (loop_hdr);
}

/* Free data structures used in pipelining of outer loops.  */
void pipeline_outer_loops_finish (void)
{
  loop_iterator li;
  struct loop *loop;

  /* Release aux fields so we don't free them later by mistake.  */
  FOR_EACH_LOOP (li, loop, 0)
    loop->aux = NULL;

  loop_optimizer_finalize ();

  VEC_free (loop_p, heap, loop_nests);

  free (rev_top_order_index);
  rev_top_order_index = NULL;
}

/* This function replaces the find_rgns when 
   FLAG_SEL_SCHED_PIPELINING_OUTER_LOOPS is set.  */
void 
sel_find_rgns (void)
{
  struct loop *loop;

  if (current_loops)
    /* Start traversing from the root node.  */
    for (loop = VEC_index (loop_p, current_loops->larray, 0)->inner; 
	 loop; loop = loop->next)
      make_regions_from_loop_nest (loop);

  /* Make regions from all the rest basic blocks and schedule them.
     These blocks include blocks that don't belong to any loop or belong  
     to irreducible loops.  */
  make_regions_from_the_rest ();

  /* We don't need bbs_in_loop_rgns anymore.  */
  sbitmap_free (bbs_in_loop_rgns);
  bbs_in_loop_rgns = NULL;
}

/* Adds the preheader blocks from previous loop to current region taking 
   it from LOOP_PREHEADER_BLOCKS (current_loop_nest).  
   This function is only used with -fsel-sched-pipelining-outer-loops.  */
void
sel_add_loop_preheader (void)
{
  int i;
  basic_block bb;
  int rgn = CONTAINING_RGN (BB_TO_BLOCK (0));
  VEC(basic_block, heap) *preheader_blocks 
    = LOOP_PREHEADER_BLOCKS (current_loop_nest);

  for (i = 0;
       VEC_iterate (basic_block,
		    LOOP_PREHEADER_BLOCKS (current_loop_nest), i, bb);
       i++)
    {
      sel_add_or_remove_bb_1 (bb, 1);
      
      /* Set variables for the current region.  */
      rgn_setup_region (rgn);
    }
  
  VEC_free (basic_block, heap, preheader_blocks);
  MARK_LOOP_FOR_PIPELINING (current_loop_nest);
}

/* While pipelining outer loops, returns TRUE if BB is a loop preheader.  */
bool
sel_is_loop_preheader_p (basic_block bb)
{
  if (!pipelining_p)
    return false;

  /* Preheader is the first block in the region.  */
  if (BLOCK_TO_BB (bb->index) == 0)
    return true;

  if (current_loop_nest)
    {
      struct loop *outer;

      /* We used to find a preheader with the topological information.
         Check that the above code is equivalent to what we did before.  */

      if (in_current_region_p (current_loop_nest->header))
	gcc_assert (!(BLOCK_TO_BB (bb->index) 
		      < BLOCK_TO_BB (current_loop_nest->header->index)));

      /* Support the situation when the latch block of outer loop
         could be from here.  */
      for (outer = loop_outer (current_loop_nest);
	   outer;
	   outer = loop_outer (outer))
        if (considered_for_pipelining_p (outer) && outer->latch == bb)
          gcc_unreachable ();
    }

  return false;
}

/* Removes the loop preheader from the current region and saves it in
   PREHEADER_BLOCKS of the father loop, so they will be added later to 
   region that represents an outer loop.  
   This function is only used with -fsel-sched-pipelining-outer-loops.  */
static void
sel_remove_loop_preheader (void)
{
  int i, old_len;
  int cur_rgn = CONTAINING_RGN (BB_TO_BLOCK (0));
  basic_block bb;
  VEC(basic_block, heap) *preheader_blocks 
    = LOOP_PREHEADER_BLOCKS (loop_outer (current_loop_nest));

  gcc_assert (flag_sel_sched_pipelining_outer_loops && current_loop_nest);
  old_len = VEC_length (basic_block, preheader_blocks);

  /* Add blocks that aren't within the current loop to PREHEADER_BLOCKS.  */
  for (i = 0; i < RGN_NR_BLOCKS (cur_rgn); i++)
    {
      bb = BASIC_BLOCK (BB_TO_BLOCK (i));

      /* If the basic block belongs to region, but doesn't belong to 
	 corresponding loop, then it should be a preheader.  */
      if (sel_is_loop_preheader_p (bb))
        VEC_safe_push (basic_block, heap, preheader_blocks, bb);
    }
  
  /* Remove these blocks only after iterating over the whole region.  */
  for (i = VEC_length (basic_block, preheader_blocks) - 1;
       i >= old_len;
       i--)
    {
       bb =  VEC_index (basic_block, preheader_blocks, i); 

       sel_add_or_remove_bb (bb, 0);
    }

  if (!considered_for_pipelining_p (loop_outer (current_loop_nest)))
    /* Immediately create new region from preheader.  */
    make_region_from_loop_preheader (&preheader_blocks);
  else
    /* Store preheader within the father's loop structure.  */
    SET_LOOP_PREHEADER_BLOCKS (loop_outer (current_loop_nest),
			       preheader_blocks);
}
#endif
