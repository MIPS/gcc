/* Inline functions for tree-flow.h
   Copyright (C) 2001, 2003 Free Software Foundation, Inc.
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
var_ann (tree t)
{
#if defined ENABLE_CHECKING
  if (t == NULL_TREE
      || !DECL_P (t)
      || (t->common.ann
	  && t->common.ann->common.type != VAR_ANN))
    abort ();
#endif

  return (var_ann_t) t->common.ann;
}

static inline var_ann_t
get_var_ann (tree var)
{
  var_ann_t ann = var_ann (var);
  return (ann) ? ann : create_var_ann (var);
}

static inline stmt_ann_t
stmt_ann (tree t)
{
#if defined ENABLE_CHECKING
  if (!is_gimple_stmt (t) && !is_essa_node (t))
    abort ();
#endif

  return (stmt_ann_t) t->common.ann;
}

static inline stmt_ann_t
get_stmt_ann (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return (ann) ? ann : create_stmt_ann (stmt);
}

static inline enum tree_ann_type
ann_type (tree_ann ann)
{
  return ann->common.type;
}

static inline basic_block
bb_for_stmt (tree t)
{
  stmt_ann_t ann = stmt_ann (t);
  return ann ? ann->bb : NULL;
}

static inline varray_type
may_aliases (tree var)
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_aliases : NULL;
}

static inline void
set_may_alias_global_mem (tree var)
{
  var_ann_t ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->may_alias_global_mem = 1;
}

static inline bool
may_alias_global_mem_p (tree var)
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_alias_global_mem : false;
}

static inline void
set_may_point_to_global_mem (tree var)
{
  var_ann_t ann;

#if defined ENABLE_CHECKING
  if (!POINTER_TYPE_P (TREE_TYPE (var)))
    abort ();
#endif

  ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->may_point_to_global_mem = 1;
}

static inline bool
may_point_to_global_mem_p (tree var)
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_point_to_global_mem : false;
}

static inline bool
has_hidden_use (tree var)
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->has_hidden_use : false;
}

static inline void
set_has_hidden_use (tree var)
{
  var_ann_t ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->has_hidden_use = 1;
}

static inline int
get_lineno (tree expr)
{
  if (expr == NULL_TREE)
    return -1;

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    expr = TREE_OPERAND (expr, 0);

  if (! EXPR_LOCUS (expr))
    return -1;

  return EXPR_LINENO (expr);
}

static inline const char *
get_filename (tree expr)
{
  if (expr == NULL_TREE)
    return "???";

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    expr = TREE_OPERAND (expr, 0);

  if (EXPR_LOCUS (expr) && EXPR_FILENAME (expr))
    return EXPR_FILENAME (expr);
  else
    return "???";
}

static inline void
modify_stmt (tree t)
{
  stmt_ann_t ann = stmt_ann (t);
  if (ann == NULL)
    ann = create_stmt_ann (t);
  ann->modified = 1;
}

static inline void
unmodify_stmt (tree t)
{
  stmt_ann_t ann = stmt_ann (t);
  if (ann == NULL)
    ann = create_stmt_ann (t);
  ann->modified = 0;
}

static inline bool
stmt_modified_p (tree t)
{
  stmt_ann_t ann = stmt_ann (t);

  /* Note that if the statement doesn't yet have an annotation, we consider it
     modified.  This will force the next call to get_stmt_operands to scan the
     statement.  */
  return ann ? ann->modified : true;
}

static inline varray_type
def_ops (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->ops ? ann->ops->def_ops : NULL) : NULL;
}

static inline varray_type
use_ops (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->ops ? ann->ops->use_ops : NULL) : NULL;
}


static inline varray_type
vdef_ops (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->vops ? ann->vops->vdef_ops : NULL) : NULL;
}

static inline varray_type
vuse_ops (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? (ann->vops ? ann->vops->vuse_ops : NULL) : NULL;
}

static inline varray_type
addresses_taken (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? ann->addresses_taken : NULL;
}

static dataflow_t
get_immediate_uses (tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);
  return ann ? ann->df : NULL;
}

