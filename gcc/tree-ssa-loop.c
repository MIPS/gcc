/* Loop optimizations over tree-ssa.
   Copyright (C) 2003 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
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
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "timevar.h"
#include "cfgloop.h"
#include "tree-inline.h"
#include "flags.h"

/* The main entry into loop optimization pass.  PHASE indicates which dump file
   from the DUMP_FILES array to use when dumping debugging information.
   FNDECL is the current function decl.  */

static void
tree_ssa_loop_opt (void)
{
  struct loops *loops;

  loops = loop_optimizer_init (tree_dump_file);

  if (loops)
    {
      /* Ensure there is a place to move the computations to.  */
      create_preheaders (loops, CP_SIMPLE_PREHEADERS);

#if 0
      /* Test unrolling and peeling.  */
      test_unrolling_and_peeling (loops);
#endif

#if 0
      test_loop_versioning (loops);
#endif

      /* Move the expensive loop invariants.  */
      tree_ssa_lim (loops);

      /* Optimize the induction variables.  */
      tree_ssa_iv_optimize (loops);

      loop_optimizer_finalize (loops,
			       (tree_dump_flags & TDF_DETAILS
				? tree_dump_file : NULL));

      cleanup_tree_cfg ();
    }
}


/* Checks whether the STMT is a call, and if so, returns the call_expr.  */
static tree
call_expr_p (tree stmt)
{
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    stmt = TREE_OPERAND (stmt, 1);

  return TREE_CODE (stmt) == CALL_EXPR ? stmt : NULL_TREE;
}

/* Check whether we should duplicate header of LOOP.  At most *LIMIT
   instructions should be duplicated, limit is decreased by the actual
   amount.  */

static bool
should_duplicate_loop_header_p (struct loop *loop, int *limit)
{
  block_stmt_iterator bsi;
  basic_block header = loop->header;
  tree last;

  if (!header->succ)
    abort ();
  if (!header->succ->succ_next)
    return false;
  if (header->succ->succ_next->succ_next)
    return false;
  if (flow_bb_inside_loop_p (loop, header->succ->dest)
      && flow_bb_inside_loop_p (loop, header->succ->succ_next->dest))
    return false;

  last = last_stmt (header);
  if (TREE_CODE (last) != COND_EXPR)
    return false;

  /* Aproximately copy the conditions that used to be used in jump.c --
     at most 20 insns and no calls.  */
  for (bsi = bsi_start (header); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      last = bsi_stmt (bsi);

      if (TREE_CODE (last) == LABEL_EXPR)
	continue;

      if (call_expr_p (last))
	return false;

      *limit -= estimate_num_insns (last);
      if (*limit < 0)
	return false;
    }

  return true;
}

/* For all loops, copy the condition at the end of the loop body in front
   of the loop.  */

static void
copy_loop_headers (void)
{
  struct loops *loops;
  unsigned i;
  struct loop *loop;
  basic_block header_copy, preheader, new_header;
  edge preheader_edge, succ_in_loop;

  loops = loop_optimizer_init (tree_dump_file);
  if (!loops)
    return;
  
  /* We are not going to need or update dominators.  */
  free_dominance_info (CDI_DOMINATORS);

  create_preheaders (loops, CP_SIMPLE_PREHEADERS);

  /* We do not try to keep the information about irreductible regions
     up-to-date.  */
  loops->state &= ~LOOPS_HAVE_MARKED_IRREDUCIBLE_REGIONS;

#ifdef ENABLE_CHECKING
  verify_loop_structure (loops);
#endif

  for (i = 1; i < loops->num; i++)
    {
      /* Copy at most 20 insns.  */
      int limit = 20;

      loop = loops->parray[i];

      /* Iterate the header copying up to limit; this takes care of the cases
	 like while (a && b) {...}, where we want to have both of the conditions
	 copied.  */
      while (should_duplicate_loop_header_p (loop, &limit))
	{
	  preheader_edge = loop_preheader_edge (loop);
	  preheader = preheader_edge->src;

	  /* Find a successor of header that is inside a loop; i.e. the new
	     header after the condition is copied.  */
	  if (flow_bb_inside_loop_p (loop, loop->header->succ->dest))
	    succ_in_loop = loop->header->succ;
	  else
	    succ_in_loop = loop->header->succ->succ_next;

	  /* But if it has more than one predecessor, split the edge so that
	     we do not create loops with multiple latch edges.  */
	  if (!succ_in_loop->dest->pred->pred_next)
	    new_header = succ_in_loop->dest;
	  else
	    new_header = loop_split_edge_with (succ_in_loop, NULL);

	  /* Copy the condition and update the loop structures.  */
	  header_copy = duplicate_block (loop->header, preheader_edge);
	  add_bb_to_loop (header_copy, preheader->loop_father);
	  loop->latch = loop->header;
	  loop->header = new_header;

	  /* Predict the loop to be entered.  */
	  predict_edge_def (loop_preheader_edge (loop), PRED_LOOP_HEADER,
			    TAKEN);

	  /* Ensure that the latch has just a single successor.  */
	  loop_split_edge_with (loop_latch_edge (loop), NULL);
	}
    }

#ifdef ENABLE_CHECKING
  verify_loop_structure (loops);
#endif

  loop_optimizer_finalize (loops,
			   (tree_dump_flags & TDF_DETAILS
			    ? tree_dump_file : NULL));

  /* Run cleanup_tree_cfg here regardless of whether we have done anything, so
     that we cleanup the blocks created in order to get the loops into a
     canonical shape.  */
  cleanup_tree_cfg ();
}

static bool
gate_loop (void)
{
  return flag_tree_loop != 0;
}

static bool
gate_ch (void)
{
  return flag_tree_ch != 0;
}

struct tree_opt_pass pass_loop = 
{
  "loop",				/* name */
  gate_loop,				/* gate */
  tree_ssa_loop_opt,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_LOOP,				/* tv_id */
  PROP_cfg,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa	/* todo_flags_finish */
};

struct tree_opt_pass pass_ch = 
{
  "ch",					/* name */
  gate_ch,				/* gate */
  copy_loop_headers,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_CH,				/* tv_id */
  PROP_cfg,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func			/* todo_flags_finish */
};
