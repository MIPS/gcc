/* Perform loop invariant motion.
   Copyright (C) 1987, 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
   1998, 1999, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "loop.h"
#include "cfglayout.h"
#include "params.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "df.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "regs.h"

#define EL_RTX
#include "algebraic.h"

/* This is part of the loop optimization pass of the compiler.
   It finds invariant computations within loops and moves them
   to the beginning of the loop.  We do not do load and store motion;
   this is handled in gcse.
   
   For each simple set, we determine to what depth of outer loops
   it is invariant; we do it this way:  We pass the blocks in dominance
   order and consider insn invariant to level d if all of its arguments
   dominate it, and all of them are invariant to this level; otherwise
   we consider it invariant to the depth of the surrounding loop (i.e.
   not at all).  Analogically mems are classified according to depths
   of arguments, except that we also must take care of aliasing.

   If the expression may trap, we must be a bit more careful and increase
   the depth so that we are guaranteed the expression is evaluated.
   
   We do the moving of invariants by storing them to temporaries -- i.e. we
   do not try to really replace them, leaving the job for copy propagation
   pass that follows.  We might later decide to do it ourselves, but we must
   be careful not to invalidate information for unrolling.
   
   TODO:
   Add handling of invariant hard regs.  */

/* Some parameters for heuristics.  */

/* Cost for each load of register that had to be spilled.  */
#define SPILL_COST (PARAM_VALUE (LIM_SPILL_COST))

/* Number of hard regs that are usable for register allocation.  */
#define N_USABLE_REGS (PARAM_VALUE (LIM_N_USABLE_REGS))

/* Number of registers to remove if the loop contains call.  */
#define N_CALL_REGS (PARAM_VALUE (LIM_N_CALL_REGS))

/* Number of registers reserved for each loop level.  */
#define REGS_PER_LOOP_LEVEL (PARAM_VALUE (LIM_REGS_PER_LOOP_LEVEL))

/* Number of insns for that we introduce new register.  Really stupid parameter
   that should prevent us from moving invariants from large loops.  */
#define INSNS_PER_REG (PARAM_VALUE (LIM_INSNS_PER_REG))

/* Weight for lifespan change.  */
#define LIFESPAN_WEIGHT (PARAM_VALUE (LIM_LIFESPAN_WEIGHT))

/* Weight for gains.  */
#define GAIN_WEIGHT (PARAM_VALUE (LIM_GAIN_WEIGHT))

/* Weight for registers used.  */
#define REG_WEIGHT (PARAM_VALUE (LIM_REG_WEIGHT))

/* During the analysis of loops, a chain of `struct movable's
   is made to record all the movable insns found.
   Then the entire chain can be scanned to decide which to move.  */
struct movable
{
  rtx insn;			/* The insn whose parameters we remeber here.  */
  rtx insns;			/* INSN_LIST of equivalent movable insns.  */
  rtx set_dests;		/* The EXPR_LIST of registers to that the sets
				   are done, corresponding one-to-one to
				   insns.  */
  rtx set_src;			/* The expression to that we are equal --
				   it is the rhs of first encountered instance,
				   i.e. the last insn in insns list.  */
  rtx value;			/* The value obtained from induction variable
				   analysis.  */
  enum machine_mode mode;	/* Mode of the expression.  */
  rtx temp;			/* The replacement register.  */
  struct movable_list *depends;	/* List of movables that must be moved to
				   enable moving of this insn.  */
  int depth;			/* To that it is invariant.  */
  struct loop *loop;		/* To that it belongs.  */
  int local;			/* The registers are only used in one loop
				   and its definition dominate all uses.  */
  int lifetime;			/* Lifetime of the defs.  */
  int cost;			/* Cost of recorded set_src.  */
  int savings;			/* Number of insns we can move for this reg,
				   including other movables that force this
				   or match this one.  */
  int move_depth;		/* To that the computation should be moved.  */
  int replaceable;		/* Number of dests that might simply be replaced
				   by temporary.  We don't do it, but copy
				   propagation will, so we take it into account
				   when computing costs.  */
  rtx single_targets;		/* Insn list of single uses of dests (or
				   NULLs if dest is used multiple times)  */
  struct movable_list *single_targets_m;
				/* The same, but the corresponding movables.  */
};

/* Describes gain obtained by moving movable.  */
struct movable_gain
{
  int gain_in_reg;		/* Gain if there is free reg.  */
  int gain_spill;		/* Gain if there is no free reg.  */
  int moved_st_bonus;		/* Bonus for moved single target.  */
  int req_regs;			/* Number of registers needed.  */
  int delta_lifespan;		/* Change of lifespan.  */
};

int *n_bb_insns;

static void detect_bb_movables (basic_block, struct movable_list **);
static void detect_movable_set (basic_block, rtx, int,
				struct movable_list **);
static int unmovable_p (rtx *, void *);
static int evaluate_deps (basic_block, rtx, struct df_link *,
			  rtx, struct movable_list **);
static void estimate_lifetime (struct ref *, int *, int *);
static void determine_move_depth (struct loops *loops, struct movable_list *);
static void move_movables (struct movable_list *);
static void free_movables (struct movable_list *);
static void prescan_loop (struct loop *);
static void note_addr_stored (rtx, rtx, void *);
static void estimate_replacement_possibilities (rtx, struct loop *, rtx,
						int *, rtx *);
