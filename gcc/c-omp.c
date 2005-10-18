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
#include "bitmap.h"
#include "langhooks.h"


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

tree
c_finish_omp_critical (tree body, tree name)
{
  tree stmt = make_node (OMP_CRITICAL);
  TREE_TYPE (stmt) = void_type_node;
  OMP_CRITICAL_BODY (stmt) = body;
  OMP_CRITICAL_NAME (stmt) = name;
  return add_stmt (stmt);
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


/* Validate and emit code for the OpenMP directive #pragma omp for.
   INIT, COND, INCR and BODY are the four basic elements of the loop
   (initialization expression, controlling predicate, increment
   expression and body of the loop).  DECL is the iteration variable.  */

tree
c_finish_omp_for (location_t locus, tree decl, tree init, tree cond,
		  tree incr, tree body)
{
  location_t elocus = locus;
  bool fail = false;

  if (EXPR_HAS_LOCATION (init))
    elocus = EXPR_LOCATION (init);

  /* Validate the iteration variable.  */
  if (!INTEGRAL_TYPE_P (TREE_TYPE (decl)))
    {
      error ("%Hinvalid type for iteration variable %qE", &elocus, decl);
      fail = true;
    }
  if (TYPE_UNSIGNED (TREE_TYPE (decl)))
    warning (0, "%Hiteration variable %qE is unsigned", &elocus, decl);

  /* In the case of "for (int i = 0...)", init will be a decl.  It should
     have a DECL_INITIAL that we can turn into an assignment.  */
  if (init == decl)
    {
      elocus = DECL_SOURCE_LOCATION (decl);

      init = DECL_INITIAL (decl);
      if (init == NULL)
	{
	  error ("%H%qE is not initialized", &elocus, decl);
	  init = integer_zero_node;
	  fail = true;
	}

      init = build_modify_expr (decl, NOP_EXPR, init);
      SET_EXPR_LOCATION (init, elocus);
    }
  gcc_assert (TREE_CODE (init) == MODIFY_EXPR);
  gcc_assert (TREE_OPERAND (init, 0) == decl);
  
  if (cond == NULL_TREE)
    {
      error ("%Hmissing controlling predicate", &elocus);
      fail = true;
    }
  else
    {
      bool cond_ok = false;

      if (EXPR_HAS_LOCATION (cond))
	elocus = EXPR_LOCATION (cond);

      if (TREE_CODE (cond) == LT_EXPR
	  || TREE_CODE (cond) == LE_EXPR
	  || TREE_CODE (cond) == GT_EXPR
	  || TREE_CODE (cond) == GE_EXPR)
	{
	  if (decl == TREE_OPERAND (cond, 0))
	    cond_ok = true;
	  else if (decl == TREE_OPERAND (cond, 1))
	    {
	      TREE_SET_CODE (cond, swap_tree_comparison (TREE_CODE (cond)));
	      TREE_OPERAND (cond, 1) = TREE_OPERAND (cond, 0);
	      TREE_OPERAND (cond, 0) = decl;
	      cond_ok = true;
	    }
	}

      if (!cond_ok)
	{
	  error ("%Hinvalid controlling predicate", &elocus);
	  fail = true;
	}
    }

  if (incr == NULL_TREE)
    {
      error ("%Hmissing increment expression", &elocus);
      fail = true;
    }
  else
    {
      bool incr_ok = false;

      if (EXPR_HAS_LOCATION (incr))
	elocus = EXPR_LOCATION (incr);

      /* Check all the valid increment expressions: v++, v--, ++v, --v,
	 v = v + incr, v = incr + v and v = v - incr.  */
      switch (TREE_CODE (incr))
	{
	case POSTINCREMENT_EXPR:
	case PREINCREMENT_EXPR:
	case POSTDECREMENT_EXPR:
	case PREDECREMENT_EXPR:
	  incr_ok = (TREE_OPERAND (incr, 0) == decl);
	  break;

	case MODIFY_EXPR:
	  if (TREE_CODE (TREE_OPERAND (incr, 1)) == PLUS_EXPR
	      && (TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == decl
		  || TREE_OPERAND (TREE_OPERAND (incr, 1), 1) == decl))
	    incr_ok = true;
	  else if (TREE_CODE (TREE_OPERAND (incr, 1)) == MINUS_EXPR
		   && TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == decl)
	    incr_ok = true;
	  break;

	default:
	  break;
	}
      if (!incr_ok)
	{
	  error ("%Hinvalid increment expression", &elocus);
	  fail = true;
	}
    }

  if (fail)
    return NULL;
  else
    {
      tree t = make_node (OMP_FOR);

      TREE_TYPE (t) = void_type_node;
      OMP_FOR_INIT (t) = init;
      OMP_FOR_COND (t) = cond;
      OMP_FOR_INCR (t) = incr;
      OMP_FOR_BODY (t) = body;

      SET_EXPR_LOCATION (t, locus);
      return add_stmt (t);
    }
}


/* Divide CLAUSES into two lists: those that apply to a parallel construct,
   and those that apply to a work-sharing construct.  Place the results in
   *PAR_CLAUSES and *WS_CLAUSES respectively.  In addition, add a nowait
   clause to the work-sharing list.  */

void
c_split_parallel_clauses (tree clauses, tree *par_clauses, tree *ws_clauses)
{
  tree next;

  *par_clauses = NULL;
  *ws_clauses = make_node (OMP_CLAUSE_NOWAIT);

  for (; clauses ; clauses = next)
    {
      next = OMP_CLAUSE_CHAIN (clauses);

      switch (TREE_CODE (clauses))
	{
	case OMP_CLAUSE_PRIVATE:
	case OMP_CLAUSE_SHARED:
	case OMP_CLAUSE_FIRSTPRIVATE:
	case OMP_CLAUSE_LASTPRIVATE:
	case OMP_CLAUSE_REDUCTION:
	case OMP_CLAUSE_COPYIN:
	case OMP_CLAUSE_IF:
	case OMP_CLAUSE_NUM_THREADS:
	case OMP_CLAUSE_DEFAULT:
	  OMP_CLAUSE_CHAIN (clauses) = *par_clauses;
	  *par_clauses = clauses;
	  break;

	case OMP_CLAUSE_SCHEDULE:
	case OMP_CLAUSE_ORDERED:
	  OMP_CLAUSE_CHAIN (clauses) = *ws_clauses;
	  *ws_clauses = clauses;
	  break;

	default:
	  gcc_unreachable ();
	}
    }
}
