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

/* This file implements operations on chains of recurrences.  Chains
   of recurrences are used for modeling evolution functions of scalar
   variables.
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "diagnostic.h"
#include "varray.h"
#include "tree-fold-const.h"
#include "tree-chrec.h"
#include "tree-pass.h"

/* Extended folder for chrecs.  */

static inline tree chrec_fold_plus_poly_cst (tree, tree);
static inline tree chrec_fold_plus_peel_cst (tree, tree);
static inline tree chrec_fold_plus_expo_cst (tree, tree);
static inline tree chrec_fold_plus_ival_cst (tree, tree);
static inline tree chrec_fold_plus_poly_poly (tree, tree);
static inline tree chrec_fold_plus_poly_peel (tree, tree);
static inline tree chrec_fold_plus_poly_expo (tree, tree);
static inline tree chrec_fold_plus_peel_peel (tree, tree);
static inline tree chrec_fold_plus_expo_expo (tree, tree);
static inline tree chrec_fold_plus_ival_ival (tree, tree);

static inline tree chrec_fold_multiply_poly_cst (tree, tree);
static inline tree chrec_fold_multiply_peel_cst (tree, tree);
static inline tree chrec_fold_multiply_expo_cst (tree, tree);
static inline tree chrec_fold_multiply_ival_cst (tree, tree);
static inline tree chrec_fold_multiply_poly_poly (tree, tree);
static inline tree chrec_fold_multiply_poly_peel (tree, tree);
static inline tree chrec_fold_multiply_poly_expo (tree, tree);
static inline tree chrec_fold_multiply_peel_peel (tree, tree);
static inline tree chrec_fold_multiply_expo_expo (tree, tree);
static inline tree chrec_fold_multiply_ival_ival (tree, tree);

/* Operations.  */
static tree chrec_evaluate (unsigned, tree, tree, tree);
static tree chrec_merge_intervals (tree, tree);
static tree remove_initial_condition (tree);
static tree add_expr_to_loop_evolution_2 (tree, tree*);
static tree add_expr_to_loop_evolution_1 (unsigned, tree, tree*);

/* Observers.  */
static bool is_multivariate_chrec_rec (tree, unsigned int);
static inline bool is_not_constant_evolution (tree);

/* Analyzers.  */
static inline bool ziv_subscript_p (tree, tree);
static bool siv_subscript_p (tree, tree);
static void analyze_ziv_subscript (tree, tree, tree*, tree*);
static void analyze_siv_subscript (tree, tree, tree*, tree*);
static void analyze_siv_subscript_cst_affine (tree, tree, tree*, tree*);
static void analyze_siv_subscript_affine_cst (tree, tree, tree*, tree*);
static void analyze_siv_subscript_affine_affine (tree, tree, tree*, tree*);
static bool chrec_steps_divide_constant_p (tree, tree);
static void analyze_miv_subscript (tree, tree, tree*, tree*);

static tree how_far_to_positive_for_affine_function (tree);
static tree how_far_to_positive_for_affine_function_scalar_scalar (tree, tree);
static tree how_far_to_positive_for_affine_function_scalar_ival (tree, tree, tree);
static tree how_far_to_positive_for_affine_function_ival_scalar (tree, tree, tree);
static tree how_far_to_positive_for_affine_function_ival_ival (tree, tree, tree, tree);


/* Basic arithmetics.  */
static int lcm (int, int);
static int gcd (int, int);
static inline int multiply_int (int, int);
static inline int divide_int (int, int);
static inline int add_int (int, int);
static inline int substract_int (int, int);
static inline bool integer_divides_p (int, int);



/* Multiply integers.  */

static inline int 
multiply_int (int a, int b)
{
  return a * b;
}

/* Divide integers.  */

static inline int 
divide_int (int a, int b)
{
  return a / b;
}

/* Add integers.  */

static inline int 
add_int (int a, int b)
{
  return a + b;
}

/* Substract integers.  */

static inline int 
substract_int (int a, int b)
{
  return a - b;
}

/* Determines whether a divides b.  */

static inline bool 
integer_divides_p (int a, int b)
{
  return (a == gcd (a, b));
}

/* Least common multiple.  */

static int 
lcm (int a, 
     int b)
{
  int pgcd;

  if (a == 1) 
    return b;

  if (b == 1) 
    return a;

  if (a == 0 || b == 0) 
    return 0;

  pgcd = gcd (a, b);

  if (pgcd == 1)
    return multiply_int (a, b);

  return pgcd * lcm (divide_int (a, pgcd), 
		     divide_int (b, pgcd));
}

/* Greatest common divisor.  */

static int 
gcd (int a, 
     int b)
{
  if (a == 0) 
    return b;
  
  if (b == 0) 
    return a;
  
  if (a < 0)
    a = multiply_int (a, -1);
  
  if (b < 0)
    b = multiply_int (b, -1);
  
  if (a == b)
    return a;
  
  if (a > b)
    return gcd (substract_int (a, b), b);

  /* if (b > a)  */
  return gcd (substract_int (b, a), a);
}



/* Extended folder for chrecs.  */

/* Determines whether CST is not a constant evolution.  */

static inline bool
is_not_constant_evolution (tree cst)
{
  return (TREE_CODE (cst) == POLYNOMIAL_CHREC
	  || TREE_CODE (cst) == EXPONENTIAL_CHREC
	  || TREE_CODE (cst) == PEELED_CHREC);
}

/* Fold the addition of a polynomial function and a constant.  */

static inline tree 
chrec_fold_plus_poly_cst (tree poly, 
			  tree cst)
{
#if defined ENABLE_CHECKING
  if (poly == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (poly) != POLYNOMIAL_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  return build_polynomial_chrec 
    (CHREC_VARIABLE (poly), 
     chrec_fold_plus (CHREC_LEFT (poly), cst),
     chrec_fold (CHREC_RIGHT (poly)));
}

/* Fold the addition of a peeled chrec and a constant.  */

static inline tree 
chrec_fold_plus_peel_cst (tree peel, 
			  tree cst)
{
#if defined ENABLE_CHECKING
  if (peel == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (peel) != PEELED_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  return build_peeled_chrec 
    (CHREC_VARIABLE (peel),
     chrec_fold_plus (CHREC_LEFT (peel), cst),
     chrec_fold_plus (CHREC_RIGHT (peel), cst));
}

/* Fold the addition of an exponential function and a constant.  */

static inline tree 
chrec_fold_plus_expo_cst (tree expo, 
			  tree cst)
{
#if defined ENABLE_CHECKING
  if (expo == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (expo) != EXPONENTIAL_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  /* For the moment, we don't know how to fold this further.  */
  return build (PLUS_EXPR, TREE_TYPE (cst), 
		chrec_fold (expo),
		chrec_fold (cst));
}

/* Fold the addition of an interval and a constant.  */

static inline tree 
chrec_fold_plus_ival_cst (tree ival, 
			  tree cst)
{
#if defined ENABLE_CHECKING
  if (ival == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (ival) != INTERVAL_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  /* Don't modify abstract values.  */
  if (ival == chrec_top)
    return chrec_top;
  if (ival == chrec_bot)
    return chrec_bot;
  
  return build_interval_chrec 
    (chrec_fold_plus (CHREC_LOW (ival), cst),
     chrec_fold_plus (CHREC_UP (ival), cst));
}

/* Fold the addition of two polynomial functions.  */

static inline tree 
chrec_fold_plus_poly_poly (tree poly0, 
			   tree poly1)
{
  tree left, right;
  
#if defined ENABLE_CHECKING
  if (poly0 == NULL_TREE
      || poly1 == NULL_TREE
      || TREE_CODE (poly0) != POLYNOMIAL_CHREC
      || TREE_CODE (poly1) != POLYNOMIAL_CHREC)
    abort ();
#endif
  
  /*
    {a, +, b}_1 + {c, +, d}_2  ->  {{a, +, b}_1 + c, +, d}_2,
    {a, +, b}_2 + {c, +, d}_1  ->  {{c, +, d}_1 + a, +, b}_2,
    {a, +, b}_x + {c, +, d}_x  ->  {a+c, +, b+d}_x.  */
  if (CHREC_VARIABLE (poly0) < CHREC_VARIABLE (poly1))
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly1), 
       chrec_fold_plus (poly0, CHREC_LEFT (poly1)),
       chrec_fold (CHREC_RIGHT (poly1)));
  
  if (CHREC_VARIABLE (poly0) > CHREC_VARIABLE (poly1))
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly0), 
       chrec_fold_plus (poly1, CHREC_LEFT (poly0)),
       chrec_fold (CHREC_RIGHT (poly0)));
  
  left = chrec_fold_plus (CHREC_LEFT (poly0), CHREC_LEFT (poly1));
  right = chrec_fold_plus (CHREC_RIGHT (poly0), CHREC_RIGHT (poly1));
  
  if (chrec_zerop (right))
    return left;
  else
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly0), left, right); 
}

/* Fold the addition of a polynomial and a peeled chrec.  */

static inline tree 
chrec_fold_plus_poly_peel (tree poly, 
			   tree peel)
{
#if defined ENABLE_CHECKING
  if (peel == NULL_TREE
      || poly == NULL_TREE
      || TREE_CODE (peel) != PEELED_CHREC
      || TREE_CODE (poly) != POLYNOMIAL_CHREC)
    abort ();
#endif
  
  /*
    {a, +, b}_1 + (c, d)_2  ->  ({a, +, b}_1 + c, {a, +, b}_1 + d)_2,
    {a, +, b}_2 + (c, d)_1  ->  {a + (c, d)_1, +, b}_2,
    {a, +, b}_x + (c, d)_x  ->  ({a, +, b}_x + c, {a, +, b}_x + d)_x.  */
  if (CHREC_VARIABLE (poly) < CHREC_VARIABLE (peel))
    return build_peeled_chrec 
      (CHREC_VARIABLE (peel), 
       chrec_fold_plus (poly, CHREC_LEFT (peel)),
       chrec_fold_plus (poly, CHREC_RIGHT (peel)));
  
  if (CHREC_VARIABLE (poly) > CHREC_VARIABLE (peel))
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly), 
       chrec_fold_plus (CHREC_LEFT (poly), peel),
       chrec_fold_plus (CHREC_RIGHT (poly), peel));
  
  return build_peeled_chrec 
    (CHREC_VARIABLE (peel), 
     chrec_fold_plus (poly, CHREC_LEFT (peel)),
     chrec_fold_plus (poly, CHREC_RIGHT (peel)));
}

/* Fold the addition of a polynomial and an exponential functions.  */

static inline tree 
chrec_fold_plus_poly_expo (tree poly, 
			   tree expo)
{
#if defined ENABLE_CHECKING
  if (expo == NULL_TREE
      || poly == NULL_TREE
      || TREE_CODE (expo) != EXPONENTIAL_CHREC
      || TREE_CODE (poly) != POLYNOMIAL_CHREC)
    abort ();
#endif
  
  /* For the moment, we don't know how to fold this further.  */
  return build (PLUS_EXPR, integer_type_node, 
		chrec_fold (expo),
		chrec_fold (poly));
}

/* Fold the addition of two peeled chrecs.  */

static inline tree 
chrec_fold_plus_peel_peel (tree peel0, 
			   tree peel1)
{
#if defined ENABLE_CHECKING
  if (peel0 == NULL_TREE
      || peel1 == NULL_TREE
      || TREE_CODE (peel0) != PEELED_CHREC
      || TREE_CODE (peel1) != PEELED_CHREC)
    abort ();
#endif
  
  /*
    (a, b)_1 + (c, d)_2  ->  ((a, b)_1 + c, (a, b)_1 + d)_2,
    (a, b)_2 + (c, d)_1  ->  (a + (c, d)_1, b + (c, d)_1)_2,
    (a, b)_x + (c, d)_x  ->  (a + c, b + d)_x.  */
  if (CHREC_VARIABLE (peel0) < CHREC_VARIABLE (peel1))
    return build_peeled_chrec 
      (CHREC_VARIABLE (peel1), 
       chrec_fold_plus (peel0, CHREC_LEFT (peel1)),
       chrec_fold_plus (peel0, CHREC_RIGHT (peel1)));
  
  if (CHREC_VARIABLE (peel0) > CHREC_VARIABLE (peel1))
    return build_peeled_chrec 
      (CHREC_VARIABLE (peel0), 
       chrec_fold_plus (CHREC_LEFT (peel0), peel1),
       chrec_fold_plus (CHREC_RIGHT (peel0), peel1));
  
  return build_peeled_chrec 
    (CHREC_VARIABLE (peel0), 
     chrec_fold_plus (CHREC_LEFT (peel0), CHREC_LEFT (peel1)),
     chrec_fold_plus (CHREC_RIGHT (peel0), CHREC_RIGHT (peel1)));
}

/* Fold the addition of two exponential functions.  */

static inline tree 
chrec_fold_plus_expo_expo (tree expo0, 
			   tree expo1)
{
#if defined ENABLE_CHECKING
  if (expo0 == NULL_TREE
      || expo1 == NULL_TREE
      || TREE_CODE (expo0) != EXPONENTIAL_CHREC
      || TREE_CODE (expo1) != EXPONENTIAL_CHREC)
    abort ();
#endif
  
  /* For the moment, we don't know how to fold this further.  */
  return build (PLUS_EXPR, integer_type_node, 
		chrec_fold (expo0), 
		chrec_fold (expo1));
}

