/* SSA form updating utilities.
   Copyright (C) 2004 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "langhooks.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "tree-gimple.h"
#include "tree-inline.h"
#include "varray.h"
#include "timevar.h"
#include "tree-dump.h"

/* This file implements utilities for updating SSA form.  The core is the
   function

   update_ssa_form (values, flags);

   This function creates a ssa form for values in list VALUES.  The description
   for each value contains the decl for the base of the ssa names that need to
   be created, list containing the positions where the value is defined and
   list containing the positions where the value is used.  In case you
   performed simple code duplication, or called "rewrite_new_def" to create
   new ssa name for the code you emitted yourself, you may obtain the lists to
   feed to this function from "get_values_for_ssa_form_update" function.

   WARNING -- be sure you do not modify/remove statements in such a way that
   would cause def operands to get invalidated if you use this mechanism.
   If you create the list of defs and uses yourself, ensure that the def
   and use operand pointers are valid (for example resizing a phi node that
   may occur during edge redirection invalidates def operand pointers).

   The most common case for using this function is when a portion of the
   code is duplicated.  Although new ssa names are created for duplicated
   definitions, you should think about them as "aliases" for the original
   definitions when arguing correctness of such transformation -- the code
   for that you call update_ssa_form should be valid in case you replace
   these aliases by the original ssa name (and forget that the program should
   be in ssa form).  For example suppose you have the following piece of code:

   L1:
     goto L2;

   L2:
     a_1 = ...
   L3:
     use (a_1)

   By duplicating of the definition of a_1 you obtain

   L1:
     a_2{eqto a_1} = ...	;; this notation is used for the variables
				;; created in code duplication (or via
				;; "rewrite_new_def") until update_ssa_form
				;; is called on them (or alternatively until
				;; you call "ssa_form_updated" on the variable
				;; to inform that you performed ssa form
				;; updating yourself.
     goto L3;

   L2:
     a_1 = ...

   L3:
     use (a_1);

   Observe that the code still has the original semantics if you replace
   a_2 by a_1.  Usually this should mean that all values in the uses list
   are the original ssa name (although this is not required, and we do not
   look at the original contents of uses at all, so even NULL pointers are
   fine).

   Call to update_ssa_form transforms this code to valid ssa form:

   L1:
     a_2 = ...
     goto L3;

   L2:
     a_1 = ...

   L3:
     a_3 = phi (a_1, a_2);
     use (a_3);

   The function creates new ssa names only for newly created phi nodes.  In
   particular it expects each def in the list to be a separate ssa name based
   on VAR.  The information associated with newly created ssa names in phi
   nodes is obtained by merging the information at the arguments of the phi
   node.  In some cases you may use some information from the optimization that
   caused violation of the ssa form to improve this information; if this is the
   case, you must create the phi nodes with the improved information yourself
   (the update_ssa_form function may still be useful for you to perform the
   rewriting of uses by the reaching defs, or even creating the phi nodes for
   that you have no special information to improve).

   The function runs in general in time proportional to the size of area in
   that the updated variables are live.  If USF_PHIS_ALREADY_EXIST flag is
   used, the function assumes that all necessary phi nodes are already
   present and uses it to make the time proportional to the size of defs and
   uses list (however it does not verify that it is really not necessary
   to create new phis, so be sure that the phi nodes really exist).
   This should be efficient enough for most purposes.  In case the function
   causes performance problems for the specific task for that you need to use,
   you may consider looking at the specific properties of the transformation
   you perform and using them to create new phi nodes yourself, or even avoiding
   use of this function altogether (but note that unless you have some such
   reason, you should prefer this generic machinery to avoid unnecessary code
   duplication).
   
   The algorithm employed is basically the standard ssa form creation algorithm
   described in
   
   R. Cytron, J. Ferrante, B. Rosen, M. Wegman, and K. Zadeck. Efficiently
   Computing Static Single Assignment Form and the Control Dependence
   Graph. ACM Transactions on Programming Languages and Systems,
   13(4):451-490, October 1991,
   
   only restricted to the area of liveness.  */

/* List of definitions.  */

struct valdef
{
  struct ssa_update_value *val;	/* The value.  */
  def_operand_p op;		/* Position of the definition.  */
  struct valdef *next;		/* Next definition in the list.  */
  struct valdef *next_in_stack;	/* Next definition in the stack.  */
};

/* List of uses.  */

struct valuse
{
  struct ssa_update_value *val; /* The value.  */
  use_operand_p op;		/* Position of the use.  */
  struct valuse *next;		/* Next use in the list.  */
};

/* Representation of subtree of dominator tree.  */

struct dom_tree_node
{
  basic_block bb;		/* The basic block represented by the node.  */
  struct dom_tree_node *son;	/* The first son of the node.  */
  struct dom_tree_node *next;	/* A brother of the node.  */

  bitmap df;			/* Dominance frontiers of the node.  */
  bitmap values_live;		/* Bitmap of values that are live here.  */
  struct valdef *defs;		/* Values defined here.  */
  struct valuse *uses;		/* Values used here.  */
};

/* Structure that may contain use or def.  */

struct use_or_def
{
  union
    {
      struct usf_use_list *use;
      struct usf_def_list *def;
    } ud;

  unsigned is_use : 1;		/* True if this is an use.  */
};

/* If function has less than this number of basic blocks, we always
   scan whole dominance tree when creating the subtree of interesting
   blocks.  */

#define LARGE_AREA_BASE 25

/* If more than LARGE_AREA_FRACTION% of blocks are interesting, whole
   dominance tree is scanner when creating the subtree of interesting
   blocks.  */

#define LARGE_AREA_FRACTION 25

/* For storing the contents of aux fields.  */

struct aux_contents
{
  /* The stored data.  */
  void **old;

  /* Actual element when passing the tree in order to save/restore the
     aux fields.  */
  unsigned act;
};

/* Releases memory occupied by USE.  */

