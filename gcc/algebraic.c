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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "expr.h"
#include "sbitmap.h"
#include "algebraic.h"

static sbitmap suitable_code;		/* Bitmap of rtl codes we are able
					   to handle.  */

/* Shared iteration rtxes in different modes.  */
static rtx iteration_rtx[NUM_MACHINE_MODES];

static rtx straighten_ops		PARAMS ((rtx));
static rtx sort_ops			PARAMS ((rtx));
static int alg_compare_rtx		PARAMS ((rtx, rtx));
static rtx use_distributive_law		PARAMS ((rtx));
static rtx simplify_alg_expr_mult	PARAMS ((rtx));
static rtx simplify_alg_expr_plus	PARAMS ((rtx));
static rtx simplify_alg_expr_relational	PARAMS ((rtx, enum machine_mode));
static bool combine_constants		PARAMS ((rtx, rtx, enum machine_mode,
						 rtx *));
static rtx compare_with_mode_bounds	PARAMS ((enum rtx_code,
						 rtx,
						 enum machine_mode,
						 enum machine_mode));

/* Returns EXPR1 <=> EXPR2 in order given by lexicografical extension
   of the following ordering:

   integer constants by value < other constants < regs by number <
   value_at by register and insn id < other rtxes by code < iteration.
   Note that the fact that we return 0 does not neccessarily imply that
   expressions are equal.  */
static int
alg_compare_rtx (expr1, expr2)
     rtx expr1;
     rtx expr2;
{
  int i, length, cmp;
  const char *format;

  if (rtx_equal_p (expr1, expr2))
    return 0;

  if (GET_CODE (expr1) == CONST_INT)
    {
      if (GET_CODE (expr2) != CONST_INT)
	return -1;
      return INTVAL (expr1) < INTVAL (expr2) ? -1 : 1;
    }
  if (GET_CODE (expr2) == CONST_INT)
    return 1;

  if (CONSTANT_P (expr1))
    return CONSTANT_P (expr2) ? 0 : -1;
  if  (CONSTANT_P (expr2))
    return 1;

  if (REG_P (expr1))
    {
      if (!REG_P (expr2))
	return -1;
      return REGNO (expr1) < REGNO (expr2) ? -1 : 1;
    }
  if (REG_P (expr2))
    return 1;

  if (GET_CODE (expr1) == VALUE_AT)
    {
      if (GET_CODE (expr2) != VALUE_AT)
	return -1;
      if (XINT (expr1, 0) < XINT (expr2, 0))
	return -1;
      if (XINT (expr1, 0) > XINT (expr2, 0))
	return 1;
      return XINT (expr1, 0) < XINT (expr2, 0) ? -1 : 1;
    }

  if (GET_CODE (expr2) == VALUE_AT)
    return 1;

  if (GET_CODE (expr1) == ITERATION)
    return 1;
  if (GET_CODE (expr2) == ITERATION)
    return -1;

  if (GET_CODE (expr1) < GET_CODE (expr2))
    return -1;
  if (GET_CODE (expr1) > GET_CODE (expr2))
    return 1;

  /* We prefer rightmost operand here, so that expressions that differ
     only in the value of a constant term are sorted adjacent in
     simplify_alg_expr_plus.  */
  length = GET_RTX_LENGTH (GET_CODE (expr1));
  format = GET_RTX_FORMAT (GET_CODE (expr1));
  for (i = length - 1; i >= 0; i--)
    {
      switch (format[i])
	{
	case 'e':
	  cmp = alg_compare_rtx (XEXP (expr1, i), XEXP (expr2, i));
	  if (cmp)
	    return cmp;

	default:
	  break;
	}
    }
  return 0;
}

/* Given expression in shape (((((a op b) op c) op d) op e) op f), return
   expression in the same shape but the operands sorted according to
   alg_compare_rtx.  The sorting is done in-place using bubble sort.  */
static rtx
sort_ops (expr)
     rtx expr;
{
  rtx pos, *op1, *op2, tmp;
  enum rtx_code code = GET_CODE (expr);
  int changed = true;

  if (code != PLUS
      && code != MULT)
    return expr;

  while (changed)
    {
      changed = false;
      pos = expr;
      while (GET_CODE (pos) == code)
	{
	  op1 = &XEXP (pos, 1);
	  op2 = GET_CODE (XEXP (pos, 0)) == code
	    ? &XEXP (XEXP (pos, 0), 1)
	    : &XEXP (pos, 0);

	  if (alg_compare_rtx (*op1, *op2) < 0)
	    {
	      tmp = *op1;
	      *op1 = *op2;
	      *op2 = tmp;
	      changed = true;
	    }
	  pos = XEXP (pos, 0);
	}
    }
  return expr;
}