static void free_loop_info (struct loop *);
static void make_dependency_substs (rtx *, struct movable *, int);
static void hoist_insn_to_depth (struct loop *, int, rtx, int);
static void replace_insn_by (rtx, rtx);
static void free_movable_list (struct movable_list *);
static void make_single_target_m (struct movable *);
extern void dump_movable (FILE *, struct movable *);
extern void dump_movables (FILE *, struct movable_list *);

/* Find movables in LOOPS.  */
struct movable_list *
find_movables (struct loops *loops)
{
  unsigned i;
  struct loop *loop;
  struct movable_list *movables, *m;

  /* Reg_scan call is needed by alias analysis.  */
  reg_scan (get_insns (), max_reg_num (), 1);
  init_alias_analysis ();

  /* Unused.  I am not quite sure how such hack could ever work, but
     lets leave it here for case someone insisted on resurrecting it.  */
  regs_may_share = NULL;

  /* Initialize global structures.  */
  n_bb_insns = xmalloc (sizeof (int) * last_basic_block);
  for (i = 0; i < (unsigned) last_basic_block; i++)
    n_bb_insns[i] = 0;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop)
	continue;

      prescan_loop (loop);
    }

  /* Find the movables and determine where to move them.  */
  movables = NULL;
  for (i = 0; i < (unsigned) n_basic_blocks; i++)
    if (block_dominance_order[i]->loop_father != loops->tree_root)
      detect_bb_movables (block_dominance_order[i], &movables);

  for (m = movables; m; m = m->next)
    make_single_target_m (m->elt);
  determine_move_depth (loops, movables);

  /* Cleanup.  */
  free (n_bb_insns);
  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop)
	continue;

      free_loop_info (loop);
    }
  end_alias_analysis ();

  return movables;
}

/* Move the MOVABLES out of LOOPS.  */
void
loops_invariant_motion (struct loops *loops ATTRIBUTE_UNUSED,
			struct movable_list *movables)
{
  move_movables (movables);
  free_movables (movables);
}

/* Record that a memory reference X is being set.  */
static void
note_addr_stored (rtx x, rtx z ATTRIBUTE_UNUSED, void *data ATTRIBUTE_UNUSED)
{
  struct loop_info *loop_info = data;
  rtx y;

  if (x == 0 || GET_CODE (x) != MEM)
    return;

  /* BLKmode MEM means all memory is clobbered.  */
  if (GET_MODE (x) == BLKmode)
    {
      if (RTX_UNCHANGING_P (x))
	loop_info->unknown_constant_address_altered = 1;
      else
	loop_info->unknown_address_altered = 1;
      return;
    }

  for (y = loop_info->store_mems; y; y = XEXP (y, 1))
    if (rtx_equal_p (XEXP (y, 0), x))
      return;

  loop_info->store_mems = gen_rtx_EXPR_LIST (0, x,
					     loop_info->store_mems);
}

/* Scan a loop setting the elements `body', `bb_after_exit', `bb_after_call',
   `has_call', `has_nonconst_call', `unknown_address_altered',
   `unknown_constant_address_altered'.  Fill in the list `store_mems'.  */
static void
prescan_loop (struct loop *loop)
{
  unsigned i, j;
  rtx insn;
  basic_block bb;
  basic_block *blocks_with_call =
	  xmalloc (sizeof (basic_block) * loop->num_nodes);
  edge *exits;
  unsigned n_blocks_with_call = 0, n_exits;
  struct df_link *def;

  loop->info = xmalloc (sizeof (struct loop_info));
  loop->info->body = get_loop_body (loop);
  loop->info->bb_after_exit = sbitmap_alloc (last_basic_block);
  sbitmap_zero (loop->info->bb_after_exit);
  loop->info->bb_after_call = sbitmap_alloc (last_basic_block);
  sbitmap_zero (loop->info->bb_after_call);
  loop->ninsns = 0;

  loop->info->has_call = 0;
  loop->info->has_nonconst_call = 0;

  loop->info->unknown_address_altered = 0;
  loop->info->unknown_constant_address_altered = 0;
  loop->info->store_mems = NULL_RTX;

  exits = get_loop_exit_edges (loop, &n_exits);
  for (i = 0; i < loop->num_nodes; i++)
    {
      int k = 0;
      bb = loop->info->body[i];

      FOR_BB_INSNS (bb, insn)
	{
	  if (INSN_P (insn))
	    {
	      k++;
	      for (def = DF_INSN_DEFS (loop_df, insn); def; def = def->next)
		DF_REF_AUX_MOVABLE (def->ref) = NULL;
	    }
	  switch (GET_CODE (insn))
	    {
	    case CALL_INSN:
	      if (! CONST_OR_PURE_CALL_P (insn))
		{
		  loop->info->unknown_address_altered = 1;
		  loop->info->has_nonconst_call = 1;
		  if (!n_blocks_with_call
		      || blocks_with_call[n_blocks_with_call - 1] != bb)
		    blocks_with_call[n_blocks_with_call++] = bb;
		}
	      else if (pure_call_p (insn))
		loop->info->has_nonconst_call = 1;
	      loop->info->has_call = 1;
	      break;

	    case JUMP_INSN:
	    case INSN:
	      note_stores (PATTERN (insn), note_addr_stored, loop->info);
	      break;

	    default:
	      break;
	    }
	}
      loop->ninsns += k;
      n_bb_insns[bb->index] = k;
    }

  /* Fill bb_after_call and bb_after_exit bitmaps.  */
  for (i = 0; i < loop->num_nodes; i++)
    {
      bb = loop->info->body[i];

      for (j = 0; j < n_exits; j++)
	if (!fast_dominated_by_p (exits[j]->src, bb))
	  {
	    SET_BIT (loop->info->bb_after_exit, bb->index);
	    break;
	  }

      for (j = 0; j < n_blocks_with_call; j++)
	if (!fast_dominated_by_p (blocks_with_call[j], bb))
	  {
	    SET_BIT (loop->info->bb_after_call, bb->index);
	    break;
	  }
    }
  free (exits);
  free (blocks_with_call);

  /* BLKmode MEMs are added to LOOP_STORE_MEM as necessary so
     that we can use true_dependence to determine what is really clobbered.  */
  if (loop->info->unknown_address_altered)
    {
      rtx mem = gen_rtx_MEM (BLKmode, const0_rtx);

      loop->info->store_mems
	= gen_rtx_EXPR_LIST (0, mem, loop->info->store_mems);
    }
  if (loop->info->unknown_constant_address_altered)
    {
      rtx mem = gen_rtx_MEM (BLKmode, const0_rtx);

      RTX_UNCHANGING_P (mem) = 1;
      loop->info->store_mems
	= gen_rtx_EXPR_LIST (0, mem, loop->info->store_mems);
    }
}