static void
free_use (struct usf_use_list *use)
{
  free (use);
}

/* Releases memory occupied by DEF.  */

static void
free_def (struct usf_def_list *def)
{
  free (def);
}

/* Determines the basic block in that USE occurs.  */

static basic_block
determine_use_bb (const struct usf_use_list *use)
{
  tree stmt = USE_STMT (use->op);
  unsigned idx;

  if (TREE_CODE (stmt) != PHI_NODE)
    return bb_for_stmt (stmt);

  idx = PHI_ARG_INDEX_FROM_USE (use->op);

  return PHI_ARG_EDGE (stmt, idx)->src;
}

/* Determines the statement in that DEF occurs.  */

tree
determine_def_stmt (const struct usf_def_list *def)
{
  tree name = DEF_FROM_PTR (def->op);
  
  return SSA_NAME_DEF_STMT (name);
}

/* Determines the basic block in that DEF occurs.  */

basic_block
determine_def_bb (const struct usf_def_list *def)
{
  return bb_for_stmt (determine_def_stmt (def));
}

/* Extracts information from X.  Basic block is stored to *BB, statement to
   *STMT.  True is returned if it is use.  */

static bool
get_use_or_def_info (const struct use_or_def *x, basic_block *bb, tree *stmt)
{
  if (x->is_use)
    {
      *stmt = USE_STMT (x->ud.use->op);
      *bb = determine_use_bb (x->ud.use);
    }
  else
    {
      *stmt = determine_def_stmt (x->ud.def);
      *bb = bb_for_stmt (*stmt);
    }

  if (*bb == NULL)
    *bb = ENTRY_BLOCK_PTR;

  return x->is_use;
}

/* Compare two struct usf_{use,def}_list elements by basic block and local
   dominance number.  */

static int
compare_by_ldom (const void *a, const void *b)
{
  basic_block bba, bbb;
  tree stmta, stmtb;
  bool usea, useb;

  usea = get_use_or_def_info (a, &bba, &stmta);
  useb = get_use_or_def_info (b, &bbb, &stmtb);

  /* The direction in that different blocks are sorted is chosen in such a way
     that the indices of the blocks in the lists that are later reassembled
     grow monotonically.  This is good for performance of bitmaps.  */
  if (bba != bbb)
    return bbb->index - bba->index;

  if (TREE_CODE (stmta) == PHI_NODE)
    return usea ? 1 : -1;
  if (TREE_CODE (stmtb) == PHI_NODE)
    return useb ? -1 : 1;

  if (stmta == stmtb)
    {
      if (usea)
	return -1;
      if (useb)
	return 1;
      return 0;
    }

  return stmt_dominated_by_p (stmtb, stmta) ? -1 : 1;
}

/* Rewrite use operand OP by DEF.  */

static void
rewrite_use (use_operand_p op, tree def)
{
  tree stmt;
  unsigned idx;

  gcc_assert (TREE_CODE (def) == SSA_NAME);
  SET_USE (op, def);

  stmt = USE_STMT (op);
  if (TREE_CODE (stmt) != PHI_NODE)
    return;

  /* Mark argument on abnormal edge.  */
  idx = PHI_ARG_INDEX_FROM_USE (op);
  if (PHI_ARG_EDGE (stmt, idx)->flags & EDGE_ABNORMAL)
    SSA_NAME_OCCURS_IN_ABNORMAL_PHI (def) = 1;
}

/* Rewrite USE by DEF.  */

static void
rewrite_by (struct usf_use_list *use, struct usf_def_list *def)
{
  rewrite_use (use->op, DEF_FROM_PTR (def->op));
}

/* Rewrite USE by DEF.  */

static void
rewrite_by_vd (struct valuse *use, struct valdef *def)
{
  tree adef;

  if (def)
    {
      gcc_assert (use->val == def->val);
      adef = DEF_FROM_PTR (def->op);
    }
  else
    {
      tree decl = use->val->decl;

      adef = default_def (decl);
      if (!adef)
	{
	  adef = make_ssa_name (decl, build_empty_stmt ());
	  set_default_def (decl, adef);
	}
    }

  rewrite_use (use->op, adef);
}

/* Updates ssa form for value VAL locally inside basic blocks.  The
   lists of uses and defs are pruned.  True is returned if there is no
   need for further ssa form updating, i.e. if all uses were rewritten
   by values inside their basic block.  */

static bool
update_ssa_form_locally_1 (struct ssa_update_value *val)
{
  unsigned i, n;
  struct usf_use_list *use;
  struct usf_def_list *def, *adef;
  struct use_or_def *uses_and_defs;
  basic_block last_def_block = NULL, bb;
  
  n = 0;
  for (def = val->defs; def; def = def->next)
    {
      tree d = DEF_FROM_PTR (def->op);
      gcc_assert (TREE_CODE (d) == SSA_NAME);
      /* Currently this should be always true.  This is not really
	 a hard constraint (we do not really care what the value of
	 the def is, as long as it is a SSA name), so this may get
	 changed in the future.  */
      gcc_assert (SSA_NAME_VAR (d) == val->decl);

      n++;
    }
  for (use = val->uses; use; use = use->next)
    n++;

  uses_and_defs = xmalloc (sizeof (struct use_or_def) * n);
  i = 0;
  for (def = val->defs; def; def = def->next, i++)
    {
      uses_and_defs[i].is_use = false;
      uses_and_defs[i].ud.def = def;
    }
  for (use = val->uses; use; use = use->next, i++)
    {
      uses_and_defs[i].is_use = true;
      uses_and_defs[i].ud.use = use;
    }

  val->defs = NULL;
  val->uses = NULL;

  qsort (uses_and_defs, n, sizeof (struct use_or_def), compare_by_ldom);
  for (i = 0; i < n; i++)
    {
      if (uses_and_defs[i].is_use)
	{
	  use = uses_and_defs[i].ud.use;

	  if (last_def_block == determine_use_bb (use))
	    {
	      rewrite_by (use, val->defs);
	      free_use (use);
	    }
	  else
	    {
	      use->next = val->uses;
	      val->uses = use;
	    }
	}
      else
	{
	  def = uses_and_defs[i].ud.def;
	  bb = determine_def_bb (def);

	  if (last_def_block == bb)
	    {
	      adef = val->defs;
	      val->defs = adef->next;
	      free_def (adef);
	    }

	  def->next = val->defs;
	  val->defs = def;
	  last_def_block = bb;
	}
    }

  free (uses_and_defs);

  return val->uses == NULL;
}

