/* Inline functions for tree-flow.h
   Copyright (C) 2001 Free Software Foundation, Inc.
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

#ifndef _TREE_FLOW_INLINE_H
#define _TREE_FLOW_INLINE_H 1

/* Inline functions for manipulating various data structures defined in
   tree-flow.h.  See tree-flow.h for documentation.  */

static inline unsigned long
ref_id (ref)
     tree_ref ref;
{
  return ref->common.id;
}

static inline enum tree_ref_type
ref_type (ref)
     tree_ref ref;
{
  return ref->common.type;
}

static inline basic_block
ref_bb (ref)
     tree_ref ref;
{
  return ref->common.bb;
}

static inline tree
ref_stmt (ref)
     tree_ref ref;
{
  return ref->common.stmt_p ? *(ref->common.stmt_p) : NULL_TREE;
}

static inline tree
ref_var (ref)
     tree_ref ref;
{
  return ref->common.var;
}

static inline ref_list
imm_uses (def)
     tree_ref def;
{
  return def->vdef.imm_uses;
}

static inline ref_list
reached_uses (ref)
     tree_ref ref;
{
  return ref->vdef.reached_uses;
}

static inline tree_ref
imm_reaching_def (use)
     tree_ref use;
{
  return use->vref.imm_rdef;
}

static inline void 
set_imm_reaching_def (use, def)
     tree_ref use;
     tree_ref def;
{
  use->vref.imm_rdef = def;
}

static inline tree_ref
alias_imm_reaching_def (ref, i)
     tree_ref ref;
     size_t i;
{
#if defined ENABLE_CHECKING
  if (i > num_may_alias (ref_var (ref)))
    abort ();
#endif
  return ref->vref.alias_imm_rdefs[i];
}

static inline void
set_phi_arg_def (phi_arg, def)
     phi_node_arg phi_arg;
     tree_ref def;
{
  phi_arg->def = def;
}

static inline tree_ref
phi_arg_def (phi_arg)
     phi_node_arg phi_arg;
{
  return phi_arg->def;
}

static inline void
set_phi_arg_edge (phi_arg, e)
     phi_node_arg phi_arg;
     edge e;
{
  phi_arg->e = e;
}

static inline edge
phi_arg_edge (phi_arg)
     phi_node_arg phi_arg;
{
  return phi_arg->e;
}

static inline ref_list
reaching_defs (use)
     tree_ref use;
{
  return use->vuse.rdefs;
}

static inline varray_type
phi_args (phi)
     tree_ref phi;
{
  return phi->vphi.phi_args;
}

static inline unsigned int
num_phi_args (phi)
     tree_ref phi;
{
  return VARRAY_ACTIVE_SIZE (phi->vphi.phi_args);
}

static inline phi_node_arg
phi_arg (phi, i)
     tree_ref phi;
     unsigned int i;
{
  return (phi_node_arg)(VARRAY_GENERIC_PTR (phi->vphi.phi_args, i));
}

static inline void
set_phi_arg (phi, i, arg)
     tree_ref phi;
     unsigned int i;
     phi_node_arg arg;
{
  VARRAY_GENERIC_PTR (phi->vphi.phi_args, i) = (PTR)arg;
}

static inline tree_ann
tree_annotation (t)
     tree t;
{
  STRIP_WFL (t);
  STRIP_NOPS (t);
  return t->common.ann;
}

static inline basic_block
bb_for_stmt (t)
     tree t;
{
  return tree_annotation (t) ? tree_annotation (t)->bb : NULL;
}


static inline void
set_bb_for_stmt (t, bb)
     tree t;
     basic_block bb;
{
  tree_ann ann;

  if (t == empty_stmt_node)
    return;

  do
    {
      ann = tree_annotation (t) ? tree_annotation (t) : create_tree_ann (t);
      ann->bb = bb;
      if (TREE_CODE (t) == COMPOUND_EXPR)
	t = TREE_OPERAND (t, 0);
      else if (TREE_CODE (t) == EXPR_WITH_FILE_LOCATION)
	t = EXPR_WFL_NODE (t);
      else
	t = NULL;
    }
  while (t);
}

static inline tree_ref
currdef_for (decl)
     tree decl;
{
  return tree_annotation (decl) ? tree_annotation (decl)->currdef : NULL;
}

