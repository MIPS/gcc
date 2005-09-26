/* OpenMP directive translation -- generate GCC trees from gfc_code.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Jakub Jelinek <jakub@redhat.com>

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
#include "tree.h"
#include "tree-gimple.h"
#include "ggc.h"
#include "toplev.h"
#include "real.h"
#include "gfortran.h"
#include "trans.h"
#include "trans-stmt.h"
#include "trans-types.h"
#include "trans-array.h"
#include "trans-const.h"
#include "arith.h"

static tree
gfc_trans_omp_variable_list (gfc_namelist *namelist)
{
  tree list = NULL_TREE;
  for (; namelist != NULL; namelist = namelist->next)
    if (namelist->sym->attr.referenced)
      {
	tree t = gfc_get_symbol_decl (namelist->sym);
	if (t != error_mark_node)
	  list = tree_cons (NULL_TREE, t, list);
      }
  return list;
}

static tree
gfc_trans_omp_clauses (stmtblock_t *block, gfc_code *code)
{
  tree omp_clauses = NULL_TREE, chunk_size;
  int clause;
  enum tree_code clause_code;
  gfc_se se;

  if (code->ext.omp_clauses == NULL)
    return NULL_TREE;

  for (clause = 0; clause < OMP_LIST_NUM; clause++)
    {
      gfc_namelist *n = code->ext.omp_clauses->lists[clause];
      tree list;

      if (n == NULL)
	continue;
      if (clause >= OMP_LIST_REDUCTION_FIRST
	  && clause <= OMP_LIST_REDUCTION_LAST)
	{
	  /* FIXME: reductions not handled yet.  */
	  continue;
	}
      switch (clause)
	{
	case OMP_LIST_PRIVATE:
	  clause_code = OMP_CLAUSE_PRIVATE;
	  goto add_clause;
	case OMP_LIST_SHARED:
	  clause_code = OMP_CLAUSE_SHARED;
	  goto add_clause;
	case OMP_LIST_FIRSTPRIVATE:
	  clause_code = OMP_CLAUSE_FIRSTPRIVATE;
	  goto add_clause;
	case OMP_LIST_LASTPRIVATE:
	  clause_code = OMP_CLAUSE_LASTPRIVATE;
	  goto add_clause;
	case OMP_LIST_COPYIN:
	  clause_code = OMP_CLAUSE_COPYIN;
	  goto add_clause;
	case OMP_LIST_COPYPRIVATE:
	  clause_code = OMP_CLAUSE_COPYPRIVATE;
	  /* FALLTHROUGH */
	add_clause:
	  list = gfc_trans_omp_variable_list (n);
	  if (list != NULL_TREE)
	    {
	      list = build1 (clause_code, NULL_TREE, list);
	      omp_clauses = tree_cons (NULL_TREE, list, omp_clauses);
	    }
	  break;
	default:
	  break;
	}
    }

  if (code->ext.omp_clauses->if_expr)
    {
      tree if_var;
      gfc_init_se (&se, NULL);
      gfc_conv_expr (&se, code->ext.omp_clauses->if_expr);
      gfc_add_block_to_block (block, &se.pre);
      if (TREE_CONSTANT (se.expr))
	if_var = se.expr;
      else
	{
	  if_var = gfc_create_var (boolean_type_node, "if");
	  gfc_add_modify_expr (block, if_var,
			       convert (boolean_type_node, se.expr));
	}
      gfc_add_block_to_block (block, &se.post);
      omp_clauses = tree_cons (NULL_TREE,
			       build1 (OMP_CLAUSE_IF, NULL_TREE, if_var),
			       omp_clauses);
    }

  if (code->ext.omp_clauses->num_threads)
    {
      tree num_threads;
      gfc_init_se (&se, NULL);
      gfc_conv_expr (&se, code->ext.omp_clauses->num_threads);
      gfc_add_block_to_block (block, &se.pre);
      if (TREE_CONSTANT (se.expr))
	num_threads = se.expr;
      else
	{
	  num_threads = gfc_create_var (integer_type_node, "num_threads");
	  gfc_add_modify_expr (block, num_threads,
			       convert (integer_type_node, se.expr));
	}
      gfc_add_block_to_block (block, &se.post);
      omp_clauses = tree_cons (NULL_TREE,
			       build1 (OMP_CLAUSE_NUM_THREADS, NULL_TREE,
				       num_threads),
			       omp_clauses);
    }

  chunk_size = NULL_TREE;
  if (code->ext.omp_clauses->chunk_size)
    {
      gfc_init_se (&se, NULL);
      gfc_conv_expr (&se, code->ext.omp_clauses->chunk_size);
      gfc_add_block_to_block (block, &se.pre);
      if (TREE_CONSTANT (se.expr))
	chunk_size = se.expr;
      else
	{
	  chunk_size = gfc_create_var (integer_type_node, "chunk_size");
	  gfc_add_modify_expr (block, chunk_size,
			       convert (integer_type_node, se.expr));
	}
      gfc_add_block_to_block (block, &se.post);
    }

  if (code->ext.omp_clauses->sched_kind != OMP_SCHED_NONE)
    {
      tree c = build1 (OMP_CLAUSE_SCHEDULE, NULL_TREE, chunk_size);
      switch (code->ext.omp_clauses->sched_kind)
	{
	case OMP_SCHED_STATIC:
	  OMP_CLAUSE_SCHEDULE_KIND (c) = OMP_CLAUSE_SCHEDULE_STATIC;
	  break;
	case OMP_SCHED_DYNAMIC:
	  OMP_CLAUSE_SCHEDULE_KIND (c) = OMP_CLAUSE_SCHEDULE_DYNAMIC;
	  break;
	case OMP_SCHED_GUIDED:
	  OMP_CLAUSE_SCHEDULE_KIND (c) = OMP_CLAUSE_SCHEDULE_GUIDED;
	  break;
	case OMP_SCHED_RUNTIME:
	  OMP_CLAUSE_SCHEDULE_KIND (c) = OMP_CLAUSE_SCHEDULE_RUNTIME;
	  break;
	default:
	  gcc_unreachable ();
	}
    }

  if (code->ext.omp_clauses->nowait)
    omp_clauses = tree_cons (NULL_TREE, build0 (OMP_CLAUSE_NOWAIT, NULL_TREE),
			     omp_clauses);

  if (code->ext.omp_clauses->ordered)
    omp_clauses = tree_cons (NULL_TREE, build0 (OMP_CLAUSE_ORDERED, NULL_TREE),
			     omp_clauses);

  return omp_clauses;
}