/* Frees value VAL.  */

static void
free_value (struct ssa_update_value *val)
{
  struct usf_def_list *def, *ndef;
  struct usf_use_list *use, *nuse;

  for (def = val->defs; def; def = ndef)
    {
      ndef = def->next;
      free_def (def);
    }
  for (use = val->uses; use; use = nuse)
    {
      nuse = use->next;
      free_use (use);
    }

  if (val->life_area)
    BITMAP_XFREE (val->life_area);

  free (val);
}

/* Updates ssa form for VALUES inside basic blocks.  The list of values for
   that global updating is necessary is returned.  The following properties
   are satisfied by uses and defs in the returned list:

   There is at most one def per basic block.
   If there are both uses and a def in a basic block, then the uses precede
   defs.
   Each value has at least one use.  */

static struct ssa_update_value *
update_ssa_form_locally (struct ssa_update_value *values)
{
  struct ssa_update_value **val, *aval;

  for (val = &values; *val; )
    {
      aval = *val;

      if (update_ssa_form_locally_1 (aval))
	{
	  *val = aval->next;

	  free_value (aval);
	}
      else
	val = &aval->next;
    }

  return values;
}

/* Mark the blocks in that VALUE is defined or used in bitmap BLOCKS.  */

static void
mark_use_and_def_blocks (struct ssa_update_value *value, bitmap blocks)
{
  struct usf_def_list *def;
  struct usf_use_list *use;
  basic_block bb;

  for (def = value->defs; def; def = def->next)
    {
      bb = determine_def_bb (def);
      if (!bb)
	continue;
      bitmap_set_bit (blocks, bb->index);
    }

  for (use = value->uses; use; use = use->next)
    bitmap_set_bit (blocks, determine_use_bb (use)->index);
}

/* Mark the blocks in that VALUE is live in bitmap BLOCKS.  */

static void
mark_liveness_area (struct ssa_update_value *value, bitmap blocks)
{
  struct usf_def_list *def;
  struct usf_use_list *use;
  basic_block bb;
  bitmap defs = BITMAP_XMALLOC (), livein = BITMAP_XMALLOC ();

  for (def = value->defs; def; def = def->next)
    {
      bb = determine_def_bb (def);
      if (!bb)
	continue;
      bitmap_set_bit (defs, bb->index);
    }
  for (use = value->uses; use; use = use->next)
    bitmap_set_bit (livein, determine_use_bb (use)->index);
  compute_global_livein (livein, defs);

  value->life_area = livein;
  bitmap_a_or_b (blocks, blocks, defs);
  bitmap_a_or_b (blocks, blocks, livein);
  free (defs);
}

/* Returns a new dominance tree node corresponding to basic block BB.  */

static struct dom_tree_node *
new_dom_tree_node (basic_block bb)
{
  struct dom_tree_node *ret = xcalloc (1, sizeof (struct dom_tree_node));
  ret->bb = bb;
  return ret;
}

/* Return a subtree of dominance tree induced by BLOCKS in a tree rooted
   at ROOT.  FATHER is the father node for the subtree.  */

static void
create_dom_subtree_walk_whole_1 (struct dom_tree_node *father,
				 basic_block root, bitmap blocks)
{
  basic_block bb;

  if (bitmap_bit_p (blocks, root->index))
    {
      struct dom_tree_node *node = new_dom_tree_node (root);

      node->next = father->son;
      father->son = node;
      father = node;
    }

  for (bb = first_dom_son (CDI_DOMINATORS, root);
       bb;
       bb = next_dom_son (CDI_DOMINATORS, bb))
    create_dom_subtree_walk_whole_1 (father, bb, blocks);
}

/* Return a subtree of dominance tree induced by BLOCKS.  The subtree is
   obtained by walking whole dominance tree.  */

static struct dom_tree_node *
create_dom_subtree_walk_whole (bitmap blocks)
{
  struct dom_tree_node *root = new_dom_tree_node (ENTRY_BLOCK_PTR);
  basic_block bb;

  for (bb = first_dom_son (CDI_DOMINATORS, ENTRY_BLOCK_PTR);
       bb;
       bb = next_dom_son (CDI_DOMINATORS, bb))
    create_dom_subtree_walk_whole_1 (root, bb, blocks);

  return root;
}

/* Return a subtree of dominance tree induced by BLOCKS.  The subtree is
   obtained by inserting the nodes in BLOCKS one by one.  */

static struct dom_tree_node *
create_dom_subtree_incremental (bitmap blocks)
{
  struct dom_tree_node *root = new_dom_tree_node (ENTRY_BLOCK_PTR);
  struct dom_tree_node *nw, *act_father, **act_p, *act;
  bitmap_iterator bi;
  unsigned index;
  basic_block bb;

  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, index, bi)
    {
      bb = BASIC_BLOCK (index);
      nw = new_dom_tree_node (bb);
      act_father = root;

      while (1)
	{
	  for (act_p = &act_father->son; *act_p;)
	    {
	      act = *act_p;

	      if (dominated_by_p (CDI_DOMINATORS, bb, act->bb))
		break;

	      if (dominated_by_p (CDI_DOMINATORS, act->bb, bb))
		{
		  *act_p = act->next;
		  act->next = nw->son;
		  nw->son = act;
		}
	      else
		act_p = &act->next;
	    }

	  if (!*act_p)
	    break;

	  act_father = *act_p;
	  gcc_assert (!nw->son);
	}

      nw->next = act_father->son;
      act_father->son = nw;
    }

  return root;
}

