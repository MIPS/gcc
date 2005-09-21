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
c_finish_omp_atomic (tree expr)
{
  tree lhs, rhs, decl, x, y;
  enum tree_code code;
  tree oldval, newval, rhsval, lhsaddr, type, label;

  switch (TREE_CODE (expr))
    {
    case PREINCREMENT_EXPR:
    case POSTINCREMENT_EXPR:
      code = PLUS_EXPR;
      lhs = TREE_OPERAND (expr, 0);
      rhs = fold_convert (TREE_TYPE (lhs), integer_one_node);
      break;

    case PREDECREMENT_EXPR:
    case POSTDECREMENT_EXPR:
      code = MINUS_EXPR;
      lhs = TREE_OPERAND (expr, 0);
      rhs = fold_convert (TREE_TYPE (lhs), integer_one_node);
      break;

    case MODIFY_EXPR:
      lhs = TREE_OPERAND (expr, 0);
      expr = TREE_OPERAND (expr, 1);
      code = TREE_CODE (expr);
      switch (code)
	{
	case PLUS_EXPR:
	case MINUS_EXPR:
	case BIT_AND_EXPR:
	case BIT_IOR_EXPR:
	case BIT_XOR_EXPR:
	case MULT_EXPR:
	case TRUNC_DIV_EXPR:
	case RDIV_EXPR:
	case LSHIFT_EXPR:
	case RSHIFT_EXPR:
	  x = TREE_OPERAND (expr, 0);
	  y = TREE_OPERAND (expr, 1);
	  break;

	default:
	  goto invalid;
	}

      if (operand_equal_p (lhs, x, 0))
	;
      else if (commutative_tree_code (code)
	       && operand_equal_p (lhs, rhs, 0))
	rhs = x;
      else
	goto invalid;
      break;

    default:
      goto invalid;
    }

  /* ??? The OpenMP spec appears to allow floating-point types.  */
  type = TREE_TYPE (lhs);
  if (!INTEGRAL_TYPE_P (type))
    goto invalid;

  lhsaddr = build_unary_op (ADDR_EXPR, lhs, 0);

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

  oldval = create_tmp_var_raw (type, NULL);
  newval = create_tmp_var_raw (type, NULL);
  rhsval = create_tmp_var_raw (type, NULL);
  label = create_artificial_label ();
  lhsaddr = save_expr (lhsaddr);

  x = build_fold_indirect_ref (lhsaddr);
  x = build2 (MODIFY_EXPR, void_type_node, oldval, x);
  add_stmt (x);

  if (TREE_CONSTANT (rhs))
    rhsval = rhs;
  else
    {
      x = build2 (MODIFY_EXPR, void_type_node, rhsval, rhs);
      add_stmt (x);
    }

  add_stmt (build_stmt (LABEL_EXPR, label));

  x = build2 (code, type, oldval, rhsval);
  x = build2 (MODIFY_EXPR, void_type_node, newval, x);
  add_stmt (x);

  y = tree_cons (NULL, newval, NULL);
  y = tree_cons (NULL, oldval, y);
  y = tree_cons (NULL, lhsaddr, y);
  decl = built_in_decls[BUILT_IN_VAL_COMPARE_AND_SWAP_N];
  x = resolve_overloaded_builtin (decl, y);
  x = build2 (MODIFY_EXPR, void_type_node, oldval, x);
  add_stmt (x);

  x = build2 (NE_EXPR, boolean_type_node, oldval, newval);
  y = build1 (GOTO_EXPR, void_type_node, label);
  x = build3 (COND_EXPR, void_type_node, x, y, NULL);
  add_stmt (x);
  return;

 invalid:
  error ("invalid expression for %<omp atomic%>");
  add_stmt (expr);
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