/* Given expression in shape ((a op b) op c) op ((d op e) op f),
   transform it into (((((a op b) op c) op d) op e) op f).  The straightening
   is done in-place.  */
static rtx
straighten_ops (expr)
     rtx expr;
{
  rtx tmp;
  enum rtx_code code = GET_CODE (expr);

  if (code != PLUS
      && code != MULT)
    return expr;

  while (GET_CODE (XEXP (expr, 1)) == code)
    {
      tmp = expr;
      expr = XEXP (expr, 1);
      XEXP (tmp, 1) = XEXP (expr, 1);
      XEXP (expr, 1) = XEXP (expr, 0);
      XEXP (expr, 0) = tmp;
    }

  return expr;
}

/* EXPR is product of two expressions in the format described at
   simplify_alg_expr.  Use distributive law to express it as sum of
   products.  The transformation is partially done in-place.  */
static rtx
use_distributive_law (expr)
     rtx expr;
{
  rtx exprl, exprr, aexprr, ml, mr, prod, tmp, *act;
  rtx result = NULL_RTX, *ares = &result;
  enum machine_mode mode = GET_MODE (expr);
  
  exprl = XEXP (expr, 0);
  exprr = XEXP (expr, 1);

  if (GET_CODE (exprr) != PLUS)
    {
      if (GET_CODE (exprl) != PLUS)
	return straighten_ops (expr);
      tmp = exprl;
      exprl = exprr;
      exprr = tmp;
    }

  while (GET_CODE (exprl) == PLUS)
    {
      aexprr = exprr;
      while (1)
	{
	  ml = copy_rtx (XEXP (exprl, 1));
	  mr = GET_CODE (aexprr) == PLUS
		  ? copy_rtx (XEXP (aexprr, 1))
		  : copy_rtx (aexprr);
	  prod = straighten_ops (gen_rtx_fmt_ee (MULT, mode, ml, mr));
	  *ares = gen_rtx_fmt_ee (PLUS, mode, NULL_RTX, prod);
	  ares = &XEXP (*ares, 0);

	  if (GET_CODE (aexprr) != PLUS)
	    break;
	  aexprr = XEXP (aexprr, 0);
	}
      exprl = XEXP (exprl, 0);
    }

  act = &exprr;
  while (GET_CODE (*act) == PLUS)
    {
      ml = copy_rtx (exprl);
      mr = XEXP (*act, 1);
      tmp = straighten_ops (gen_rtx_fmt_ee (MULT, mode, ml, mr));
      XEXP (*act, 1) = tmp;
      act = &XEXP (*act, 0);
    }
  *act = straighten_ops (gen_rtx_fmt_ee (MULT, mode, exprl, *act));
  *ares = exprr;
  
  return result;
}

/* Simplify expression EXPR (some product).  We just fold constants and
   sort operands according to alg_compare_rtx here.  The simplification
   is done in-place.  */
static rtx
simplify_alg_expr_mult (expr)
     rtx expr;
{
  enum machine_mode mode = GET_MODE (expr);
  rtx folded_constant = const1_rtx;
  rtx *act = &expr, *last = NULL;
  rtx op;

  if (GET_CODE (expr) != MULT)
    return expr;

  while (GET_CODE (*act) == MULT)
    {
      op = XEXP (*act, 1);
      if (GET_CODE (op) == CONST_INT)
	{
	  folded_constant = simplify_gen_binary (MULT, mode,
						 folded_constant, op);
	  *act = XEXP (*act, 0);
	}
      else
	{
	  last = act;
	  act = &XEXP (*act, 0);
	}
    }
  if (GET_CODE (*act) == CONST_INT)
    {
      folded_constant = simplify_gen_binary (MULT, mode,
					     folded_constant, *act);
      if (!last || folded_constant == const0_rtx)
	return folded_constant;
      if (folded_constant == const1_rtx)
	*last = XEXP (*last, 1);
      else
	*act = folded_constant;
    }
  else if (folded_constant == const0_rtx)
    return const0_rtx;
  else if (folded_constant != const1_rtx)
    *act = gen_rtx_fmt_ee (MULT, mode, folded_constant, *act);
  
  if (GET_CODE (expr) == MULT)
    expr = sort_ops (expr);

  return expr;
}

/* If EXPR1 and EXPR2 differ only by a multiplicative constant,
   add the multiplicative constant of EXPR2 to FOLDED_CONSTANT.  */