/* Passes dominance tree DOM_TREE in dfs. CALLBACK_BEFORE is called for each
   node before its sons are traversed, and CALLBACK_AFTER is called after
   they are traversed (any of the callbacks may be NULL, in which case it is
   ignored).  DATA is passed to the callbacks.  */

static void
walk_dom_tree (struct dom_tree_node *dom_tree,
	       void (*callback_before) (struct dom_tree_node *, void *),
	       void (*callback_after) (struct dom_tree_node *, void *),
	       void *data)
{
  struct dom_tree_node *node, *next;

  if (callback_before)
    callback_before (dom_tree, data);
  for (node = dom_tree->son; node; node = next)
    {
      next = node->next;
      walk_dom_tree (node, callback_before, callback_after, data);
    }
  if (callback_after)
    callback_after (dom_tree, data);
}

/* Moves list of valdefs from aux field of the basic block to NODE.
   Callback for walk_dom_tree.  */

static void
wdt_set_defs (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  node->defs = node->bb->aux;
  node->bb->aux = NULL;
}

/* Returns a new valdef for VALUE at position OP.  NEXT is the next valdef in
   the list.  */

static struct valdef *
new_valdef (struct ssa_update_value *value,
	    def_operand_p op,
	    struct valdef *next)
{
  struct valdef *ret = xmalloc (sizeof (struct valdef));

  ret->val = value;
  ret->op = op;
  ret->next = next;
  ret->next_in_stack = NULL;

  return ret;
}

/* Move the information about defs from VALUES to DOM_TREE.  */

static void
attach_def_lists_to_dom_tree (struct dom_tree_node *dom_tree,
			      struct ssa_update_value *values)
{
  struct ssa_update_value *value;
  struct usf_def_list *def;

  for (value = values; value; value = value->next)
    for (def = value->defs; def; def = def->next)
      {
	basic_block bb = determine_def_bb (def);
	bb->aux = new_valdef (value, def->op, bb->aux);
      }

  walk_dom_tree (dom_tree, wdt_set_defs, NULL, NULL);
}

/* Moves list of valuses from aux field of the basic block to NODE.
   Callback for walk_dom_tree.  */

static void
wdt_set_uses (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  node->uses = node->bb->aux;
  node->bb->aux = NULL;
}

/* Returns a new valuse for VALUE at position OP.  NEXT is the next valuse in
   the list.  */

static struct valuse *
new_valuse (struct ssa_update_value *value,
	    use_operand_p op,
	    struct valuse *next)
{
  struct valuse *ret = xmalloc (sizeof (struct valuse));

  ret->val = value;
  ret->op = op;
  ret->next = next;

  return ret;
}

/* Move the information about uses from VALUES to DOM_TREE.  */

static void
attach_use_lists_to_dom_tree (struct dom_tree_node *dom_tree,
			      struct ssa_update_value *values)
{
  struct ssa_update_value *value;
  struct usf_use_list *use;

  for (value = values; value; value = value->next)
    for (use = value->uses; use; use = use->next)
      {
	basic_block bb = determine_use_bb (use);
	bb->aux = new_valuse (value, use->op, bb->aux);
      }

  walk_dom_tree (dom_tree, wdt_set_uses, NULL, NULL);
}

/* Moves liveness bitmaps from aux field of the basic block to NODE.
   Callback for walk_dom_tree.  */

static void
wdt_set_live (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  node->values_live = node->bb->aux;
  node->bb->aux = NULL;
}

/* Move the information about liveness of VALUES to DOM_TREE.  */

static void
attach_liveness_to_dom_tree (struct dom_tree_node *dom_tree,
			     struct ssa_update_value *values)
{
  struct ssa_update_value *value;
  bitmap_iterator bi;
  basic_block bb;
  unsigned index;

  for (value = values; value; value = value->next)
    {
      if (!value->life_area)
	continue;

      EXECUTE_IF_SET_IN_BITMAP (value->life_area, 0, index, bi)
	{
	  bb = BASIC_BLOCK (index);

	  if (!bb->aux)
	    bb->aux = BITMAP_XMALLOC ();

	  bitmap_set_bit (bb->aux, value->id);
	}
    }

  walk_dom_tree (dom_tree, wdt_set_live, NULL, NULL);
}

/* Set aux field of NODE->bb to NODE.  Callback for walk_dom_tree.  */

static void
wdt_set_aux_to_node (struct dom_tree_node *node,
		     void *data ATTRIBUTE_UNUSED)
{
  node->bb->aux = node;
}

/* Clear the aux field of NODE->bb and store the original value to the array
   passed in DATA.  */

static void
wdt_save_aux_field (struct dom_tree_node *node, void *data)
{
  struct aux_contents *old = data;

  old->old[old->act++] = node->bb->aux;
  node->bb->aux = NULL;
}

/* Save and clear aux fields for nodes of DOM_TREE.  SIZE is the number
   of nodes of the DOM_TREE.  The structure containing the previous contents
   of aux fields is returned.  */

static struct aux_contents
save_aux_fields (struct dom_tree_node *dom_tree, unsigned size)
{
  struct aux_contents ret;

  ret.old = xmalloc (size * sizeof (void *));
  ret.act = 0;
  walk_dom_tree (dom_tree, wdt_save_aux_field, NULL, &ret);

  gcc_assert (ret.act == size);
  return ret;
}

/* Set aux field of NODE->bb to the original value (passed to it in array in
   DATA).  Callback for walk_dom_tree.  */

static void
wdt_restore_aux_field (struct dom_tree_node *node, void *data)
{
  struct aux_contents *old = data;

  node->bb->aux = old->old[old->act++];
}

/* Restores the aux fields for nodes of DOM_TREE using the data from OLD.  */

