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
#include "except.h"
#include "tree-pass.h"
#include "flags.h"


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
static bool optimize_tail_call (struct tailcall *, bool *, bool);
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
/* Returns false when the function is not suitable for tail call optimization
   from some reason (e.g. if it takes variable number of arguments).
   This test must pass in addition to suitable_for_tail_opt_p in order to make
   tail call discovery happen.  */

static bool
suitable_for_tail_call_opt_p (void)
{
  /* alloca (until we have stack slot life analysis) inhibits
     sibling call optimizations, but not tail recursion.  */
  if (current_function_calls_alloca)
    return false;

  /* If we are using sjlj exceptions, we may need to add a call to
     _Unwind_SjLj_Unregister at exit of the function.  Which means
     that we cannot do any sibcall transformations.  */
  if (USING_SJLJ_EXCEPTIONS && current_function_has_exception_handlers ())
    return false;

  /* Any function that calls setjmp might have longjmp called from
     any called function.  ??? We really should represent this
     properly in the CFG so that this needn't be special cased.  */
  if (current_function_calls_setjmp)
    return false;

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

	  if (STMT_VDEF_OPS (stmt)
	      || STMT_VUSE_OPS (stmt))
	    return;

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

  /* Unless we found something that stops the search, recurse to the
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
  basic_block bb, first;
  edge e;
  tree phi;
  stmt_ann_t ann;
  vdef_optype vdefs;
  unsigned i;

  stmt = bsi_stmt (t->call_bsi);
  get_stmt_operands (stmt);
  ann = stmt_ann (stmt);
  bb = t->call_block;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Eliminated tail recursion in bb %d : ",
	       bb->index);
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

  first = ENTRY_BLOCK_PTR->succ->dest;
  e = redirect_edge_and_branch (t->call_block->succ, first);
  if (!e)
    abort ();
  PENDING_STMT (e) = NULL_TREE;


  /* Add phi node entries for arguments.  Not every PHI node corresponds to
     a function argument (there may be PHI nodes for virtual definitions of the
     eliminated calls), so we search for a PHI corresponding to each argument
     rather than searching for which argument a PHI node corresponds to.  */
  
  for (param = DECL_ARGUMENTS (current_function_decl),
       args = TREE_OPERAND (stmt, 1);
       param;
       param = TREE_CHAIN (param),
       args = TREE_CHAIN (args))
    {
      
      for (phi = phi_nodes (first); phi; phi = TREE_CHAIN (phi))
	if (param == SSA_NAME_VAR (PHI_RESULT (phi)))
	  break;

      /* The phi node indeed does not have to be there, in case the operand is
	 invariant in the function.  */
      if (!phi)
	continue;

      add_phi_arg (&phi, TREE_VALUE (args), e);
    }

  /* Add phi nodes for the call clobbered variables.  */
  vdefs = VDEF_OPS (ann);
  for (i = 0; i < NUM_VDEFS (vdefs); i++)
    {
      param = SSA_NAME_VAR (VDEF_RESULT (vdefs, i));
      for (phi = phi_nodes (first); phi; phi = TREE_CHAIN (phi))
	if (param == SSA_NAME_VAR (PHI_RESULT (phi)))
	  break;

      if (!phi)
	{
	  tree name = var_ann (param)->default_def;
	  tree new_name = make_ssa_name (param, SSA_NAME_DEF_STMT (name));

	  var_ann (param)->default_def = new_name;
	  phi = create_phi_node (name, first);
	  SSA_NAME_DEF_STMT (name) = phi;
	  add_phi_arg (&phi, new_name, ENTRY_BLOCK_PTR->succ);

	  /* For all calls the same set of variables should be clobbered.  This
	     means that there always should be the appropriate phi node except
	     for the first time we eliminate the call.  */
	  if (first->pred->pred_next->pred_next)
	    abort ();
	}

      add_phi_arg (&phi, VDEF_OP (vdefs, i), e);
    }
}

/* Optimizes the tailcall described by T.  First time we optimize tail
   call, we need to create PHI nodes for arguments.  This is indicated
   by PHIS_CONSTRUCTED.  */

static bool
optimize_tail_call (struct tailcall *t, bool *phis_constructed,
		    bool opt_tailcalls)
{
  if (t->tail_recursion)
    {
      basic_block first = ENTRY_BLOCK_PTR->succ->dest;
      tree param;

      if (!*phis_constructed)
	{
	  /* Ensure that there is only one predecessor of the block.  */
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
  else if (opt_tailcalls)
    {
      tree stmt = bsi_stmt (t->call_bsi);

      if (TREE_CODE (stmt) == MODIFY_EXPR)
	stmt = TREE_OPERAND (stmt, 1);
      if (TREE_CODE (stmt) != CALL_EXPR)
	abort ();
      CALL_EXPR_TAILCALL (stmt) = 1;
      if (dump_file && (dump_flags & TDF_DETAILS))
        {
	  fprintf (dump_file, "Found tail call ");
	  print_generic_expr (dump_file, stmt, 0);
	  fprintf (dump_file, " in bb %i", t->call_block->index);
	}
    }
  return false;
}

/* Optimizes tail calls in the function, turning the tail recursion
   into iteration.  */

static void
tree_optimize_tail_calls_1 (bool opt_tailcalls)
{
  edge e;
  bool phis_constructed = false;
  struct tailcall common, *tailcalls = NULL, *next;
  bool changed = false;

  if (!suitable_for_tail_opt_p ())
    return;
  if (opt_tailcalls)
    opt_tailcalls = suitable_for_tail_call_opt_p ();

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
      changed |= optimize_tail_call (tailcalls, &phis_constructed, 
		      		     opt_tailcalls);
      free (tailcalls);
    }

  if (changed)
    {
      free_dominance_info (CDI_DOMINATORS);
      cleanup_tree_cfg ();
    }
}

static void
execute_tail_recursion (void)
{
  tree_optimize_tail_calls_1 (false);
}

static bool
gate_tail_calls (void)
{
  return flag_optimize_sibling_calls != 0;
}

static void
execute_tail_calls (void)
{
  tree_optimize_tail_calls_1 (true);
}

struct tree_opt_pass pass_tail_recursion = 
{
  "tailr",				/* name */
  NULL,					/* gate */
  execute_tail_recursion,		/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa	/* todo_flags_finish */
};

struct tree_opt_pass pass_tail_calls = 
{
  "tailc",				/* name */
  gate_tail_calls,			/* gate */
  execute_tail_calls,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa	/* todo_flags_finish */
};
