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
restrict_cst_to_precision (double_int mask, double_int cst)
{
  double_int r;
  
  r.low = cst.low & mask.low;
  r.high = cst.high & mask.high;

  return r;
}

/* Returns true if X fits in HOST_WIDE_INT, with respect to precision
   of MASK.  */

bool
double_int_fits_in_hwi_p (double_int mask, double_int x)
{
  if (double_int_negative_p (mask, x))
    return x.high == mask.high;
  else
    return x.high == 0 && (HOST_WIDE_INT) x.low >= 0;
}

/* Returns true if X fits in unsigned HOST_WIDE_INT.  */

bool
double_int_fits_in_unsigned_hwi_p (double_int x)
{
  return x.high == 0;
}

/* Returns true if SCALE is negative in precision of MASK.  */

bool
double_int_negative_p (double_int mask, double_int scale)
{
  if (mask.high)
    return (scale.high & ~(mask.high >> 1)) != 0;
  else
    return (scale.low & ~(mask.low >> 1)) != 0;
}

/* Returns value of X with respect to precision of MASK.  */

HOST_WIDE_INT
double_int_to_hwi (double_int mask, double_int x)
{
  if (mask.high || !double_int_negative_p (mask, x))
    return x.low;
  else
    return x.low | ~mask.low;
}

/* Returns value of X.  */

unsigned HOST_WIDE_INT
double_int_to_unsigned_hwi (double_int x)
{
  return x.low;
}

/* Returns A * B, truncated so that it fits MASK.  */

double_int
double_int_mul (double_int mask, double_int a, double_int b)
{
  unsigned HOST_WIDE_INT lo;
  HOST_WIDE_INT hi;
  double_int ret;

  mul_double (a.low, a.high, b.low, b.high, &lo, &hi);
  ret.low = lo;
  ret.high = hi;

  return restrict_cst_to_precision (mask, ret);
}

/* Returns A + B, truncated so that it fits MASK.  */

double_int
double_int_add (double_int mask, double_int a, double_int b)
{
  unsigned HOST_WIDE_INT lo;
  HOST_WIDE_INT hi;
  double_int ret;

  add_double (a.low, a.high, b.low, b.high, &lo, &hi);
  ret.low = lo;
  ret.high = hi;

  return restrict_cst_to_precision (mask, ret);
}

/* Returns -A, truncated so that it fits MASK.  */

double_int
double_int_negate (double_int mask, double_int a)
{
  unsigned HOST_WIDE_INT lo;
  HOST_WIDE_INT hi;
  double_int ret;

  neg_double (a.low, a.high, &lo, &hi);
  ret.low = lo;
  ret.high = hi;

  return restrict_cst_to_precision (mask, ret);
}

/* Returns A / B (computed as unsigned, rounded down).  */

