/* Loop unswitching for GNU compiler.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.

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

/* This pass moves constant conditions out of loops, duplicating the loop
   in progress, i.e. this code:

   while (loop_cond)
     {
       A;
       if (cond)
         branch1;
       else
	 branch2;
       B;
       if (cond)
         branch3;
       C;
     }
   where nothing inside the loop alters cond is transformed
   into

   if (cond)
     {
       while (loop_cond)
	 {
	   A;
	   branch1;
	   B;
	   branch3;
	   C;
	 }
     }
   else
     {
       while (loop_cond)
	 {
	   A;
	   branch2;
	   B;
	   C;
	 }
     }

  Duplicating the loop might lead to code growth exponential in number of
  branches inside loop, so we limit the number of unswitchings performed
  in a single loop to PARAM_MAX_UNSWITCH_LEVEL.  We only perform the
  transformation on innermost loops, as the benefit of doing it on loops
  containing subloops would not be very large compared to complications
  with handling this case.  */

static struct loop *unswitch_loop (struct loops *, struct loop *,
				   basic_block, rtx);
static void unswitch_single_loop (struct loops *, struct loop *, rtx, int);
static rtx may_unswitch_on (basic_block, struct loop *);

/* Prepare a sequence comparing OP0 with OP1 using COMP and jumping to LABEL if
   true, with probability PROB.  */

rtx
compare_and_jump_seq (rtx op0, rtx op1, enum rtx_code comp, rtx label, int prob)
{
  rtx seq, jump;
  enum machine_mode mode;

  start_sequence ();
  op0 = force_operand (op0, NULL_RTX);
  op1 = force_operand (op1, NULL_RTX);
  mode = GET_MODE (op0);
  if (mode == VOIDmode)
    mode = GET_MODE (op1);
  do_compare_rtx_and_jump (op0, op1, comp, 0,
			   mode, NULL_RTX, NULL_RTX, label);
  jump = get_last_insn ();
  REG_NOTES (jump) = gen_rtx_EXPR_LIST (REG_BR_PROB, GEN_INT (prob),
					REG_NOTES (jump));
  JUMP_LABEL (jump) = label;
  LABEL_NUSES (label)++;
  seq = get_insns ();
  end_sequence ();

  return seq;
}

/* Main entry point.  Perform loop unswitching on all suitable LOOPS.  */
void
unswitch_loops (struct loops *loops)
{
  int i, num;
  struct loop *loop;

  /* Go through inner loops (only original ones).  */
  num = loops->num;

  for (i = 1; i < num; i++)
    {
      /* Removed loop?  */
      loop = loops->parray[i];
      if (!loop)
	continue;

      if (loop->inner)
	continue;

      unswitch_single_loop (loops, loop, NULL_RTX, 0);
#ifdef ENABLE_CHECKING
      verify_dominators (CDI_DOMINATORS);
      verify_loop_structure (loops);
#endif
    }

  iv_analysis_done ();
}

/* Checks whether we can unswitch LOOP on condition at end of BB -- one of its
   basic blocks (for what it means see comments below).  */

static rtx
may_unswitch_on (basic_block bb, struct loop *loop)
{
  rtx test, at, insn, op0, op1;
  struct rtx_iv iv;

  /* BB must end in a simple conditional jump.  */
  if (!bb->succ || !bb->succ->succ_next || bb->succ->succ_next->succ_next)
    return NULL_RTX;
  if (!any_condjump_p (BB_END (bb)))
    return NULL_RTX;

  /* With branches inside loop.  */
  if (!flow_bb_inside_loop_p (loop, bb->succ->dest)
      || !flow_bb_inside_loop_p (loop, bb->succ->succ_next->dest))
    return NULL_RTX;

  /* It must be executed just once each iteration (because otherwise we
     are unable to update dominator/irreducible loop information correctly).  */
  if (!just_once_each_iteration_p (loop, bb))
    return NULL_RTX;

  /* Condition must be invariant.  */
  test = get_condition (BB_END (bb), &at, true);
  if (!test)
    return NULL_RTX;

  op0 = XEXP (test, 0);
  if (!CONSTANT_P (op0))
    {
      if (!REG_P (op0))
	return NULL_RTX;

      insn = iv_get_reaching_def (at, op0);
      if (!iv_analyse (insn, op0, &iv))
	return NULL_RTX;
      if (!iv.base
	  || iv.step != const0_rtx)
	return NULL_RTX;

      op0 = iv.base;
    }

  op1 = XEXP (test, 1);
  if (!CONSTANT_P (op1))
    {
      if (!REG_P (op1))
	return NULL_RTX;

      insn = iv_get_reaching_def (at, op1);
      if (!iv_analyse (insn, op1, &iv))
	return NULL_RTX;
      if (!iv.base
	  || iv.step != const0_rtx)
	return NULL_RTX;

      op1 = iv.base;
    }

  return canon_condition (gen_rtx_fmt_ee (GET_CODE (test), SImode, op0, op1));
}