static bool
combine_constants (expr1, expr2, mode, folded_constant)
     rtx expr1;
     rtx expr2;
     enum machine_mode mode;
     rtx *folded_constant;
{
  rtx cnst;

  while (GET_CODE (expr1) == MULT && GET_CODE (expr2) == MULT)
    {
      if (!rtx_equal_p (XEXP (expr1, 1), XEXP (expr2, 1)))
	return false;
      expr1 = XEXP (expr1, 0);
      expr2 = XEXP (expr2, 0);
    }

  if (GET_CODE (expr1) == MULT)
    {
      if (GET_CODE (XEXP (expr1, 0)) != CONST_INT)
	return false;
      if (!rtx_equal_p (XEXP (expr1, 1), expr2))
	return false;
      cnst = const1_rtx;
    }
  else if (GET_CODE (expr2) == MULT)
    {
      cnst = XEXP (expr2, 0);
      if (GET_CODE (cnst) != CONST_INT)
	return false;
      if (!rtx_equal_p (expr1, XEXP (expr2, 1)))
	return false;
    }
  else if (GET_CODE (expr1) == CONST_INT && GET_CODE (expr2) == CONST_INT)
    cnst = expr2;
  else if (rtx_equal_p (expr1, expr2))
    cnst = const1_rtx;
  else return false;

  *folded_constant = simplify_gen_binary (PLUS, mode, cnst, *folded_constant);
  return true;
}

/* Simplify expression EXPR (sum of products in simplify_alg_expr shape).  We
   fold constants, transform c1 * x + c2 * x into (result of c1 + c2) * x,
   (where c_i are constants) and sort operands according to
   alg_compare_rtx.  The simplification is done in-place.  */
static rtx
simplify_alg_expr_plus (expr)
     rtx expr;
{
  enum machine_mode mode = GET_MODE (expr);
  rtx *act1, *pact2, *act2, op, *tmp, *last;
  rtx old_folded_constant, folded_constant;

  if (GET_CODE (expr) != PLUS)
    return expr;

  /* Transform c1 * x + c2 * x -> (c1 + c2) * x.  */
  act1 = &expr;
  while (GET_CODE (*act1) == PLUS)
    {
      if (GET_CODE (XEXP (*act1, 1)) == CONST_INT)
	{
	  act1 = &XEXP (*act1, 0);
	  continue;
	}
      op = XEXP (*act1, 1);
      while (GET_CODE (op) == MULT)
	op = XEXP (op, 0);
      if (GET_CODE (op) == CONST_INT)
	folded_constant = op;
      else
	folded_constant = const1_rtx;
      op = XEXP (*act1, 1);
      old_folded_constant = folded_constant;
	
      pact2 = &XEXP (*act1, 0);
      while (pact2)
	{
	  if (GET_CODE (*pact2) == PLUS)
	    {
	      act2 = &XEXP (*pact2, 1);
	      pact2 = &XEXP (*pact2, 0);
	    }
	  else
	    {
	      act2 = pact2;
	      pact2 = NULL;
	    }
	  if (GET_CODE (*act2) == CONST_INT)
	    continue;
	  if (combine_constants (op, *act2, mode, &folded_constant))
	    *act2 = const0_rtx;
	}

      if (folded_constant == const0_rtx)
	XEXP (*act1, 1) = const0_rtx;
      else if (INTVAL (folded_constant) != INTVAL (old_folded_constant))
	{
	  last = NULL;
	  tmp = &XEXP (*act1, 1);
	  while (GET_CODE (*tmp) == MULT)
	    {
	      last = tmp;
	      tmp = &XEXP (*tmp, 0);
	    }
	  if (folded_constant == const1_rtx)
	    {
	      if (!last)
		abort ();
	      *last = XEXP (*last, 1);
	    }
	  else if (GET_CODE (*tmp) == CONST_INT)
	    *tmp = folded_constant;
	  else
	    *tmp = gen_rtx_fmt_ee (MULT, mode, folded_constant, *tmp);
	}
      act1 = &XEXP (*act1, 0);
    }

  /* Fold constants.  */
  if (GET_CODE (expr) != PLUS)
    return expr;
  act1 = &expr;
  folded_constant = const0_rtx;
  last = NULL;

  while (GET_CODE (*act1) == PLUS)
    {
      op = XEXP (*act1, 1);
      if (GET_CODE (op) == CONST_INT)
	{
	  folded_constant = simplify_gen_binary (PLUS, mode,
						 folded_constant, op);
	  *act1 = XEXP (*act1, 0);
	}
      else
	{
	  last = act1;
	  act1 = &XEXP (*act1, 0);
	}
    }
  if (GET_CODE (*act1) == CONST_INT)
    {
      folded_constant = simplify_gen_binary (PLUS, mode,
					     folded_constant, *act1);
      if (!last)
	return folded_constant;
      if (folded_constant == const0_rtx)
	*last = XEXP (*last, 1);
      else
	*act1 = folded_constant;
    }
  else if (folded_constant != const0_rtx)
    *act1 = gen_rtx_fmt_ee (PLUS, mode, folded_constant, *act1);

  if (GET_CODE (expr) == PLUS)
    expr = sort_ops (expr);

  return expr;
}

