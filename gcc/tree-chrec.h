/* Chains of recurrences.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef GCC_TREE_CHREC_H
#define GCC_TREE_CHREC_H

/* Debugging macros.  Put an A after the macro definition if you want the 
   debug informations.  */
#define DBG_S(A) 

/* Accessors for the chains of recurrences.  */
#define CHREC_LEFT(NODE)          TREE_OPERAND (NODE, 0)
#define CHREC_RIGHT(NODE)         TREE_OPERAND (NODE, 1)
#define CHREC_CST(NODE)           TREE_OPERAND (CONSTANT_CHREC_CHECK (NODE), 0)
#define CHREC_VARIABLE(NODE)      TREE_INT_CST_LOW (TREE_TYPE (NODE))
#define CHREC_PERIOD(NODE)        TREE_VEC_LENGTH (TREE_OPERAND (PERIODIC_CHREC_CHECK (NODE), 0))
#define CHREC_ELT_PERIOD(NODE, I) TREE_VEC_ELT (TREE_OPERAND (PERIODIC_CHREC_CHECK (NODE), 0), I)
#define CHREC_LOW(NODE)           TREE_OPERAND (INTERVAL_CHREC_CHECK (NODE), 0)
#define CHREC_UP(NODE)            TREE_OPERAND (INTERVAL_CHREC_CHECK (NODE), 1)



/* The following trees are unique elements.  Thus the comparison of another 
   element to these elements should be done on the pointer to these trees, 
   and not on their value.  */

extern tree chrec_not_analyzed_yet;
extern tree chrec_top;
extern tree chrec_bot;
extern tree chrec_symbolic_parameter;

static inline bool automatically_generated_chrec_p (tree);

/* After having added an automatically generated element, please
   include it in the following function.  */

static inline bool
automatically_generated_chrec_p (tree chrec)
{
  return (chrec == chrec_not_analyzed_yet 
	  || chrec == chrec_top
	  || chrec == chrec_bot
	  || chrec == chrec_symbolic_parameter);
}

/* The tree nodes aka. CHRECs.  */

static inline bool
tree_is_chrec (tree expr)
{
  if (TREE_CODE (expr) == INTERVAL_CHREC
      || TREE_CODE (expr) == POLYNOMIAL_CHREC
      || TREE_CODE (expr) == EXPONENTIAL_CHREC
      || TREE_CODE (expr) == PERIODIC_CHREC)
    return true;
  else
    return false;
}



/* Constructors.  */
static inline tree build_interval_chrec (tree, tree);
static inline tree build_polynomial_chrec (unsigned, tree, tree);
static inline tree build_exponential_chrec (unsigned, tree, tree);
static inline tree build_periodic_chrec (unsigned, tree);

/* Chrec folding functions.  */
extern tree chrec_fold (tree);
extern tree chrec_fold_plus (tree, tree);
extern tree chrec_fold_minus (tree, tree);
extern tree chrec_fold_multiply (tree, tree);

/* Operations.  */
extern void chrec_linearize_representation (tree, varray_type, varray_type);
extern void chrec_eval_next (varray_type, varray_type);
extern void chrec_factorize_univar_poly (tree, varray_type);
extern tree chrec_apply (unsigned, tree, tree);
extern tree replace_initial_condition (tree, tree);
extern tree update_initial_condition_to_origin (tree);
extern tree initial_condition (tree);
extern tree evolution_part_in_loop_num (tree, unsigned);
extern tree evolution_function_in_loop_num (tree, unsigned);
extern tree reset_evolution_in_loop (unsigned, tree, tree);
extern tree chrec_eval_next_init_cond (unsigned, tree);
extern tree chrec_merge (tree, tree);
extern tree build_polynomial_evolution_in_loop (unsigned, tree, tree);
extern tree build_exponential_evolution_in_loop (unsigned, tree, tree);
extern tree add_expr_to_loop_evolution (unsigned, tree, tree);
extern tree multiply_by_expr_the_loop_evolution (unsigned, tree, tree);
extern tree chrec_fold_automatically_generated_operands (tree, tree);

