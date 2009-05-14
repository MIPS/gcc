/* Propagate local profiles into IPA profiles.
   Copyright (C) 2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "timevar.h"
#include "tree-pass.h"
#include "cgraph.h"
#include "tree.h"

#define CGRAPH_NODE_FREQUENCY(node)  (DECL_STRUCT_FUNCTION((node)->decl)->function_frequency)

static bool
try_update (struct cgraph_node *node)
{
  bool maybe_unlikely_executed = true, maybe_executed_once = true;
  struct cgraph_edge *edge;
  if (node->local.externally_visible
      || DECL_STATIC_CONSTRUCTOR (node->decl)
      || DECL_STATIC_DESTRUCTOR (node->decl))
    return false;
  if (node->address_taken)
    return false;
  if (!node->analyzed)
    return false;
  if (CGRAPH_NODE_FREQUENCY (node) == FUNCTION_FREQUENCY_HOT)
    return false;
  if (CGRAPH_NODE_FREQUENCY (node) == FUNCTION_FREQUENCY_UNLIKELY_EXECUTED)
    return false;
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Processing %s\n", cgraph_node_name (node));
  for (edge = node->callers;
       edge && (maybe_unlikely_executed || maybe_executed_once);
       edge = edge->next_caller)
    {
      switch (CGRAPH_NODE_FREQUENCY (edge->caller))
        {
	case FUNCTION_FREQUENCY_UNLIKELY_EXECUTED:
	  break;
	case FUNCTION_FREQUENCY_EXECUTED_ONCE:
	  if (edge->frequency == 0)
	    break;
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  Called by %s that is executed once\n", cgraph_node_name (node));
	  maybe_unlikely_executed = false;
	  if (edge->loop_nest)
	    {
	      maybe_executed_once = false;
	      if (dump_file && (dump_flags & TDF_DETAILS))
	        fprintf (dump_file, "  Called in loop\n");
	    }
	  break;
	case FUNCTION_FREQUENCY_HOT:
	case FUNCTION_FREQUENCY_NORMAL:
	  if (!edge->frequency)
	    continue;
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  Called by %s that is normal or hot\n", cgraph_node_name (node));
	  maybe_unlikely_executed = false;
	  maybe_executed_once = false;
	  break;
	}
    }
   if (maybe_unlikely_executed)
     {
       CGRAPH_NODE_FREQUENCY (node) = FUNCTION_FREQUENCY_UNLIKELY_EXECUTED;
       if (dump_file)
         fprintf (dump_file, "Node %s promoted to unlikely executed.\n", cgraph_node_name (node));
       return true;
     }
   if (maybe_executed_once && CGRAPH_NODE_FREQUENCY (node) != FUNCTION_FREQUENCY_EXECUTED_ONCE)
     {
       CGRAPH_NODE_FREQUENCY (node) = FUNCTION_FREQUENCY_EXECUTED_ONCE;
       if (dump_file)
         fprintf (dump_file, "Node %s promoted to executed once.\n", cgraph_node_name (node));
       return true;
     }
   return false;
}

static unsigned int
propagate (void)
{
  struct cgraph_node **order = XCNEWVEC (struct cgraph_node *, cgraph_n_nodes);
  int order_pos;
  bool something_changed = true;
  int i;

  order_pos = cgraph_postorder (order);

  while (something_changed)
    {
      something_changed = false;
      for (i = order_pos - 1; i >= 0; i--)
        something_changed |= try_update (order[i]);
    }
  free (order);
  return 0;
}

static bool
gate_profile (void)
{
  return true;
}

struct ipa_opt_pass pass_ipa_profile =
{
 {
  IPA_PASS,
  "ipa-profile",				/* name */
  gate_profile,			/* gate */
  propagate,			        /* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_REFERENCE,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0                                     /* todo_flags_finish */
 },
 NULL,				        /* generate_summary */
 NULL,					/* write_summary */
 NULL,					/* read_summary */
 NULL,					/* function_read_summary */
 0,					/* TODOs */
 NULL,			                /* function_transform */
 NULL					/* variable_transform */
};
