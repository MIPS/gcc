/* This file contains routines to construct GNU OpenMP constructs, 
   called from parsing in the C and C++ front ends.

   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>,
		  Diego Novillo <dnovillo@redhat.com>.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "function.h"
#include "c-common.h"
#include "toplev.h"
#include "tree-gimple.h"


/* Complete a #pragma omp master construct.  STMT is the structured-block
   that follows the pragma.  In this case all we need to do is wrap the
   block in a conditional that verifies that this is the master thread for
   the parallel team.  */

void
c_finish_omp_master (tree stmt)
{
  tree x;

  x = built_in_decls[BUILT_IN_OMP_GET_THREAD_NUM];
  x = build_function_call_expr (x, NULL);
  x = build2 (EQ_EXPR, boolean_type_node, x, integer_zero_node);
  x = build3 (COND_EXPR, void_type_node, x, stmt, NULL);
  add_stmt (x);
}


/* Complete a #pragma omp critical construct.  STMT is the structured-block
   that follows the pragma, NAME is the identifier in the pragma, or null
   if it was omitted.  */

void
c_finish_omp_critical (tree stmt, tree name)
{
  add_stmt (build2 (OMP_CRITICAL, void_type_node, name, stmt));
}


/* Complete a #pragma omp ordered construct.  STMT is the structured-block
   that follows the pragma.  */

void
c_finish_omp_ordered (tree stmt)
{
  tree x;

  x = built_in_decls[BUILT_IN_GOMP_ORDERED_START];
  x = build_function_call_expr (x, NULL);
  add_stmt (x);

  add_stmt (stmt);

  x = built_in_decls[BUILT_IN_GOMP_ORDERED_END];
  x = build_function_call_expr (x, NULL);
  add_stmt (x);
}


/* Complete a #pragma omp barrier construct.  */

void
c_finish_omp_barrier (void)
{
  tree x;

  x = built_in_decls[BUILT_IN_GOMP_BARRIER];
  x = build_function_call_expr (x, NULL);
  add_stmt (x);
}


/* Complete a #pragma omp atomic construct.  The expression to be 
   implemented atomically is LHS code= RHS.  */

void
c_finish_omp_atomic (enum tree_code code, tree lhs, tree rhs)
{
  tree x, type, addr;

  if (lhs == error_mark_node || rhs == error_mark_node)
    return;

  /* ??? According to one reading of the OpenMP spec, complex type are
     supported, but there are no atomic stores for any architecture.
     But at least icc 9.0 doesn't support complex types here either.
     And lets not even talk about vector types...  */
  type = TREE_TYPE (lhs);
  if (!INTEGRAL_TYPE_P (type)
      && !POINTER_TYPE_P (type)
      && !SCALAR_FLOAT_TYPE_P (type))
    {
      error ("invalid expression type for %<#pragma omp atomic%>");
      return;
    }

  /* ??? Validate that rhs does not overlap lhs.  */

  /* Take and save the address of the lhs.  From then on we'll reference it
     via indirection.  */
  addr = build_unary_op (ADDR_EXPR, lhs, 0);
  if (addr == error_mark_node)
    return;
  addr = save_expr (addr);
  lhs = build_indirect_ref (addr, NULL);

  /* There are lots of warnings, errors, and conversions that need to happen
     in the course of interpreting a statement.  Use the normal mechanisms
     to do this, and then take it apart again.  */
  x = build_modify_expr (lhs, code, rhs);
  if (x == error_mark_node)
    return;
  gcc_assert (TREE_CODE (x) == MODIFY_EXPR);  
  rhs = TREE_OPERAND (x, 1);

  /* Punt the actual generation of atomic operations to common code.  */
  add_stmt (build2 (OMP_ATOMIC, void_type_node, addr, rhs));
}


/* Complete a #pragma omp flush construct.  We don't do anything with the
   variable list that the syntax allows.  */

void
c_finish_omp_flush (void)
{
  tree x;

  x = built_in_decls[BUILT_IN_SYNCHRONIZE];
  x = build_function_call_expr (x, NULL);
  add_stmt (x);
}


/* Divide CLAUSES into two lists: those that apply to a parallel construct,
   and those that apply to a work-sharing construct.  Place the results in
   *PAR_CLAUSES and *WS_CLAUSES respectively.  In addition, add a nowait
   clause to the work-sharing list.  */

void
c_split_parallel_clauses (tree clauses, tree *par_clauses, tree *ws_clauses)
{
  tree c, next;

  *par_clauses = NULL;
  *ws_clauses = NULL;

  for (; clauses ; clauses = next)
    {
      next = TREE_CHAIN (clauses);

      switch (TREE_CODE (TREE_VALUE (clauses)))
	{
	case OMP_CLAUSE_PRIVATE:
	case OMP_CLAUSE_SHARED:
	case OMP_CLAUSE_FIRSTPRIVATE:
	case OMP_CLAUSE_REDUCTION:
	case OMP_CLAUSE_COPYIN:
	case OMP_CLAUSE_IF:
	case OMP_CLAUSE_NUM_THREADS:
	  TREE_CHAIN (clauses) = *par_clauses;
	  *par_clauses = clauses;
	  break;

	case OMP_CLAUSE_ORDERED:
	case OMP_CLAUSE_SCHEDULE:
	case OMP_CLAUSE_LASTPRIVATE:
	  TREE_CHAIN (clauses) = *ws_clauses;
	  *ws_clauses = clauses;
	  break;

	default:
	  gcc_unreachable ();
	}
    }

  c = build (OMP_CLAUSE_NOWAIT, NULL_TREE);
  c = tree_cons (NULL_TREE, c, *ws_clauses);
  *ws_clauses = c;
}