/* Observers.  */
extern bool is_multivariate_chrec (tree);
extern bool is_pure_sum_chrec (tree);
extern bool no_evolution_in_loop_p (tree, unsigned);
extern bool chrec_is_positive (tree);
extern bool chrec_is_negative (tree);
extern bool chrec_contains_symbols (tree);
extern bool chrec_contains_undetermined (tree);
extern bool chrec_contains_intervals (tree);
extern bool tree_contains_chrecs (tree);
extern bool evolution_function_is_affine_multivariate_p (tree);
extern bool evolution_function_is_univariate_p (tree);
static inline bool is_chrec (tree);
static inline bool chrec_zerop (tree);
static inline bool chrec_eq (tree, tree);
static inline bool ival_is_included_in (tree, tree);
static inline bool ival_is_equal_to (tree, tree);
static inline bool symbolic_parameter_expr_p (tree);
static inline bool evolution_function_is_constant_p (tree);
static inline bool evolution_function_is_affine_p (tree);
static inline bool chrec_should_remain_symbolic (tree);

/* Analyzers.  */
extern tree how_far_to_positive (unsigned, tree);
extern tree how_far_to_zero (unsigned, tree);
extern tree how_far_to_non_zero (unsigned, tree);
extern void analyze_overlapping_iterations (tree, tree, tree*, tree*); 



/* Constructors.  */

/* Build an interval.  */

static inline tree 
build_interval_chrec (tree low, 
		      tree up)
{
  if (automatically_generated_chrec_p (low)
      || automatically_generated_chrec_p (up))
    return chrec_fold_automatically_generated_operands (low, up);
  
  if (integer_zerop (tree_fold_int_minus (up, low)))
    return low;
  else
    return build (INTERVAL_CHREC, NULL_TREE, low, up);
}

/* Build a polynomial chain of recurrence.  */

static inline tree 
build_polynomial_chrec (unsigned loop_num, 
			tree left, 
			tree right)
{
  return build (POLYNOMIAL_CHREC, build_int_2 (loop_num, 0), left, right);
}

/* Build an exponential chain of recurrence.  */

static inline tree 
build_exponential_chrec (unsigned loop_num, 
			 tree left, 
			 tree right)
{
  return build (EXPONENTIAL_CHREC, build_int_2 (loop_num, 0), left, right);
}

/* Build a periodic chain of recurrence.  */

static inline tree 
build_periodic_chrec (unsigned loop_num, 
		      tree vec)
{
  return build (PERIODIC_CHREC, build_int_2 (loop_num, 0), vec);
}



/* Observers.  */

/* Determine whether the given tree is a chain of recurrence or not.  */

static inline bool 
is_chrec (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
    case EXPONENTIAL_CHREC:
    case PERIODIC_CHREC:
    case INTERVAL_CHREC:
      return true;
      
    default:
      return false;
    }
}

/* Determines whether CHREC is equal to zero.  */

static inline bool 
chrec_zerop (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (TREE_CODE (chrec) == INTEGER_CST)
    return integer_zerop (chrec);
  
  if (TREE_CODE (chrec) == INTERVAL_CHREC)
    return (integer_zerop (CHREC_LOW (chrec))
	    && integer_zerop (CHREC_UP (chrec)));
  
  return false;
}

/* Determines whether CHREC_A == CHREC_B.  */

static inline bool
chrec_eq (tree chrec_a, 
	  tree chrec_b)
{
  return chrec_zerop (chrec_fold_minus (chrec_a, chrec_b));
}

/* This answers to the question "is CHREC1 included or equal to CHREC2", 
   where CHREC1 and CHREC2 are intervals.  */

