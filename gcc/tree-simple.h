/* Functions to analyze and validate GIMPLE trees.
   Copyright (C) 2002 Free Software Foundation, Inc.
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
extern void insert_before_continue_end PARAMS ((tree, tree));
extern void tree_build_scope           PARAMS ((tree *));
extern tree create_tmp_var             PARAMS ((tree, const char *));
extern tree create_tmp_alias_var       PARAMS ((tree, const char *));
extern bool is_gimple_tmp_var	       PARAMS ((tree));
extern tree get_initialized_tmp_var    PARAMS ((tree, tree *));
extern tree get_formal_tmp_var         PARAMS ((tree, tree *));
extern void declare_tmp_vars           PARAMS ((tree, tree));
extern tree deep_copy_list             PARAMS ((tree));
extern tree deep_copy_node             PARAMS ((tree));
extern tree update_line_number         PARAMS ((tree, int));

extern tree rationalize_compound_expr  PARAMS ((tree));
extern tree right_assocify_expr		PARAMS ((tree));
extern void annotate_all_with_file_line PARAMS ((tree *, const char *, int));

/* Validation of GIMPLE expressions.  */
int is_gimple_expr                     PARAMS ((tree));
int is_gimple_rhs                      PARAMS ((tree));
int is_gimple_modify_expr              PARAMS ((tree));
int is_gimple_modify_expr_lhs          PARAMS ((tree));
bool is_gimple_relop		       PARAMS ((enum tree_code));
int is_gimple_binary_expr              PARAMS ((tree));
int is_gimple_condexpr                 PARAMS ((tree));
int is_gimple_unary_expr               PARAMS ((tree));
int is_gimple_call_expr                PARAMS ((tree));
int is_gimple_arglist                  PARAMS ((tree));
int is_gimple_const                    PARAMS ((tree));
int is_gimple_id                       PARAMS ((tree));
int is_gimple_varname                  PARAMS ((tree));
int is_gimple_addr_expr_arg            PARAMS ((tree));
int is_gimple_val                      PARAMS ((tree));
int is_gimple_min_lval                 PARAMS ((tree));
int is_gimple_compound_lval            PARAMS ((tree));
int is_gimple_arrayref                 PARAMS ((tree));
int is_gimple_compref                  PARAMS ((tree));
int is_gimple_cast                     PARAMS ((tree));
int is_gimple_cast_op                  PARAMS ((tree));
int is_gimple_exprseq                  PARAMS ((tree));
int is_gimple_constructor              PARAMS ((tree));
int is_gimple_constructor_elt          PARAMS ((tree));
int is_gimple_initializer              PARAMS ((tree));
int is_gimplifiable_builtin            PARAMS ((tree));
int is_gimple_stmt                     PARAMS ((tree));

void recalculate_side_effects		PARAMS ((tree));

typedef void foreach_stmt_fn PARAMS ((tree *));
void foreach_stmt PARAMS ((tree *, foreach_stmt_fn *));

/* FIXME this needs a better name.  */
tree add_tree			       PARAMS ((tree, tree *));

/* FIXME we should deduce this from the predicate.  */
typedef enum fallback_t {
  fb_none = 0,
  fb_rvalue=1,
  fb_lvalue=2,
  fb_either=1|2
} fallback_t;
int gimplify_expr		       PARAMS ((tree *, tree *, tree *,
						int (*) PARAMS ((tree)),
						fallback_t));
int gimplify_stmt		       PARAMS ((tree *));

/* Miscellaneous helpers.  */
tree get_base_symbol			PARAMS ((tree));
void gimple_add_tmp_var			PARAMS ((tree));
tree gimple_current_bind_expr	        PARAMS ((void));
void gimple_push_bind_expr	        PARAMS ((tree));
void gimple_pop_bind_expr	        PARAMS ((void));
void mark_not_gimple			PARAMS ((tree *));
void unshare_all_trees			PARAMS ((tree));
tree voidify_wrapper_expr		PARAMS ((tree));
tree gimple_build_eh_filter		PARAMS ((tree, tree, tree));
tree maybe_protect_cleanup		PARAMS ((tree));


#endif /* _TREE_SIMPLE_H  */
