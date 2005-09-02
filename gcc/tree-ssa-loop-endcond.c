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
      -- if X = BASE + STEP * i and final value V is of shape V' + C, where
         0 < C <= STEP, and there is no overflow, then we use
	 X <= V' compare.
      -- otherwise we use X != V compare.
      
   TODO: implement loop reversal
	 addition removal in condition shape selection
	 use information about bounds on number of iterations in condition
	   shape selection  */

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

/* Given exit condition based on induction variable BASE + STEP * i,
   whose final value is *BOUND, try altering comparison operator *CMP so
   that the *BOUND can be replaced by zero.  Return true if this succeeds,
   false otherwise.  */

static bool
modify_to_compare_with_zero (tree base, tree step,
			     enum tree_code *cmp, tree *bound)
{
  tree type = TREE_TYPE (step), signed_type = signed_type_for (type);
  tree signed_bound = fold_convert (signed_type, *bound);
  tree signed_step = fold_convert (signed_type, step);
  tree signed_base = fold_convert (signed_type, base);
  tree signed_zero = build_int_cst_type (signed_type, 0);

  if (tree_int_cst_sign_bit (step))
    {
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
				      signed_bound, signed_zero))
	  /* We can apply the optimization, assuming that BASE >= 0.  */
	  || !nonzero_p (fold_build2 (GE_EXPR, boolean_type_node,
				      signed_base, signed_zero)))
	return false;

      *cmp = GE_EXPR;
    }
  else
    {
      /* Condition in shape base + step * i.  This case does not appear as
	 often in practice, but let's try anyway.  Check whether
	 0 < bound <= step.  */
      if (!nonzero_p (fold_build2 (LT_EXPR, boolean_type_node,
				   signed_zero, signed_bound))
	  || !nonzero_p (fold_build2 (LE_EXPR, boolean_type_node,
				      signed_bound, signed_step))
	  /* We can apply the optimization, assuming that BASE <= 0.  */
	  || !nonzero_p (fold_build2 (LE_EXPR, boolean_type_node,
				      signed_base, signed_zero)))
	return false;

      *cmp = LE_EXPR;
    }
  
  *bound = signed_zero;
  return true;
}

/* Given exit condition based on induction variable BASE + STEP * i,
   whose final value is *BOUND, try altering comparison operator *CMP and
   *BOUND so that *BOUND does not contain unnecessary additions.  Return
   true if this succeeds, false otherwise.  */

static bool
try_removing_addition_from_bound (tree base ATTRIBUTE_UNUSED,
				  tree step ATTRIBUTE_UNUSED,
				  enum tree_code *cmp ATTRIBUTE_UNUSED,
				  tree *bound ATTRIBUTE_UNUSED)
{
  /* NIY.  */
  return false;
}

/* Check whether iv with step STEP wraps around the range of the type
   after at most NITER iterations.  */

static bool
may_wrap_around_type_range_p (tree niter, tree step)
{
  tree unsigned_type = unsigned_type_for (TREE_TYPE (step));
  tree range = iv_period (build_int_cst_type (unsigned_type, 1));

  step = fold_convert (unsigned_type, step);
  if (tree_int_cst_sign_bit (step))
    step = fold_build1 (NEGATE_EXPR, unsigned_type, step);
  if (tree_int_cst_sign_bit (step))
    return true;
  niter = fold_convert (unsigned_type, niter);
 
  /* We need to check whether niter * step <= range.  */
  range = fold_build2 (FLOOR_DIV_EXPR, unsigned_type, range, step);
  return !nonzero_p (fold_build2 (LE_EXPR, boolean_type_node, niter, range));
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
select_condition_shape (tree niter, tree base, tree step,
			bool exit_p, enum tree_code *cmp, tree *bound)
{
  tree nit_type = TREE_TYPE (niter), per_type, wider_type;
  tree iv_type = TREE_TYPE (step);
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
  final = fold_build2 (PLUS_EXPR, iv_type, base,
		       fold_build2 (MULT_EXPR, iv_type, step,
				    fold_convert (iv_type, niter)));

  /* At this point, we know that we can express the condition as
     IV != FINAL.  */
  *bound = final;
  *cmp = NE_EXPR;

  /* If final value is zero, this is the ideal shape.  */
  if (zero_p (final))
    goto end;

  /* Check whether the variable wraps around the type range, i.e., whether
     niter * step >= range.  If this is the case, we cannot do any of the
     following optimizations.  */
  if (may_wrap_around_type_range_p (niter, step))
    goto end;

  /* Else try transforming the condition to a compare with zero.  */
  if (!modify_to_compare_with_zero (base, step, cmp, bound))
    {
      /* If everything else failed, try removing addition or subtraction
	 from the bound.  */
      try_removing_addition_from_bound (base, step, cmp, bound);
    }

end:
  if (exit_p)
    *cmp = invert_tree_comparison (*cmp, false);

  return true;
}
