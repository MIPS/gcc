/* Functions to analyze and validate GIMPLE trees.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _TREE_SIMPLE_H
#define _TREE_SIMPLE_H 1


#include "tree-iterator.h"

extern tree create_tmp_var (tree, const char *);
extern tree create_tmp_alias_var (tree, const char *);
extern bool is_gimple_tmp_var (tree);
extern tree get_initialized_tmp_var (tree, tree *, tree *);
extern tree get_formal_tmp_var (tree, tree *);
extern void declare_tmp_vars (tree, tree);

extern tree rationalize_compound_expr (tree);
extern tree right_assocify_expr (tree);
extern void annotate_all_with_file_line (tree *, const char *, int);

/* Validation of GIMPLE expressions.  Note that these predicates only check
   the basic form of the expression, they don't recurse to make sure that
   underlying nodes are also of the right form.  */

/* Returns 1 iff T is a valid GIMPLE statement.  */
int is_gimple_stmt (tree);

/* Returns 1 iff TYPE is a valid type for a scalar register variable.  */
bool is_gimple_reg_type (tree);
/* Returns 1 iff T is a scalar register variable.  */
int is_gimple_reg (tree);
/* Returns 1 iff T is any sort of variable.  */
int is_gimple_variable (tree);
/* Returns 1 iff T is a variable or an INDIRECT_REF (of a variable).  */
int is_gimple_min_lval (tree);
/* Returns 1 iff T is an lvalue other than an INDIRECT_REF.  */
int is_gimple_addr_expr_arg (tree);
/* Returns 1 iff T is any valid GIMPLE lvalue.  */
int is_gimple_lvalue (tree);

/* Returns 1 iff T is a GIMPLE constant.  */
int is_gimple_const (tree);
/* Returns 1 iff T is a GIMPLE rvalue.  */
int is_gimple_val (tree);
/* Returns 1 iff T is a valid rhs for a MODIFY_EXPR.  */
int is_gimple_rhs (tree);

/* Returns 1 iff T is a valid if-statement condition.  */
int is_gimple_condexpr (tree);

/* Returns 1 iff T is a type conversion.  */
int is_gimple_cast (tree);
/* Returns 1 iff T is a valid CONSTRUCTOR element (either an rvalue or
   another CONSTRUCTOR).  */
int is_gimple_constructor_elt (tree);

void recalculate_side_effects (tree);

typedef void foreach_stmt_fn (tree *);
void foreach_stmt (tree *, foreach_stmt_fn *);

/* FIXME this needs a better name.  */
void add_tree (tree, tree *);

/* FIXME we should deduce this from the predicate.  */
typedef enum fallback_t {
  fb_none = 0,
  fb_rvalue=1,
  fb_lvalue=2,
  fb_either=1|2
} fallback_t;
int gimplify_expr (tree *, tree *, tree *, int (*) (tree), fallback_t);
int gimplify_stmt (tree *);
int gimplify_body (tree *, tree);

/* Miscellaneous helpers.  */
tree get_base_symbol (tree);
void gimple_add_tmp_var (tree);
tree gimple_current_bind_expr (void);
void gimple_push_bind_expr (tree);
void gimple_pop_bind_expr (void);
void mark_not_gimple (tree *);
void unshare_all_trees (tree);
tree voidify_wrapper_expr (tree);
tree gimple_build_eh_filter (tree, tree, tree);
tree maybe_protect_cleanup (tree);
tree build_and_jump (tree *);


#endif /* _TREE_SIMPLE_H  */