/* Checks whether it is ok to hoist the possibly invariant set of REG
   in INSN out of LOOP -- then REPLACEABLE is set to 1, and whether
   there is just a single use inside LOOP (then it is set to SINGLE_USE).  */
static void
estimate_replacement_possibilities (rtx reg, struct loop *loop, rtx insn,
				    int *replaceable, rtx *single_use)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  int maybe_never = TEST_BIT (loop->info->bb_after_exit, bb->index);
  struct df_link *def, *use, *ddef;
  int n_uses = 0;

  for (def = DF_INSN_DEFS (loop_df, insn);
       DF_REF_REGNO (def->ref) != REGNO (reg);
       def = def->next)
    continue;

  *replaceable = 1;
  *single_use = NULL;

  /* Check all the uses of the reg.  */
  for (use = loop_df->regs[REGNO (reg)].uses; use; use = use->next)
    {
      bb = DF_REF_BB (use->ref);

      /* All uses inside loop, as well as uses outside of it in case def
	 does not have to be executed when loop is entered, must be dominated
	 by the def.  */
      for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
	if (ddef->ref == def->ref)
	  break;
      if (ddef)
	{
	  if (maybe_never || flow_bb_inside_loop_p (loop, bb))
	    if (DF_REF_CHAIN (use->ref)->next)
	      *replaceable = 0;
	  if (!flow_bb_inside_loop_p (loop, bb)
	      || DF_REF_CHAIN (use->ref)->next)
	    n_uses = 2;
	  else
	    {
	      n_uses++;
	      *single_use = DF_REF_INSN (use->ref);
	    }
	}

      /* Shortcut for defs that are used on many places.  */
      if (!*replaceable && n_uses > 1)
	{
	  *single_use = NULL;
	  return;
	}
    }

  if (n_uses > 1)
    *single_use = NULL;
}

/* Tries to estimate LIFETIME of reference DEF as well as whether it is
   local to the loop it belongs to.  */
static void
estimate_lifetime (struct ref *def, int *local, int *lifetime)
{
  basic_block bb = DF_REF_BB (def);
  struct loop *loop = bb->loop_father;
  basic_block *pbb = xmalloc (sizeof (basic_block) * loop->num_nodes);
  int n, i;
  rtx insn = DF_REF_INSN (def);
  int luid = DF_INSN_LUID (loop_df, insn);
  int mluid = luid, uluid;
  struct df_link *use, *ddef;
  bitmap bbs = BITMAP_XMALLOC ();

  /* We conclude that def is local if it dominates all its uses
     and all of the uses belong to the same loop as def.  */
  *local = 1;
  for (use = loop_df->regs[DF_REF_REGNO (def)].uses; use; use = use->next)
    {
      for (ddef = DF_REF_CHAIN (use->ref); ddef; ddef = ddef->next)
	if (ddef->ref == def)
	  break;
      if (!ddef)
	continue;

      if (!flow_bb_inside_loop_p (loop, DF_REF_BB (use->ref))
	  || !fast_dominated_by_p (DF_REF_BB (use->ref), bb)
	  || (DF_REF_BB (use->ref) == bb
	      && DF_INSN_LUID (loop_df, DF_REF_INSN (use->ref)) <= luid))
	{
	  *local = 0;
	  break;
	}

      /* Make some estimate of lifetime.  */
      uluid = DF_INSN_LUID (loop_df, DF_REF_INSN (use->ref));
      if (bb != DF_REF_BB (use->ref))
	{
	  n = dfs_enumerate_from (DF_REF_BB (use->ref), true, def_enum_to_p,
				  pbb, loop->num_nodes, bb);
	  for (i = 1; i < n; i++)
	    bitmap_set_bit (bbs, pbb[i]->index);
	  mluid = DF_INSN_LUID (loop_df, bb->end);
	}
      else if (uluid > mluid)
	mluid = uluid;
    }

  if (!*local)
    {
      for (use = loop_df->regs[DF_REF_REGNO (def)].uses;
	   use;
	   use = use->next)
	loop = find_common_loop (loop, DF_REF_BB (use->ref)->loop_father);
      for (ddef = loop_df->regs[DF_REF_REGNO (def)].defs;
	   ddef;
	   ddef = ddef->next)
	loop = find_common_loop (loop, DF_REF_BB (ddef->ref)->loop_father);
      *lifetime = loop->ninsns;
    }
  else
    {
      *lifetime = mluid - luid;
      EXECUTE_IF_SET_IN_BITMAP (bbs, 0, i,
	{
	  *lifetime += n_bb_insns[i];
	});
    }

  free (pbb);
  BITMAP_XFREE (bbs);
}

