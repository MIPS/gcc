/* A functions for algebraic simplification and manipulation with
   expressions.
   Copyright (C) 2003 Free Software Foundation, Inc.

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

/* This file contains some utilities for a manipulation with algebraic
   expressions (that are expressed in rtl).  It is used by induction
   variable analysis and the operations it does are therefore biased
   to this purpose, but it could be useful on other places too.  It also
   handles some nonstandard rtl codes due to this usage; the description
   of their semantics and usage can be found in loop-iv.c.

   While most of the file deals with the simplification of expressions,
   it is quite different from what is provided by simplify-rtx.c.  While
   utilities in that file understand by simple "fast to compute", in our
   sense simple means "in a unique canonical shape".  You should be very
   careful if you ever use expressions produced by our simplification in
   the compiled program.

   The main functions provided are simplify_alg_expr and substitute_into_expr.
   The exact specification of what they do can be found in comments before
   them.  The simplification is done in-place in order to reduce amount
   of memory wasted; so unless you are sure this is what you want, you
   must copy the rtx passed to it (this does not concern substitute_into_expr
   that does this automatically).  */

#define EL_RTX

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#ifdef EL_RTX
#include "rtl.h"
#include "expr.h"
#endif
#include "sbitmap.h"
#include "algebraic.h"
#include "ggc.h"

static sbitmap suitable_operator;	/* Bitmap of operators we are able
					   to handle.  */

#ifdef EL_RTX
/* Shared iteration rtxes in different modes.  */
static GTY (()) rtx iteration_rtx[NUM_MACHINE_MODES];
#endif

static bool equal_when_iteration_zero_p	PARAMS ((EXPR, EXPR));
static EXPR straighten_ops		PARAMS ((EXPR));
static EXPR sort_ops			PARAMS ((EXPR));
static int alg_compare_expr		PARAMS ((EXPR, EXPR));
static EXPR use_distributive_law	PARAMS ((EXPR));
static EXPR simplify_alg_expr_mult	PARAMS ((EXPR));
static EXPR simplify_alg_expr_plus	PARAMS ((EXPR));
static EXPR simplify_alg_expr_relational PARAMS ((EXPR, EXPR_TYPE));
static EXPR float_if_then_else		PARAMS ((EXPR));
static EXPR simplify_alg_expr_one_level	PARAMS ((EXPR, EXPR_TYPE));
static bool combine_constants		PARAMS ((EXPR, EXPR, EXPR_TYPE,
						 EXPR *));
static EXPR compare_with_type_bounds	PARAMS ((OPERATOR,
						 EXPR,
						 EXPR_TYPE,
						 EXPR_TYPE));

/* Checks whether EXPR1 == EXPR2, provided that iteration == 0.  */
static bool
equal_when_iteration_zero_p (expr1, expr2)
     EXPR expr1;
     EXPR expr2;
{
  EXPR e1, e2;
  EXPR zero = CONST_INT_EXPR (0);

  /* ??? It might be better to have a specialized version, in order not to
     copy the rtl unnecesarily.  */
  e1 = substitute_into_expr (expr1, NULL, &zero, SIE_SIMPLIFY);
  e2 = substitute_into_expr (expr2, NULL, &zero, SIE_SIMPLIFY);
  return e1 && e2 && EXPR_EQUAL_P (e1, e2);
}

/* Returns EXPR1 <=> EXPR2 in order given by lexicografical extension
   of the following ordering:

   integer constants by value < other constants < regs by number <
   value_at by register and insn id < other rtxes by code < iteration.
   Note that the fact that we return 0 does not neccessarily imply that
   expressions are equal.  */
static int
alg_compare_expr (expr1, expr2)
     EXPR expr1;
     EXPR expr2;
{
  int i, length, cmp;
  OPERATOR op1 = GET_OPERATOR (expr1), op2 = GET_OPERATOR (expr2);

  if (EXPR_EQUAL_P (expr1, expr2))
    return 0;

  if (op1 == OP_CONST_INT)
    {
      if (op2 != OP_CONST_INT)
	return -1;
      return CONST_INT_VALUE (expr1) < CONST_INT_VALUE (expr2) ? -1 : 1;
    }
  if (op2 == OP_CONST_INT)
    return 1;

  if (EXPR_CONSTANT_P (expr1))
    return EXPR_CONSTANT_P (expr2) ? 0 : -1;
  if  (EXPR_CONSTANT_P (expr2))
    return 1;

  if (op1 == OP_VARIABLE)
    {
      if (op2 != OP_VARIABLE)
	return -1;
      return VARIABLE_ID (expr1) < VARIABLE_ID (expr2) ? -1 : 1;
    }
  if (op2 == OP_VARIABLE)
    return 1;

  if (op1 == OP_UNSPECIFIED_VALUE)
    {
      if (op2 != OP_UNSPECIFIED_VALUE)
	return -1;
      return EXPR_COMPARE_UNSPECIFIED_VALUES (expr1, expr2);
    }
  if (op2 == OP_UNSPECIFIED_VALUE)
    return 1;

  if (op1 == OP_ITERATION)
    return 1;
  if (op2 == OP_ITERATION)
    return -1;

  if (op1 < op2)
    return -1;
  if (op1 > op2)
    return 1;

  /* We prefer rightmost operand here, so that expressions that differ
     only in the value of a constant term are sorted adjacent in
     simplify_alg_expr_plus.  */
  length = NUM_OP_ARGS (op1);
  for (i = length - 1; i >= 0; i--)
    if (ITH_ARG_EXPR_P (op1, i))
      {
	cmp = alg_compare_expr (ARG (expr1, i), ARG (expr2, i));
	if (cmp)
	  return cmp;
      }

  return 0;
}

/* Given expression in shape (((((a op b) op c) op d) op e) op f), return
   expression in the same shape but the operands sorted according to
   alg_compare_expr.  The sorting is done in-place using bubble sort.  */
