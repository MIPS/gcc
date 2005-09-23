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
#include "c-tree.h"
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
  tree lock, unlock;

  if (name)
    {
      char *new_str;
      tree decl, args;

      new_str = ACONCAT ((".gomp_critical_user_",
			  IDENTIFIER_POINTER (name), NULL));
      name = get_identifier (new_str);
      decl = identifier_global_value (name);
      if (decl == NULL)
	{
	  decl = build_decl (VAR_DECL, name, ptr_type_node);
	  TREE_PUBLIC (decl) = 1;
	  TREE_STATIC (decl) = 1;
	  DECL_COMMON (decl) = 1;
	  DECL_ARTIFICIAL (decl) = 1;
	  DECL_IGNORED_P (decl) = 1;
	  decl = pushdecl_top_level (decl);
	}

      args = tree_cons (NULL, build_fold_addr_expr (decl), NULL);
      lock = built_in_decls[BUILT_IN_GOMP_CRITICAL_NAME_START];
      lock = build_function_call_expr (lock, args);

      args = tree_cons (NULL, build_fold_addr_expr (decl), NULL);
      unlock = built_in_decls[BUILT_IN_GOMP_CRITICAL_NAME_END];
      unlock = build_function_call_expr (unlock, args);
    }
  else
    {
      lock = built_in_decls[BUILT_IN_GOMP_CRITICAL_START];
      lock = build_function_call_expr (lock, NULL);

      unlock = built_in_decls[BUILT_IN_GOMP_CRITICAL_END];
      unlock = build_function_call_expr (unlock, NULL);
    }

  add_stmt (lock);
  add_stmt (stmt);
  add_stmt (unlock);
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
  tree decl, x, y;
  tree oldval, newval, rhsval, lhsaddr, type, label;
  tree itype, oldival, newival, lhsiaddr;

  if (lhs == error_mark_node || rhs == error_mark_node)
    return;

  type = TYPE_MAIN_VARIANT (TREE_TYPE (lhs));

  /* We want rhs converted into a type compatible with lhs, without the
     default promotions that would happen for normal arithmetic.  The
     easiest way to do this is to build a dummy assignment expression
     and then extract the components.  Except that that doesn't work for
     pointer types, so we have to handle that by hand.  */
  if (POINTER_TYPE_P (type))
    {
      if ((code != PLUS_EXPR && code != MINUS_EXPR)
	  || !INTEGRAL_TYPE_P (TREE_TYPE (rhs)))
	{
	  binary_op_error (code);
	  return;
	}

      x = pointer_int_sum (code, lhs, rhs);
      /* Stupid NON_LVALUE_EXPR... */
      STRIP_TYPE_NOPS (x);
      /* Be careful: pointer_int_sum calls fold.  Here we hope it can't
	 do too much besides simplify to no operation.  */
      if (x == lhs)
	return;
      gcc_assert (TREE_CODE (x) == code);
      gcc_assert (TREE_OPERAND (x, 0) == lhs);
      rhs = TREE_OPERAND (x, 1);
    }
  else if (INTEGRAL_TYPE_P (type) || SCALAR_FLOAT_TYPE_P (type))
    {
      x = build_modify_expr (lhs, NOP_EXPR, rhs);
      if (x == error_mark_node)
	return;
      gcc_assert (TREE_CODE (x) == MODIFY_EXPR);  
      lhs = TREE_OPERAND (x, 0);
      rhs = TREE_OPERAND (x, 1);
    }
  else
    {
      error ("invalid expression type for %<#pragma omp atomic%>");
      return;
    }

  lhsaddr = build_unary_op (ADDR_EXPR, lhs, 0);
  if (lhsaddr == error_mark_node)
    return;

  /* When possible, use specialized atomic update functions.  */
  if (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type))
    switch (code)
      {
      case PLUS_EXPR:
	decl = built_in_decls[BUILT_IN_FETCH_AND_ADD_N];
	goto do_fetch_op;
      case MINUS_EXPR:
	decl = built_in_decls[BUILT_IN_FETCH_AND_SUB_N];
	goto do_fetch_op;
      case BIT_AND_EXPR:
	decl = built_in_decls[BUILT_IN_FETCH_AND_AND_N];
	goto do_fetch_op;
      case BIT_IOR_EXPR:
	decl = built_in_decls[BUILT_IN_FETCH_AND_OR_N];
	goto do_fetch_op;
      case BIT_XOR_EXPR:
	decl = built_in_decls[BUILT_IN_FETCH_AND_XOR_N];
	goto do_fetch_op;

      do_fetch_op:
	y = tree_cons (NULL, rhs, NULL);
	y = tree_cons (NULL, lhsaddr, y);
	x = resolve_overloaded_builtin (decl, y);
	add_stmt (x);
	return;

      default:
	break;
      }

  /* In these cases, we don't have specialized __sync builtins,
     so we need to implement a compare and swap loop.  */

  itype = NULL;
  if (SCALAR_FLOAT_TYPE_P (type))
    {
      if (TYPE_PRECISION (type) == 32 || TYPE_PRECISION (type) == 64)
	itype = c_common_type_for_size (TYPE_PRECISION (type), true);
      else
	{
	  sorry ("unsupported expression type for %<#pragma omp atomic%>");
	  return;
	}
    }

  oldival = oldval = pushdecl (create_tmp_var_raw (type, NULL));
  newival = newval = pushdecl (create_tmp_var_raw (type, NULL));
  lhsiaddr = lhsaddr = save_expr (lhsaddr);
  if (itype)
    {
      oldival = pushdecl (create_tmp_var_raw (itype, NULL));
      newival = pushdecl (create_tmp_var_raw (itype, NULL));
      lhsiaddr = fold_convert (build_pointer_type (itype), lhsaddr);
    }
  rhsval = pushdecl (create_tmp_var_raw (type, NULL));
  label = create_artificial_label ();

  x = build_fold_indirect_ref (lhsaddr);
  x = build2 (MODIFY_EXPR, void_type_node, oldval, x);
  add_stmt (x);

  if (itype)
    {
      x = build1 (VIEW_CONVERT_EXPR, itype, oldval);
      x = build2 (MODIFY_EXPR, void_type_node, oldival, x);
      add_stmt (x);
    }

  if (TREE_CONSTANT (rhs))
    rhsval = rhs;
  else
    {
      x = build2 (MODIFY_EXPR, void_type_node, rhsval, rhs);
      add_stmt (x);
    }

  add_stmt (build_stmt (LABEL_EXPR, label));

  x = build_binary_op (code, oldval, rhsval, false);
  x = build2 (MODIFY_EXPR, void_type_node, newval, x);
  add_stmt (x);

  if (itype)
    {
      x = build1 (VIEW_CONVERT_EXPR, itype, newval);
      x = build2 (MODIFY_EXPR, void_type_node, newival, x);
      add_stmt (x);
    }

  y = tree_cons (NULL, newival, NULL);
  y = tree_cons (NULL, oldival, y);
  y = tree_cons (NULL, lhsiaddr, y);
  decl = built_in_decls[BUILT_IN_VAL_COMPARE_AND_SWAP_N];
  x = resolve_overloaded_builtin (decl, y);
  x = build2 (MODIFY_EXPR, void_type_node, oldival, x);
  add_stmt (x);

  if (itype)
    {
      x = build1 (VIEW_CONVERT_EXPR, type, oldival);
      x = build2 (MODIFY_EXPR, void_type_node, oldval, x);
      add_stmt (x);
    }

  x = build2 (NE_EXPR, boolean_type_node, oldival, newival);
  y = build1 (GOTO_EXPR, void_type_node, label);
  x = build3 (COND_EXPR, void_type_node, x, y, NULL);
  add_stmt (x);
  return;
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
