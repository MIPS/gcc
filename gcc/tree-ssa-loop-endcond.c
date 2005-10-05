/* Loop exit test optimizations.
   Copyright (C) 2005 Free Software Foundation, Inc.
   
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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* This file implements optimizations with goal to transform the loop so that
   testing of the ending condition is as cheap as possible.  Concretely:

   -- loop reversal.  Loops are reversed if
      1) they can be
	   -- data dependences are OK
	   -- number of iterations of the loop can be determined
      2) it won't spoil the code
	   -- on targets where traversing arrays in forward direction is more
	      profitable, we do not make more arrays to be traversed backwards.
	   -- initial values of ivs for the new loop are not too costly to
	      compute.
      3) it is usefull
           -- there is an induction variable whose initial value is a constant,
	      such that this iv may be used in the exit test.  And there is no
	      iv whose final value would be a nicer constant such that this iv
	      is used for anything else but the exit test.

   -- condition shape selection.  We try to transform the condition so that it
      performs compare with zero, which often is less expensive, or make the
      value compared with simpler:
      -- if we compare an induction variable X with value BASE - STEP * i
         ((signed) STEP > 0), whose final value V satisfies
	 -STEP <= V < 0 and whose initial value satisfies (signed) BASE >= 0,
	 we use (signed) X >= 0 compare.
      -- otherwise we use X != V compare.  */

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
#include "timevar.h"
#include "cfgloop.h"
#include "varray.h"
#include "expr.h"
#include "tree-pass.h"
#include "ggc.h"
#include "insn-config.h"
#include "recog.h"
#include "hashtab.h"
#include "tree-chrec.h"
#include "tree-scalar-evolution.h"
#include "cfgloop.h"
#include "params.h"
#include "langhooks.h"
#include "tree-data-ref.h"
#include "lambda.h"

/* Returns period of induction variable with step STEP.  */

static tree
iv_period (tree step)
{
  tree period, type, pow2div;

  gcc_assert (step && TREE_CODE (step) == INTEGER_CST);

  /* Period of the iv is gcd (step, type range).  Since type range is power
     of two, it suffices to determine the maximum power of two that divides
     step.  */
  pow2div = num_ending_zeros (step);
  type = unsigned_type_for (TREE_TYPE (step));

  period = build_low_bits_mask (type,
				(TYPE_PRECISION (type)
				 - tree_low_cst (pow2div, 1)));

  return period;
}

/* Check whether iv of LOOP with step STEP may wraps around the range of
   the type.  */

static bool
may_wrap_around_type_range_p (struct loop *loop, tree step)
{
  tree unsigned_type = unsigned_type_for (TREE_TYPE (step));
  tree range = iv_period (build_int_cst_type (unsigned_type, 1));

  step = fold_convert (unsigned_type, step);
  if (tree_int_cst_sign_bit (step))
    step = fold_build1 (NEGATE_EXPR, unsigned_type, step);
  if (tree_int_cst_sign_bit (step))
    return true;
 
  /* We need to check whether niter * step <= range.  */
  range = fold_build2 (FLOOR_DIV_EXPR, unsigned_type, range, step);

  return !loop_iterations_at_most_p (loop, range, NULL_TREE);
}

/* Given exit condition of LOOP based on induction variable BASE + STEP * i,
   whose final value is *BOUND, try altering comparison operator *CMP so
   that the *BOUND can be replaced by zero.  Return true if this succeeds,
   false otherwise.  */

static bool
modify_to_compare_with_zero (struct loop *loop,
			     tree base, tree step,
			     enum tree_code *cmp, tree *bound)
{
  tree type = TREE_TYPE (step), signed_type = signed_type_for (type);
  tree unsigned_type = unsigned_type_for (type);
  tree signed_bound = fold_convert (signed_type, *bound);
  tree signed_step = fold_convert (signed_type, step);
  tree signed_base = fold_convert (signed_type, base);
  tree signed_zero = build_int_cst_type (signed_type, 0);
  tree nit_bound;
  enum tree_code ok_cmp = ERROR_MARK;