/* Fold the addition of two intervals.  */

static inline tree
chrec_fold_plus_ival_ival (tree ival0,
			   tree ival1)
{
#if defined ENABLE_CHECKING
  if (ival0 == NULL_TREE
      || ival1 == NULL_TREE
      || TREE_CODE (ival0) != INTERVAL_CHREC
      || TREE_CODE (ival1) != INTERVAL_CHREC)
    abort ();
#endif
  
  /* Don't modify abstract values.  */
  if (ival0 == chrec_top
      || ival1 == chrec_top)
    return chrec_top;
  if (ival0 == chrec_bot
      || ival1 == chrec_bot)
    return chrec_bot;
  
  /* [a, b] + [c, d]  ->  [a+c, b+d].  */
  return build_interval_chrec 
    (chrec_fold_plus (CHREC_LOW (ival0), CHREC_LOW (ival1)),
     chrec_fold_plus (CHREC_UP (ival0), CHREC_UP (ival1)));
}



/* Fold the multiplication of a polynomial function and a
   constant.  */

static inline tree 
chrec_fold_multiply_poly_cst (tree poly, 
			      tree cst)
{
#if defined ENABLE_CHECKING
  if (poly == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (poly) != POLYNOMIAL_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  return build_polynomial_chrec 
    (CHREC_VARIABLE (poly), 
     chrec_fold_multiply (CHREC_LEFT (poly), cst),
     chrec_fold_multiply (CHREC_RIGHT (poly), cst));
}

/* Fold the multiplication of a peeled chrec and a constant.  */

static inline tree 
chrec_fold_multiply_peel_cst (tree peel, 
			      tree cst)
{
#if defined ENABLE_CHECKING
  if (peel == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (peel) != PEELED_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  return build_peeled_chrec 
    (CHREC_VARIABLE (peel),
     chrec_fold_multiply (CHREC_LEFT (peel), cst),
     chrec_fold_multiply (CHREC_RIGHT (peel), cst));
}

/* Fold the multiplication of an exponential function and a
   constant.  */

static inline tree 
chrec_fold_multiply_expo_cst (tree expo, 
			      tree cst)
{
#if defined ENABLE_CHECKING
  if (expo == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (expo) != EXPONENTIAL_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  return build_exponential_chrec 
    (CHREC_VARIABLE (expo),
     chrec_fold_multiply (CHREC_LEFT (expo), cst),
     chrec_fold (CHREC_RIGHT (expo)));
}

/* Fold the multiplication of an interval and a constant.  */

static inline tree 
chrec_fold_multiply_ival_cst (tree ival, 
			      tree cst)
{
#if defined ENABLE_CHECKING
  if (ival == NULL_TREE
      || cst == NULL_TREE
      || TREE_CODE (ival) != INTERVAL_CHREC
      || is_not_constant_evolution (cst))
    abort ();
#endif
  
  /* Don't modify abstract values.  */
  if (ival == chrec_top)
    return chrec_top;
  if (ival == chrec_bot)
    return chrec_bot;
  
  return build_interval_chrec 
    (chrec_fold_multiply (CHREC_LOW (ival), cst),
     chrec_fold_multiply (CHREC_UP (ival), cst));
}

/* Fold the multiplication of two polynomial functions.  */

static inline tree 
chrec_fold_multiply_poly_poly (tree poly0, 
			       tree poly1)
{
#if defined ENABLE_CHECKING
  if (poly0 == NULL_TREE
      || poly1 == NULL_TREE
      || TREE_CODE (poly0) != POLYNOMIAL_CHREC
      || TREE_CODE (poly1) != POLYNOMIAL_CHREC)
    abort ();
#endif
  
  /* {a, +, b}_1 * {c, +, d}_2  ->  {c*{a, +, b}_1, +, d}_2,
     {a, +, b}_2 * {c, +, d}_1  ->  {a*{c, +, d}_1, +, b}_2,
     {a, +, b}_x * {c, +, d}_x  ->  {a*c, +, a*d + b*c + b*d, +, 2*b*d}_x.  */
  if (CHREC_VARIABLE (poly0) < CHREC_VARIABLE (poly1))
    /* poly0 is a constant wrt. poly1.  */
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly1), 
       chrec_fold_multiply (CHREC_LEFT (poly1), poly0),
       chrec_fold (CHREC_RIGHT (poly1)));
  
  if (CHREC_VARIABLE (poly1) < CHREC_VARIABLE (poly0))
    /* poly1 is a constant wrt. poly0.  */
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly0), 
       chrec_fold_multiply (CHREC_LEFT (poly0), poly1),
       chrec_fold (CHREC_RIGHT (poly0)));
  
  /* poly0 and poly1 are two polynomials in the same variable,
     {a, +, b}_x * {c, +, d}_x  ->  {a*c, +, a*d + b*c + b*d, +, 2*b*d}_x.  */
  return 
    build_polynomial_chrec 
    (CHREC_VARIABLE (poly0), 
     build_polynomial_chrec 
     (CHREC_VARIABLE (poly0), 
      
      /* "a*c".  */
      chrec_fold_multiply (CHREC_LEFT (poly0), CHREC_LEFT (poly1)),
      
      /* "a*d + b*c + b*d".  */
      chrec_fold_plus 
      (chrec_fold_multiply (CHREC_LEFT (poly0), CHREC_RIGHT (poly1)),
       
       chrec_fold_plus 
       (chrec_fold_multiply (CHREC_RIGHT (poly0), CHREC_LEFT (poly1)),
	chrec_fold_multiply (CHREC_RIGHT (poly0), CHREC_RIGHT (poly1))))),
     
     /* "2*b*d".  */
     chrec_fold_multiply
     (build_int_2 (2, 0),
      chrec_fold_multiply (CHREC_RIGHT (poly0), CHREC_RIGHT (poly1))));
}

/* Fold the addition of a polynomial and a peeled chrec.  */

static inline tree 
chrec_fold_multiply_poly_peel (tree poly, 
			       tree peel)
{
#if defined ENABLE_CHECKING
  if (peel == NULL_TREE
      || poly == NULL_TREE
      || TREE_CODE (peel) != PEELED_CHREC
      || TREE_CODE (poly) != POLYNOMIAL_CHREC)
    abort ();
#endif
  
  /*
    {a, +, b}_1 * (c, d)_2  ->  ({a, +, b}_1 * c, {a, +, b}_1 * d)_2,
    {a, +, b}_2 * (c, d)_1  ->  {a * (c, d)_1, +, b}_2,
    {a, +, b}_x * (c, d)_x  ->  ({a, +, b}_x * c, {a, +, b}_x * d)_x.  */
  if (CHREC_VARIABLE (poly) < CHREC_VARIABLE (peel))
    return build_peeled_chrec 
      (CHREC_VARIABLE (peel), 
       chrec_fold_multiply (poly, CHREC_LEFT (peel)),
       chrec_fold_multiply (poly, CHREC_RIGHT (peel)));
  
  if (CHREC_VARIABLE (poly) > CHREC_VARIABLE (peel))
    return build_polynomial_chrec 
      (CHREC_VARIABLE (poly), 
       chrec_fold_multiply (CHREC_LEFT (poly), peel),
       chrec_fold_multiply (CHREC_RIGHT (poly), peel));
  
  return build_peeled_chrec 
    (CHREC_VARIABLE (peel), 
     chrec_fold_multiply (poly, CHREC_LEFT (peel)),
     chrec_fold_multiply (poly, CHREC_RIGHT (peel)));
}

/* Fold the multiplication of a polynomial and an exponential
   functions.  */

static inline tree 
chrec_fold_multiply_poly_expo (tree poly, 
			       tree expo)
{
#if defined ENABLE_CHECKING
  if (expo == NULL_TREE
      || poly == NULL_TREE
      || TREE_CODE (expo) != EXPONENTIAL_CHREC
      || TREE_CODE (poly) != POLYNOMIAL_CHREC)
    abort ();
#endif
  
  /* For the moment, we don't know how to fold this further.  */
  return build (MULT_EXPR, integer_type_node, 
		chrec_fold (expo),
		chrec_fold (poly));
}

/* Fold the addition of two peeled chrecs.  */

static inline tree 
chrec_fold_multiply_peel_peel (tree peel0, 
			       tree peel1)
{
#if defined ENABLE_CHECKING
  if (peel0 == NULL_TREE
      || peel1 == NULL_TREE
      || TREE_CODE (peel0) != PEELED_CHREC
      || TREE_CODE (peel1) != PEELED_CHREC)
    abort ();
#endif
  
  /*
    (a, b)_1 * (c, d)_2  ->  ((a, b)_1 * c, (a, b)_1 * d)_2,
    (a, b)_2 * (c, d)_1  ->  (a * (c, d)_1, b * (c, d)_1)_2,
    (a, b)_x * (c, d)_x  ->  (a * c, b * d)_x.  */
  if (CHREC_VARIABLE (peel0) < CHREC_VARIABLE (peel1))
    return build_peeled_chrec 
      (CHREC_VARIABLE (peel1), 
       chrec_fold_multiply (peel0, CHREC_LEFT (peel1)),
       chrec_fold_multiply (peel0, CHREC_RIGHT (peel1)));
  
  if (CHREC_VARIABLE (peel0) > CHREC_VARIABLE (peel1))
    return build_peeled_chrec 
      (CHREC_VARIABLE (peel0), 
       chrec_fold_multiply (CHREC_LEFT (peel0), peel1),
       chrec_fold_multiply (CHREC_RIGHT (peel0), peel1));
  
  return build_peeled_chrec 
    (CHREC_VARIABLE (peel0), 
     chrec_fold_multiply (CHREC_LEFT (peel0), CHREC_LEFT (peel1)),
     chrec_fold_multiply (CHREC_RIGHT (peel0), CHREC_RIGHT (peel1)));
}

/* Fold the multiplication of two exponential functions.  */

static inline tree 
chrec_fold_multiply_expo_expo (tree expo0, 
			       tree expo1)
{
#if defined ENABLE_CHECKING
  if (expo0 == NULL_TREE
      || expo1 == NULL_TREE
      || TREE_CODE (expo0) != EXPONENTIAL_CHREC
      || TREE_CODE (expo1) != EXPONENTIAL_CHREC)
    abort ();
#endif
  
  if (CHREC_VARIABLE (expo0) < CHREC_VARIABLE (expo1))
    /* expo0 is a constant wrt. expo1.  */
    return build_exponential_chrec 
      (CHREC_VARIABLE (expo1), 
       chrec_fold_multiply (CHREC_LEFT (expo1), expo0),
       chrec_fold (CHREC_RIGHT (expo1)));
  
  if (CHREC_VARIABLE (expo1) < CHREC_VARIABLE (expo0))
    /* expo1 is a constant wrt. expo0.  */
    return build_exponential_chrec
      (CHREC_VARIABLE (expo0), 
       chrec_fold_multiply (CHREC_LEFT (expo0), expo1),
       chrec_fold (CHREC_RIGHT (expo0)));
  
  return build_exponential_chrec 
    (CHREC_VARIABLE (expo0), 
     chrec_fold_multiply (CHREC_LEFT (expo0), CHREC_LEFT (expo1)),
     chrec_fold_multiply (CHREC_RIGHT (expo0), CHREC_RIGHT (expo1)));
}

/* Fold the multiplication of two intervals.  */

static inline tree
chrec_fold_multiply_ival_ival (tree ival0,
			       tree ival1)
{
  tree ac, ad, bc, bd;
  
#if defined ENABLE_CHECKING
  if (ival0 == NULL_TREE
      || ival1 == NULL_TREE
      || TREE_CODE (ival0) != INTERVAL_CHREC
      || TREE_CODE (ival1) != INTERVAL_CHREC)
    abort ();
#endif
  
  /* Don't modify abstract values.  */
  if (automatically_generated_chrec_p (ival0)
      || automatically_generated_chrec_p (ival1))
    chrec_fold_automatically_generated_operands (ival0, ival1);
  
  ac = tree_fold_int_multiply (CHREC_LOW (ival0), CHREC_LOW (ival1));
  ad = tree_fold_int_multiply (CHREC_LOW (ival0), CHREC_UP (ival1));
  bc = tree_fold_int_multiply (CHREC_UP (ival0), CHREC_LOW (ival1));
  bd = tree_fold_int_multiply (CHREC_UP (ival0), CHREC_UP (ival1));
  
  /* [a, b] * [c, d]  ->  [min (ac, ad, bc, bd), max (ac, ad, bc, bd)],
     for reference, see Moore's "Interval Arithmetic".  */
  return build_interval_chrec 
    (tree_fold_int_min 
     (tree_fold_int_min
      (tree_fold_int_min (ac, ad), bc), bd),
     tree_fold_int_max
     (tree_fold_int_max
      (tree_fold_int_max (ac, ad), bc), bd));
}

/* When the operands are automatically_generated_chrec_p, the fold has
   to respect the semantics of the operands.  */

tree 
chrec_fold_automatically_generated_operands (tree op0, 
					     tree op1)
{
  /* TOP op x = TOP,
     x op TOP = TOP.  */
  if (op0 == chrec_top
      || op1 == chrec_top)
    return chrec_top;
  
  /* BOT op TOP = TOP, 
     TOP op BOT = TOP, 
     BOT op x = BOT,
     x op BOT = BOT.  */
  if (op0 == chrec_bot
      || op1 == chrec_bot)
    return chrec_bot;
  
  if (op0 == chrec_not_analyzed_yet
      || op1 == chrec_not_analyzed_yet)
    return chrec_not_analyzed_yet;
  
  /* The default case produces a safe result. */
  return chrec_top;
}