/* Tries to simplify MODE subreg of EXPR of mode EXPR_MODE (important for
   constants).  The simplification is done in place.  */
rtx
simplify_alg_expr_subreg (expr, expr_mode, mode)
     rtx expr;
     enum machine_mode expr_mode;
     enum machine_mode mode;
{
  enum machine_mode inner_mode;

  if (expr_mode == mode)
    return expr;
  switch (GET_CODE (expr))
    {
    case PLUS:
    case MULT:
      XEXP (expr, 0) = simplify_alg_expr_subreg (XEXP (expr, 0), expr_mode, mode);
      XEXP (expr, 1) = simplify_alg_expr_subreg (XEXP (expr, 1), expr_mode, mode);
      PUT_MODE (expr, mode);
      return expr;

    case ITERATION:
      PUT_MODE (expr, mode);
      return expr;

    case CONST_INT:
      return simplify_gen_subreg (mode, expr, expr_mode, 0);

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      inner_mode = GET_MODE (XEXP (expr, 0));
      if (GET_MODE_SIZE (inner_mode) < GET_MODE_SIZE (mode))
	{
	  PUT_MODE (expr, mode);
	  return expr;
	}
      return simplify_alg_expr_subreg (XEXP (expr, 0), inner_mode, mode);

    case SUBREG:
      inner_mode = GET_MODE (XEXP (expr, 0));
      if (GET_MODE_SIZE (expr_mode) >= GET_MODE_SIZE (inner_mode)
	  || XINT (expr, 1) != 0)
	break;

      return simplify_alg_expr_subreg (XEXP (expr, 0), inner_mode, mode);

    default:
      break;
    }

  return gen_rtx_fmt_ei (SUBREG, mode, expr, 0);
}

/* Return const0_rtx if the comparison CODE with constant argument PAR is
   always false for operand in INNER_MODE extended to MODE, if the PAR is
   on right side.  If it is always true, return const_true_rtx.  Otherwise
   return NULL.  */
static rtx
compare_with_mode_bounds (code, par, mode, inner_mode)
     enum rtx_code code;
     rtx par;
     enum machine_mode mode;
     enum machine_mode inner_mode;
{
  rtx mmin, mmax, rlow, rhigh;
  int sign;

  switch (code)
    {
    case LTU:
    case GTU:
    case LEU:
    case GEU:
      sign = 0;
      break;
    default:
      sign = 1;
    }
  get_mode_bounds (inner_mode, sign, &mmin, &mmax);
  switch (code)
    {
    case EQ:
      if (simplify_gen_relational (LT, SImode, mode,
				   par, mmin) == const_true_rtx
	  || simplify_gen_relational (GT, SImode, mode,
				      par, mmax) == const_true_rtx)
	return const0_rtx;
      break;
    case NE:
      if (simplify_gen_relational (LT, SImode, mode,
				   par, mmin) == const_true_rtx
	  || simplify_gen_relational (GT, SImode, mode,
				      par, mmax) == const_true_rtx)
	return const_true_rtx;
      break;
      
    default:
      rlow = simplify_gen_relational (code, SImode, mode, par, mmin);
      rhigh = simplify_gen_relational (code, SImode, mode, par, mmax);
      if (rlow == rhigh)
	return rlow;
    }

  return NULL_RTX;
}

/* Attempt to simplify relational expression EXPR, whose operands are
   in mode INNER_MODE.  The simplification is done in place.  */
static rtx
simplify_alg_expr_relational (expr, inner_mode)
     rtx expr;
     enum machine_mode inner_mode;
{
  rtx left, right, comp;
  enum rtx_code code, left_code, right_code, extend;
  enum machine_mode result_mode, extended_mode;

  /* If one of operands of comparison is {sign,zero}_extend and the other
     one is either constant or the same extend, we simplify them to work
     on the inner mode of extend instead.  */

  if (GET_MODE_CLASS (inner_mode) != MODE_INT
      && GET_MODE_CLASS (inner_mode) != MODE_PARTIAL_INT)
    return expr;

  code = GET_CODE (expr);
  if (!TEST_BIT (suitable_code, code))
      return NULL_RTX;
  result_mode = GET_MODE (expr);
  left = XEXP (expr, 0);
  left_code = GET_CODE (left);
  right = XEXP (expr, 1);
  right_code = GET_CODE (right);

