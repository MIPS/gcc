/* Induction variable canonicalization.
   Copyright (C) 2004 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* This pass detects the loops that iterate a constant number of times,
   adds a canonical induction variable (step -1, tested against 0) 
   and replaces the exit test.  This enables the less powerful rtl
   level analysis to use this information.

   This might spoil the code in some cases (by increasing register pressure).
   Note that in the case the new variable is not needed, ivopts will get rid
   of it, so it might only be a problem when there are no other linear induction
   variables.  In that case the created optimization possibilities are likely
   to pay up.

   Additionally in case we detect that it is benefitial to unroll the
   loop completely, we do it right here to expose the optimization
   possibilities to the following passes.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "cfgloop.h"
#include "tree-pass.h"
#include "ggc.h"
#include "tree-fold-const.h"
#include "tree-chrec.h"
#include "tree-scalar-evolution.h"
#include "params.h"
#include "flags.h"
#include "tree-inline.h"

/* Bound on the number of iterations we try to evaluate.  */

#define MAX_ITERATIONS_TO_TRACK 1000

/* Determines a loop phi node of LOOP such that X is derived from it
   by a chain of operations with constants.  */

static tree
chain_of_csts_start (struct loop *loop, tree x)
{
  tree stmt = SSA_NAME_DEF_STMT (x);
  basic_block bb = bb_for_stmt (stmt);
  use_optype uses;

  if (!bb
      || !flow_bb_inside_loop_p (loop, bb))
    return NULL_TREE;
  
  if (TREE_CODE (stmt) == PHI_NODE)
    {
      if (bb == loop->header)
	return stmt;

      return NULL_TREE;
    }

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL_TREE;

  get_stmt_operands (stmt);
  if (NUM_VUSES (STMT_VUSE_OPS (stmt)) > 0)
    return NULL_TREE;
  if (NUM_VDEFS (STMT_VDEF_OPS (stmt)) > 0)
    return NULL_TREE;
  if (NUM_DEFS (STMT_DEF_OPS (stmt)) > 1)
    return NULL_TREE;
  uses = STMT_USE_OPS (stmt);
  if (NUM_USES (uses) != 1)
    return NULL_TREE;

  return chain_of_csts_start (loop, USE_OP (uses, 0));
}

/* Determines whether X is derived from a value of a phi node in LOOP
   such that

   * this derivation consists only from operations with constants
   * the initial value of the phi node is constant
   * its value in the next iteration can be derived from the current one
     by a chain of operations with constants.  */

static tree
get_base_for (struct loop *loop, tree x)
{
  tree phi, init, next;

  if (is_gimple_min_invariant (x))
    return x;

  phi = chain_of_csts_start (loop, x);
  if (!phi)
    return NULL_TREE;

  init = phi_element_for_edge (phi, loop_preheader_edge (loop))->def;
  next = phi_element_for_edge (phi, loop_latch_edge (loop))->def;

  if (TREE_CODE (next) != SSA_NAME)
    return NULL_TREE;

  if (!is_gimple_min_invariant (init))
    return NULL_TREE;

  if (chain_of_csts_start (loop, next) != phi)
    return NULL_TREE;

  return phi;
}

/* Evaluates value of X, provided that the value of the variable defined
   in the loop phi node from that X is derived by operations with constants
   is BASE.  */

static tree
get_val_for (tree x, tree base)
{
  tree stmt, *op, nx, val;
  use_optype uses;

  if (!x)
    return base;

  stmt = SSA_NAME_DEF_STMT (x);
  if (TREE_CODE (stmt) == PHI_NODE)
    return base;

  uses = STMT_USE_OPS (stmt);
  op = USE_OP_PTR (uses, 0);

  nx = *op;
  val = get_val_for (nx, base);
  *op = val;
  val = fold (TREE_OPERAND (stmt, 1));
  *op = nx;

  return val;
}

/* Tries to count the number of iterations of LOOP till it exits by EXIT
   by brute force.  */