static void
restore_aux_fields (struct dom_tree_node *dom_tree, struct aux_contents old)
{
  old.act = 0;
  walk_dom_tree (dom_tree, wdt_restore_aux_field, NULL, &old);
  free (old.old);
}

/* Move the information about uses and defs from VALUES to DOM_TREE.  */

static void
attach_use_and_def_info_to_dom_tree (struct dom_tree_node *dom_tree,
				     struct ssa_update_value *values)
{
  attach_def_lists_to_dom_tree (dom_tree, values);
  attach_use_lists_to_dom_tree (dom_tree, values);
  attach_liveness_to_dom_tree (dom_tree, values);
  walk_dom_tree (dom_tree, wdt_set_aux_to_node, NULL, NULL);
}

/* Calculate a dominance frontiers for NODE, using the df information
   that is already computed for all sons of NODE in the dominance tree.
   Callback for walk_dom_tree.  */

static void
wdt_calc_df (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  edge e;
  edge_iterator ei;
  struct dom_tree_node *son;
  unsigned index;
  basic_block bb;
  bitmap_iterator bi;

  node->df = BITMAP_XMALLOC ();

  FOR_EACH_EDGE (e, ei, node->bb->succs)
    {
      /* If the block does not belong to the area of liveness, ignore it.  */
      if (!e->dest->aux)
	continue;

      if (dominated_by_p (CDI_DOMINATORS, e->dest, node->bb))
	continue;

      bitmap_set_bit (node->df, e->dest->index);
    }

  for (son = node->son; son; son = son->next)
    {
      EXECUTE_IF_SET_IN_BITMAP (son->df, 0, index, bi)
	{
	  bb = BASIC_BLOCK (index);

	  if (!dominated_by_p (CDI_DOMINATORS, bb, node->bb))
	    bitmap_set_bit (node->df, index);
	}
    }
}

/* Calculate dominance frontiers in DOM_TREE.  */

static void
calculate_df (struct dom_tree_node *dom_tree)
{
  walk_dom_tree (dom_tree, NULL, wdt_calc_df, NULL);
}

/* Create a phi node for VALUE in NODE and record new definitions and uses.
   Returns true if there is not another definition for value in node.  */

static void
create_a_phi (struct ssa_update_value *value,
	      struct dom_tree_node *node)
{
  tree name;
  tree phi = create_phi_node (value->decl, node->bb), aphi = phi;
  edge e;
  edge_iterator ei;
  unsigned i;
  struct dom_tree_node *unode;

  /* FIXME -- for now we simply copy the values attached to the newly created
     ssa name from the original ssa name of the value.  This would be
     wrong in the case the pass calling update_ssa_form would make such
     information more precise in the original definition (and possibly some
     of the copies).

     So we need to replace this by a trivial dataflow over the newly created
     phi nodes that merges the information once the optimizations will start
     to need this (alternatively even without this they may create the phi
     nodes together with the appropriate information themselves and call
     update_ssa_form with USF_PHIS_ALREADY_EXIST, but in some cases this might
     be quite inconvenient).  */
  name = duplicate_ssa_name (value->orig_name, phi);
  SET_PHI_RESULT (phi, name);
  node->defs = new_valdef (value, PHI_RESULT_PTR (phi), node->defs);

  FOR_EACH_EDGE (e, ei, node->bb->preds)
    {
      add_phi_arg (&phi, name, e);
    }

  /* Phi should not get reallocated (if it happened, def_operand_p to
     the result would get invalidated).  */
  gcc_assert (phi == aphi);

  for (i = 0; i < (unsigned) PHI_NUM_ARGS (phi); i++)
    {
      e = PHI_ARG_EDGE (phi, i);
      unode = e->src->aux;
      unode->uses = new_valuse (value, PHI_ARG_DEF_PTR (phi, i), unode->uses);
    }
}

/* Walk dominance frontiers from NODE and create new phi nodes for VALUE if
   necessary.  */

static void
create_new_phi_nodes_for (struct ssa_update_value *value,
			  struct dom_tree_node *node)
{
  bitmap_iterator bi;
  unsigned index;
  basic_block bb;
  struct dom_tree_node *bb_node;

  EXECUTE_IF_SET_IN_BITMAP (node->df, 0, index, bi)
    {
      bb = BASIC_BLOCK (index);
      bb_node = bb->aux;

      if (!bb_node->values_live)
	continue;
      if (!bitmap_bit_p (bb_node->values_live, value->id))
	continue;
      bitmap_clear_bit (bb_node->values_live, value->id);

      create_a_phi (value, bb_node);
      create_new_phi_nodes_for (value, bb_node);
    }
}

/* Create new phi nodes for values in NODE.  Callback for walk_dom_tree.  */

static void
wdt_create_new_phi_nodes (struct dom_tree_node *node,
			  void *data ATTRIBUTE_UNUSED)
{
  struct valdef *def;

  for (def = node->defs; def; def = def->next)
    create_new_phi_nodes_for (def->val, node);
}

/* Create new phi nodes for values in DOM_TREE.  */

static void
create_new_phi_nodes (struct dom_tree_node *dom_tree)
{
  walk_dom_tree (dom_tree, wdt_create_new_phi_nodes, NULL, NULL);
}

/* Rewrites uses in basic block corresponding to NODE and registers new
   definitions.  Callback for walk_dom_tree.  */

