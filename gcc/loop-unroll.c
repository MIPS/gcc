/* Loop unrolling and peeling.
   Copyright (C) 2002 Free Software Foundation, Inc.

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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "cfglayout.h"
#include "params.h"
#include "output.h"
#include "expr.h"

static void decide_peel_simple PARAMS ((struct loops *, struct loop *, int));
static void decide_peel_once_rolling PARAMS ((struct loops *, struct loop *, int));
static void decide_peel_completely PARAMS ((struct loops *, struct loop *, int));
static void decide_unroll_stupid PARAMS ((struct loops *, struct loop *, int));
static void decide_unroll_constant_iterations PARAMS ((struct loops *, struct loop *, int));
static void decide_unroll_runtime_iterations PARAMS ((struct loops *, struct loop *, int));
static void peel_loop_simple PARAMS ((struct loops *, struct loop *));
static void peel_loop_completely PARAMS ((struct loops *, struct loop *));
static void unroll_loop_stupid PARAMS ((struct loops *, struct loop *));
static void unroll_loop_constant_iterations PARAMS ((struct loops *,
						     struct loop *));
static void unroll_loop_runtime_iterations PARAMS ((struct loops *,
						    struct loop *));

/* Unroll and peel LOOPS, according to decision already stored in
   lpt_decision field of the loop structure.  */
void
unroll_and_peel_loops (loops)
     struct loops *loops;
{
  struct loop *loop, *next;
  int check;

  loop = loops->tree_root;
  while (loop->inner)
    loop = loop->inner;

  /* Scan the loops, inner ones first.  */
  while (loop != loops->tree_root)
    {
      if (loop->next)
	{
	  next = loop->next;
	  while (next->inner)
	    next = next->inner;
	}
      else
	next = loop->outer;

      check = 1;
      switch (loop->lpt_decision.decision)
	{
	case LPT_PEEL_COMPLETELY:
	  peel_loop_completely (loops, loop);
	  break;
	case LPT_PEEL_SIMPLE:
	  peel_loop_simple (loops, loop);
	  break;
	case LPT_UNROLL_CONSTANT:
	  unroll_loop_constant_iterations (loops, loop);
	  break;
	case LPT_UNROLL_RUNTIME:
	  unroll_loop_runtime_iterations (loops, loop);
	  break;
	case LPT_UNROLL_STUPID:
	  unroll_loop_stupid (loops, loop);
	  break;
	case LPT_NONE:
	  check = 0;
	  break;
	default:
	  abort ();
	}
      if (check)
	{
#ifdef ENABLE_CHECKING
	  verify_dominators (loops->cfg.dom);
	  verify_loop_structure (loops);
#endif
	}
      loop = next;
    }
}

