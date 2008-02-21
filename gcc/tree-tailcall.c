/* Tail call optimization on trees.
   Copyright (C) 2003, 2004, 2005, 2006, 2007 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

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
#include "langhooks.h"
#include "dbgcnt.h"

/* The file implements the tail recursion elimination.  It is also used to
   analyze the tail calls in general, passing the results to the rtl level
   where they are used for sibcall optimization.

   In addition to the standard tail recursion elimination, we handle the most
   trivial cases of making the call tail recursive by creating accumulators.
   For example the following function

   int sum (int n)
   {
     if (n > 0)
       return n + sum (n - 1);
     else
       return 0;
   }

   is transformed into

   int sum (int n)
   {
     int acc = 0;

     while (n > 0)
       acc += n--;

     return acc;
   }

   To do this, we maintain two accumulators (a_acc and m_acc) that indicate 
   when we reach the return x statement, we should return a_acc + x * m_acc
   instead.  They are initially initialized to 0 and 1, respectively,
   so the semantics of the function is obviously preserved.  If we are
   guaranteed that the value of the accumulator never change, we
   omit the accumulator.

   There are three cases how the function may exit.  The first one is
   handled in adjust_return_value, the other two in adjust_accumulator_values
   (the second case is actually a special case of the third one and we
   present it separately just for clarity):

   1) Just return x, where x is not in any of the remaining special shapes.
      We rewrite this to a gimple equivalent of return m_acc * x + a_acc.
      
   2) return f (...), where f is the current function, is rewritten in a
      classical tail-recursion elimination way, into assignment of arguments
      and jump to the start of the function.  Values of the accumulators
      are unchanged.
	       
   3) return a + m * f(...), where a and m do not depend on call to f.
      To preserve the semantics described before we want this to be rewritten
      in such a way that we finally return

      a_acc + (a + m * f(...)) * m_acc = (a_acc + a * m_acc) + (m * m_acc) * f(...).

      I.e. we increase a_acc by a * m_acc, multiply m_acc by m and
      eliminate the tail call to f.  Special cases when the value is just
      added or just multiplied are obtained by setting a = 0 or m = 1.

   TODO -- it is possible to do similar tricks for other operations.  */

/* A structure that describes the tailcall.  */

struct tailcall
{
  /* The block in that the call occur.  */
  basic_block call_block;

  /* The iterator pointing to the call statement.  */
  gimple_stmt_iterator call_bsi;

  /* True if it is a call to the current function.  */
  bool tail_recursion;

  /* The return value of the caller is mult * f + add, where f is the return
     value of the call.  */
  tree mult, add;

  /* Next tailcall in the chain.  */
  struct tailcall *next;
};

/* The variables holding the value of multiplicative and additive
   accumulator.  */
static tree m_acc, a_acc;

static bool suitable_for_tail_opt_p (void);
static bool optimize_tail_call (struct tailcall *, bool);
static void eliminate_tail_call (struct tailcall *);
static void find_tail_calls (basic_block, struct tailcall **);

/* Returns false when the function is not suitable for tail call optimization
   from some reason (e.g. if it takes variable number of arguments).  */

