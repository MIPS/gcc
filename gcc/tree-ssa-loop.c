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
#include "timevar.h"
#include "cfgloop.h"
#include "tree-inline.h"

FILE *loop_dump_file;
int loop_dump_flags;

/* The main entry into loop optimization pass.  PHASE indicates which dump file
   from the DUMP_FILES array to use when dumping debugging information.
   FNDECL is the current function decl.  */

void
tree_ssa_loop_opt (tree fndecl ATTRIBUTE_UNUSED,
		   enum tree_dump_index phase ATTRIBUTE_UNUSED)
{
  struct loops *loops;

  loop_dump_file = dump_begin (phase, &loop_dump_flags);

  timevar_push (TV_TREE_LOOP);
  loops = loop_optimizer_init (loop_dump_file);

  if (loops)
    {
      /* Ensure there is a place to move the computations to.  */
      create_preheaders (loops, 0);

      /* Move the expensive loop invariants.  */
      tree_ssa_lim (loops);

      loop_optimizer_finalize (loops,
			       ((loop_dump_flags & TDF_DETAILS)
				? loop_dump_file 
				: NULL));
      cleanup_tree_cfg ();
    }
  timevar_pop (TV_TREE_LOOP);

  if (loop_dump_file)
    {
      dump_function_to_file (fndecl, loop_dump_file, loop_dump_flags);
      dump_end (phase, loop_dump_file);
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
   of the loop.  PHASE indicates which dump file from the DUMP_FILES array
   to use when dumping debugging information.  FNDECL is the current function
   decl.  */

void
copy_loop_headers (tree fndecl, enum tree_dump_index phase)
{
  struct loops *loops;
  unsigned i;
  struct loop *loop;
  basic_block header_copy, preheader, new_header;
  edge preheader_edge, succ_in_loop;

  timevar_push (TV_TREE_LOOP);

  loop_dump_file = dump_begin (phase, &loop_dump_flags);

  loops = loop_optimizer_init (loop_dump_file);
  if (!loops)
    {
      if (loop_dump_file)
	dump_end (phase, loop_dump_file);
      timevar_pop (TV_TREE_LOOP);
      return;
    }
  
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
	  header_copy = tree_duplicate_bb (loop->header, preheader_edge);
	  add_bb_to_loop (header_copy, preheader->loop_father);
	  loop->latch = loop->header;
	  loop->header = new_header;

	  /* Ensure that the latch has just a single successor.  */
	  loop_split_edge_with (loop_latch_edge (loop), NULL);
	}
    }

#ifdef ENABLE_CHECKING
  verify_loop_structure (loops);
#endif

  loop_optimizer_finalize (loops,
			   ((loop_dump_flags & TDF_DETAILS)
			    ? loop_dump_file 
			    : NULL));

  timevar_pop (TV_TREE_LOOP);

  /* Run cleanup_tree_cfg here regardless of whether we have done anything, so
     that we cleanup the blocks created in order to get the loops into a
     canonical shape.  */
  cleanup_tree_cfg ();

  if (loop_dump_file)
    {
      dump_function_to_file (fndecl, loop_dump_file, loop_dump_flags);
      dump_end (phase, loop_dump_file);
    }
}