/* Folds chrec expressions.  */

tree 
chrec_fold (tree chrec)
{
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      return build_polynomial_chrec 
	(CHREC_VARIABLE (chrec), 
	 chrec_fold (CHREC_LEFT (chrec)),
	 chrec_fold (CHREC_RIGHT (chrec)));
      
    case EXPONENTIAL_CHREC:
      return build_exponential_chrec 
	(CHREC_VARIABLE (chrec), 
	 chrec_fold (CHREC_LEFT (chrec)),
	 chrec_fold (CHREC_RIGHT (chrec)));
      
    case INTERVAL_CHREC:
      if (automatically_generated_chrec_p (chrec))
	return chrec;
      else
	return build_interval_chrec 
	  (chrec_fold (CHREC_LOW (chrec)),
	   chrec_fold (CHREC_UP (chrec)));
      
    case PLUS_EXPR:
      return chrec_fold_plus 
	(TREE_OPERAND (chrec, 0), 
	 TREE_OPERAND (chrec, 1));
      
    case MINUS_EXPR:
      return chrec_fold_minus 
	(TREE_OPERAND (chrec, 0), 
	 TREE_OPERAND (chrec, 1));
      
    case MULT_EXPR:
      return chrec_fold_multiply 
	(TREE_OPERAND (chrec, 0), 
	 TREE_OPERAND (chrec, 1));
      
    default:
      /* Do not risk a folding.  */
      return chrec;
    }
}

/* Fold the addition of two chrecs.  */

tree
chrec_fold_plus (tree op0,
		 tree op1)
{
  if (automatically_generated_chrec_p (op0)
      || automatically_generated_chrec_p (op1))
    return chrec_fold_automatically_generated_operands (op0, op1);
  
  switch (TREE_CODE (op0))
    {
    case INTEGER_CST:
      if (integer_zerop (op0))
	return op1;
      
    case REAL_CST:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case SSA_NAME:
    case VAR_DECL:
    case PARM_DECL:
      switch (TREE_CODE (op1))
	{
	case POLYNOMIAL_CHREC:
	  return chrec_fold_plus_poly_cst (op1, op0);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_plus_expo_cst (op1, op0);
	  
	case PEELED_CHREC:
	  return chrec_fold_plus_peel_cst (op1, op0);
	  
	case INTERVAL_CHREC:
	  return chrec_fold_plus_ival_cst (op1, op0);
	  
	case INTEGER_CST:
	  if (integer_zerop (op1))
	    return op0;
	  
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	  if (tree_does_not_contain_chrecs (op0)
	      && tree_does_not_contain_chrecs (op1)
	      && TREE_CODE (TREE_TYPE (op0)) != REAL_TYPE
	      && TREE_CODE (TREE_TYPE (op0)) != REAL_TYPE)
	    return tree_fold_int_plus (op0, op1);
	  
	case REAL_CST:
	default:
	  return build (PLUS_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    case POLYNOMIAL_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_zerop (op1))
	    return op0;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTERVAL_CHREC:
	  return chrec_fold_plus_poly_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_plus_poly_poly (op0, op1);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_plus_poly_expo (op0, op1);
	  
	case PEELED_CHREC:
	  return chrec_fold_plus_poly_peel (op0, op1);
	  
	default:
	  return build (PLUS_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    case EXPONENTIAL_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_zerop (op1))
	    return op0;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTERVAL_CHREC:
	  return chrec_fold_plus_expo_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_plus_poly_expo (op1, op0);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_plus_expo_expo (op0, op1);
	  
	default:
	  return build (PLUS_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}

    case PEELED_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_zerop (op1))
	    return op0;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTERVAL_CHREC:
	  return chrec_fold_plus_peel_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_plus_poly_peel (op1, op0);
	  
	case PEELED_CHREC:
	  return chrec_fold_plus_peel_peel (op0, op1);
	  
	case EXPONENTIAL_CHREC:
	default:
	  return build (PLUS_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    case INTERVAL_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_zerop (op1))
	    return op0;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	  return chrec_fold_plus_ival_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_plus_poly_cst (op1, op0);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_plus_expo_cst (op1, op0);
	  
	case INTERVAL_CHREC:
	  return chrec_fold_plus_ival_ival (op0, op1);
	  
	default:
	  return build (PLUS_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    default:
      return build (PLUS_EXPR, TREE_TYPE (op0), 
		    chrec_fold (op0), 
		    chrec_fold (op1));
    }
}

/* Fold the multiplication of two chrecs.  */

tree
chrec_fold_multiply (tree op0,
		     tree op1)
{
  if (automatically_generated_chrec_p (op0)
      || automatically_generated_chrec_p (op1))
    return chrec_fold_automatically_generated_operands (op0, op1);
  
  switch (TREE_CODE (op0))
    {
    case INTEGER_CST:
      if (integer_onep (op0))
	return op1;
      if (integer_zerop (op0))
	return integer_zero_node;
      
    case REAL_CST:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case SSA_NAME:
    case VAR_DECL:
    case PARM_DECL:
      switch (TREE_CODE (op1))
	{
	case POLYNOMIAL_CHREC:
	  return chrec_fold_multiply_poly_cst (op1, op0);
	  
	case PEELED_CHREC:
	  return chrec_fold_multiply_peel_cst (op1, op0);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_multiply_expo_cst (op1, op0);
	  
	case INTERVAL_CHREC:
	  return chrec_fold_multiply_ival_cst (op1, op0);
	  
	case INTEGER_CST:
	  if (integer_onep (op1))
	    return op0;
	  if (integer_zerop (op1))
	    return integer_zero_node;
	  
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	  /* testsuite/.../ssa-chrec-45.c.  */
	  if (tree_does_not_contain_chrecs (op0)
	      && tree_does_not_contain_chrecs (op1)
	      && TREE_CODE (TREE_TYPE (op0)) != REAL_TYPE
	      && TREE_CODE (TREE_TYPE (op0)) != REAL_TYPE)
	    return tree_fold_int_multiply (op0, op1);
	  
	case REAL_CST:
	default:
	  return build (MULT_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    case POLYNOMIAL_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_onep (op1))
	    return op0;
	  if (integer_zerop (op1))
	    return integer_zero_node;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTERVAL_CHREC:
	  return chrec_fold_multiply_poly_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_multiply_poly_poly (op0, op1);
	  
	case PEELED_CHREC:
	  return chrec_fold_multiply_poly_peel (op0, op1);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_multiply_poly_expo (op0, op1);
	  
	default:
	  return build (MULT_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}

    case PEELED_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_onep (op1))
	    return op0;
	  if (integer_zerop (op1))
	    return integer_zero_node;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTERVAL_CHREC:
	  return chrec_fold_multiply_peel_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_multiply_poly_peel (op1, op0);
	  
	case PEELED_CHREC:
	  return chrec_fold_multiply_peel_peel (op0, op1);
	  
	default:
	  return build (MULT_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    case EXPONENTIAL_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_onep (op1))
	    return op0;
	  if (integer_zerop (op1))
	    return integer_zero_node;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTERVAL_CHREC:
	  return chrec_fold_multiply_expo_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_multiply_poly_expo (op1, op0);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_multiply_expo_expo (op0, op1);
	  
	default:
	  return build (MULT_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    case INTERVAL_CHREC:
      switch (TREE_CODE (op1))
	{
	case INTEGER_CST:
	  if (integer_onep (op1))
	    return op0;
	  
	  if (integer_zerop (op1))
	    return integer_zero_node;
	  
	case REAL_CST:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	  return chrec_fold_multiply_ival_cst (op0, op1);
	  
	case POLYNOMIAL_CHREC:
	  return chrec_fold_multiply_poly_cst (op1, op0);
	  
	case PEELED_CHREC:
	  return chrec_fold_multiply_peel_cst (op1, op0);
	  
	case EXPONENTIAL_CHREC:
	  return chrec_fold_multiply_expo_cst (op1, op0);
	  
	case INTERVAL_CHREC:
	  return chrec_fold_multiply_ival_ival (op0, op1);
	  
	default:
	  return build (MULT_EXPR, TREE_TYPE (op1), 
			chrec_fold (op0), 
			chrec_fold (op1));
	}
      
    default:
      return build (MULT_EXPR, TREE_TYPE (op0), 
		    chrec_fold (op0), 
		    chrec_fold (op1));
    }
}

/* Fold the substraction of two chrecs.  */

tree 
chrec_fold_minus (tree op0, 
		  tree op1)
{
  if (automatically_generated_chrec_p (op0)
      || automatically_generated_chrec_p (op1))
    return chrec_fold_automatically_generated_operands (op0, op1);
  
  if (integer_zerop (op1)
      || (TREE_CODE (op1) == INTERVAL_CHREC
	  && integer_zerop (CHREC_LOW (op1))
	  && integer_zerop (CHREC_UP (op1))))
    return op0;
  
  if (tree_does_not_contain_chrecs (op1))
    {
      if (tree_does_not_contain_chrecs (op0))
	{
	  if (TREE_CODE (op0) == INTEGER_CST
	      && TREE_CODE (op1) == INTEGER_CST)
	    return tree_fold_int_minus (op0, op1);
	  else
	    return build (MINUS_EXPR, TREE_TYPE (op0), op0, op1);
	}
      
      else
	switch (TREE_CODE (op0))
	  {
	  case INTERVAL_CHREC:
	    return build_interval_chrec 
	      (chrec_fold_minus (CHREC_LOW (op0), op1), 
	       chrec_fold_minus (CHREC_UP (op0), op1));
	    
	  case POLYNOMIAL_CHREC:
	    return build_polynomial_chrec 
	      (CHREC_VARIABLE (op0), 
	       chrec_fold_minus (CHREC_LEFT (op0), op1), 
	       CHREC_RIGHT (op0));
	    
	  case EXPONENTIAL_CHREC:
	  default:
	    return chrec_top;
	  }
    }
  
  else
    return chrec_fold_plus 
      (op0, chrec_fold_multiply (op1, integer_minus_one_node));
}



/* Operations.  */

/* Try to recognize the nature of the peeled chrec and to transform it
   into a more specific chrec: polynomial, periodic, ...  For example,
   this function transforms (6, {9, +, 3}_1)_1 into {6, +, 3}_1.  */

tree
simplify_peeled_chrec (tree peel)
{
  tree res = peel;
  
  return res;
}

/* Helper function.  Use the Newton's interpolating formula for
   evaluating the value of the evolution function.  */

static tree 
chrec_evaluate (unsigned var,
		tree chrec,
		tree n,
		tree k)
{
  tree binomial_n_k = tree_fold_int_binomial (n, k);
  
  if (TREE_CODE (chrec) == EXPONENTIAL_CHREC
      && CHREC_VARIABLE (chrec) == var)
    return chrec_top;
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC)
    {
      if (CHREC_VARIABLE (chrec) > var)
	return chrec_evaluate (var, CHREC_LEFT (chrec), n, k);
      
      if (CHREC_VARIABLE (chrec) == var)
	return chrec_fold_plus 
	  (chrec_fold_multiply (binomial_n_k,
				CHREC_LEFT (chrec)),
	   chrec_evaluate (var, CHREC_RIGHT (chrec), n, 
			   tree_fold_int_plus (k, integer_one_node)));
      
      return chrec_fold_multiply (binomial_n_k, chrec);
    }
  else
    return chrec_fold_multiply (binomial_n_k, chrec);
}

/* Evaluates "CHREC (X)" when the varying variable is VAR.  
   Example:  Given the following parameters, 
   
   var = 1
   chrec = {5, +, {3, +, 4}_1}_1
   x = 10
   
   The result is given by the Newton's interpolating formula: 
   5 * \binom{10}{0} + 3 * \binom{10}{1} + 4 * \binom{10}{2}.
*/

tree 
chrec_apply (unsigned var,
	     tree chrec, 
	     tree x)
{
  tree res = chrec_top;

#if defined ENABLE_CHECKING 
  if (chrec == NULL_TREE
      || x == NULL_TREE)
    abort ();
#endif

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(chrec_apply \n");
  
  if (evolution_function_is_affine_p (chrec))
    {
      /* "{a, +, b} (x)"  ->  "a + b*x".  */
      if (TREE_CODE (CHREC_LEFT (chrec)) == INTEGER_CST
	  && integer_zerop (CHREC_LEFT (chrec)))
	res = chrec_fold_multiply (CHREC_RIGHT (chrec), x);
      
      else
	res = chrec_fold_plus (CHREC_LEFT (chrec), 
			       chrec_fold_multiply (CHREC_RIGHT (chrec), x));
    }
  
  else if (TREE_CODE (chrec) != POLYNOMIAL_CHREC
	   && TREE_CODE (chrec) != EXPONENTIAL_CHREC)
    res = chrec;
  
  else if (TREE_CODE (x) == INTEGER_CST
	   && tree_int_cst_sgn (x) == 1)
    /* testsuite/.../ssa-chrec-38.c.  */
    res = chrec_evaluate (var, chrec, x, integer_zero_node);
  
  else
    res = chrec_top;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (var = %d\n", var);
      fprintf (tree_dump_file, ")\n  (chrec = ");
      print_generic_expr (tree_dump_file, chrec, 0);
      fprintf (tree_dump_file, ")\n  (x = ");
      print_generic_expr (tree_dump_file, x, 0);
      fprintf (tree_dump_file, ")\n  (res = ");
      print_generic_expr (tree_dump_file, res, 0);
      fprintf (tree_dump_file, "))\n");
    }
  
  return res;
}

/* Replaces the initial condition in CHREC with INIT_COND.  */

tree 
chrec_replace_initial_condition (tree chrec, 
				 tree init_cond)
{
#if defined ENABLE_CHECKING 
  if (chrec == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      return build_polynomial_chrec 
	(CHREC_VARIABLE (chrec),
	 chrec_replace_initial_condition (CHREC_LEFT (chrec), init_cond),
	 CHREC_RIGHT (chrec));
      
    case EXPONENTIAL_CHREC:
      return build_exponential_chrec
	(CHREC_VARIABLE (chrec),
	 chrec_replace_initial_condition (CHREC_LEFT (chrec), init_cond),
	 CHREC_RIGHT (chrec));
      
    default:
      return init_cond;
    }
}

/* Updates the initial condition to the origin, ie. zero for a 
   polynomial function, and one for an exponential function.  */

tree 
update_initial_condition_to_origin (tree chrec)
{
#if defined ENABLE_CHECKING 
  if (chrec == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      if (TREE_CODE (CHREC_LEFT (chrec)) == POLYNOMIAL_CHREC
	  || TREE_CODE (CHREC_LEFT (chrec)) == EXPONENTIAL_CHREC)
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec),
	   update_initial_condition_to_origin (CHREC_LEFT (chrec)),
	   CHREC_RIGHT (chrec));
      else
	return build_polynomial_chrec
	  (CHREC_VARIABLE (chrec),
	   integer_zero_node,
	   CHREC_RIGHT (chrec));
      
    case EXPONENTIAL_CHREC:
      if (TREE_CODE (CHREC_LEFT (chrec)) == POLYNOMIAL_CHREC
	  || TREE_CODE (CHREC_LEFT (chrec)) == EXPONENTIAL_CHREC)
	return build_exponential_chrec
	  (CHREC_VARIABLE (chrec),
	   integer_one_node,
	   CHREC_RIGHT (chrec));
      else
	return build_polynomial_chrec
	  (CHREC_VARIABLE (chrec),
	   integer_zero_node,
	   CHREC_RIGHT (chrec));
      
    default:
      return chrec_top;
    }
}