static inline int
num_immediate_uses (dataflow_t df)
{
  varray_type imm;

  if (!df)
    return 0;

  imm = df->immediate_uses;
  if (!imm)
    return df->uses[1] ? 2 : 1;

  return VARRAY_ACTIVE_SIZE (imm) + 2;
}

static inline tree
immediate_use (dataflow_t df, int num)
{
#ifdef ENABLE_CHECKING
  if (num >= num_immediate_uses (df))
    abort ();
#endif
  if (num < 2)
    return df->uses[num];
  return VARRAY_TREE (df->immediate_uses, num - 2);
}

static inline bb_ann_t
bb_ann (basic_block bb)
{
  return (bb_ann_t)bb->tree_annotations;
}

static inline tree
phi_nodes (basic_block bb)
{
  return bb_ann (bb)->phi_nodes;
}


/* Return the phi index number for an edge.  */
static inline int
phi_arg_from_edge (tree phi, edge e)
{
  int i;
#if defined ENABLE_CHECKING
  if (!phi || TREE_CODE (phi) != PHI_NODE)
    abort();
#endif

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    if (PHI_ARG_EDGE (phi, i) == e)
      return i;

  return -1;
}


/* Return the phi argument number for an edge.  */
static inline struct phi_arg_d *
phi_element_for_edge (tree phi, edge e)
{
  int i;

  i = phi_arg_from_edge (phi, e);
  if (i != -1)
    return &(PHI_ARG_ELT (phi, i));
  else
    return (struct phi_arg_d *)NULL;
}

static inline void
add_dom_child (basic_block bb, basic_block child_bb)
{
  bb_ann_t ann = bb_ann (bb);
  if (ann->dom_children == NULL)
    ann->dom_children = BITMAP_GGC_ALLOC ();
  bitmap_set_bit (ann->dom_children, child_bb->index);
}

static inline void
remove_dom_child (basic_block bb, basic_block child_bb)
{
  bb_ann_t ann = bb_ann (bb);

#if defined ENABLE_CHECKING
  if (ann->dom_children == NULL)
    abort ();
#endif

  bitmap_clear_bit (ann->dom_children, child_bb->index);
}

static inline void
clear_dom_children (basic_block bb)
{
  bb_ann (bb)->dom_children = NULL;
}

static inline bitmap
dom_children (basic_block bb)
{
  return bb_ann (bb)->dom_children;
}

/*  -----------------------------------------------------------------------  */

static inline struct block_tree *
bti_start ()
{
  return block_tree;
}

static inline bool
bti_end_p (struct block_tree *block)
{
  return !block;
}

static inline void
bti_next (struct block_tree **block)
{
  struct block_tree *act = *block;

  if (act->subtree)
    {
      *block = act->subtree;
      return;
    }

  while (act->outer && !act->next)
    act = act->outer;

  *block = act->next;
}

static inline bool
bsi_end_p (block_stmt_iterator i)
{
  return i.curr_stmt == NULL;
}

/* Similar to tsi_next() but stops at basic block boundaries.  */

static inline void
bsi_next (block_stmt_iterator *i)
{
  i->curr_stmt = i->curr_stmt->next;
}

static inline tree
bsi_stmt (block_stmt_iterator i)
{
  return i.curr_stmt->stmt;
}

static inline tree_cell
bsi_cell (block_stmt_iterator i)
{
  return i.curr_stmt;
}

static inline tree *
bsi_stmt_ptr (block_stmt_iterator i)
{
  return &i.curr_stmt->stmt;
}

/* Return a tree_stmt_iterator for the stmt a block iterator refers to.  */

static inline tree_stmt_iterator
tsi_from_bsi (block_stmt_iterator bi)
{
  return tsi_start (&bi.curr_stmt->stmt);
}

static inline bool
is_exec_stmt (tree t)
{
  return (t && !IS_EMPTY_STMT (t) && t != error_mark_node);
}


/* Return true if this stmt can be the target of a control transfer stmt such
   as a goto.  */
static inline bool
is_label_stmt (tree t)
{
  if (t)
    switch (TREE_CODE (t))
      {
	case LABEL_DECL:
	case LABEL_EXPR:
	case CASE_LABEL_EXPR:
	  return true;
	default:
	  return false;
      }
  return false;
}

