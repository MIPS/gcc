/* Support routines for Value Range Propagation (VRP).
   Contributed by Diego Novillo <dnovillo@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "tree.h"
#include "basic-block.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "timevar.h"
#include "diagnostic.h"

/* Given a COND_EXPR COND of the form 'V OP W', and a symbol V, return
   the assertion assignment 'V = ASSERT_EXPR <V, V OP W>'.  */

static tree
build_assert_expr_for (tree cond, tree v)
{
  tree assertion = NULL_TREE;

  gcc_assert (DECL_P (v));

  if (TREE_CODE_CLASS (TREE_CODE (cond)) == tcc_comparison)
    {
      /* Build V = ASSERT_EXPR <V, COND>.  */
      assertion = build (MODIFY_EXPR, TREE_TYPE (v), v,
		         build (ASSERT_EXPR, TREE_TYPE (v), v, cond));
    }
  else if (TREE_CODE (cond) == TRUTH_NOT_EXPR)
    {
      /* Given !V, build the assignment V = false.  */
      tree op0 = TREE_OPERAND (cond, 0);
      gcc_assert (op0 == v);
      assertion = build (MODIFY_EXPR, TREE_TYPE (v), v, boolean_false_node);
    }
  else if (DECL_P (cond))
    {
      /* Given V, build the assignment V = true.  */
      gcc_assert (v == cond);
      assertion = build (MODIFY_EXPR, TREE_TYPE (v), v, boolean_true_node);
    }
  else
    gcc_unreachable ();


  return assertion;
}


/* Return false if EXPR is a predicate expression involving floating
   point values.  */

static inline bool
fp_predicate (tree expr)
{
  return TREE_CODE_CLASS (TREE_CODE (expr)) == tcc_comparison
         && FLOAT_TYPE_P (TREE_TYPE (TREE_OPERAND (expr, 0)));
}

/* Traverse all the statements in block BB looking for used variables.
   Variables used in BB are added to bitmap FOUND.  The algorithm
   works in three main parts:

   1- For every statement S in BB, all the variables used by S are
      added to bitmap FOUND.

   2- If the last statement of BB is a conditional expression, then

      a) The variables involved in the conditional are removed from
	  FOUND.

      b) We recurse into the sub-graph starting at the THEN_CLAUSE.
	 On return, if X and/or Y are marked in FOUND, then an
	 ASSERT_EXPR is added for the corresponding variable.

      c) Step (b) is repeated on the ELSE_CLAUSE.

      d) X and Y are marked in FOUND.

   3- If BB does not end in a conditional expression, then we recurse
      into BB's dominator children.
   
   At the end of the recursive traversal, ASSERT_EXPRs will have been
   added to the edges of COND_EXPR blocks that have sub-graphs using
   one or both predicate operands.  For instance,

   	if (a == 9)
	  b = a;
	else
	  b = c + 1;

   In this case, an assertion on the THEN clause is useful to
   determine that 'a' is always 9 on that edge.  However, an assertion
   on the ELSE clause would be unnecessary.
   
   FIXME. This may still cause the SSA renamer to add a PHI node for
	  'a' at the 'endif' block.  This creates unnecessary churn in
	  the optimizers as this PHI node produces nothing new.

   TODO 1.  It may be better to do some simplified life analysis to
	    avoid adding ASSERT_EXPRs unnecessarily.  In the example
	    above, if variable 'a' is set before its first use on the
	    THEN_CLAUSE, then an ASSERT_EXPR would just be killed by
	    the first DCE run.

   TODO 2.  Handle SWITCH_EXPR.  */