  if (may_wrap_around_type_range_p (loop, step))
    return false;

  if (tree_int_cst_sign_bit (step))
    {
      ok_cmp = GE_EXPR;

      step = fold_build1 (NEGATE_EXPR, type, step);
      if (tree_int_cst_sign_bit (step))
	{
	  /* step = 0x7ffff... Rather avoid doing anything.  Nobody uses such
	     an iv, anyway.  */
	  return false;
	}

      /* Condition in shape base - step * i.  Check whether
	 -step <= bound < 0.  */
      if (!nonzero_p (fold_build2 (LE_EXPR, boolean_type_node,
				   signed_step, signed_bound))
	  || !nonzero_p (fold_build2 (LT_EXPR, boolean_type_node,
				      signed_bound, signed_zero)))
	return false;


      /* We can apply the optimization, assuming that the initial value
	 of the counter (signed_base) is >= the final value (signed_bound).  */
      if (condition_holds_in_loop_p (loop,
				     fold_build2 (GE_EXPR, boolean_type_node,
						  signed_base, signed_bound)))
	goto ok;

      /* The other way how to check this is using the fact that
	 signed_base = signed_bound + step * niter, and to prove that

	 niter <= (signed_max - bound) / step.  */
      nit_bound = fold_convert (unsigned_type,
				upper_bound_in_type (signed_type, signed_type));
      step = fold_convert (unsigned_type, step);
      nit_bound = fold_build2 (PLUS_EXPR, unsigned_type,
			       nit_bound,
			       fold_convert (unsigned_type, *bound));
      nit_bound = fold_build2 (FLOOR_DIV_EXPR, unsigned_type, nit_bound, step);
      if (loop_iterations_at_most_p (loop, nit_bound, NULL_TREE))
	goto ok;
    }
  else
    {
      ok_cmp = LE_EXPR;
      /* Condition in shape base + step * i.  This case does not appear as
	 often in practice, but let's try anyway.  Check whether
	 0 < bound <= step.  */
      if (!nonzero_p (fold_build2 (LT_EXPR, boolean_type_node,
				   signed_zero, signed_bound))
	  || !nonzero_p (fold_build2 (LE_EXPR, boolean_type_node,
				      signed_bound, signed_step)))
	return false;

      if (condition_holds_in_loop_p (loop,
				     fold_build2 (LE_EXPR, boolean_type_node,
						  signed_base, signed_bound)))
	goto ok;

      /* The other way how to check this is using the fact that
	 signed_base = signed_bound - step * niter, and to prove that

	 niter <= (bound - signed_min) / step.  */
      nit_bound = fold_convert (unsigned_type,
				lower_bound_in_type (signed_type, signed_type));
      step = fold_convert (unsigned_type, step);
      nit_bound = fold_build2 (MINUS_EXPR, unsigned_type,
			       fold_convert (unsigned_type, *bound),
			       nit_bound);
      nit_bound = fold_build2 (FLOOR_DIV_EXPR, unsigned_type, nit_bound, step);
      if (loop_iterations_at_most_p (loop, nit_bound, NULL_TREE))
	goto ok;
    }
 
  return false;

ok:
  *cmp = ok_cmp;
  *bound = signed_zero;
  return true;
}

/* Determines the final value of iv with base BASE and step STEP, in loop
   that iterates NITER times.  */

static tree
final_value_of_iv (tree base, tree step, tree niter)
{
  tree type = TREE_TYPE (base);

  return fold_build2 (PLUS_EXPR, type, base,
		      fold_build2 (MULT_EXPR, type, step,
				   fold_convert (type, niter)));
}

/* Given an exit condition comparing an induction variable with value
   BASE + STEP * i, such that the condition should be true
   (false if EXIT_P is true) for exactly NITER iterations, choose a best shape
   for the comparison.  Store the value to compare with to BOUND and the
   comparison operator to CMP.  Signedness of BOUND and the variable may
   differ; in that case, the induction variable must be cast to the type of
   BOUND before comparing.
   
   Returns false if we are unable to find any suitable compare shape, true
   if we succeed.  */

