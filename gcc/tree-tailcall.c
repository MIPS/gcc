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
static bool optimize_tail_call (struct tailcall *, bool *);
static void eliminate_tail_call (struct tailcall *);
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
	  && !TREE_STATIC (var)
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
  tree old_ret_variable = act->ret_variable;

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
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	if (PHI_RESULT (phi) == act->ret_variable)
	  break;
      if (phi)
	{
	  int phi_num_args = PHI_NUM_ARGS (phi);
	  int i;

	  for (i = 0; i < phi_num_args; i++)
	    {
	      act->ret_variable = PHI_ARG_DEF (phi, i);
	      find_tail_calls (PHI_ARG_EDGE (phi, i)->src, act, ret);
	    }
	}
      else
        for (e = bb->pred; e; e = e->pred_next)
	  find_tail_calls (e->src, act, ret);
    }

  if (seen_return)
    {
      /* Undo the changes recorded in this basic block.  */
      act->ret_variable = NULL_TREE;
      act->return_block = NULL;
    }
  act->ret_variable = old_ret_variable;
}

/* Eliminates tail call described by T.  TMP_VARS is a list of
   temporary variables used to copy the function arguments.  */

static void
eliminate_tail_call (struct tailcall *t)
{
  tree param, stmt, args;
  basic_block bb;
  edge e;
  tree phi;

  stmt = bsi_stmt (t->call_bsi);
  bb = t->call_block;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Eliminated tail recursion in bb %d : ", bb->index);
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  if (TREE_CODE (stmt) == MODIFY_EXPR)
    stmt = TREE_OPERAND (stmt, 1);

  /* Replace the call by jump to the start of function.  */
  if (t->return_block == bb)
    {
      bsi_remove (&t->return_bsi);
      bb->succ->flags |= EDGE_FALLTHRU;
    }
  bsi_remove (&t->call_bsi);

  e = redirect_edge_and_branch (t->call_block->succ,
				ENTRY_BLOCK_PTR->succ->dest);
  if (!e)
    abort ();
  /* We expect that each PHI node on first basic block represent an argument.
     Add proper entries.  */
  for (phi = phi_nodes (ENTRY_BLOCK_PTR->succ->dest); phi;
       phi = TREE_CHAIN (phi))
    {
      for (param = DECL_ARGUMENTS (current_function_decl),
	   args = TREE_OPERAND (stmt, 1);
	   param;
	   param = TREE_CHAIN (param),
	   args = TREE_CHAIN (args))
	if (param == SSA_NAME_VAR (PHI_RESULT (phi)))
	  break;
      if (!param)
	abort ();
      add_phi_arg (&phi, TREE_VALUE (args), e);
    }
}

/* Optimizes the tailcall described by T.  First time we optimize tail
   call, we need to create PHI nodes for arguments.  This is indicated
   by PHIS_CONSTRUCTED.  */

static bool
optimize_tail_call (struct tailcall *t, bool *phis_constructed)
{
  if (t->tail_recursion)
    {
      basic_block first = ENTRY_BLOCK_PTR->succ->dest;
      tree param;

      if (!*phis_constructed)
	{
	  /* Ensure that there is only one predecestor of the block.  */
	  if (first->pred->pred_next)
	    first = split_edge (ENTRY_BLOCK_PTR->succ);
	  /* Copy the args if needed.  */
	  for (param = DECL_ARGUMENTS (current_function_decl);
	       param;
	       param = TREE_CHAIN (param))
	    if (var_ann (param)
		/* Also parameters that are only defined but never used need not
		   be copied.  */
		&& (var_ann (param)->default_def
		    && TREE_CODE (var_ann (param)->default_def) == SSA_NAME))
	    {
	      tree name = var_ann (param)->default_def;
	      tree new_name = make_ssa_name (param, SSA_NAME_DEF_STMT (name));
	      tree phi;

	      var_ann (param)->default_def = new_name;
	      phi = create_phi_node (name, first);
	      SSA_NAME_DEF_STMT (name) = phi;
	      add_phi_arg (&phi, new_name, first->pred);
	    }
	  *phis_constructed = true;
	}

      eliminate_tail_call (t);
      return true;
    }
  return false;
}

/* Optimizes tail calls in the function, turning the tail recursion
   into iteration.  */

void
tree_optimize_tail_calls (void)
{
  edge e;
  bool phis_constructed = false;
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
      changed |= optimize_tail_call (tailcalls, &phis_constructed);
      free (tailcalls);
    }

  if (changed)
    cleanup_tree_cfg ();

  if (dump_file)
    {
      dump_function_to_file (current_function_decl, dump_file, dump_flags);
      dump_end (TDI_tail, dump_file);
    }
}
