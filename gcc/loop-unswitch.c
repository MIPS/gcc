/* Loop unswitching for GNU compiler.
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

static struct loop *unswitch_loop	PARAMS ((struct loops *,
						struct loop *, basic_block));
static void unswitch_single_loop	PARAMS ((struct loops *, struct loop *,
						rtx, int));
static bool may_unswitch_on_p		PARAMS ((struct loops *, basic_block,
						struct loop *, basic_block *));
static rtx reversed_condition		PARAMS ((rtx));

/* Unswitch LOOPS.  */
void
unswitch_loops (loops)
     struct loops *loops;
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
      /* Not innermost?  */
      if (loop->inner)
	continue;

      unswitch_single_loop (loops, loop, NULL_RTX, 0);
#ifdef ENABLE_CHECKING
      verify_dominators (loops->cfg.dom);
      verify_loop_structure (loops);
#endif
    }
}

/* Checks whether we can unswitch LOOP on basic block BB.  LOOP BODY
   is provided to save time.  */
static bool
may_unswitch_on_p (loops, bb, loop, body)
     struct loops *loops;
     basic_block bb;
     struct loop *loop;
     basic_block *body;
{
  rtx test;
  unsigned i;

  /* It must be a simple conditional jump.  */
  if (!bb->succ || !bb->succ->succ_next || bb->succ->succ_next->succ_next)
    return false;
  if (!any_condjump_p (bb->end))
    return false;

  /* Are both branches inside loop?  */
  if (!flow_bb_inside_loop_p (loop, bb->succ->dest)
      || !flow_bb_inside_loop_p (loop, bb->succ->succ_next->dest))
    return false;

  /* It must be executed just once each iteration (because otherwise we
     are unable to update dominator/irreducible loop information correctly).  */
  if (!just_once_each_iteration_p (loops, loop, bb))
    return false;

  /* Condition must be invariant.  */
  test = get_condition (bb->end, NULL);
  if (!test)
    return false;

  for (i = 0; i < loop->num_nodes; i++)
    if (modified_between_p (test, body[i]->head, NEXT_INSN (body[i]->end)))
      return false;

  return true;
}

/* Reverses CONDition; returns NULL if we cannot.  */
static rtx
reversed_condition (cond)
     rtx cond;
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
   unswitched on and are true in our branch.  NUM is number of unswitchings
   done; do not allow it to grow too much, it is too easy to create example
   on that the code would grow exponentially.  */