bool
select_condition_shape (struct loop *loop, tree niter, tree base, tree step,
			bool exit_p, enum tree_code *cmp, tree *bound)
{
  tree nit_type = TREE_TYPE (niter), per_type, wider_type;
  tree period, final;

  /* To have chance to use this induction variable, its period must be at least
     as large as the number of iterations.  */
  period = iv_period (step);
  per_type = TREE_TYPE (period);
  if (TYPE_PRECISION (nit_type) < TYPE_PRECISION (per_type))
    wider_type = per_type;
  else
    wider_type = nit_type;

  if (!integer_nonzerop (fold_build2 (GE_EXPR, boolean_type_node,
				      fold_convert (wider_type, period),
				      fold_convert (wider_type, niter))))
    return false;

  /* Determine the final value of the iv.  */
  final = final_value_of_iv (base, step, niter);

  /* At this point, we know that we can express the condition as
     IV != FINAL.  */
  *bound = final;
  *cmp = NE_EXPR;

  /* If final value is zero, this is the ideal shape.  */
  if (zero_p (final))
    goto end;

  /* Try transforming the condition to a compare with zero.  */
  modify_to_compare_with_zero (loop, base, step, cmp, bound);

end:
  if (exit_p)
    *cmp = invert_tree_comparison (*cmp, false);

  return true;
}

/* Estimate cost of comparing with BOUND.  */

unsigned
compare_cost (tree bound)
{
  /* Prefer costants, and prefer zero even more.  */
  if (zero_p (bound))
    return 0;
  else if (TREE_CODE (bound) == INTEGER_CST)
    return 1;
  else
    return 2;
}

/* Returns true if it is possible to reverse the LOOP.  List of data references
   in LOOP is stored to DATAREFS.  Number of iterations is stored in NITER.  */

static bool
may_reverse_loop_p (struct loop *loop, varray_type *datarefs, tree *niter)
{
  edge exit = single_dom_exit (loop);
  varray_type dependence_relations;
  lambda_trans_matrix trans;
  bool ret = false;
  struct tree_niter_desc niter_desc;
  tree phi;

  /* Only consider innermost loops with just one exit.  Not strictly necessary,
     but it makes things simpler.  */
  if (loop->inner || !exit)
    return false;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nConsidering loop %d\n", loop->num);

  /* Latch should be empty.  Similarly, this condition is not strictly
     necessary, but keeps things simple.  */
  if (!empty_block_p (loop->latch)
      || !single_pred_p (loop->latch)
      || single_pred (loop->latch) != exit->src)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  FAILED: nonempty latch\n");
      return false;
    }

  /* We need to know # of iterations, and there should be no uses of values
     defined inside loop outside of it, unless the values are invariants of
     the loop.  */
  if (!number_of_iterations_exit (loop, exit, &niter_desc, false)
      || !zero_p (niter_desc.may_be_zero))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  FAILED: cannot determine precise number of iterations\n");
      return false;
    }
  *niter = niter_desc.niter;

  for (phi = phi_nodes (exit->dest); phi; phi = PHI_CHAIN (phi))
    {
      tree val = PHI_ARG_DEF_FROM_EDGE (phi, exit);

      if (is_gimple_reg (val)
	  && !expr_invariant_in_loop_p (loop, val))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  FAILED: value used outside loop\n");
	  return false;
	}
    }

  /* The iterations of the loop may comunicate only through bivs that can be
     reversed.  */
  for (phi = phi_nodes (loop->header); phi; phi = PHI_CHAIN (phi))
    {
      tree def = PHI_RESULT (phi), base, step;

      if (is_gimple_reg (def)
	  && !simple_iv (loop, phi, def, &base, &step, true))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  FAILED: non-biv phi node\n");
	  return false;
	}
    }

  VARRAY_GENERIC_PTR_INIT (*datarefs, 10, "datarefs");
  VARRAY_GENERIC_PTR_INIT (dependence_relations, 10, "dependence_relations");

  /* Check for problems with dependences.  */
  compute_data_dependences_for_loop (loop, true, datarefs,
				     &dependence_relations);
  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_data_dependence_relations (dump_file, dependence_relations);

  trans = lambda_trans_matrix_new (1, 1);
  LTM_MATRIX (trans)[0][0] = -1;

  if (lambda_transform_legal_p (trans, 1, dependence_relations))
    {
      ret = true;
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  SUCCESS: may be reversed\n");
    }
  else if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "  FAILED: dependence check failed\n");

  free_dependence_relations (dependence_relations);
  return ret;
}