  switch (left_code)
    {
    case CONST_INT:
      extend = NIL;
      extended_mode = VOIDmode;
      break;

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      extend = left_code;
      extended_mode = GET_MODE (XEXP (left, 0));
      break;

    default:
     return expr; 
    }

  switch (right_code)
    {
    case CONST_INT:
      if (extend == NIL)
	return simplify_gen_relational (code, result_mode, VOIDmode,
					left, right);
      comp = compare_with_mode_bounds (swap_condition (code),
				       right, inner_mode, extended_mode);
      if (comp)
	return comp;
      XEXP (expr, 0) = XEXP (left, 0);
      return expr;

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      if (extend == NIL)
	{
	  extended_mode = GET_MODE (XEXP (right, 0));
	  comp = compare_with_mode_bounds (code, left,
					   inner_mode, extended_mode);
	  if (comp)
	    return comp;
	  XEXP (expr, 1) = XEXP (right, 0);
	  return expr;
	}

      if (extend != right_code
	  || extended_mode != GET_MODE (XEXP (right, 0)))
	return expr;
      break;

    default:
     return expr; 
    }

  XEXP (expr, 0) = XEXP (left, 0);
  XEXP (expr, 1) = XEXP (right, 0);
  return expr;
}

/* Attempt to bring EXPR into canonical shape described below.  It would
   be nice if we could use simplify_rtx for it; but it is too low level
   for our purposes, and does basically the reverse of transformations
   we want.  The simplification is partly done in-place.  */
