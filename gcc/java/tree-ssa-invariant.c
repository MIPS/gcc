/* Java-specific "invariant propagation" pass.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "timevar.h"
#include "tree-dump.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-ssa-propagate.h"
#include "langhooks.h"
#include "target.h"
#include "java-tree.h"
#include "domwalk.h"

/* This pass implements a few simple gcj-specific optimizations
   related to handling of invariants.  In Java, certain fields of
   some objects are known to be invariant after the object has been
   created.  For instance, the vtable pointer of an object cannot
   change; neither can the length of an array.

   Also, this pass knows that 'new Type[n]' sets the length of an
   array to 'n'.

   Ideally GCC would recognize these cases and optimize for us.
   However, currently there is no way to express these properties to
   the optimizers.
 */

/* FIXME: lame data structure here.  But for now we only support
   vtable (index == 0) and length (vtable == 1) lookups.  */
#define MAX_INDEX 2
#define VTABLE_INDEX 0
#define LENGTH_INDEX 1

struct invariant_type_d
{
  /* If this record represents a copy of an SSA_NAME, this is the
     canonical name.  */
  tree canonical;
  /* The replacement lookups.  */
  tree replacements[MAX_INDEX];
};

typedef struct invariant_type_d invariant_type_t;

static void
register_copy (invariant_type_t *global_data, tree copy, tree orig)
{
  invariant_type_t *rec;
  gcc_assert (TREE_CODE (orig) == SSA_NAME);
  gcc_assert (TREE_CODE (copy) == SSA_NAME);
  rec = &global_data[SSA_NAME_VERSION (copy)];
  rec->canonical = orig;
}

static void
hoist_ref (invariant_type_t *global_data, tree object, tree var,
	   tree *def, int field_index)
{
  invariant_type_t *rec = &global_data[SSA_NAME_VERSION (object)];
  if (rec->canonical)
    {
      object = rec->canonical;
      rec = &global_data[SSA_NAME_VERSION (object)];
      gcc_assert (! rec->canonical);
    }

  if (rec->replacements[field_index])
    {
      /* Replace.  */
      *def = rec->replacements[field_index];
    }
  else
    {
      /* Insert a new record.  */
      rec->replacements[field_index] = var;
    }
}

static void
inv_walk_block (struct dom_walk_data *walk_data, basic_block bb,
		block_stmt_iterator iter)
{
  invariant_type_t *global_data = (invariant_type_t *) walk_data->global_data;
  tree indir, obj, field;
  tree stmt = bsi_stmt (iter);
  /* We're only interested in modifies.  */
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return;

  /* Keep track of copies of SSA names.  */
  if (TREE_CODE (TREE_OPERAND (stmt, 1)) == SSA_NAME)
    {
      register_copy (global_data, TREE_OPERAND (stmt, 0),
		     TREE_OPERAND (stmt, 1));
      return;
    }
  if (TREE_CODE (TREE_OPERAND (stmt, 1)) == NOP_EXPR
      || TREE_CODE (TREE_OPERAND (stmt, 1)) == CONVERT_EXPR)
    {
      register_copy (global_data, TREE_OPERAND (stmt, 0),
		     TREE_OPERAND (TREE_OPERAND (stmt, 1), 0));
      return;
    }

  /* Look for field references that we care about.
     FIXME: should also know to look for references *into* the vtable
     here.
     FIXME: should also know to look at CALL_EXPR to record the known
     length of new arrays.
  */
  if (TREE_CODE (TREE_OPERAND (stmt, 1)) != COMPONENT_REF)
    return;
  indir = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
  if (TREE_CODE (indir) != INDIRECT_REF)
    return;
  obj = TREE_OPERAND (indir, 0);
  field = TREE_OPERAND (TREE_OPERAND (stmt, 1), 1);
  if (POINTER_TYPE_P (TREE_TYPE (obj))
      && TYPE_ARRAY_P (TREE_TYPE (TREE_TYPE (obj)))
      && DECL_NAME (field) == length_identifier_node)
    hoist_ref (global_data, obj, TREE_OPERAND (stmt, 0),
	       &TREE_OPERAND (stmt, 1), LENGTH_INDEX);
  else if (DECL_NAME (field) == get_identifier ("vtable")
	   /* We don't keep the decl around anywhere for comparison,
	      and a use field could be named 'vtable', so we check the
	      offset as a hack.  */
	   && integer_zerop (DECL_FIELD_OFFSET (field)))
    hoist_ref (global_data, obj, TREE_OPERAND (stmt, 0),
	       &TREE_OPERAND (stmt, 1), VTABLE_INDEX);
}

static void
inv_clean_block (struct dom_walk_data *walk_data, basic_block bb)
{
  /* Remove all replacements which were defined in the given block.  */
  invariant_type_t *global_data = (invariant_type_t *) walk_data->global_data;
  unsigned int i;
  for (i = 0; i < num_ssa_names; ++i)
    {
      int j;
      for (j = 0; j < MAX_INDEX; ++j)
	{
	  tree rep = global_data[i].replacements[j];
	  if (rep)
	    {
	      tree stmt = SSA_NAME_DEF_STMT (rep);
	      stmt_ann_t ann = get_stmt_ann (stmt);
	      if (ann->bb == bb)
		global_data[i].replacements[j] = NULL_TREE;
	    }
	}
    }
}

static unsigned int
gcj_invariants (void)
{
  struct dom_walk_data walk_data;
  invariant_type_t *recs;

  recs = XNEWVEC (invariant_type_t, num_ssa_names);
  memset (recs, 0, num_ssa_names * sizeof (invariant_type_t));

  init_walk_dominator_tree (&walk_data);
  walk_data.walk_stmts_backward = false;
  walk_data.dom_direction = CDI_DOMINATORS;
  walk_data.initialize_block_local_data = NULL;
  walk_data.before_dom_children_before_stmts = NULL;
  walk_data.before_dom_children_walk_stmts = inv_walk_block;
  walk_data.before_dom_children_after_stmts = NULL;
  walk_data.after_dom_children_before_stmts = inv_clean_block;
  walk_data.after_dom_children_walk_stmts = NULL;
  walk_data.after_dom_children_after_stmts = NULL;
  walk_data.global_data = recs;
  walk_data.block_data_stack = NULL;	/* FIXME? */
  walk_data.block_local_data_size = 0;
  /* FIXME: domwalk should have a function to initialize a
     dom_walk_data object.  This would eliminate a lot of this NULL
     clutter as well.  Also this field is marked as dom-private, but I
     don't see how that can be useful.  */
  walk_data.interesting_blocks = NULL;

  walk_dominator_tree (&walk_data, ENTRY_BLOCK_PTR);
  fini_walk_dominator_tree (&walk_data);

  free (recs);
  return 0;
}

struct tree_opt_pass pass_gcj_invariant = 
{
  "gcjinv",				/* name */
  NULL,					/* gate */
  gcj_invariants,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_ssa,				/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func
    | TODO_verify_ssa
    | TODO_update_ssa
    | TODO_cleanup_cfg,			/* todo_flags_finish */
  0					/* letter */
};

void
init_gcj_invariant (void)
{
  /* FIXME: GCC doesn't have a way to register lang-specific passes.  */
/*  struct tree_opt_pass **p = &pass_all_optimizations.sub;
  while (*p != &pass_merge_phi)
    p = &((*p)->next);
  *p = &pass_gcj_invariant;
  pass_gcj_invariant.next = &pass_merge_phi;*/
}