static void
wdt_rewrite_uses_in (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  struct valuse *use;
  struct valdef *def;
  tree stmt;

  /* Definitions in the newly created phi nodes need to be processed before
     uses (there may be also definitions in phi nodes that were not created
     by this optimization, but in that case there are no uses of the value
     in this block due to update_ssa_form_locally, so processing them here is
     harmless).  */
  for (def = node->defs; def; def = def->next)
    {
      stmt = SSA_NAME_DEF_STMT (DEF_FROM_PTR (def->op));

      if (TREE_CODE (stmt) != PHI_NODE)
	continue;

      def->next_in_stack = def->val->stack;
      def->val->stack = def;
    }

  /* Next come the uses that are not in phi nodes.  */
  for (use = node->uses; use; use = use->next)
    {
      stmt = USE_STMT (use->op);
      if (TREE_CODE (stmt) != PHI_NODE)
	rewrite_by_vd (use, use->val->stack);
    }

  /* Now record non-phi defs.  Due to update_ssa_form_locally they must indeed
     be after uses.  */
  for (def = node->defs; def; def = def->next)
    {
      stmt = SSA_NAME_DEF_STMT (DEF_FROM_PTR (def->op));

      if (TREE_CODE (stmt) == PHI_NODE)
	continue;

      def->next_in_stack = def->val->stack;
      def->val->stack = def;
    }

  /* Next come the uses in newly created (possibly -- see the comment for defs)
     that are in phi nodes.  */
  for (use = node->uses; use; use = use->next)
    {
      stmt = USE_STMT (use->op);
      if (TREE_CODE (stmt) == PHI_NODE)
	rewrite_by_vd (use, use->val->stack);
    }
}

/* Restores definitions in NODE.  Callback for walk_dom_tree.  */

static void
wdt_rewrite_uses_out (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  struct valdef *def;

  for (def = node->defs; def; def = def->next)
    {
      def->val->stack = def->next_in_stack;
      def->next_in_stack = NULL;
    }
}

/* Rewrite uses in DOM_TREE by their reaching definitions.  */

static void
rewrite_uses (struct dom_tree_node *dom_tree)
{
  walk_dom_tree (dom_tree, wdt_rewrite_uses_in, wdt_rewrite_uses_out, NULL);
}

/* Release memory for VALUES.  */

static void
free_values_list (struct ssa_update_value *values)
{
  struct ssa_update_value *value, *next;

  for (value = values; value; value = next)
    {
      next = value->next;

      free_value (value);
    }
}

/* Releases memory occupied by NODE.  Callback for walk_dom_tree.  */

static void
wdt_free_dom_node (struct dom_tree_node *node, void *data ATTRIBUTE_UNUSED)
{
  struct valdef *def, *ndef;
  struct valuse *use, *nuse;

  for (def = node->defs; def; def = ndef)
    {
      ndef = def->next;
      free (def);
    }
  for (use = node->uses; use; use = nuse)
    {
      nuse = use->next;
      free (use);
    }

  if (node->df)
    BITMAP_XFREE (node->df);

  if (node->values_live)
    BITMAP_XFREE (node->values_live);

  free (node);
}

/* Release memory for DOM_TREE.  */

static void
free_dom_subtree (struct dom_tree_node *dom_tree)
{
  walk_dom_tree (dom_tree, NULL, wdt_free_dom_node, NULL);
}

/* Update ssa form for VALUES.  If USF_PHIS_ALREADY_EXIST is set in FLAGS,
   assumes that it is not necessary to create any new phi nodes (i.e. only
   rewrites the uses by dominating definitions.  The VALUES list is freed
   by the function.  */

void
update_ssa_form (struct ssa_update_value *values, unsigned flags)
{
  bitmap interesting_blocks;
  struct ssa_update_value *value;
  struct dom_tree_node *dom_tree;
  unsigned id, n, i;
  bitmap_iterator bi;
  bool large_area;
  struct aux_contents aux_contents;

  if (!values)
    return;

  values = update_ssa_form_locally (values);
  if (!values)
    return;

  interesting_blocks = BITMAP_XMALLOC ();

  for (value = values, id = 0; value; value = value->next, id++)
    {
      value->id = id; 
      if (flags & USF_PHIS_ALREADY_EXIST)
	mark_use_and_def_blocks (value, interesting_blocks);
      else
	mark_liveness_area (value, interesting_blocks);
    }

  n = 0;
  EXECUTE_IF_SET_IN_BITMAP (interesting_blocks, 0, i, bi)
    {
      n++;
    }

  if (n_basic_blocks < LARGE_AREA_BASE)
    large_area = true;
  else
    large_area = (100 * n > LARGE_AREA_FRACTION * (unsigned) n_basic_blocks);

  if (large_area)
    dom_tree = create_dom_subtree_walk_whole (interesting_blocks);
  else
    dom_tree = create_dom_subtree_incremental (interesting_blocks);
  BITMAP_XFREE (interesting_blocks);

  /* We need to use aux fields at blocks, but we cannot assume that the caller
     does not already use them.  So save the original contents here and restore
     it at the end.  */
  aux_contents = save_aux_fields (dom_tree, n + 1);

  attach_use_and_def_info_to_dom_tree (dom_tree, values);

  if (!(flags & USF_PHIS_ALREADY_EXIST))
    {
      calculate_df (dom_tree);
      create_new_phi_nodes (dom_tree);
    }

  rewrite_uses (dom_tree);

  restore_aux_fields (dom_tree, aux_contents);
  free_values_list (values);
  free_dom_subtree (dom_tree);
}

/* Updates ssa form for all defs registered through rewrite_new_def.
   FLAGS are passed to update_ssa_form.  */

void
update_ssa_form_for_registered_defs (unsigned flags)
{
  update_ssa_form (get_values_for_ssa_form_update (), flags);
  ssa_form_updated_all ();
}

/* Management of ssa names for that ssa form is violated.  */

struct ssa_name_eqto_elt
{
  /* The ssa name version.  */
  unsigned ver;

  /* Operand for the definition.  */
  def_operand_p op;

  /* Original definition.  */
  tree orig;

  /* Next definition in chain of the new definitions.  */
  struct ssa_name_eqto_elt *next;

  /* Previous definition in the chain.  */
  struct ssa_name_eqto_elt *prev;
};

static varray_type ssa_name_eqto;

static bitmap ssa_name_eqto_set;

/* Initializes the records for ssa names.  */

