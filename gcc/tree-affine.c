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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-dump.h"
#include "tree-affine.h"

/* Restricts constant CST according to precision of combination COMB.  */

static inline double_int
restrict_cst_to_precision (aff_tree *comb, double_int cst)
{
  double_int r;
  
  r.low = cst.low & comb->mask.low;
  r.high = cst.high & comb->mask.high;

  return r;
}

/* Returns true if X fits in HOST_WIDE_INT, with respect to precision
   of COMB.  */

bool
double_int_fits_in_hwi_p (aff_tree *comb, double_int x)
{
  return x.high == comb->mask.high;
}

/* Returns true if SCALE is negative in precision of COMB.  */

bool
double_int_negative_p (aff_tree *comb, double_int scale)
{
  if (comb->mask.high)
    return (scale.high & ~(comb->mask.high >> 1)) != 0;
  else
    return (scale.low & ~(comb->mask.low >> 1)) != 0;
}

/* Returns value of X with respect to precision of COMB.  */

HOST_WIDE_INT
double_int_to_hwi (aff_tree *comb, double_int x)
{
  if (comb->mask.high || !double_int_negative_p (comb, x))
    return x.low;
  else
    return x.low | ~comb->mask.low;
}

/* Returns A * B, truncated so that it fits COMB.  */

double_int
double_int_mul (aff_tree *comb, double_int a, double_int b)
{
  unsigned HOST_WIDE_INT lo;
  HOST_WIDE_INT hi;
  double_int ret;

  mul_double (a.low, a.high, b.low, b.high, &lo, &hi);
  ret.low = lo;
  ret.high = hi;

  return restrict_cst_to_precision (comb, ret);
}

/* Returns A + B, truncated so that it fits COMB.  */

double_int
double_int_add (aff_tree *comb, double_int a, double_int b)
{
  unsigned HOST_WIDE_INT lo;
  HOST_WIDE_INT hi;
  double_int ret;

  add_double (a.low, a.high, b.low, b.high, &lo, &hi);
  ret.low = lo;
  ret.high = hi;

  return restrict_cst_to_precision (comb, ret);
}

/* Returns -A, truncated so that it fits COMB.  */

double_int
double_int_negate (aff_tree *comb, double_int a)
{
  unsigned HOST_WIDE_INT lo;
  HOST_WIDE_INT hi;
  double_int ret;

  neg_double (a.low, a.high, &lo, &hi);
  ret.low = lo;
  ret.high = hi;

  return restrict_cst_to_precision (comb, ret);
}

/* Constructs tree in type TYPE from double_int CST.  */

tree
double_int_to_tree (tree type, double_int cst)
{
  unsigned HOST_WIDE_INT lo = cst.low;
  HOST_WIDE_INT hi = cst.high;
  unsigned prec = TYPE_PRECISION (type);
  bool negative;

  if (prec > HOST_BITS_PER_WIDE_INT)
    {
      prec -= HOST_BITS_PER_WIDE_INT;
      negative = (hi >> (prec - 1)) & 1;
      if (negative)
	hi |= (~(unsigned HOST_WIDE_INT) 0) << (prec - 1) << 1;
    }
  else
    {
      negative = (cst.low >> (prec - 1)) & 1;
      if (negative)
	{
	  lo |= (~(unsigned HOST_WIDE_INT) 0) << (prec - 1) << 1;
	  hi = ~(unsigned HOST_WIDE_INT) 0;
	}
    }

  return build_int_cst_wide (type, lo, hi);
}

/* Initializes mask of COMB according to its type.  */

static void
affine_combination_set_mask (aff_tree *comb)
{
  unsigned prec = TYPE_PRECISION (comb->type);

  if (prec > HOST_BITS_PER_WIDE_INT)
    {
      prec -= HOST_BITS_PER_WIDE_INT;
      comb->mask.high = (((unsigned HOST_WIDE_INT) 2 << (prec - 1)) - 1);
      comb->mask.low = ~(unsigned HOST_WIDE_INT) 0;
    }
  else
    {
      comb->mask.high = 0;
      comb->mask.low = (((unsigned HOST_WIDE_INT) 2 << (prec - 1)) - 1);
    }
}

/* Initializes affine combination COMB so that its value is zero in TYPE.  */

static void
aff_combination_zero (aff_tree *comb, tree type)
{
  comb->type = type;
  affine_combination_set_mask (comb);

  comb->offset.low = 0;
  comb->offset.high = 0;
  comb->n = 0;
  comb->rest = NULL_TREE;
}

/* Sets COMB to CST.  */

void
aff_combination_const (aff_tree *comb, tree type, double_int cst)
{
  aff_combination_zero (comb, type);
  comb->offset = restrict_cst_to_precision (comb, cst);
}

/* Sets COMB to single element ELT.  */