/* Decide whether unroll or peel and how much.  */
void
decide_unrolling_and_peeling (loops, flags)
     struct loops *loops;
     int flags;
{
  struct loop *loop = loops->tree_root, *next, *subloop;

  while (loop->inner)
    loop = loop->inner;

  /* Scan the loops, inner ones first.  */
  for (; loop != loops->tree_root; loop = next)
    {
      if (loop->next)
	{
	  next = loop->next;
	  while (next->inner)
	    next = next->inner;
	}
      else
	next = loop->outer;

      loop->lpt_decision.decision = LPT_NONE;
      /* Unless we are asked to, we do nothing (just fill the "do nothing"
	 decision so that other passes know it too).  */
      if (!flags)
	continue;
      loop->ninsns = num_loop_insns (loop);
      loop->av_ninsns = average_num_loop_insns (loop);

      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Considering loop %d\n", loop->num);

      /* This is an exception; doing this is always a win and it also decreases
	 a code size, so we want to try it always.  */
      decide_peel_once_rolling (loops, loop, flags);

      /* Do not optimize cold areas.  */
      if (!maybe_hot_bb_p (loop->header))
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, ";; Not considering loop, cold area\n");
	  continue;
	}

      /* Can the loop be manipulated?  */
      if (!can_duplicate_loop_p (loop))
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     ";; Not considering loop, cannot duplicate\n");
	  continue;
	}

      /* Skip non-innermost loops.  */
      if (loop->inner)
	{
	  /* An exception -- if the loops are completely peeled, consider
	     it anyway, as they will be eliminated.  */
	  for (subloop = loop->inner; subloop; subloop = subloop->next)
	    if (subloop->lpt_decision.decision != LPT_PEEL_COMPLETELY)
	      break;
	  if (subloop)
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file, ";; Not considering loop, is not innermost\n");
	      continue;
	    }

	  /* But in this case we must add a size of the unrolled subloops
	     to the size of the loop.  */
	  for (subloop = loop->inner; subloop; subloop = subloop->next)
	    {
	      basic_block preheader = loop_preheader_edge (subloop)->src;
	      unsigned ratio = loop->header->frequency == 0
		? BB_FREQ_MAX
		: (preheader->frequency * BB_FREQ_MAX) / loop->header->frequency;
	      loop->ninsns += subloop->ninsns * subloop->lpt_decision.times;
	      loop->av_ninsns +=
		subloop->av_ninsns * subloop->lpt_decision.times
		* ratio / BB_FREQ_MAX;
	    }
	}

      /* Try transformations one by one in decreasing order of
	 priority.  */

      if (loop->lpt_decision.decision == LPT_NONE)
	decide_peel_completely (loops, loop, flags);
      if (loop->lpt_decision.decision == LPT_NONE)
	decide_unroll_constant_iterations (loops, loop, flags);
      if (loop->lpt_decision.decision == LPT_NONE)
	decide_unroll_runtime_iterations (loops, loop, flags);
      if (loop->lpt_decision.decision == LPT_NONE)
	decide_unroll_stupid (loops, loop, flags);
      if (loop->lpt_decision.decision == LPT_NONE)
	decide_peel_simple (loops, loop, flags);
    }
}

/* Decide whether the loop is once rolling and suitable for complete
   peeling.  */
static void
decide_peel_once_rolling (loops, loop, flags)
     struct loops *loops ATTRIBUTE_UNUSED;
     struct loop *loop;
     int flags ATTRIBUTE_UNUSED;
{
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Considering peeling once rolling loop\n");

  /* Is the loop small enough?  */
  if ((unsigned) PARAM_VALUE (PARAM_MAX_ONCE_PEELED_INSNS) < loop->ninsns)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is too big\n");
      return;
    }

  /* Check number of iterations.  */
  if (!loop->has_desc
      || !loop->simple
      || loop->desc.assumptions
      || !loop->desc.const_iter
      || loop->desc.niter != 0)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Unable to prove that the loop rolls exactly once\n");
      return;
    }

  /* Success.  */
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Decided to peel exactly once rolling loop\n");
  loop->lpt_decision.decision = LPT_PEEL_COMPLETELY;
}

/* Decide whether the loop is suitable for complete peeling.  */
static void
decide_peel_completely (loops, loop, flags)
     struct loops *loops ATTRIBUTE_UNUSED;
     struct loop *loop;
     int flags ATTRIBUTE_UNUSED;
{
  unsigned npeel;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Considering peeling completely\n");

  /* Skip non-innermost loops.  */
  if (loop->inner)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is not innermost\n");
      return;
    }

  /* Do not peel cold areas.  */
  if (!maybe_hot_bb_p (loop->header))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, cold area\n");
      return;
    }

  /* Can the loop be manipulated?  */
  if (!can_duplicate_loop_p (loop))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 ";; Not considering loop, cannot duplicate\n");
      return;
    }

  /* npeel = number of iterations to peel.  */
  npeel = PARAM_VALUE (PARAM_MAX_COMPLETELY_PEELED_INSNS) / loop->ninsns;
  if (npeel > (unsigned) PARAM_VALUE (PARAM_MAX_COMPLETELY_PEEL_TIMES))
    npeel = PARAM_VALUE (PARAM_MAX_COMPLETELY_PEEL_TIMES);

  /* Is the loop small enough?  */
  if (!npeel)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is too big\n");
      return;
    }

  /* Check number of iterations.  */
  if (!loop->has_desc
      || !loop->simple
      || loop->desc.assumptions
      || !loop->desc.const_iter)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Unable to prove that the loop iterates constant times\n");
      return;
    }

  if (loop->desc.niter > npeel - 1)
    {
      if (rtl_dump_file)
      	{
	  fprintf (rtl_dump_file, ";; Not peeling loop completely, rolls too much (");
	  fprintf (rtl_dump_file, HOST_WIDEST_INT_PRINT_DEC,(HOST_WIDEST_INT) loop->desc.niter);
	  fprintf (rtl_dump_file, " iterations > %d [maximum peelings])\n", npeel);
	}
      return;
    }

  /* Success.  */
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Decided to peel loop completely\n");
  loop->lpt_decision.decision = LPT_PEEL_COMPLETELY;
}

