/* Functions to analyze and validate SIMPLE trees.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _TREE_SIMPLE_H
#define _TREE_SIMPLE_H 1

/* Interface used in [break/goto]-elimination: to be declared in a .h file. */
extern void insert_before_continue_end PARAMS ((tree, tree));
extern void tree_build_scope           PARAMS ((tree *));
extern tree create_tmp_var             PARAMS ((tree, const char *));
extern tree create_tmp_alias_var       PARAMS ((tree, const char *));
extern bool is_simple_tmp_var	       PARAMS ((tree));
extern tree get_initialized_tmp_var    PARAMS ((tree, tree *));
extern tree declare_tmp_vars           PARAMS ((tree, tree));
extern tree deep_copy_list             PARAMS ((tree));
extern tree deep_copy_node             PARAMS ((tree));
extern tree update_line_number         PARAMS ((tree, int));

/* Validation of SIMPLE expressions.  */
int is_simple_expr                     PARAMS ((tree));
int is_simple_rhs                      PARAMS ((tree));
int is_simple_modify_expr              PARAMS ((tree));
int is_simple_modify_expr_lhs          PARAMS ((tree));
bool is_simple_relop		       PARAMS ((enum tree_code));
int is_simple_binary_expr              PARAMS ((tree));
int is_simple_condexpr                 PARAMS ((tree));
int is_simple_unary_expr               PARAMS ((tree));
int is_simple_call_expr                PARAMS ((tree));
int is_simple_arglist                  PARAMS ((tree));
int is_simple_const                    PARAMS ((tree));
int is_simple_id                       PARAMS ((tree));
int is_simple_varname                  PARAMS ((tree));
int is_simple_addr_expr_arg            PARAMS ((tree));
int is_simple_val                      PARAMS ((tree));
int is_simple_min_lval                 PARAMS ((tree));
int is_simple_compound_lval            PARAMS ((tree));
int is_simple_arrayref                 PARAMS ((tree));
int is_simple_compref                  PARAMS ((tree));
int is_simple_cast                     PARAMS ((tree));
int is_simple_cast_op                  PARAMS ((tree));
int is_simple_exprseq                  PARAMS ((tree));
int is_simple_constructor              PARAMS ((tree));
int is_simple_constructor_elt          PARAMS ((tree));
int is_simple_initializer              PARAMS ((tree));
int is_simplifiable_builtin            PARAMS ((tree));
int is_simple_decl_stmt                PARAMS ((tree));

/* FIXME this needs a better name.  */
tree add_tree			       PARAMS ((tree, tree *));
/* FIXME we should deduce this from the predicate.  */
typedef enum fallback_t {
  fb_rvalue=1,
  fb_lvalue=2,
  fb_either=1|2
} fallback_t;
int simplify_expr		       PARAMS ((tree *, tree *, tree *,
						int (*) PARAMS ((tree)),
						fallback_t));

/* Miscellaneous helpers.  */
tree get_base_symbol                   PARAMS ((tree));

#endif /* _TREE_SIMPLE_H */