/* Returns cost of basing exit condition for LOOP that exits after NITER
   iterations on induction variable with BASE and STEP.  If REVERSED
   is true, we reverse the variable first.  */

static unsigned
endcond_candidate_cost (struct loop *loop,
			tree base, tree step, tree niter, bool reversed)
{
  tree bound;
  enum tree_code cmp;

  if (reversed)
    {
      base = final_value_of_iv (base, step, niter);
      step = fold_build1 (NEGATE_EXPR, TREE_TYPE (step), step);
    }

  if (!select_condition_shape (loop, niter, base, step, true, &cmp, &bound))
    return 10;

  return compare_cost (bound);
}

/* Returns the best cost of ending condition of LOOP based on an index of one
   of DATAREFS.  NITER is number of iterations of the loop.  If REVERSED
   is true, cost for reversed loop is returned, otherwise cost for original
   loop is returned.  */

static unsigned
best_endcond_candidate_cost (struct loop *loop, varray_type datarefs,
			     tree niter, bool reversed)
{
  unsigned best = 10, act, i, it;
  tree base, step;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (datarefs, i);

      for (it = 0; it < DR_NUM_DIMENSIONS (dr); it++)
	{
	  tree chrec = DR_ACCESS_FN (dr, it);

	  if (TREE_CODE (chrec) != POLYNOMIAL_CHREC
	      || CHREC_VARIABLE (chrec) != (unsigned) loop->num)
	    continue;

	  base = CHREC_LEFT (chrec);
	  step = CHREC_RIGHT (chrec);
	  if (TREE_CODE (step) != INTEGER_CST
	      || tree_contains_chrecs (base, NULL)
	      || chrec_contains_symbols_defined_in_loop (base, loop->num))
	    continue;

	  act = endcond_candidate_cost (loop, base, step, niter, reversed);
	  if (act < best)
	    best = act;
	}
    }

  return best;
}

/* Returns difference between number of arrays that are traversed forwards
   and backwards in LOOP, according to DATAREFS.  */

static int
num_of_forward_traversals (struct loop *loop, varray_type datarefs)
{
  unsigned i, it;
  int n_forward = 0;

  /* Determine number of data references passed forwards/backwards
     in loop.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (datarefs, i);

      for (it = 0; it < DR_NUM_DIMENSIONS (dr); it++)
	{
	  tree chrec = DR_ACCESS_FN (dr, it);
	  tree tstride = evolution_part_in_loop_num (chrec, loop->num);
	  
	  if (tstride != NULL_TREE && !zero_p (tstride))
	    {
	      if (TREE_CODE (tstride) != INTEGER_CST)
		{
		  /* Let's be conservative, and assume everything we do not
		     understand goes forward.  */
		  n_forward++;
		}
	      else if (tree_int_cst_sign_bit (tstride))
		n_forward--;
	      else
		n_forward++;
	      break;
	    }
	}
    }

  return n_forward;
}

/* This can be redefined by md, for architectures that prefer the arrays to be
   traversed forwards (because of some simplistic hardware prefetching).  */

#ifndef PREFER_PASSING_MEMORY_FORWARDS
#define PREFER_PASSING_MEMORY_FORWARDS 0
#endif

/* Returns true if it seems to be profitable to reverse the direction
   in that loop iterates.  DATAREFS is the list of data references in
   loop.  NITER is the number of iterations of a loop.  */