static EXPR
sort_ops (expr)
     EXPR expr;
{
  EXPR pos, *op1, *op2, tmp;
  OPERATOR code = GET_OPERATOR (expr);
  int changed = true;

  if (code != OP_PLUS
      && code != OP_MULT)
    return expr;

  while (changed)
    {
      changed = false;
      pos = expr;
      while (GET_OPERATOR (pos) == code)
	{
	  op1 = &ARG (pos, 1);
	  op2 = GET_OPERATOR (ARG (pos, 0)) == code
	    ? &ARG (ARG (pos, 0), 1)
	    : &ARG (pos, 0);

	  if (alg_compare_expr (*op1, *op2) < 0)
	    {
	      tmp = *op1;
	      *op1 = *op2;
	      *op2 = tmp;
	      changed = true;
	    }
	  pos = ARG (pos, 0);
	}
    }
  return expr;
}

/* Given expression in shape ((a op b) op c) op ((d op e) op f),
   transform it into (((((a op b) op c) op d) op e) op f).  The straightening
   is done in-place.  */
static EXPR
straighten_ops (expr)
     EXPR expr;
{
  EXPR tmp;
  OPERATOR code = GET_OPERATOR (expr);

  if (code != OP_PLUS
      && code != OP_MULT)
    return expr;

  while (GET_OPERATOR (ARG (expr, 1)) == code)
    {
      tmp = expr;
      expr = ARG (expr, 1);
      ARG (tmp, 1) = ARG (expr, 1);
      ARG (expr, 1) = ARG (expr, 0);
      ARG (expr, 0) = tmp;
    }

  return expr;
}

/* EXPR is product of two expressions in the format described at
   simplify_alg_expr.  Use distributive law to express it as sum of
   products.  The transformation is partially done in-place.  */
static EXPR
use_distributive_law (expr)
     EXPR expr;
{
  EXPR exprl, exprr, aexprr, ml, mr, prod, tmp, *act;
  EXPR result = NULL_EXPR, *ares = &result;
  EXPR_TYPE mode = GET_EXPR_TYPE (expr);
  
  exprl = ARG (expr, 0);
  exprr = ARG (expr, 1);

  if (GET_OPERATOR (exprr) != OP_PLUS)
    {
      if (GET_OPERATOR (exprl) != OP_PLUS)
	return straighten_ops (expr);
      tmp = exprl;
      exprl = exprr;
      exprr = tmp;
    }

  while (GET_OPERATOR (exprl) == OP_PLUS)
    {
      aexprr = exprr;
      while (1)
	{
	  ml = COPY_EXPR (ARG (exprl, 1));
	  mr = GET_OPERATOR (aexprr) == OP_PLUS
		  ? COPY_EXPR (ARG (aexprr, 1))
		  : COPY_EXPR (aexprr);
	  prod = straighten_ops (GEN_BINARY (OP_MULT, mode, ml, mr));
	  *ares = GEN_BINARY (OP_PLUS, mode, NULL_EXPR, prod);
	  ares = &ARG (*ares, 0);

	  if (GET_OPERATOR (aexprr) != OP_PLUS)
	    break;
	  aexprr = ARG (aexprr, 0);
	}
      exprl = ARG (exprl, 0);
    }

  act = &exprr;
  while (GET_OPERATOR (*act) == OP_PLUS)
    {
      ml = COPY_EXPR (exprl);
      mr = ARG (*act, 1);
      tmp = straighten_ops (GEN_BINARY (OP_MULT, mode, ml, mr));
      ARG (*act, 1) = tmp;
      act = &ARG (*act, 0);
    }
  *act = straighten_ops (GEN_BINARY (OP_MULT, mode, exprl, *act));
  *ares = exprr;
  
  return result;
}

/* Simplify expression EXPR (some product).  We just fold constants and
   sort operands according to alg_compare_expr here.  The simplification
   is done in-place.  */
static EXPR
simplify_alg_expr_mult (expr)
     EXPR expr;
{
  EXPR_TYPE mode = GET_EXPR_TYPE (expr);
  EXPR folded_constant = CONST_INT_EXPR (1);
  EXPR *act = &expr, *last = NULL;
  EXPR op;

  if (GET_OPERATOR (expr) != OP_MULT)
    return expr;

  while (GET_OPERATOR (*act) == OP_MULT)
    {
      op = ARG (*act, 1);
      if (GET_OPERATOR (op) == OP_CONST_INT)
	{
	  folded_constant = FOLD_BINARY (OP_MULT, mode, folded_constant, op);
	  *act = ARG (*act, 0);
	}
      else
	{
	  last = act;
	  act = &ARG (*act, 0);
	}
    }
  if (GET_OPERATOR (*act) == OP_CONST_INT)
    {
      folded_constant = FOLD_BINARY (OP_MULT, mode, folded_constant, *act);
      if (!last || CONST_INT_VALUE (folded_constant) == 0)
	return folded_constant;
      if (CONST_INT_VALUE (folded_constant) == 1)
	*last = ARG (*last, 1);
      else
	*act = folded_constant;
    }
  else if (CONST_INT_VALUE (folded_constant) == 0)
    return folded_constant;
  else if (CONST_INT_VALUE (folded_constant) != 1)
    *act = GEN_BINARY (OP_MULT, mode, folded_constant, *act);
  
  if (GET_OPERATOR (expr) == OP_MULT)
    expr = sort_ops (expr);

  return expr;
}

/* If EXPR1 and EXPR2 differ only by a multiplicative constant,
   add the multiplicative constant of EXPR2 to FOLDED_CONSTANT.  */
