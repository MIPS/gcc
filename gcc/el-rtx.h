/* Rtl hooks for generic expression manipulation.
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

#define ELNF(FN) el_rtx_##FN

#define EXPR rtx
#define EXPR_TYPE enum machine_mode
#define OPERATOR enum rtx_code

#define EXPR_EQUAL_P(X1, X2) (rtx_equal_p (X1, X2))
#define EXPR_CONSTANT_P(X) (CONSTANT_P (X))
#define COPY_EXPR(X) (copy_rtx (X))
#define SHALLOW_COPY_EXPR(X) (shallow_copy_rtx (X))
#define EXPR_MENTIONS_OPERATOR_P(X, O) (expr_mentions_code_p (X, O))

#define GEN_BINARY(OP, TYPE, X1, X2) (gen_rtx_fmt_ee (OP, TYPE, X1, X2))
#define FOLD_BINARY(OP, TYPE, X1, X2) (simplify_gen_binary (OP, TYPE, X1, X2))
#define GEN_UNARY(OP, TYPE, X) (gen_rtx_fmt_e (OP, TYPE, X))
#define FOLD_EXTEND(OP, TYPE, X, XTYPE) (simplify_gen_unary (OP, TYPE, X, XTYPE))
#define FOLD_RELATIONAL(OP, TYPE, X1, X2) \
  (simplify_gen_relational (OP, VOIDmode, TYPE, X1, X2))
#define GEN_SHORTEN(TYPE, X) (gen_rtx_fmt_ei (SUBREG, TYPE, X, 0))
#define FOLD_CAST(TYPE, X, FROM) (simplify_gen_subreg (TYPE, X, FROM, 0))
#define GEN_TERNARY(O, TYPE, X1, X2, X3) (gen_rtx_fmt_eee (O, TYPE, X1, X2, X3))
#define GENERIC_FOLD(X) (simplify_rtx (X))

#define ARG(X, N) (XEXP (X, N))
#define NUM_OP_ARGS(O) (GET_RTX_LENGTH (O))
#define ITH_ARG_EXPR_P(O, I) (GET_RTX_FORMAT (O)[i] == 'e')
#define ITH_ARG_CONST_P(O, I) (GET_RTX_FORMAT (O)[i] == 'i')

#define NULL_EXPR NULL_RTX
#define CONST_INT_VALUE(N) (INTVAL (N))
#define CONST_INT_EXPR(N) (GEN_INT (N))
#define VARIABLE_ID(X) (REGNO (X))
#define TRUE_P(X) ((X) == const_true_rtx)
#define EXPR_TRUE const_true_rtx
#define FALSE_P(X) ((X) == const0_rtx)
#define EXPR_FALSE const0_rtx
#define EXPR_COMPARE_UNSPECIFIED_VALUES(X1, X2)			\
  (XINT (X1, 0) < XINT (X2, 0)					\
  ? -1								\
  : XINT (X1, 0) > XINT (X2, 0)					\
    ? 1								\
    : XINT (X1, 1) < XINT (X1, 1)				\
      ? -1							\
      : XINT (X1, 1) > XINT (X1, 1)				\
	? 1							\
	: 0)

#define GET_OPERATOR(X) (GET_CODE (X))
#define COMUTATIVE_OP_P(OP) (GET_RTX_CLASS (OP) == 'c')
#define COMPARISON_OP_P(OP) (GET_RTX_CLASS (OP) == '<')
#define BINARY_OP_P(OP) (GET_RTX_CLASS (OP) == '<'		\
			 || GET_RTX_CLASS (OP) == 'c'		\
			 || GET_RTX_CLASS (OP) == '2')
#define SWAP_CONDITION(X) (swap_condition (X))

#define OP_NOP NIL
#define OP_CONST_INT CONST_INT
#define OP_CONST_ADDRESS SYMBOL_REF
#define OP_VARIABLE REG
#define OP_UNSPECIFIED_VALUE VALUE_AT
#define OP_ITERATION ITERATION
#define OP_INITIAL_VALUE INITIAL_VALUE
#define OP_IF_THEN_ELSE IF_THEN_ELSE
#define OP_PLUS PLUS
#define OP_MINUS MINUS
#define OP_MULT MULT
#define OP_DIV DIV
#define OP_UDIV UDIV
#define OP_MOD MOD
#define OP_UMOD UMOD
#define OP_NEG NEG
#define OP_NOT NOT
#define OP_ASHIFT ASHIFT
#define OP_AND AND
#define OP_EQ EQ
#define OP_LT LT
#define OP_LE LE
#define OP_NE NE
#define OP_GT GT
#define OP_GE GE
#define OP_LTU LTU
#define OP_LEU LEU
#define OP_GTU GTU
#define OP_GEU GEU
#define OP_EXTEND_SIGNED SIGN_EXTEND
#define OP_EXTEND_UNSIGNED ZERO_EXTEND
#define OP_SHORTEN SUBREG
#define OP_IOR IOR
#define OP_ASHIFTRT ASHIFTRT
#define OP_LSHIFTRT LSHIFTRT

#define EXPR_UNSPECIFIED_TYPE VOIDmode
#define STD_INT_TYPE SImode
#define LONG_INT_TYPE DImode
#define GET_EXPR_TYPE(X) (GET_MODE (X))
#define SET_EXPR_TYPE(X, TYPE) (PUT_MODE (X, TYPE))
#define GET_ARGS_TYPE(X)						\
  (GET_MODE (XEXP (X, 0)) == VOIDmode					\
   ? GET_MODE (XEXP (X, 1))						\
   : GET_MODE (XEXP (X, 0)))
#define ALL_ONES_VALUE(TYPE) (GET_MODE_MASK (TYPE))
#define INTEGER_TYPE_P(TYPE) 						\
  (GET_MODE_CLASS (TYPE) == MODE_INT					\
   || GET_MODE_CLASS (TYPE) == MODE_PARTIAL_INT)
#define GET_EXPR_TYPE_SIZE(TYPE) (GET_MODE_SIZE (TYPE))
#define DONT_SIMPLIFY_CASTS(X, TYPE)					\
  (GET_MODE_SIZE (TYPE) >= GET_MODE_SIZE (GET_MODE (XEXP (X, 0)))	\
   || XINT (X, 1) != 0)
#define GET_COMPARISON_BOUNDS(TYPE, COND, MIN, MAX)			\
  (get_mode_bounds (TYPE,						\
		    COND != LEU						\
		    && COND != LTU					\
		    && COND != GEU					\
		    && COND != GTU, MIN, MAX))
