/* Callgraph transitive closure
   Copyright (C) 2004, 2005 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
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
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "pointer-set.h"
#include "ggc.h"
#include "ipa-utils.h"
#include "c-common.h"
#include "tree-gimple.h"
#include "cgraph.h"
#include "output.h"
#include "flags.h"
#include "timevar.h"
#include "diagnostic.h"
#include "langhooks.h"

static bitmap_obstack callee_obstack;

void debug_bitmap_of_cgraph_nodes (bitmap);
void print_bitmap_of_cgraph_nodes (FILE *, bitmap);

void
print_bitmap_of_cgraph_nodes (FILE *file, bitmap bmap)
{
  bitmap_iterator bi;
  unsigned int i;

  fprintf (file, " { ");
  EXECUTE_IF_SET_IN_BITMAP (bmap, 0, i, bi)
    {
      struct cgraph_node *node = cgraph_node_by_uid (i);
      fprintf (file, "%s ", cgraph_node_name (node));
    }
    fprintf (file, "}\n");
}

void
debug_bitmap_of_cgraph_nodes (bitmap a)
{

  print_bitmap_of_cgraph_nodes (stderr, a);
}

/* Mark UID in IDS if FN contains indirect function calls.  */

static void
scan_for_indirect_calls (tree fn, unsigned int uid, bitmap ids)
{
  struct function *cfun = DECL_STRUCT_FUNCTION (fn);
  basic_block bb;

  /* The pop happens in the caller due to the possible early return.  */
  push_cfun (cfun);

  FOR_EACH_BB_FN (bb, cfun)
    {
      block_stmt_iterator bsi;
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  tree possible_call = NULL;
	  if (TREE_CODE (stmt) == RETURN_EXPR)
	    stmt = TREE_OPERAND (stmt, 0);

	  if (!stmt)
	    continue;

	  if (TREE_CODE (stmt) == MODIFY_EXPR
	      || TREE_CODE (stmt) == RETURN_EXPR)
	    possible_call = TREE_OPERAND (stmt, 1);
	  else if (TREE_CODE (stmt) == CALL_EXPR)
	    possible_call = stmt;

	  if (possible_call && TREE_CODE (possible_call) == CALL_EXPR)
	    if (!get_callee_fndecl (possible_call))
	      {
		bitmap_set_bit (ids, uid);
		return;
	      }
	}
    }

}

/* Return true if PARG, a pointer type, points to a readonly
   non-pointer type.  */

static bool
argument_readonly_nonpointer (tree parg)
{
  if (POINTER_TYPE_P (TREE_TYPE (parg)))
    return false;
  if (!TYPE_READONLY (TREE_TYPE (parg)))
    return false;
  return true;
}

/* Return true if fn has non-readonly pointer arguments.  */

static bool
has_nonreadonly_pointer_arguments (tree fn)
{
  tree arg;


  for (arg = DECL_ARGUMENTS (fn);
       arg;
       arg = TREE_CHAIN (arg))
    {
      if (POINTER_TYPE_P (TREE_TYPE (arg))
	  && !argument_readonly_nonpointer (arg))
	return true;
    }

  for (arg = TYPE_ARG_TYPES (TREE_TYPE (fn));
       arg && arg != void_list_node;
       arg = TREE_CHAIN (arg))
    {
      if (POINTER_TYPE_P (TREE_VALUE (arg))
	  && !argument_readonly_nonpointer (TREE_VALUE (arg)))
	return true;
    }

  return false;
}

static void
callees_execute (void)
{
  struct cgraph_node *node;
  bitmap global_and_address_taken = BITMAP_ALLOC (NULL);
  bitmap contains_indirect_call = BITMAP_ALLOC (NULL);
  struct cgraph_node **postorder = xcalloc (cgraph_n_nodes,
					    sizeof (struct cgraph_node *));
  bool changed = true;
  int order_pos;
  int i;

  bitmap_obstack_initialize (&callee_obstack);

  /* Go through the cgraph nodes, record which are global or address
     taken, and make a first pass at the callees.  */
  for (node = cgraph_nodes; node; node = node->next)
      {
	tree decl = node->decl;
	enum availability avail = cgraph_function_body_availability (node);
	function_ann_t ann = get_function_ann (decl);
	struct cgraph_edge *e;

	ann->has_pointer_arguments = has_nonreadonly_pointer_arguments (decl);
	if (!node->analyzed)
	  continue;
	/* If this is global, addressable or no body is available,
	   mark it global/address taken.  Otherwise, scan for indirect
	   calls.  */

	if (TREE_ADDRESSABLE (decl) || avail == AVAIL_NOT_AVAILABLE)
	  bitmap_set_bit (global_and_address_taken, node->uid);
	else if (avail != AVAIL_NOT_AVAILABLE)
	  {

	    scan_for_indirect_calls (node->decl, node->uid,
				     contains_indirect_call);
	    pop_cfun ();
	  }

	ann->callees = BITMAP_ALLOC (&callee_obstack);
	for (e = node->callees; e; e = e->next_callee)
	  {
	    struct cgraph_node *cnode = e->callee;
	    bitmap_set_bit (ann->callees, cnode->uid);
	  }
      }

  order_pos = cgraph_postorder (postorder);

  /* Iterate in reverse postorder until we are done with the
     closure.
     XXX: Use the SCC stuff to make this faster.  */
  while (changed)
    {
      changed = false;
      for (i = order_pos - 1; i >= 0; i--)
	{
	  struct cgraph_edge *e;
	  function_ann_t ann;
	  node = postorder[i];
	  ann = function_ann (node->decl);

	  if (!node->analyzed)
	    continue;

	  if (bitmap_bit_p (contains_indirect_call, node->uid))
	    changed |= bitmap_ior_into (ann->callees, global_and_address_taken);
	  for (e = node->callees; e; e = e->next_callee)
	    {
	      struct cgraph_node *cnode = e->callee;
	      enum availability avail = cgraph_function_body_availability (cnode);
	      if (avail == AVAIL_NOT_AVAILABLE)
		changed |= bitmap_ior_into (ann->callees,
					    global_and_address_taken);
	      else
		changed |= bitmap_ior_into (ann->callees,
					    function_ann (cnode->decl)->callees);

	    }
	}
    }
  BITMAP_FREE (global_and_address_taken);
  BITMAP_FREE (contains_indirect_call);
  free (postorder);
}

static bool
gate_callees (void)
{
  return (flag_unit_at_a_time != 0
	  /* Don't bother doing anything if the program has errors.  */
	  && !(errorcount || sorrycount));
}

struct tree_opt_pass pass_ipa_callees =
{
  "callees",		                /* name */
  gate_callees,			/* gate */
  callees_execute,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_CALLEES,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0					/* letter */
};