static bool
combine_constants (expr1, expr2, mode, folded_constant)
     EXPR expr1;
     EXPR expr2;
     EXPR_TYPE mode;
     EXPR *folded_constant;
{
  EXPR cnst;

  while (GET_OPERATOR (expr1) == OP_MULT && GET_OPERATOR (expr2) == OP_MULT)
    {
      if (!EXPR_EQUAL_P (ARG (expr1, 1), ARG (expr2, 1)))
	return false;
      expr1 = ARG (expr1, 0);
      expr2 = ARG (expr2, 0);
    }

  if (GET_OPERATOR (expr1) == OP_MULT)
    {
      if (GET_OPERATOR (ARG (expr1, 0)) != OP_CONST_INT)
	return false;
      if (!EXPR_EQUAL_P (ARG (expr1, 1), expr2))
	return false;
      cnst = CONST_INT_EXPR (1);
    }
  else if (GET_OPERATOR (expr2) == OP_MULT)
    {
      cnst = ARG (expr2, 0);
      if (GET_OPERATOR (cnst) != OP_CONST_INT)
	return false;
      if (!EXPR_EQUAL_P (expr1, ARG (expr2, 1)))
	return false;
    }
  else if (GET_OPERATOR (expr1) == OP_CONST_INT && GET_OPERATOR (expr2) == OP_CONST_INT)
    cnst = expr2;
  else if (EXPR_EQUAL_P (expr1, expr2))
    cnst = CONST_INT_EXPR (1);
  else return false;

  *folded_constant = FOLD_BINARY (OP_PLUS, mode, cnst, *folded_constant);
  return true;
}

/* Simplify expression EXPR (sum of products in simplify_alg_expr shape).  We
   fold constants, transform c1 * x + c2 * x into (result of c1 + c2) * x,
   (where c_i are constants) and sort operands according to
   alg_compare_expr.  The simplification is done in-place.  */
static EXPR
simplify_alg_expr_plus (expr)
     EXPR expr;
{
  EXPR_TYPE mode = GET_EXPR_TYPE (expr);
  EXPR *act1, *pact2, *act2, op, *tmp, *last;
  EXPR old_folded_constant, folded_constant;

  if (GET_OPERATOR (expr) != OP_PLUS)
    return expr;

  /* Transform c1 * x + c2 * x -> (c1 + c2) * x.  */
  act1 = &expr;
  while (GET_OPERATOR (*act1) == OP_PLUS)
    {
      if (GET_OPERATOR (ARG (*act1, 1)) == OP_CONST_INT)
	{
	  act1 = &ARG (*act1, 0);
	  continue;
	}
      op = ARG (*act1, 1);
      while (GET_OPERATOR (op) == OP_MULT)
	op = ARG (op, 0);
      if (GET_OPERATOR (op) == OP_CONST_INT)
	folded_constant = op;
      else
	folded_constant = CONST_INT_EXPR (1);
      op = ARG (*act1, 1);
      old_folded_constant = folded_constant;
	
      pact2 = &ARG (*act1, 0);
      while (pact2)
	{
	  if (GET_OPERATOR (*pact2) == OP_PLUS)
	    {
	      act2 = &ARG (*pact2, 1);
	      pact2 = &ARG (*pact2, 0);
	    }
	  else
	    {
	      act2 = pact2;
	      pact2 = NULL;
	    }
	  if (GET_OPERATOR (*act2) == OP_CONST_INT)
	    continue;
	  if (combine_constants (op, *act2, mode, &folded_constant))
	    *act2 = CONST_INT_EXPR (0);
	}

      if (CONST_INT_VALUE (folded_constant) == 0)
	ARG (*act1, 1) = const0_rtx;
      else if (INTVAL (folded_constant) != INTVAL (old_folded_constant))
	{
	  last = NULL;
	  tmp = &ARG (*act1, 1);
	  while (GET_OPERATOR (*tmp) == OP_MULT)
	    {
	      last = tmp;
	      tmp = &ARG (*tmp, 0);
	    }
	  if (folded_constant == CONST_INT_EXPR (1))
	    {
	      if (!last)
		abort ();
	      *last = ARG (*last, 1);
	    }
	  else if (GET_OPERATOR (*tmp) == OP_CONST_INT)
	    *tmp = folded_constant;
	  else
	    *tmp = GEN_BINARY (OP_MULT, mode, folded_constant, *tmp);
	}
      act1 = &ARG (*act1, 0);
    }

  /* Fold constants.  */
  if (GET_OPERATOR (expr) != OP_PLUS)
    return expr;
  act1 = &expr;
  folded_constant = CONST_INT_EXPR (0);
  last = NULL;

  while (GET_OPERATOR (*act1) == OP_PLUS)
    {
      op = ARG (*act1, 1);
      if (GET_OPERATOR (op) == OP_CONST_INT)
	{
	  folded_constant = FOLD_BINARY (OP_PLUS, mode, folded_constant, op);
	  *act1 = ARG (*act1, 0);
	}
      else
	{
	  last = act1;
	  act1 = &ARG (*act1, 0);
	}
    }
  if (GET_OPERATOR (*act1) == OP_CONST_INT)
    {
      folded_constant = FOLD_BINARY (OP_PLUS, mode, folded_constant, *act1);
      if (!last)
	return folded_constant;
      if (CONST_INT_VALUE (folded_constant) == 0)
	*last = ARG (*last, 1);
      else
	*act1 = folded_constant;
    }
  else if (CONST_INT_VALUE (folded_constant) != 0)
    *act1 = GEN_BINARY (OP_PLUS, mode, folded_constant, *act1);

  if (GET_OPERATOR (expr) == OP_PLUS)
    expr = sort_ops (expr);

  return expr;
}

/* Tries to simplify MODE subreg of EXPR of mode EXPR_MODE (important for
   constants).  The simplification is done in place.  */
EXPR
simplify_alg_expr_shorten (expr, expr_mode, mode)
     EXPR expr;
     EXPR_TYPE expr_mode;
     EXPR_TYPE mode;
{
  EXPR_TYPE inner_mode;

  if (expr_mode == mode)
    return expr;
  switch (GET_OPERATOR (expr))
    {
    case OP_PLUS:
    case OP_MULT:
      ARG (expr, 0) = simplify_alg_expr_shorten (ARG (expr, 0), expr_mode, mode);
      ARG (expr, 1) = simplify_alg_expr_shorten (ARG (expr, 1), expr_mode, mode);
      SET_EXPR_TYPE (expr, mode);
      return expr;

    case OP_ITERATION:
      return gen_iteration (mode);

    case OP_CONST_INT:
      return FOLD_CAST (mode, expr, expr_mode);

    case OP_EXTEND_SIGNED:
    case OP_EXTEND_UNSIGNED:
      inner_mode = GET_EXPR_TYPE (ARG (expr, 0));
      if (GET_EXPR_TYPE_SIZE (inner_mode) < GET_EXPR_TYPE_SIZE (mode))
	{
	  SET_EXPR_TYPE (expr, mode);
	  return expr;
	}
      return simplify_alg_expr_shorten (ARG (expr, 0), inner_mode, mode);

    case OP_SHORTEN:
      if (DONT_SIMPLIFY_CASTS (expr, expr_mode))
	break;

      inner_mode = GET_EXPR_TYPE (ARG (expr, 0));
      return simplify_alg_expr_shorten (ARG (expr, 0), inner_mode, mode);

    default:
      break;
    }

  return GEN_SHORTEN (mode, expr);
}

