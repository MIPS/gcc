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

tree
gfc_trans_omp_directive (gfc_code *code)
{
  tree stmt, omp_clauses = NULL_TREE;
  int clause;
  enum tree_code clause_code;

  /* FIXME */
  if (1 || code->op != EXEC_OMP_PARALLEL)
    {
      if (code->op == EXEC_OMP_FLUSH || code->op == EXEC_OMP_BARRIER)
	return build_empty_stmt ();
      if (code->op == EXEC_OMP_SECTIONS
	  || code->op == EXEC_OMP_PARALLEL_SECTIONS)
	{
	  stmtblock_t block;
	  gfc_start_block (&block);
	  for (code = code->block; code; code = code->block)
	    {
	      stmt = gfc_trans_code (code->next);
	      if (stmt != NULL_TREE && ! IS_EMPTY_STMT (stmt))
		gfc_add_expr_to_block (&block, stmt);
	    }
	  return gfc_finish_block (&block);
	}
      return gfc_trans_code (code->block->next);
    }

  stmt = gfc_trans_code (code->block->next);
  if (code->ext.omp_clauses)
    for (clause = 0; clause < OMP_LIST_NUM; clause++)
      if (code->ext.omp_clauses->lists[clause])
	{
	  clause_code = ERROR_MARK;
	  if (clause >= OMP_LIST_REDUCTION_FIRST
	      && clause <= OMP_LIST_REDUCTION_LAST)
	    {
	      printf ("reduction not handled yet\n");
	      continue;
	    }
	  switch (clause)
	    {
	    case OMP_LIST_PRIVATE:
	      clause_code = OMP_CLAUSE_PRIVATE;
	      break;
	    case OMP_LIST_SHARED:
	      clause_code = OMP_CLAUSE_SHARED;
	      break;
	    case OMP_LIST_FIRSTPRIVATE:
	      clause_code = OMP_CLAUSE_FIRSTPRIVATE;
	      break;
	    case OMP_LIST_LASTPRIVATE:
	      clause_code = OMP_CLAUSE_LASTPRIVATE;
	      break;
	    case OMP_LIST_COPYIN:
	      clause_code = OMP_CLAUSE_COPYIN;
	      break;
	    case OMP_LIST_COPYPRIVATE:
	      clause_code = OMP_CLAUSE_COPYPRIVATE;
	      break;
	    default:
	      break;
	    }
	  if (clause_code != ERROR_MARK)
	    {
	      gfc_namelist *n = code->ext.omp_clauses->lists[clause];
	      tree list = gfc_trans_omp_variable_list (n);
	      if (list != NULL_TREE)
		{
		  list = build1 (clause_code, NULL_TREE, list);
		  omp_clauses = tree_cons (NULL_TREE, list, omp_clauses);
		}
	    }
	}
  return build2 (OMP_PARALLEL, void_type_node, omp_clauses, stmt);
}
