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

/* Interface used in [break/goto]-elimination: to be declared in a .h file. */
extern void insert_before_continue_end (tree, tree);
extern void tree_build_scope (tree *);
extern tree create_tmp_var (tree, const char *);
extern tree create_tmp_alias_var (tree, const char *);
extern bool is_gimple_tmp_var (tree);
extern tree get_initialized_tmp_var (tree, tree *);
extern tree get_formal_tmp_var (tree, tree *);
extern void declare_tmp_vars (tree, tree);
extern tree deep_copy_list (tree);
extern tree deep_copy_node (tree);
extern tree update_line_number (tree, int);

extern tree rationalize_compound_expr (tree);
extern tree right_assocify_expr (tree);
extern void annotate_all_with_file_line (tree *, const char *, int);

/* Validation of GIMPLE expressions.  */
int is_gimple_expr (tree);
int is_gimple_rhs (tree);
int is_gimple_modify_expr (tree);
int is_gimple_modify_expr_lhs (tree);
bool is_gimple_relop (enum tree_code);
int is_gimple_binary_expr (tree);
int is_gimple_condexpr (tree);
int is_gimple_unary_expr (tree);
int is_gimple_call_expr (tree);
int is_gimple_arglist (tree);
int is_gimple_const (tree);
int is_gimple_id (tree);
int is_gimple_varname (tree);
int is_gimple_addr_expr_arg (tree);
int is_gimple_val (tree);
int is_gimple_min_lval (tree);
int is_gimple_compound_lval (tree);
int is_gimple_arrayref (tree);
int is_gimple_compref (tree);
int is_gimple_cast (tree);
int is_gimple_cast_op (tree);
int is_gimple_exprseq (tree);
int is_gimple_constructor (tree);
int is_gimple_constructor_elt (tree);
int is_gimple_initializer (tree);
int is_gimplifiable_builtin (tree);
int is_gimple_stmt (tree);

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


#endif /* _TREE_SIMPLE_H  */