/* Return const0_rtx if the comparison CODE with constant argument PAR is
   always false for operand in INNER_MODE extended to MODE, if the PAR is
   on right side.  If it is always true, return const_true_rtx.  Otherwise
   return NULL.  */
static EXPR
compare_with_type_bounds (code, par, mode, inner_mode)
     OPERATOR code;
     EXPR par;
     EXPR_TYPE mode;
     EXPR_TYPE inner_mode;
{
  EXPR mmin, mmax, rlow, rhigh;

  GET_COMPARISON_BOUNDS (inner_mode, code, &mmin, &mmax);
  switch (code)
    {
    case OP_EQ:
      if (TRUE_P (FOLD_RELATIONAL (OP_LT, mode, par, mmin))
	  || TRUE_P (FOLD_RELATIONAL (OP_GT, mode, par, mmax)))
	return EXPR_FALSE;
      break;
    case OP_NE:
      if (TRUE_P (FOLD_RELATIONAL (OP_LT, mode, par, mmin))
	  || TRUE_P (FOLD_RELATIONAL (OP_GT, mode, par, mmax)))
	return EXPR_TRUE;
      break;
      
    default:
      rlow = FOLD_RELATIONAL (code, mode, par, mmin);
      rhigh = FOLD_RELATIONAL (code, mode, par, mmax);
      if ((TRUE_P (rlow) && TRUE_P (rhigh))
	  || (FALSE_P (rlow) && FALSE_P (rhigh)))
	return rlow;
    }

  return NULL_EXPR;
}

/* Attempt to simplify relational expression EXPR, whose operands are
   in mode INNER_MODE.  The simplification is done in place.  */
static EXPR
simplify_alg_expr_relational (expr, inner_mode)
     EXPR expr;
     EXPR_TYPE inner_mode;
{
  EXPR left, right, comp;
  OPERATOR code, left_code, right_code, extend;
  EXPR_TYPE result_mode, extended_mode;

  /* If one of operands of comparison is {sign,zero}_extend and the other
     one is either constant or the same extend, we simplify them to work
     on the inner mode of extend instead.  */

  if (!INTEGER_TYPE_P (inner_mode))
    return expr;

  code = GET_OPERATOR (expr);
  if (!TEST_BIT (suitable_operator, code))
      return NULL_EXPR;
  result_mode = GET_EXPR_TYPE (expr);
  left = ARG (expr, 0);
  left_code = GET_OPERATOR (left);
  right = ARG (expr, 1);
  right_code = GET_OPERATOR (right);

  switch (left_code)
    {
    case OP_CONST_INT:
      extend = OP_NOP;
      extended_mode = GET_EXPR_TYPE (left);
      break;

    case OP_EXTEND_SIGNED:
    case OP_EXTEND_UNSIGNED:
      extend = left_code;
      extended_mode = GET_EXPR_TYPE (ARG (left, 0));
      break;

    default:
     return expr; 
    }

  switch (right_code)
    {
    case OP_CONST_INT:
      if (extend == OP_NOP)
	return FOLD_RELATIONAL (code, extended_mode, left, right);
      comp = compare_with_type_bounds (SWAP_CONDITION (code),
				       right, inner_mode, extended_mode);
      if (comp)
	return comp;
      ARG (expr, 0) = ARG (left, 0);
      return expr;

    case OP_EXTEND_SIGNED:
    case OP_EXTEND_UNSIGNED:
      if (extend == OP_NOP)
	{
	  extended_mode = GET_EXPR_TYPE (ARG (right, 0));
	  comp = compare_with_type_bounds (code, left,
					   inner_mode, extended_mode);
	  if (comp)
	    return comp;
	  ARG (expr, 1) = ARG (right, 0);
	  return expr;
	}

      if (extend != right_code
	  || extended_mode != GET_EXPR_TYPE (ARG (right, 0)))
	return expr;
      break;

    default:
     return expr; 
    }

  ARG (expr, 0) = ARG (left, 0);
  ARG (expr, 1) = ARG (right, 0);
  return expr;
}

/* Float if_then_else (iteration == 0) to the outer level of the EXPR,
   eliminating the superfluous ones.  ??? Hopefully later we could be able
   to extend it to more general class of if_then_else transformations,
   so that this special case is not that ugly.  Done in-place.  */
static EXPR
float_if_then_else (expr)
     EXPR expr;
{
  int i, length;
  EXPR right;
  OPERATOR code = GET_OPERATOR (expr);

  if (bival_p (expr))
    {
      if (bival_p (ARG (expr, 1)))
	ARG (expr, 1) = ARG (ARG (expr, 1), 1);
      if (bival_p (ARG (expr, 2)))
	ARG (expr, 2) = ARG (ARG (expr, 2), 2);
    }
  else
    {
      length = NUM_OP_ARGS (code);
      for (i = 0; i < length; i++)
	if (ITH_ARG_EXPR_P (code, i) && bival_p (ARG (expr, i)))
	  break;
      if (i == length)
	return expr;

      right = SHALLOW_COPY_EXPR (expr);
      for (i = 0; i < length; i++)
	if (ITH_ARG_EXPR_P (code, i))
	  {
	    if (bival_p (ARG (expr, i)))
	      {
		ARG (right, i) = ARG (ARG (expr, i), 2);
		ARG (expr, i) = ARG (ARG (expr, i), 1);
	      }
	    else
	      ARG (right, i) = COPY_EXPR (ARG (expr, i));
	  }

      expr = gen_bival (GET_EXPR_TYPE (expr), expr, right);
    }

  return expr;
}