/* Returns the initial condition of a given CHREC.  */

tree 
initial_condition (tree chrec)
{
#if defined ENABLE_CHECKING 
  if (chrec == NULL_TREE)
    abort ();
#endif
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC
      || TREE_CODE (chrec) == EXPONENTIAL_CHREC)
    return initial_condition (CHREC_LEFT (chrec));
  else
    return chrec;
}

/* Returns a univariate function that represents the evolution in
   LOOP_NUM.  */

tree 
evolution_function_in_loop_num (tree chrec, 
				unsigned loop_num)
{
#if defined ENABLE_CHECKING 
  if (chrec == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec) == loop_num)
	return build_polynomial_chrec 
	  (loop_num, 
	   evolution_function_in_loop_num (CHREC_LEFT (chrec), loop_num), 
	   CHREC_RIGHT (chrec));
      
      else if (CHREC_VARIABLE (chrec) < loop_num)
	/* There is no evolution in this loop.  */
	return initial_condition (chrec);
      
      else
	return evolution_function_in_loop_num (CHREC_LEFT (chrec), loop_num);
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec) == loop_num)
	return build_exponential_chrec 
	  (loop_num,
	   evolution_function_in_loop_num (CHREC_LEFT (chrec), loop_num),
	   CHREC_RIGHT (chrec));
      
      else if (CHREC_VARIABLE (chrec) < loop_num)
	/* There is no evolution in this loop.  */
	return initial_condition (chrec);
      
      else
	return evolution_function_in_loop_num (CHREC_LEFT (chrec), loop_num);
      
    default:
      return chrec;
    }
}

/* Returns the evolution part in LOOP_NUM.  Example: the call
   get_evolution_in_loop (1, {{0, +, 1}_1, +, 2}_1) returns 
   {1, +, 2}_1  */

tree 
evolution_part_in_loop_num (tree chrec, 
			    unsigned loop_num)
{
#if defined ENABLE_CHECKING 
  if (chrec == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec) == loop_num)
	{
	  if (TREE_CODE (CHREC_LEFT (chrec)) != POLYNOMIAL_CHREC
	      || CHREC_VARIABLE (CHREC_LEFT (chrec)) != CHREC_VARIABLE (chrec))
	    return CHREC_RIGHT (chrec);
	  
	  else
	    return build_polynomial_chrec
	      (loop_num, 
	       evolution_part_in_loop_num (CHREC_LEFT (chrec), loop_num), 
	       CHREC_RIGHT (chrec));
	}
      
      else if (CHREC_VARIABLE (chrec) < loop_num)
	/* There is no evolution part in this loop.  */
	return NULL_TREE;
      
      else
	return evolution_part_in_loop_num (CHREC_LEFT (chrec), loop_num);
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec) == loop_num)
	{
	  if (TREE_CODE (CHREC_LEFT (chrec)) != EXPONENTIAL_CHREC
	      || CHREC_VARIABLE (CHREC_LEFT (chrec)) != CHREC_VARIABLE (chrec))
	    return CHREC_RIGHT (chrec);
	  
	  else
	    return build_exponential_chrec 
	      (loop_num,
	       evolution_part_in_loop_num (CHREC_LEFT (chrec), loop_num),
	       CHREC_RIGHT (chrec));
	}
      
      else if (CHREC_VARIABLE (chrec) < loop_num)
	/* There is no evolution part in this loop.  */
	return NULL_TREE;
      
      else
	return evolution_part_in_loop_num (CHREC_LEFT (chrec), loop_num);
      
    default:
      return NULL_TREE;
    }
}

/* Set or reset the evolution of CHREC to NEW_EVOL in loop LOOP_NUM.
   This function is essentially used for setting the evolution to
   chrec_top, for example after having determined that it is
   impossible to say how many times a loop will execute.  */

tree 
reset_evolution_in_loop (unsigned loop_num,
			 tree chrec, 
			 tree new_evol)
{
  if ((TREE_CODE (chrec) == POLYNOMIAL_CHREC
       || TREE_CODE (chrec) == EXPONENTIAL_CHREC)
      && CHREC_VARIABLE (chrec) > loop_num)
    return build 
      (TREE_CODE (chrec), 
       build_int_2 (CHREC_VARIABLE (chrec), 0), 
       reset_evolution_in_loop (loop_num, CHREC_LEFT (chrec), new_evol), 
       reset_evolution_in_loop (loop_num, CHREC_RIGHT (chrec), new_evol));
  
  while ((TREE_CODE (chrec) == POLYNOMIAL_CHREC
	  || TREE_CODE (chrec) == EXPONENTIAL_CHREC)
	 && CHREC_VARIABLE (chrec) == loop_num)
    chrec = CHREC_LEFT (chrec);
  
  return build_polynomial_chrec (loop_num, chrec, new_evol);
}


/* Returns the value of the variable after one execution of the loop
   LOOP_NB, supposing that CHREC is the evolution function of the
   variable.
   
   Example:  
   chrec_eval_next_init_cond (4, {{1, +, 3}_2, +, 10}_4) = 11.  */

tree 
chrec_eval_next_init_cond (unsigned loop_nb, 
			   tree chrec)
{
  tree init_cond;
  
  init_cond = initial_condition (chrec);

  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC
      || TREE_CODE (chrec) == EXPONENTIAL_CHREC)
    {
      if (CHREC_VARIABLE (chrec) < loop_nb)
	/* There is no evolution in this dimension.  */
	return init_cond;
      
      while ((TREE_CODE (CHREC_LEFT (chrec)) == POLYNOMIAL_CHREC
	      || TREE_CODE (CHREC_LEFT (chrec)) == EXPONENTIAL_CHREC)
	     && CHREC_VARIABLE (CHREC_LEFT (chrec)) >= loop_nb)
	chrec = CHREC_LEFT (chrec);
      
      if (CHREC_VARIABLE (chrec) != loop_nb)
	/* There is no evolution in this dimension.  */
	return init_cond;
      
      if (TREE_CODE (chrec) == POLYNOMIAL_CHREC)
	/* testsuite/.../ssa-chrec-14.c */
	return chrec_fold_plus (init_cond, 
				initial_condition (CHREC_RIGHT (chrec)));
      
      else
	return chrec_fold_multiply (init_cond, 
				    initial_condition (CHREC_RIGHT (chrec)));
    }
  
  else
    return init_cond;
}

/* Merge the information contained in two intervals. 
   merge ([a, b], [c, d])  ->  [min (a, c), max (b, d)],
   merge ([a, b], c)       ->  [min (a, c), max (b, c)],
   merge (a, [b, c])       ->  [min (a, b), max (a, c)],
   merge (a, b)            ->  [min (a, b), max (a, b)].  */

static tree 
chrec_merge_intervals (tree chrec1, 
		       tree chrec2)
{
#if defined ENABLE_CHECKING
  if (chrec1 == NULL_TREE 
      || chrec2 == NULL_TREE)
    abort ();
#endif
  
  /* Don't modify abstract values.  */
  if (chrec1 == chrec_top
      || chrec2 == chrec_top)
    return chrec_top;
  if (chrec1 == chrec_bot 
      || chrec2 == chrec_bot)
    return chrec_bot;
  if (chrec1 == chrec_not_analyzed_yet)
    return chrec2;
  if (chrec2 == chrec_not_analyzed_yet)
    return chrec1;
  
  if (TREE_CODE (chrec1) == INTERVAL_CHREC)
    {
      if (TREE_CODE (chrec2) == INTERVAL_CHREC)
	return build_interval_chrec 
	  (tree_fold_int_min (CHREC_LOW (chrec1), CHREC_LOW (chrec2)),
	   tree_fold_int_max (CHREC_UP (chrec1), CHREC_UP (chrec2)));
      else
	return build_interval_chrec 
	  (tree_fold_int_min (CHREC_LOW (chrec1), chrec2),
	   tree_fold_int_max (CHREC_UP (chrec1), chrec2));
    }
  else if (TREE_CODE (chrec2) == INTERVAL_CHREC)
    return build_interval_chrec 
      (tree_fold_int_min (CHREC_LOW (chrec2), chrec1),
       tree_fold_int_max (CHREC_UP (chrec2), chrec1));
  else
    return build_interval_chrec 
      (tree_fold_int_min (chrec1, chrec2),
       tree_fold_int_max (chrec1, chrec2));
}

/* Merges two evolution functions that were found by following two
   alternate paths of a conditional expression.  */