static inline bool 
ival_is_included_in (tree chrec1, 
		     tree chrec2)
{
  int low_diff_sgn, up_diff_sgn;
  
#if defined ENABLE_CHECKING
  if (chrec1 == NULL_TREE
      || chrec2 == NULL_TREE
      || (TREE_CODE (chrec1) != INTERVAL_CHREC
	  && TREE_CODE (chrec1) != INTEGER_CST)
      || (TREE_CODE (chrec2) != INTERVAL_CHREC
	  && TREE_CODE (chrec2) != INTEGER_CST))
    abort ();
#endif
  
  if (TREE_CODE (chrec1) == INTEGER_CST)
    {
      tree low_diff, up_diff;
      
      low_diff = tree_fold_int_minus (chrec1, CHREC_LOW (chrec2));
      if (TREE_CODE (low_diff) == INTEGER_CST)
	low_diff_sgn = tree_int_cst_sgn (low_diff);
      else
	return false;
      
      up_diff = tree_fold_int_minus (chrec1, CHREC_UP (chrec2));
      if (TREE_CODE (up_diff) == INTEGER_CST)
	up_diff_sgn = tree_int_cst_sgn (up_diff);
      else 
	return false;
    }
  else if (TREE_CODE (chrec2) == INTEGER_CST)
    {
      tree low_diff, up_diff;
      
      low_diff = tree_fold_int_minus (CHREC_LOW (chrec1), chrec2);
      if (TREE_CODE (low_diff) == INTEGER_CST)
	low_diff_sgn = tree_int_cst_sgn (low_diff);
      else
	return false;
      
      up_diff = tree_fold_int_minus (CHREC_UP (chrec1), chrec2);
      if (TREE_CODE (up_diff) == INTEGER_CST)
	up_diff_sgn = tree_int_cst_sgn (up_diff);
      else 
	return false;
    }
  else
    {
      tree low_diff, up_diff;
      
      low_diff = tree_fold_int_minus (CHREC_LOW (chrec1), CHREC_LOW (chrec2));
      if (TREE_CODE (low_diff) == INTEGER_CST)
	low_diff_sgn = tree_int_cst_sgn (low_diff);
      else
	return false;
      
      up_diff = tree_fold_int_minus (CHREC_UP (chrec1), CHREC_UP (chrec2));
      if (TREE_CODE (up_diff) == INTEGER_CST)
	up_diff_sgn = tree_int_cst_sgn (up_diff);
      else 
	return false;
    }
  
  if ((low_diff_sgn == 1 || low_diff_sgn == 0)
      && (up_diff_sgn == -1 || up_diff_sgn == 0))
    return true;
  else
    return false;
}

/* Checks whether two intervals are equal: 
   (CHREC1 == CHREC2) ? true : false.  */

static inline bool
ival_is_equal_to (tree chrec1, 
		  tree chrec2)
{
#if defined ENABLE_CHECKING
  if (chrec1 == NULL_TREE
      || chrec2 == NULL_TREE
      || TREE_CODE (chrec1) != INTERVAL_CHREC
      || TREE_CODE (chrec2) != INTERVAL_CHREC)
    abort ();
#endif
  
  if (integer_zerop (tree_fold_int_minus (CHREC_LOW (chrec1), 
					  CHREC_LOW (chrec2)))
      && integer_zerop (tree_fold_int_minus (CHREC_UP (chrec1), 
					     CHREC_UP (chrec2))))
    return true;
  else
    return false;
}

/* Determines whether the expression CHREC is a symbolic parameter.
   Be aware of the fact that the expression is supposed to be part of
   an evolution function, and not an expression from the AST of the
   program.
   
   A symbolic parameter is matches the following pattern: "a variable
   that does not have a loop-phi node", this variable is either a loop
   invariant, or a secondary induction variable.
*/

static inline bool 
symbolic_parameter_expr_p (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (chrec == chrec_symbolic_parameter
      || TREE_CODE (chrec) == VAR_DECL
      || TREE_CODE (chrec) == PARM_DECL
      || TREE_CODE (chrec) == SSA_NAME)
    return true;
  
  return false;
}

/* Determines whether the expression CHREC is a constant.  Be aware of
   the fact that the expression is supposed to be part of an evolution
   function, and not an expression from the AST of the program.  */

static inline bool 
evolution_function_is_constant_p (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC
      || TREE_CODE (chrec) == EXPONENTIAL_CHREC
      || TREE_CODE (chrec) == PERIODIC_CHREC)
    return false;
  
  return true;
}

/* Determine whether the given tree is an affine evolution function or not.  */

static inline bool 
evolution_function_is_affine_p (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      if (evolution_function_is_constant_p (CHREC_LEFT (chrec))
	  && evolution_function_is_constant_p (CHREC_RIGHT (chrec)))
	return true;
      else
	return false;
      
    case EXPONENTIAL_CHREC:
    case PERIODIC_CHREC:
    case INTERVAL_CHREC:
    default:
      return false;
    }
}

/* Determines which expressions are simpler to be {handled | kept} in a 
   symbolic form.  */

static inline bool
chrec_should_remain_symbolic (tree evfun)
{
  if (evfun == NULL_TREE
      || evfun == chrec_top)
    return true;
  
  return false;
}

/* Determines whether EXPR does not contains chrec expressions.  */

static inline bool
tree_does_not_contain_chrecs (tree expr)
{
  return !tree_contains_chrecs (expr);
}

#endif  /* GCC_TREE_CHREC_H  */