rtx
simplify_alg_expr (expr)
     rtx expr;
{
  enum machine_mode mode, inner_mode = VOIDmode;
  HOST_WIDE_INT val;
  int i, length;
  const char *format;
  rtx tmp, *current;
  enum rtx_code code;

  if (!expr || simple_expr_p (expr))
    return expr;
  mode = GET_MODE (expr);

  /* The shape of the resulting expression is

     expr = mexp | expr + mexp
     mexp = whatever | nexp * whatever

     Operands to commutative operations and comparisons are ordered
     according to alg_compare_rtx inside whatevers.  The list of mexp
     operands in expr, as well as list of whatever operands in mexp
     is in this order as well.
     
     We do also a few other optimizations on subregs and extends.  */

  /* Do some canonicalization.  */
  code = GET_CODE (expr);
  switch (code)
    {
    case MINUS:
      /* (MINUS x y) == (PLUS x (MULT (-1) y))  */
      tmp = XEXP (expr, 1);
      tmp = gen_rtx_fmt_ee (MULT, mode, constm1_rtx, tmp);
      expr = gen_rtx_fmt_ee (PLUS, mode, XEXP (expr, 0), tmp);
      break;
      
    case NEG:
      /* (NEG x) == (MULT (-1) x)  */
      tmp = XEXP (expr, 0);
      expr = gen_rtx_fmt_ee (MULT, mode, constm1_rtx, tmp);
      break;

    case ASHIFT:
      /* (ASHIFT x const) == (MULT (2^const) x)  */
      tmp = XEXP (expr, 1);
      if (GET_CODE (tmp) != CONST_INT)
	break;
      val = INTVAL (tmp);
      expr = gen_rtx_fmt_ee (MULT, mode, GEN_INT (1 << val), XEXP (expr, 0));
      break;

    case NOT:
      /* (NOT x) == (PLUS (-1) (NEG x)) */
      tmp = XEXP (expr, 0);
      tmp = gen_rtx_fmt_e (NEG, mode, tmp);
      expr = gen_rtx_fmt_ee (PLUS, mode, constm1_rtx, tmp);
      break;

    case CONST:
      /* Throw away consts so that we see what is inside.  */
      return simplify_alg_expr (XEXP (expr, 0));

    case CONST_INT:
    case SYMBOL_REF:
      /* These take the shortcut above.  */
      abort ();

    case INITIAL_VALUE:
    case REG:
    case ITERATION:
      return expr;

    case SUBREG:
    case ZERO_EXTEND:
    case SIGN_EXTEND:
      /* Remember mode, as it is lost when the inner expression is simplified
	 to constant.  */
      inner_mode = GET_MODE (XEXP (expr, 0));
      break;

    default: ;
    }
  code = GET_CODE (expr);

  if (GET_RTX_CLASS (code) == '<')
    {
      /* Remember the mode of operands, for purposes of constant folding.  */
      inner_mode = GET_MODE (XEXP (expr, 0));
      if (inner_mode == VOIDmode)
	inner_mode = GET_MODE (XEXP (expr, 1));
    }

  /* Simplify subexpressions.  */
  length = GET_RTX_LENGTH (code);
  format = GET_RTX_FORMAT (code);
  for (i = 0; i < length; i++)
    {
      switch (format[i])
	{
	case 'e':
	  XEXP (expr, i) = simplify_alg_expr (XEXP (expr, i));
	  break;

	default:
	  break;
	}
    }

  switch (code)
    {
    case MULT:
      expr = use_distributive_law (expr);
      current = &expr;
      while (GET_CODE (*current) == PLUS)
	{
	  XEXP (*current, 1) = simplify_alg_expr_mult (XEXP (*current, 1));
	  current = &XEXP (*current, 0);
	}
      *current = simplify_alg_expr_mult (*current);

      if (GET_CODE (expr) != PLUS)
	return expr;

      /* Fallthru.  */
    case PLUS:
      /* Straighten the list of summands.  */
      expr = straighten_ops (expr);
      expr = simplify_alg_expr_plus (expr);
      return expr;

    case SUBREG:
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (inner_mode)
	  || XINT (expr, 1) != 0)
	break;

      expr = simplify_alg_expr_subreg (XEXP (expr, 0), inner_mode, mode);
      if (GET_CODE (expr) != SUBREG)
	expr = simplify_alg_expr (expr);
      return expr;

    case DIV:
    case UDIV:
      if (XEXP (expr, 1) == const1_rtx)
	expr = XEXP (expr, 0);
      break;

    case MOD:
    case UMOD:
      if (XEXP (expr, 1) == const1_rtx)
	expr = const0_rtx;
      break;

    default:
      if (GET_RTX_CLASS (code) == '<')
	expr = simplify_alg_expr_relational (expr, inner_mode);
    }
  code = GET_CODE (expr);

  /* Fold constants.  */
  length = GET_RTX_LENGTH (code);
  format = GET_RTX_FORMAT (code);
  for (i = 0; i < length; i++)
    switch (format[i])
      {
      case 'e':
	if (GET_CODE (XEXP (expr, i)) != CONST_INT)
	  i = length + 1;
	break;
      case 'i':
	break;
      default:
	i = length + 1;
      }
  if (i == length)
    {
      switch (code)
	{
	case SUBREG:
	  tmp = simplify_gen_subreg (mode, XEXP (expr, 0),
				     inner_mode, XINT (expr, 1));
	  break;
	case SIGN_EXTEND:
	case ZERO_EXTEND:
	  tmp = simplify_gen_unary (code, mode, XEXP (expr, 0), inner_mode);
	  break;
	default:
	    tmp = simplify_rtx (expr);
	}
      if (tmp && GET_CODE (tmp) == CONST_INT)
	return tmp;
    }

  /* Sort the operands if possible.  */
  if (GET_RTX_CLASS (code) == 'c')
    {
      if (alg_compare_rtx (XEXP (expr, 0), XEXP (expr, 1)) > 0)
	{
     	  tmp = XEXP (expr, 0);
	  XEXP (expr, 0) = XEXP (expr, 1);
	  XEXP (expr, 1) = tmp;
	}
    }
  else if (GET_RTX_CLASS (code) == '<')
    {
      code = swap_condition (code);
      if (code != UNKNOWN
	  && alg_compare_rtx (XEXP (expr, 0), XEXP (expr, 1)) > 0)
	{
	  PUT_CODE (expr, code);
     	  tmp = XEXP (expr, 0);
	  XEXP (expr, 0) = XEXP (expr, 1);
	  XEXP (expr, 1) = tmp;
	}
    }

  return expr;
}

/* Initialize table of codes we are able to process.  */
void
init_algebraic ()
{
  unsigned i;

  suitable_code = sbitmap_alloc (NUM_RTX_CODE);
  sbitmap_zero (suitable_code);
  SET_BIT (suitable_code, CONST_INT);
  SET_BIT (suitable_code, CONST);
  SET_BIT (suitable_code, REG);
  SET_BIT (suitable_code, SYMBOL_REF);
  SET_BIT (suitable_code, IF_THEN_ELSE);
  SET_BIT (suitable_code, PLUS);
  SET_BIT (suitable_code, MINUS);
  SET_BIT (suitable_code, NEG);
  SET_BIT (suitable_code, MULT);
  SET_BIT (suitable_code, DIV);
  SET_BIT (suitable_code, MOD);
  SET_BIT (suitable_code, UMOD);
  SET_BIT (suitable_code, UDIV);
  SET_BIT (suitable_code, AND);
  SET_BIT (suitable_code, IOR);
  SET_BIT (suitable_code, NOT);
  SET_BIT (suitable_code, ASHIFT);
  SET_BIT (suitable_code, ASHIFTRT);
  SET_BIT (suitable_code, LSHIFTRT);
  SET_BIT (suitable_code, NE);
  SET_BIT (suitable_code, EQ);
  SET_BIT (suitable_code, GE);
  SET_BIT (suitable_code, GT);
  SET_BIT (suitable_code, LE);
  SET_BIT (suitable_code, LT);
  SET_BIT (suitable_code, GEU);
  SET_BIT (suitable_code, GTU);
  SET_BIT (suitable_code, LEU);
  SET_BIT (suitable_code, LTU);
  SET_BIT (suitable_code, SUBREG);
  SET_BIT (suitable_code, SIGN_EXTEND);
  SET_BIT (suitable_code, ZERO_EXTEND);
  SET_BIT (suitable_code, INITIAL_VALUE);
  SET_BIT (suitable_code, VALUE_AT);
  SET_BIT (suitable_code, ITERATION);

  for (i = 0; i < NUM_MACHINE_MODES; i++)
    iteration_rtx[i] = gen_rtx (ITERATION, i);
}