void
aff_combination_elt (aff_tree *comb, tree type, tree elt)
{
  aff_combination_zero (comb, type);

  comb->n = 1;
  comb->elts[0] = elt;
  comb->coefs[0] = hwi_to_double_int (1);
}

/* Scales COMB by SCALE.  */

void
aff_combination_scale (aff_tree *comb, double_int scale)
{
  unsigned i, j;

  scale = restrict_cst_to_precision (comb, scale);
  if (double_int_one_p (scale))
    return;

  if (double_int_zero_p (scale))
    {
      aff_combination_zero (comb, comb->type);
      return;
    }

  comb->offset = double_int_mul (comb, scale, comb->offset);
  for (i = 0, j = 0; i < comb->n; i++)
    {
      comb->coefs[j] = double_int_mul (comb, scale, comb->coefs[i]);
      comb->elts[j] = comb->elts[i];
      /* A coefficient may become zero due to overflow.  Remove the zero
	 elements.  */
      if (!double_int_zero_p (comb->coefs[j]))
	j++;
    }
  comb->n = j;

  if (comb->rest)
    {
      if (comb->n < MAX_AFF_ELTS)
	{
	  comb->coefs[comb->n] = scale;
	  comb->elts[comb->n] = comb->rest;
	  comb->rest = NULL_TREE;
	  comb->n++;
	}
      else
	comb->rest = fold_build2 (MULT_EXPR, comb->type, comb->rest, 
				  double_int_to_tree (comb->type, scale));
    }
}

/* Adds ELT * SCALE to COMB.  */

void
aff_combination_add_elt (aff_tree *comb, tree elt, double_int scale)
{
  unsigned i;

  if (double_int_zero_p (scale))
    return;

  for (i = 0; i < comb->n; i++)
    if (operand_equal_p (comb->elts[i], elt, 0))
      {
	comb->coefs[i] = double_int_add (comb, comb->coefs[i], scale);
	if (!double_int_zero_p (comb->coefs[i]))
	  return;

	comb->n--;
	comb->coefs[i] = comb->coefs[comb->n];
	comb->elts[i] = comb->elts[comb->n];
	return;
      }
  if (comb->n < MAX_AFF_ELTS)
    {
      comb->coefs[comb->n] = scale;
      comb->elts[comb->n] = elt;
      comb->n++;
      return;
    }

  if (double_int_one_p (scale))
    elt = fold_convert (comb->type, elt);
  else
    elt = fold_build2 (MULT_EXPR, comb->type,
		       fold_convert (comb->type, elt),
		       double_int_to_tree (comb->type, scale)); 

  if (comb->rest)
    comb->rest = fold_build2 (PLUS_EXPR, comb->type, comb->rest, elt);
  else
    comb->rest = elt;
}

/* Adds COMB2 to COMB1.  */

void
aff_combination_add (aff_tree *comb1, aff_tree *comb2)
{
  unsigned i;

  comb1->offset = double_int_add (comb1, comb1->offset, comb2->offset);
  for (i = 0; i < comb2->n; i++)
    aff_combination_add_elt (comb1, comb2->elts[i], comb2->coefs[i]);
  if (comb2->rest)
    aff_combination_add_elt (comb1, comb2->rest, hwi_to_double_int (1));
}

/* Converts affine combination COMB to TYPE.  */

void
aff_combination_convert (aff_tree *comb, tree type)
{
  unsigned i, j;
  tree comb_type = comb->type;

  gcc_assert (TYPE_PRECISION (type) <= TYPE_PRECISION (comb_type));
  comb->type = type;
  if (comb->rest)
    comb->rest = fold_convert (type, comb->rest);

  if (TYPE_PRECISION (type) == TYPE_PRECISION (comb_type))
    return;
  affine_combination_set_mask (comb);

  comb->offset = restrict_cst_to_precision (comb, comb->offset);
  for (i = j = 0; i < comb->n; i++)
    {
      comb->coefs[j] = restrict_cst_to_precision (comb, comb->coefs[i]);
      if (double_int_zero_p (comb->coefs[j]))
	continue;
      comb->elts[j] = fold_convert (type, comb->elts[i]);
      j++;
    }

  comb->n = j;
  if (comb->n < MAX_AFF_ELTS && comb->rest)
    {
      comb->coefs[comb->n] = hwi_to_double_int (1);
      comb->elts[comb->n] = comb->rest;
      comb->rest = NULL_TREE;
      comb->n++;
    }
}

/* Splits EXPR into an affine combination of parts.  */

