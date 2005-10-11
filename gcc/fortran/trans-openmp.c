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


static inline tree
gfc_trans_add_clause (tree node, tree tail)
{
  OMP_CLAUSE_CHAIN (node) = tail;
  return node;
}

static tree
gfc_trans_omp_variable_list (enum tree_code code, gfc_namelist *namelist,
			     tree list)
{
  for (; namelist != NULL; namelist = namelist->next)
    if (namelist->sym->attr.referenced)
      {
	tree t = gfc_get_symbol_decl (namelist->sym);
	if (t != error_mark_node)
	  {
	    tree node = make_node (code);
	    OMP_CLAUSE_DECL (node) = t;
	    list = gfc_trans_add_clause (node, list);
	  }
      }
  return list;
}

static tree
gfc_trans_omp_clauses (stmtblock_t *block, gfc_omp_clauses *clauses)
{
  tree omp_clauses = NULL_TREE, chunk_size, c, old_clauses;
  int list;
  enum tree_code clause_code;
  gfc_se se;

  if (clauses == NULL)
    return NULL_TREE;

  for (list = 0; list < OMP_LIST_NUM; list++)
    {
      gfc_namelist *n = clauses->lists[list];

      if (n == NULL)
	continue;
      if (list >= OMP_LIST_REDUCTION_FIRST
	  && list <= OMP_LIST_REDUCTION_LAST)
	{
	  enum tree_code reduction_code;
	  switch (list)
	    {
	    case OMP_LIST_PLUS:
	      reduction_code = PLUS_EXPR;
	      break;
	    case OMP_LIST_MULT:
	      reduction_code = MULT_EXPR;
	      break;
	    case OMP_LIST_SUB:
	      reduction_code = MINUS_EXPR;
	      break;
	    case OMP_LIST_AND:
	      reduction_code = TRUTH_ANDIF_EXPR;
	      break;
	    case OMP_LIST_OR:
	      reduction_code = TRUTH_ORIF_EXPR;
	      break;
	    case OMP_LIST_EQV:
	      reduction_code = EQ_EXPR;
	      break;
	    case OMP_LIST_NEQV:
	      reduction_code = NE_EXPR;
	      break;
	    case OMP_LIST_MAX:
	      reduction_code = MAX_EXPR;
	      break;
	    case OMP_LIST_MIN:
	      reduction_code = MIN_EXPR;
	      break;
	    case OMP_LIST_IAND:
	      reduction_code = BIT_AND_EXPR;
	      break;
	    case OMP_LIST_IOR:
	      reduction_code = BIT_IOR_EXPR;
	      break;
	    case OMP_LIST_IEOR:
	      reduction_code = BIT_XOR_EXPR;
	      break;
	    default:
	      gcc_unreachable ();
	    }
	  old_clauses = omp_clauses;
	  omp_clauses
	    = gfc_trans_omp_variable_list (OMP_CLAUSE_REDUCTION, n,
					   omp_clauses);
	  for (c = omp_clauses; c != old_clauses; c = OMP_CLAUSE_CHAIN (c))
	    OMP_CLAUSE_REDUCTION_CODE (c) = reduction_code;
	  continue;
	}
      switch (list)
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
	  omp_clauses
	    = gfc_trans_omp_variable_list (clause_code, n, omp_clauses);
	  break;
	default:
	  break;
	}
    }

  if (clauses->if_expr)
    {
      tree if_var;

      gfc_init_se (&se, NULL);
      gfc_conv_expr (&se, clauses->if_expr);
      gfc_add_block_to_block (block, &se.pre);
      if_var = gfc_evaluate_now (se.expr, block);
      gfc_add_block_to_block (block, &se.post);

      c = make_node (OMP_CLAUSE_IF);
      OMP_CLAUSE_IF_EXPR (c) = if_var;
      omp_clauses = gfc_trans_add_clause (c, omp_clauses);
    }

  if (clauses->num_threads)
    {
      tree num_threads;

      gfc_init_se (&se, NULL);
      gfc_conv_expr (&se, clauses->num_threads);
      gfc_add_block_to_block (block, &se.pre);
      num_threads = gfc_evaluate_now (se.expr, block);
      gfc_add_block_to_block (block, &se.post);

      c = make_node (OMP_CLAUSE_NUM_THREADS);
      OMP_CLAUSE_NUM_THREADS_EXPR (c) = num_threads;
      omp_clauses = gfc_trans_add_clause (c, omp_clauses);
    }

  chunk_size = NULL_TREE;
  if (clauses->chunk_size)
    {
      gfc_init_se (&se, NULL);
      gfc_conv_expr (&se, clauses->chunk_size);
      gfc_add_block_to_block (block, &se.pre);
      chunk_size = gfc_evaluate_now (se.expr, block);
      gfc_add_block_to_block (block, &se.post);
    }

  if (clauses->sched_kind != OMP_SCHED_NONE)
    {
      c = make_node (OMP_CLAUSE_SCHEDULE);
      OMP_CLAUSE_SCHEDULE_CHUNK_EXPR (c) = chunk_size;
      switch (clauses->sched_kind)
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
      omp_clauses = gfc_trans_add_clause (c, omp_clauses);
    }

  if (clauses->default_sharing != OMP_DEFAULT_UNKNOWN)
    {
      c = make_node (OMP_CLAUSE_DEFAULT);
      switch (clauses->default_sharing)
	{
	case OMP_DEFAULT_NONE:
	  OMP_CLAUSE_DEFAULT_KIND (c) = OMP_CLAUSE_DEFAULT_NONE;
	  break;
	case OMP_DEFAULT_SHARED:
	  OMP_CLAUSE_DEFAULT_KIND (c) = OMP_CLAUSE_DEFAULT_SHARED;
	  break;
	case OMP_DEFAULT_PRIVATE:
	  OMP_CLAUSE_DEFAULT_KIND (c) = OMP_CLAUSE_DEFAULT_PRIVATE;
	  break;
	default:
	  gcc_unreachable ();
	}
      omp_clauses = gfc_trans_add_clause (c, omp_clauses);
    }

  if (clauses->nowait)
    {
      c = make_node (OMP_CLAUSE_NOWAIT);
      omp_clauses = gfc_trans_add_clause (c, omp_clauses);
    }

  if (clauses->ordered)
    {
      c = make_node (OMP_CLAUSE_ORDERED);
      omp_clauses = gfc_trans_add_clause (c, omp_clauses);
    }

  return omp_clauses;
}