/* Attempt to bring EXPR into canonical shape described in simplify_alg_expr.
   We are expecting the parameters of the topmost operation to already be
   in a canonical shape.  INNER_MODE is senseful for subregs and conditionals
   and stands for the original mode of the operands before simplification
   (it might become VOIDmode due to it). The simplification is partly done
   in-place.  */
static EXPR
simplify_alg_expr_one_level (expr, inner_mode)
     EXPR expr;
     EXPR_TYPE inner_mode;
{
  int i, length;
  OPERATOR code = GET_OPERATOR (expr);
  EXPR *current, tmp;
  EXPR_TYPE mode = GET_EXPR_TYPE (expr);

  switch (code)
    {
    case OP_MULT:
      expr = use_distributive_law (expr);
      current = &expr;
      while (GET_OPERATOR (*current) == OP_PLUS)
	{
	  ARG (*current, 1) = simplify_alg_expr_mult (ARG (*current, 1));
	  current = &ARG (*current, 0);
	}
      *current = simplify_alg_expr_mult (*current);

      if (GET_OPERATOR (expr) != OP_PLUS)
	return expr;

      /* Fallthru.  */
    case OP_PLUS:
      /* Straighten the list of summands.  */
      expr = straighten_ops (expr);
      expr = simplify_alg_expr_plus (expr);
      return expr;

    case OP_SHORTEN:
      if (DONT_SIMPLIFY_CASTS (expr, mode))
	break;

      expr = simplify_alg_expr_shorten (ARG (expr, 0), inner_mode, mode);
      if (GET_OPERATOR (expr) != OP_SHORTEN)
	expr = simplify_alg_expr (expr);
      return expr;

    case OP_DIV:
    case OP_UDIV:
      if (CONST_INT_VALUE (ARG (expr, 1)) == 1)
	expr = ARG (expr, 0);
      break;

    case OP_MOD:
    case OP_UMOD:
      if (CONST_INT_VALUE (ARG (expr, 1)) == 1)
	expr = CONST_INT_EXPR (0);
      break;

    case OP_IF_THEN_ELSE:
      if (bival_p (expr)
	  && equal_when_iteration_zero_p (ARG (expr, 1), ARG (expr, 2)))
	expr = ARG (expr, 2);
      break;

    default:
      if (COMPARISON_OP_P (code))
	expr = simplify_alg_expr_relational (expr, inner_mode);
    }
  code = GET_OPERATOR (expr);

  /* Fold constants.  */
  length = NUM_OP_ARGS (code);
  for (i = 0; i < length; i++)
    {
      if (ITH_ARG_EXPR_P (code, i))
	{
	  if (GET_OPERATOR (ARG (expr, i)) != OP_CONST_INT)
	    break;
	}
      else if (!ITH_ARG_CONST_P (code, i))
	break;
    }
  if (i == length)
    {
      switch (code)
	{
	case OP_SHORTEN:
#ifdef EL_RTX
	  /* Nontrivial subregs are rtl specific.  */
	  tmp = simplify_gen_subreg (mode, ARG (expr, 0),
				     inner_mode, XINT (expr, 1));
#else
	  tmp = FOLD_CAST (mode, ARG (expr, 0), inner_mode);
#endif
	  break;
	case OP_EXTEND_SIGNED:
	case OP_EXTEND_UNSIGNED:
	  tmp = FOLD_EXTEND (code, mode, ARG (expr, 0), inner_mode);
	  break;
	default:
	    tmp = GENERIC_FOLD (expr);
	}
      if (tmp && GET_OPERATOR (tmp) == OP_CONST_INT)
	return tmp;
    }

  /* Sort the operands if possible.  */
  if (COMUTATIVE_OP_P (code))
    {
      if (alg_compare_expr (ARG (expr, 0), ARG (expr, 1)) > 0)
	{
     	  tmp = ARG (expr, 0);
	  ARG (expr, 0) = ARG (expr, 1);
	  ARG (expr, 1) = tmp;
	}
    }
  else if (COMPARISON_OP_P (code))
    {
      code = SWAP_CONDITION (code);
      if (code != UNKNOWN
	  && alg_compare_expr (ARG (expr, 0), ARG (expr, 1)) > 0)
	{
	  PUT_CODE (expr, code);
     	  tmp = ARG (expr, 0);
	  ARG (expr, 0) = ARG (expr, 1);
	  ARG (expr, 1) = tmp;
	}
    }

  return expr;
}

/* Attempt to bring EXPR into canonical shape described below.  It would
   be nice if we could use simplify_rtx for it; but it is too low level
   for our purposes, and does basically the reverse of transformations
   we want.  The simplification is partly done in-place.  */