static inline void
set_currdef_for (v, def)
     tree v;
     tree_ref def;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (v)) != 'd'
      && TREE_CODE (v) != INDIRECT_REF)
    abort ();

  if (def && ref_type (def) != V_DEF && ref_type (def) != V_PHI)
    abort ();
#endif
  ann = tree_annotation (v) ? tree_annotation (v) : create_tree_ann (v);
  ann->currdef = def;
}

static inline ref_list
tree_refs (t)
     tree t;
{
  return tree_annotation (t) ? tree_annotation (t)->refs : NULL;
}

static inline void
add_tree_ref (t, ref)
     tree t;
     tree_ref ref;
{
  tree_ann ann;
  ann = tree_annotation (t) ? tree_annotation (t) : create_tree_ann (t);
  if (ann->refs == NULL)
    ann->refs = create_ref_list ();
  add_ref_to_list_end (ann->refs, ref);
}

static inline void
remove_tree_ref (t, ref)
     tree t;
     tree_ref ref;
{
  remove_ref_from_list (tree_refs (t), ref);
}

static inline tree
may_alias (var, i)
     tree var;
     size_t i;
{
  return VARRAY_TREE (tree_annotation (var)->may_aliases, i);
}

static inline size_t
num_may_alias (var)
     tree var;
{
  tree_ann ann = tree_annotation (var);
  return (ann->may_aliases) ? VARRAY_ACTIVE_SIZE (ann->may_aliases) : 0;
}

static inline int
get_lineno (expr)
     tree expr;
{
  if (expr == NULL_TREE)
    return -1;

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    expr = TREE_OPERAND (expr, 0);

  if (TREE_CODE (expr) == EXPR_WITH_FILE_LOCATION)
    return EXPR_WFL_LINENO (expr);
  else
    return -1;
}

static inline const char *
get_filename (expr)
     tree expr;
{
  if (expr == NULL_TREE)
    return "???";

  if (TREE_CODE (expr) == COMPOUND_EXPR)
    expr = TREE_OPERAND (expr, 0);

  if (TREE_CODE (expr) == EXPR_WITH_FILE_LOCATION)
    return EXPR_WFL_FILENAME (expr);
  else
    return "???";
}

static inline void
set_tree_flag (t, flag)
     tree t;
     enum tree_flags flag;
{
  tree_ann  ann;
  ann = tree_annotation (t) ? tree_annotation (t) : create_tree_ann (t);
  ann->flags |= flag;
}

static inline void
clear_tree_flag (t, flag)
     tree t;
     enum tree_flags flag;
{
  tree_ann ann;
  ann = tree_annotation (t) ? tree_annotation (t) : create_tree_ann (t);
  ann->flags &= ~flag;
}

static inline void
reset_tree_flags (t)
     tree t;
{
  tree_ann ann;
  ann = tree_annotation (t) ? tree_annotation (t) : create_tree_ann (t);
  ann->flags = 0;
}

static inline enum tree_flags
tree_flags (t)
     tree t;
{
  return tree_annotation (t) ? tree_annotation (t)->flags : 0;
}

static inline tree
indirect_var (ptr)
     tree ptr;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (ptr)) != 'd'
      || !POINTER_TYPE_P (TREE_TYPE (ptr)))
    abort ();
#endif
  return tree_annotation (ptr)->indirect_var;
}

static inline void
set_indirect_var (ptr, indirect)
     tree ptr;
     tree indirect;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (ptr)) != 'd'
      || !POINTER_TYPE_P (TREE_TYPE (ptr))
      || TREE_CODE (indirect) != INDIRECT_REF)
    abort ();
#endif
  ann = tree_annotation (ptr);
  ann->indirect_var = indirect;
}

static inline bb_ann
bb_annotation (b)
     basic_block b;
{
  return (bb_ann)b->aux;
}

static inline basic_block
parent_block (b)
     basic_block b;
{
  return bb_annotation (b)->parent_block;
}

static inline void
set_parent_block (b, parent)
     basic_block b;
     basic_block parent;
{
  bb_ann ann = bb_annotation (b);
  ann->parent_block = parent;
}

static inline ref_list
bb_refs (b)
     basic_block b;
{
  return bb_annotation (b)->refs;
}

static inline void
remove_bb_ref (b, ref)
     basic_block b;
     tree_ref ref;
{
  remove_ref_from_list (bb_annotation (b)->refs, ref);
}

static inline void 
set_exprref_class (ref, class)
     tree_ref ref;
     unsigned int class;
{
  ref->ecommon.class = class;
}

