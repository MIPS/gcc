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

/* A predicate for what we consider a constant.  */
#define good_constant_p(EXPR)				\
  (GET_CODE (EXPR) == CONST_INT				\
   || GET_CODE (EXPR) == SYMBOL_REF)

/* A predicate for what we consider a simple expression.  */
#define simple_expr_p(EXPR)				\
  (good_constant_p (EXPR)				\
   || (GET_CODE (EXPR) == PLUS				\
       && GET_CODE (XEXP (EXPR, 1)) == SYMBOL_REF	\
       && GET_CODE (XEXP (EXPR, 0)) == CONST_INT))

/* A wrapper around expr_mentions_code_p to speed it up it on the most common
   cases in induction variable analysis.  Designed to be used with ITERATION
   and INITIAL_VALUE code.  They expect EXPR to be in the simplify_alg_expr
   form.  */
#define fast_expr_mentions_code_p(EXPR, CODE) 		\
  (GET_CODE(EXPR) == CODE				\
   || (GET_RTX_CLASS (GET_CODE (EXPR)) == 'c'		\
       && GET_CODE(XEXP (EXPR, 1)) == CODE)		\
   || (!good_constant_p (EXPR)				\
       && GET_CODE(EXPR) != VALUE_AT			\
       && expr_mentions_code_p ((EXPR), CODE)))
     
/* Flags for substitute_into_expr.  */
enum
{
  SIE_SIMPLIFY = 1,	/* Simplify the resulting expression.  */
  SIE_ONLY_SIMPLE = 2	/* Only substitute the simple expressions.  */
};

extern void init_algebraic		PARAMS ((void));
extern rtx substitute_into_expr		PARAMS ((rtx, sbitmap, rtx *, int));
extern rtx simplify_alg_expr_subreg	PARAMS ((rtx, enum machine_mode,
						 enum machine_mode));
extern rtx simplify_alg_expr		PARAMS ((rtx));
extern rtx simplify_alg_expr_using_values PARAMS ((rtx, sbitmap, rtx *));

/* These functions are directly specific to induction variable analysis,
   but it is easier to include them here due to a common code shared.  */
extern rtx gen_iteration		PARAMS ((enum machine_mode));
extern void iv_split			PARAMS ((rtx, rtx *, rtx *));
