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


/* A subroutine of c_split_parallel_clauses.  For each decl in *TP,
   replace it with the decl of the same name in the current binding.  */

static tree
relookup_decls (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		void *data ATTRIBUTE_UNUSED)
{
  tree old = *tp;

  if (DECL_P (old))
    {
      gcc_assert (DECL_NAME (old) != NULL);
      *tp = lookup_name (DECL_NAME (old));
    }

  return NULL_TREE;
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

	case OMP_CLAUSE_SCHEDULE:
	  /* We need this expression in the context of the block inside
	     to the parallel, rather than outside.  Adjust.  */
	  /* ??? Except when we start using GOMP_parallel_loop_*_start,
	     when we'll need it in the outside context.  Likely we should
	     provide both expressions.  */
	  walk_tree_without_duplicates (
		&OMP_CLAUSE_SCHEDULE_CHUNK_SIZE (TREE_VALUE (clauses)),
		relookup_decls, NULL);
	  /* FALLTHRU */

	case OMP_CLAUSE_ORDERED:
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
   expression and body of the loop).  DECL is the iteration variable.  */

tree
c_finish_omp_for (tree decl, tree init, tree cond, tree incr, tree body)
{
  source_locus locus = EXPR_LOCUS (init);
  bool fail = false;

  /* Validate the iteration variable.  */
  if (!INTEGRAL_TYPE_P (TREE_TYPE (decl)))
    {
      error ("%Hinvalid type for iteration variable %qE", locus, decl);
      fail = true;
    }
  if (TYPE_UNSIGNED (TREE_TYPE (decl)))
    warning (0, "%Hiteration variable %qE is unsigned", locus, decl);

  /* In the case of "for (int i = 0...)", init will be a decl.  It should
     have a DECL_INITIAL that we can turn into an assignment.  */
  if (init == decl)
    {
      init = DECL_INITIAL (decl);
      if (init == NULL)
	{
	  error ("%J%qE is not initialized", decl, decl);
	  init = integer_zero_node;
	  fail = true;
	}

      init = build_modify_expr (decl, NOP_EXPR, init);
    }
  gcc_assert (TREE_CODE (init) == MODIFY_EXPR);
  gcc_assert (TREE_OPERAND (init, 0) == decl);
  
  if (cond == NULL_TREE)
    {
      error ("%Hmissing controlling predicate", locus);
      fail = true;
    }
  else
    {
      bool cond_ok = false;

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
	  error ("%Hinvalid controlling predicate",
		 EXPR_HAS_LOCATION (cond) ? EXPR_LOCUS (cond) : locus);
	  fail = true;
	}
    }

  if (incr == NULL_TREE)
    {
      error ("%Hmissing increment expression", locus);
      fail = true;
    }
  else
    {
      bool incr_ok = false;
      enum tree_code code = TREE_CODE (incr);

      /* Check all the valid increment expressions: v++, v--, ++v, --v,
	 v = v + incr, v = incr + v and v = v - incr.  */
      if ((code == POSTINCREMENT_EXPR && TREE_OPERAND (incr, 0) == decl)
	  || (code == PREINCREMENT_EXPR && TREE_OPERAND (incr, 0) == decl)
	  || (code == POSTDECREMENT_EXPR && TREE_OPERAND (incr, 0) == decl)
	  || (code == PREDECREMENT_EXPR && TREE_OPERAND (incr, 0) == decl)
	  || (code == MODIFY_EXPR
	      && TREE_CODE (TREE_OPERAND (incr, 1)) == PLUS_EXPR
	      && (TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == decl
		  || TREE_OPERAND (TREE_OPERAND (incr, 1), 1) == decl))
	  || (code == MODIFY_EXPR
	      && TREE_CODE (TREE_OPERAND (incr, 1)) == MINUS_EXPR
	      && TREE_OPERAND (TREE_OPERAND (incr, 1), 0) == decl))
	incr_ok = true;

      if (!incr_ok)
	{
	  error ("%Hinvalid increment expression",
		 EXPR_HAS_LOCATION (incr) ? EXPR_LOCUS (incr) : locus);
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

      SET_EXPR_LOCUS (t, locus);
      return add_stmt (t);
    }
}

/* Instantiate new variables in the current scope to implement the private,
   shared, firstprivate, lastprivate, and reduction openmp clauses.  If 
   any initialization needed doing, it is added to INIT_SEQ.  Code for any
   lastprivate copies is added to LAST_SEQ.  Code for finalizing the 
   reductions is added to REDUC_SEQ.  */