static tree
gfc_trans_omp_atomic (gfc_code *code)
{
  gfc_se lse;
  gfc_se rse;
  gfc_expr *expr2, *e;
  gfc_symbol *var;
  stmtblock_t block;
  int size;
  tree lhsaddr, type, itype, x, oldival, oldval, newival, newval;
  tree lhsiaddr, rhsval, label;
  enum tree_code op = ERROR_MARK;
  bool var_on_left = false;
  enum built_in_function bcode;

  code = code->block->next;
  gcc_assert (code->op == EXEC_ASSIGN);
  gcc_assert (code->next == NULL);
  var = code->expr->symtree->n.sym;

  gfc_init_se (&lse, NULL);
  gfc_init_se (&rse, NULL);
  gfc_start_block (&block);

  gfc_conv_expr (&lse, code->expr);
  gfc_add_block_to_block (&block, &lse.pre);
  type = TREE_TYPE (lse.expr);
  lhsaddr = gfc_build_addr_expr (NULL, lse.expr);

  expr2 = code->expr2;
  if (expr2->expr_type == EXPR_FUNCTION
      && expr2->value.function.isym->generic_id == GFC_ISYM_CONVERSION)
    expr2 = expr2->value.function.actual->expr;

  if (expr2->expr_type == EXPR_OP)
    {
      gfc_expr *e;
      switch (expr2->value.op.operator)
	{
	case INTRINSIC_PLUS:
	  op = PLUS_EXPR;
	  break;
	case INTRINSIC_TIMES:
	  op = MULT_EXPR;
	  break;
	case INTRINSIC_MINUS:
	  op = MINUS_EXPR;
	  break;
	case INTRINSIC_DIVIDE:
	  if (expr2->ts.type == BT_INTEGER)
	    op = TRUNC_DIV_EXPR;
	  else
	    op = RDIV_EXPR;
	  break;
	case INTRINSIC_AND:
	  op = TRUTH_ANDIF_EXPR;
	  break;
	case INTRINSIC_OR:
	  op = TRUTH_ORIF_EXPR;
	  break;
	case INTRINSIC_EQV:
	  op = EQ_EXPR;
	  break;
	case INTRINSIC_NEQV:
	  op = NE_EXPR;
	  break;
	default:
	  gcc_unreachable ();
	}
      e = expr2->value.op.op1;
      if (e->expr_type == EXPR_FUNCTION
	  && e->value.function.isym->generic_id == GFC_ISYM_CONVERSION)
	e = e->value.function.actual->expr;
      if (e->expr_type == EXPR_VARIABLE
	  && e->symtree != NULL
	  && e->symtree->n.sym == var)
	{
	  expr2 = expr2->value.op.op2;
	  var_on_left = true;
	}
      else
	{
	  e = expr2->value.op.op2;
	  if (e->expr_type == EXPR_FUNCTION
	      && e->value.function.isym->generic_id == GFC_ISYM_CONVERSION)
	    e = e->value.function.actual->expr;
	  gcc_assert (e->expr_type == EXPR_VARIABLE
		      && e->symtree != NULL
		      && e->symtree->n.sym == var);
	  expr2 = expr2->value.op.op1;
	  var_on_left = false;
	}
      gfc_conv_expr (&rse, expr2);
      gfc_add_block_to_block (&block, &rse.pre);
    }
  else
    {
      gcc_assert (expr2->expr_type == EXPR_FUNCTION);
      switch (expr2->value.function.isym->generic_id)
	{
	case GFC_ISYM_MIN:
	  op = MIN_EXPR;
	  break;
	case GFC_ISYM_MAX:
	  op = MAX_EXPR;
	  break;
	case GFC_ISYM_IAND:
	  op = BIT_AND_EXPR;
	  break;
	case GFC_ISYM_IOR:
	  op = BIT_IOR_EXPR;
	  break;
	case GFC_ISYM_IEOR:
	  op = BIT_XOR_EXPR;
	  break;
	default:
	  gcc_unreachable ();
	}
      e = expr2->value.function.actual->expr;
      gcc_assert (e->expr_type == EXPR_VARIABLE
		  && e->symtree != NULL
		  && e->symtree->n.sym == var);

      gfc_conv_expr (&rse, expr2->value.function.actual->next->expr);
      gfc_add_block_to_block (&block, &rse.pre);
      if (expr2->value.function.actual->next->next != NULL)
	{
	  tree accum = gfc_create_var (TREE_TYPE (rse.expr), NULL);
	  gfc_actual_arglist *arg;

	  gfc_add_modify_expr (&block, accum, rse.expr);
	  for (arg = expr2->value.function.actual->next->next; arg;
	       arg = arg->next)
	    {
	      gfc_init_block (&rse.pre);
	      gfc_conv_expr (&rse, arg->expr);
	      gfc_add_block_to_block (&block, &rse.pre);
	      x = fold_build2 (op, TREE_TYPE (accum), accum, rse.expr);
	      gfc_add_modify_expr (&block, accum, x);
	    }

	  rse.expr = accum;
	}

      expr2 = expr2->value.function.actual->next->expr;
    }

  /* When possible, use specialized atomic update functions.  */
  if (code->expr->ts.type == BT_INTEGER && expr2->ts.type == BT_INTEGER)
    {
      switch (op)
	{
	case PLUS_EXPR:
	  bcode = BUILT_IN_FETCH_AND_ADD_N;
	  goto maybe_do_fetch_op;
	case MINUS_EXPR:
	  if (var_on_left)
	    {
	      bcode = BUILT_IN_FETCH_AND_SUB_N;
	      goto maybe_do_fetch_op;
	    }
	  break;
	case BIT_AND_EXPR:
	  bcode = BUILT_IN_FETCH_AND_AND_N;
	  goto maybe_do_fetch_op;
	case BIT_IOR_EXPR:
	  bcode = BUILT_IN_FETCH_AND_OR_N;
	  goto maybe_do_fetch_op;
	case BIT_XOR_EXPR:
	  bcode = BUILT_IN_FETCH_AND_XOR_N;
	  /* FALLTHROUGH */
	maybe_do_fetch_op:
	  switch (code->expr->ts.kind)
	    {
	    case 1:
	      bcode += 1;
	      goto do_fetch_op;
	    case 2:
	      bcode += 2;
	      goto do_fetch_op;
	    case 4:
	      bcode += 3;
	      goto do_fetch_op;
	    case 8:
	      bcode += 4;
	      /* FALLTHROUGH */
	    do_fetch_op:
	      x = gfc_chainon_list (NULL_TREE, lhsaddr);
	      x = gfc_chainon_list (x, convert (type, rse.expr));
	      x = gfc_build_function_call (built_in_decls[bcode], x);
	      gfc_add_expr_to_block (&block, x);
	      goto finish;
	    default:
	      break;
	    }
	  break;
	default:
	  break;
	}
    }

  /* In these cases, we don't have specialized __sync builtins,
     so we need to implement a compare and swap loop.  */
  itype = NULL;
  size = GET_MODE_PRECISION (TYPE_MODE (type));
  if (code->expr->ts.type != BT_INTEGER && code->expr->ts.type != BT_LOGICAL)
    itype = gfc_type_for_size (size, true);

  oldival = oldval = gfc_create_var (type, "oldval");
  newival = newval = gfc_create_var (type, "newval");
  lhsiaddr = lhsaddr = save_expr (lhsaddr);
  if (itype)
    {
      oldival = gfc_create_var (itype, "oldival");
      newival = gfc_create_var (itype, "newival");
      lhsiaddr = convert (build_pointer_type (itype), lhsaddr);
    }

  if (TREE_CONSTANT (rse.expr))
    rhsval = rse.expr;
  else
    rhsval = gfc_create_var (TREE_TYPE (rse.expr), "rhsval");
  label = gfc_build_label_decl (NULL_TREE);
  TREE_USED (label) = 1;

  gfc_add_modify_expr (&block, oldval, gfc_build_indirect_ref (lhsaddr));

  if (itype)
    gfc_add_modify_expr (&block, oldival,
			 build1 (VIEW_CONVERT_EXPR, itype, oldval));

  if (!TREE_CONSTANT (rse.expr))
    gfc_add_modify_expr (&block, rhsval, rse.expr);

  gfc_add_expr_to_block (&block, build1_v (LABEL_EXPR, label));

  x = convert (TREE_TYPE (rhsval), oldval);

  if (var_on_left)
    x = fold_build2 (op, TREE_TYPE (rhsval), x, rhsval);
  else
    x = fold_build2 (op, TREE_TYPE (rhsval), rhsval, x);

  if (TREE_CODE (TREE_TYPE (rhsval)) == COMPLEX_TYPE
      && TREE_CODE (type) != COMPLEX_TYPE)
    x = build1 (REALPART_EXPR, TREE_TYPE (TREE_TYPE (rhsval)), x);

  gfc_add_modify_expr (&block, newval, convert (type, x));

  if (itype)
    gfc_add_modify_expr (&block, newival,
			 build1 (VIEW_CONVERT_EXPR, itype, newval));

  bcode = BUILT_IN_VAL_COMPARE_AND_SWAP_N;
  switch (size)
    {
    case 8:
      bcode += 1;
      break;
    case 16:
      bcode += 2;
      break;
    case 32:
      bcode += 3;
      break;
    case 64:
      bcode += 4;
      break;
    default:
      gcc_unreachable ();
    }
  x = gfc_chainon_list (NULL_TREE, lhsiaddr);
  x = gfc_chainon_list (x, oldival);
  x = gfc_chainon_list (x, newival);
  x = gfc_build_function_call (built_in_decls[bcode], x);
  gfc_add_modify_expr (&block, oldival, convert (TREE_TYPE (oldival), x));

  if (itype)
    gfc_add_modify_expr (&block, oldval,
			 build1 (VIEW_CONVERT_EXPR, type, oldival));

  x = build2 (NE_EXPR, boolean_type_node, oldival, newival);
  x = build3_v (COND_EXPR, x,
		build1 (GOTO_EXPR, void_type_node, label),
		build_empty_stmt ());
  gfc_add_expr_to_block (&block, x);

finish:
  gfc_add_block_to_block (&block, &lse.pre);
  gfc_add_block_to_block (&block, &rse.pre);

  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_barrier (void)
{
  tree decl = built_in_decls [BUILT_IN_GOMP_BARRIER];
  return gfc_build_function_call (decl, NULL);
}

static tree
gfc_trans_omp_critical (gfc_code *code)
{
  tree name = NULL_TREE, stmt;
  if (code->ext.omp_name != NULL)
    name = get_identifier (code->ext.omp_name);
  stmt = gfc_trans_code (code->block->next);
  return build2 (OMP_CRITICAL, void_type_node, name, stmt);
}

static tree
gfc_trans_omp_flush (void)
{
  tree decl = built_in_decls [BUILT_IN_SYNCHRONIZE];
  return gfc_build_function_call (decl, NULL);
}

static tree
gfc_trans_omp_master (gfc_code *code)
{
  tree decl = built_in_decls [BUILT_IN_OMP_GET_THREAD_NUM], x, stmt;
  x = gfc_build_function_call (decl, NULL);
  stmt = gfc_trans_code (code->block->next);
  if (stmt == NULL_TREE || IS_EMPTY_STMT (stmt))
    return stmt;
  x = build2 (EQ_EXPR, boolean_type_node, x, integer_zero_node);
  x = build3_v (COND_EXPR, x, stmt, build_empty_stmt ());
  return x;
}

static tree
gfc_trans_omp_ordered (gfc_code *code)
{
  stmtblock_t block;
  tree decl = built_in_decls [BUILT_IN_GOMP_ORDERED_START];
  gfc_init_block (&block);
  gfc_add_expr_to_block (&block, gfc_build_function_call (decl, NULL));
  gfc_add_expr_to_block (&block, gfc_trans_code (code->block->next));
  decl = built_in_decls [BUILT_IN_GOMP_ORDERED_END];
  gfc_add_expr_to_block (&block, gfc_build_function_call (decl, NULL));
  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_parallel (gfc_code *code)
{
  stmtblock_t block;
  tree stmt, omp_clauses;

  gfc_start_block (&block);
  omp_clauses = gfc_trans_omp_clauses (&block, code);
  stmt = gfc_trans_code (code->block->next);
  if (0)
    stmt = build2 (OMP_PARALLEL, void_type_node, omp_clauses, stmt);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

tree
gfc_trans_omp_directive (gfc_code *code)
{
  switch (code->op)
    {
    case EXEC_OMP_ATOMIC:
      return gfc_trans_omp_atomic (code);
    case EXEC_OMP_BARRIER:
      return gfc_trans_omp_barrier ();
    case EXEC_OMP_CRITICAL:
      return gfc_trans_omp_critical (code);
    case EXEC_OMP_FLUSH:
      return gfc_trans_omp_flush ();
    case EXEC_OMP_MASTER:
      return gfc_trans_omp_master (code);
    case EXEC_OMP_ORDERED:
      return gfc_trans_omp_ordered (code);
    case EXEC_OMP_PARALLEL:
      return gfc_trans_omp_parallel (code);
    default:
      break;
    }

  /* Temporary hack.  */
  if (code->op == EXEC_OMP_SECTIONS
      || code->op == EXEC_OMP_PARALLEL_SECTIONS)
    {
      stmtblock_t block;
      gfc_start_block (&block);
      for (code = code->block; code; code = code->block)
	{
	  tree stmt = gfc_trans_code (code->next);
	  if (stmt != NULL_TREE && ! IS_EMPTY_STMT (stmt))
	    gfc_add_expr_to_block (&block, stmt);
	}
      return gfc_finish_block (&block);
    }
  return gfc_trans_code (code->block->next);
}