static inline unsigned int 
exprref_class (ref)
     tree_ref ref;
{
  return ref->ecommon.class;
}

static inline void 
set_exprref_inserted (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ecommon.inserted = flag;
}

static inline bool
exprref_inserted (ref)
     tree_ref ref;
{
  return ref->ecommon.inserted;
}

static inline void 
set_exprref_save (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ecommon.save = flag;
}

static inline bool
exprref_save (ref)
     tree_ref ref;
{
  return ref->ecommon.save;
}

static inline void 
set_exprref_reload (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ecommon.reload = flag;
}

static inline bool
exprref_reload (ref)
     tree_ref ref;
{
  return ref->ecommon.reload;
}

static inline void 
set_exprref_processed (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ecommon.processed = flag;
}

static inline bool
exprref_processed (ref)
     tree_ref ref;
{
  return ref->ecommon.processed;
}

static inline void 
set_exprref_processed2 (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ecommon.processed2  = flag;
}

static inline bool
exprref_processed2 (ref)
     tree_ref ref;
{
  return ref->ecommon.processed2;
}

static inline ref_list
exprref_uses (ref)
     tree_ref ref;
{
  return ref->ecommon.uses;
}
static inline void
set_exprref_uses (ref, list)
     tree_ref ref;
     ref_list list;
{
  ref->ecommon.uses = list;
}

static inline void
set_expruse_def (ref, def)
     tree_ref ref;
     tree_ref def;
{
  if (def)
    {
      if (!exprref_uses (def))
	set_exprref_uses (def, create_ref_list ());
      
      if (expruse_def (ref))
	remove_ref_from_list (exprref_uses (expruse_def (ref)), ref);
            
      add_ref_to_list_end (exprref_uses (def), ref);
    }
  ref->euse.def = def;
}

static inline tree_ref
expruse_def (ref)
     tree_ref ref;
{
  return ref->euse.def;
}

static inline void 
set_expruse_phiop (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->euse.op_occurrence = flag;
}

static inline bool
expruse_phiop (ref)
     tree_ref ref;
{
  return ref->euse.op_occurrence;
}

static inline void
set_expruse_phi (ref, phi)
     tree_ref ref;
     tree_ref phi;
{
  ref->euse.phi = phi;
}

static inline tree_ref
expruse_phi (ref)
     tree_ref ref;
{
  return ref->euse.phi;
}

static inline void 
set_expruse_has_real_use (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->euse.has_real_use = flag;
}

static inline bool
expruse_has_real_use (ref)
     tree_ref ref;
{
  return ref->euse.has_real_use;
}

static inline void
set_exprphi_phi_args (ref, args)
     tree_ref ref;
     varray_type args;
{
  ref->ephi.phi_args = args;
}

static inline varray_type
exprphi_phi_args (ref)
     tree_ref ref;
{
  return ref->ephi.phi_args;
}

static inline unsigned int
num_ephi_args (phi)
     tree_ref phi;
{
  return VARRAY_ACTIVE_SIZE (phi->ephi.phi_args);
}

static inline phi_node_arg
ephi_arg (phi, i)
     tree_ref phi;
     unsigned int i;
{
  return (phi_node_arg)(VARRAY_GENERIC_PTR (phi->ephi.phi_args, i));
}

static inline void
set_ephi_arg (phi, i, arg)
     tree_ref phi;
     unsigned int i;
     phi_node_arg arg;
{
  VARRAY_GENERIC_PTR (phi->ephi.phi_args, i) = (PTR)arg;
}

static inline void
set_exprphi_downsafe (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ephi.downsafe = flag;
}

static inline bool
exprphi_downsafe (ref)
     tree_ref ref;
{
  return ref->ephi.downsafe;
}

static inline void
set_exprphi_canbeavail (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ephi.can_be_avail = flag;
}

static inline bool
exprphi_canbeavail (ref)
     tree_ref ref;
{
  return ref->ephi.can_be_avail;
}

static inline void
set_exprphi_later (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ephi.later = flag;
}

static inline bool
exprphi_later (ref)
     tree_ref ref;
{
  return ref->ephi.later;
}

static inline void
set_exprphi_extraneous (ref, flag)
     tree_ref ref;
     unsigned int flag;
{
  ref->ephi.extraneous = flag;
}

static inline bool
exprphi_extraneous (ref)
     tree_ref ref;
{
  return ref->ephi.extraneous;
}