static tree
loop_niter_by_eval (struct loop *loop, edge exit)
{
  tree cond, cnd, acnd;
  tree op[2], val[2], next[2], aval[2], phi[2];
  unsigned i, j;
  enum tree_code cmp;

  cond = last_stmt (exit->src);
  if (!cond || TREE_CODE (cond) != COND_EXPR)
    return chrec_top;

  cnd = COND_EXPR_COND (cond);
  if (exit->flags & EDGE_TRUE_VALUE)
    cnd = invert_truthvalue (cnd);

  cmp = TREE_CODE (cnd);
  switch (cmp)
    {
    case EQ_EXPR:
    case NE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case LT_EXPR:
    case LE_EXPR:
      for (j = 0; j < 2; j++)
	op[j] = TREE_OPERAND (cnd, j);
      break;

    default:
      return chrec_top;
    }

  for (j = 0; j < 2; j++)
    {
      phi[j] = get_base_for (loop, op[j]);
      if (!phi[j])
	return chrec_top;
    }

  for (j = 0; j < 2; j++)
    {
      if (TREE_CODE (phi[j]) == PHI_NODE)
	{
	  val[j] = phi_element_for_edge (phi[j],
					 loop_preheader_edge (loop))->def;
	  next[j] = phi_element_for_edge (phi[j],
					  loop_latch_edge (loop))->def;
	}
      else
	{
	  val[j] = phi[j];
	  next[j] = NULL_TREE;
	  op[j] = NULL_TREE;
	}
    }

  for (i = 0; i < MAX_ITERATIONS_TO_TRACK; i++)
    {
      for (j = 0; j < 2; j++)
	aval[j] = get_val_for (op[j], val[j]);

      acnd = fold (build (cmp, boolean_type_node, aval[0], aval[1]));
      if (integer_zerop (acnd))
	{
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file,
		     "Proved that loop %d iterates %d times using brute force.\n",
		     loop->num, i);
	  return build_int_2 (i, 0);
	}

      for (j = 0; j < 2; j++)
	val[j] = get_val_for (next[j], val[j]);
    }

  return chrec_top;
}

/* Finds the exit of the LOOP by that the loop exits after a constant
   number of iterations and stores it to *EXIT.  The iteration count
   is returned.  */

static tree
find_loop_niter_by_eval (struct loop *loop, edge *exit)
{
  unsigned n_exits, i;
  edge *exits = get_loop_exit_edges (loop, &n_exits);
  edge ex;
  tree niter = NULL_TREE, aniter;

  *exit = NULL;
  for (i = 0; i < n_exits; i++)
    {
      ex = exits[i];
      if (!just_once_each_iteration_p (loop, ex->src))
	continue;

      aniter = loop_niter_by_eval (loop, ex);
      if (TREE_CODE (aniter) != INTEGER_CST)
	continue;

      if (niter
	  && !integer_nonzerop (fold (build (LT_EXPR, boolean_type_node,
					     aniter, niter))))
	continue;

      niter = aniter;
      *exit = ex;
    }
  free (exits);

  return niter ? niter : chrec_top;
}

/* Adds a canonical induction variable to LOOP iterating NITER times.  EXIT
   is the exit edge whose condition is replaced.  */

static void
create_canonical_iv (struct loop *loop, edge exit, tree niter)
{
  edge in;
  tree cond, type, var;
  block_stmt_iterator incr_at;
  enum tree_code cmp;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Added canonical iv to loop %d, ", loop->num);
      print_generic_expr (tree_dump_file, niter, TDF_SLIM);
      fprintf (tree_dump_file, " iterations.\n");
    }

  cond = last_stmt (exit->src);
  in = exit->src->succ;
  if (in == exit)
    in = in->succ_next;

  type = TREE_TYPE (niter);
  niter = fold (build (PLUS_EXPR, type,
		       niter, convert (type, integer_one_node)));
  incr_at = bsi_last (in->src);
  create_iv (niter, convert (type, integer_minus_one_node), NULL_TREE, loop,
	     &incr_at, false, NULL, &var);

  cmp = (exit->flags & EDGE_TRUE_VALUE) ? EQ_EXPR : NE_EXPR;
  COND_EXPR_COND (cond) = build (cmp, boolean_type_node,
				 var, convert (type, integer_zero_node));
  modify_stmt (cond);
}