/* Uses heuristics to determine the depth to that we should move the
   MOVABLES.  */
static void
determine_move_depth (struct loops *loops, struct movable_list *movables)
{
  struct movable **marray;
  struct movable *act;
  int n_movables, depth, d, am, i;
  struct movable_gain **gains;
  int *avail_regs;
  int *niter, aniter;
  struct movable_list *m, *m1;
  struct loop *loop, *tloop;
  rtx p;
  int n_sets;
  int best_gain, best_depth, act_gain, act_lifespan;

  /* We use the following algorithm: for reach movable and each depth
     it can be moved to, we estimate gains of move in the following cases:
     -- there will be enough registers
     -- there will not be enough registers

     To savings we add costs of movables that single_target it.
     Rationale -- we want to move costly chains of computations out.

     The gain in the first case is determined this way:

     gain = (savings + replaceable) * n_iter - cost
     Rationale -- old cost is savings + n_sets, new one n_sets - replaceable.

     In the second case, if local:
     gain = (savings - (SPILL_COST - 1) * n_sets) * n_iter - cost
     Rationale -- replacing here is not worthwhile; old cost is
	savings + n_sets, new one SPILL_COST * n_sets
     In the second case, if !local:
     gain = (savings - (SPILL_COST - 1) * n_sets
	     + SPILL_COST * replaceable) * n_iter - cost
     Rationale -- old cost savings + n_sets, new one
	SPILL_COST * (n_sets - replaceable)

     Additionally, we count correction for every set that has exactly one
     use and this use is moved to the same level as well.

     In the first case, 
     delta_gain = 0
     Rationale -- we gain nothing, except for shorter lifespan, which we do
	not take into account here

     In the second case,
     delta_gain = SPILL_COST * n_iter
     Rationale -- the sets don't have to be done there.

     Secondly, we estimate effect on number of available registers
     If local, delta_regs = 1
     If !local, delta_regs = 1 - replaceable (i.e. we support merging regs
       into one, if their lifespans overlap)
     
     and on change of lifespans of registers
     delta_lifespan = 0 if all defs are used exactly once in movables that are
			also moved
     delta_lifespan = replaceable / n_sets * old_lifespan - size of target loop
     Rationale: we add a new reg, but also eliminate the replaced ones; since
       we do not keep exact track of lifespans, we expect them all to be same.

     For each loop we calculate an estimate number of free regs (from total
     number of regs available and size of loop, i.e. quite stupidly).

     Then we pass the list of movables from the end (so that we know positions
     of all movables that depend on us); we decide whether to move further up
     depending on costs, changes in lifetime and number of registers that we
     have.
     
     ??? It would be better to take actual frequencies of execution of the
     insns into account, rather then expecting that they are executed in
     every iteration.
     
     ??? Perhaps we should not optimize cold areas.
     
     ??? We should somehow prefer movables with high gain.  */

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file, "Movables:\n\n");
      dump_movables (rtl_dump_file, movables);
    }

  if (flag_move_all_movables)
    {
      for (m = movables; m; m = m->next)
	m->elt->move_depth = m->elt->depth;
      return;
    }
  n_movables = 0;
  for (m = movables; m; m = m->next)
    {
      n_movables++;

      /* Propagate savings to subsequent movables.  */
      for (m1 = m->elt->single_targets_m; m1; m1 = m1->next)
	if (m1->elt)
	  m1->elt->savings += m->elt->savings;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "\nLoops:\n\n");

  /* Count the information for loops.  */
  avail_regs = xmalloc (loops->num * sizeof (int));
  niter = xmalloc (loops->num * sizeof (int));
  niter[0] = 1;
  for (i = 1; i < (int) loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop)
	continue;
      niter[i] = expected_loop_iterations (loop);
      if (niter[i] == 0)
	niter[i] = 1;

      avail_regs[i] = N_USABLE_REGS
		      - (loop->info->has_call ? N_CALL_REGS : 0)
		      - REGS_PER_LOOP_LEVEL * get_loop_level (loop) 
		      - loop->ninsns / INSNS_PER_REG;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, " %d niter %d regs %d\n",
		 i, niter[i], avail_regs[i]);
    }
  
  /* Allocate the structures and count the costs.  */
  gains = xmalloc (n_movables * sizeof (struct movable_gain *));
  marray = xmalloc (n_movables * sizeof (struct movable *));

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "\nCosts:\n\n");
  for (am = 0, m = movables; m; m = m->next, am++)
    {
      act = m->elt;
      marray[am] = act;
      depth = act->loop->depth;
      act->move_depth = depth;
      gains[am] = xmalloc (depth * sizeof (struct movable_gain));

      n_sets = 0;
      for (p = act->insns; p; p = XEXP (p, 1))
	n_sets++;

      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "  movable %p (%d sets, total savings %d):\n",
		 (void *) act, n_sets, act->savings);
      aniter = 1;
      tloop = act->loop;
      for (d = depth - 1; d >= act->depth; tloop = tloop->outer, d--)
	{
	  aniter *= niter[tloop->num];
	  if (aniter > 10000)
	    aniter = 10000;
	  
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "    depth %d, niter %d:", d, aniter);
	  gains[am][d].gain_in_reg =
		  (act->savings + act->replaceable) * aniter - act->cost;
	  if (act->local)
	    {
	      gains[am][d].gain_spill =
		      (act->savings - (SPILL_COST - 1) * n_sets) * aniter
		      - act->cost;
	      gains[am][d].req_regs = 1;
	    }
	  else
	    {
	      gains[am][d].gain_spill =
		      (act->savings
		       - (SPILL_COST - 1) * n_sets
		       + SPILL_COST * act->replaceable) * aniter
		      - act->cost;
	      gains[am][d].req_regs = 1 - act->replaceable;
	    }
	  gains[am][d].moved_st_bonus = SPILL_COST * aniter;
	  gains[am][d].delta_lifespan =
		  (act->replaceable * act->lifetime) / n_sets
		  - tloop->ninsns;
	  if (rtl_dump_file)
	    {
	      fprintf (rtl_dump_file, " gain_in_reg %d",
	  	       gains[am][d].gain_in_reg);
	      fprintf (rtl_dump_file, " gain_spill %d",
	  	       gains[am][d].gain_spill);
	      fprintf (rtl_dump_file, " mst_bonus %d",
	  	       gains[am][d].moved_st_bonus);
	      fprintf (rtl_dump_file, " req_regs %d",
	  	       gains[am][d].req_regs);
	      fprintf (rtl_dump_file, " lifespan %d",
	  	       gains[am][d].delta_lifespan);
	      fprintf (rtl_dump_file, "\n");
	    }
	}

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "\n");
    }
  free (niter);

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "\nDecision:\n\n");
  for (am = n_movables - 1; am >= 0; am--)
    {
      act = marray[am];
      best_gain = 0;
      best_depth = act->move_depth;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "  movable %p:\n", (void *) act);

      if (act->move_depth == act->loop->depth)
	d = act->loop->depth - 1;
      else
	d = act->move_depth;

      /* Choose the best depth.  */
      for (; d >= act->depth; d--)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "    depth %d\n", d);
	  act_gain = 0;
	  if (d == act->loop->depth - 1)
	    tloop = act->loop;
	  else
	    tloop = act->loop->pred[d + 1];
	  if (avail_regs[tloop->num] >= gains[am][d].req_regs)
	    {
	      act_gain = gains[am][d].gain_in_reg;
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "      enough regs, gain %d\n", act_gain);
	    }
	  else
	    {
	      act_gain = gains[am][d].gain_spill;
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "      few regs, gain %d", act_gain);
	      for (m1 = act->single_targets_m; m1; m1 = m1->next)
		if (m1->elt && m1->elt->move_depth == d)
		  act_gain += gains[am][d].moved_st_bonus;
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "(%d with bonus)\n", act_gain);
	    }

	  for (m1 = act->single_targets_m; m1; m1 = m1->next)
	    if (!m1->elt || m1->elt->move_depth != d)
	      break;
	  act_lifespan = m1 ? gains[am][d].delta_lifespan : 0;
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "      lifespan change %d\n", act_lifespan);

	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "      needs %d extra regs\n",
		     gains[am][d].req_regs);
	  act_gain = GAIN_WEIGHT * act_gain
		     + LIFESPAN_WEIGHT * act_lifespan
		     - REG_WEIGHT * gains[am][d].req_regs;
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "      total gain %d\n", act_gain);

	  if (act_gain > best_gain)
	    {
	      best_gain = act_gain;
	      best_depth = d;
	    }
	}
      act->move_depth = best_depth;
      if (rtl_dump_file && best_depth != act->loop->depth)
	fprintf (rtl_dump_file, "Move %p to depth %d.\n",
		 (void *) act, best_depth);

      /* Update numbers of available registers.  */
      if (best_depth != act->loop->depth)
	{
	  for (tloop = act->loop;
	       tloop->depth != best_depth;
	       tloop = tloop->outer)
	    avail_regs[tloop->num] -= gains[am][best_depth].req_regs;
	}

      /* Force dependencies to move low enough.  */
      for (m1 = act->depends; m1; m1 = m1->next)
	if (m1->elt->move_depth > best_depth)
	  m1->elt->move_depth = best_depth;
      free (gains[am]);
    }

  free (gains);
  free (marray);
  free (avail_regs);
}

