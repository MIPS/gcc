/* Elimination of redundant checks.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <sebastian.pop@cri.ensmp.fr>

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

/* 
   Description:
   
     Compute the scalar evolutions for all the scalar variables of a
     condition expression, and based on this information performs a
     proof.  The condition is rewritten based on the result of this
     proof.

   Examples:
   
     Example 1: A simple illustration of the algorithm.
     
     Given the COND_EXPR "if (a < b)" with "a -> {2, +, 1}_1" and "b
     -> {3, +, 1}_1", the proof consists in comparing these evolution
     functions: is it always true for a given iteration x that "{2, +,
     1}_1 (x) < {3, +, 1}_1 (x)"?  The answer is yes, and the test of
     the condition is consequently replaced by "1".  

   Further readings:
   
     There is no further readings for the moment.  

     Based on the fact that this algorithm is similar to the Value
     Range Propagation you can have a look at the corresponding
     papers.  
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "tree-fold-const.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-pass.h"
#include "flags.h"

static void remove_redundant_check (tree, bool);
static void try_eliminate_check (tree);
static void scan_all_loops_r (struct loop *);

/* Remove the check by setting the condition COND to VALUE.  */

static void 
remove_redundant_check (tree cond, bool value)
{
  /* A dead COND_EXPR means the condition is dead. We don't change any
     flow, just replace the expression with a constant.  */
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Replacing one of the conditions.\n");

  if (value == true)
    COND_EXPR_COND (cond) = integer_one_node;
  
  else
    COND_EXPR_COND (cond) = integer_zero_node;
  
  modify_stmt (cond);
}

/* If the condition TEST is decidable at compile time, then eliminate
   the check.  */

static void
try_eliminate_check (tree cond)
{
  bool value;
  tree test, opnd0, opnd1;
  tree chrec0, chrec1;
  unsigned loop_nb = loop_num (loop_of_stmt (cond));

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(try_eliminate_check \n");
      fprintf (tree_dump_file, "  (cond = ");
      print_generic_expr (tree_dump_file, cond, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  test = COND_EXPR_COND (cond);
  switch (TREE_CODE (test))
    {
    case SSA_NAME:
      /* Matched "if (opnd0)" ie, "if (opnd0 != 0)".  */
      opnd0 = test;
      chrec0 = analyze_scalar_evolution (loop_nb, opnd0);
      if (chrec_contains_undetermined (chrec0))
	break;
      chrec0 = instantiate_parameters (loop_nb, chrec0);
      
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "  (test = ");
	  print_generic_expr (tree_dump_file, test, 0);
	  fprintf (tree_dump_file, ")\n  (loop_nb = %d)\n  (chrec0 = ", loop_nb);
	  print_generic_expr (tree_dump_file, chrec0, 0);
	  fprintf (tree_dump_file, ")\n");
	}
      
      if (prove_truth_value_ne (chrec0, integer_zero_node, &value))
	remove_redundant_check (cond, value);
      break;

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
      opnd0 = TREE_OPERAND (test, 0);
      opnd1 = TREE_OPERAND (test, 1);
      chrec0 = analyze_scalar_evolution (loop_nb, opnd0);
      if (chrec_contains_undetermined (chrec0))
	break;
      
      chrec1 = analyze_scalar_evolution (loop_nb, opnd1);
      if (chrec_contains_undetermined (chrec1))
	break;
      
      chrec0 = instantiate_parameters (loop_nb, chrec0);
      chrec1 = instantiate_parameters (loop_nb, chrec1);
      
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "  (test = ");
	  print_generic_expr (tree_dump_file, test, 0);
	  fprintf (tree_dump_file, ")\n  (loop_nb = %d)\n  (chrec0 = ", loop_nb);
	  print_generic_expr (tree_dump_file, chrec0, 0);
	  fprintf (tree_dump_file, ")\n  (chrec1 = ");
	  print_generic_expr (tree_dump_file, chrec1, 0);
	  fprintf (tree_dump_file, ")\n");
	}
      
      switch (TREE_CODE (test))
	{
	case LT_EXPR:
	  if (prove_truth_value_lt (chrec0, chrec1, &value))
	    remove_redundant_check (cond, value);
	  break;
	  
	case LE_EXPR:
	  if (prove_truth_value_le (chrec0, chrec1, &value))
	    remove_redundant_check (cond, value);
	  break;
	  
	case GT_EXPR:
	  if (prove_truth_value_gt (chrec0, chrec1, &value))
	    remove_redundant_check (cond, value);
	  break;
	  
	case GE_EXPR:
	  if (prove_truth_value_ge (chrec0, chrec1, &value))
	    remove_redundant_check (cond, value);
	  break;
	    
	case EQ_EXPR:
	  if (prove_truth_value_eq (chrec0, chrec1, &value))
	    remove_redundant_check (cond, value);
	  break;
	  
	case NE_EXPR:
	  if (prove_truth_value_ne (chrec0, chrec1, &value))
	    remove_redundant_check (cond, value);
	  break;
	  
	default:
	  break;
	}
      break;
      
    default:
      break;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}

/* Compute the exit edges for all the loops.  */

static void 
scan_all_loops_r (struct loop *loop)
{
  if (!loop)
    return;
  
  /* Recurse on the inner loops, then on the next (sibling) loops.  */
  scan_all_loops_r (inner_loop (loop));
  scan_all_loops_r (next_loop (loop));
  
  flow_loop_scan (loop, LOOP_EXIT_EDGES);
}

/* Walk over all the statements, searching for conditional statements.
   
   A better way to determine the conditional expressions that are good
   candidates for elimination would be needed.  For the moment
   systematically search the conditional expressions over the whole
   function.  */

void 
eliminate_redundant_checks (void)
{
  basic_block bb;
  block_stmt_iterator bsi;
  
  bb = BASIC_BLOCK (0);
  if (bb && bb->loop_father)
    {
      scan_all_loops_r (bb->loop_father);
      
      FOR_EACH_BB (bb)
	{
	  struct loop *loop = bb->loop_father;
	  
	  /* Don't try to prove anything about the loop exit
	     conditions: avoid the block that contains the condition
	     that guards the exit of the loop.  */
	  if (!loop_exit_edges (loop)
	      || edge_source (loop_exit_edge (loop, 0)) == bb)
	    continue;
	  
	  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	    {
	      tree expr = bsi_stmt (bsi);
	      
	      switch (TREE_CODE (expr))
		{
		case COND_EXPR:
		  try_eliminate_check (expr);
		  break;
		  
		default:
		  break;
		}
	    }
	}
    }
}

