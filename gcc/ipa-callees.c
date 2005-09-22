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

static void
callees_execute (void)
{
  struct cgraph_node *node;
  bitmap global_and_address_taken = BITMAP_ALLOC (NULL);
  struct cgraph_node **postorder = xcalloc (cgraph_n_nodes,
					    sizeof (struct cgraph_node *));
  bool changed = true;
  int order_pos;
  int i;

  bitmap_obstack_initialize (&callee_obstack);

  /* Go through the cgraph nodes, record which are global or address
     taken, and make a first pass at the callees.  */
  for (node = cgraph_nodes; node; node = node->next)
    if (node->analyzed && cgraph_is_master_clone (node))
      {
	tree decl = node->decl;
	enum availability avail = cgraph_function_body_availability (node);
	var_ann_t ann;
	struct cgraph_edge *e;

	if (TREE_ADDRESSABLE (decl) || avail == AVAIL_NOT_AVAILABLE)
	  bitmap_set_bit (global_and_address_taken, node->uid);
	ann = get_var_ann (decl);
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
	  node = postorder[i];
	  for (e = node->callees; e; e = e->next_callee)
	    {
	      struct cgraph_node *cnode = e->callee;
	      var_ann_t ann;
	      if (!cnode->analyzed)
		continue;
	      ann = var_ann (node->decl);

	      if (cgraph_function_body_availability (cnode) == AVAIL_NOT_AVAILABLE)
		changed |= bitmap_ior_into (ann->callees,
					    global_and_address_taken);
	      changed |= bitmap_ior_into (ann->callees,
					  var_ann (cnode->decl)->callees);

	    }
	}
    }
  BITMAP_FREE (global_and_address_taken);
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