/* Reverses CONDition; returns NULL if we cannot.  */
rtx
reversed_condition (rtx cond)
{
  enum rtx_code reversed;
  reversed = reversed_comparison_code (cond, NULL);
  if (reversed == UNKNOWN)
    return NULL_RTX;
  else
    return gen_rtx_fmt_ee (reversed,
			   GET_MODE (cond), XEXP (cond, 0),
			   XEXP (cond, 1));
}

/* Unswitch single LOOP.  COND_CHECKED holds list of conditions we already
   unswitched on and are therefore known to be true in this LOOP.  NUM is
   number of unswitchings done; do not allow it to grow too much, it is too
   easy to create example on that the code would grow exponentially.  */
static void
unswitch_single_loop (struct loops *loops, struct loop *loop,
		      rtx cond_checked, int num)
{
  basic_block *bbs;
  struct loop *nloop;
  unsigned i;
  rtx cond, rcond, conds, rconds, acond;
  int repeat;
  edge e;

  /* Do not unswitch too much.  */
  if (num > PARAM_VALUE (PARAM_MAX_UNSWITCH_LEVEL))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching anymore, hit max level\n");
      return;
    }

  /* Only unswitch innermost loops.  */
  if (loop->inner)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, not innermost loop\n");
      return;
    }

  /* We must be able to duplicate loop body.  */
  if (!can_duplicate_loop_p (loop))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, can't duplicate loop\n");
      return;
    }

  /* The loop should not be too large, to limit code growth.  */
  if (num_loop_insns (loop) > PARAM_VALUE (PARAM_MAX_UNSWITCH_INSNS))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, loop too big\n");
      return;
    }

  /* Do not unswitch in cold areas.  */
  if (!maybe_hot_bb_p (loop->header))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, not hot area\n");
      return;
    }

  /* Nor if the loop usually does not roll.  */
  if (expected_loop_iterations (loop) < 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, loop iterations < 1\n");
      return;
    }

  do
    {
      repeat = 0;

      /* Find a bb to unswitch on.  */
      bbs = get_loop_body (loop);
      iv_analysis_loop_init (loop);
      for (i = 0; i < loop->num_nodes; i++)
	if ((cond = may_unswitch_on (bbs[i], loop)))
	  break;

      if (i == loop->num_nodes)
	{
	  free (bbs);
	  return;
	}

      rcond = reversed_condition (cond);
      if (rcond)
	rcond = canon_condition (rcond);

      /* Check whether the result can be predicted.  */
      for (acond = cond_checked; acond; acond = XEXP (acond, 1))
	simplify_using_condition (XEXP (acond, 0), &cond, NULL);

      if (cond == const_true_rtx)
	{
	  /* Remove false path.  */
	  e = FALLTHRU_EDGE (bbs[i]);
	  remove_path (loops, e);
	  free (bbs);
	  repeat = 1;
	}
      else if (cond == const0_rtx)
	{
	  /* Remove true path.  */
	  e = BRANCH_EDGE (bbs[i]);
	  remove_path (loops, e);
	  free (bbs);
	  repeat = 1;
	}
    } while (repeat);

  /* We found the condition we can unswitch on.  */
  conds = alloc_EXPR_LIST (0, cond, cond_checked);
  if (rcond)
    rconds = alloc_EXPR_LIST (0, rcond, cond_checked);
  else
    rconds = cond_checked;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unswitching loop\n");

  /* Unswitch the loop on this condition.  */
  nloop = unswitch_loop (loops, loop, bbs[i], cond);
  if (!nloop)
  abort ();

  /* Invoke itself on modified loops.  */
  unswitch_single_loop (loops, nloop, conds, num + 1);
  unswitch_single_loop (loops, loop, rconds, num + 1);

  free_EXPR_LIST_node (conds);
  if (rcond)
    free_EXPR_LIST_node (rconds);
}

