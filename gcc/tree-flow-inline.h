/* Inline functions for tree-flow.h
   Copyright (C) 2001 Free Software Foundation, Inc.
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

#ifndef _TREE_FLOW_INLINE_H
#define _TREE_FLOW_INLINE_H 1

/* Inline functions for manipulating various data structures defined in
   tree-flow.h.  See tree-flow.h for documentation.  */

static inline var_ann_t
var_ann (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (!SSA_VAR_P (t))
    abort ();
#endif
  /* SSA_NAME nodes share the same annotations as the VAR_DECL/INDIRECT_REF
     node that they wrap.  */
  if (TREE_CODE (t) == SSA_NAME)
    t = SSA_NAME_VAR (t);

  return (t->common.ann && t->common.ann->common.type == VAR_ANN)
	 ? (var_ann_t) t->common.ann
	 : NULL;
}

static inline stmt_ann_t
stmt_ann (t)
     tree t;
{
  return (t->common.ann && t->common.ann->common.type == STMT_ANN)
	 ? (stmt_ann_t) t->common.ann
	 : NULL;
}

static inline enum tree_ann_type
ann_type (ann)
     tree_ann ann;
{
  return ann->common.type;
}

static inline basic_block
bb_for_stmt (t)
     tree t;
{
  stmt_ann_t ann = stmt_ann (t);
  return ann ? ann->bb : NULL;
}

static inline varray_type
may_aliases (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_aliases : NULL;
}

static inline bool
may_alias_global_mem_p (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_alias_global_mem : false;
}

static inline void
set_indirect_ref (var, indirect)
     tree var;
     tree indirect;
{
  var_ann_t ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->indirect_ref = indirect;
}

static inline tree
indirect_ref (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->indirect_ref : NULL_TREE;
}

static inline int
get_lineno (expr)
     tree expr;
{
  if (expr == NULL_TREE)
    return -1;

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    expr = TREE_OPERAND (expr, 0);

  if (! TREE_LOCUS (expr))
    return -1;

  return TREE_LINENO (expr);
}

static inline const char *
get_filename (expr)
     tree expr;
{
  if (expr == NULL_TREE)
    return "???";

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    expr = TREE_OPERAND (expr, 0);

  if (TREE_LOCUS (expr) && TREE_FILENAME (expr))
    return TREE_FILENAME (expr);
  else
    return "???";
}

static inline void
modify_stmt (t)
     tree t;
{
  stmt_ann_t ann = stmt_ann (t);
  if (ann == NULL)
    ann = create_stmt_ann (t);
  ann->modified = 1;
}

static inline void
unmodify_stmt (t)
     tree t;
{
  stmt_ann_t ann = stmt_ann (t);
  if (ann == NULL)
    ann = create_stmt_ann (t);
  ann->modified = 0;
}

static inline bool
stmt_modified_p (t)
     tree t;
{
  stmt_ann_t ann = stmt_ann (t);

  /* Note that if the statement doesn't yet have an annotation, we consider it
     modified.  This will force the next call to get_stmt_operands to scan the
     statement.  */
  return ann ? ann->modified : true;
}

static inline tree
create_indirect_ref (ptr_sym)
     tree ptr_sym;
{
#if defined ENABLE_CHECKING
  if (!POINTER_TYPE_P (TREE_TYPE (ptr_sym)))
    abort ();
#endif
  return build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (ptr_sym)), ptr_sym);
}

static inline tree *
def_op (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->ops ? ann->ops->def_op : NULL) : NULL;
}

static inline varray_type
use_ops (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->ops ? ann->ops->use_ops : NULL) : NULL;
}


static inline varray_type
vdef_ops (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->vops ? ann->vops->vdef_ops : NULL) : NULL;
}

static inline varray_type
vuse_ops (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->vops ? ann->vops->vuse_ops : NULL) : NULL;
}

static inline varray_type
immediate_uses (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->df ? ann->df->immediate_uses : NULL) : NULL;
}

static inline varray_type
reaching_defs (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->df ? ann->df->reaching_defs : NULL) : NULL;
}

static inline bb_ann_t
bb_ann (bb)
     basic_block bb;
{
  return (bb_ann_t)bb->aux;
}

static inline basic_block
parent_block (bb)
     basic_block bb;
{
  return bb_ann (bb)->parent_block;
}

static inline void
set_parent_block (bb, parent)
     basic_block bb;
     basic_block parent;
{
  bb_ann (bb)->parent_block = parent;
}

static inline tree
phi_nodes (bb)
     basic_block bb;
{
  return bb_ann (bb)->phi_nodes;
}

static inline void
add_dom_child (bb, child_bb)
     basic_block bb;
     basic_block child_bb;
{
  bb_ann_t ann = bb_ann (bb);
  if (ann->dom_children == NULL)
    ann->dom_children = BITMAP_GGC_ALLOC ();
  bitmap_set_bit (ann->dom_children, child_bb->index);
}

static inline bitmap
dom_children (bb)
     basic_block bb;
{
  return bb_ann (bb)->dom_children;
}

/* Similar to gsi_step() but stops at basic block boundaries and ignores
   empty_stmt_nodes inside a basic block.  */
static inline void
gsi_step_in_bb (i, bb)
     gimple_stmt_iterator *i;
     basic_block bb;
{
  do
    gsi_step (i);
  while (i->tp && gsi_stmt (*i) == NULL_TREE);

  if (i->tp && bb_for_stmt (*(i->tp)) != bb)
    i->tp = NULL;
}


/* Similar to gsi_step() but stops at basic block boundaries. Assumes stmt
   has bb_for_stmt() set (can't be an empty_stmt_node).  */
static inline void
gsi_step_bb (i)
     gimple_stmt_iterator *i;
{
  basic_block bb = bb_for_stmt (*(i->tp));
  gsi_step_in_bb (i, bb);
}

static inline bool
gsi_end_bb_p (i)
gimple_stmt_iterator i;
{
  return (i.tp == NULL || gsi_stmt (i) == NULL_TREE);
}

static inline bool
is_exec_stmt (t)
     tree t;
{
  return (t && t != empty_stmt_node && t != error_mark_node);
}

#endif /* _TREE_FLOW_INLINE_H  */