EXPR
simplify_alg_expr (expr)
     EXPR expr;
{
  EXPR_TYPE mode, inner_mode = EXPR_UNSPECIFIED_TYPE;
  HOST_WIDE_INT val;
  int i, length, was_bival;
  EXPR tmp;
  OPERATOR code;

  if (!expr || simple_expr_p (expr))
    return expr;
  mode = GET_EXPR_TYPE (expr);

  /* The shape of the resulting expression is

     expr = exp | if_then_else (iteration == 0) exp exp
     exp = mexp | expr + mexp
     mexp = whatever | nexp * whatever

     The (ugly) special case with the iteration is here in order
     to be able to cope with induction variables that are done
     in wider mode register.

     Operands to commutative operations and comparisons are ordered
     according to alg_compare_expr inside whatevers.  The list of mexp
     operands in expr, as well as list of whatever operands in mexp
     is in this order as well.
     
     We do also a few other optimizations on subregs and extends.  */

  /* Do some canonicalization.  */
  code = GET_OPERATOR (expr);
  switch (code)
    {
    case OP_MINUS:
      /* (MINUS x y) == (OP_PLUS x (OP_MULT (-1) y))  */
      tmp = ARG (expr, 1);
      tmp = GEN_BINARY (OP_MULT, mode, CONST_INT_EXPR (-1), tmp);
      expr = GEN_BINARY (OP_PLUS, mode, ARG (expr, 0), tmp);
      break;
      
    case OP_NEG:
      /* (NEG x) == (OP_MULT (-1) x)  */
      tmp = ARG (expr, 0);
      expr = GEN_BINARY (OP_MULT, mode, CONST_INT_EXPR (-1), tmp);
      break;

    case OP_ASHIFT:
      /* (ASHIFT x const) == (OP_MULT (2^const) x)  */
      tmp = ARG (expr, 1);
      if (GET_OPERATOR (tmp) != OP_CONST_INT)
	break;
      val = CONST_INT_VALUE (tmp);
      expr = GEN_BINARY (OP_MULT, mode,
			 CONST_INT_EXPR (1 << val), ARG (expr, 0));
      break;

    case OP_NOT:
      /* (NOT x) == (OP_PLUS (-1) (NEG x)) */
      tmp = ARG (expr, 0);
      tmp = GEN_UNARY (OP_NEG, mode, tmp);
      expr = GEN_BINARY (OP_PLUS, mode, CONST_INT_EXPR (-1), tmp);
      break;

#ifdef EL_RTX
    case CONST:
      /* Throw away consts so that we see what is inside.  */
      return simplify_alg_expr (ARG (expr, 0));
#endif

    case OP_CONST_INT:
    case OP_CONST_ADDRESS:
      /* These take the shortcut above.  */
      abort ();

    case OP_INITIAL_VALUE:
    case OP_VARIABLE:
    case OP_ITERATION:
      return expr;

    case OP_SHORTEN:
    case OP_EXTEND_SIGNED:
    case OP_EXTEND_UNSIGNED:
      /* Remember mode, as it is lost when the inner expression is simplified
	 to constant.  */
      inner_mode = GET_EXPR_TYPE (ARG (expr, 0));
      break;

    default: ;
    }
  code = GET_OPERATOR (expr);

  if (COMPARISON_OP_P (code))
    inner_mode = GET_ARGS_TYPE (expr);

  /* Simplify subexpressions.  */
  length = NUM_OP_ARGS (code);
  for (i = 0; i < length; i++)
    if (ITH_ARG_EXPR_P (code, i))
      ARG (expr, i) = simplify_alg_expr (ARG (expr, i));

  was_bival = bival_p (expr);
  expr = float_if_then_else (expr);
  if (!was_bival && bival_p (expr))
    {
      ARG (expr, 1) = simplify_alg_expr_one_level (ARG (expr, 1), inner_mode);
      ARG (expr, 2) = simplify_alg_expr_one_level (ARG (expr, 2), inner_mode);
    }

  return simplify_alg_expr_one_level (expr, inner_mode);
}

/* Initialize table of codes we are able to process.  */
void
init_algebraic ()
{
  unsigned i;

  suitable_operator = sbitmap_alloc (NUM_RTX_CODE);
  sbitmap_zero (suitable_operator);
  SET_BIT (suitable_operator, OP_CONST_INT);
#ifdef EL_RTX
  SET_BIT (suitable_operator, CONST);
#endif
  SET_BIT (suitable_operator, OP_VARIABLE);
  SET_BIT (suitable_operator, OP_CONST_ADDRESS);
  SET_BIT (suitable_operator, OP_IF_THEN_ELSE);
  SET_BIT (suitable_operator, OP_PLUS);
  SET_BIT (suitable_operator, OP_MINUS);
  SET_BIT (suitable_operator, OP_NEG);
  SET_BIT (suitable_operator, OP_MULT);
  SET_BIT (suitable_operator, OP_DIV);
  SET_BIT (suitable_operator, OP_MOD);
  SET_BIT (suitable_operator, OP_UMOD);
  SET_BIT (suitable_operator, OP_UDIV);
  SET_BIT (suitable_operator, OP_AND);
  SET_BIT (suitable_operator, OP_IOR);
  SET_BIT (suitable_operator, OP_NOT);
  SET_BIT (suitable_operator, OP_ASHIFT);
  SET_BIT (suitable_operator, OP_ASHIFTRT);
  SET_BIT (suitable_operator, OP_LSHIFTRT);
  SET_BIT (suitable_operator, OP_NE);
  SET_BIT (suitable_operator, OP_EQ);
  SET_BIT (suitable_operator, OP_GE);
  SET_BIT (suitable_operator, OP_GT);
  SET_BIT (suitable_operator, OP_LE);
  SET_BIT (suitable_operator, OP_LT);
  SET_BIT (suitable_operator, OP_GEU);
  SET_BIT (suitable_operator, OP_GTU);
  SET_BIT (suitable_operator, OP_LEU);
  SET_BIT (suitable_operator, OP_LTU);
  SET_BIT (suitable_operator, OP_SHORTEN);
  SET_BIT (suitable_operator, OP_EXTEND_SIGNED);
  SET_BIT (suitable_operator, OP_EXTEND_UNSIGNED);
  SET_BIT (suitable_operator, OP_INITIAL_VALUE);
  SET_BIT (suitable_operator, OP_UNSPECIFIED_VALUE);
  SET_BIT (suitable_operator, OP_ITERATION);

#ifdef EL_RTX
  for (i = 0; i < NUM_MACHINE_MODES; i++)
    iteration_rtx[i] = gen_rtx (OP_ITERATION, i);
#endif
}

/* Substitutes values from SUBSTITUTION (i-th element corresponds to a value
   of reg i) into EXPR.  Only values of registers specified by a
   INTERESTING_REG bitmap are considered valid.
   If SIE_SIMPLIFY bit is set in FLAGS, also simplify the resulting expression
   (the substituted expressions are assumed to be already simplified).
   If SIE_ONLY_SIMPLE is set, only substitute simple expressions.
   RESULT_MODE indicates the real mode of target of resulting expression
   (used when simplifying).  If the substitution fails to be performed,
   NULL_EXPR is returned instead.
 
   If INTERESTING_REG is NULL, then we replace iteration rtl by *substitution
   instead (??? this should be done in a cleaner way!).  */
