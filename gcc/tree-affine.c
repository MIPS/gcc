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
#include "tree-gimple.h"
#include "hashtab.h"

/* Extends CST as appropriate for the affine combinations COMB.  */

double_int
double_int_ext_for_comb (double_int cst, aff_tree *comb)
{
  return double_int_sext (cst, TYPE_PRECISION (comb->type));
}

/* Initializes affine combination COMB so that its value is zero in TYPE.  */

static void
aff_combination_zero (aff_tree *comb, tree type)
{
  comb->type = type;
  comb->offset = double_int_zero;
  comb->n = 0;
  comb->rest = NULL_TREE;
}

/* Sets COMB to CST.  */

void
aff_combination_const (aff_tree *comb, tree type, double_int cst)
{
  aff_combination_zero (comb, type);
  comb->offset = double_int_ext_for_comb (cst, comb);
}

/* Sets COMB to single element ELT.  */

void
aff_combination_elt (aff_tree *comb, tree type, tree elt)
{
  aff_combination_zero (comb, type);

  comb->n = 1;
  comb->elts[0].val = elt;
  comb->elts[0].coef = double_int_one;
}

/* Scales COMB by SCALE.  */

void
aff_combination_scale (aff_tree *comb, double_int scale)
{
  unsigned i, j;

  scale = double_int_ext_for_comb (scale, comb);
  if (double_int_one_p (scale))
    return;

  if (double_int_zero_p (scale))
    {
      aff_combination_zero (comb, comb->type);
      return;
    }

  comb->offset
    = double_int_ext_for_comb (double_int_mul (scale, comb->offset), comb);
  for (i = 0, j = 0; i < comb->n; i++)
    {
      double_int new_coef;

      new_coef
	= double_int_ext_for_comb (double_int_mul (scale, comb->elts[i].coef),
				   comb);
      /* A coefficient may become zero due to overflow.  Remove the zero
	 elements.  */
      if (double_int_zero_p (new_coef))
	continue;
      comb->elts[j].coef = new_coef;
      comb->elts[j].val = comb->elts[i].val;
      j++;
    }
  comb->n = j;

  if (comb->rest)
    {
      if (comb->n < MAX_AFF_ELTS)
	{
	  comb->elts[comb->n].coef = scale;
	  comb->elts[comb->n].val = comb->rest;
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

  scale = double_int_ext_for_comb (scale, comb);
  if (double_int_zero_p (scale))
    return;

  for (i = 0; i < comb->n; i++)
    if (operand_equal_p (comb->elts[i].val, elt, 0))
      {
	double_int new_coef;

	new_coef = double_int_add (comb->elts[i].coef, scale);
	new_coef = double_int_ext_for_comb (new_coef, comb);
	if (!double_int_zero_p (new_coef))
	  {
	    comb->elts[i].coef = new_coef;
	    return;
	  }

	comb->n--;
	comb->elts[i] = comb->elts[comb->n];

	if (comb->rest)
	  {
	    gcc_assert (comb->n == MAX_AFF_ELTS - 1);
	    comb->elts[comb->n].coef = double_int_one;
	    comb->elts[comb->n].val = comb->rest;
	    comb->rest = NULL_TREE;
	    comb->n++;
	  }
	return;
      }
  if (comb->n < MAX_AFF_ELTS)
    {
      comb->elts[comb->n].coef = scale;
      comb->elts[comb->n].val = elt;
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

/* Adds CST to C.  */

static void
aff_combination_add_cst (aff_tree *c, double_int cst)
{
  c->offset = double_int_ext_for_comb (double_int_add (c->offset, cst), c);
}

/* Adds COMB2 to COMB1.  */

void
aff_combination_add (aff_tree *comb1, aff_tree *comb2)
{
  unsigned i;

  aff_combination_add_cst (comb1, comb2->offset);
  for (i = 0; i < comb2->n; i++)
    aff_combination_add_elt (comb1, comb2->elts[i].val, comb2->elts[i].coef);
  if (comb2->rest)
    aff_combination_add_elt (comb1, comb2->rest, double_int_one);
}

/* Converts affine combination COMB to TYPE.  */

void
aff_combination_convert (aff_tree *comb, tree type)
{
  unsigned i, j;
  tree comb_type = comb->type;

  if  (TYPE_PRECISION (type) > TYPE_PRECISION (comb_type))
    {
      tree val = fold_convert (type, aff_combination_to_tree (comb));
      tree_to_aff_combination (val, type, comb);
      return;
    }

  comb->type = type;
  if (comb->rest)
    comb->rest = fold_convert (type, comb->rest);

  if (TYPE_PRECISION (type) == TYPE_PRECISION (comb_type))
    return;

  comb->offset = double_int_ext_for_comb (comb->offset, comb);
  for (i = j = 0; i < comb->n; i++)
    {
      double_int new_coef = double_int_ext_for_comb (comb->elts[i].coef, comb);
      if (double_int_zero_p (new_coef))
	continue;
      comb->elts[j].coef = new_coef;
      comb->elts[j].val = fold_convert (type, comb->elts[i].val);
      j++;
    }

  comb->n = j;
  if (comb->n < MAX_AFF_ELTS && comb->rest)
    {
      comb->elts[comb->n].coef = double_int_one;
      comb->elts[comb->n].val = comb->rest;
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
	aff_combination_scale (&tmp, double_int_minus_one);
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
      aff_combination_scale (comb, double_int_minus_one);
      return;

    case BIT_NOT_EXPR:
      /* ~x = -x - 1 */
      tree_to_aff_combination (TREE_OPERAND (expr, 0), type, comb);
      aff_combination_scale (comb, double_int_minus_one);
      aff_combination_add_cst (comb, double_int_minus_one);
      return;

    case ADDR_EXPR:
      core = get_inner_reference (TREE_OPERAND (expr, 0), &bitsize, &bitpos,
				  &toffset, &mode, &unsignedp, &volatilep,
				  false);
      if (bitpos % BITS_PER_UNIT != 0)
	break;
      aff_combination_const (comb, type,
			     uhwi_to_double_int (bitpos / BITS_PER_UNIT));
      core = build_fold_addr_expr (core);
      if (TREE_CODE (core) == ADDR_EXPR)
	aff_combination_add_elt (comb, core, double_int_one);
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

  scale = double_int_ext_for_comb (scale, comb);
  elt = fold_convert (type, elt);

  if (double_int_one_p (scale))
    {
      if (!expr)
	return elt;

      return fold_build2 (PLUS_EXPR, type, expr, elt);
    }

  if (double_int_minus_one_p (scale))
    {
      if (!expr)
	return fold_build1 (NEGATE_EXPR, type, elt);

      return fold_build2 (MINUS_EXPR, type, expr, elt);
    }

  if (!expr)
    return fold_build2 (MULT_EXPR, type, elt,
			double_int_to_tree (type, scale));

  if (double_int_negative_p (scale))
    {
      code = MINUS_EXPR;
      scale = double_int_neg (scale);
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
    expr = add_elt_to_tree (expr, type, comb->elts[i].val, comb->elts[i].coef,
			    comb);

  /* Ensure that we get x - 1, not x + (-1) or x + 0xff..f if x is
     unsigned.  */
  if (double_int_negative_p (comb->offset))
    {
      off = double_int_neg (comb->offset);
      sgn = double_int_minus_one;
    }
  else
    {
      off = comb->offset;
      sgn = double_int_one;
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
    comb->elts[i].val = unshare_expr (comb->elts[i].val);
  if (comb->rest)
    comb->rest = unshare_expr (comb->rest);
}

/* Remove M-th element from COMB.  */

void
aff_combination_remove_elt (aff_tree *comb, unsigned m)
{
  comb->n--;
  if (m <= comb->n)
    comb->elts[m] = comb->elts[comb->n];
  if (comb->rest)
    {
      comb->elts[comb->n].coef = double_int_one;
      comb->elts[comb->n].val = comb->rest;
      comb->rest = NULL_TREE;
      comb->n++;
    }
}

/* Adds C * COEF * VAL to R.  VAL may be NULL, in that case only
   C * COEF is added to R.  */
   

static void
aff_combination_add_product (aff_tree *c, double_int coef, tree val,
			     aff_tree *r)
{
  unsigned i;
  tree aval, type;

  for (i = 0; i < c->n; i++)
    {
      aval = c->elts[i].val;
      if (val)
	{
	  type = TREE_TYPE (aval);
	  aval = fold_build2 (MULT_EXPR, type, aval,
			      fold_convert (type, val));
	}

      aff_combination_add_elt (r, aval,
			       double_int_mul (coef, c->elts[i].coef));
    }

  if (c->rest)
    {
      aval = c->rest;
      if (val)
	{
	  type = TREE_TYPE (aval);
	  aval = fold_build2 (MULT_EXPR, type, aval,
			      fold_convert (type, val));
	}

      aff_combination_add_elt (r, aval, coef);
    }

  if (val)
    aff_combination_add_elt (r, val,
			     double_int_mul (coef, c->offset));
  else
    aff_combination_add_cst (r, double_int_mul (coef, c->offset));
}

/* Multiplies C1 by C2, storing the result to R  */

void
aff_combination_mult (aff_tree *c1, aff_tree *c2, aff_tree *r)
{
  unsigned i;
  gcc_assert (TYPE_PRECISION (c1->type) == TYPE_PRECISION (c2->type));

  aff_combination_zero (r, c1->type);

  for (i = 0; i < c2->n; i++)
    aff_combination_add_product (c1, c2->elts[i].coef, c2->elts[i].val, r);
  if (c2->rest)
    aff_combination_add_product (c1, double_int_one, c2->rest, r);
  aff_combination_add_product (c1, c2->offset, NULL, r);
}

/* Element of the cache that maps ssa name NAME to its expanded form
   as an affine expression EXPANSION.  */

struct name_expansion
{
  aff_tree expansion;
  tree name;

  /* True if the expansion for the name is just being generated.  */
  unsigned in_progress : 1;
};

/* Hash function for struct name_expansion.  */

static hashval_t
name_expansion_hash (const void *e)
{
  return SSA_NAME_VERSION (((struct name_expansion *) e)->name);
}

/* Equality function for struct name_expansion.  The second argument is an
   SSA name.  */

static int
name_expansion_eq (const void *e, const void *n)
{
  return ((struct name_expansion *) e)->name == n;
}

/* Expands SSA names in COMB recursively.  CACHE is used to cache the
   results.  */

void
aff_combination_expand (aff_tree *comb, htab_t *cache)
{
  unsigned i;
  aff_tree to_add, current, curre;
  tree e, def, rhs;
  double_int scale;
  void **slot;
  struct name_expansion *exp;

  aff_combination_zero (&to_add, comb->type);
  for (i = 0; i < comb->n; i++)
    {
      e = comb->elts[i].val;
      if (TREE_CODE (e) != SSA_NAME)
	continue;
      def = SSA_NAME_DEF_STMT (e);
      if (TREE_CODE (def) != GIMPLE_MODIFY_STMT
	  || GIMPLE_STMT_OPERAND (def, 0) != e)
	continue;

      rhs = GIMPLE_STMT_OPERAND (def, 1);
      if (TREE_CODE (rhs) != SSA_NAME
	  && !EXPR_P (rhs)
	  && !is_gimple_min_invariant (rhs))
	continue;

      /* We do not know whether the reference retains its value at the
	 place where the expansion is used.  */
      if (REFERENCE_CLASS_P (rhs))
	continue;

      /* Also, we do not want to return call_exprs.  */
      if (get_call_expr_in (def) != NULL)
	continue;

      if (!*cache)
	*cache = htab_create (10, name_expansion_hash, name_expansion_eq,
			      free);
      slot = htab_find_slot_with_hash (*cache, e, SSA_NAME_VERSION (e), INSERT);
      exp = *slot;

      if (exp)
	{
	  /* Since we follow the definitions in the SSA form, we should not
	     enter a cycle unless we pass through a phi node.  */
	  gcc_assert (!exp->in_progress);
	  current = exp->expansion;
	}
      else
	{
	  exp = XNEW (struct name_expansion);
	  exp->name = e;
	  exp->in_progress = 1;
	  *slot = exp;
	  tree_to_aff_combination_expand (rhs, comb->type, &current, cache);
	  exp->expansion = current;
	  exp->in_progress = 0;
	}

      /* Accumulate the new terms to TO_ADD, so that we do not modify
	 COMB while traversing it; include the term -coef * E, to remove
         it from COMB.  */
      scale = comb->elts[i].coef;
      aff_combination_zero (&curre, comb->type);
      aff_combination_add_elt (&curre, e, double_int_neg (scale));
      aff_combination_scale (&current, scale);
      aff_combination_add (&to_add, &current);
      aff_combination_add (&to_add, &curre);
    }
  aff_combination_add (comb, &to_add);
}

/* Similar to tree_to_aff_combination, but follows SSA name definitions
   and expands them recursively.  CACHE is used to cache the expansions
   of the ssa names, to avoid exponential time complexity for cases
   like
 
   a1 = a0 + a0;
   a2 = a1 + a1;
   a3 = a2 + a2;
   ...  */

void
tree_to_aff_combination_expand (tree expr, tree type, aff_tree *comb,
				htab_t *cache)
{
  tree_to_aff_combination (expr, type, comb);
  aff_combination_expand (comb, cache);
}

/* Returns address of the reference REF in ADDR.  The size of the accessed
   location is stored to SIZE.  */

void
get_inner_reference_aff (tree ref, aff_tree *addr, double_int *size)
{
  HOST_WIDE_INT bitsize, bitpos;
  tree toff;
  enum machine_mode mode;
  int uns, vol;
  aff_tree tmp;
  tree base = get_inner_reference (ref, &bitsize, &bitpos, &toff, &mode,
				   &uns, &vol, false);
  tree base_addr = build_fold_addr_expr (base);

  /* ADDR = &BASE + TOFF + BITPOS / BITS_PER_UNIT.  */

  tree_to_aff_combination (base_addr, sizetype, addr);

  if (toff)
    {
      tree_to_aff_combination (toff, sizetype, &tmp);
      aff_combination_add (addr, &tmp);
    }

  aff_combination_const (&tmp, sizetype,
			 shwi_to_double_int (bitpos / BITS_PER_UNIT));
  aff_combination_add (addr, &tmp);

  *size = shwi_to_double_int ((bitsize + BITS_PER_UNIT - 1) / BITS_PER_UNIT);
}

