/* Fold GENERIC expressions.
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
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


/* This file defines an interface of the tree folder.  
   For the moment, the functions in this file are just wrappers around 
   the "big-ugly" fold function.  The final aim is to completely split
   up the fold function into small pieces in such a way that client 
   passes don't see the changes to the underlying implementation.  */

#ifndef GCC_TREE_FOLD_H
#define GCC_TREE_FOLD_H



extern tree id_tree (tree);

/* Interface for boolean operations folding.  */
static inline tree tree_fold_bool_lt (tree, tree);
static inline tree tree_fold_bool_le (tree, tree);
static inline tree tree_fold_bool_gt (tree, tree);
static inline tree tree_fold_bool_ge (tree, tree);
static inline tree tree_fold_bool_eq (tree, tree);
static inline tree tree_fold_bool_ne (tree, tree);

/* Interface for integer operations folding.  */
extern tree tree_fold_int_lcm (tree, tree);
extern tree tree_fold_int_gcd (tree, tree);
extern tree tree_fold_bezout (tree, tree, tree *, tree *, tree *, tree *);
extern tree tree_fold_int_factorial (tree);

static inline tree chrec_merge_types (tree, tree);
static inline tree tree_fold_int_plus (tree, tree);
static inline tree tree_fold_int_minus (tree, tree);
static inline tree tree_fold_int_multiply (tree, tree);
static inline tree tree_fold_int_trunc_div (tree, tree);
static inline tree tree_fold_int_ceil_div (tree, tree);
static inline tree tree_fold_int_floor_div (tree, tree);

static inline tree tree_fold_int_exact_div (tree, tree);
static inline tree tree_fold_int_min (tree, tree);
static inline tree tree_fold_int_max (tree, tree);
static inline tree tree_fold_int_abs (tree);
static inline tree tree_fold_int_binomial (tree, tree);
static inline bool tree_fold_divides_p (tree, tree);



/* When computing on chrecs, determine the type of the result from the
   types TYPE0 and TYPE1 of the operands.  */

static inline tree 
chrec_merge_types (tree type0, 
		   tree type1)
{
  if (type0 == type1)
    return type0;
  
  if (type0 == char_type_node
      || type1 == char_type_node)
    return char_type_node;
  
  if (type0 == integer_type_node
      || type1 == integer_type_node)
    return integer_type_node;
  
  /* FIXME.  */
  return type0;
}

static inline tree 
tree_fold_bool_lt (tree a,
		   tree b)
{
  return fold (build (LT_EXPR, boolean_type_node, a, b));
}

static inline tree 
tree_fold_bool_le (tree a,
		   tree b)
{
  return fold (build (LE_EXPR, boolean_type_node, a, b));
}

static inline tree 
tree_fold_bool_gt (tree a,
		   tree b)
{
  return fold (build (GT_EXPR, boolean_type_node, a, b));
}

static inline tree 
tree_fold_bool_ge (tree a,
		   tree b)
{
  return fold (build (GE_EXPR, boolean_type_node, a, b));
}

static inline tree 
tree_fold_bool_eq (tree a,
		   tree b)
{
  return fold (build (EQ_EXPR, boolean_type_node, a, b));
}

static inline tree 
tree_fold_bool_ne (tree a,
		   tree b)
{
  return fold (build (NE_EXPR, boolean_type_node, a, b));
}



/* Fold the addition.  */

static inline tree 
tree_fold_int_plus (tree a,
		    tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  tree res = fold (build (PLUS_EXPR, type_node, a, b));
  
  /* Strip away the NON_LVALUE_EXPR: fixes bootstrap on Darwin.  */
  if (TREE_CODE (res) == NON_LVALUE_EXPR)
    return TREE_OPERAND (res, 0);
  
  else
    return res;
}

/* Fold the substraction.  */

static inline tree 
tree_fold_int_minus (tree a,
		     tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  tree res = fold (build (MINUS_EXPR, type_node, a, b));
  
  /* Strip away the NON_LVALUE_EXPR: fixes bootstrap on Darwin.  */
  if (TREE_CODE (res) == NON_LVALUE_EXPR)
    return TREE_OPERAND (res, 0);
  
  else 
    return res;
}

/* Fold the multiplication.  */

static inline tree 
tree_fold_int_multiply (tree a,
			tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  tree res = fold (build (MULT_EXPR, type_node, a, b));
  
  /* Strip away the NON_LVALUE_EXPR: fixes bootstrap on Darwin.  */
  if (TREE_CODE (res) == NON_LVALUE_EXPR)
    return TREE_OPERAND (res, 0);
  
  else
    return res;
}

/* Division for integer result that rounds the quotient toward zero.  */

static inline tree 
tree_fold_int_trunc_div (tree a,
			 tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  return fold (build (TRUNC_DIV_EXPR, type_node, a, b));
}

/* Division for integer result that rounds the quotient toward infinity.  */

static inline tree 
tree_fold_int_ceil_div (tree a,
			tree b)
{
  return fold (build (CEIL_DIV_EXPR, integer_type_node, a, b));
}

/* Division for integer result that rounds toward minus infinity.  */

static inline tree 
tree_fold_int_floor_div (tree a,
			 tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  return fold (build (FLOOR_DIV_EXPR, type_node, a, b));
}

/* Division which is not supposed to need rounding.  */

static inline tree 
tree_fold_int_exact_div (tree a,
			 tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  return fold (build (EXACT_DIV_EXPR, type_node, a, b));
}

/* Computes the minimum.  */

static inline tree 
tree_fold_int_min (tree a,
		   tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  return fold (build (MIN_EXPR, type_node, a, b));
}

/* Computes the maximum.  */

static inline tree 
tree_fold_int_max (tree a,
		   tree b)
{
  tree type_node = chrec_merge_types (TREE_TYPE (a), TREE_TYPE (b));
  return fold (build (MAX_EXPR, type_node, a, b));
}

/* Computes the absolute value.  */

static inline tree 
tree_fold_int_abs (tree a)
{
  return fold (build1 (ABS_EXPR, TREE_TYPE (a), a));
}

/* The binomial coefficient.  */

static inline tree 
tree_fold_int_binomial (tree n,
			tree k)
{
  return tree_fold_int_exact_div 
    (tree_fold_int_factorial (n), 
     tree_fold_int_multiply 
     (tree_fold_int_factorial (k),
      tree_fold_int_factorial 
      (tree_fold_int_minus (n, k))));
}

/* Determines whether (a divides b), or (a == gcd (a, b)).  */

static inline bool 
tree_fold_divides_p (tree a, 
		     tree b)
{
  if (integer_onep (a))
    return true;
  
  return integer_zerop 
    (tree_fold_int_minus 
     (a, tree_fold_int_gcd (a, b)));
}

#endif  /* GCC_TREE_FOLD_H  */