static bool
suitable_for_tail_opt_p (void)
{
  referenced_var_iterator rvi;
  tree var;

  if (current_function_stdarg)
    return false;

  /* No local variable nor structure field should be call-clobbered.  We
     ignore any kind of memory tag, as these are not real variables.  */

  FOR_EACH_REFERENCED_VAR (var, rvi)
    {
      if (!is_global_var (var)
	  && (!MTAG_P (var) || TREE_CODE (var) == STRUCT_FIELD_TAG)
	  && (gimple_aliases_computed_p (cfun) ? is_call_clobbered (var)
	      : TREE_ADDRESSABLE (var)))
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
  tree param;

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

  /* ??? It is OK if the argument of a function is taken in some cases,
     but not in all cases.  See PR15387 and PR19616.  Revisit for 4.1.  */
  for (param = DECL_ARGUMENTS (current_function_decl);
       param;
       param = TREE_CHAIN (param))
    if (TREE_ADDRESSABLE (param))
      return false;

  return true;
}

/* Checks whether the expression EXPR in stmt AT is independent of the
   statement pointed to by BSI (in a sense that we already know EXPR's value
   at BSI).  We use the fact that we are only called from the chain of
   basic blocks that have only single successor.  Returns the expression
   containing the value of EXPR at BSI.  */

static tree
independent_of_stmt_p (tree expr, gimple at, gimple_stmt_iterator bsi)
{
  basic_block bb, call_bb, at_bb;
  edge e;
  edge_iterator ei;

  if (is_gimple_min_invariant (expr))
    return expr;

  if (TREE_CODE (expr) != SSA_NAME)
    return NULL_TREE;

  /* Mark the blocks in the chain leading to the end.  */
  at_bb = gimple_bb (at);
  call_bb = gimple_bb (gsi_stmt (bsi));
  for (bb = call_bb; bb != at_bb; bb = single_succ (bb))
    bb->aux = &bb->aux;
  bb->aux = &bb->aux;

  while (1)
    { 
      at = SSA_NAME_DEF_STMT (expr);
      bb = gimple_bb (at);

      /* The default definition or defined before the chain.  */
      if (!bb || !bb->aux)
	break;

      if (bb == call_bb)
	{
	  for (; !gsi_end_p (bsi); gsi_next (&bsi))
	    if (gsi_stmt (bsi) == at)
	      break;

	  if (!gsi_end_p (bsi))
	    expr = NULL_TREE;
	  break;
	}

      if (gimple_code (at) != GIMPLE_PHI)
	{
	  expr = NULL_TREE;
	  break;
	}

      FOR_EACH_EDGE (e, ei, bb->preds)
	if (e->src->aux)
	  break;
      gcc_assert (e);

      expr = PHI_ARG_DEF_FROM_EDGE (at, e);
      if (TREE_CODE (expr) != SSA_NAME)
	{
	  /* The value is a constant.  */
	  break;
	}
    }

  /* Unmark the blocks.  */
  for (bb = call_bb; bb != at_bb; bb = single_succ (bb))
    bb->aux = NULL;
  bb->aux = NULL;

  return expr;
}

/* Simulates the effect of an assignment in STMT on the return value
   of the tail recursive CALL passed in ASS_VAR.  M and A are the
   multiplicative and the additive factor for the real return value.  */

static bool
process_assignment (gimple stmt, gimple_stmt_iterator call, tree *m,
		    tree *a, tree *ass_var)
{
  tree op0, op1, non_ass_var, src_var;
  tree dest = gimple_assign_lhs (stmt);
  enum tree_code code = gimple_subcode (stmt);

  /* See if this is a simple copy operation of an SSA name to the function
     result.  In that case we may have a simple tail call.  Ignore type
     conversions that can never produce extra code between the function
     call and the function return.  */
  src_var = copy_or_nop_cast_stmt_rhs (stmt);
  if (src_var != NULL_TREE)
    {
      if (src_var != *ass_var)
	return false;

      *ass_var = dest;
      return true;
    }

  if (TREE_CODE_CLASS (code) != tcc_binary)
    return false;

  /* Accumulator optimizations will reverse the order of operations.
     We can only do that for floating-point types if we're assuming
     that addition and multiplication are associative.  */
  if (!flag_associative_math
      && FLOAT_TYPE_P (TREE_TYPE (DECL_RESULT (current_function_decl))))
    return false;

  /* We only handle the code like

     x = call ();
     y = m * x;
     z = y + a;
     return z;

     TODO -- Extend it for cases where the linear transformation of the output
     is expressed in a more complicated way.  */

  op0 = gimple_assign_rhs1 (stmt);
  op1 = gimple_assign_rhs2 (stmt);

  if (op0 == *ass_var
      && (non_ass_var = independent_of_stmt_p (op1, stmt, call)))
    ;
  else if (op1 == *ass_var
	   && (non_ass_var = independent_of_stmt_p (op0, stmt, call)))
    ;
  else
    return false;

  switch (code)
    {
    case PLUS_EXPR:
      /* There should be no previous addition.  TODO -- it should be fairly
	 straightforward to lift this restriction -- just allow storing
	 more complicated expressions in *A, and gimplify it in
	 adjust_accumulator_values.  */
      if (*a)
	return false;
      *a = non_ass_var;
      *ass_var = dest;
      return true;

    case MULT_EXPR:
      /* Similar remark applies here.  Handling multiplication after addition
	 is just slightly more complicated -- we need to multiply both *A and
	 *M.  */
      if (*a || *m)
	return false;
      *m = non_ass_var;
      *ass_var = dest;
      return true;

      /* TODO -- Handle other codes (NEGATE_EXPR, MINUS_EXPR, POINTER_PLUS_EXPR).  */

    default:
      return false;
    }
}

/* Propagate VAR through phis on edge E.  */

static tree
propagate_through_phis (tree var, edge e)
{
  basic_block dest = e->dest;
  gimple phi;
  gimple_seq phis = phi_nodes (dest);
  gimple_stmt_iterator psi;

  for (psi = gsi_start (phis); !gsi_end_p (psi); gsi_next (&psi))
    {
      phi = gsi_stmt (psi);
      if (PHI_ARG_DEF_FROM_EDGE (phi, e) == var)
	return PHI_RESULT (phi);
    }

  return var;
}

/* Finds tailcalls falling into basic block BB. The list of found tailcalls is
   added to the start of RET.  */

static void
find_tail_calls (basic_block bb, struct tailcall **ret)
{
  tree ass_var, ret_var, func, param, m, a;
  gimple stmt, call = NULL;
  gimple_stmt_iterator bsi, absi;
  bool tail_recursion;
  struct tailcall *nw;
  edge e;
  basic_block abb;
  unsigned i;

  if (!single_succ_p (bb))
    return;

  for (bsi = gsi_last_bb (bb); !gsi_end_p (bsi); gsi_prev (&bsi))
    {
      stmt = gsi_stmt (bsi);

      /* Ignore labels.  */
      if (gimple_code (stmt) == GIMPLE_LABEL)
	continue;

      /* Check for a call.  */
      if (gimple_code (stmt) == GIMPLE_CALL)
	{
	  ass_var = gimple_call_lhs (stmt);
	  call = stmt;
	  break;
	}

      /* If the statement has virtual or volatile operands, fail.  */
      if (stmt_references_memory_p (stmt))
	return;
    }

  if (call == NULL)
    {
      edge_iterator ei;
      /* Recurse to the predecessors.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
	find_tail_calls (e->src, ret);

      return;
    }

  /* We found the call, check whether it is suitable.  */
  tail_recursion = false;
  func = gimple_call_fndecl (call);
  if (func == current_function_decl)
    {
      tree arg;

      for (param = DECL_ARGUMENTS (func), i = 0;
	   param && i < gimple_call_num_args (call);
	   param = TREE_CHAIN (param), i++)
	{
	  arg = gimple_call_arg (call, i);

	  if (param != arg)
	    {
	      /* Make sure there are no problems with copying.  The parameter
	         have a copyable type and the two arguments must have reasonably
	         equivalent types.  The latter requirement could be relaxed if
	         we emitted a suitable type conversion statement.  */
	      if (!is_gimple_reg_type (TREE_TYPE (param))
		  || !useless_type_conversion_p (TREE_TYPE (param),
					        TREE_TYPE (arg)))
		break;

	      /* The parameter should be a real operand, so that phi node
		 created for it at the start of the function has the meaning
		 of copying the value.  This test implies is_gimple_reg_type
		 from the previous condition, however this one could be
		 relaxed by being more careful with copying the new value
		 of the parameter (emitting appropriate GIMPLE_MODIFY_STMT and
		 updating the virtual operands).  */
	      if (!is_gimple_reg (param))
		break;
	    }
	}
      if (i == gimple_call_num_args (call) && param == NULL_TREE)
	tail_recursion = true;
    }

  /* Now check the statements after the call.  None of them has virtual
     operands, so they may only depend on the call through its return
     value.  The return value should also be dependent on each of them,
     since we are running after dce.  */
  m = NULL_TREE;
  a = NULL_TREE;

  abb = bb;
  absi = bsi;
  while (1)
    {
      gsi_next (&absi);

      while (gsi_end_p (absi))
	{
	  ass_var = propagate_through_phis (ass_var, single_succ_edge (abb));
	  abb = single_succ (abb);
	  absi = gsi_start_bb (abb);
	}

      stmt = gsi_stmt (absi);

      if (gimple_code (stmt) == GIMPLE_LABEL)
	continue;

      if (gimple_code (stmt) == GIMPLE_RETURN)
	break;

      if (gimple_code (stmt) != GIMPLE_ASSIGN)
	return;

      if (!process_assignment (stmt, bsi, &m, &a, &ass_var))
	return;
    }


  /* We may proceed if there either is no return value, or the return value
     is identical to the call's return.  */
  ret_var = gimple_return_retval (stmt);
  if (ret_var
      && (ret_var != ass_var))
    return;

  /* If this is not a tail recursive call, we cannot handle addends or
     multiplicands.  */
  if (!tail_recursion && (m || a))
    return;

  nw = XNEW (struct tailcall);

  nw->call_block = bb;
  nw->call_bsi = bsi;

  nw->tail_recursion = tail_recursion;

  nw->mult = m;
  nw->add = a;

  nw->next = *ret;
  *ret = nw;
}

/* Adjust the accumulator values according to A and M after BSI, and update
   the phi nodes on edge BACK.  */

static void
adjust_accumulator_values (gimple_stmt_iterator bsi, tree m, tree a, edge back)
{
  gimple stmt, phi = NULL;
  tree var, tmp;
  tree ret_type = TREE_TYPE (DECL_RESULT (current_function_decl));
  tree a_acc_arg = a_acc, m_acc_arg = m_acc;
  gimple_seq phis;
  gimple_stmt_iterator psi;

  if (a)
    {
      if (m_acc)
	{
	  if (integer_onep (a))
	    var = m_acc;
	  else
	    {
	      tmp = create_tmp_var (ret_type, "acc_tmp");
	      add_referenced_var (tmp);
	      stmt = gimple_build_assign_with_ops (MULT_EXPR,
						   tmp, m_acc, a);
	      var = make_ssa_name (tmp, stmt);
	      gimple_assign_set_lhs (stmt, var);
	      gsi_insert_after (&bsi, stmt, GSI_NEW_STMT);
	    }
	}
      else
	var = a;

      stmt = gimple_build_assign_with_ops (PLUS_EXPR,
					   SSA_NAME_VAR (a_acc),
					   a_acc, var);
      var = make_ssa_name (SSA_NAME_VAR (a_acc), stmt);
      gimple_assign_set_lhs (stmt, var);
      gsi_insert_after (&bsi, stmt, GSI_NEW_STMT);
      a_acc_arg = var;
    }

  if (m)
    {
      stmt = gimple_build_assign_with_ops (MULT_EXPR,
					   SSA_NAME_VAR (m_acc),
					   m_acc, m);
      var = make_ssa_name (SSA_NAME_VAR (m_acc), stmt);
      gimple_assign_set_lhs (stmt, var);
      gsi_insert_after (&bsi, stmt, GSI_NEW_STMT);
      m_acc_arg = var;
    }

  if (a_acc)
    { 
      phis = phi_nodes (back->dest);
      for (psi = gsi_start (phis); !gsi_end_p (psi); gsi_next (&psi))
	{
	  phi = gsi_stmt (psi);
	  if (PHI_RESULT (phi) == a_acc)
	    break;
	}

      add_phi_arg (phi, a_acc_arg, back);
    }

  if (m_acc)
    {
      phis = phi_nodes (back->dest);
      for (psi = gsi_start (phis); !gsi_end_p (psi); gsi_next (&psi))
	{
	  phi = gsi_stmt (psi);
	  if (PHI_RESULT (phi) == m_acc)
	    break;
	}

      add_phi_arg (phi, m_acc_arg, back);
    }
}

/* Adjust value of the return at the end of BB according to M and A
   accumulators.  */

static void
adjust_return_value (basic_block bb, tree m, tree a)
{
  gimple ret_stmt = last_stmt (bb), stmt;
  tree ret_var, var, tmp;
  tree ret_type = TREE_TYPE (DECL_RESULT (current_function_decl));
  gimple_stmt_iterator bsi = gsi_last_bb (bb);

  gcc_assert (gimple_code (ret_stmt) == GIMPLE_RETURN);

  ret_var = gimple_return_retval (ret_stmt);
  if (!ret_var)
    return;

  if (m)
    {
      tmp = create_tmp_var (ret_type, "acc_tmp");
      add_referenced_var (tmp);
      stmt = gimple_build_assign_with_ops (MULT_EXPR,
					   tmp, m_acc, ret_var);
      var = make_ssa_name (tmp, stmt);
      gimple_assign_set_lhs (stmt, var);
      gsi_insert_before (&bsi, stmt, GSI_SAME_STMT);
    }
  else
    var = ret_var;

  if (a)
    {
      tmp = create_tmp_var (ret_type, "acc_tmp");
      add_referenced_var (tmp);
      stmt = gimple_build_assign_with_ops (PLUS_EXPR,
					   tmp, a_acc, var);
      var = make_ssa_name (tmp, stmt);
      gimple_assign_set_lhs (stmt, var);
      gsi_insert_before (&bsi, stmt, GSI_SAME_STMT);
    }

  gimple_return_set_retval (ret_stmt, var);
  update_stmt (ret_stmt);
}

/* Subtract COUNT and FREQUENCY from the basic block and it's
   outgoing edge.  */
static void
decrease_profile (basic_block bb, gcov_type count, int frequency)
{
  edge e;
  bb->count -= count;
  if (bb->count < 0)
    bb->count = 0;
  bb->frequency -= frequency;
  if (bb->frequency < 0)
    bb->frequency = 0;
  if (!single_succ_p (bb))
    {
      gcc_assert (!EDGE_COUNT (bb->succs));
      return;
    }
  e = single_succ_edge (bb);
  e->count -= count;
  if (e->count < 0)
    e->count = 0;
}

/* Returns true if argument PARAM of the tail recursive call needs to be copied
   when the call is eliminated.  */

static bool
arg_needs_copy_p (tree param)
{
  tree def;

  if (!is_gimple_reg (param) || !var_ann (param))
    return false;
		
  /* Parameters that are only defined but never used need not be copied.  */
  def = gimple_default_def (cfun, param);
  if (!def)
    return false;

  return true;
}

/* Eliminates tail call described by T.  TMP_VARS is a list of
   temporary variables used to copy the function arguments.  */

static void
eliminate_tail_call (struct tailcall *t)
{
  tree param, rslt;
  gimple stmt, phi;
  tree arg;
  unsigned i;
  basic_block bb, first;
  edge e;
  gimple_stmt_iterator bsi;
  gimple_seq phis;
  gimple_stmt_iterator psi;

  stmt = gsi_stmt (t->call_bsi);
  bb = t->call_block;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Eliminated tail recursion in bb %d : ",
	       bb->index);
      print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  first = single_succ (ENTRY_BLOCK_PTR);

  /* Remove the code after call_bsi that will become unreachable.  The
     possibly unreachable code in other blocks is removed later in
     cfg cleanup.  */
  bsi = t->call_bsi;
  gsi_next (&bsi);
  while (!gsi_end_p (bsi))
    {
      gimple t = gsi_stmt (bsi);
      /* Do not remove the return statement, so that redirect_edge_and_branch
	 sees how the block ends.  */
      if (gimple_code (t) == GIMPLE_RETURN)
	break;

      gsi_remove (&bsi, true);
      release_defs (t);
    }

  /* Number of executions of function has reduced by the tailcall.  */
  e = single_succ_edge (t->call_block);
  decrease_profile (EXIT_BLOCK_PTR, e->count, EDGE_FREQUENCY (e));
  decrease_profile (ENTRY_BLOCK_PTR, e->count, EDGE_FREQUENCY (e));
  if (e->dest != EXIT_BLOCK_PTR)
    decrease_profile (e->dest, e->count, EDGE_FREQUENCY (e));

  /* Replace the call by a jump to the start of function.  */
  e = redirect_edge_and_branch (single_succ_edge (t->call_block), first);
  gcc_assert (e);
  PENDING_STMT (e) = NULL;

  /* Add phi node entries for arguments.  The ordering of the phi nodes should
     be the same as the ordering of the arguments.  */
  phis = phi_nodes (first);
  for (param = DECL_ARGUMENTS (current_function_decl), i = 0,
	 psi = gsi_start (phis);
       param;
       param = TREE_CHAIN (param), i++)
    {
      arg = gimple_call_arg (stmt, i);
      if (!arg_needs_copy_p (param))
	continue;

      phi = gsi_stmt (psi);
      gcc_assert (param == SSA_NAME_VAR (PHI_RESULT (phi)));

      add_phi_arg (phi, arg, e);
      gsi_next (&psi);
    }

  /* Update the values of accumulators.  */
  adjust_accumulator_values (t->call_bsi, t->mult, t->add, e);

  /* Result of the call will no longer be defined.  So adjust the
     SSA_NAME_DEF_STMT accordingly.  */
  rslt = gimple_call_lhs (stmt);
  if (rslt)
    SSA_NAME_DEF_STMT (rslt) = gimple_build_nop ();

  gsi_remove (&t->call_bsi, true);
  release_defs (stmt);
}

/* Add phi nodes for the virtual operands defined in the function to the
   header of the loop created by tail recursion elimination.

   Originally, we used to add phi nodes only for call clobbered variables,
   as the value of the non-call clobbered ones obviously cannot be used
   or changed within the recursive call.  However, the local variables
   from multiple calls now share the same location, so the virtual ssa form
   requires us to say that the location dies on further iterations of the loop,
   which requires adding phi nodes.
*/
static void
add_virtual_phis (void)
{
  referenced_var_iterator rvi;
  tree var;

  /* The problematic part is that there is no way how to know what
     to put into phi nodes (there in fact does not have to be such
     ssa name available).  A solution would be to have an artificial
     use/kill for all virtual operands in EXIT node.  Unless we have
     this, we cannot do much better than to rebuild the ssa form for
     possibly affected virtual ssa names from scratch.  */

  FOR_EACH_REFERENCED_VAR (var, rvi)
    {
      if (!is_gimple_reg (var) && gimple_default_def (cfun, var) != NULL_TREE)
	mark_sym_for_renaming (var);
    }
}

/* Optimizes the tailcall described by T.  If OPT_TAILCALLS is true, also
   mark the tailcalls for the sibcall optimization.  */

static bool
optimize_tail_call (struct tailcall *t, bool opt_tailcalls)
{
  if (t->tail_recursion)
    {
      eliminate_tail_call (t);
      return true;
    }

  if (opt_tailcalls)
    {
      gimple stmt = gsi_stmt (t->call_bsi);

      gimple_call_set_tail (stmt);
      if (dump_file && (dump_flags & TDF_DETAILS))
        {
	  fprintf (dump_file, "Found tail call ");
	  print_gimple_stmt (dump_file, stmt, 0, dump_flags);
	  fprintf (dump_file, " in bb %i\n", t->call_block->index);
	}
    }

  return false;
}

/* Optimizes tail calls in the function, turning the tail recursion
   into iteration.  */

static unsigned int
tree_optimize_tail_calls_1 (bool opt_tailcalls)
{
  edge e;
  bool phis_constructed = false;
  struct tailcall *tailcalls = NULL, *act, *next;
  bool changed = false;
  basic_block first = single_succ (ENTRY_BLOCK_PTR);
  gimple stmt, phi;
  tree param, ret_type, tmp;
  edge_iterator ei;

  if (!suitable_for_tail_opt_p ())
    return 0;
  if (opt_tailcalls)
    opt_tailcalls = suitable_for_tail_call_opt_p ();

  FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR->preds)
    {
      /* Only traverse the normal exits, i.e. those that end with return
	 statement.  */
      stmt = last_stmt (e->src);

      if (stmt
	  && gimple_code (stmt) == GIMPLE_RETURN)
	find_tail_calls (e->src, &tailcalls);
    }

  /* Construct the phi nodes and accumulators if necessary.  */
  a_acc = m_acc = NULL_TREE;
  for (act = tailcalls; act; act = act->next)
    {
      if (!act->tail_recursion)
	continue;

      if (!phis_constructed)
	{
	  /* Ensure that there is only one predecessor of the block.  */
	  if (!single_pred_p (first))
	    first = split_edge (single_succ_edge (ENTRY_BLOCK_PTR));

	  /* Copy the args if needed.  */
	  for (param = DECL_ARGUMENTS (current_function_decl);
	       param;
	       param = TREE_CHAIN (param))
	    if (arg_needs_copy_p (param))
	      {
		tree name = gimple_default_def (cfun, param);
		tree new_name = make_ssa_name (param, SSA_NAME_DEF_STMT (name));

		set_default_def (param, new_name);
		phi = create_phi_node (name, first);
		SSA_NAME_DEF_STMT (name) = phi;
		add_phi_arg (phi, new_name, single_pred_edge (first));
	      }
	  phis_constructed = true;
	}

      if (act->add && !a_acc)
	{
	  ret_type = TREE_TYPE (DECL_RESULT (current_function_decl));

	  tmp = create_tmp_var (ret_type, "add_acc");
	  add_referenced_var (tmp);

	  phi = create_phi_node (tmp, first);
	  add_phi_arg (phi,
		       /* RET_TYPE can be a float when -ffast-maths is
			  enabled.  */
		       fold_convert (ret_type, integer_zero_node),
		       single_pred_edge (first));
	  a_acc = PHI_RESULT (phi);
	}

      if (act->mult && !m_acc)
	{
	  ret_type = TREE_TYPE (DECL_RESULT (current_function_decl));

	  tmp = create_tmp_var (ret_type, "mult_acc");
	  add_referenced_var (tmp);

	  phi = create_phi_node (tmp, first);
	  add_phi_arg (phi,
		       /* RET_TYPE can be a float when -ffast-maths is
			  enabled.  */
		       fold_convert (ret_type, integer_one_node),
		       single_pred_edge (first));
	  m_acc = PHI_RESULT (phi);
	}
    }


  for (; tailcalls; tailcalls = next)
    {
      next = tailcalls->next;
      changed |= optimize_tail_call (tailcalls, opt_tailcalls);
      free (tailcalls);
    }

  if (a_acc || m_acc)
    {
      /* Modify the remaining return statements.  */
      FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR->preds)
	{
	  stmt = last_stmt (e->src);

	  if (stmt
	      && gimple_code (stmt) == GIMPLE_RETURN)
	    adjust_return_value (e->src, m_acc, a_acc);
	}
    }

  if (changed)
    free_dominance_info (CDI_DOMINATORS);

  if (phis_constructed)
    add_virtual_phis ();
  if (changed)
    return TODO_cleanup_cfg | TODO_update_ssa_only_virtuals;
  return 0;
}

static unsigned int
execute_tail_recursion (void)
{
  return tree_optimize_tail_calls_1 (false);
}

static bool
gate_tail_calls (void)
{
  return flag_optimize_sibling_calls != 0 && dbg_cnt (tail_call);
}

static unsigned int
execute_tail_calls (void)
{
  return tree_optimize_tail_calls_1 (true);
}

struct tree_opt_pass pass_tail_recursion = 
{
  "tailr",				/* name */
  gate_tail_calls,			/* gate */
  execute_tail_recursion,		/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa,	/* todo_flags_finish */
  0					/* letter */
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
  PROP_cfg | PROP_ssa | PROP_alias,	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_verify_ssa,	/* todo_flags_finish */
  0					/* letter */
};