double_int
double_int_divide (double_int a, double_int b)
{
  unsigned HOST_WIDE_INT lo, rem_lo;
  HOST_WIDE_INT hi, rem_hi;
  double_int ret;

  div_and_round_double (FLOOR_DIV_EXPR, true, a.low, a.high, b.low, b.high,
			&lo, &hi, &rem_lo, &rem_hi);
  ret.low = lo;
  ret.high = hi;

  return ret;
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

/* Returns true if VAL is smaller or equal to the maximal value
   representable in TYPE.  */

bool
double_int_fits_to_type_p (tree type, double_int val)
{
  unsigned prec = TYPE_PRECISION (type);
  double_int mask = double_int_mask (TYPE_UNSIGNED (type) ? prec : prec - 1);

  return (((val.low & mask.low) == val.low)
	  && ((val.high & mask.high) == val.high));
}

/* Returns true if A < B, unsigned comparison.  */

bool
double_int_smaller_p (double_int a, double_int b)
{
  if (a.high < b.high)
    return true;
  if (a.high > b.high)
    return false;
  return a.low < b.low;
}

/* Splits last digit of *X in BASE and returns it.  */

static unsigned
double_int_split_digit (double_int *x, unsigned base)
{
  unsigned HOST_WIDE_INT resl, reml;
  HOST_WIDE_INT resh, remh;

  div_and_round_double (FLOOR_DIV_EXPR, true, x->low, x->high, base, 0,
			&resl, &resh, &reml, &remh);
  x->high = resh;
  x->low = resl;

  return reml;
}

/* Dumps X (in precision MASK) to FILE.  If SIGN is true, X is considered to
   be signed.  */

void
dump_double_int (FILE *file, double_int mask, double_int x, bool sign)
{
  unsigned digits[100], n;
  int i;

  if (double_int_zero_p (x))
    {
      fprintf (file, "0");
      return;
    }

  if (sign && double_int_negative_p (mask, x))
    {
      fprintf (file, "-");
      x = double_int_negate (mask, x);
    }

  for (n = 0; !double_int_zero_p (x); n++)
    digits[n] = double_int_split_digit (&x, 10);
  for (i = n - 1; i >= 0; i--)
    fprintf (file, "%u", digits[i]);
}

/* Returns a MASK for PREC bytes.  */

double_int 
double_int_mask (unsigned prec)
{
  double_int mask;

  if (prec > HOST_BITS_PER_WIDE_INT)
    {
      prec -= HOST_BITS_PER_WIDE_INT;
      mask.high = (((unsigned HOST_WIDE_INT) 2 << (prec - 1)) - 1);
      mask.low = ~(unsigned HOST_WIDE_INT) 0;
    }
  else
    {
      mask.high = 0;
      mask.low = (((unsigned HOST_WIDE_INT) 2 << (prec - 1)) - 1);
    }

  return mask;
}

/* Initializes affine combination COMB so that its value is zero in TYPE.  */

static void
aff_combination_zero (aff_tree *comb, tree type)
{
  comb->type = type;
  comb->mask = double_int_mask (TYPE_PRECISION (type));

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
  comb->offset = restrict_cst_to_precision (comb->mask, cst);
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

  scale = restrict_cst_to_precision (comb->mask, scale);
  if (double_int_one_p (scale))
    return;

  if (double_int_zero_p (scale))
    {
      aff_combination_zero (comb, comb->type);
      return;
    }

  comb->offset = double_int_mul (comb->mask, scale, comb->offset);
  for (i = 0, j = 0; i < comb->n; i++)
    {
      comb->coefs[j] = double_int_mul (comb->mask, scale, comb->coefs[i]);
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
	comb->coefs[i] = double_int_add (comb->mask, comb->coefs[i], scale);
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

  comb1->offset = double_int_add (comb1->mask, comb1->offset, comb2->offset);
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
  comb->mask = double_int_mask (TYPE_PRECISION (type));

  comb->offset = restrict_cst_to_precision (comb->mask, comb->offset);
  for (i = j = 0; i < comb->n; i++)
    {
      comb->coefs[j] = restrict_cst_to_precision (comb->mask, comb->coefs[i]);
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

  scale = restrict_cst_to_precision (comb->mask, scale);
  elt = fold_convert (type, elt);

  if (double_int_one_p (scale))
    {
      if (!expr)
	return elt;

      return fold_build2 (PLUS_EXPR, type, expr, elt);
    }

  if (double_int_minus_one_p (comb->mask, scale))
    {
      if (!expr)
	return fold_build1 (NEGATE_EXPR, type, elt);

      return fold_build2 (MINUS_EXPR, type, expr, elt);
    }

  if (!expr)
    return fold_build2 (MULT_EXPR, type, elt,
			double_int_to_tree (type, scale));

  if (double_int_negative_p (comb->mask, scale))
    {
      code = MINUS_EXPR;
      scale = double_int_negate (comb->mask, scale);
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

  if (double_int_negative_p (comb->mask, comb->offset))
    {
      /* Offset is negative.  */
      off = double_int_negate (comb->mask, comb->offset);
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