void
ssa_name_eqto_init (void)
{
  VARRAY_GENERIC_PTR_NOGC_INIT (ssa_name_eqto, 100, "ssa_name_eqto");
  ssa_name_eqto_set = BITMAP_XMALLOC ();
}

/* Returns entry for NAME in ssa_name_eqto array.  */

static struct ssa_name_eqto_elt *
get_ssa_name_eqto_entry (tree name)
{
  unsigned ver = SSA_NAME_VERSION (name);
  struct ssa_name_eqto_elt *elt;

  if (ver >= VARRAY_SIZE (ssa_name_eqto))
    VARRAY_GROW (ssa_name_eqto, 5 * ver / 4);

  elt = VARRAY_GENERIC_PTR_NOGC (ssa_name_eqto, ver);
  if (!elt)
    {
      elt = xmalloc (sizeof (struct ssa_name_eqto_elt));
      elt->ver = SSA_NAME_VERSION (name);
      elt->orig = NULL;
      elt->prev = NULL;
      elt->next = NULL;

      VARRAY_GENERIC_PTR_NOGC (ssa_name_eqto, ver) = elt;
    }

  return elt;
}

/* Returns a def operand pointer for NAME.  */

static def_operand_p
def_op_for_name (tree name)
{
  tree stmt = SSA_NAME_DEF_STMT (name);
  def_operand_p ret;
  ssa_op_iter oi;
  unsigned flags;

  if (TREE_CODE (stmt) == PHI_NODE)
    return PHI_RESULT_PTR (stmt);

  get_stmt_operands (stmt);

  if (is_gimple_reg (name))
    flags = SSA_OP_DEF;
  else
    flags = SSA_OP_VIRTUAL_DEFS;

  FOR_EACH_SSA_DEF_OPERAND (ret, stmt, oi, flags)
    {
      if (DEF_FROM_PTR (ret) == name)
	return ret;
    }

  gcc_unreachable ();
}

/* Returns entry for NAME in ssa_name_eqto array.  If there is no entry
   for NAME yet, create it and mark it as original.  */

static struct ssa_name_eqto_elt *
get_ssa_name_eqto_entry_orig (tree name)
{
  struct ssa_name_eqto_elt *elt = get_ssa_name_eqto_entry (name);

  if (elt->orig)
    return elt;

  elt->orig = name;
  elt->next = elt;
  elt->prev = elt;
  elt->op = def_op_for_name (name);
  bitmap_set_bit (ssa_name_eqto_set, SSA_NAME_VERSION (name));

  return elt;
}

/* Returns true if there is any ssa name with equivalent definitions
   set.  */

bool
any_values_for_ssa_update_p (void)
{
  return bitmap_first_set_bit (ssa_name_eqto_set) >= 0;
}

/* Returns a bitmap of ssa names with equivalent definitions set.  */

bitmap
ssa_names_for_ssa_update (void)
{
  bitmap ret = BITMAP_XMALLOC ();
  bitmap_copy (ret, ssa_name_eqto_set);
  return ret;
}

/* Returns a def operand for DEF.  */

static def_operand_p
eqto_entry_def_op (struct ssa_name_eqto_elt *def)
{
  tree stmt = SSA_NAME_DEF_STMT (ssa_name (def->ver));

  if (TREE_CODE (stmt) == PHI_NODE)
    {
      /* The def operand may get invalidated if the phi node is
	 reallocated, so determine it again.  */
      return PHI_RESULT_PTR (stmt);
    }
  else
    return def->op;
}

/* Get list of equivalent definitions for NAME.  */

struct usf_def_list *
get_defs_to_update (tree name)
{
  struct usf_def_list *ret, **last = &ret, *act_usf;
  struct ssa_name_eqto_elt *act = get_ssa_name_eqto_entry (name), *end;

  if (!act->orig)
    return NULL;

  act = get_ssa_name_eqto_entry (act->orig);
  end = act;
  do
    {
      act_usf = xmalloc (sizeof (struct usf_def_list));
      act_usf->op = eqto_entry_def_op (act);
      *last = act_usf;
      last = &act_usf->next;

      act = act->next;
    } while (act != end);

  *last = NULL;
  return ret;
}

/* Finds uses for update for DEFS.  */

static struct usf_use_list *
get_uses_to_update (struct usf_def_list *defs)
{
  struct usf_use_list *uses, *nw;
  struct usf_def_list *def;
  tree name;
  use_operand_p use;
  imm_use_iterator iui;

  uses = NULL;
  for (def = defs; def; def = def->next)
    {
      name = DEF_FROM_PTR (def->op);

      FOR_EACH_IMM_USE_FAST (use, iui, name)
	{
	  nw = xmalloc (sizeof (struct usf_use_list));
	  nw->op = use;
	  nw->next = uses;
	  uses = nw;
	}
    }

  return uses;
}

/* Releases a list of definitions DEFS.  */

void
free_def_list (struct usf_def_list *defs)
{
  struct usf_def_list *act, *next;

  for (act = defs; act; act = next)
    {
      next = act->next;

      free (act);
    }
}

/* Get all recorded definitions to update.  */

struct ssa_update_value *
get_values_for_ssa_form_update (void)
{
  struct ssa_update_value *ret, **last, *act;
  bitmap_iterator bi;
  unsigned ver;
  tree name;

  last = &ret;

  EXECUTE_IF_SET_IN_BITMAP (ssa_name_eqto_set, 0, ver, bi)
    {
      name = ssa_name (ver);
      act = xcalloc (1, sizeof (struct ssa_update_value));

      act->decl = SSA_NAME_VAR (name);
      act->orig_name = name;
      act->defs = get_defs_to_update (name);
      act->uses = get_uses_to_update (act->defs);
      *last = act;
      last = &act->next;
    }
  *last = NULL;

  return ret;
}

/* Returns the original ssa name to that NAME is equivalent.  */