/* Like gfc_trans_code, but force creation of a BIND_EXPR around it.  */

static tree
gfc_trans_omp_code (gfc_code *code, bool force_empty)
{
  tree stmt;

  pushlevel (0);
  stmt = gfc_trans_code (code);
  if (TREE_CODE (stmt) != BIND_EXPR)
    {
      if (!IS_EMPTY_STMT (stmt) || force_empty)
	{
	  tree block = poplevel (1, 0, 0);
	  stmt = build3_v (BIND_EXPR, NULL, stmt, block);
	}
      else
	poplevel (0, 0, 0);
    }
  else
    poplevel (0, 0, 0);
  return stmt;
}


static tree gfc_trans_omp_sections (gfc_code *, gfc_omp_clauses *);
static tree gfc_trans_omp_workshare (gfc_code *, gfc_omp_clauses *);

static tree
gfc_trans_omp_atomic (gfc_code *code)
{
  gfc_se lse;
  gfc_se rse;
  gfc_expr *expr2, *e;
  gfc_symbol *var;
  stmtblock_t block;
  tree lhsaddr, type, rhs, x;
  enum tree_code op = ERROR_MARK;
  bool var_on_left = false;

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

  lhsaddr = save_expr (lhsaddr);
  rhs = gfc_evaluate_now (rse.expr, &block);
  x = convert (TREE_TYPE (rhs), gfc_build_indirect_ref (lhsaddr));

  if (var_on_left)
    x = fold_build2 (op, TREE_TYPE (rhs), x, rhs);
  else
    x = fold_build2 (op, TREE_TYPE (rhs), rhs, x);

  if (TREE_CODE (TREE_TYPE (rhs)) == COMPLEX_TYPE
      && TREE_CODE (type) != COMPLEX_TYPE)
    x = build1 (REALPART_EXPR, TREE_TYPE (TREE_TYPE (rhs)), x);

  x = build2_v (OMP_ATOMIC, lhsaddr, convert (type, x));
  gfc_add_expr_to_block (&block, x);

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
  return build2_v (OMP_CRITICAL, name, stmt);
}