/* Called through for_each_rtx, evaluates whether the EXPR is movable.
   DST is register that we want to prove invariant.  To ADDITIONAL_DEPS
   add memories that must be invariant.  */
static rtx additional_deps = NULL_RTX;
static int
unmovable_p (rtx *expr, void *dst)
{
  rtx dest = dst;
  rtx x = *expr;
  enum rtx_code code;

  if (x == 0)
    return 0;
  code = GET_CODE (x);

  switch (code)
    {
    case SET:
      return SET_DEST (x) != dest;

    case CONST_INT:
    case CONST_DOUBLE:
    case SYMBOL_REF:
    case CONST:
    case LABEL_REF:
    case REG:
      return 0;

    case PC:
    case CC0:
    case UNSPEC_VOLATILE:
      return 1;

    case MEM:
      /* Volatile memory references must be rejected.  */
      if (MEM_VOLATILE_P (x))
	return 1;

      /* Arrange for check whether there is any dependence between any store and
	 this load.  */
      additional_deps = alloc_EXPR_LIST (0, x, additional_deps);
      return 0;

    case ASM_OPERANDS:
      /* Don't mess with insns declared volatile.  */
      if (MEM_VOLATILE_P (x))
	return 1;

    default:
      return 0;
    }
}

/* For each of the USES (in INSN in BB), determine how far up we may move it. 
   MEM_DEPS is list of mems in insn, for those we must also take aliasing
   into account.  For invariant uses, record corresponding movables to
   list DEPS.  */
