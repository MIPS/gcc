/* Tail calls optimization on trees.
   Copyright (C) 2003 Free Software Foundation, Inc.

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
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "function.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "diagnostic.h"

/* Dump files and flags.  */
static FILE *dump_file;		/* CFG dump file. */
static int dump_flags;		/* CFG dump flags.  */

static bool suitable_for_tail_opt_p (void);
static void bb_optimize_tail_calls (basic_block, tree *);
static bool find_tail_call_p (basic_block, block_stmt_iterator *, bool *);
static void eliminate_tail_call (block_stmt_iterator, bool, tree);

/* Returns false when the function is not suitable for tail call optimization
   from some reason (e.g. if it takes variable number of arguments).  */

static bool
suitable_for_tail_opt_p (void)
{
  int i;

  if (current_function_stdarg)
    return false;

  /* No local variable should have its address taken, as otherwise it might
     be passed to the recursive call.  This of course is overly
     conservative and should be replaced by a dataflow analysis later.  */
  for (i = 0; i < (int) VARRAY_ACTIVE_SIZE (referenced_vars); i++)
    {
      tree var = VARRAY_TREE (referenced_vars, i);

      if (decl_function_context (var) == current_function_decl
	  && TREE_ADDRESSABLE (var))
	return false;
    }

  return true;
}

/* Checks whether basic block BB ends in a tail call.  If so, BSI is set
   to point to it.  HAS_RETURN is set to true if the call is followed by
   return.  */

static bool
find_tail_call_p (basic_block bb, block_stmt_iterator *bsi, bool *has_return)
{
  tree ret_var, ass_var, stmt, func, param, args;

  if (bb->succ->succ_next)
    return false;

  *bsi = bsi_last (bb);
  if (bsi_end_p (*bsi))
    return false;

  stmt = bsi_stmt (*bsi);
  *has_return = TREE_CODE (stmt) == RETURN_EXPR;
  if (*has_return)
    {
      ret_var = TREE_OPERAND (stmt, 0);
      if (ret_var && TREE_CODE (ret_var) == MODIFY_EXPR)
	ret_var = TREE_OPERAND (ret_var, 1);
      bsi_prev (bsi);
    }
  else
    ret_var = NULL_TREE;

  if (bsi_end_p (*bsi))
    return false;

  stmt = bsi_stmt (*bsi);
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      ass_var = TREE_OPERAND (stmt, 0);
      stmt = TREE_OPERAND (stmt, 1);
    }
  else
    ass_var = NULL_TREE;

  if (ass_var != ret_var)
    return false;

  if (TREE_CODE (stmt) != CALL_EXPR)
    return false;

  func = get_callee_fndecl (stmt);
  if (func != current_function_decl)
    return false;

  for (param = DECL_ARGUMENTS (func), args = TREE_OPERAND (stmt, 1);
       param && args;
       param = TREE_CHAIN (param), args = TREE_CHAIN (args))
    if (param != TREE_VALUE (args)
	/* Make sure there are no problems with copying.  */
	&& !is_gimple_reg_type (TREE_TYPE (param)))
      return false;
  if (args || param)
    return false;

  return true;
}

/* Eliminates tail call pointed by BSI.  HAS_RETURN is true if we also should
   remove the return statement following the call.  TMP_VARS is a list of
   temporary variables used to copy the function arguments.  */