EXPR
substitute_into_expr (expr, interesting_reg, substitution, flags)
     EXPR expr;
     sbitmap interesting_reg;
     EXPR *substitution;
     int flags;
{
  EXPR old_expr, new_expr, sub_expr;
  unsigned regno;
  int i, length;
  EXPR_TYPE mode, inner_mode = EXPR_UNSPECIFIED_TYPE;
  OPERATOR code;
  EXPR val = NULL_EXPR;

  if (!expr || good_constant_p (expr))
    return expr;

  mode = GET_EXPR_TYPE (expr);
  if (mode != EXPR_UNSPECIFIED_TYPE
      && !INTEGER_TYPE_P (mode))
    return NULL_EXPR;

  old_expr = expr;
  if (interesting_reg && GET_OPERATOR (expr) == OP_INITIAL_VALUE)
    expr = ARG (expr, 0);

  code = GET_OPERATOR (expr);
  if (interesting_reg && code == OP_VARIABLE)
    {
      regno = VARIABLE_ID (expr);
      if (!TEST_BIT (interesting_reg, regno))
	return NULL_EXPR;

      val = substitution[regno];
      if (!val)
	return NULL_EXPR;
    }
  else if (!interesting_reg && code == OP_ITERATION)
    {
      val = *substitution;
      if (GET_EXPR_TYPE (val) != EXPR_UNSPECIFIED_TYPE
	  && GET_EXPR_TYPE (val) != GET_EXPR_TYPE (expr))
	abort ();
    }

  if (val)
    {
      if ((flags & SIE_ONLY_SIMPLE)
	  && !simple_expr_p (val))
	return old_expr;

      /* Optimize some common cases that may be shared.  */
      switch (GET_OPERATOR (val))
	{
	case OP_INITIAL_VALUE:
	case OP_UNSPECIFIED_VALUE:
	case OP_CONST_INT:
	case OP_CONST_ADDRESS:
	  return val;

	default:
	  return COPY_EXPR (val);
	}
    }
  
  /* Just ignore the codes that do not seem to be good for further
     processing.  */
  if (!TEST_BIT (suitable_operator, code))
      return NULL_EXPR;

  if (code == OP_SHORTEN || code == OP_EXTEND_SIGNED || code == OP_EXTEND_UNSIGNED)
    inner_mode = GET_EXPR_TYPE (ARG (expr, 0));

  new_expr = SHALLOW_COPY_EXPR (expr);
  length = NUM_OP_ARGS (code);
  for (i = 0; i < length; i++)
    {
      if (ITH_ARG_EXPR_P (code, i))
	{
	  sub_expr = substitute_into_expr (ARG (expr, i),
					   interesting_reg, substitution,
					   flags & ~SIE_SIMPLIFY);
	  if (!sub_expr)
	    return NULL_EXPR;
	  ARG (new_expr, i) = sub_expr;
	}
      else if (!ITH_ARG_CONST_P (code, i))
	return NULL_EXPR;
    }

  if ((code == OP_SHORTEN || code == OP_EXTEND_SIGNED || code == OP_EXTEND_UNSIGNED)
      && inner_mode != EXPR_UNSPECIFIED_TYPE
      && GET_EXPR_TYPE (ARG (new_expr, 0)) == EXPR_UNSPECIFIED_TYPE)
    {
      if (code == OP_SHORTEN)
	{
#ifdef EL_RTX
	  new_expr = simplify_gen_subreg (mode, ARG (new_expr, 0),
					  inner_mode, XINT (expr, 1));
#else
	  new_expr = FOLD_CAST (mode, ARG (new_expr, 0), inner_mode);
#endif
	}
      else
	new_expr = FOLD_EXTEND (code, mode, ARG (new_expr, 0), inner_mode);
    }

  if ((flags & SIE_SIMPLIFY) && !simple_expr_p (new_expr))
    new_expr = simplify_alg_expr (new_expr);

  return new_expr;
}

/* Try to simplify expression VAR using register initial values stored
   in INITIAL_VALUES; only registers in INTERESTING_REG are considered
   to have a valid value in INITIAL_VALUES.  Returns the simplified form
   of VAR or VAR if no simplification is possible.  Strongly biased
   for an induction variable analysis.  Partially done in-place.  */