static bool
maybe_add_assert_expr_on_edges (basic_block bb, sbitmap found)
{
  block_stmt_iterator si;
  tree last;
  size_t i;
  bool added;

  /* This is not a true dominator traversal, so we have to avoid
     duplicate visits.  See the logic for COND_EXPRs in 'Step 2'
     below.  */
  if (bb->flags & BB_VISITED)
    return false;

  bb->flags |= BB_VISITED;

  /* Step 1.  Mark all the variables used in BB in bitmap FOUND.  */
  added = false;
  last = NULL_TREE;
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      tree stmt;
      use_optype uses;
      
      stmt = bsi_stmt (si);
      get_stmt_operands (stmt);
      uses = STMT_USE_OPS (stmt);

      /* Mark all the variables used by STMT in bitmap FOUND. */
      for (i = 0; i < NUM_USES (uses); i++)
	SET_BIT (found, var_ann (USE_OP (uses, i))->uid);

      /* Remember the last statement of the block.  */
      last = stmt;
    }

  /* Step 2.  If BB's last statement is a conditional expression
     involving integer operands, recurse into each of the sub-graphs
     rooted at BB to determine if we need to add ASSERT_EXPRs.  */
  if (last
      && TREE_CODE (last) == COND_EXPR
      && !fp_predicate (COND_EXPR_COND (last)))
    {
      edge e;
      edge_iterator ei;
      size_t nuses;
      tree cond = COND_EXPR_COND (last);
      use_optype uses = STMT_USE_OPS (last);

      /* Remove the COND_EXPR operands from the FOUND bitmap.
	 Otherwise, when we finish traversing each of the sub-graphs,
	 we won't know whether the variables were found in the
	 sub-graphs or if they had been found in a block upstream from
	 BB.  */
      nuses = NUM_USES (uses);
      for (i = 0; i < nuses; i++)
	RESET_BIT (found, var_ann (USE_OP (uses, i))->uid);

      /* Look for uses of the operands in each of the sub-graphs
	 rooted at BB.  We need to check each of the outgoing edges
	 separately, so that we know what kind of ASSERT_EXPR to
	 insert.  */
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  /* Notice that by traversing the sub-graphs directly, we may
	     be breaking the dominator traversal we have been doing.
	     This happens if BB does not dominate one of its clauses:

		    1	if (y <= 10)
		    2	  {
		    3	    y = x + 1
		    4	    if (y > 10)
		    5	      y--;
		    6	  }
		    7	z = y + x;

	     The COND_EXPR at line 4 does not dominate its ELSE_CLAUSE
	     at line 7, but we still want to add an ASSERT_EXPR at
	     line 6 as it may still be useful.  */
	  added |= maybe_add_assert_expr_on_edges (e->dest, found);

	  for (i = 0; i < nuses; i++)
	    {
	      tree op = USE_OP (uses, i);

	      if (TEST_BIT (found, var_ann (op)->uid))
		{
		  /* We found a use of OP in the sub-graph rooted at
		     E->DEST.  Add an ASSERT_EXPR according to whether
		     E goes to THEN_CLAUSE or ELSE_CLAUSE.  */
		  tree c, t;

		  if (e->flags & EDGE_TRUE_VALUE)
		    c = unshare_expr (cond);
		  else if (e->flags & EDGE_FALSE_VALUE)
		    c = invert_truthvalue (cond);
		  else
		    gcc_unreachable ();

		  t = build_assert_expr_for (c, op);
		  bsi_insert_on_edge (e, t);
		  added = true;
		}
	    }
	}

      /* Finally, mark all the COND_EXPR operands as found.  */
      for (i = 0; i < nuses; i++)
	SET_BIT (found, var_ann (USE_OP (uses, i))->uid);
    }
  else
    {
      /* Step 3.  Recurse into the dominator children of BB.  */
      basic_block son;

      for (son = first_dom_son (CDI_DOMINATORS, bb);
	   son;
	   son = next_dom_son (CDI_DOMINATORS, son))
	added |= maybe_add_assert_expr_on_edges (son, found);
    }

  return added;
}


/* Traverse the flowgraph looking for conditional jumps to insert range
   expressions.  These range expressions are meant to provide information
   to optimizations that need to reason in terms of value ranges.  They
   will not be expanded into RTL.  For instance, given:

   x = ...
   y = ...
   if (x < y)
     y = x - 2;
   else
     x = y + 3;

   this pass will transform the code into:

   x = ...
   y = ...
   if (x < y)
    {
      x = ASSERT_EXPR <x, x < y>
      y = x - 2
    }
   else
    {
      y = ASSERT_EXPR <y, x <= y>
      x = y + 3
    }

   The idea is that once copy and constant propagation have run, other
   optimizations will be able to determine what ranges of values can 'x'
   take in different paths of the code, simply by checking the reaching
   definition of 'x'.  */

static void
execute_insert_range_assertions (void)
{
  basic_block bb;
  edge e;
  edge_iterator ei;
  sbitmap found;
  
  found = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (found);

  /* FIXME.  Passes are free to assume that BB_VISITED will always be
     clear.  This is error-prone.  We now have to assert that it is
     indeed clear to avoid hard-to-find bugs.  */
#if defined ENABLE_CHECKING
  FOR_ALL_BB (bb)
    gcc_assert (!(bb->flags & BB_VISITED));
#endif

  calculate_dominance_info (CDI_DOMINATORS);

  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR->succs)
    if (maybe_add_assert_expr_on_edges (e->dest, found))
      bsi_commit_edge_inserts ();

  FOR_ALL_BB (bb)
    bb->flags &= ~BB_VISITED;

  sbitmap_free (found);
}

struct tree_opt_pass pass_insert_range_assertions =
{
  "add_assert",				/* name */
  NULL,					/* gate */
  execute_insert_range_assertions,	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_INSERT_ASSERT,		/* tv_id */
  PROP_cfg | PROP_gimple_leh,		/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  TODO_cleanup_cfg,			/* todo_flags_start */
  TODO_dump_func | TODO_ggc_collect,	/* todo_flags_finish */
  0					/* letter */
};


/* Convert range assertion expressions into copies.  FIXME, explain why.  */

static void
execute_remove_range_assertions (void)
{
  basic_block bb;
  block_stmt_iterator si;

  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
      {
	tree stmt = bsi_stmt (si);

	if (TREE_CODE (stmt) == MODIFY_EXPR
	    && TREE_CODE (TREE_OPERAND (stmt, 1)) == ASSERT_EXPR)
	  {
	    tree rhs = TREE_OPERAND (stmt, 1);
	    tree cond = fold (TREE_OPERAND (rhs, 1));
	    gcc_assert (cond != boolean_false_node);
	    TREE_OPERAND (stmt, 1) = TREE_OPERAND (rhs, 0);
	    modify_stmt (stmt);
	  }
      }
}


struct tree_opt_pass pass_remove_range_assertions =
{
  "remove_assert",			/* name */
  NULL,					/* gate */
  execute_remove_range_assertions,	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_ggc_collect,	/* todo_flags_finish */
  0					/* letter */
};