static inline bool
exprphi_willbeavail (ref)
     tree_ref ref;
{
  return exprphi_canbeavail (ref) && !exprphi_later (ref);
}

/* Similar to gsi_step() but stops at basic block boundaries.  */
static inline void
gsi_step_bb (i)
     gimple_stmt_iterator *i;
{
  basic_block bb = bb_for_stmt (*(i->tp));
  if (bb && i->tp != bb->end_tree_p)
    gsi_step (i);
  else
    i->tp = NULL;
}

/* Similar to gsi_start() but initializes the iterator at the first
   statement in basic block BB.  */
static inline gimple_stmt_iterator
gsi_start_bb (bb)
     basic_block bb;
{
  if (bb && bb->index != INVALID_BLOCK)
    {
      tree *tp = bb->head_tree_p;
      return gsi_start (tp);
    }
  else
    {
      gimple_stmt_iterator i;
      i.tp = NULL;
      return i;
    }
}

static inline bool
is_exec_stmt (t)
     tree t;
{
  return (t && t != empty_stmt_node && t != error_mark_node);
}

static inline bool
is_assignment_stmt (t)
     tree t;
{
  STRIP_WFL (t);
  STRIP_NOPS (t);
  return (TREE_CODE (t) == MODIFY_EXPR || TREE_CODE (t) == INIT_EXPR);
}

static inline bool
is_may_ref (ref)
     tree_ref ref;
{
  return ref->vref.m_may;
}

static inline bool
is_may_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && is_may_ref (ref);
}

static inline bool
is_may_use (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_USE && is_may_ref (ref);
}

static inline bool
is_partial_ref (ref)
     tree_ref ref;
{
  return ref->vref.m_partial;
}

static inline bool
is_partial_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && is_partial_ref (ref);
}

static inline bool
is_partial_use (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_USE && is_partial_ref (ref);
}

static inline bool
is_volatile_ref (ref)
     tree_ref ref;
{
  return ref->vref.m_volatile;
}

static inline bool
is_volatile_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && is_volatile_ref (ref);
}

static inline bool
is_volatile_use (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_USE && is_volatile_ref (ref);
}

static inline bool
is_default_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && ref->vdef.m_default;
}

static inline bool
is_clobbering_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && ref->vdef.m_clobber;
}

static inline bool
is_initializing_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && ref->vdef.m_initial;
}

static inline bool
is_relocating_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF && ref->vdef.m_relocate;
}

static inline bool
is_addressof_use (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_USE && ref->vuse.m_addressof;
}

static inline bool
is_pure_use (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_USE
	 && !ref->vuse.m_addressof
         && !is_may_ref (ref)
	 && !is_partial_ref (ref)
	 && !is_volatile_ref (ref);
}

static inline bool
is_pure_def (ref)
     tree_ref ref;
{
  return ref_type (ref) == V_DEF
	 && !ref->vdef.m_default
	 && !ref->vdef.m_clobber
	 && !ref->vdef.m_initial
	 && !ref->vdef.m_relocate
         && !is_may_ref (ref)
	 && !is_partial_ref (ref)
	 && !is_volatile_ref (ref);
}

/* Return TRUE if we reached the end of the list with iterator I.  */
static inline bool
rli_after_end (i)
     ref_list_iterator i;
{
  return i.node == NULL;
}

/* Move iterator I to the next element in the list.  */
static inline void
rli_step (i)
     ref_list_iterator *i;
{
  i->node = i->node->next;
}

/* Move iterator I to the previous element in the list.  */
static inline void
rli_step_rev (i)
     ref_list_iterator *i;
{
  i->node = i->node->prev;
}

/* Return the reference at the current position of iterator I.  */
static inline tree_ref
rli_ref (i)
     ref_list_iterator i;
{
  return i.node->ref;
}

static inline tree_ref
get_last_ref (list)
     ref_list list;
{
  return list->last->ref;
}

static inline tree_ref
get_first_ref (list)
     ref_list list;
{
  return list->first->ref;
}

static inline bool
ref_list_is_empty (list)
     ref_list list;
{
  return (list == NULL
          || list->first == NULL
	  || list->last == NULL
	  /* rli_delete cannot remove list->first nor list->last.  */
	  || (list->first->ref == NULL && list->last->ref == NULL));
}

#endif /* _TREE_FLOW_INLINE_H  */
