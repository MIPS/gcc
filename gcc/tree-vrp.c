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

/* Given a comparison code, return its opposite.  Note that this is *not*
   the same as inverting its truth value (invert_tree_comparison).  Here we
   just want to literally flip the comparison around.
   
   So, '<' gets '>', '<=' gets '>='.  Both '==' and '!=' are returned
   unchanged.  */

static enum tree_code
opposite_comparison (enum tree_code code)
{
  switch (code)
    {
    case EQ_EXPR:
    case NE_EXPR:
    case ORDERED_EXPR:
    case UNORDERED_EXPR:
    case LTGT_EXPR:
    case UNEQ_EXPR:
      return code;
    case GT_EXPR:
      return LT_EXPR;
    case GE_EXPR:
      return LE_EXPR;
    case LT_EXPR:
      return GT_EXPR;
    case LE_EXPR:
      return GE_EXPR;
    case UNGT_EXPR:
      return UNLT_EXPR;
    case UNGE_EXPR:
      return UNLE_EXPR;
    case UNLT_EXPR:
      return UNGT_EXPR;
    case UNLE_EXPR:
      return UNGE_EXPR;
    default:
      gcc_unreachable ();
    }
}


/* Given a COND_EXPR node E of the form 'V OP W', return a list of
   assignments to ASSERT_EXPRs that mimic the comparison operand OP.
   For instance, given 'V <= W', this function returns the assignments:

   	V = ASSERT_EXPR <V, V <= W>
	W = ASSERT_EXPR <W, W >= V>  */

static tree
build_assert_exprs (tree cond)
{
  tree list = NULL_TREE;
  tree a1 = NULL_TREE;
  tree a2 = NULL_TREE;

  if (TREE_CODE_CLASS (TREE_CODE (cond)) == tcc_comparison)
    {
      /* Given X OP Y, build the assignments

	 	X = ASSERT_EXPR <X, X OP Y>
		Y = ASSERT_EXPR <Y, Y opposite<OP> X>

	 Make sure that either X or Y are a renameable _DECL,
	 otherwise we won't be able to create the assignments.  */
      tree op0 = TREE_OPERAND (cond, 0);
      tree op1 = TREE_OPERAND (cond, 1);

      cond = unshare_expr (cond);

      if (DECL_P (op0))
	{
	  a1 = build (MODIFY_EXPR, TREE_TYPE (op0), op0,
		      build (ASSERT_EXPR, TREE_TYPE (op0), op0, cond));
	}

      if (DECL_P (op1))
	{
	  /* Switch the operands around and use the opposite comparison.  */
	  cond = build (opposite_comparison (TREE_CODE (cond)),
	                TREE_TYPE (cond), op1, op0);
	  a2 = build (MODIFY_EXPR, TREE_TYPE (op1), op1,
		      build (ASSERT_EXPR, TREE_TYPE (op1), op1, cond));
	}
    }
  else if (TREE_CODE (cond) == TRUTH_NOT_EXPR)
    {
      /* Given !X, build the assignment X = false.  */
      tree op0 = TREE_OPERAND (cond, 0);
      if (DECL_P (op0))
	a1 = build (MODIFY_EXPR, TREE_TYPE (op0), op0, boolean_false_node);
    }
  else if (DECL_P (cond))
    {
      /* Given X, build the assignment X = true.  */
      a1 = build (MODIFY_EXPR, TREE_TYPE (cond), cond, boolean_true_node);
    }
  else
    gcc_unreachable ();

  if (a1)
    {
      list = alloc_stmt_list ();
      append_to_statement_list (a1, &list);
      if (a2) append_to_statement_list (a2, &list);
    }

  return list;
}


/* Given a list L of assertion assignments, build another list with the
   inverted version of each assertion.  Assertions are inverted by
   calling invert_truthvalue().  */

static tree
negate_assert_exprs (tree l)
{
  tree_stmt_iterator i;
  tree neg_l;

  neg_l = alloc_stmt_list ();

  for (i = tsi_start (l); !tsi_end_p (i); tsi_next (&i))
    {
      tree rhs, a, neg_a;

      a = tsi_stmt (i);

      gcc_assert (TREE_CODE (a) == MODIFY_EXPR);

      neg_a = unshare_expr (a);
      rhs = TREE_OPERAND (neg_a, 1);

      if (TREE_CODE (rhs) == ASSERT_EXPR)
	{
	  tree op1 = TREE_OPERAND (rhs, 1);
	  TREE_OPERAND (rhs, 1) = invert_truthvalue (op1);

	  /* The inverted conditional must be GIMPLE, otherwise we lose
	     the ability to relate the variable on the LHS of the
	     assignment of an ASSERT_EXPR.  */
	  gcc_assert (is_gimple_condexpr (TREE_OPERAND (rhs, 1)));
	}
      else if (TREE_CODE (rhs) == TRUTH_NOT_EXPR)
	{
	  tree op0 = TREE_OPERAND (rhs, 0);
	  TREE_OPERAND (rhs, 0) = invert_truthvalue (op0);
	}
      else if (rhs == boolean_true_node
	       || rhs == boolean_false_node)
	{
	  TREE_OPERAND (neg_a, 1) = invert_truthvalue (rhs);
	}
      else
	gcc_unreachable ();

      append_to_statement_list (neg_a, &neg_l);
    }

  return neg_l;
}


/* Return false if EXPR is a predicate expression involving floating
   point values.  */

static inline bool
fp_predicate (tree expr)
{
  return TREE_CODE_CLASS (TREE_CODE (expr)) == tcc_comparison
         && FLOAT_TYPE_P (TREE_TYPE (TREE_OPERAND (expr, 0)));
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
      y = ASSERT_EXPR <y, y > x>
      y = x - 2
    }
   else
    {
      x = ASSERT_EXPR <x, x >= y>
      y = ASSERT_EXPR <y, y <= x>
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

  FOR_EACH_BB (bb)
    {
      tree stmt = last_stmt (bb);

      /* The last statement of the block must be an integer
	 comparison involving at least one renameable _DECL.  */
      if (stmt
	  && TREE_CODE (stmt) == COND_EXPR
	  && !fp_predicate (TREE_OPERAND (stmt, 0)))
	{
	  edge e;
	  edge_iterator ei;
	  tree assertions, neg_assertions;

	  assertions = build_assert_exprs (TREE_OPERAND (stmt, 0));
	  if (assertions)
	    neg_assertions = negate_assert_exprs (assertions);

	  if (assertions)
	    FOR_EACH_EDGE (e, ei, bb->succs)
	      {
		if (e->flags & EDGE_TRUE_VALUE)
		  bsi_insert_on_edge (e, assertions);
		else if ((e->flags & EDGE_FALSE_VALUE)
		         && neg_assertions)
		  bsi_insert_on_edge (e, neg_assertions);
		else
		  gcc_unreachable ();
	      }
	}
    }

  bsi_commit_edge_inserts ();
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
