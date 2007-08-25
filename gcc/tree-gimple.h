/* Functions to analyze and validate GIMPLE trees.
   Copyright (C) 2002, 2003, 2005, 2007 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef _TREE_GIMPLE_H
#define _TREE_GIMPLE_H 1

#include "tree-iterator.h"
#include "gimple.h"


/* Class of GIMPLE expressions suitable for the RHS of assignments.  See
   get_gimple_rhs_class.  */
enum gimple_rhs_class
{
  GIMPLE_INVALID_RHS,	/* The expression cannot be used on the RHS.  */
  GIMPLE_BINARY_RHS,	/* The expression is a binary operation.  */
  GIMPLE_UNARY_RHS,	/* The expression is a unary operation.  */
  GIMPLE_SINGLE_RHS	/* The expression is a single object (an SSA
			   name, a _DECL, a _REF, etc.  */
};

/* In gimplify.c  */
extern tree create_tmp_var_raw (tree, const char *);
extern tree create_tmp_var_name (const char *);
extern tree create_tmp_var (tree, const char *);
extern tree get_initialized_tmp_var (tree, gimple_seq, gimple_seq);
extern tree get_formal_tmp_var (tree, gimple_seq);
extern void declare_vars (tree, gimple, bool);
extern void annotate_all_with_locus (gimple_seq, location_t);

/* Validation of GIMPLE expressions.  Note that these predicates only check
   the basic form of the expression, they don't recurse to make sure that
   underlying nodes are also of the right form.  */

typedef bool (*gimple_predicate)(tree);

/* Returns true iff T is a valid GIMPLE statement.  */
extern bool is_gimple_stmt (tree);

/* Returns true iff TYPE is a valid type for a scalar register variable.  */
extern bool is_gimple_reg_type (tree);
/* Returns true iff T is a scalar register variable.  */
extern bool is_gimple_reg (tree);
/* Returns true if T is a GIMPLE temporary variable, false otherwise.  */
extern bool is_gimple_formal_tmp_var (tree);
/* Returns true if T is a GIMPLE temporary register variable.  */
extern bool is_gimple_formal_tmp_reg (tree);
/* Returns true iff T is any sort of variable.  */
extern bool is_gimple_variable (tree);
/* Returns true iff T is any sort of symbol.  */
extern bool is_gimple_id (tree);
/* Returns true iff T is a variable or an INDIRECT_REF (of a variable).  */
extern bool is_gimple_min_lval (tree);
/* Returns true iff T is something whose address can be taken.  */
extern bool is_gimple_addressable (tree);
/* Returns true iff T is any valid GIMPLE lvalue.  */
extern bool is_gimple_lvalue (tree);

/* Returns true iff T is a GIMPLE restricted function invariant.  */
extern bool is_gimple_min_invariant (const_tree);
/* Returns true iff T is a GIMPLE rvalue.  */
extern bool is_gimple_val (tree);
/* Returns true iff T is a GIMPLE asm statement input.  */
extern bool is_gimple_asm_val (tree);
/* Returns true iff T is a valid rhs for a MODIFY_EXPR where the LHS is a
   GIMPLE temporary, a renamed user variable, or something else,
   respectively.  */
extern bool is_gimple_formal_tmp_rhs (tree);
extern bool is_gimple_reg_rhs (tree);
extern bool is_gimple_mem_rhs (tree);
/* Returns the appropriate one of the above three predicates for the LHS
   T.  */
extern gimple_predicate rhs_predicate_for (tree);

/* Returns true iff T is a valid if-statement condition.  */
extern bool is_gimple_condexpr (tree);

/* Returns true iff T is a type conversion.  */
extern bool is_gimple_cast (tree);
/* Returns true iff T is a variable that does not need to live in memory.  */
extern bool is_gimple_non_addressable (tree t);

/* Returns true iff T is a valid call address expression.  */
extern bool is_gimple_call_addr (tree);
/* If T makes a function call, returns the CALL_EXPR operand.  */
extern tree get_call_expr_in (tree t);

extern void recalculate_side_effects (tree);
extern enum gimple_rhs_class get_gimple_rhs_class (tree);

/* FIXME we should deduce this from the predicate.  */
typedef enum fallback_t {
  fb_none = 0,		/* Do not generate a temporary.  */

  fb_rvalue = 1,	/* Generate an rvalue to hold the result of a
			   gimplified expression.  */

  fb_lvalue = 2,	/* Generate an lvalue to hold the result of a
			   gimplified expression.  */

  fb_mayfail = 4,	/* Gimplification may fail.  Error issued
			   afterwards.  */
  fb_either= fb_rvalue | fb_lvalue
} fallback_t;

enum gimplify_status {
  GS_ERROR	= -2,	/* Something Bad Seen.  */
  GS_UNHANDLED	= -1,	/* A langhook result for "I dunno".  */
  GS_OK		= 0,	/* We did something, maybe more to do.  */
  GS_ALL_DONE	= 1	/* The expression is fully gimplified.  */
};

extern enum gimplify_status gimplify_expr (tree *, gimple_seq, gimple_seq,
					   bool (*) (tree), fallback_t);
extern void gimplify_type_sizes (tree, gimple_seq);
extern void gimplify_one_sizepos (tree *, gimple_seq);
extern bool gimplify_stmt (tree *, gimple_seq);
extern gimple gimplify_body (tree *, tree, bool);
extern void push_gimplify_context (void);
extern void pop_gimplify_context (gimple);
extern void gimplify_and_add (tree, gimple_seq);

/* Miscellaneous helpers.  */
extern void gimple_add_tmp_var (tree);
extern gimple gimple_current_bind_expr (void);
extern tree voidify_wrapper_expr (tree, tree);
extern tree build_gimple_eh_filter_tree (tree, tree, tree);
extern tree build_and_jump (tree *);
extern tree alloc_stmt_list (void);
extern void free_stmt_list (tree);
extern tree force_labels_r (tree *, int *, void *);
extern enum gimplify_status gimplify_va_arg_expr (tree *, gimple_seq, gimple_seq);
struct gimplify_omp_ctx;
extern void omp_firstprivatize_variable (struct gimplify_omp_ctx *, tree);
extern tree gimple_boolify (tree);

/* In omp-low.c.  */
extern void diagnose_omp_structured_block_errors (tree);
extern tree omp_reduction_init (tree, tree);

/* In tree-nested.c.  */
extern void lower_nested_functions (tree);
extern void insert_field_into_struct (tree, tree);

/* In gimplify.c.  */
extern void gimplify_function_tree (tree);

#endif /* _TREE_GIMPLE_H  */