static int
evaluate_deps (basic_block bb, rtx insn,
	       struct df_link *uses, rtx mem_deps,
	       struct movable_list **deps)
{
  int level = 0, alevel;
  struct movable_list *new;
  struct movable *dep;
  struct df_link *defs;
  struct loop *loop = bb->loop_father, *def_loop;
  basic_block def_bb;
  rtx def_insn;
  rtx mem, smem;

  *deps = NULL;

  for (; uses; uses = uses->next)
    {
      /* There are four cases:

	 0) There is no def.  Then we may move as high as we want.
	 1) There is exactly one def that dominates us -- then we
	    take a level of its movable (or its level if it has none).
	 2) All defs are in outer loops; then we take level of innermost
	    of superloops of them and our loop.
	 3) Otherwise we fail.  */
      defs = DF_REF_CHAIN (uses->ref);
      if (!defs)
	continue;
      if (!defs->next)
	{
	  def_insn = DF_REF_INSN (defs->ref);
	  def_bb = DF_REF_BB (defs->ref);
	  def_loop = find_common_loop (def_bb->loop_father, loop);
	  if (fast_dominated_by_p (bb, DF_REF_BB (uses->ref))
	      && (def_bb != bb
		  || DF_INSN_LUID (loop_df, insn)
		     > DF_INSN_LUID (loop_df, def_insn)))
	    {
	      if (!DF_REF_AUX_MOVABLE (defs->ref)
		  /* Unless the loop with definition is comparable with
		     our loop, the recorded dependency would make no sense.
		     We may be able to move the movable without moving
		     the "dependency", for example.  */
		  || (def_loop != def_bb->loop_father
		      && def_loop != loop))
		{
		  if (def_loop == loop)
		    return loop->depth;

		  if (def_loop->depth > level)
		    level = def_loop->depth;
		  continue;
		}

	      dep = DF_REF_AUX_MOVABLE (defs->ref);
	      if (dep->depth >= loop->depth)
		return loop->depth;
	      new = xmalloc (sizeof (struct movable_list));
	      new->elt = dep;
	      new->next = *deps;
	      *deps = new;

	      if (dep->depth > level)
		level = dep->depth;

	      continue;
	    }
	}

      for (; defs; defs = defs->next)
	{
	  def_bb = DF_REF_BB (defs->ref);
	  def_loop = find_common_loop (def_bb->loop_father, loop);
	      
	  if (def_loop == loop)
	    return loop->depth;

	  if (def_loop->depth > level)
	    level = def_loop->depth;
	}
    }

  /* Now we know the depth to that arguments (including adresses of mems) 
     are invariant.  Let's take aliasing into account now.  */
  for (alevel = loop->depth; alevel > level; alevel--, loop = loop->outer)
    for (mem = mem_deps; mem; mem = XEXP (mem, 1))
      for (smem = loop->info->store_mems; smem; smem = XEXP (smem, 1))
	if (true_dependence (XEXP (smem, 0), VOIDmode, XEXP (mem, 0),
			     rtx_varies_p))
	  return alevel;

  return alevel;
}

/* Find movable invariants in basic block BB and add them to the list
   of MOVABLES.  */
static void
detect_bb_movables (basic_block bb, struct movable_list **movables)
{
  rtx insn;
  int after_call = TEST_BIT (bb->loop_father->info->bb_after_call, bb->index);

  FOR_BB_INSNS (bb, insn)
    {
      if (!INSN_P (insn))
	continue;

      /* Prevent removal of the insn with libcall/retval note.  */
      if (!find_reg_note (insn, REG_RETVAL, NULL_RTX)
	  && !find_reg_note (insn, REG_LIBCALL, NULL_RTX))
	detect_movable_set (bb, insn, after_call, movables);

      if (GET_CODE (insn) == CALL
	  && !CONST_OR_PURE_CALL_P (insn))
	after_call = true;
    }
}

/* Tries to prove that INSN is movable, and if so, add it to list of MOVABLES.
   BB is the basic block INSN belongs to.  AFTER_CALL is true if there is a
   call that does not have to return before INSN in BB.  */
static void
detect_movable_set (basic_block bb, rtx insn, int after_call,
		    struct movable_list **movables)
{
  rtx set = single_set (insn);
  rtx src, dest, value;
  struct movable_list *deps, **m;
  struct movable *m1;
  int level, local, lifetime;
  struct df_link *def;
  int cost, may_trap;
  struct loop *loop = bb->loop_father;
  int replaceable;
  rtx single_target;

  if (!set)
    return;
  dest = SET_DEST (set);
  src = SET_SRC (set);
  if (!REG_P (dest)
      || REGNO (dest) < FIRST_PSEUDO_REGISTER)
    return;
    
  value = get_def_value (insn, REGNO (dest));
  if (!value
      || fast_expr_mentions_operator_p (value, ITERATION))
    return;

  /* If the expression may trap and we may exit without reaching
     it, we may not move it.  */
  may_trap = may_trap_p (src);
  if ((after_call || TEST_BIT (loop->info->bb_after_exit, bb->index))
      && (may_trap = may_trap_p (src)))
    return;

  if (for_each_rtx (&PATTERN (insn), unmovable_p, dest))
    {
      free_EXPR_LIST_list (&additional_deps);
      return;
    }

