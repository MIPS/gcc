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

#ifdef EL_RTX
#include "el-rtx.h"
#else
 #error "You must specify representation of expressions to use this file."
#endif

/* A predicate for what we consider a constant.  */
#define good_constant_p(EXPR)					\
  (GET_OPERATOR (EXPR) == OP_CONST_INT				\
   || GET_OPERATOR (EXPR) == OP_CONST_ADDRESS)

/* A predicate for what we consider a simple expression.  */
#define simple_expr_p(EXPR)					\
  (good_constant_p (EXPR)					\
   || (GET_OPERATOR (EXPR) == OP_PLUS				\
       && GET_OPERATOR (ARG (EXPR, 1)) == OP_CONST_ADDRESS	\
       && GET_OPERATOR (ARG (EXPR, 0)) == OP_CONST_INT))

/* A wrapper around expr_mentions_code_p to speed it up it on the most common
   cases in induction variable analysis.  Designed to be used with ITERATION
   and INITIAL_VALUE code.  They expect EXPR to be in the simplify_alg_expr
   form.  */
#define fast_expr_mentions_operator_p(EXPR, OP)		\
  (GET_OPERATOR (EXPR) == OP				\
   || (COMUTATIVE_OP_P (GET_OPERATOR (EXPR))		\
       && GET_OPERATOR (ARG (EXPR, 1)) == OP)		\
   || (!good_constant_p (EXPR)				\
       && GET_OPERATOR (EXPR) != OP_UNSPECIFIED_VALUE	\
       && EXPR_MENTIONS_OPERATOR_P ((EXPR), OP)))
     
/* Recognizes the special case of if_then_else we use to enable handling of 
   the induction variables that are represented in a wider mode register
   than the mode in that they iterate.  */
#define bival_p(EXPR)						\
  (GET_OPERATOR (EXPR) == OP_IF_THEN_ELSE			\
   && GET_OPERATOR (ARG ((EXPR), 0)) == OP_EQ			\
   && GET_OPERATOR (ARG (ARG ((EXPR), 0), 1)) == OP_ITERATION	\
   && GET_OPERATOR (ARG (ARG ((EXPR), 0), 0)) == OP_CONST_INT	\
   && CONST_INT_VALUE (ARG (ARG ((EXPR), 0), 0)) == 0)

/* Flags for substitute_into_expr.  */
enum
{
  SIE_SIMPLIFY = 1,	/* Simplify the resulting expression.  */
  SIE_ONLY_SIMPLE = 2,	/* Only substitute the simple expressions.  */
  SIE_DEFAULT_ID = 4	/* NULL members of substitution should be considered
			   identity.  */
};

#define init_algebraic ELNF (init_algebraic)
#define substitute_into_expr ELNF (substitute_into_expr)
#define simplify_alg_expr_shorten ELNF (simplify_alg_expr_shorten)
#define simplify_alg_expr ELNF (simplify_alg_expr)
#define simplify_alg_expr_using_values ELNF (simplify_alg_expr_using_values)
#define gen_iteration ELNF (gen_iteration)
#define gen_bival ELNF (gen_bival)
#define iv_split ELNF (iv_split)

extern void init_algebraic (void);
extern EXPR substitute_into_expr (EXPR, sbitmap, EXPR *, int);
extern EXPR simplify_alg_expr_shorten (EXPR, EXPR_TYPE, EXPR_TYPE);
extern EXPR simplify_alg_expr (EXPR);
extern EXPR simplify_alg_expr_using_values (EXPR, sbitmap, EXPR *);

/* These functions are directly specific to induction variable analysis,
   but it is easier to include them here due to a common code shared.  */
extern EXPR gen_iteration (EXPR_TYPE);
extern EXPR gen_bival (EXPR_TYPE, EXPR, EXPR);
extern void iv_split (EXPR, EXPR *, EXPR *);