/* Unswitch a LOOP w.r. to given basic block UNSWITCH_ON.  We only support
   unswitching of innermost loops.  UNSWITCH_ON must be executed in every
   iteration, i.e. it must dominate LOOP latch.  COND is the condition
   determining which loop is entered.  Returns NULL if impossible, new loop
   otherwise.  The new loop is entered if COND is true.  */

static struct loop *
unswitch_loop (struct loops *loops, struct loop *loop, basic_block unswitch_on,
	       rtx cond)
{
  edge entry, latch_edge, true_edge, false_edge, e;
  basic_block switch_bb, unswitch_on_alt, src;
  struct loop *nloop;
  sbitmap zero_bitmap;
  int irred_flag, prob;
  rtx seq;

  /* Some sanity checking.  */
  if (!flow_bb_inside_loop_p (loop, unswitch_on))
    abort ();
  if (!unswitch_on->succ || !unswitch_on->succ->succ_next ||
      unswitch_on->succ->succ_next->succ_next)
    abort ();
  if (!just_once_each_iteration_p (loop, unswitch_on))
    abort ();
  if (loop->inner)
    abort ();
  if (!flow_bb_inside_loop_p (loop, unswitch_on->succ->dest))
    abort ();
  if (!flow_bb_inside_loop_p (loop, unswitch_on->succ->succ_next->dest))
    abort ();

  entry = loop_preheader_edge (loop);

  /* Make a copy.  */
  src = entry->src;
  irred_flag = entry->flags & EDGE_IRREDUCIBLE_LOOP;
  entry->flags &= ~EDGE_IRREDUCIBLE_LOOP;
  zero_bitmap = sbitmap_alloc (2);
  sbitmap_zero (zero_bitmap);
  if (!duplicate_loop_to_header_edge (loop, entry, loops, 1,
	zero_bitmap, NULL, NULL, NULL, 0))
    return NULL;
  free (zero_bitmap);
  entry->flags |= irred_flag;

  /* Record the block with condition we unswitch on.  */
  unswitch_on_alt = unswitch_on->rbi->copy;
  true_edge = BRANCH_EDGE (unswitch_on);
  false_edge = FALLTHRU_EDGE (unswitch_on_alt);
  latch_edge = loop->latch->rbi->copy->succ;

  /* Create a block with the condition.  */
  prob = true_edge->probability;
  switch_bb = create_empty_bb (EXIT_BLOCK_PTR->prev_bb);
  seq = compare_and_jump_seq (XEXP (cond, 0), XEXP (cond, 1), GET_CODE (cond),
			      block_label (true_edge->dest),
			      prob);
  emit_insn_after (seq, BB_END (switch_bb));
  e = make_edge (switch_bb, true_edge->dest, 0);
  e->probability = prob;
  e->count = latch_edge->count * prob / REG_BR_PROB_BASE;
  e = make_edge (switch_bb, FALLTHRU_EDGE (unswitch_on)->dest, EDGE_FALLTHRU);
  e->probability = false_edge->probability;
  e->count = latch_edge->count * (false_edge->probability) / REG_BR_PROB_BASE;

  if (irred_flag)
    {
      switch_bb->flags |= BB_IRREDUCIBLE_LOOP;
      switch_bb->succ->flags |= EDGE_IRREDUCIBLE_LOOP;
      switch_bb->succ->succ_next->flags |= EDGE_IRREDUCIBLE_LOOP;
    }
  else
    {
      switch_bb->flags &= ~BB_IRREDUCIBLE_LOOP;
      switch_bb->succ->flags &= ~EDGE_IRREDUCIBLE_LOOP;
      switch_bb->succ->succ_next->flags &= ~EDGE_IRREDUCIBLE_LOOP;
    }

  /* Loopify from the copy of LOOP body, constructing the new loop.  */
  nloop = loopify (loops, latch_edge,
		   loop->header->rbi->copy->pred, switch_bb, true);

  /* Remove branches that are now unreachable in new loops.  We rely on the
     fact that cfg_layout_duplicate_bb reverses list of edges.  */
  remove_path (loops, true_edge);
  remove_path (loops, false_edge);

  /* One of created loops do not have to be subloop of the outer loop now,
     so fix its placement in loop data structure.  */
  fix_loop_placement (loop);
  fix_loop_placement (nloop);

  /* Preserve the simple loop preheaders.  */
  loop_split_edge_with (loop_preheader_edge (loop), NULL_RTX);
  loop_split_edge_with (loop_preheader_edge (nloop), NULL_RTX);

  return nloop;
}