  level = evaluate_deps (bb, insn, DF_INSN_USES (loop_df, insn),
			 additional_deps, &deps);
  free_EXPR_LIST_list (&additional_deps);
  if (level == loop->depth)
    {
      free_movable_list (deps);
      return;
    }
  if (may_trap)
    {
      struct loop *ploop;

      /* If the expression may trap, we must additionally ensure that
	 it is always executed after entering the loop.  */
      while (level < loop->depth - 1)
	{
	  ploop = loop->pred[level + 1];
	  if (!TEST_BIT (loop->info->bb_after_exit, bb->index)
	      && !TEST_BIT (loop->info->bb_after_call, bb->index))
	    break;
	  level++;
	}
    }

  value = substitute_into_expr (value, iv_interesting_reg,
				initial_values[loop->num], SIE_SIMPLIFY);
  cost = rtx_cost (src, SET);

  for (def = DF_INSN_DEFS (loop_df, insn);
       DF_REF_REGNO (def->ref) != REGNO (dest);
       def = def->next)
    continue;
  estimate_lifetime (def->ref, &local, &lifetime);
  estimate_replacement_possibilities (dest, loop, insn,
				      &replaceable, &single_target);
  
  /* Now we are sure that it is movable on some higher level and know its
     dependencies.  Let's find whether there is not already equivalent
     movable.  */
  for (m = movables; *m; m = &(*m)->next)
    {
      if (!value)
	continue;

      m1 = (*m)->elt;

      /* It is in principle possible to merge equivalent movables from
	 different loops; but it would require some extra care, so don't
	 do it for now.  */
      if (loop != m1->loop
	  || !m1->value
	  || m1->mode != GET_MODE (dest))
	continue;

      /* We don't have to worry about VALUE_ATs (those from outside
	 of loop are safe, and those inside must be movables, i.e.
	 invariants.  */
      if (!rtx_equal_p (value, m1->value))
	continue;

      /* Merge the movables.  */
      m1->insns = alloc_INSN_LIST (insn, m1->insns);
      m1->set_dests = alloc_EXPR_LIST (0, dest, m1->set_dests);
      m1->lifetime += lifetime;
      if (!local)
	m1->local = 0;

      m1->savings += cost;
      m1->replaceable += replaceable;
      m1->single_targets = alloc_INSN_LIST (single_target,
						m1->single_targets);
      DF_REF_AUX_MOVABLE (def->ref) = m1;
      free_movable_list (deps);
      /* ??? We might like to replace the expression with the new one if
	 it is cheaper; it is in principle possible, but we would have
	 to ensure that we are not creating forward dependencies;
	 the typical case is
	     
	 x = comp
	 y = x
	     
	 where we definitely do not want to take the second cheap
	 computation.  */
      return;
    }

  m1 = xmalloc (sizeof (struct movable));
  m1->insn = insn;
  m1->insns = alloc_INSN_LIST (insn, NULL_RTX);
  m1->set_dests = alloc_EXPR_LIST (0, dest, NULL_RTX);
  m1->set_src = copy_rtx (src);
  m1->mode = GET_MODE (dest);
  m1->temp = NULL_RTX;
  m1->value = value;
  m1->depends = deps;
  m1->depth = level;
  m1->loop = loop;
  m1->local = local;
  m1->lifetime = lifetime;
  m1->cost = cost;
  m1->savings = cost;
  m1->move_depth = loop->depth;
  m1->replaceable = replaceable;
  m1->single_targets = alloc_INSN_LIST (single_target, NULL);
  m1->single_targets_m = NULL;
  DF_REF_AUX_MOVABLE (def->ref) = m1;

  *m = xmalloc (sizeof (struct movable_list));
  (*m)->elt = m1;
  (*m)->next = NULL;
}

/* Move the MOVABLES to the corresponding preheaders.  Because of the ordering
   we know we already have moved the prerequisities when we are moving the
   dependent movables -- we just have to ensure that we substitute the
   regs.  */
static void
move_movables (struct movable_list *movables)
{
  struct movable *act;
  struct movable_list *dep;
  int max_regno = max_reg_num ();
  rtx *substitution = xcalloc (max_regno, sizeof (rtx));
  rtx seq, set;
  rtx insn, set_dest;
  int n_moved = 0, l_moved = 0;

  for (; movables; movables = movables->next)
    {
      act = movables->elt;
      if (act->move_depth == act->loop->depth)
	continue;

      if (getenv ("NOPT") && n_moved >= atoi (getenv ("NOPT")))
	break;
      act->temp = gen_reg_rtx (act->mode);

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Moving movable %p to depth %d.\n",
		 (void *) act, act->depth);

      /* Prepare the substitution for moved dependencies.  */
      for (dep = act->depends; dep; dep = dep->next)
	make_dependency_substs (substitution, dep->elt, false);

      /* Substitute and emit the load to temp.  */
      act->insn = replace_regs (act->insn, substitution, max_regno,
				true);
      set = single_set (act->insn);
      SET_DEST (set) = act->temp;
      hoist_insn_to_depth (act->loop, act->move_depth, PATTERN (act->insn),
			   true);

      /* Clear the substitutions.  */
      for (dep = act->depends; dep; dep = dep->next)
	make_dependency_substs (substitution, dep->elt, true);

      for (insn = act->insns, set_dest = act->set_dests;
	   insn;
	   insn = XEXP (insn, 1), set_dest = XEXP (set_dest, 1))
	{
	  start_sequence ();
	  emit_move_insn (XEXP (set_dest, 0), act->temp);
	  seq = get_insns ();
	  end_sequence ();

	  replace_insn_by (XEXP (insn, 0), seq);
	}

      n_moved++;
      l_moved += act->loop->depth - act->move_depth;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "%d movables moved for total of %d levels.\n\n",
	     n_moved, l_moved);
  free (substitution);
}