/* Peel NPEEL iterations from LOOP, remove exit edges (and cancel the loop
   completely).  */
static void
peel_loop_completely (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  sbitmap wont_exit;
  unsigned HOST_WIDE_INT npeel;
  unsigned n_remove_edges, i;
  edge *remove_edges;
  struct loop_desc *desc = &loop->desc;
  
  npeel = desc->niter;

  if (npeel)
    {
      wont_exit = sbitmap_alloc (npeel + 1);
      sbitmap_ones (wont_exit);
      RESET_BIT (wont_exit, 0);
      if (desc->noloop_assumptions)
	RESET_BIT (wont_exit, 1);

      remove_edges = xcalloc (npeel, sizeof (edge));
      n_remove_edges = 0;

      if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, npeel,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ
		| (loop->histogram 
		   ? DLTHE_USE_HISTOGRAM_PROB
		   : DLTHE_USE_WONT_EXIT)))
	abort ();

      free (wont_exit);

      /* Remove the exit edges.  */
      for (i = 0; i < n_remove_edges; i++)
	remove_path (loops, remove_edges[i]);
      free (remove_edges);
    }
  
  /* Now remove the unreachable part of the last iteration and cancel
     the loop.  */
  remove_path (loops, desc->in_edge);

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Peeled loop completely, %d times\n",
	     (int) npeel);
}

/* Decide whether to unroll loop iterating constant number of times and how much.  */
static void
decide_unroll_constant_iterations (loops, loop, flags)
     struct loops *loops ATTRIBUTE_UNUSED;
     struct loop *loop;
     int flags;
{
  unsigned nunroll, nunroll_by_av, best_copies, best_unroll = -1, n_copies, i;

  if (!(flags & UAP_UNROLL))
    {
      /* We were not asked to, just return back silently.  */
      return;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Considering unrolling loop with constant number of iterations\n");

  /* nunroll = total number of copies of the original loop body in
     unrolled loop (i.e. if it is 2, we have to duplicate loop body once.  */
  nunroll = PARAM_VALUE (PARAM_MAX_UNROLLED_INSNS) / loop->ninsns;
  nunroll_by_av = PARAM_VALUE (PARAM_MAX_AVERAGE_UNROLLED_INSNS) / loop->av_ninsns;
  if (nunroll > nunroll_by_av)
    nunroll = nunroll_by_av;
  if (nunroll > (unsigned) PARAM_VALUE (PARAM_MAX_UNROLL_TIMES))
    nunroll = PARAM_VALUE (PARAM_MAX_UNROLL_TIMES);

  /* Skip big loops.  */
  if (nunroll <= 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is too big\n");
      return;
    }

  /* Check number of iterations.  */
  if (!loop->has_desc
      || !loop->simple
      || loop->desc.assumptions
      || !loop->desc.const_iter)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 ";; Unable to prove that the loop iterates constant times\n");
      return;
    }

  /* Check whether the loop rolls enough to consider.  */
  if (loop->desc.niter < 2 * nunroll)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unrolling loop, doesn't roll\n");
      return;
    }

  /* Success; now compute number of iterations to unroll.  */
  best_copies = 2 * nunroll + 10;

  i = 2 * nunroll + 2;
  if ((unsigned) i - 1 >= loop->desc.niter)
    i = loop->desc.niter - 2;

  for (; i >= nunroll - 1; i--)
    {
      unsigned exit_mod = loop->desc.niter % (i + 1);

      if (loop->desc.postincr)
	n_copies = exit_mod + i + 1;
      else if (exit_mod != (unsigned) i || loop->desc.noloop_assumptions)
	n_copies = exit_mod + i + 2;
      else
	n_copies = i + 1;

      if (n_copies < best_copies)
	{
	  best_copies = n_copies;
	  best_unroll = i;
	}
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; max_unroll %d (%d copies, initial %d).\n",
	     best_unroll + 1, best_copies, nunroll);

  loop->lpt_decision.decision = LPT_UNROLL_CONSTANT;
  loop->lpt_decision.times = best_unroll;
}

