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