/* Substitutes values from SUBSTITUTION (i-th element corresponds to a value
   of reg i) into EXPR.  Only values of registers specified by a
   INTERESTING_REG bitmap are considered valid.
   If SIE_SIMPLIFY bit is set in FLAGS, also simplify the resulting expression.
   If SIE_ONLY_SIMPLE is set, only substitute simple expressions.
   RESULT_MODE indicates the real mode of target of resulting expression
   (used when simplifying).  If the substitution fails to be performed,
   NULL_RTX is returned instead.  */
rtx
substitute_into_expr (expr, interesting_reg, substitution, flags)
     rtx expr;
     sbitmap interesting_reg;
     rtx *substitution;
     int flags;
{
  rtx old_expr, new_expr, sub_expr;
  unsigned regno;
  int i, length;
  const char *format;
  enum machine_mode mode, inner_mode = VOIDmode;
  enum rtx_code code;
 
  if (!expr || good_constant_p (expr))
    return expr;

  mode = GET_MODE (expr);
  if (mode != VOIDmode
      && GET_MODE_CLASS (mode) != MODE_INT
      && GET_MODE_CLASS (mode) != MODE_PARTIAL_INT)
    return NULL_RTX;

  old_expr = expr;
  if (GET_CODE (expr) == INITIAL_VALUE)
    expr = XEXP (expr, 0);

  code = GET_CODE (expr);
  if (code == REG)
    {
      rtx val;
      regno = REGNO (expr);
      if (!TEST_BIT (interesting_reg, regno))
	return NULL_RTX;

      val = substitution[regno];
      if (!val)
	return NULL_RTX;

      if ((flags & SIE_ONLY_SIMPLE)
	  && !simple_expr_p (val))
	return old_expr;

      /* Optimize some common cases that may be shared.  */
      switch (GET_CODE (val))
	{
	case INITIAL_VALUE:
	case VALUE_AT:
	case CONST_INT:
	case SYMBOL_REF:
	  return val;

	default:
	  return copy_rtx (val);
	}
    }
  
  /* Just ignore the codes that do not seem to be good for further
     processing.  */
  if (!TEST_BIT (suitable_code, code))
      return NULL_RTX;

  if (code == SUBREG || code == ZERO_EXTEND || code == SIGN_EXTEND)
    inner_mode = GET_MODE (XEXP (expr, 0));

  length = GET_RTX_LENGTH (code);
  format = GET_RTX_FORMAT (code);
  new_expr = shallow_copy_rtx (expr);
  for (i = 0; i < length; i++)
    {
      switch (format[i])
	{
	case 'e':
	  sub_expr = substitute_into_expr (XEXP (expr, i),
					   interesting_reg, substitution,
					   flags & ~SIE_SIMPLIFY);
	  if (!sub_expr)
	    return NULL_RTX;
	  XEXP (new_expr, i) = sub_expr;
	  break;

	case 'V':
	case 'E':
	  return NULL_RTX;

	default:
	  /* Nothing to do.  */
	  break;
	}
    }

  if ((code == SUBREG || code == ZERO_EXTEND || code == SIGN_EXTEND)
      && inner_mode != VOIDmode
      && GET_MODE (XEXP (new_expr, 0)) == VOIDmode)
    {
      if (code == SUBREG)
	new_expr = simplify_gen_subreg (mode, XEXP (new_expr, 0),
					inner_mode, XINT (expr, 1));
      else
	new_expr = simplify_gen_unary (code, mode,
				       XEXP (new_expr, 0), inner_mode);
    }

  if ((flags & SIE_SIMPLIFY) && !simple_expr_p (new_expr))
    new_expr = simplify_alg_expr (new_expr);

  return new_expr;
}

/* Try to simplify expression VAR using register initial values stored
   in INITIAL_VALUES; only registers in INTERESTING_REG are considered
   to have a valid value in INITIAL_VALUES.  Returns the simplified form
   of VAR or VAR if no simplification is possible.  Strongly biassed
   for an induction variable analysis.  */