tree
original_equivalent_name (tree name)
{
  struct ssa_name_eqto_elt *elt = get_ssa_name_eqto_entry (name);

  if (!elt->orig)
    {
      /* No name was set as original to NAME yet.  So consider NAME to be
	 original.  */

      return name;
    }

  return elt->orig;
}

/* Rewrites the definition DEF in statement STMT by new ssa name.  The ssa
   name is returned.  The new ssa name is recorded as one of the aliases of
   the old one.  */

tree
rewrite_new_def (tree stmt, def_operand_p def)
{
  tree name = DEF_FROM_PTR (def);
  tree orig = original_equivalent_name (name);
  tree new_name = duplicate_ssa_name (orig, stmt);
  struct ssa_name_eqto_elt *orig_elt, *new_elt;

  SET_DEF (def, new_name);

  if (TREE_CODE (stmt) == PHI_NODE)
    {
      edge e;
      edge_iterator ei;
      basic_block bb = bb_for_stmt (stmt);

      /* Mark the result of abnormal phi node if needed.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
	{
	  if (e->flags & EDGE_ABNORMAL)
	    break;
	}

      if (e)
	SSA_NAME_OCCURS_IN_ABNORMAL_PHI (new_name) = 1;
    }

  orig_elt = get_ssa_name_eqto_entry_orig (orig);
  new_elt = get_ssa_name_eqto_entry (new_name);

  new_elt->orig = orig;
  new_elt->prev = orig_elt->prev;
  new_elt->prev->next = new_elt;
  orig_elt->prev = new_elt;
  new_elt->next = orig_elt;
  new_elt->op = def;

  return new_name;
}

/* Records that form updating for NAME was performed.  */

void
ssa_form_updated (tree name)
{
  struct ssa_name_eqto_elt *elt = get_ssa_name_eqto_entry (name);
  struct ssa_name_eqto_elt *next, *end;

  if (!elt->orig)
    return;

  bitmap_clear_bit (ssa_name_eqto_set, SSA_NAME_VERSION (elt->orig));

  end = elt;
  do
    {
      next = elt->next;

      elt->orig = NULL;
      elt->next = NULL;
      elt->prev = NULL;

      elt = next;
    } while (elt != end);
}

/* Records that ssa form updating for all recorded ssa names was performed.  */

void
ssa_form_updated_all (void)
{
  bitmap_iterator bi;
  unsigned ver;
  bitmap defs = ssa_names_for_ssa_update ();

  EXECUTE_IF_SET_IN_BITMAP (defs, 0, ver, bi)
    {
      ssa_form_updated (ssa_name (ver));
    }

  BITMAP_XFREE (defs);
}

/* Releases ssa name NAME from eqto tables.  */

void
release_ssa_name_from_eqto (tree name)
{
  struct ssa_name_eqto_elt *elt = get_ssa_name_eqto_entry (name);
  struct ssa_name_eqto_elt *prev, *next;
  tree orig, eq;
  use_operand_p use;
  imm_use_iterator iui;

  if (!elt->orig)
    return;

  next = elt->next;
  prev = elt->prev;
  orig = elt->orig;
  elt->next = elt->prev = NULL;
  elt->orig = NULL_TREE;

  if (orig == name)
    bitmap_clear_bit (ssa_name_eqto_set, SSA_NAME_VERSION (name));

  if (next == elt)
    {
      gcc_assert (orig == name);
      return;
    }

  /* Since there are equivalent ssa names, it is possible that there still may
     be uses of this ssa name that will not be removed when the definition
     ceases to exist.  So rewrite the uses to some of the equivalents.  */

  if (orig != name)
    eq = orig;
  else
    eq = ssa_name (next->ver);

  FOR_EACH_IMM_USE_SAFE (use, iui, name)
    {
      SET_USE (use, eq);
    }

  next->prev = prev;
  prev->next = next;

  if (orig != name)
    return;

  /* Make the next element in the list the original.  */
  bitmap_set_bit (ssa_name_eqto_set, next->ver);
  orig = ssa_name (next->ver);

  elt = next;
  do
    {
      elt->orig = orig;
      elt = elt->next;
    } while (elt != next);
}

/* Rewrites USE by VER-th version of the definition.  */

static void
rewrite_use_by_ver (use_operand_p use, unsigned ver)
{
  tree name = USE_FROM_PTR (use);
  struct ssa_name_eqto_elt *act, *def_elt;

  if (TREE_CODE (name) != SSA_NAME)
    {
      gcc_assert (is_gimple_min_invariant (name));
      return;
    }

  act = get_ssa_name_eqto_entry (name);

  /* Unless we have some alternative definitions for name recorded,
     there is nothing to do.  */
  if (!act->orig)
    return;

  act = def_elt = get_ssa_name_eqto_entry (act->orig);
  for (; ver != 0; ver--)
    {
      def_elt = def_elt->next;

      /* Check that we do not wrap around in the list.  */
      gcc_assert (def_elt != act);
    }

  SET_USE (use, ssa_name (def_elt->ver));
}

/* Rewrites uses in BB by VER-th version of definitions.  */

void
rewrite_uses_bb (basic_block bb, unsigned ver)
{
  block_stmt_iterator bsi;
  edge_iterator ei;
  tree phi, stmt;
  ssa_op_iter oi;
  use_operand_p use;
  edge e;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);

      FOR_EACH_SSA_USE_OPERAND (use, stmt, oi, SSA_OP_ALL_USES)
	{
	  rewrite_use_by_ver (use, ver);
	}
    }

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
	{
	  use = PHI_ARG_DEF_PTR_FROM_EDGE (phi, e);
	  rewrite_use_by_ver (use, ver);
	}
    }
}

/* Rewrites uses in REGION of size N_REGION by VER-th version of
   definitions.  */

void
rewrite_uses_region (basic_block *region, unsigned n_region, unsigned ver)
{
  unsigned i;

  for (i = 0; i < n_region; i++)
    rewrite_uses_bb (region[i], ver);
}
