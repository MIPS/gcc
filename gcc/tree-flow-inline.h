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

static inline tree
tree_stmt (t)
     tree t;
{
  return t->common.ann->common.stmt;
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

static inline void
set_may_alias_global_mem (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->may_alias_global_mem = 1;
}

static inline bool
may_alias_global_mem_p (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_alias_global_mem : false;
}

static inline void
set_may_point_to_global_mem (var)
     tree var;
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
may_point_to_global_mem_p (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->may_point_to_global_mem : false;
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

static inline bool
has_hidden_use (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  return ann ? ann->has_hidden_use : false;
}

static inline void
set_has_hidden_use (var)
     tree var;
{
  var_ann_t ann = var_ann (var);
  if (ann == NULL)
    ann = create_var_ann (var);
  ann->has_hidden_use = 1;
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
  tree parent = (bb->head_tree_p) ? parent_stmt (*bb->head_tree_p) : NULL_TREE;
  return parent ? bb_for_stmt (parent) : NULL;
}

static inline tree
parent_stmt (stmt)
     tree stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);
  return (ann) ? ann->parent_stmt : NULL_TREE;
}

static inline tree
phi_nodes (bb)
     basic_block bb;
{
  return bb_ann (bb)->phi_nodes;
}


/* Return the phi index number for an edge.  */
static inline int
phi_arg_from_edge (phi, e)
     tree phi;
     edge e;
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
phi_element_for_edge (phi, e)
     tree phi;
     edge e;
{
  int i;

  i = phi_arg_from_edge (phi, e);
  if (i != -1)
    return &(PHI_ARG_ELT (phi, i));
  else
    return (struct phi_arg_d *)NULL;
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

/*  -----------------------------------------------------------------------  */

static inline bool
bsi_end_p (i)
block_stmt_iterator i;
{
  return (i.tp == NULL || bsi_stmt (i) == NULL_TREE);
}

/* Similar to tsi_next() but stops at basic block boundaries. Assumes stmt
   has bb_for_stmt() set (can't be an empty_stmt_node).  */
static inline void
bsi_next (i)
     block_stmt_iterator *i;
{
  extern void bsi_next_in_bb (block_stmt_iterator *, basic_block);

  basic_block bb = bb_for_stmt (*(i->tp));
  bsi_next_in_bb (i, bb);
}

static inline tree *
bsi_stmt_ptr (i)
     block_stmt_iterator i;
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
bsi_stmt (i)
     block_stmt_iterator i;
{
  tree t = *(bsi_stmt_ptr (i));
  STRIP_NOPS (t);
  if (t == empty_stmt_node || t == error_mark_node)
    t = NULL_TREE;
  return t;
}

static inline tree *
bsi_container (i)
     block_stmt_iterator i;
{
  return i.tp;
}

/* Return a tree_stmt_iterator for the stmt a block iterator refers to.  */

static inline tree_stmt_iterator
tsi_from_bsi (bi)
     block_stmt_iterator bi;
{
  return tsi_start (bi.tp);
}

static inline bool
is_exec_stmt (t)
     tree t;
{
  return (t && t != empty_stmt_node && t != error_mark_node);
}


/* Return true if this stmt can be the target of a control transfer stmt such
   as a goto.  */
static inline bool
is_label_stmt (t)
     tree t;
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


static inline bsi_list_p new_bsi_list	PARAMS ((void));
static inline int empty_bsi_stack	PARAMS ((bsi_list_p));
extern void push_bsi			PARAMS ((bsi_list_p *, 
						 block_stmt_iterator));
extern block_stmt_iterator pop_bsi	PARAMS ((bsi_list_p *));


/* Allocate a bsi_list structure.  */
static inline bsi_list_p
new_bsi_list ()
{
  bsi_list_p b;
  b = (bsi_list_p) xmalloc (sizeof (struct bsi_list_d));
  b->curr_index = 0;
  b->next = NULL;

  return b;
}

/* Is the iterator stack empty?  */
static inline int
empty_bsi_stack (list)
     bsi_list_p list;
{
  if (!list || (list->curr_index < 0 && list->next == NULL))
    return 1;
  return 0;
}


/* Process an entire block of bsi's in reverse by pushing them on a stack
   as they are encountered, and then popping them off as they are needed.  
   There are a couple of odd things. Since the last loop is a for loop, 
   a dummy entry is pushed on the beginning of the stack, this allows the first
   item pushed on the stack to be processed in the final for loop, as well
   as guaranteeing there will be at least one to pop off.  

   usage:
     bsi_list_p  stack;
     block_stmt_iterator bsi;
     basic_block bb;

     FOR_EACH_BSI_IN_REVERSE (stack, bb, bsi)
       {
         ...
       }
*/
#define FOR_EACH_BSI_IN_REVERSE(bsi_stack, bb, bsi)		\
  bsi_stack = new_bsi_list ();					\
  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))	\
    push_bsi (&bsi_stack, bsi);					\
  bsi = pop_bsi (&bsi_stack);					\
  for ( ; !empty_bsi_stack (bsi_stack); bsi = pop_bsi (&bsi_stack))



/* This macro can be used if all that is ever examined is the stmt nodes
   of bsi. Ie, if the usage is
      FOR_EACH_BSI_IN_REVERSE (stack, bb, bsi) 
        {
	  tree stmt = bsi_stmt (bsi);
	  ...
  Then less overhead exists to simply use this macro.  

  usage:
    varray_type stmt_stack;
    basic_block bb;
    tree stmt;

    FOR_EACH_STMT_IN_REVERSE (stmt_stack, bb, stmt)
      {
        ...
      }
*/
#define FOR_EACH_STMT_IN_REVERSE(stmt_stack, bb, stmt)		\
  VARRAY_TREE_INIT (stmt_stack, 50, "stmt_stack");		\
  VARRAY_PUSH_TREE (stmt_stack, NULL_TREE);			\
  {								\
    block_stmt_iterator bsi;					\
    for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))	\
      VARRAY_PUSH_TREE (stmt_stack, bsi_stmt (bsi));		\
  }								\
  stmt = VARRAY_TOP_TREE (stmt_stack);				\
  VARRAY_POP (stmt_stack);					\
  for ( ; VARRAY_ACTIVE_SIZE (stmt_stack) > 0 ; 		\
	      stmt = VARRAY_TOP_TREE (stmt_stack), VARRAY_POP (stmt_stack))	

#endif /* _TREE_FLOW_INLINE_H  */