rtx
simplify_alg_expr_using_values (var, interesting_reg, initial_values)
     rtx var;
     sbitmap interesting_reg;
     rtx *initial_values;
{
  rtx base, step, sbase, sstep, wrap = NULL_RTX;
  int changed;
  enum machine_mode mode;

  if (simple_expr_p (var)
      || GET_CODE (var) == VALUE_AT
      || GET_CODE (var) == ITERATION)
    return var;

  if (GET_CODE (var) == SIGN_EXTEND || GET_CODE (var) == ZERO_EXTEND)
    {
      wrap = var;
      var = XEXP (var, 0);
    }
  mode = GET_MODE (var);

  iv_split (var, &base, &step);
  if (!base)
    return NULL_RTX;

  changed = false;
  if (fast_expr_mentions_code_p (base, INITIAL_VALUE))
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
  if (fast_expr_mentions_code_p (step, INITIAL_VALUE))
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
      if (step == const0_rtx)
	var = base;
      else
	{
	  var = gen_iteration (mode);
	  if (step != const1_rtx)
	    var = gen_rtx_fmt_ee (MULT, mode, step, var);
	  if (base != const0_rtx)
	    var = gen_rtx_fmt_ee (PLUS, mode, base, var);
	}
    }

  if (wrap && GET_MODE (var) == mode)
    {
      XEXP (wrap, 0) = var;
      var = wrap;
    }
  return var;
}

/* Splits expression for induction variable into BASE and STEP.  We expect
   EXPR to come from simplify_alg_expr.  EXPR is not modified, but part of
   it may be shared as BASE or STEP.  */
void
iv_split (expr, base, step)
     rtx expr;
     rtx *base;
     rtx *step;
{
  rtx abase, astep, *pabase = &abase, *pastep = &astep;
  rtx next, *act, tmp;
  enum machine_mode mode;

  *base = *step = NULL_RTX;
  if (!expr)
    return;
  mode = GET_MODE (expr);

  if (good_constant_p (expr)
      || GET_CODE (expr) == VALUE_AT
      || GET_CODE (expr) == INITIAL_VALUE
      || !fast_expr_mentions_code_p (expr, ITERATION))
    {
      *base = expr;
      *step = const0_rtx;
      return;
    }

  /* Avoid copying in the most common cases:  */
  if (GET_CODE (expr) != PLUS
      || !fast_expr_mentions_code_p (XEXP (expr, 0), ITERATION))
    {
      if (GET_CODE (expr) == PLUS)
	{
	  *base = XEXP (expr, 0);
	  tmp = XEXP (expr, 1);
	}
      else
	{
	  *base = const0_rtx;
	  tmp = expr;
	}

      if (GET_CODE (tmp) == ITERATION)
	*step = const1_rtx;
      else if (GET_CODE (tmp) == MULT
	       && GET_CODE (XEXP (tmp, 0)) == CONST_INT
	       && GET_CODE (XEXP (tmp, 1)) == ITERATION)
	*step = XEXP (tmp, 0);
      else
	*base = NULL_RTX;

      if (*base)
	return;
    }

  expr = copy_rtx (expr);
  while (GET_CODE (expr) == PLUS)
    {
      next = XEXP (expr, 0);
      act = &XEXP (expr, 1);
      if (GET_CODE (*act) == MULT)
	act = &XEXP (*act, 1);
      if (GET_CODE (*act) == ITERATION)
	{
	  *act = const1_rtx;
	  *pastep = expr;
	  pastep = &XEXP (expr, 0);
	}
      else
	{
	  *pabase = expr;
	  pabase = &XEXP (expr, 0);
	}
      expr = next;
    }
  if (GET_CODE (expr) == ITERATION)
    {
      *pastep = const1_rtx;
      *pabase = const0_rtx;
    }
  else if (GET_CODE (expr) == MULT
	   && GET_CODE (XEXP (expr, 1)) == ITERATION)
    {
      *pastep = expr;
      XEXP (expr, 1) = const1_rtx;
      *pabase = const0_rtx;
    }
  else
    {
      *pastep = const0_rtx;
      *pabase = expr;
    }

  if (fast_expr_mentions_code_p (astep, ITERATION)
      || fast_expr_mentions_code_p (abase, ITERATION))
    return;

  *base = simplify_alg_expr (abase);
  *step = simplify_alg_expr (astep);
}

/* Generate ITERATION in MODE (they are shared, so just return the rtx).  */
rtx
gen_iteration (mode)
     enum machine_mode mode;
{
  return iteration_rtx[mode];
}