/* Unroll LOOP with constant number of iterations described by DESC.
   MAX_UNROLL is maximal number of allowed unrollings.  */
static void
unroll_loop_constant_iterations (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  unsigned HOST_WIDE_INT niter;
  unsigned exit_mod;
  sbitmap wont_exit;
  unsigned n_remove_edges, i;
  edge *remove_edges;
  unsigned max_unroll = loop->lpt_decision.times;
  struct loop_desc *desc = &loop->desc;

  niter = desc->niter;

  if (niter <= (unsigned) max_unroll + 1)
    abort ();  /* Should not get here.  */

  exit_mod = niter % (max_unroll + 1);

  wont_exit = sbitmap_alloc (max_unroll + 1);
  sbitmap_ones (wont_exit);

  remove_edges = xcalloc (max_unroll + exit_mod + 1, sizeof (edge));
  n_remove_edges = 0;

  if (desc->postincr)
    {
      /* Counter is incremented after the exit test; leave exit test
	 in the first copy.  */

      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Condition on beginning of loop.\n");

      /* Peel exit_mod iterations.  */
      RESET_BIT (wont_exit, 0);
      if (desc->noloop_assumptions)
	RESET_BIT (wont_exit, 1);

      if (exit_mod
	  && !duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, exit_mod,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
	abort ();
      desc->niter -= exit_mod;

      SET_BIT (wont_exit, 1);
    }
  else
    {
      /* Leave exit test in last copy.  */

      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Condition on end of loop.\n");

      /* We know that niter >= max_unroll + 2; so we do not need to care of
	 case when we would exit before reaching the loop.  So just peel
	 exit_mod + 1 iterations.
	 */
      if (exit_mod != (unsigned) max_unroll || desc->noloop_assumptions)
	{
	  RESET_BIT (wont_exit, 0);
	  if (desc->noloop_assumptions)
	    RESET_BIT (wont_exit, 1);

	  if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, exit_mod + 1,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
	    abort ();
	  desc->niter -= exit_mod + 1;

	  SET_BIT (wont_exit, 0);
	  SET_BIT (wont_exit, 1);
	}

      RESET_BIT (wont_exit, max_unroll);
    }

  /* Now unroll the loop.  */
  if (!duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
		loops, max_unroll,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
    abort ();

  free (wont_exit);
  if (!desc->postincr)
    {
      basic_block exit_block = RBI (desc->in_edge->src)->copy;
      /* Find a new in and out edge; they are in the last copy we have made.  */
      
      if (exit_block->succ->dest == desc->out_edge->dest)
	{
	  desc->out_edge = exit_block->succ;
	  desc->in_edge = exit_block->succ->succ_next;
	}
      else
	{
	  desc->out_edge = exit_block->succ->succ_next;
	  desc->in_edge = exit_block->succ;
	}
    }

  /* Remove the edges.  */
  for (i = 0; i < n_remove_edges; i++)
    remove_path (loops, remove_edges[i]);
  free (remove_edges);

  desc->niter /= max_unroll + 1;
  desc->niter_expr = GEN_INT (desc->niter);
  desc->noloop_assumptions = NULL_RTX;

  if (loop->histogram)
    {
      /* We could try to update histogram, but there is no sense as the information
	 is not used any more.  */
      free_histogram (loop->histogram);
      loop->histogram = NULL;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unrolled loop %d times, constant # of iterations %i insns\n",max_unroll, num_loop_insns (loop));
}

/* Decide whether to unroll loop iterating runtime computable number of times
   and how much.  */
static void
decide_unroll_runtime_iterations (loops, loop, flags)
     struct loops *loops ATTRIBUTE_UNUSED;
     struct loop *loop;
     int flags;
{
  unsigned nunroll, nunroll_by_av, i;

  if (!(flags & UAP_UNROLL))
    {
      /* We were not asked to, just return back silently.  */
      return;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Considering unrolling loop with runtime computable number of iterations\n");

  /* nunroll = total number of copies of the original loop body in
     unrolled loop (i.e. if it is 2, we have to duplicate loop body once.  */
  nunroll = PARAM_VALUE (PARAM_MAX_UNROLLED_INSNS) / loop->ninsns;
  nunroll_by_av = PARAM_VALUE (PARAM_MAX_AVERAGE_UNROLLED_INSNS) / loop->av_ninsns;
  if (nunroll > nunroll_by_av)
    nunroll = nunroll_by_av;
  if (nunroll > (unsigned) PARAM_VALUE (PARAM_MAX_UNROLL_TIMES))
    nunroll = PARAM_VALUE (PARAM_MAX_UNROLL_TIMES);

  /* Skip big loops.  */
  if (nunroll <= 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is too big\n");
      return;
    }

  /* Check simpleness.  */
  if (!loop->simple
      || !loop->has_desc
      || loop->desc.assumptions)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Unable to prove that the number of iterations can be counted in runtime\n");
      return;
    }

  if (loop->desc.const_iter)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Loop iterates constant times\n");
      return;
    }

  /* If we have profile feedback, check whether the loop rolls.  */
  if (loop->header->count && expected_loop_iterations (loop) < 2 * nunroll)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unrolling loop, doesn't roll\n");
      return;
    }

  /* Success; now force nunroll to be power of 2.  */
  for (i = 1; 2 * i <= nunroll; i *= 2);

  loop->lpt_decision.decision = LPT_UNROLL_RUNTIME;
  loop->lpt_decision.times = i - 1;
}

/* Unroll LOOP for that we are able to count number of iterations in runtime.
   MAX_UNROLL is maximal number of allowed unrollings.  DESC describes the loop.  */
static void
unroll_loop_runtime_iterations (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  rtx old_niter, niter, init_code, branch_code, jump, label;
  unsigned i, j, p;
  basic_block preheader, *body, *dom_bbs, swtch, ezc_swtch;
  unsigned n_dom_bbs;
  sbitmap wont_exit;
  int may_exit_copy;
  unsigned n_peel, n_remove_edges;
  edge *remove_edges, e;
  bool extra_zero_check, last_may_exit;
  unsigned max_unroll = loop->lpt_decision.times;
  struct loop_desc *desc = &loop->desc;
  enum machine_mode mode;

  /* Remember blocks whose dominators will have to be updated.  */
  dom_bbs = xcalloc (n_basic_blocks, sizeof (basic_block));
  n_dom_bbs = 0;

  body = get_loop_body (loop);
  for (i = 0; i < loop->num_nodes; i++)
    {
      unsigned nldom;
      basic_block *ldom;

      nldom = get_dominated_by (loops->cfg.dom, body[i], &ldom);
      for (j = 0; j < nldom; j++)
	if (!flow_bb_inside_loop_p (loop, ldom[j]))
	  dom_bbs[n_dom_bbs++] = ldom[j];

      free (ldom);
    }
  free (body);

  if (desc->postincr)
    {
      /* Leave exit in first copy.  */
      may_exit_copy = 0;
      n_peel = max_unroll - 1;
      extra_zero_check = true;
      last_may_exit = false;
    }
  else
    {
      /* Leave exit in last copy.  */
      may_exit_copy = max_unroll;
      n_peel = max_unroll;
      extra_zero_check = false;
      last_may_exit = true;
    }

  /* Normalization.  */
  start_sequence ();
  niter = iv_omit_initial_values (desc->niter_expr);
  old_niter = niter = force_operand (niter, NULL);
  mode = desc->mode;

  /* Count modulo by ANDing it with max_unroll.  */
  niter = expand_simple_binop (mode, AND,
			       niter,
			       GEN_INT (max_unroll),
			       NULL_RTX, 0, OPTAB_LIB_WIDEN);

  init_code = get_insns ();
  end_sequence ();

  /* Precondition the loop.  */
  loop_split_edge_with (loop_preheader_edge (loop), init_code, loops);

  remove_edges = xcalloc (max_unroll + n_peel + 1, sizeof (edge));
  n_remove_edges = 0;

  wont_exit = sbitmap_alloc (max_unroll + 2);

  /* Peel the first copy of loop body (almost always we must leave exit test
     here; the only exception is when we have extra_zero_check and the number
     of iterations is reliable).  Also record the place of (possible) extra
     zero check.  */
  sbitmap_zero (wont_exit);
  if (extra_zero_check && !desc->noloop_assumptions)
    SET_BIT (wont_exit, 1);
  ezc_swtch = loop_preheader_edge (loop)->src;
  if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, 1,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
    abort ();

  /* Record the place where switch will be built for preconditioning.  */
  swtch = loop_split_edge_with (loop_preheader_edge (loop),
				NULL_RTX, loops);

  for (i = 0; i < n_peel; i++)
    {
      /* Peel the copy.  */
      sbitmap_zero (wont_exit);
      if (i != n_peel - 1 || !last_may_exit)
	SET_BIT (wont_exit, 1);
      if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, 1,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
    	abort ();

      /* Create item for switch.  */
      j = n_peel - i - (extra_zero_check ? 0 : 1);
      p = REG_BR_PROB_BASE / (i + 2);

      preheader = loop_split_edge_with (loop_preheader_edge (loop),
					NULL_RTX, loops);
      label = block_label (preheader);
      start_sequence ();
      do_compare_rtx_and_jump (copy_rtx (niter), GEN_INT (j), EQ, 0,
			       mode, NULL_RTX, NULL_RTX, label);
      jump = get_last_insn ();
      JUMP_LABEL (jump) = label;
      REG_NOTES (jump)
	= gen_rtx_EXPR_LIST (REG_BR_PROB,
			     GEN_INT (p), REG_NOTES (jump));
	
      LABEL_NUSES (label)++;
      branch_code = get_insns ();
      end_sequence ();

      swtch = loop_split_edge_with (swtch->pred, branch_code, loops);
      set_immediate_dominator (loops->cfg.dom, preheader, swtch);
      swtch->succ->probability = REG_BR_PROB_BASE - p;
      e = make_edge (swtch, preheader,
		     swtch->succ->flags & EDGE_IRREDUCIBLE_LOOP);
      e->probability = p;
    }

  if (extra_zero_check)
    {
      /* Add branch for zero iterations.  */
      p = REG_BR_PROB_BASE / (max_unroll + 1);
      swtch = ezc_swtch;
      preheader = loop_split_edge_with (loop_preheader_edge (loop),
					NULL_RTX, loops);
      label = block_label (preheader);
      start_sequence ();
      do_compare_rtx_and_jump (copy_rtx (niter), const0_rtx, EQ, 0,
			       mode, NULL_RTX, NULL_RTX, label);
      jump = get_last_insn ();
      JUMP_LABEL (jump) = label;
      REG_NOTES (jump)
	      = gen_rtx_EXPR_LIST (REG_BR_PROB,
				   GEN_INT (p), REG_NOTES (jump));
      
      LABEL_NUSES (label)++;
      branch_code = get_insns ();
      end_sequence ();

      swtch = loop_split_edge_with (swtch->succ, branch_code, loops);
      set_immediate_dominator (loops->cfg.dom, preheader, swtch);
      swtch->succ->probability = REG_BR_PROB_BASE - p;
      e = make_edge (swtch, preheader,
		     swtch->succ->flags & EDGE_IRREDUCIBLE_LOOP);
      e->probability = p;
    }

  /* Recount dominators for outer blocks.  */
  iterate_fix_dominators (loops->cfg.dom, dom_bbs, n_dom_bbs);

  /* And unroll loop.  */

  sbitmap_ones (wont_exit);
  RESET_BIT (wont_exit, may_exit_copy);

  if (!duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
	 	loops, max_unroll,
		wont_exit, desc->out_edge, remove_edges, &n_remove_edges,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
    abort ();

  free (wont_exit);

  if (!desc->postincr)
    {
      basic_block exit_block = RBI (desc->in_edge->src)->copy;
      /* Find a new in and out edge; they are in the last copy we have made.  */
      
      if (exit_block->succ->dest == desc->out_edge->dest)
	{
	  desc->out_edge = exit_block->succ;
	  desc->in_edge = exit_block->succ->succ_next;
	}
      else
	{
	  desc->out_edge = exit_block->succ->succ_next;
	  desc->in_edge = exit_block->succ;
	}
    }

  /* Remove the edges.  */
  for (i = 0; i < n_remove_edges; i++)
    remove_path (loops, remove_edges[i]);
  free (remove_edges);

  /* We must be careful when updating the number of iterations due to
     preconditioning and the fact that the value must be valid at entry
     of the loop.  After passing through the above code, we see that
     the correct new number of iterations is this:  */
  if (desc->const_iter)
    abort ();
  desc->niter_expr =
    simplify_gen_binary (UDIV, mode, old_niter, GEN_INT (max_unroll + 1));
  if (!desc->postincr)
    {
      desc->niter_expr =
	simplify_gen_binary (MINUS, mode, desc->niter_expr, const1_rtx);
      desc->noloop_assumptions = NULL_RTX;
    }

  if (loop->histogram)
    {
      /* We could try to update histogram, but there is no sense as the information
	 is not used any more.  */
      free_histogram (loop->histogram);
      loop->histogram = NULL;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file,
	     ";; Unrolled loop %d times, counting # of iterations in runtime, %i insns\n",
	     max_unroll, num_loop_insns (loop));
}
  
/* Decide whether to simply peel loop and how much.  */
static void
decide_peel_simple (loops, loop, flags)
     struct loops *loops ATTRIBUTE_UNUSED;
     struct loop *loop;
     int flags;
{
  unsigned npeel, i;

  if (!(flags & UAP_PEEL))
    {
      /* We were not asked to, just return back silently.  */
      return;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Considering simply peeling loop\n");

  /* npeel = number of iterations to peel.  */
  npeel = PARAM_VALUE (PARAM_MAX_PEELED_INSNS) / loop->ninsns;
  if (npeel > (unsigned) PARAM_VALUE (PARAM_MAX_PEEL_TIMES))
    npeel = PARAM_VALUE (PARAM_MAX_PEEL_TIMES);

  /* Skip big loops.  */
  if (!npeel)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is too big\n");
      return;
    }

  /* Check number of iterations.  */
  if (loop->has_desc
      && loop->simple
      && !loop->desc.assumptions
      && loop->desc.const_iter)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Loop iterates constant times\n");
      return;
    }

  /* Do not simply peel loops with branches inside -- it increases number
     of mispredicts.  */
  if (loop->desc.n_branches > 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not peeling, contains branches\n");
      return;
    }

  /* If we have profile feedback, check whether the loop rolls.  */
  if (loop->histogram)
    {
      gcov_type all_counters, act_counters;
      all_counters = loop->histogram->more;
      for (i = 0; i < loop->histogram->steps; i++)
	all_counters += loop->histogram->counts[i];
      act_counters = 0;
      if (!all_counters)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, ";; Not peeling loop, never exits\n");
	  return;
	}
      for (i = 0; i < loop->histogram->steps; i++)
	{
	  if (act_counters * REG_BR_PROB_BASE >= all_counters * PARAM_VALUE (PARAM_HISTOGRAM_PEEL_RATIO))
	    break;
	  act_counters += loop->histogram->counts[i];
	}
      if (act_counters * REG_BR_PROB_BASE < all_counters * PARAM_VALUE (PARAM_HISTOGRAM_PEEL_RATIO))
	i = npeel + 1;
      if (i > npeel)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     ";; Not peeling loop, rolls too much (%d iterations > %d [maximum peelings])\n",
		     i, (int) npeel);
	  return;
	}
      npeel = i;
    }
  else if (loop->header->count)
    {
      unsigned niter = expected_loop_iterations (loop);
      if (niter + 1 > npeel)
	{
	  if (rtl_dump_file)
	    {
	      fprintf (rtl_dump_file, ";; Not peeling loop, rolls too much (");
	      fprintf (rtl_dump_file, HOST_WIDEST_INT_PRINT_DEC, (HOST_WIDEST_INT) (niter + 1));
	      fprintf (rtl_dump_file, " iterations > %d [maximum peelings])\n", npeel);
	    }
	  return;
	}
      npeel = niter + 1;
    }
  else
    {
      /* For now we have no good heuristics to decide whether loop peeling
         will be effective, so disable it.  */
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 ";; Not peeling loop, no evidence it will be profitable\n");
      return;
    }

  /* Success.  */
  loop->lpt_decision.decision = LPT_PEEL_SIMPLE;
  loop->lpt_decision.times = npeel;
}