void
c_finish_omp_bindings (tree omp_clauses, tree *init_seq, tree *last_seq,
		       tree *reduc_seq)
{
  tree clause, c, *plist, x;
  enum tree_code kind;
  bitmap_head spr_head, fp_head, lp_head, error_head;

  bitmap_obstack_initialize (NULL);
  bitmap_initialize (&spr_head, &bitmap_default_obstack);
  bitmap_initialize (&fp_head, &bitmap_default_obstack);
  bitmap_initialize (&lp_head, &bitmap_default_obstack);
  bitmap_initialize (&error_head, &bitmap_default_obstack);

  *init_seq = NULL;
  *last_seq = NULL;
  *reduc_seq = NULL;

  for (clause = omp_clauses; clause ; clause = TREE_CHAIN (clause))
    {
      c = TREE_VALUE (clause);
      kind = TREE_CODE (c);
      switch (kind)
	{
	case OMP_CLAUSE_SHARED:
	case OMP_CLAUSE_PRIVATE:
	case OMP_CLAUSE_FIRSTPRIVATE:
	case OMP_CLAUSE_LASTPRIVATE:
	case OMP_CLAUSE_REDUCTION:
	  break;
	default:
	  continue;
	}
	
      plist = &TREE_OPERAND (c, 0);
      while (*plist)
	{
	  tree old, new;
	  bool decl_ok = true;
	  bitmap update_map;

	  old = TREE_PURPOSE (*plist);

	  /* OpenMP 2.5, section 2.8.3: A list item that specifies a given
	     variable may not appear in more than one clause on the same
	     directive, except that a variable may be specified in both
	     firstprivate and lastprivate clauses.  */
	  if (bitmap_bit_p (&spr_head, DECL_UID (old)))
	    decl_ok = false;
	  if (kind != OMP_CLAUSE_LASTPRIVATE
	      && bitmap_bit_p (&fp_head, DECL_UID (old)))
	    decl_ok = false;
	  if (kind != OMP_CLAUSE_FIRSTPRIVATE
	      && bitmap_bit_p (&lp_head, DECL_UID (old)))
	    decl_ok = false;
	  if (!decl_ok)
	    {
	      if (!bitmap_bit_p (&error_head, DECL_UID (old)))
		{
		  error ("%qE listed more than once in sharing clauses", old);
		  bitmap_set_bit (&error_head, DECL_UID (old));
		}
	      *plist = TREE_CHAIN (*plist);
	      continue;
	    }

	  /* OpenMP 2.5 section 2.8.1.1: Variables with predetermined
	     sharing attributes may not be listed in data-sharing clauses.  */
	  if (c_omp_sharing_predetermined (old))
	    {
	      if (!bitmap_bit_p (&error_head, DECL_UID (old)))
		{
		  error ("%qE may not be listed in a sharing clause", old);
		  bitmap_set_bit (&error_head, DECL_UID (old));
		}
	      *plist = TREE_CHAIN (*plist);
	      continue;
	    }

	  new = c_omp_remap_decl (old, kind != OMP_CLAUSE_SHARED);
	  TREE_VALUE (*plist) = new;

	  /* Shared variables can be remapped to themselves.  When this
	     happens, remove the node from the list.  */
	  if (old == new)
	    *plist = TREE_CHAIN (*plist);
	  else
	    plist = &TREE_CHAIN (*plist);

	  switch (kind)
	    {
	    case OMP_CLAUSE_PRIVATE:
	      /* Nothing needs doing.  We've just created the new
		 variable.  */
	      update_map = &spr_head;
	      break;

	    case OMP_CLAUSE_SHARED:
	      /* We'll set DECL_VALUE_EXPR on NEW when lowering the
		 parallel.  But go ahead and mark OLD as addressable,
		 since we'll be taking its address across the parallel.  */
	      lang_hooks.mark_addressable (old);
	      update_map = &spr_head;
	      break;

	    case OMP_CLAUSE_FIRSTPRIVATE:
	      x = build_modify_expr (new, NOP_EXPR, old);
	      if (x != error_mark_node)
		append_to_statement_list (x, init_seq);

	      update_map = &fp_head;
	      break;

	    case OMP_CLAUSE_LASTPRIVATE:
	      x = build_modify_expr (old, NOP_EXPR, new);
	      if (x != error_mark_node)
		append_to_statement_list (x, last_seq);

	      update_map = &lp_head;
	      break;

	    case OMP_CLAUSE_REDUCTION:
	      switch (OMP_CLAUSE_REDUCTION_CODE (c))
		{
		case PLUS_EXPR:
		case MINUS_EXPR:
		case BIT_IOR_EXPR:
		case BIT_XOR_EXPR:
		case TRUTH_ORIF_EXPR:
		  x = integer_zero_node;
		  break;
		case MULT_EXPR:
		case TRUTH_ANDIF_EXPR:
		  x = integer_one_node;
		  break;
		case BIT_AND_EXPR:
		  x = integer_minus_one_node;
		  break;
		default:
		  gcc_unreachable ();
		}
	      x = build_modify_expr (new, NOP_EXPR, x);
	      if (x != error_mark_node)
		append_to_statement_list (x, init_seq);
	      x = build_modify_expr (old, OMP_CLAUSE_REDUCTION_CODE (c), new);
	      if (x != error_mark_node)
		append_to_statement_list (x, reduc_seq);

	      update_map = &spr_head;
	      break;

	    default:
	      gcc_unreachable ();
	    }

	  bitmap_set_bit (update_map, DECL_UID (old));
	}
    }

  bitmap_clear (&spr_head);
  bitmap_clear (&fp_head);
  bitmap_clear (&lp_head);
  bitmap_clear (&error_head);
  bitmap_obstack_initialize (NULL);
}