void
tree_to_aff_combination (tree expr, tree type, aff_tree *comb)
{
  aff_tree tmp;
  enum tree_code code;
  tree cst, core, toffset;
  HOST_WIDE_INT bitpos, bitsize;
  enum machine_mode mode;
  int unsignedp, volatilep;

  STRIP_NOPS (expr);

  code = TREE_CODE (expr);
  switch (code)
    {
    case INTEGER_CST:
      aff_combination_const (comb, type, tree_to_double_int (expr));
      return;

    case PLUS_EXPR:
    case MINUS_EXPR:
      tree_to_aff_combination (TREE_OPERAND (expr, 0), type, comb);
      tree_to_aff_combination (TREE_OPERAND (expr, 1), type, &tmp);
      if (code == MINUS_EXPR)
	aff_combination_scale (&tmp, hwi_to_double_int (-1));
      aff_combination_add (comb, &tmp);
      return;

    case MULT_EXPR:
      cst = TREE_OPERAND (expr, 1);
      if (TREE_CODE (cst) != INTEGER_CST)
	break;
      tree_to_aff_combination (TREE_OPERAND (expr, 0), type, comb);
      aff_combination_scale (comb, tree_to_double_int (cst));
      return;

    case NEGATE_EXPR:
      tree_to_aff_combination (TREE_OPERAND (expr, 0), type, comb);
      aff_combination_scale (comb, hwi_to_double_int (-1));
      return;

    case ADDR_EXPR:
      core = get_inner_reference (TREE_OPERAND (expr, 0), &bitsize, &bitpos,
				  &toffset, &mode, &unsignedp, &volatilep,
				  false);
      if (bitpos % BITS_PER_UNIT != 0)
	break;
      aff_combination_const (comb, type,
			     hwi_to_double_int (bitpos / BITS_PER_UNIT));
      core = build_fold_addr_expr (core);
      if (TREE_CODE (core) == ADDR_EXPR)
	aff_combination_add_elt (comb, core, hwi_to_double_int (1));
      else
	{
	  tree_to_aff_combination (core, type, &tmp);
	  aff_combination_add (comb, &tmp);
	}
      if (toffset)
	{
	  tree_to_aff_combination (toffset, type, &tmp);
	  aff_combination_add (comb, &tmp);
	}
      return;

    default:
      break;
    }

  aff_combination_elt (comb, type, expr);
}

/* Creates EXPR + ELT * SCALE in TYPE.  EXPR is taken from affine
   combination COMB.  */

static tree
add_elt_to_tree (tree expr, tree type, tree elt, double_int scale,
		 aff_tree *comb)
{
  enum tree_code code;

  scale = restrict_cst_to_precision (comb, scale);
  elt = fold_convert (type, elt);

  if (double_int_one_p (scale))
    {
      if (!expr)
	return elt;

      return fold_build2 (PLUS_EXPR, type, expr, elt);
    }

  if (double_int_minus_one_p (comb, scale))
    {
      if (!expr)
	return fold_build1 (NEGATE_EXPR, type, elt);

      return fold_build2 (MINUS_EXPR, type, expr, elt);
    }

  if (!expr)
    return fold_build2 (MULT_EXPR, type, elt,
			double_int_to_tree (type, scale));

  if (double_int_negative_p (comb, scale))
    {
      code = MINUS_EXPR;
      scale = double_int_negate (comb, scale);
    }
  else
    code = PLUS_EXPR;

  elt = fold_build2 (MULT_EXPR, type, elt,
		     double_int_to_tree (type, scale));
  return fold_build2 (code, type, expr, elt);
}

/* Makes tree from the affine combination COMB.  */

tree
aff_combination_to_tree (aff_tree *comb)
{
  tree type = comb->type;
  tree expr = comb->rest;
  unsigned i;
  double_int off, sgn;

  gcc_assert (comb->n == MAX_AFF_ELTS || comb->rest == NULL_TREE);

  for (i = 0; i < comb->n; i++)
    expr = add_elt_to_tree (expr, type, comb->elts[i], comb->coefs[i], comb);

  if (double_int_negative_p (comb, comb->offset))
    {
      /* Offset is negative.  */
      off = double_int_negate (comb, comb->offset);
      sgn = comb->mask;
    }
  else
    {
      off = comb->offset;
      sgn = hwi_to_double_int (1);
    }
  return add_elt_to_tree (expr, type, double_int_to_tree (type, off), sgn,
			  comb);
}

/* Copies the tree elements of COMB to ensure that they are not shared.  */

void
unshare_aff_combination (aff_tree *comb)
{
  unsigned i;

  for (i = 0; i < comb->n; i++)
    comb->elts[i] = unshare_expr (comb->elts[i]);
  if (comb->rest)
    comb->rest = unshare_expr (comb->rest);
}

/* Remove M-th element from COMB.  */

void
aff_combination_remove_elt (aff_tree *comb, unsigned m)
{
  comb->n--;
  if (m <= comb->n)
    {
      comb->coefs[m] = comb->coefs[comb->n];
      comb->elts[m] = comb->elts[comb->n];
    }
  if (comb->rest)
    {
      comb->coefs[comb->n] = hwi_to_double_int (1);
      comb->elts[comb->n] = comb->rest;
      comb->rest = NULL_TREE;
      comb->n++;
    }
}
