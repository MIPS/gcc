/* Functions to analyze and validate SIMPLE trees.
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

/* Interface used in [break/goto]-elimination: to be declared in a .h file. */
extern void insert_before_continue_end PARAMS ((tree, tree));
extern void tree_build_scope           PARAMS ((tree *));
extern tree create_tmp_var             PARAMS ((tree, const char *));
extern tree create_tmp_alias_var       PARAMS ((tree, const char *));
extern bool is_simple_tmp_var	       PARAMS ((tree));
extern tree get_initialized_tmp_var    PARAMS ((tree, tree *));
extern void declare_tmp_vars           PARAMS ((tree, tree));
extern tree deep_copy_list             PARAMS ((tree));
extern tree deep_copy_node             PARAMS ((tree));
extern tree update_line_number         PARAMS ((tree, int));

extern tree rationalize_compound_expr  PARAMS ((tree));
extern tree right_assocify_expr		PARAMS ((tree));
extern void annotate_all_with_file_line PARAMS ((tree *, const char *, int));

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
int is_simple_stmt                     PARAMS ((tree));

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
int simplify_expr		       PARAMS ((tree *, tree *, tree *,
						int (*) PARAMS ((tree)),
						fallback_t));
int simplify_stmt		       PARAMS ((tree *));

/* Miscellaneous helpers.  */
tree get_base_symbol			PARAMS ((tree));
void gimple_add_tmp_var			PARAMS ((tree));
tree gimple_current_bind_expr	        PARAMS ((void));
void gimple_push_bind_expr	        PARAMS ((tree));
void gimple_pop_bind_expr	        PARAMS ((void));
void mark_not_simple			PARAMS ((tree *));
void unshare_all_trees			PARAMS ((tree));
tree voidify_wrapper_expr		PARAMS ((tree));
tree gimple_build_eh_filter		PARAMS ((tree, tree, tree));
tree maybe_protect_cleanup		PARAMS ((tree));


/* Iterator object for GIMPLE statements.  */
typedef struct {
  tree *tp;
} gimple_stmt_iterator;

static inline gimple_stmt_iterator gsi_start PARAMS ((tree *));
static inline bool gsi_end_p		PARAMS ((gimple_stmt_iterator));
static inline void gsi_step		PARAMS ((gimple_stmt_iterator *));
static inline tree gsi_stmt		PARAMS ((gimple_stmt_iterator));
static inline tree *gsi_stmt_ptr	PARAMS ((gimple_stmt_iterator));
static inline tree *gsi_container	PARAMS ((gimple_stmt_iterator));

static inline gimple_stmt_iterator
gsi_start (tp)
     tree *tp;
{
  gimple_stmt_iterator i;
  i.tp = tp;
  return i;
}

static inline bool
gsi_end_p (i)
     gimple_stmt_iterator i;
{
  return (i.tp == NULL || *(i.tp) == error_mark_node);
}

static inline void
gsi_step (i)
     gimple_stmt_iterator *i;
{
  tree t = *(i->tp);
  STRIP_NOPS (t);
  if (TREE_CODE (t) == COMPOUND_EXPR)
    i->tp = &(TREE_OPERAND (t, 1));
  else
    i->tp = NULL;
}

static inline tree *
gsi_stmt_ptr (i)
     gimple_stmt_iterator i;
{
  tree t;

#if defined ENABLE_CHECKING
  if (i.tp == NULL || *i.tp == NULL_TREE)
    abort ();
#endif

  t = *(i.tp);
  STRIP_NOPS (t);

  if (TREE_CODE (t) == COMPOUND_EXPR)
    return &TREE_OPERAND (t, 0);
  else
    return i.tp;
}

static inline tree
gsi_stmt (i)
     gimple_stmt_iterator i;
{
  tree t = *(gsi_stmt_ptr (i));
  STRIP_NOPS (t);
  if (t == empty_stmt_node || t == error_mark_node)
    t = NULL_TREE;
  return t;
}

static inline tree *
gsi_container (i)
     gimple_stmt_iterator i;
{
  return i.tp;
}

#endif /* _TREE_SIMPLE_H  */