tree
chrec_merge (tree chrec1, 
	     tree chrec2)
{
#if defined ENABLE_CHECKING
  if (chrec1 == NULL_TREE 
      || chrec2 == NULL_TREE)
    abort ();
#endif
  
  if (chrec1 == chrec_not_analyzed_yet)
    return chrec2;
  if (chrec2 == chrec_not_analyzed_yet)
    return chrec1;
  
  switch (TREE_CODE (chrec1))
    {
    case REAL_CST:
    case SSA_NAME:
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
      switch (TREE_CODE (chrec2))
	{
	case REAL_CST:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTEGER_CST:
	case INTERVAL_CHREC:
	  return chrec_merge_intervals (chrec1, chrec2);
	  
	case POLYNOMIAL_CHREC:
	  return build_polynomial_chrec 
	    (CHREC_VARIABLE (chrec2),
	     chrec_merge (chrec1, CHREC_LEFT (chrec2)),
	     chrec_merge (integer_zero_node, CHREC_RIGHT (chrec2)));
	  
	case EXPONENTIAL_CHREC:
	  return build_exponential_chrec 
	    (CHREC_VARIABLE (chrec2),
	     chrec_merge (chrec1, CHREC_LEFT (chrec2)),
	     chrec_merge (integer_one_node, CHREC_RIGHT (chrec2)));
	  
	  return chrec_merge_intervals 
	    (chrec1, build_interval_chrec (chrec2, chrec2));
	  
	default:
	  return chrec_top;
	}
      
    case POLYNOMIAL_CHREC:
      switch (TREE_CODE (chrec2))
	{
	case REAL_CST:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTEGER_CST:
	case INTERVAL_CHREC:
	  return build_polynomial_chrec 
	    (CHREC_VARIABLE (chrec1),
	     chrec_merge (CHREC_LEFT (chrec1), chrec2),
	     chrec_merge (CHREC_RIGHT (chrec1), integer_zero_node));
	  
	case POLYNOMIAL_CHREC:
	  if (CHREC_VARIABLE (chrec1) == CHREC_VARIABLE (chrec2))
	    return build_polynomial_chrec 
	      (CHREC_VARIABLE (chrec2),
	       chrec_merge (CHREC_LEFT (chrec1), CHREC_LEFT (chrec2)),
	       chrec_merge (CHREC_RIGHT (chrec1), CHREC_RIGHT (chrec2)));
	  else if (CHREC_VARIABLE (chrec1) < CHREC_VARIABLE (chrec2))
	    return build_polynomial_chrec 
	      (CHREC_VARIABLE (chrec2),
	       chrec_merge (chrec1, CHREC_LEFT (chrec2)),
	       chrec_merge (integer_zero_node, CHREC_RIGHT (chrec2)));
	  else
	    return build_polynomial_chrec 
	      (CHREC_VARIABLE (chrec1),
	       chrec_merge (CHREC_LEFT (chrec1), chrec2),
	       chrec_merge (CHREC_RIGHT (chrec1), integer_zero_node));
	  
	case EXPONENTIAL_CHREC:
	  if (CHREC_VARIABLE (chrec1) == CHREC_VARIABLE (chrec2))
	    return chrec_top;
	  else if (CHREC_VARIABLE (chrec1) < CHREC_VARIABLE (chrec2))
	    return build_exponential_chrec
	      (CHREC_VARIABLE (chrec2),
	       chrec_merge (chrec1, CHREC_LEFT (chrec2)),
	       chrec_merge (integer_one_node, CHREC_RIGHT (chrec2)));
	  else
	    return build_polynomial_chrec
	      (CHREC_VARIABLE (chrec1),
	       chrec_merge (CHREC_LEFT (chrec1), chrec2),
	       chrec_merge (CHREC_RIGHT (chrec1), integer_zero_node));
	  
	default:
	  return chrec_top;
	}
      
    case EXPONENTIAL_CHREC:
      switch (TREE_CODE (chrec2))
	{
	case REAL_CST:
	case SSA_NAME:
	case VAR_DECL:
	case PARM_DECL:
	case INTEGER_CST:
	case INTERVAL_CHREC:
	  return build_exponential_chrec 
	    (CHREC_VARIABLE (chrec1),
	     chrec_merge (CHREC_LEFT (chrec1), chrec2),
	     chrec_merge (CHREC_RIGHT (chrec1), integer_one_node));
	  
	case POLYNOMIAL_CHREC:
	  if (CHREC_VARIABLE (chrec1) == CHREC_VARIABLE (chrec2))
	    return chrec_top;
	  else if (CHREC_VARIABLE (chrec1) < CHREC_VARIABLE (chrec2))
	    return build_polynomial_chrec
	      (CHREC_VARIABLE (chrec2),
	       chrec_merge (chrec1, CHREC_LEFT (chrec2)),
	       chrec_merge (integer_zero_node, CHREC_RIGHT (chrec2)));
	  else
	    return build_exponential_chrec
	      (CHREC_VARIABLE (chrec1),
	       chrec_merge (CHREC_LEFT (chrec1), chrec2),
	       chrec_merge (CHREC_RIGHT (chrec1), integer_one_node));
	  
	case EXPONENTIAL_CHREC:
	  return build_exponential_chrec 
	    (CHREC_VARIABLE (chrec1), 
	     chrec_merge (CHREC_LEFT (chrec1), CHREC_LEFT (chrec2)),
	     chrec_merge (CHREC_RIGHT (chrec1), CHREC_RIGHT (chrec2)));
	  
	default:
	  return chrec_top;
	}
      
    default:
      return chrec_top;
    }
}

/* Returns a chrec for which the initial condition has been removed.
   Example: {{1, +, 2}, +, 3} returns {2, +, 3}.  */

static tree
remove_initial_condition (tree chrec)
{
  tree left;
  
  if (chrec == NULL_TREE)
    return NULL_TREE;
  
  if (TREE_CODE (chrec) != POLYNOMIAL_CHREC
      && TREE_CODE (chrec) != EXPONENTIAL_CHREC)
    return NULL_TREE;
  
  left = remove_initial_condition (CHREC_LEFT (chrec));
  if (left == NULL_TREE)
    return CHREC_RIGHT (chrec);
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC)
    return build_polynomial_chrec 
      (CHREC_VARIABLE (chrec), left, CHREC_RIGHT (chrec));
  
  else
    return build_exponential_chrec
      (CHREC_VARIABLE (chrec), left, CHREC_RIGHT (chrec));
}

/* Search for the leftmost evolution element from CHREC_BEFORE, then
   begin the addition left to right from this locus.  */

static tree 
add_expr_to_loop_evolution_1 (unsigned loop_num, 
			      tree chrec_before, 
			      tree *to_add)
{
  if (*to_add == NULL_TREE)
    return chrec_before;
  
  /* Search for the leftmost locus.  */
  if (CHREC_VARIABLE (chrec_before) == loop_num
      && ((TREE_CODE (CHREC_LEFT (chrec_before)) != POLYNOMIAL_CHREC
	   && TREE_CODE (CHREC_LEFT (chrec_before)) != EXPONENTIAL_CHREC)
	  || (CHREC_VARIABLE (CHREC_LEFT (chrec_before)) != loop_num)))
    {
      /* And from this point, start adding to the right.  */
      tree init_cond = initial_condition (*to_add);
      
      *to_add = remove_initial_condition (*to_add);
      return build_polynomial_chrec 
	(CHREC_VARIABLE (chrec_before), CHREC_LEFT (chrec_before), 
	 chrec_fold_plus (CHREC_RIGHT (chrec_before), init_cond));
    }
  
  else
    {
      /* Search.  */
      tree res = add_expr_to_loop_evolution_1 
	(loop_num, CHREC_LEFT (chrec_before), to_add);
      
      /* If there are elements to be added...  */
      if (*to_add != NULL_TREE)
	{
	  tree init_cond = initial_condition (*to_add);
	  *to_add = remove_initial_condition (*to_add);
	  
	  /* ...add the element.  */
	  return build_polynomial_chrec 
	    (CHREC_VARIABLE (chrec_before), res, 
	     chrec_fold_plus (CHREC_RIGHT (chrec_before), init_cond));
	}
      
      return build_polynomial_chrec 
	(CHREC_VARIABLE (chrec_before), res, CHREC_RIGHT (chrec_before));
    }
}

/* Finish the copying of the evolution from TO_ADD into
   CHREC_BEFORE.  */

static tree 
add_expr_to_loop_evolution_2 (tree chrec_before, 
			      tree *to_add)
{
  tree init_cond;
  
  if (*to_add == NULL_TREE)
    return chrec_before;
  
  init_cond = initial_condition (*to_add);
  *to_add = remove_initial_condition (*to_add);
  
  return build_polynomial_chrec 
    (CHREC_VARIABLE (chrec_before), chrec_before, init_cond);
}

/* The expression CHREC_BEFORE has an evolution part in LOOP_NUM.  
   Add to this evolution the expression TO_ADD.  The invariant attribute 
   means that the TO_ADD expression is one of the nodes that do not depend
   on a loop: INTERVAL_CHREC, INTEGER_CST, VAR_DECL, ...  */

tree
add_expr_to_loop_evolution (unsigned loop_num, 
			    tree chrec_before, 
			    tree to_add)
{
#if defined ENABLE_CHECKING 
  if (to_add == NULL_TREE
      || chrec_before == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec_before))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) == loop_num)
	{
	  /* Add the polynomial TO_ADD to the evolution in LOOP_NUM of
	     CHREC_BEFORE.  */
	  tree res = add_expr_to_loop_evolution_1 
	    (loop_num, chrec_before, &to_add);
	  
	  /* When the degree of TO_ADD is greater than the degree of
	     CHREC_BEFORE in the LOOP_NUM dimension, build the new
	     evolution.  */
	  if (to_add != NULL_TREE)
	    res = add_expr_to_loop_evolution_2
	      (res, &to_add);
	  
	  return res;
	}
      
      else
	/* Search the evolution in LOOP_NUM.  */
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   add_expr_to_loop_evolution (loop_num, 
				       CHREC_LEFT (chrec_before), 
				       to_add),
	   /* testsuite/.../ssa-chrec-38.c
	      Do not modify the CHREC_RIGHT part: this part is a fixed part
	      completely determined by the evolution of other scalar variables.
	      The same comment is included in the no_evolution_in_loop_p 
	      function.  */
	   CHREC_RIGHT (chrec_before));
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) == loop_num)
	return build_exponential_chrec
	  (loop_num, 
	   CHREC_LEFT (chrec_before),
	   /* We still don't know how to fold these operations that mix 
	      polynomial and exponential functions.  For the moment, give a 
	      rough approximation: [-oo, +oo].  */
	   chrec_top);
      else
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   add_expr_to_loop_evolution (loop_num, 
				       CHREC_LEFT (chrec_before), 
				       to_add),
	   /* Do not modify the CHREC_RIGHT part: this part is a fixed part
	      completely determined by the evolution of other scalar variables.
	      The same comment is included in the no_evolution_in_loop_p 
	      function.  */
	   CHREC_RIGHT (chrec_before));
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not dependent on a loop.  */
      return chrec_before;
    }
}

/* The expression CHREC_BEFORE has an evolution part in LOOP_NUM.  
   Multiply this evolution by the expression TO_MULT.  The invariant attribute 
   means that the TO_MULT expression is one of the nodes that do not depend
   on a loop: INTERVAL_CHREC, INTEGER_CST, VAR_DECL, ...  */

tree
multiply_by_expr_the_loop_evolution (unsigned loop_num, 
				     tree chrec_before, 
				     tree to_mult)
{
#if defined ENABLE_CHECKING 
  if (chrec_before == NULL_TREE
      || to_mult == NULL_TREE)
    abort ();
#endif
  
  switch (TREE_CODE (chrec_before))
    {
    case POLYNOMIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) == loop_num)
	return build_polynomial_chrec 
	  (loop_num, 
	   CHREC_LEFT (chrec_before),
	   /* We still don't know how to fold these operations that mix 
	      polynomial and exponential functions.  For the moment, give a 
	      rough approximation: [-oo, +oo].  */
	   chrec_top);
      else
	return build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   multiply_by_expr_the_loop_evolution 
	   (loop_num, CHREC_LEFT (chrec_before), to_mult),
	   /* Do not modify the CHREC_RIGHT part: this part is a fixed part
	      completely determined by the evolution of other scalar variables.
	      The same comment is included in the no_evolution_in_loop_p 
	      function.  */
	   CHREC_RIGHT (chrec_before));
      
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (chrec_before) == loop_num
	  /* The evolution has to be multiplied on the leftmost position for 
	     loop_num.  */
	  && ((TREE_CODE (CHREC_LEFT (chrec_before)) != POLYNOMIAL_CHREC
	       && TREE_CODE (CHREC_LEFT (chrec_before)) != EXPONENTIAL_CHREC)
	      || (CHREC_VARIABLE (CHREC_LEFT (chrec_before)) != loop_num)))
	return build_exponential_chrec
	  (loop_num, 
	   CHREC_LEFT (chrec_before),
	   chrec_fold_multiply (CHREC_RIGHT (chrec_before), to_mult));
      else
	return build_exponential_chrec 
	  (CHREC_VARIABLE (chrec_before),
	   multiply_by_expr_the_loop_evolution
	   (loop_num, CHREC_LEFT (chrec_before), to_mult),
	   /* Do not modify the CHREC_RIGHT part: this part is a fixed part
	      completely determined by the evolution of other scalar variables.
	      The same comment is included in the no_evolution_in_loop_p 
	      function.  */
	   CHREC_RIGHT (chrec_before));
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not dependent on a loop.  */
      return chrec_before;
    }
}



/* Observers.  */

/* Helper function for is_multivariate_chrec.  */

static bool 
is_multivariate_chrec_rec (tree chrec, unsigned int rec_var)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC
      || TREE_CODE (chrec) == EXPONENTIAL_CHREC)
    {
      if (CHREC_VARIABLE (chrec) != rec_var)
	return true;
      else
	return (is_multivariate_chrec_rec (CHREC_LEFT (chrec), rec_var) 
		|| is_multivariate_chrec_rec (CHREC_RIGHT (chrec), rec_var));
    }
  else
    return false;
}

/* Determine whether the given chrec is multivariate or not.  */

bool 
is_multivariate_chrec (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC
      || TREE_CODE (chrec) == EXPONENTIAL_CHREC)
    return (is_multivariate_chrec_rec (CHREC_LEFT (chrec), 
				       CHREC_VARIABLE (chrec))
	    || is_multivariate_chrec_rec (CHREC_RIGHT (chrec), 
					  CHREC_VARIABLE (chrec)));
  else
    return false;
}

/* Determine whether the given chrec is a polynomial or not.  */

bool
is_pure_sum_chrec (tree chrec)
{
  if (chrec == NULL_TREE)
    return true;
  
  if (TREE_CODE (chrec) == EXPONENTIAL_CHREC)
    return false;
  
  if (TREE_CODE (chrec) == POLYNOMIAL_CHREC)
    return (is_pure_sum_chrec (CHREC_LEFT (chrec))
	    && is_pure_sum_chrec (CHREC_RIGHT (chrec)));
  
  return true;
}

/* Determines whether EXPR is a loop invariant wrt. LOOP_NUM.  */