/* Peel a LOOP.  */
static void
peel_loop_simple (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  sbitmap wont_exit;
  unsigned npeel = loop->lpt_decision.times;

  wont_exit = sbitmap_alloc (npeel + 1);
  sbitmap_zero (wont_exit);

  if (!duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
		loops, npeel, wont_exit, NULL, NULL, NULL,
		DLTHE_FLAG_UPDATE_FREQ | (loop->histogram ? DLTHE_USE_HISTOGRAM_PROB : DLTHE_USE_WONT_EXIT)))
    abort ();
  
  free (wont_exit);

  if (loop->has_desc && loop->simple)
    {
      if (loop->desc.const_iter)
	loop->desc.niter -= npeel;
      loop->desc.niter_expr =
	simplify_gen_binary (MINUS, GET_MODE (loop->desc.niter_expr),
			     loop->desc.niter_expr, GEN_INT (npeel));
      loop->desc.noloop_assumptions = NULL_RTX;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Peeling loop %d times\n", npeel);
}

/* Decide whether to unroll loop stupidly and how much.  */
static void
decide_unroll_stupid (loops, loop, flags)
     struct loops *loops ATTRIBUTE_UNUSED;
     struct loop *loop;
     int flags;
{
  unsigned nunroll, nunroll_by_av, i;

  if (!(flags & UAP_UNROLL_ALL))
    {
      /* We were not asked to, just return back silently.  */
      return;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Considering unrolling loop stupidly\n");

  /* nunroll = total number of copies of the original loop body in
     unrolled loop (i.e. if it is 2, we have to duplicate loop body once.  */
  nunroll = PARAM_VALUE (PARAM_MAX_UNROLLED_INSNS) / loop->ninsns;
  nunroll_by_av = PARAM_VALUE (PARAM_MAX_AVERAGE_UNROLLED_INSNS) / loop->av_ninsns;
  if (nunroll > nunroll_by_av)
    nunroll = nunroll_by_av;
  if (nunroll > (unsigned) PARAM_VALUE (PARAM_MAX_UNROLL_TIMES))
    nunroll = PARAM_VALUE (PARAM_MAX_UNROLL_TIMES);

  /* Skip big loops.  */
  if (nunroll <= 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not considering loop, is too big\n");
      return;
    }

  /* Check simpleness.  */
  if (loop->has_desc
      && loop->simple
      && !loop->desc.assumptions)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; The loop is simple\n");
      return;
    }

  /* Do not unroll loops with branches inside -- it increases number
     of mispredicts.  */
  if (loop->desc.n_branches > 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unrolling, contains branches\n");
      return;
    }

  /* If we have profile feedback, check whether the loop rolls.  */
  if (loop->header->count && expected_loop_iterations (loop) < 2 * nunroll)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unrolling loop, doesn't roll\n");
      return;
    }

  /* Success; now force nunroll to be power of 2.  */
  for (i = 1; 2 * i <= nunroll; i *= 2);

  loop->lpt_decision.decision = LPT_UNROLL_STUPID;
  loop->lpt_decision.times = i - 1;
}

/* Unroll a LOOP.  */
static void
unroll_loop_stupid (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  sbitmap wont_exit;
  unsigned nunroll = loop->lpt_decision.times;

  wont_exit = sbitmap_alloc (nunroll + 1);
  sbitmap_zero (wont_exit);

  if (!duplicate_loop_to_header_edge (loop, loop_latch_edge (loop),
		loops, nunroll, wont_exit, NULL, NULL, NULL,
		DLTHE_FLAG_UPDATE_FREQ | DLTHE_USE_WONT_EXIT))
    abort ();

  free (wont_exit);
 
  if (loop->has_desc && loop->simple)
    {
      /* We indeed may get here provided that there are nontrivial assumptions
	 for a loop to be really simple.  We could update the counts, but the
	 problem is that we are unable to decide which exit will be taken
	 (not really true in case the number of iterations is constant,
	 but noone will do anything with this information, so we do not
	 worry about it).  */
      loop->simple = false;
    }

  if (loop->histogram)
    {
      /* We could try to update histogram, but there is no sense as the information
	 is not used any more.  */
      free_histogram (loop->histogram);
      loop->histogram = NULL;
    }

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unrolled loop %d times, %i insns\n",
	     nunroll, num_loop_insns (loop));
}