/* Routines to allow a block to be walked backwards reasonably efficiently.
   Once a decent implementation of bsi_prev() is implemented, this can
   be removed.  */

#define BSI_NUM_ELEMENTS	50

typedef struct bsi_list_d {
  block_stmt_iterator bsi[BSI_NUM_ELEMENTS];
  int curr_index;
  struct bsi_list_d *next;
} *bsi_list_p;


static inline bsi_list_p new_bsi_list (void);
static inline int empty_bsi_stack (bsi_list_p);
extern void push_bsi (bsi_list_p *, block_stmt_iterator);
extern block_stmt_iterator pop_bsi (bsi_list_p *);


/* Allocate a bsi_list structure.  */
static inline bsi_list_p
new_bsi_list (void)
{
  bsi_list_p b;
  b = (bsi_list_p) xmalloc (sizeof (struct bsi_list_d));
  b->curr_index = 0;
  b->next = NULL;

  return b;
}

/* Is the iterator stack empty?  */
static inline int
empty_bsi_stack (bsi_list_p list)
{
  if (!list || (list->curr_index < 0 && list->next == NULL))
    return 1;
  return 0;
}

/* This macro can be used if all that is ever examined is the stmt nodes
   of bsi.  */
#define FOR_EACH_STMT_IN_REVERSE(bsi, bb, stmt)			\
  for (bsi = bsi_last (bb);					\
       !bsi_end_p (bsi) && ((stmt = bsi_stmt (bsi)) || 1);	\
       bsi_prev (&bsi))

static inline bool
is_unchanging_value (tree val)
{
  return ((TREE_CONSTANT (val) || really_constant_p (val))
	  && is_gimple_val (val));
}

static inline bool
may_propagate_copy (tree dest, tree orig)
{
  /* FIXME.  GIMPLE is allowing pointer assignments and comparisons of
     pointers that have different alias sets.  This means that these
     pointers will have different memory tags associated to them.
     
     If we allow copy propagation in these cases, statements de-referencing
     the new pointer will now have a reference to a different memory tag
     with potentially incorrect SSA information.

     This was showing up in libjava/java/util/zip/ZipFile.java with code
     like:

     	struct java.io.BufferedInputStream *T.660;
	struct java.io.BufferedInputStream *T.647;
	struct java.io.InputStream *is;
	struct java.io.InputStream *is.662;
	[ ... ]
	T.660 = T.647;
	is = T.660;	<-- This ought to be type-casted
	is.662 = is;

     Also, f/name.c exposed a similar problem with a COND_EXPR predicate
     that was causing DOM to generate and equivalence with two pointers of
     alias-incompatible types:

     	struct _ffename_space *n;
	struct _ffename *ns;
	[ ... ]
	if (n == ns)
	  goto lab;
	...
	lab:
	return n;

     I think that GIMPLE should emit the appropriate type-casts.  For the
     time being, blocking copy-propagation in these cases is the safe thing
     to do.  */
  if (TREE_CODE (dest) == SSA_NAME
      && TREE_CODE (orig) == SSA_NAME
      && POINTER_TYPE_P (TREE_TYPE (dest))
      && POINTER_TYPE_P (TREE_TYPE (orig)))
    {
      tree mt_dest = var_ann (SSA_NAME_VAR (dest))->mem_tag;
      tree mt_orig = var_ann (SSA_NAME_VAR (orig))->mem_tag;
      if (mt_dest && mt_orig && mt_dest != mt_orig)
	return false;
    }

  return (!SSA_NAME_OCCURS_IN_ABNORMAL_PHI (dest)
	  && (TREE_CODE (orig) != SSA_NAME
	      || !SSA_NAME_OCCURS_IN_ABNORMAL_PHI (orig))
	  && !DECL_HARD_REGISTER (SSA_NAME_VAR (dest)));
}

static inline void
set_default_def (tree var, tree def)
{
  var_ann_t ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->default_def = def;
}

static inline tree
default_def (tree var)
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->default_def : NULL_TREE;
}

#endif /* _TREE_FLOW_INLINE_H  */