bool
no_evolution_in_loop_p (tree expr,
			unsigned loop_num)
{
#if defined ENABLE_CHECKING 
  if (expr == NULL_TREE)
    abort ();
#endif
  
  /* A not so efficient, but equivalent and short to write method to compute 
     this property.  */
  if (1)
    {
      tree ev = evolution_function_in_loop_num (expr, loop_num);
      
      return (TREE_CODE (ev) != POLYNOMIAL_CHREC
	      && TREE_CODE (ev) != EXPONENTIAL_CHREC);
    }
  
  switch (TREE_CODE (expr))
    {
    case POLYNOMIAL_CHREC:
    case EXPONENTIAL_CHREC:
      if (CHREC_VARIABLE (expr) == loop_num)
	return false;
      else
	return no_evolution_in_loop_p (CHREC_LEFT (expr), loop_num);
      /* Do not look into the evolution part CHREC_RIGHT: this part does not 
	 describe the evolution of the analyzed variable, but the evolution 
	 of other scalar variables.  You'll find a similar comment in the 
	 add_expr_to_loop_evolution function.
	 
	 testsuite/.../ssa-chrec-38.c
	 Do not include the following code. 
	 && no_evolution_in_loop_p (CHREC_RIGHT (expr), loop_num));
      */
      
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
    case INTERVAL_CHREC:
    default:
      /* These nodes do not dependent on a loop.  */
      return true;
    }
}

/* Determines whether the chrec contains symbolic names or not.  */

bool 
chrec_contains_symbols (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (TREE_CODE (chrec) == SSA_NAME
      || TREE_CODE (chrec) == VAR_DECL
      || TREE_CODE (chrec) == PARM_DECL
      || TREE_CODE (chrec) == FUNCTION_DECL
      || TREE_CODE (chrec) == LABEL_DECL
      || TREE_CODE (chrec) == RESULT_DECL
      || TREE_CODE (chrec) == FIELD_DECL)
    return true;
  
  switch (TREE_CODE_LENGTH (TREE_CODE (chrec)))
    {
    case 3:
      if (chrec_contains_symbols (TREE_OPERAND (chrec, 2)))
	return true;
      
    case 2:
      if (chrec_contains_symbols (TREE_OPERAND (chrec, 1)))
	return true;
      
    case 1:
      if (chrec_contains_symbols (TREE_OPERAND (chrec, 0)))
	return true;
      
    default:
      return false;
    }
}

/* Determines whether the chrec contains undetermined coefficients.  */

bool 
chrec_contains_undetermined (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  if (chrec == chrec_top
      || chrec == chrec_not_analyzed_yet)
    return true;
  
  switch (TREE_CODE_LENGTH (TREE_CODE (chrec)))
    {
    case 3:
      if (chrec_contains_undetermined (TREE_OPERAND (chrec, 2)))
	return true;
      
    case 2:
      if (chrec_contains_undetermined (TREE_OPERAND (chrec, 1)))
	return true;
      
    case 1:
      if (chrec_contains_undetermined (TREE_OPERAND (chrec, 0)))
	return true;
      
    default:
      return false;
    }
}

/* Determines whether the chrec contains interval coefficients.  */

bool 
chrec_contains_intervals (tree chrec)
{
  if (chrec == NULL_TREE
      || automatically_generated_chrec_p (chrec))
    return false;
  
  if (TREE_CODE (chrec) == INTERVAL_CHREC)
    return true;
  
  switch (TREE_CODE_LENGTH (TREE_CODE (chrec)))
    {
    case 3:
      if (chrec_contains_intervals (TREE_OPERAND (chrec, 2)))
	return true;
      
    case 2:
      if (chrec_contains_intervals (TREE_OPERAND (chrec, 1)))
	return true;
      
    case 1:
      if (chrec_contains_intervals (TREE_OPERAND (chrec, 0)))
	return true;
      
    default:
      return false;
    }
}

/* Determines whether the tree EXPR contains chrecs.  */

bool
tree_contains_chrecs (tree expr)
{
  if (expr == NULL_TREE)
    return false;
  
  if (tree_is_chrec (expr))
    return true;
  
  switch (TREE_CODE_LENGTH (TREE_CODE (expr)))
    {
    case 3:
      if (tree_contains_chrecs (TREE_OPERAND (expr, 2)))
	return true;
      
    case 2:
      if (tree_contains_chrecs (TREE_OPERAND (expr, 1)))
	return true;
      
    case 1:
      if (tree_contains_chrecs (TREE_OPERAND (expr, 0)))
	return true;
      
    default:
      return false;
    }
}

/* Determine whether the given tree is an affine multivariate
   evolution.  */

bool 
evolution_function_is_affine_multivariate_p (tree chrec)
{
  if (chrec == NULL_TREE)
    return false;
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      if (evolution_function_is_constant_p (CHREC_LEFT (chrec)))
	{
	  if (evolution_function_is_constant_p (CHREC_RIGHT (chrec)))
	    return true;
	  else
	    {
	      if (CHREC_VARIABLE (CHREC_RIGHT (chrec)) 
		  != CHREC_VARIABLE (chrec)
		  && evolution_function_is_affine_multivariate_p 
		  (CHREC_RIGHT (chrec)))
		return true;
	      else
		return false;
	    }
	}
      else
	{
	  if (evolution_function_is_constant_p (CHREC_RIGHT (chrec))
	      && CHREC_VARIABLE (CHREC_LEFT (chrec)) != CHREC_VARIABLE (chrec)
	      && evolution_function_is_affine_multivariate_p 
	      (CHREC_LEFT (chrec)))
	    return true;
	  else
	    return false;
	}
      
      
    case EXPONENTIAL_CHREC:
    case INTERVAL_CHREC:
    default:
      return false;
    }
}

/* Determine whether the given tree is a function in zero or one 
   variables.  */

bool
evolution_function_is_univariate_p (tree chrec)
{
  if (chrec == NULL_TREE)
    return true;
  
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
    case EXPONENTIAL_CHREC:
      switch (TREE_CODE (CHREC_LEFT (chrec)))
	{
	case POLYNOMIAL_CHREC:
	case EXPONENTIAL_CHREC:
	  if (CHREC_VARIABLE (chrec) != CHREC_VARIABLE (CHREC_LEFT (chrec)))
	    return false;
	  if (!evolution_function_is_univariate_p (CHREC_LEFT (chrec)))
	    return false;
	  break;
	  
	default:
	  break;
	}
      
      switch (TREE_CODE (CHREC_RIGHT (chrec)))
	{
	case POLYNOMIAL_CHREC:
	case EXPONENTIAL_CHREC:
	  if (CHREC_VARIABLE (chrec) != CHREC_VARIABLE (CHREC_RIGHT (chrec)))
	    return false;
	  if (!evolution_function_is_univariate_p (CHREC_RIGHT (chrec)))
	    return false;
	  break;
	  
	default:
	  break;	  
	}
      
    default:
      return true;
    }
}



/* Analyzers.  */

/* This is the ZIV test.  ZIV = Zero Index Variable, ie. both
   functions do not depend on the iterations of a loop.  */

static inline bool
ziv_subscript_p (tree chrec_a, 
		 tree chrec_b)
{
  return (evolution_function_is_constant_p (chrec_a)
	  && evolution_function_is_constant_p (chrec_b));
}

/* Determines whether the subscript depends on the evolution of a
   single loop or not.  SIV = Single Index Variable.  */

static bool
siv_subscript_p (tree chrec_a,
		 tree chrec_b)
{
  if ((evolution_function_is_constant_p (chrec_a)
       && evolution_function_is_univariate_p (chrec_b))
      || (evolution_function_is_constant_p (chrec_b)
	  && evolution_function_is_univariate_p (chrec_a)))
    return true;
  
  if (evolution_function_is_univariate_p (chrec_a)
      && evolution_function_is_univariate_p (chrec_b))
    {
      switch (TREE_CODE (chrec_a))
	{
	case POLYNOMIAL_CHREC:
	case EXPONENTIAL_CHREC:
	  switch (TREE_CODE (chrec_b))
	    {
	    case POLYNOMIAL_CHREC:
	    case EXPONENTIAL_CHREC:
	      if (CHREC_VARIABLE (chrec_a) != CHREC_VARIABLE (chrec_b))
		return false;
	      
	    default:
	      return true;
	    }
	  
	default:
	  return true;
	}
    }
  
  return false;
}

/* Analyze a ZIV (Zero Index Variable) subscript.  */