static void
eliminate_tail_call (block_stmt_iterator bsi, bool has_return, tree tmp_vars)
{
  tree param, stmt, args, tmp_var, label;
  block_stmt_iterator bsi_s;
  bool emit_label;
  basic_block bb;

  stmt = bsi_stmt (bsi);
  bb = bb_for_stmt (stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Eliminated tail call in bb %d : ", bb->index);
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  if (TREE_CODE (stmt) == MODIFY_EXPR)
    stmt = TREE_OPERAND (stmt, 1);

  /* Copy the args if needed.  */
  for (param = DECL_ARGUMENTS (current_function_decl),
       args = TREE_OPERAND (stmt, 1),
       tmp_var = tmp_vars;
       param;
       param = TREE_CHAIN (param),
       args = TREE_CHAIN (args),
       tmp_var = TREE_CHAIN (tmp_var))
    if (param != TREE_VALUE (args)
	/* If the parameter is unused, it was not scanned in
	   find_referenced_vars, so we don't want to introduce
	   it here.  Additionally, it would obviously be
	   useless anyway.  */
	&& var_ann (param))
      bsi_insert_before (&bsi,
			 build (MODIFY_EXPR, TREE_TYPE (param),
				TREE_VALUE (tmp_var),
				unshare_expr (TREE_VALUE (args))),
			 BSI_SAME_STMT);
  for (param = DECL_ARGUMENTS (current_function_decl),
       args = TREE_OPERAND (stmt, 1),
       tmp_var = tmp_vars;
       param;
       param = TREE_CHAIN (param),
       args = TREE_CHAIN (args),
       tmp_var = TREE_CHAIN (tmp_var))
    if (param != TREE_VALUE (args)
	&& var_ann (param))
      bsi_insert_before (&bsi,
			 build (MODIFY_EXPR, TREE_TYPE (param),
				param, TREE_VALUE (tmp_var)),
			 BSI_SAME_STMT);

  /* Replace the call by jump to the start of function.  */
  bsi_s = bsi_start (ENTRY_BLOCK_PTR->succ->dest);
  if (bsi_end_p (bsi_s) || TREE_CODE (bsi_stmt (bsi_s)) != LABEL_EXPR)
    {
      label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
      DECL_ARTIFICIAL (label) = 1;
      DECL_CONTEXT (label) = current_function_decl;
      emit_label = true;
    }
  else
    {
      label = LABEL_EXPR_LABEL (bsi_stmt (bsi_s));
      emit_label = false;
    }

  if (has_return)
    bsi_remove (&bsi);
  bsi_replace (bsi, build1 (GOTO_EXPR, void_type_node, label));

  if (emit_label)
    {
      /* Emit the label; do it now, since otherwise we would conflict
	 with bsi in case the call is the first statement of the
	 program.  */
      bsi_s = bsi_start (ENTRY_BLOCK_PTR->succ->dest);
      bsi_insert_before (&bsi_s,
			 build1 (LABEL_EXPR, void_type_node, label),
			 BSI_NEW_STMT);
    }

  /* Update the cfg.  */
  remove_edge (bb->succ);
  make_edge (bb, ENTRY_BLOCK_PTR->succ->dest, 0);
}

/* Optimizes tail calls in the basic block BB.  *TMP_VARS is set to a list of
   temporary variables used to copy the function arguments the first time
   they are needed.  */

static void
bb_optimize_tail_calls (basic_block bb, tree *tmp_vars)
{
  block_stmt_iterator bsi;
  bool has_return = 0;
  tree tmp_var, param;

  /* Find the tail call.  Again, this should be more involved, catching
     the cases when the call and return are not in the same block.  */
  if (!find_tail_call_p (bb, &bsi, &has_return))
    return;

  if (!*tmp_vars)
    {
      /* Prepare the temporary variables.  */
      for (param = DECL_ARGUMENTS (current_function_decl);
	   param;
	   param = TREE_CHAIN (param))
	{
	  tmp_var = create_tmp_var (TREE_TYPE (param), "tailtmp");
	  add_referenced_tmp_var (tmp_var);
	  *tmp_vars = tree_cons (NULL_TREE, tmp_var, *tmp_vars);
	}
      *tmp_vars = nreverse (*tmp_vars);
    }

  eliminate_tail_call (bsi, has_return, *tmp_vars);
}

/* Optimizes tail calls in the function, turning the tail recursion
   into iteration.  */

void
tree_optimize_tail_calls (void)
{
  edge e, next;
  tree tmp_vars = NULL_TREE;

  if (!suitable_for_tail_opt_p ())
    return;

  dump_file = dump_begin (TDI_tail, &dump_flags);

  for (e = EXIT_BLOCK_PTR->pred; e; e = next)
    {
      next = e->pred_next;
      bb_optimize_tail_calls (e->src, &tmp_vars);
    }

  if (dump_file)
    {
      dump_function_to_file (current_function_decl, dump_file, dump_flags);
      dump_end (TDI_tail, dump_file);
    }
}