static bool
profitable_to_reverse_loop_p (struct loop *loop, varray_type datarefs,
			      tree niter)
{
  unsigned orig_cost, reversed_cost;

  if (PREFER_PASSING_MEMORY_FORWARDS && datarefs)
    {
      int n_forward = num_of_forward_traversals (loop, datarefs);

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  %d forward traversals\n", n_forward);

      if (n_forward > 0)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  FAILED: prefer traversing forwards\n");
	  return false;
	}

      /* ??? Maybe we would like to reverse the loop just to get the arrays
	 traversed in the right direction?  */
    }

  /* Check whether reversing the loop enables us to use a better ending
     condition for it.  */
  orig_cost = best_endcond_candidate_cost (loop, datarefs, niter, false);
  reversed_cost = best_endcond_candidate_cost (loop, datarefs, niter, true);
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "  costs: %d orig, %d reversed\n",
	     orig_cost, reversed_cost);

  if (orig_cost <= reversed_cost)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  FAILED: no gain from reversing the loop\n");
      return false;
    }
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "  SUCCESS: profitable to reverse\n");
  return true;
}

/* Reverse the biv of LOOP defined in phi node PHI.  LOOP iterates NITER
   times.  */

static void
reverse_biv (struct loop *loop, tree phi, tree niter)
{
  block_stmt_iterator bsi_incr = bsi_last (single_dom_exit (loop)->src);
  edge entry = loop_preheader_edge (loop);
  edge latch = loop_latch_edge (loop);
  tree base, step, def = PHI_RESULT (phi), var = SSA_NAME_VAR (def);
  tree new_def, stmts;
  bool ok;
      
  ok = simple_iv (loop, phi, def, &base, &step, true);
  gcc_assert (ok);

  base = unshare_expr (final_value_of_iv (base, step, niter));
  base = force_gimple_operand (base, &stmts, true, var);
  if (stmts)
    bsi_insert_on_edge_immediate_loop (entry, stmts);

  SET_USE (PHI_ARG_DEF_PTR_FROM_EDGE (phi, entry), base);

  /* Create the increment.  */
  new_def = create_increment_stmt (&bsi_incr, false, def, MINUS_EXPR,
				   unshare_expr (step), var);
  SET_USE (PHI_ARG_DEF_PTR_FROM_EDGE (phi, latch), new_def);
}

/* Replace exit condition of LOOP by a test whether NITER iterations
   have elapsed.  */

static void
replace_exit_condition (struct loop *loop, tree niter)
{
  create_canonical_iv (loop, single_dom_exit (loop), niter);
}

/* Reverse LOOP.  NITER is number of its iterations.  */

static void
reverse_loop (struct loop *loop, tree niter)
{
  tree phi, var;

  /* Rewrite the phi nodes and adjust increments of the bivs.  */
  for (phi = phi_nodes (loop->header); phi; phi = PHI_CHAIN (phi))
    {
      var = PHI_RESULT (phi);
      if (!is_gimple_reg (var))
	continue;

      reverse_biv (loop, phi, niter);
    }

  replace_exit_condition (loop, niter);
}

/* Reverse LOOP if possible and profitable.  Returns true if LOOP was
   reversed.  */

static bool
loop_reversal (struct loop *loop)
{
  varray_type datarefs = NULL;
  tree niter;
  bool reversed = false;

  if (may_reverse_loop_p (loop, &datarefs, &niter)
      && profitable_to_reverse_loop_p (loop, datarefs, niter))
    {
      reverse_loop (loop, niter);
      reversed = true;
    }

  free_data_refs (datarefs);
  return reversed;
}

/* Try reversing the LOOPS, in case it gives a better ending condition.  */

void
tree_ssa_reverse_loops (struct loops *loops)
{
  unsigned i;
  struct loop *loop;
  bool changed = false;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (loop)
	changed |= loop_reversal (loop);
    }

  if (changed)
    scev_reset ();
}