static void 
analyze_ziv_subscript (tree chrec_a, 
		       tree chrec_b, 
		       tree *overlaps_a,
		       tree *overlaps_b)
{
  tree difference;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(analyze_ziv_subscript \n");
  
  difference = chrec_fold_minus (chrec_a, chrec_b);
  
  switch (TREE_CODE (difference))
    {
    case INTEGER_CST:
      if (integer_zerop (difference))
	{
	  /* The difference is equal to zero: the accessed index
	     overlaps for each iteration in the loop.  */
	  *overlaps_a = integer_zero_node;
	  *overlaps_b = integer_zero_node;
	}
      else
	{
	  /* The accesses do not overlap.  */
	  *overlaps_a = chrec_bot;
	  *overlaps_b = chrec_bot;	  
	}
      break;
      
    case INTERVAL_CHREC:
      if (integer_zerop (CHREC_LOW (difference)) 
	  && integer_zerop (CHREC_UP (difference)))
	{
	  /* The difference is equal to zero: the accessed index 
	     overlaps for each iteration in the loop.  */
	  *overlaps_a = integer_zero_node;
	  *overlaps_b = integer_zero_node;
	}
      else 
	{
	  /* There could be an overlap, conservative answer: 
	     "don't know".  */
	  *overlaps_a = chrec_top;
	  *overlaps_b = chrec_top;	  
	}
      break;
      
    default:
      /* We're not sure whether the indexes overlap.  For the moment, 
	 conservatively answer "don't know".  */
      *overlaps_a = chrec_top;
      *overlaps_b = chrec_top;	  
      break;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}

/* Analyze single index variable subscript.  Note that the dependence
   testing is not commutative, and that's why both versions of
   analyze_siv_subscript_x_y and analyze_siv_subscript_y_x are
   implemented.  */

static void
analyze_siv_subscript (tree chrec_a, 
		       tree chrec_b,
		       tree *overlaps_a, 
		       tree *overlaps_b)
{
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(analyze_siv_subscript \n");
  
  if (evolution_function_is_constant_p (chrec_a)
      && evolution_function_is_affine_p (chrec_b))
    analyze_siv_subscript_cst_affine (chrec_a, chrec_b, 
				      overlaps_a, overlaps_b);
  
  else if (evolution_function_is_affine_p (chrec_a)
	   && evolution_function_is_constant_p (chrec_b))
    analyze_siv_subscript_affine_cst (chrec_a, chrec_b, 
				      overlaps_a, overlaps_b);
  
  else if (evolution_function_is_affine_p (chrec_a)
	   && evolution_function_is_affine_p (chrec_b)
	   && (CHREC_VARIABLE (chrec_a) == CHREC_VARIABLE (chrec_b)))
    analyze_siv_subscript_affine_affine (chrec_a, chrec_b, 
					 overlaps_a, overlaps_b);
  else
    {
      *overlaps_a = chrec_top;
      *overlaps_b = chrec_top;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}

/* This is a part of the SIV subscript analyzer (Single Index
   Variable).  */

static void
analyze_siv_subscript_cst_affine (tree chrec_a, 
				  tree chrec_b,
				  tree *overlaps_a, 
				  tree *overlaps_b)
{
  bool value0, value1, value2;
  tree difference = chrec_fold_minus (CHREC_LEFT (chrec_b), chrec_a);
  
  if (!chrec_is_positive (initial_condition (difference), &value0))
    {
      /* Static analysis failed.  */
      *overlaps_a = chrec_top;
      *overlaps_b = chrec_top;      
      return;
    }
  else
    {
      if (value0 == false)
	{
	  if (!chrec_is_positive (CHREC_RIGHT (chrec_b), &value1))
	    {
	      /* The static analysis failed.  */
	      *overlaps_a = chrec_top;
	      *overlaps_b = chrec_top;      
	      return;
	    }
	  else
	    {
	      if (value1 == true)
		{
		  /* Example:  
		     chrec_a = 12
		     chrec_b = {10, +, 1}
		  */
		  
		  if (tree_fold_divides_p (CHREC_RIGHT (chrec_b), difference))
		    {
		      *overlaps_a = integer_zero_node;
		      *overlaps_b = tree_fold_int_exact_div 
			(tree_fold_int_abs (difference), 
			 CHREC_RIGHT (chrec_b));
		      return;
		    }
		  
		  /* When the step does not divides the difference, there are
		     no overlaps.  */
		  else
		    {
		      *overlaps_a = chrec_bot;
		      *overlaps_b = chrec_bot;      
		      return;
		    }
		}
	      
	      else
		{
		  /* Example:  
		     chrec_a = 12
		     chrec_b = {10, +, -1}
		     
		     In this case, chrec_a will not overlap with chrec_b.  */
		  *overlaps_a = chrec_bot;
		  *overlaps_b = chrec_bot;
		  return;
		}
	    }
	}
      else 
	{
	  if (!chrec_is_positive (CHREC_RIGHT (chrec_b), &value2))
	    {
	      /* Static analysis failed.  */
	      *overlaps_a = chrec_top;
	      *overlaps_b = chrec_top;      
	      return;
	    }
	  else
	    {
	      if (value2 == false)
		{
		  /* Example:  
		     chrec_a = 3
		     chrec_b = {10, +, -1}
		  */
		  if (tree_fold_divides_p (CHREC_RIGHT (chrec_b), difference))
		    {
		      *overlaps_a = integer_zero_node;
		      *overlaps_b = tree_fold_int_exact_div 
			(difference, CHREC_RIGHT (chrec_b));
		      return;
		    }
		  
		  /* When the step does not divides the difference, there
		     are no overlaps.  */
		  else
		    {
		      *overlaps_a = chrec_bot;
		      *overlaps_b = chrec_bot;      
		      return;
		    }
		}
	      else
		{
		  /* Example:  
		     chrec_a = 3  
		     chrec_b = {4, +, 1}
		 
		     In this case, chrec_a will not overlap with chrec_b.  */
		  *overlaps_a = chrec_bot;
		  *overlaps_b = chrec_bot;
		  return;
		}
	    }
	}
    }
}

/* This is a part of the SIV subscript analyzer (Single Index
   Variable).  */

static void
analyze_siv_subscript_affine_cst (tree chrec_a, 
				  tree chrec_b,
				  tree *overlaps_a, 
				  tree *overlaps_b)
{
  analyze_siv_subscript_cst_affine (chrec_b, chrec_a, overlaps_b, overlaps_a);
}

/* This is a part of the SIV subscript analyzer (Single Index
   Variable).  */

static void
analyze_siv_subscript_affine_affine (tree chrec_a, 
				     tree chrec_b,
				     tree *overlaps_a, 
				     tree *overlaps_b)
{
  tree left_a, left_b, right_a, right_b;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(analyze_siv_subscript_affine_affine \n");
  
#if defined ENABLE_CHECKING
  if (chrec_a == NULL_TREE
      || chrec_b == NULL_TREE)
    abort ();
#endif
  
  /* For determining the initial intersection, we have to solve a
     Diophantine equation.  This is the most time consuming part.
     
     For answering to the question: "Is there a dependence?" we have
     to prove that there exists a solution to the Diophantine
     equation, and that the solution is in the iteration domain,
     ie. the solution is positive or zero, and that the solution
     happens before the upper bound loop.nb_iterations.  Otherwise
     there is no dependence.  This function outputs a description of
     the iterations that hold the intersections.  */

  left_a = CHREC_LEFT (chrec_a);
  left_b = CHREC_LEFT (chrec_b);
  right_a = CHREC_RIGHT (chrec_a);
  right_b = CHREC_RIGHT (chrec_b);
  
  if (chrec_zerop (chrec_fold_minus (left_a, left_b)))
    {
      /* The first element accessed twice is on the first
	 iteration.  */
      *overlaps_a = build_polynomial_chrec 
	(CHREC_VARIABLE (chrec_a), 
	 integer_zero_node, integer_one_node);
      *overlaps_b = build_polynomial_chrec 
	(CHREC_VARIABLE (chrec_b), 
	 integer_zero_node, integer_one_node);
    }
  
  else if (TREE_CODE (left_a) == INTEGER_CST
	   && TREE_CODE (left_b) == INTEGER_CST
	   && TREE_CODE (right_a) == INTEGER_CST 
	   && TREE_CODE (right_b) == INTEGER_CST
	   
	   /* Both functions should have the same evolution sign.  */
	   && ((tree_int_cst_sgn (right_a) > 0 
		&& tree_int_cst_sgn (right_b) > 0)
	       || (tree_int_cst_sgn (right_a) < 0
		   && tree_int_cst_sgn (right_b) < 0)))
    {
      /* Here we have to solve the Diophantine equation.  Reference
	 book: "Loop Transformations for Restructuring Compilers - The
	 Foundations" by Utpal Banerjee, pages 59-80.
	 
	 ALPHA * X0 = BETA * Y0 + GAMMA.  
	 
	 with:
	 ALPHA = RIGHT_A
	 BETA = RIGHT_B
	 GAMMA = LEFT_B - LEFT_A
	 CHREC_A = {LEFT_A, +, RIGHT_A}
	 CHREC_B = {LEFT_B, +, RIGHT_B}
	 
	 The Diophantine equation has a solution if and only if gcd
	 (ALPHA, BETA) divides GAMMA.  This is commonly known under
	 the name of the "gcd-test".
      */
      tree alpha, beta, gamma;
      tree la, lb;
      tree gcd_alpha_beta;
      tree u11, u12, u21, u22;

      /* Both alpha and beta have to be integer_type_node. The gcd
	 function does not work correctly otherwise.  */
      alpha = copy_node (right_a);
      beta = copy_node (right_b);
      la = copy_node (left_a);
      lb = copy_node (left_b);
      TREE_TYPE (alpha) = integer_type_node;
      TREE_TYPE (beta) = integer_type_node;
      TREE_TYPE (la) = integer_type_node;
      TREE_TYPE (lb) = integer_type_node;
      
      gamma = tree_fold_int_minus (lb, la);
      
      /* FIXME: Use lambda_*_Hermite for implementing Bezout.  */
      gcd_alpha_beta = tree_fold_bezout 
	(alpha, tree_fold_int_multiply (beta, integer_minus_one_node),
	 &u11, &u12, 
	 &u21, &u22);
      
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "  (alpha = ");
	  print_generic_expr (tree_dump_file, alpha, 0);
	  fprintf (tree_dump_file, ")\n  (beta = ");
	  print_generic_expr (tree_dump_file, beta, 0);
	  fprintf (tree_dump_file, ")\n  (gamma = ");
	  print_generic_expr (tree_dump_file, gamma, 0);
	  fprintf (tree_dump_file, ")\n  (gcd_alpha_beta = ");
	  print_generic_expr (tree_dump_file, gcd_alpha_beta, 0);
	  fprintf (tree_dump_file, ")\n");
	}
      
      /* The classic "gcd-test".  */
      if (!tree_fold_divides_p (gcd_alpha_beta, gamma))
	{
	  /* The "gcd-test" has determined that there is no integer
	     solution, ie. there is no dependence.  */
	  *overlaps_a = chrec_bot;
	  *overlaps_b = chrec_bot;
	}
      
      else
	{
	  /* The solutions are given by:
	     | 
	     | [GAMMA/GCD_ALPHA_BETA  t].[u11 u12]  = [X]
	     |                           [u21 u22]    [Y]
	     
	     For a given integer t.  Using the following variables,
	     
	     | i0 = u11 * gamma / gcd_alpha_beta
	     | j0 = u12 * gamma / gcd_alpha_beta
	     | i1 = u21
	     | j1 = u22
	     
	     the solutions are:
	     
	     | x = i0 + i1 * t, 
	     | y = j0 + j1 * t.  */
	  
	  tree i0, j0, i1, j1, t;
	  tree gamma_gcd;
	  
	  /* X0 and Y0 are the first iterations for which there is a
	     dependence.  X0, Y0 are two solutions of the Diophantine
	     equation: chrec_a (X0) = chrec_b (Y0).  */
	  tree x0, y0;
      
	  /* Exact div because in this case gcd_alpha_beta divides
	     gamma.  */
	  gamma_gcd = tree_fold_int_exact_div (gamma, gcd_alpha_beta);
	  i0 = tree_fold_int_multiply (u11, gamma_gcd);
	  j0 = tree_fold_int_multiply (u12, gamma_gcd);
	  i1 = u21;
	  j1 = u22;
	  
	  if ((tree_int_cst_sgn (i1) == 0
	       && tree_int_cst_sgn (i0) < 0)
	      || (tree_int_cst_sgn (j1) == 0
		  && tree_int_cst_sgn (j0) < 0))
	    {
	      /* There is no solution.  
		 FIXME: The case "i0 > nb_iterations, j0 > nb_iterations" 
		 falls in here, but for the moment we don't look at the 
		 upper bound of the iteration domain.  */
	      *overlaps_a = chrec_bot;
	      *overlaps_b = chrec_bot;
  	    }
	  
	  else 
	    {
	      if (tree_int_cst_sgn (i1) > 0)
		{
		  t = tree_fold_int_ceil_div 
		    (tree_fold_int_multiply (i0, integer_minus_one_node), i1);
		  
		  if (tree_int_cst_sgn (j1) > 0)
		    {
		      t = tree_fold_int_max 
			(t, tree_fold_int_ceil_div 
			 (tree_fold_int_multiply 
			  (j0, integer_minus_one_node), j1));
		      
		      x0 = tree_fold_int_plus 
			(i0, tree_fold_int_multiply (i1, t));
		      y0 = tree_fold_int_plus 
			(j0, tree_fold_int_multiply (j1, t));
		      
		      *overlaps_a = build_polynomial_chrec 
			(CHREC_VARIABLE (chrec_a), x0, u21);
		      *overlaps_b = build_polynomial_chrec 
			(CHREC_VARIABLE (chrec_b), y0, u22);
		    }
		  else
		    {
		      /* FIXME: For the moment, the upper bound of the
			 iteration domain for j is not checked. */
		      *overlaps_a = chrec_top;
		      *overlaps_b = chrec_top;
		    }
		}
	      
	      else
		{
		  /* FIXME: For the moment, the upper bound of the
		     iteration domain for i is not checked. */
		  *overlaps_a = chrec_top;
		  *overlaps_b = chrec_top;
		}
	    }
	}
    }
  
  else
    {
      /* For the moment, "don't know".  */
      *overlaps_a = chrec_top;
      *overlaps_b = chrec_top;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (overlaps_a = ");
      print_generic_expr (tree_dump_file, *overlaps_a, 0);
      fprintf (tree_dump_file, ")\n  (overlaps_b = ");
      print_generic_expr (tree_dump_file, *overlaps_b, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}

/* Helper for determining whether the evolution steps of an affine
   CHREC divide the constant CST.  */

static bool
chrec_steps_divide_constant_p (tree chrec, 
			       tree cst)
{
  switch (TREE_CODE (chrec))
    {
    case POLYNOMIAL_CHREC:
      return (tree_fold_divides_p (CHREC_RIGHT (chrec), cst)
	      && chrec_steps_divide_constant_p (CHREC_LEFT (chrec), cst));
      
    default:
      /* On the initial condition, return true.  */
      return true;
    }
}

/* This is the MIV subscript analyzer (Multiple Index Variable).  */

static void
analyze_miv_subscript (tree chrec_a, 
		       tree chrec_b, 
		       tree *overlaps_a, 
		       tree *overlaps_b)
{
  /* FIXME:  This is a MIV subscript, not yet handled.
     Example: (A[{1, +, 1}_1] vs. A[{1, +, 1}_2]) that comes from 
     (A[i] vs. A[j]).  
     
     In the SIV test we had to solve a Diophantine equation with two
     variables.  In the MIV case we have to solve a Diophantine
     equation with 2*n variables (if the subscript uses n IVs).
  */
  tree difference;
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(analyze_miv_subscript \n");
  
  difference = chrec_fold_minus (chrec_a, chrec_b);
  
  if (chrec_zerop (difference))
    {
      /* Access functions are the same: all the elements are accessed
	 in the same order.  */
      *overlaps_a = integer_zero_node;
      *overlaps_b = integer_zero_node;
    }
  
  else if (evolution_function_is_constant_p (difference)
	   /* For the moment, the following is verified:
	      evolution_function_is_affine_multivariate_p (chrec_a) */
	   && !chrec_steps_divide_constant_p (chrec_a, difference))
    {
      /* testsuite/.../ssa-chrec-33.c
	 {{21, +, 2}_1, +, -2}_2  vs.  {{20, +, 2}_1, +, -2}_2 
        
	 The difference is 1, and the evolution steps are equal to 2,
	 consequently there are no overlapping elements.  */
      *overlaps_a = chrec_bot;
      *overlaps_b = chrec_bot;
    }
  
  else if (evolution_function_is_univariate_p (chrec_a)
	   && evolution_function_is_univariate_p (chrec_b))
    {
      /* testsuite/.../ssa-chrec-35.c
	 {0, +, 1}_2  vs.  {0, +, 1}_3
	 the overlapping elements are respectively located at iterations:
	 {0, +, 1}_3 and {0, +, 1}_2.
        
	 The overlaps are computed by the classic siv tester, then the
	 evolution loops are exchanged.
      */
      
      tree fake_b, fake_overlaps_a;
      fake_b = build_polynomial_chrec 
	(CHREC_VARIABLE (chrec_a), 
	 CHREC_LEFT (chrec_b), CHREC_RIGHT (chrec_b));
      /* Analyze the siv.  */
      analyze_siv_subscript (chrec_a, fake_b, 
			     &fake_overlaps_a, overlaps_b);
      
      /* Exchange the variables.  */
      if (evolution_function_is_constant_p (*overlaps_b))
	*overlaps_b = build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_a), *overlaps_b, 
	   integer_one_node);
      
      if (evolution_function_is_constant_p (fake_overlaps_a))
	*overlaps_a = build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_b), fake_overlaps_a, 
	   integer_one_node);
      
      else
	*overlaps_a = build_polynomial_chrec 
	  (CHREC_VARIABLE (chrec_b), 
	   CHREC_LEFT (fake_overlaps_a),
	   CHREC_RIGHT (fake_overlaps_a));
    }
  
  else
    {
      /* When the analysis is too difficult, answer "don't know".  */
      *overlaps_a = chrec_top;
      *overlaps_b = chrec_top;
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}

/* Determines the iterations for which CHREC_A is equal to CHREC_B.
   OVERLAP_ITERATIONS_A and OVERLAP_ITERATIONS_B are two functions
   that describe the iterations that contain conflicting elements.
   
   Remark: For an integer k >= 0, the following equality is true:
   
   CHREC_A (OVERLAP_ITERATIONS_A (k)) == CHREC_B (OVERLAP_ITERATIONS_B (k)).
*/

void 
analyze_overlapping_iterations (tree chrec_a, 
				tree chrec_b, 
				tree *overlap_iterations_a, 
				tree *overlap_iterations_b)
{
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(analyze_overlapping_iterations \n");
      fprintf (tree_dump_file, "  (chrec_a = ");
      print_generic_expr (tree_dump_file, chrec_a, 0);
      fprintf (tree_dump_file, ")\n  chrec_b = ");
      print_generic_expr (tree_dump_file, chrec_b, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  if (chrec_a == NULL_TREE
      || chrec_b == NULL_TREE
      || chrec_contains_undetermined (chrec_a)
      || chrec_contains_undetermined (chrec_b)
      || chrec_contains_symbols (chrec_a)
      || chrec_contains_symbols (chrec_b)
      || chrec_contains_intervals (chrec_a)
      || chrec_contains_intervals (chrec_b))
    {
      *overlap_iterations_a = chrec_top;
      *overlap_iterations_b = chrec_top;
    }
  
  else if (ziv_subscript_p (chrec_a, chrec_b))
    analyze_ziv_subscript (chrec_a, chrec_b, 
			   overlap_iterations_a, overlap_iterations_b);
  
  else if (siv_subscript_p (chrec_a, chrec_b))
    analyze_siv_subscript (chrec_a, chrec_b, 
			   overlap_iterations_a, overlap_iterations_b);
  
  else
    analyze_miv_subscript (chrec_a, chrec_b, 
			   overlap_iterations_a, overlap_iterations_b);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "  (overlap_iterations_a = ");
      print_generic_expr (tree_dump_file, *overlap_iterations_a, 0);
      fprintf (tree_dump_file, ")\n  (overlap_iterations_b = ");
      print_generic_expr (tree_dump_file, *overlap_iterations_b, 0);
      fprintf (tree_dump_file, ")\n");
    }
}

/* Given an affine function "CHREC = {a, +, b}_x", this function
   determines the first index (x >= 0) for which the value of the
   function becomes positive.  */

static tree 
how_far_to_positive_for_affine_function (tree chrec)
{
  if (chrec == NULL_TREE)
    return chrec_top;
  
  switch (TREE_CODE (CHREC_LEFT (chrec)))
    {
    case INTERVAL_CHREC:
      switch (TREE_CODE (CHREC_RIGHT (chrec)))
	{
	case INTERVAL_CHREC:
	  return how_far_to_positive_for_affine_function_ival_ival 
	    (CHREC_LOW (CHREC_LEFT (chrec)), CHREC_UP (CHREC_LEFT (chrec)),
	     CHREC_LOW (CHREC_RIGHT (chrec)), CHREC_UP (CHREC_RIGHT (chrec)));
	  
	default:
	  return how_far_to_positive_for_affine_function_ival_scalar 
	    (CHREC_LOW (CHREC_LEFT (chrec)), CHREC_UP (CHREC_LEFT (chrec)),
	     CHREC_RIGHT (chrec));
	}
      
    default:
      switch (TREE_CODE (CHREC_RIGHT (chrec)))
	{
	case INTERVAL_CHREC:
	  return how_far_to_positive_for_affine_function_scalar_ival 
	    (CHREC_LEFT (chrec), 
	     CHREC_LOW (CHREC_RIGHT (chrec)), CHREC_UP (CHREC_RIGHT (chrec)));
	  
	default:
	  return how_far_to_positive_for_affine_function_scalar_scalar
	    (CHREC_LEFT (chrec), CHREC_RIGHT (chrec));
	}
    }
}

/* Given an affine function "{INIT_COND, +, STEP}_x", with INIT_COND
   and STEP two scalars, this function determines the first index (x
   >= 0) for which the value of the function becomes positive.  */

static tree 
how_far_to_positive_for_affine_function_scalar_scalar (tree init_cond, 
						       tree step)
{
  if (tree_contains_chrecs (init_cond)
      || tree_contains_chrecs (step))
    return chrec_top;
  
  if (TREE_CODE (step) == INTEGER_CST)
    {
      if (TREE_CODE (init_cond) == INTEGER_CST)
	{
	  int init_cond_sign, step_sign;
	  
	  init_cond_sign = tree_int_cst_sgn (init_cond);
	  step_sign = tree_int_cst_sgn (step);
	  
	  if (init_cond_sign > 0)
	    return integer_zero_node;
	  
	  if (init_cond_sign == 0)
	    {
	      if (step_sign <= 0)
		return chrec_top;
	      else
		return integer_one_node;
	    }
	  
	  if (step_sign > 0)
	    /* abs (trunc (init_cond / step)) + 1.  */
	    return tree_fold_int_plus 
	      (tree_fold_int_abs 
	       (tree_fold_int_trunc_div (init_cond, step)), 
	       integer_one_node);
	  
	  /* if (step_low_sign <= 0 && step_up_sign >= 0)  */
	  /* [0, +oo]  */
	  return chrec_top;
	}
      
      /* The initial condition is a symbolic scalar.  */
      else
	{
	  int step_sign = tree_int_cst_sgn (step);
	  
	  if (step_sign > 0)
	    /* abs (trunc (init_cond / step)) + 1, or the equivalent
	       floor (abs (init_cond) / step) + 1.
	       
	       In the case where init_cond is a symbolic scalar, the
	       fold cannot reduce the expression due to the
	       undetermined sign of the symbol.  */
	    return tree_fold_int_plus 
	      (tree_fold_int_floor_div 
	       (tree_fold_int_abs (init_cond), step), 
	       integer_one_node);
	  
	  /* Else, the iterator goes to -MAX_INT, then becomes positive. */
	  else
	    /* For the moment this is not implemented.  */
	    return chrec_top;
	}
    }
  
  /* The step is a symbolic scalar.  */
  else
    {
      /* If we can determine the signs of the initial condition and of
	 the step, then it is possible to answer with a symbolic AST.
	 Otherwise it is not safe to answer anything else than
	 chrec_top.
	 
	 For the moment this is not implemented.  */
      return chrec_top;
    }
}

/* Same operation as above, for a scalar and an interval.  */

static tree 
how_far_to_positive_for_affine_function_scalar_ival (tree init_cond, 
						     tree step_low, 
						     tree step_up)
{
  /* This is not an efficient implementation.  */
  return how_far_to_positive_for_affine_function_ival_ival 
    (init_cond, init_cond, step_low, step_up);
}

/* Same operation as above, for an interval and a scalar.  */

static tree 
how_far_to_positive_for_affine_function_ival_scalar (tree init_cond_low,
						     tree init_cond_up,
						     tree step)
{
  /* This is not an efficient implementation.  */
  return how_far_to_positive_for_affine_function_ival_ival
    (init_cond_low, init_cond_up, step, step);
}

/* Same operation as above, for two intervals.  */

static tree 
how_far_to_positive_for_affine_function_ival_ival (tree init_cond_low, 
						   tree init_cond_up, 
						   tree step_low,
						   tree step_up)
{
  int init_cond_low_sign, init_cond_up_sign, step_low_sign, step_up_sign;
  
  if (TREE_CODE (init_cond_low) != INTEGER_CST
      || TREE_CODE (init_cond_up) != INTEGER_CST
      || TREE_CODE (step_low) != INTEGER_CST
      || TREE_CODE (step_up) != INTEGER_CST)
    return chrec_top;
  
  init_cond_low_sign = tree_int_cst_sgn (init_cond_low);
  init_cond_up_sign = tree_int_cst_sgn (init_cond_up);
  step_low_sign = tree_int_cst_sgn (step_low);
  step_up_sign = tree_int_cst_sgn (step_up);
  
  if (init_cond_low_sign > 0)
    return integer_zero_node;
  else if (init_cond_low_sign == 0
	   && init_cond_up_sign == 0)
    {
      if (step_up_sign < 0
	  || (step_low_sign == 0
	      && step_up_sign == 0))
	return chrec_top;
      
      if (step_low_sign > 0)
	return integer_one_node;
      
      /* if (step_low_sign <= 0 && step_up_sign >= 0)  */
      return chrec_top;
    }
  else if (init_cond_up_sign > 0)
    {
      /* The minimum is zero.  */
      
      if (init_cond_low_sign == 0)
	{
	  if (step_up_sign < 0
	      || (step_low_sign == 0
		  && step_up_sign == 0))
	    /* [-oo, 0]  */
	    return chrec_top;
	  
	  if (step_low_sign > 0)
	    /* [0, 1]  */
	    return build_interval_chrec (integer_zero_node, 
					 integer_one_node);
	  
	  /* if (step_low_sign <= 0 && step_up_sign >= 0)  */
	  /* [0, +oo]  */
	  return chrec_top;
	}
      else /* if (init_cond_low_sign < 0) */
	{
	  if (step_up_sign < 0
	      || (step_low_sign == 0
		  && step_up_sign == 0))
	    /* [-oo, 0]  */
	    return chrec_top;
	  
	  if (step_low_sign > 0)
	    /* [0, abs (trunc (init_cond_low / step_low)) + 1]  */
	    return build_interval_chrec 
	      (integer_zero_node,
	       tree_fold_int_plus
	       (tree_fold_int_abs 
		(tree_fold_int_trunc_div (init_cond_low, 
					  step_low)),
		integer_one_node));
	  
	  /* if (step_low_sign <= 0 && step_up_sign >= 0)  */
	  /* [0, +oo]  */
	  return chrec_top;
	}
    }
  else /* if (init_cond_up_sign <= 0) */
    {
      if (step_up_sign < 0
	  || (step_low_sign == 0
	      && step_up_sign == 0))
	/* [+oo, -oo]  */
	return chrec_bot;
      
      if (step_low_sign > 0)
	/* [abs (trunc (init_cond_up / step_up)) + 1,
	   abs (trunc (init_cond_low / step_low)) + 1]  */
	{
	  tree low, up;
	  
	  low = tree_fold_int_plus 
	    (tree_fold_int_abs 
	     (tree_fold_int_trunc_div (init_cond_up, step_up)),
	     integer_one_node);
	  up = tree_fold_int_plus
	    (tree_fold_int_abs 
	     (tree_fold_int_trunc_div (init_cond_low, step_low)),
	     integer_one_node);
	  
	  return build_interval_chrec (low, up);
	}
      /* if (step_low_sign <= 0 && step_up_sign >= 0)  */
      /* [0, +oo]  */
      return chrec_top;
    }
}



/* Evaluate the minimal number of iterations 'i' that satisfy "chrec (i) > 0". 
   EVOLUTION_LOOP_NUM is the loop number for which the analyzer has to 
   determine the number of iterations.
   
   In the general case, the zeros of a chrec are not easily analyzable.  
   There are two solutions:
   
   - work hard: evaluate the chrec successively for all the indices
     (the interpretation could include a widening technique that avoid
     the slow linear convergence.  However this could be difficult
     since the analyzed functions are not all monotone.  This is still
     work in progress...),
     
   - don't worry: just say don't know, ie. [-oo, +oo],
   
   - meta solution: find a mathematical way to solve the problem.  */

tree 
how_far_to_positive (unsigned evolution_loop_num, 
		     tree chrec)
{
#if defined ENABLE_CHECKING
  if (chrec == NULL_TREE)
    abort ();
#endif
  
  if (no_evolution_in_loop_p (chrec, evolution_loop_num))
    {
      chrec = initial_condition (chrec);
      
      if ((TREE_CODE (chrec) == INTERVAL_CHREC
	   && tree_expr_nonnegative_p (CHREC_LOW (chrec)))
	  || tree_expr_nonnegative_p (chrec))
	/* The variable is already positive.  */
	return integer_zero_node;
      else
	/* The variable will probably never reach positive values.  */
	return chrec_top;
    }
  
  if (chrec_contains_undetermined (chrec))
    return chrec_top;
  
  else if (is_multivariate_chrec (chrec))
    {
      tree scev_fn;
      
      /* Select the evolution function in the current loop.  */
      scev_fn = evolution_function_in_loop_num (chrec, evolution_loop_num);
      
      if (evolution_function_is_affine_p (scev_fn))
	{
	  tree res = how_far_to_positive_for_affine_function (scev_fn);
	  /* When we have an outer loop evolution: {{23, +, 1}_1, +, 1}_3. 
	     Example: ssa-chrec-01.c */
	  res = chrec_fold_plus (res, chrec_fold_minus (scev_fn, chrec));
	  return res;
	}
      else
	return chrec_top;
    }
  
  else if (evolution_function_is_affine_p (chrec))
    return how_far_to_positive_for_affine_function (chrec);

  else
    /* Don't worry.  */
    return chrec_top;
}

/* Evaluate the minimal number of iterations 'i' that satisfy 
   "chrec (i) == 0".  EVOLUTION_LOOP_NUM is the loop number for which 
   the analyzer has to determine the number of iterations.
*/

tree 
how_far_to_zero (unsigned evolution_loop_num ATTRIBUTE_UNUSED, 
		 tree chrec ATTRIBUTE_UNUSED)
{
  return chrec_top;
}

/* Evaluate the minimal number of iterations 'i' that satisfy 
   "chrec (i) != 0".  */

tree 
how_far_to_non_zero (unsigned evolution_loop_num ATTRIBUTE_UNUSED, 
		     tree chrec ATTRIBUTE_UNUSED)
{
  return chrec_top;
}

