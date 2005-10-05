/* Operations with affine combinations of trees.
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

/* Affine combination of trees.  We keep track of at most MAX_AFF_ELTS elements
   to make things simpler; this is sufficient in most cases.  */

#define MAX_AFF_ELTS 8

typedef struct affine_tree_combination
{
  /* Type of the result of the combination.  */
  tree type;

  /* Mask modulo that the operations are performed.  */
  double_int mask;

  /* Constant offset.  */
  double_int offset;

  /* Number of elements of the combination.  */
  unsigned n;

  /* Elements and their coefficients.  Type of elements may be different from
     TYPE, but their sizes must be the same (STRIP_NOPS is applied to the
     elements).  */
  tree elts[MAX_AFF_ELTS];
  double_int coefs[MAX_AFF_ELTS];

  /* Remainder of the expression.  Usually NULL, used only if there are more
     than MAX_AFF_ELTS elements.  Type of REST must be TYPE.  */
  tree rest;
} aff_tree;

void aff_combination_const (aff_tree *, tree, double_int);
void aff_combination_elt (aff_tree *, tree, tree);
void aff_combination_scale (aff_tree *, double_int);
void aff_combination_add (aff_tree *, aff_tree *);
void aff_combination_add_elt (aff_tree *, tree, double_int);
void aff_combination_remove_elt (aff_tree *, unsigned);
void aff_combination_convert (aff_tree *, tree);
void tree_to_aff_combination (tree, tree, aff_tree *);
tree aff_combination_to_tree (aff_tree *);
void unshare_aff_combination (aff_tree *);
tree double_int_to_tree (tree, double_int);
bool double_int_fits_to_type_p (tree, double_int);
bool double_int_fits_in_hwi_p (double_int, double_int);
HOST_WIDE_INT double_int_to_hwi (double_int, double_int);
bool double_int_fits_in_unsigned_hwi_p (double_int);
unsigned HOST_WIDE_INT double_int_to_unsigned_hwi (double_int);
double_int double_int_mul (double_int, double_int, double_int);
double_int double_int_add (double_int, double_int, double_int);
double_int double_int_negate (double_int, double_int);
double_int double_int_divide (double_int, double_int);
bool double_int_negative_p (double_int, double_int);
bool double_int_smaller_p (double_int, double_int);
void dump_double_int (FILE *, double_int, double_int, bool);
double_int double_int_mask (unsigned);

/* Constructs double_int from tree CST.  */

static inline double_int
tree_to_double_int (tree cst)
{
  double_int r;
  
  r.low = TREE_INT_CST_LOW (cst);
  r.high = TREE_INT_CST_HIGH (cst);

  return r;
}

/* Constructs double_int from integer CST.  */

static inline double_int
hwi_to_double_int (HOST_WIDE_INT cst)
{
  double_int r;
  
  r.low = cst;
  r.high = cst < 0 ? ~(unsigned HOST_WIDE_INT) 0 : 0;

  return r;
}

/* Constructs mask with all bits 1.  */

static inline double_int
double_int_all (void)
{
  return hwi_to_double_int (-1);
}

/* Returns true if CST is zero.  */

static inline bool
double_int_zero_p (double_int cst)
{
  return cst.low == 0 && cst.high == 0;
}

/* Returns true if CST is one.  */

static inline bool
double_int_one_p (double_int cst)
{
  return cst.low == 1 && cst.high == 0;
}

/* Returns true if CST is minus one in precision of MASK.  */

static inline bool
double_int_minus_one_p (double_int mask, double_int cst)
{
  return cst.low == mask.low && cst.high == mask.high;
}

/* Returns true if CST1 == CST2.  */

static inline bool
double_int_equal_p (double_int cst1, double_int cst2)
{
  return cst1.low == cst2.low && cst1.high == cst2.high;
}

