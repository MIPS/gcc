/* Fold GENERIC expressions.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "tree-fold-const.h"



/* Debugging functions.  */

/* This is the identity function.  It can be used to insert breakpoints 
   in complex tree constructions such as those in the fold functions.  */

tree 
id_tree (tree t)
{
  return t;
}



/* Least common multiple.  */

tree 
tree_fold_int_lcm (tree a, 
		   tree b)
{
  tree pgcd;
  
#if defined ENABLE_CHECKING
  if (TREE_CODE (a) != INTEGER_CST
      || TREE_CODE (b) != INTEGER_CST)
    abort ();
#endif
  
  if (integer_onep (a)) 
    return b;
  
  if (integer_onep (b)) 
    return a;
  
  if (integer_zerop (a)
      || integer_zerop (b)) 
    return integer_zero_node;
  
  pgcd = tree_fold_int_gcd (a, b);
  
  if (integer_onep (pgcd))
    return tree_fold_int_multiply (a, b);
  else
    return tree_fold_int_multiply 
      (pgcd, tree_fold_int_lcm (tree_fold_int_exact_div (a, pgcd), 
				tree_fold_int_exact_div (b, pgcd)));
}

/* Greatest common divisor.  */

tree 
tree_fold_int_gcd (tree a, 
		   tree b)
{
  tree a_minus_b;
  
#if defined ENABLE_CHECKING
  if (TREE_CODE (a) != INTEGER_CST
      || TREE_CODE (b) != INTEGER_CST)
    abort ();
#endif
  
  if (integer_zerop (a)) 
    return b;
  
  if (integer_zerop (b)) 
    return a;
  
  if (tree_int_cst_sgn (a) == -1)
    a = tree_fold_int_multiply (a, integer_minus_one_node);
  
  if (tree_int_cst_sgn (b) == -1)
    b = tree_fold_int_multiply (b, integer_minus_one_node);
  
  a_minus_b = tree_fold_int_minus (a, b);
  
  if (integer_zerop (a_minus_b))
    return a;
  
  if (tree_int_cst_sgn (a_minus_b) == 1)
    return tree_fold_int_gcd (a_minus_b, b);
  
  return tree_fold_int_gcd (tree_fold_int_minus (b, a), a);
}

/* Computes the greatest common divisor using the Bezout algorithm.
   The gcd is returned, and the coefficients of the unimodular matrix
   U are (u11, u12, u21, u22) such that: U.A = S
   
   (u11 u12) (a1) = (gcd)
   (u21 u22) (a2)   (0)
*/

tree 
tree_fold_bezout (tree a1, 
		  tree a2,
		  tree *u11, tree *u12,
		  tree *u21, tree *u22)
{
  tree s1, s2;
  
  /* Initialize S with the coefficients of A.  */
  s1 = a1;
  s2 = a2;
  
  /* Initialize the U matrix */
  *u11 = integer_one_node; 
  *u12 = integer_zero_node;
  *u21 = integer_zero_node;
  *u22 = integer_one_node;
  
  while (!integer_zerop (s2))
    {
      int sign;
      tree z, zu21, zu22, zs2;
      
      sign = tree_int_cst_sgn (s1) * tree_int_cst_sgn (s2);
      z = tree_fold_int_floor_div (tree_fold_int_abs (s1), 
				   tree_fold_int_abs (s2));
      zu21 = tree_fold_int_multiply (z, *u21);
      zu22 = tree_fold_int_multiply (z, *u22);
      zs2 = tree_fold_int_multiply (z, s2);
      
      /* row1 -= z * row2.  */
      if (sign < 0)
	{
	  *u11 = tree_fold_int_plus (*u11, zu21);
	  *u12 = tree_fold_int_plus (*u12, zu22);
	  s1 = tree_fold_int_plus (s1, zs2);
	}
      else if (sign > 0)
	{
	  *u11 = tree_fold_int_minus (*u11, zu21);
	  *u12 = tree_fold_int_minus (*u12, zu22);
	  s1 = tree_fold_int_minus (s1, zs2);
	}
      else
	/* Should not happen.  */
	abort ();
      
      /* Interchange row1 and row2.  */
      {
	tree flip;
	
	flip = *u11;
	*u11 = *u21;
	*u21 = flip;

	flip = *u12;
	*u12 = *u22;
	*u22 = flip;
	
	flip = s1;
	s1 = s2;
	s2 = flip;
      }
    }
  
  if (tree_int_cst_sgn (s1) < 0)
    {
      *u11 = tree_fold_int_multiply (*u11, integer_minus_one_node);
      *u12 = tree_fold_int_multiply (*u12, integer_minus_one_node);
      s1 = tree_fold_int_multiply (s1, integer_minus_one_node);
    }
  
  return s1;
}

/* The factorial.  */

tree 
tree_fold_int_factorial (tree f)
{
  if (tree_int_cst_sgn (f) <= 0)
    return integer_one_node;
  else
    return tree_fold_int_multiply 
      (f, tree_fold_int_factorial (tree_fold_int_minus (f, integer_one_node)));
}