static tree
gfc_trans_omp_do (gfc_code *code, gfc_omp_clauses *clauses)
{
  gfc_se se;
  tree dovar, stmt, from, to, step, type, init, cond, incr;
  tree count = NULL_TREE, cycle_label, tmp, omp_clauses;
  stmtblock_t block;
  stmtblock_t body;
  int simple = 0;
  bool dovar_found = false;

  code = code->block->next;
  gcc_assert (code->op == EXEC_DO);

  gfc_start_block (&block);

  omp_clauses = gfc_trans_omp_clauses (&block, clauses);
  if (clauses)
    {
      gfc_namelist *n;
      for (n = clauses->lists[OMP_LIST_LASTPRIVATE]; n != NULL; n = n->next)
	if (code->ext.iterator->var->symtree->n.sym == n->sym)
	  break;
      if (n == NULL)
	for (n = clauses->lists[OMP_LIST_PRIVATE]; n != NULL; n = n->next)
	  if (code->ext.iterator->var->symtree->n.sym == n->sym)
	    break;
      if (n != NULL)
	dovar_found = true;
    }

  /* Evaluate all the expressions in the iterator.  */
  gfc_init_se (&se, NULL);
  gfc_conv_expr_lhs (&se, code->ext.iterator->var);
  gfc_add_block_to_block (&block, &se.pre);
  dovar = se.expr;
  type = TREE_TYPE (dovar);
  gcc_assert (TREE_CODE (type) == INTEGER_TYPE);

  gfc_init_se (&se, NULL);
  gfc_conv_expr_val (&se, code->ext.iterator->start);
  gfc_add_block_to_block (&block, &se.pre);
  from = gfc_evaluate_now (se.expr, &block);

  gfc_init_se (&se, NULL);
  gfc_conv_expr_val (&se, code->ext.iterator->end);
  gfc_add_block_to_block (&block, &se.pre);
  to = gfc_evaluate_now (se.expr, &block);

  gfc_init_se (&se, NULL);
  gfc_conv_expr_val (&se, code->ext.iterator->step);
  gfc_add_block_to_block (&block, &se.pre);
  step = gfc_evaluate_now (se.expr, &block);

  /* Special case simple loops.  */
  if (integer_onep (step))
    simple = 1;
  else if (tree_int_cst_equal (step, integer_minus_one_node))
    simple = -1;

  /* Loop body.  */
  gfc_start_block (&body);

  if (simple)
    {
      init = build2_v (MODIFY_EXPR, dovar, from);
      cond = build2 (simple > 0 ? LE_EXPR : GE_EXPR, boolean_type_node,
		     dovar, to);
      incr = fold_build2 (PLUS_EXPR, type, dovar, step);
      incr = fold_build2 (MODIFY_EXPR, type, dovar, incr);
    }
  else
    {
      /* STEP is not 1 or -1.  Use:
	 for (count = 0; count < (to + step - from) / step; count++)
	   {
	     dovar = from + count * step;
	     body;
	   cycle_label:;
	   }  */
      tmp = fold_build2 (MINUS_EXPR, type, step, from);
      tmp = fold_build2 (PLUS_EXPR, type, to, tmp);
      tmp = fold_build2 (TRUNC_DIV_EXPR, type, tmp, step);
      tmp = gfc_evaluate_now (tmp, &block);
      count = gfc_create_var (type, "count");
      init = build2_v (MODIFY_EXPR, count, build_int_cst (type, 0));
      cond = build2 (GT_EXPR, boolean_type_node, count, tmp);
      incr = fold_build2 (PLUS_EXPR, type, count, build_int_cst (type, 1));
      incr = fold_build2 (MODIFY_EXPR, type, count, incr);

      /* Initialize DOVAR.  */
      tmp = fold_build2 (MULT_EXPR, type, count, incr);
      tmp = build2 (PLUS_EXPR, type, from, tmp);
      gfc_add_modify_expr (&body, dovar, tmp);
    }

  if (!dovar_found)
    {
      tmp = make_node (OMP_CLAUSE_PRIVATE);
      OMP_CLAUSE_DECL (tmp) = dovar;
      omp_clauses = gfc_trans_add_clause (tmp, omp_clauses);
    }
  if (!simple)
    {
      tmp = make_node (OMP_CLAUSE_PRIVATE);
      OMP_CLAUSE_DECL (tmp) = count;
      omp_clauses = gfc_trans_add_clause (tmp, omp_clauses);
    }

  /* Cycle statement is implemented with a goto.  Exit statement must not be
     present for this loop.  */
  cycle_label = gfc_build_label_decl (NULL_TREE);

  /* Put these labels where they can be found later. We put the
     labels in a TREE_LIST node (because TREE_CHAIN is already
     used). cycle_label goes in TREE_PURPOSE (backend_decl), exit
     label in TREE_VALUE (backend_decl).  */

  code->block->backend_decl = tree_cons (cycle_label, NULL, NULL);

  /* Main loop body.  */
  tmp = gfc_trans_omp_code (code->block->next, true);
  gfc_add_expr_to_block (&body, tmp);

  /* Label for cycle statements (if needed).  */
  if (TREE_USED (cycle_label))
    {
      tmp = build1_v (LABEL_EXPR, cycle_label);
      gfc_add_expr_to_block (&body, tmp);
    }

  /* End of loop body.  */
  stmt = gfc_finish_block (&body);

  stmt = build (OMP_FOR, void_type_node, omp_clauses, init, cond, incr, stmt);
  gfc_add_expr_to_block (&block, stmt);

  return gfc_finish_block (&block);
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
  if (IS_EMPTY_STMT (stmt))
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
  omp_clauses = gfc_trans_omp_clauses (&block, code->ext.omp_clauses);
  stmt = gfc_trans_omp_code (code->block->next, true);
  stmt = build2_v (OMP_PARALLEL, omp_clauses, stmt);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_parallel_do (gfc_code *code)
{
  stmtblock_t block;
  gfc_omp_clauses parallel_clauses, do_clauses;
  tree stmt, omp_clauses = NULL_TREE;

  gfc_start_block (&block);

  memset (&do_clauses, 0, sizeof (do_clauses));
  if (code->ext.omp_clauses != NULL)
    {
      memcpy (&parallel_clauses, code->ext.omp_clauses,
	      sizeof (parallel_clauses));
      do_clauses.sched_kind = parallel_clauses.sched_kind;
      do_clauses.chunk_size = parallel_clauses.chunk_size;
      do_clauses.ordered = parallel_clauses.ordered;
      parallel_clauses.sched_kind = OMP_SCHED_NONE;
      parallel_clauses.chunk_size = NULL;
      parallel_clauses.ordered = false;
      omp_clauses = gfc_trans_omp_clauses (&block, &parallel_clauses);
    }
  do_clauses.nowait = true;
  pushlevel (0);
  stmt = gfc_trans_omp_do (code, &do_clauses);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0, 0));
  else
    poplevel (0, 0, 0);
  stmt = build2_v (OMP_PARALLEL, omp_clauses, stmt);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_parallel_sections (gfc_code *code)
{
  stmtblock_t block;
  gfc_omp_clauses section_clauses;
  tree stmt, omp_clauses;

  memset (&section_clauses, 0, sizeof (section_clauses));
  section_clauses.nowait = true;

  gfc_start_block (&block);
  omp_clauses = gfc_trans_omp_clauses (&block, code->ext.omp_clauses);
  pushlevel (0);
  stmt = gfc_trans_omp_sections (code, &section_clauses);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0, 0));
  else
    poplevel (0, 0, 0);
  stmt = build2_v (OMP_PARALLEL, omp_clauses, stmt);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_parallel_workshare (gfc_code *code)
{
  stmtblock_t block;
  gfc_omp_clauses workshare_clauses;
  tree stmt, omp_clauses;

  memset (&workshare_clauses, 0, sizeof (workshare_clauses));
  workshare_clauses.nowait = true;

  gfc_start_block (&block);
  omp_clauses = gfc_trans_omp_clauses (&block, code->ext.omp_clauses);
  pushlevel (0);
  stmt = gfc_trans_omp_workshare (code, &workshare_clauses);
  if (TREE_CODE (stmt) != BIND_EXPR)
    stmt = build3_v (BIND_EXPR, NULL, stmt, poplevel (1, 0, 0));
  else
    poplevel (0, 0, 0);
  stmt = build2_v (OMP_PARALLEL, omp_clauses, stmt);
  gfc_add_expr_to_block (&block, stmt);
  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_sections (gfc_code *code, gfc_omp_clauses *clauses)
{
  stmtblock_t block, body;
  tree omp_clauses, stmt;
  bool has_lastprivate = clauses->lists[OMP_LIST_LASTPRIVATE] != NULL;

  gfc_start_block (&block);

  omp_clauses = gfc_trans_omp_clauses (&block, clauses);

  gfc_init_block (&body);
  for (code = code->block; code; code = code->block)
    {
      /* Last section is special because of lastprivate, so even if it
	 is empty, chain it in.  */
      stmt = gfc_trans_omp_code (code->next,
				 has_lastprivate && code->block == NULL);
      if (! IS_EMPTY_STMT (stmt))
	{
	  stmt = build1_v (OMP_SECTION, stmt);
	  gfc_add_expr_to_block (&body, stmt);
	}
    }
  stmt = gfc_finish_block (&body);

  stmt = build2_v (OMP_SECTIONS, omp_clauses, stmt);
  gfc_add_expr_to_block (&block, stmt);

  return gfc_finish_block (&block);
}

static tree
gfc_trans_omp_single (gfc_code *code, gfc_omp_clauses *clauses)
{
  tree omp_clauses = gfc_trans_omp_clauses (NULL, clauses);
  tree stmt = gfc_trans_omp_code (code->block->next, true);
  stmt = build2_v (OMP_SINGLE, omp_clauses, stmt);
  return stmt;
}

static tree
gfc_trans_omp_workshare (gfc_code *code, gfc_omp_clauses *clauses)
{
  /* XXX */
  return gfc_trans_omp_single (code, clauses);
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
    case EXEC_OMP_DO:
      return gfc_trans_omp_do (code, code->ext.omp_clauses);
    case EXEC_OMP_FLUSH:
      return gfc_trans_omp_flush ();
    case EXEC_OMP_MASTER:
      return gfc_trans_omp_master (code);
    case EXEC_OMP_ORDERED:
      return gfc_trans_omp_ordered (code);
    case EXEC_OMP_PARALLEL:
      return gfc_trans_omp_parallel (code);
    case EXEC_OMP_PARALLEL_DO:
      return gfc_trans_omp_parallel_do (code);
    case EXEC_OMP_PARALLEL_SECTIONS:
      return gfc_trans_omp_parallel_sections (code);
    case EXEC_OMP_PARALLEL_WORKSHARE:
      return gfc_trans_omp_parallel_workshare (code);
    case EXEC_OMP_SECTIONS:
      return gfc_trans_omp_sections (code, code->ext.omp_clauses);
    case EXEC_OMP_SINGLE:
      return gfc_trans_omp_single (code, code->ext.omp_clauses);
    case EXEC_OMP_WORKSHARE:
      return gfc_trans_omp_workshare (code, code->ext.omp_clauses);
    default:
      gcc_unreachable ();
    }
}