/* Release memory allocated for MOVABLES.  */
static void
free_movables (struct movable_list *movables)
{
  struct movable_list *next;
  struct movable *m;

  for (; movables; movables = next)
    {
      next = movables->next;
      m = movables->elt;

      free_INSN_LIST_list (&m->insns);
      free_INSN_LIST_list (&m->single_targets);
      free_EXPR_LIST_list (&m->set_dests);
      free_movable_list (m->depends);
      free_movable_list (m->single_targets_m);

      free (movables);
      free (m);
    }
}

/* Release a list of movables.  */
static void
free_movable_list (struct movable_list *movables)
{
  struct movable_list *next;

  for (; movables; movables = next)
    {
      next = movables->next;
      free (movables);
    }
}

/* Releases memory allocated for loop info of LOOP.  */
static void
free_loop_info (struct loop *loop)
{
  free (loop->info->body);
  free (loop->info->bb_after_exit);
  free (loop->info->bb_after_call);
  free_EXPR_LIST_list (&loop->info->store_mems);
  free (loop->info);
}

/* Prepare substitution SUBST for movable M (or clear it if CLEAR).  */
static void
make_dependency_substs (rtx *subst, struct movable *m, int clear)
{
  rtx reg;

  if (m->move_depth == m->loop->depth)
    return;

  for (reg = m->set_dests; reg; reg = XEXP (reg, 1))
    subst[REGNO (XEXP (reg, 0))] = clear ? NULL : m->temp;
}

/* Hoist insn sequence SEQ to DEPTH above LOOP; if PREHEADER is true,
   then to preheader, otherwise to landing pad.  */
static void
hoist_insn_to_depth (struct loop *loop, int depth, rtx seq, int preheader)
{
  struct loop *tgt = depth == loop->depth - 1 ? loop : loop->pred[depth + 1];
  rtx where;

  if (preheader)
    where = loop_preheader_edge (tgt)->src->end;
  else
    {
      where = block_label (tgt->landing_pad);
      if (NEXT_INSN (where)
	  && GET_CODE (NEXT_INSN (where)) == NOTE)
	where = NEXT_INSN (where);
    }

  emit_insn_after (seq, where);
}

/* Replace INSN by sequence SEQ.  */
static void
replace_insn_by (rtx insn, rtx seq)
{
  emit_insn_after (seq, insn);
  delete_insn (insn);
}

/* Finds movables for insns single_targets list of movable M.  */
static void
make_single_target_m (struct movable *m)
{
  rtx insn, set, dest;
  struct movable_list **last = &m->single_targets_m;
  struct movable *tgt;
  struct df_link *def;

  for (insn = m->single_targets; insn; insn = XEXP (insn, 1))
    {
      if (!XEXP (insn, 0)
	  || !(set = single_set (XEXP (insn, 0)))
	  || !REG_P (SET_DEST (set)))
	tgt = NULL;
      else
	{
	  dest = SET_DEST (set);
	  for (def = DF_INSN_DEFS (loop_df, XEXP (insn, 0)); ; def = def->next)
	    if (REGNO (dest) == DF_REF_REGNO (def->ref))
	      break;
	  tgt = DF_REF_AUX_MOVABLE (def->ref);
	}
      *last = xmalloc (sizeof (struct movable_list));
      (*last)->elt = tgt;
      last = &(*last)->next;
    }
  *last = NULL;
}

/* Dump information about movable M to FILE.  */
void
dump_movable (FILE *file, struct movable *m)
{
  rtx p;
  struct movable_list *m1;

  fprintf (file, "Movable %p:\n", (void *) m);

  fprintf (file, "  insn:\n");
  print_rtl_single (file, m->insn);
  fprintf (file, "\n");

  fprintf (file, "  in loop %d, invariant to depth %d\n",
	   m->loop->num, m->depth);
  
  fprintf (file, "  src:\n");
  print_rtl (file, m->set_src);
  fprintf (file, "\n");

  fprintf (file, "  cost: %d\n", m->cost);

  fprintf (file, "  value:\n");
  print_rtl (file, m->value);
  fprintf (file, "\n");

  fprintf (file, "  insns:");
  for (p = m->insns; p; p = XEXP (p, 1))
    fprintf (file, " %d", INSN_UID (XEXP (p, 0)));
  fprintf (file, "\n");
  
  fprintf (file, "  dests:\n");
  print_rtl (file, m->set_dests);
  fprintf (file, "\n");

  fprintf (file, "  savings %d lifetime %d replaceable %d %s\n",
	   m->savings, m->lifetime, m->replaceable,
	   m->local ? "local" : "global");

  fprintf (file, "  depends on:");
  for (m1 = m->depends; m1; m1 = m1->next)
    fprintf (file, " %p", (void *) m1->elt);
  fprintf (file, "\n");

  fprintf (file, "  single targets:");
  for (m1 = m->single_targets_m; m1; m1 = m1->next)
    fprintf (file, " %p", (void *) m1->elt);
  fprintf (file, "\n\n");
}

/* Dump information about movables M to FILE.  */
void
dump_movables (FILE *file, struct movable_list *m)
{
  for (; m; m = m->next)
    dump_movable (file, m->elt);
}
