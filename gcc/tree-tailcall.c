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

/* A structure that describes the tailcall.  */

struct tailcall
{
  /* The blocks in that the call and corresponding return occur.  */
  basic_block call_block, return_block;

  /* The iterators pointing to the statements.  */
  block_stmt_iterator call_bsi, return_bsi;

  /* Variable used to return the call value.  */
  tree ret_variable;

  /* True if it is a call to the current function.  */
  bool tail_recursion;

  /* Next tailcall in the chain.  */
  struct tailcall *next;
};

static bool suitable_for_tail_opt_p (void);
static bool optimize_tail_call (struct tailcall *, tree *);
static void eliminate_tail_call (struct tailcall *, tree);
static void find_tail_calls (basic_block, struct tailcall *, struct tailcall **);

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

/* Finds tailcalls falling into basic block BB.  The current state of the
   recursive search is stored inside ACT, the list of found tailcalls is
   added to the start of RET.  */

static void
find_tail_calls (basic_block bb, struct tailcall *act, struct tailcall **ret)
{
  tree ass_var, stmt, func, param, args;
  block_stmt_iterator bsi;
  bool seen_return = false, found = false, tail_recursion = false;
  struct tailcall *nw;
  edge e;

  if (bb->succ->succ_next)
    return;

  for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      stmt = bsi_stmt (bsi);

      /* Ignore labels and gotos.  */
      if (TREE_CODE (stmt) == LABEL_EXPR
	  || TREE_CODE (stmt) == GOTO_EXPR)
	continue;

      /* Check for a return statement.  */
      if (TREE_CODE (stmt) == RETURN_EXPR)
	{
	  if (act->return_block)
	    abort ();

	  act->ret_variable = TREE_OPERAND (stmt, 0);
	  if (act->ret_variable
	      && TREE_CODE (act->ret_variable) == MODIFY_EXPR)
	    act->ret_variable = TREE_OPERAND (act->ret_variable, 1);

	  act->return_bsi = bsi;
	  act->return_block = bb;
	  seen_return = true;

	  continue;
	}

      found = true;

      /* Check for a call.  */
      if (TREE_CODE (stmt) == MODIFY_EXPR)
	{
	  ass_var = TREE_OPERAND (stmt, 0);
	  stmt = TREE_OPERAND (stmt, 1);
	}
      else
	ass_var = NULL_TREE;

      if (ass_var != act->ret_variable)
	break;

      if (TREE_CODE (stmt) != CALL_EXPR)
	break;

      func = get_callee_fndecl (stmt);
      if (func == current_function_decl)
	{
	  for (param = DECL_ARGUMENTS (func), args = TREE_OPERAND (stmt, 1);
	       param && args;
	       param = TREE_CHAIN (param), args = TREE_CHAIN (args))
	    if (param != TREE_VALUE (args)
		/* Make sure there are no problems with copying.  */
		&& !is_gimple_reg_type (TREE_TYPE (param)))
	      break;
	  if (!args && !param)
	    tail_recursion = true;
	}

      /* We found the call, record it.  */
      nw = xmalloc (sizeof (struct tailcall));

      nw->return_block = act->return_block;
      nw->return_bsi = act->return_bsi;

      nw->call_block = bb;
      nw->call_bsi = bsi;

      nw->ret_variable = act->ret_variable;
      nw->tail_recursion = tail_recursion;

      nw->next = *ret;
      *ret = nw;
      break;
    }

  /* Unless we found soumething that stops the search, recurse to the
     predecessors.  */
  if (!found)
    {
      for (e = bb->pred; e; e = e->pred_next)
	find_tail_calls (e->src, act, ret);
    }

  if (seen_return)
    {
      /* Undo the changes recorded in this basic block.  */
      act->ret_variable = NULL_TREE;
      act->return_block = NULL;
    }
}

/* Eliminates tail call described by T.  TMP_VARS is a list of
   temporary variables used to copy the function arguments.  */

static void
eliminate_tail_call (struct tailcall *t, tree tmp_vars)
{
  tree param, stmt, args, tmp_var;
  basic_block bb;

  stmt = bsi_stmt (t->call_bsi);
  bb = t->call_block;

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
      bsi_insert_before (&t->call_bsi,
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
      bsi_insert_before (&t->call_bsi,
			 build (MODIFY_EXPR, TREE_TYPE (param),
				param, TREE_VALUE (tmp_var)),
			 BSI_SAME_STMT);

  /* Replace the call by jump to the start of function.  */
  if (t->return_block == bb)
    {
      bsi_remove (&t->return_bsi);
      bb->succ->flags |= EDGE_FALLTHRU;
    }
  bsi_remove (&t->call_bsi);

  if (!redirect_edge_and_branch (t->call_block->succ,
				 ENTRY_BLOCK_PTR->succ->dest))
    abort ();
}

/* Optimizes the tailcall described by T.  *TMP_VARS is set to a list of
   temporary variables used to copy the function arguments the first time
   they are needed.  Returns true if cfg is changed.  */

static bool
optimize_tail_call (struct tailcall *t, tree *tmp_vars)
{
  tree tmp_var, param;

  /* Nothing to do unless it is tail recursion.  */
  if (!t->tail_recursion)
    return false;

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

  eliminate_tail_call (t, *tmp_vars);
  return true;
}

/* Optimizes tail calls in the function, turning the tail recursion
   into iteration.  */

void
tree_optimize_tail_calls (void)
{
  edge e;
  tree tmp_vars = NULL_TREE;
  struct tailcall common, *tailcalls = NULL, *next;
  bool changed = false;

  if (!suitable_for_tail_opt_p ())
    return;

  dump_file = dump_begin (TDI_tail, &dump_flags);

  common.return_block = NULL;
  common.ret_variable = NULL_TREE;

  for (e = EXIT_BLOCK_PTR->pred; e; e = e->pred_next)
    {
      find_tail_calls (e->src, &common, &tailcalls);

      if (common.return_block)
	abort ();
    }

  for (; tailcalls; tailcalls = next)
    {
      next = tailcalls->next;
      changed |= optimize_tail_call (tailcalls, &tmp_vars);
      free (tailcalls);
    }

  if (changed)
    cleanup_tree_cfg ();

  if (dump_file)
    {
      dump_cfg_function_to_file (current_function_decl, dump_file, dump_flags);
      dump_end (TDI_tail, dump_file);
    }
}