static void
unswitch_single_loop (loops, loop, cond_checked, num)
     struct loops *loops;
     struct loop *loop;
     rtx cond_checked;
     int num;
{
  basic_block *bbs, bb;
  struct loop *nloop;
  unsigned i;
  int true_first;
  rtx cond, rcond, conds, rconds, acond, split_before;
  int always_true;
  int always_false;
  int repeat;
  edge e;

  /* Do not unswitch too much.  */
  if (num > PARAM_VALUE (PARAM_MAX_UNSWITCH_LEVEL))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching anymore, hit max level\n");
      return;
    }

  /* We only unswitch innermost loops (at least for now).  */
  if (loop->inner)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, not innermost loop\n");
      return;
    }
  
  /* And we must be able to duplicate loop body.  */
  if (!can_duplicate_loop_p (loop))
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, can't duplicate loop\n");
      return;
    }

  /* Check the size of loop.  */
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
  
  /* Nor if it usually do not pass.  */
  if (expected_loop_iterations (loop) < 1)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, ";; Not unswitching, loop iterations < 1\n");
      return;
    }

  do
    {
      repeat = 0;
    
      /* Find a bb to unswitch on.  We use just a stupid test of invariantness
	 of the condition: all used regs must not be modified inside loop body.  */
      bbs = get_loop_body (loop);
      for (i = 0; i < loop->num_nodes; i++)
	if (may_unswitch_on_p (loops, bbs[i], loop, bbs))
	  break;

      if (i == loop->num_nodes)
	{
	  free (bbs);
	  return;
	}

      if (!(cond = get_condition (bbs[i]->end, &split_before)))
	abort ();
      rcond = reversed_condition (cond);
      
      /* Check whether the result can be predicted.  */
      always_true = 0;
      always_false = 0;
      for (acond = cond_checked; acond; acond = XEXP (acond, 1))
	{
	  if (rtx_equal_p (cond, XEXP (acond, 0)))
	    {
	      /* True.  */
	      always_true = 1;
	      break;
	    }
	  if (rtx_equal_p (rcond, XEXP (acond, 0)))
	    {
	      /* False.  */
	      always_false = 1;
	      break;
	    }
	}

      if (always_true)
	{
	  /* Remove false path.  */
 	  for (e = bbs[i]->succ; !(e->flags & EDGE_FALLTHRU); e = e->succ_next);
	  remove_path (loops, e);
	  free (bbs);
	  repeat = 1;
	}
      else if (always_false)
	{
	  /* Remove true path.  */
	  for (e = bbs[i]->succ; e->flags & EDGE_FALLTHRU; e = e->succ_next);
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

  /* Separate condition.  */
  bb = split_loop_bb (loops, bbs[i], PREV_INSN (split_before))->dest;
  free (bbs);
  true_first = !(bb->succ->flags & EDGE_FALLTHRU);
  if (rtl_dump_file)
    fprintf (rtl_dump_file, ";; Unswitching loop\n");
  /* Unswitch the loop.  */
  nloop = unswitch_loop (loops, loop, bb);
  if (!nloop)
  abort ();

  /* Invoke itself on modified loops.  */
  unswitch_single_loop (loops, nloop, true_first ? conds : rconds, num + 1);
  unswitch_single_loop (loops, loop, true_first ? rconds : conds, num + 1);

  free_EXPR_LIST_node (conds);
  if (rcond)
    free_EXPR_LIST_node (rconds);
}

/* Unswitch a LOOP w.r. to given basic block.  We only support unswitching
   of innermost loops (this is not a principal restriction, I'm just lazy
   to handle subloops).  UNSWITCH_ON must be executed in every iteration,
   i.e. it must dominate LOOP latch.  Returns NULL if impossible, new
   loop otherwise.  */
static struct loop *
unswitch_loop (loops, loop, unswitch_on)
     struct loops *loops;
     struct loop *loop;
     basic_block unswitch_on;
{
  edge entry, latch_edge;
  basic_block switch_bb, unswitch_on_alt, src;
  struct loop *nloop;
  sbitmap zero_bitmap;
  int irred_flag;
  int prob;

  /* Some sanity checking.  */
  if (!flow_bb_inside_loop_p (loop, unswitch_on))
    abort ();
  if (!unswitch_on->succ || !unswitch_on->succ->succ_next ||
      unswitch_on->succ->succ_next->succ_next)
    abort ();
  if (!just_once_each_iteration_p (loops, loop, unswitch_on))
    abort ();
  if (loop->inner)
    abort ();
  if (!flow_bb_inside_loop_p (loop, unswitch_on->succ->dest))
    abort ();
  if (!flow_bb_inside_loop_p (loop, unswitch_on->succ->succ_next->dest))
    abort ();
  
  /* Will we be able to perform redirection?  */
  if (!any_condjump_p (unswitch_on->end))
    return NULL;
  if (!cfg_layout_can_duplicate_bb_p (unswitch_on))
    return NULL;

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

  /* Record switch block.  */
  unswitch_on_alt = RBI (unswitch_on)->copy;

  /* Make a copy of unswitched block.  */
  switch_bb = cfg_layout_duplicate_bb (unswitch_on, NULL);
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
  add_to_dominance_info (loops->cfg.dom, switch_bb);
  RBI (unswitch_on)->copy = unswitch_on_alt;

  /* Loopify the copy.  */
  prob = switch_bb->succ->probability;
  for (latch_edge = RBI (loop->latch)->copy->succ;
       latch_edge->dest != loop->header;
       latch_edge = latch_edge->succ_next);
  nloop = loopify (loops, latch_edge,
		   RBI (loop->header)->copy->pred, switch_bb);

  /* Fix histograms.  */
  if (loop->histogram)
    {
      nloop->histogram = copy_histogram (loop->histogram, prob);
      add_histogram (loop->histogram, nloop->histogram, -REG_BR_PROB_BASE);
    }

  /* Remove paths from loop copies.  We rely on the fact that
     cfg_layout_duplicate_bb reverses list of edges here.  */
  remove_path (loops, unswitch_on->succ);
  remove_path (loops, unswitch_on_alt->succ);

  /* One of created loops do not have to be subloop of the outer loop now.  */
  fix_loop_placement (loop);
  fix_loop_placement (nloop);

  return nloop;
}