/* Computes an estimated number of insns in LOOP.  */

static unsigned
estimate_loop_size (struct loop *loop)
{
  basic_block *body = get_loop_body (loop);
  block_stmt_iterator bsi;
  unsigned size = 0, i;

  for (i = 0; i < loop->num_nodes; i++)
    for (bsi = bsi_start (body[i]); !bsi_end_p (bsi); bsi_next (&bsi))
      size += estimate_num_insns (bsi_stmt (bsi));
  free (body);

  return size;
}

/* Tries to unroll LOOP completely, i.e. NITER times.  LOOPS is the
   loop tree.  COMPLETELY_UNROLL is true if we should unroll the loop
   even if it may cause code growth.  EXIT is the exit of the loop
   that should be eliminated.  */

static bool
try_unroll_loop_completely (struct loops *loops, struct loop *loop,
			    edge exit, tree niter,
			    bool completely_unroll)
{
  tree max_unroll = build_int_2 (PARAM_VALUE (PARAM_MAX_COMPLETELY_PEEL_TIMES),
				 0);
  unsigned n_unroll, ninsns;
  tree cond, dont_exit, do_exit;

  if (loop->inner)
    return false;

  if (!integer_nonzerop (fold (build (LE_EXPR, boolean_type_node,
				      niter, max_unroll))))
    return false;
  n_unroll = tree_low_cst (niter, 1);

  if (n_unroll && !completely_unroll)
    return false;

  ninsns = estimate_loop_size (loop);

  if (n_unroll * ninsns
      > (unsigned) PARAM_VALUE (PARAM_MAX_COMPLETELY_PEELED_INSNS))
    return false;

  if (exit->flags & EDGE_TRUE_VALUE)
    {
      dont_exit = boolean_false_node;
      do_exit = boolean_true_node;
    }
  else
    {
      dont_exit = boolean_true_node;
      do_exit = boolean_false_node;
    }
  cond = last_stmt (exit->src);
    
  if (n_unroll)
    {
      if (!flag_unroll_loops)
	return false;

      COND_EXPR_COND (cond) = dont_exit;
      modify_stmt (cond);

      if (!tree_duplicate_loop_to_header_edge (loop, loop_preheader_edge (loop),
					       loops, n_unroll, NULL,
					       NULL, NULL, NULL, 0))
	return false;
    }
  
  COND_EXPR_COND (cond) = do_exit;
  modify_stmt (cond);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "Unrolled loop %d completely.\n", loop->num);

  return true;
}

/* Adds a canonical induction variable to LOOP if suitable.  LOOPS is the loops
   tree.  CREATE_IV is true if we may create a new iv.  COMPLETELY_UNROLL is
   true if we should do complete unrolling even if it may cause the code
   growth.  */

static void
canonicalize_loop_induction_variables (struct loops *loops, struct loop *loop,
				       bool create_iv, bool completely_unroll)
{
  edge exit = NULL;
  tree niter 
#if 0 /* Causes bootstrap to fail  */
	  = number_of_iterations_in_loop (loop);

  if (TREE_CODE (niter) == INTEGER_CST)
    exit = loop_exit_edge (loop, 0);
  else
    niter  
#endif
	  = find_loop_niter_by_eval (loop, &exit);

  if (TREE_CODE (niter) != INTEGER_CST)
    return;

  if (try_unroll_loop_completely (loops, loop, exit, niter, completely_unroll))
    return;

  if (create_iv)
    create_canonical_iv (loop, exit, niter);
}

/* The main entry point of the pass.  Adds canonical induction variables
   to the suitable LOOPS.  */

void
canonicalize_induction_variables (struct loops *loops)
{
  unsigned i;
  struct loop *loop;
  bool create_ivs = flag_unroll_loops || flag_branch_on_count_reg;
  bool completely_unroll_loops = flag_unroll_loops;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];

      if (loop)
	canonicalize_loop_induction_variables (loops, loop, create_ivs,
					       completely_unroll_loops);
    }
}