EXPR
simplify_alg_expr_using_values (var, interesting_reg, initial_values)
     EXPR var;
     sbitmap interesting_reg;
     EXPR *initial_values;
{
  EXPR base, step, sbase, sstep, wrap = NULL_EXPR, svar;
  int changed;
  EXPR_TYPE mode;

  /* Simplify the bival.  If it is simplified out using initial values,
     we may omit it; otherwise we just attempt to simplify the single
     fields.  */
  if (bival_p (var))
    {
      svar = substitute_into_expr (var, interesting_reg, initial_values,
				   SIE_SIMPLIFY);
      if (bival_p (svar))
	{
	  EXPR thn, els;
	  
	  thn = simplify_alg_expr_using_values (ARG (var, 1), interesting_reg,
						initial_values);
	  els = simplify_alg_expr_using_values (ARG (var, 2), interesting_reg,
						initial_values);
	  if (!thn || !els)
	    return NULL_EXPR;

	  ARG (var, 1) = thn;
	  ARG (var, 2) = els;
	  return var;
	}
      else
	var = ARG (var, 2);
    }

  if (simple_expr_p (var)
      || GET_OPERATOR (var) == OP_UNSPECIFIED_VALUE
      || GET_OPERATOR (var) == OP_ITERATION)
    return var;

  if (GET_OPERATOR (var) == OP_EXTEND_SIGNED
      || GET_OPERATOR (var) == OP_EXTEND_UNSIGNED)
    {
      wrap = var;
      var = ARG (var, 0);
    }
  mode = GET_EXPR_TYPE (var);

  iv_split (var, &base, &step);
  if (!base)
    return NULL_EXPR;

  changed = false;
  if (fast_expr_mentions_operator_p (base, OP_INITIAL_VALUE))
    {
      sbase = substitute_into_expr (base, interesting_reg, initial_values,
				    SIE_SIMPLIFY);
      if (!sbase || !simple_expr_p (sbase))
	sbase = substitute_into_expr (base, interesting_reg, initial_values,
				      SIE_ONLY_SIMPLE | SIE_SIMPLIFY);
      if (sbase)
	{
	  base = sbase;
	  changed = true;
	}
    }
  if (fast_expr_mentions_operator_p (step, OP_INITIAL_VALUE))
    {
      sstep = substitute_into_expr (step, interesting_reg, initial_values,
				    SIE_SIMPLIFY);
      if (!sstep || !simple_expr_p (sstep))
	sstep = substitute_into_expr (step, interesting_reg, initial_values,
				      SIE_ONLY_SIMPLE | SIE_SIMPLIFY);
      if (sstep)
	{
	  step = sstep;
	  changed = true;
	}
    }

  if (changed)
    {
      if (GET_OPERATOR (step) == OP_CONST_INT
	  && CONST_INT_VALUE (step) == 0)
	var = base;
      else
	{
	  var = gen_iteration (mode);
	  if (GET_OPERATOR (step) != OP_CONST_INT
	      || CONST_INT_VALUE (step) != 1)
	    var = GEN_BINARY (OP_MULT, mode, step, var);
	  if (GET_OPERATOR (base) != OP_CONST_INT
	      || CONST_INT_VALUE (base) != 0)
	    var = GEN_BINARY (OP_PLUS, mode, base, var);
	}
    }

  if (wrap && GET_EXPR_TYPE (var) == mode)
    {
      ARG (wrap, 0) = var;
      var = wrap;
    }
  return var;
}

/* Splits expression for induction variable into BASE and STEP.  We expect
   EXPR to come from simplify_alg_expr.  EXPR is not modified, but part of
   it may be shared as BASE or STEP.  */
void
iv_split (expr, base, step)
     EXPR expr;
     EXPR *base;
     EXPR *step;
{
  EXPR abase, astep, *pabase = &abase, *pastep = &astep;
  EXPR next, *act, tmp;
  EXPR_TYPE mode;

  *base = *step = NULL_EXPR;
  if (!expr)
    return;
  mode = GET_EXPR_TYPE (expr);

  if (good_constant_p (expr)
      || GET_OPERATOR (expr) == OP_UNSPECIFIED_VALUE
      || GET_OPERATOR (expr) == OP_INITIAL_VALUE
      || !fast_expr_mentions_operator_p (expr, OP_ITERATION))
    {
      *base = expr;
      *step = const0_rtx;
      return;
    }

  /* Avoid copying in the most common cases:  */
  if (GET_OPERATOR (expr) != OP_PLUS
      || !fast_expr_mentions_operator_p (ARG (expr, 0), OP_ITERATION))
    {
      if (GET_OPERATOR (expr) == OP_PLUS)
	{
	  *base = ARG (expr, 0);
	  tmp = ARG (expr, 1);
	}
      else
	{
	  *base = const0_rtx;
	  tmp = expr;
	}

      if (GET_OPERATOR (tmp) == OP_ITERATION)
	*step = const1_rtx;
      else if (GET_OPERATOR (tmp) == OP_MULT
	       && GET_OPERATOR (ARG (tmp, 0)) == OP_CONST_INT
	       && GET_OPERATOR (ARG (tmp, 1)) == OP_ITERATION)
	*step = ARG (tmp, 0);
      else
	*base = NULL_EXPR;

      if (*base)
	return;
    }

  expr = COPY_EXPR (expr);
  while (GET_OPERATOR (expr) == OP_PLUS)
    {
      next = ARG (expr, 0);
      act = &ARG (expr, 1);
      if (GET_OPERATOR (*act) == OP_MULT)
	act = &ARG (*act, 1);
      if (GET_OPERATOR (*act) == OP_ITERATION)
	{
	  *act = CONST_INT_EXPR (1);
	  *pastep = expr;
	  pastep = &ARG (expr, 0);
	}
      else
	{
	  *pabase = expr;
	  pabase = &ARG (expr, 0);
	}
      expr = next;
    }
  if (GET_OPERATOR (expr) == OP_ITERATION)
    {
      *pastep = CONST_INT_EXPR (1);
      *pabase = CONST_INT_EXPR (0);
    }
  else if (GET_OPERATOR (expr) == OP_MULT
	   && GET_OPERATOR (ARG (expr, 1)) == OP_ITERATION)
    {
      *pastep = expr;
      ARG (expr, 1) = CONST_INT_EXPR (1);
      *pabase = CONST_INT_EXPR (0);
    }
  else
    {
      *pastep = CONST_INT_EXPR (0);
      *pabase = expr;
    }

  if (fast_expr_mentions_operator_p (astep, OP_ITERATION)
      || fast_expr_mentions_operator_p (abase, OP_ITERATION))
    return;

  *base = simplify_alg_expr (abase);
  *step = simplify_alg_expr (astep);
}

/* Generate OP_ITERATION in MODE (they are shared, so just return the EXPR).  */
EXPR
gen_iteration (mode)
     EXPR_TYPE mode;
{
#ifdef EL_RTX
  return iteration_rtx[mode];
#else
#error "Something really, really weird is happening."
#endif
}

/* Generate a rtl to represent an expression in MODE that has value
   FIRST_ITERATION in the first iteration of the enclosing loop and
   OTHER_ITERATIONS in the remaining ones.  ITMODE is a mode in that
   iteration count is measured.  */
EXPR
gen_bival (mode, first_iteration, other_iterations)
     EXPR_TYPE mode;
     EXPR first_iteration;
     EXPR other_iterations;
{
  return GEN_TERNARY (OP_IF_THEN_ELSE, mode,
		      GEN_BINARY (EQ, STD_INT_TYPE,
				  const0_rtx,
				  gen_iteration (STD_INT_TYPE)),
			  first_iteration,
			  other_iterations);
}

#include "gt-algebraic.h"