/* Validate and emit code for the OpenMP directive #pragma omp for.
   INIT, COND, INCR and BODY are the four basic elements of the loop
   (initialization expression, controlling predicate, increment
   expression and body of the loop).  CLAUSES is the set of data
   sharing and copying clauses found at the start of the directive.  */

tree
c_finish_omp_for (tree init, tree cond, tree incr, tree body, tree clauses)
{
  bool found;
  tree t, loop_ix = NULL_TREE;

  /* Validate the form of the loop.  It must be of the form (OpenMP
     public spec v2.5)

     	for (init-expr; var relop b; incr-expr)

	init-expr	One of the following
				var = lb
				integer-type var = lb

	incr-expr	One of the following
				++var
				var++
				--var
				var--
				var += incr
				var -= incr
				var = var + incr
				var = incr + var
				var = var - incr

	var		A signed integer variable.  If it was shared, 
			it is implicitly made private.  It may only be
			modified inside incr-expr.  After the loop its
			value is indeterminate, unles it is marked
			lastprivate.

	relop		One of <, <=, > or >=

	lb, b, incr	Loop invariant integer expressions.  There is
			no synchronization during the evaluation of
			these expressions.  Th order, frequency and
			side-effects of these expressions are
			unspecified.  */
  if (cond == NULL_TREE)
    {
      error ("missing controlling predicate in %<omp for%> loop");
      return NULL_TREE;
    }
  else
    {
      bool cond_ok = false;

      if (TREE_CODE (cond) == LT_EXPR
	  || TREE_CODE (cond) == LE_EXPR
	  || TREE_CODE (cond) == GT_EXPR
	  || TREE_CODE (cond) == GE_EXPR)
	{
	  loop_ix = TREE_OPERAND (cond, 0);
	  if (DECL_P (loop_ix))
	    cond_ok = true;
	}

      if (!cond_ok)
	{
	  error ("invalid controlling predicate in %<omp for%> loop");
	  return NULL_TREE;
	}
    }

  /* If we got to this point, we must have a loop index variable.  */
  gcc_assert (loop_ix);

  if (incr == NULL_TREE)
    {
      error ("missing increment expression in %<omp for%> loop");
      return NULL_TREE;
    }
  else
    {
      bool incr_ok = false;
      enum tree_code code = TREE_CODE (incr);

      /* Check all the valid increment expressions: v++, v--, ++v, --v,
	 v = v + incr, v = incr + v and v = v - incr.  */
      if ((code == POSTINCREMENT_EXPR && TREE_OPERAND (incr, 0) == loop_ix)
	  || (code == PREINCREMENT_EXPR && TREE_OPERAND (incr, 0) == loop_ix)
	  || (code == POSTDECREMENT_EXPR && TREE_OPERAND (incr, 0) == loop_ix)
	  || (code == PREDECREMENT_EXPR && TREE_OPERAND (incr, 0) == loop_ix)
	  || (code == MODIFY_EXPR
	      && TREE_CODE (TREE_OPERAND (incr, 1)) == PLUS_EXPR
	      && (TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == loop_ix
		  || TREE_OPERAND (TREE_OPERAND (incr, 1), 1) == loop_ix))
	  || (code == MODIFY_EXPR
	      && TREE_CODE (TREE_OPERAND (incr, 1)) == MINUS_EXPR
	      && TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == loop_ix))
	incr_ok = true;

      if (!incr_ok)
	{
	  error ("invalid increment expression in %<omp for%> loop");
	  return NULL_TREE;
	}
    }

  if (init == NULL_TREE)
    {
      if (flag_isoc99)
	{
	  /* Only in C99 may the init expression be empty.  If the
	     loop index variable has a DECL_INITIAL expression, use it
	     to build the OMP_FOR_INIT operand.  */
	  if (DECL_INITIAL (loop_ix))
	    init = build (MODIFY_EXPR, TREE_TYPE (loop_ix),
			  loop_ix, DECL_INITIAL (loop_ix));
	}

      if (init == NULL_TREE)
	{
	  error ("missing initialization expression in %<omp for%> loop");
	  return NULL_TREE;
	}
    }

  /* The loop controlling variable is always private.  Add it to the
     list of private clauses.  */
  found = false;
  for (t = clauses; t; t = TREE_CHAIN (t))
    {
      tree clause = TREE_VALUE (t);

      if (TREE_CODE (clause) == OMP_CLAUSE_PRIVATE)
	{
	  tree n;

	  for (n = OMP_PRIVATE_VARS (clause); n; n = TREE_CHAIN (n))
	    {
	      tree v = TREE_VALUE (n);
	      if (v == loop_ix)
		{
		  found = true;
		  break;
		}
	    }

	  /* If LOOP_IX is not mentioned in a private clause, add it.  */
	  if (!found)
	    {
	      OMP_PRIVATE_VARS (clause) = 
		tree_cons (NULL_TREE, loop_ix, OMP_PRIVATE_VARS (clause));
	      found = true;
	      break;
	    }
	}
    }

  /* If we did not even have a private clause, add one.  */
  if (!found)
    clauses = tree_cons (NULL_TREE,
			 build (OMP_CLAUSE_PRIVATE,
				NULL_TREE,
				tree_cons (NULL_TREE, loop_ix, NULL_TREE)),
			 clauses);

  /* Build and return an OMP_FOR tree.  */
  return (build (OMP_FOR, void_type_node, clauses, init, cond, incr, body));
}
