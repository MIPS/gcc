/* Write and read the cgraph to the memory mapped representation of a
   .o file.

   Copyright 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"
#include "tree.h"
#include "expr.h"
#include "flags.h"
#include "params.h"
#include "input.h"
#include "varray.h"
#include "hashtab.h"
#include "langhooks.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "tree-flow.h"
#include "cgraph.h"
#include "function.h"
#include "ggc.h"
#include "diagnostic.h"
#include "except.h"
#include "debug.h"
#include "vec.h"
#include "timevar.h"
#include "dwarf2asm.h"
#include "dwarf2out.h"
#include "output.h"
#include "lto-section-in.h"
#include "lto-section-out.h"
#include <ctype.h>

enum LTO_cgraph_tags
{
  /* Must leave 0 for the stopper.  */
  LTO_cgraph_avail_node = 1,
  LTO_cgraph_overwritable_node,
  LTO_cgraph_unavail_node,
  LTO_cgraph_edge,
  LTO_cgraph_last_tag
};

#ifdef LTO_STREAM_DEBUGGING
static const char * LTO_cgraph_tag_names[LTO_cgraph_last_tag] = 
{"", "avail", "overwrite", "unavail", "edge"};
#endif


/* Output the cgraph EDGE to OB.  */

static void
output_edge (struct lto_simple_output_block *ob, struct cgraph_edge *edge)
{
  unsigned int uid;

  lto_output_uleb128_stream (ob->main_stream, LTO_cgraph_edge);
  LTO_DEBUG_INDENT (LTO_cgraph_edge);
  lto_output_fn_decl_index (ob->decl_state, ob->main_stream, edge->callee->decl);
  LTO_DEBUG_FN_NAME (edge->callee->decl);

  LTO_DEBUG_TOKEN ("stmt");
  uid = flag_wpa ? edge->lto_stmt_uid : gimple_uid (edge->call_stmt);
  lto_output_uleb128_stream (ob->main_stream, uid);
  LTO_DEBUG_TOKEN ("count");
  lto_output_uleb128_stream (ob->main_stream, edge->count);
  LTO_DEBUG_TOKEN ("frequency");
  lto_output_uleb128_stream (ob->main_stream, edge->frequency);
  LTO_DEBUG_TOKEN ("loop_next");
  lto_output_uleb128_stream (ob->main_stream, edge->loop_nest);
  LTO_DEBUG_TOKEN ("call_stmt_cannot_inline_p");
  lto_output_uleb128_stream (ob->main_stream, edge->call_stmt_cannot_inline_p);
  LTO_DEBUG_UNDENT();
}


/* Output the cgraph NODE to OB.  If BOUNDARY_P is true, NODE is a boundary
   of a cgraph_node_set and we pretend NODE just have a decl and no callees.
   */

static void
output_node (struct lto_simple_output_block *ob, struct cgraph_node *node,
	     bool boundary_p)
{
  unsigned int tag;
  unsigned HOST_WIDEST_INT flags = 0;
  struct cgraph_edge *callees = node->callees;

  switch (cgraph_function_body_availability (node))
    {
    case AVAIL_NOT_AVAILABLE:
      tag = LTO_cgraph_unavail_node;
      break;

    case AVAIL_AVAILABLE:
    case AVAIL_LOCAL:
      tag = LTO_cgraph_avail_node;
      break;
    
    case AVAIL_OVERWRITABLE:
      tag = LTO_cgraph_overwritable_node;
      break;
      
    default:
    gcc_unreachable();
    }
 
  if (boundary_p)
    tag = LTO_cgraph_unavail_node;

  lto_output_uleb128_stream (ob->main_stream, tag);
  LTO_DEBUG_INDENT (tag);

  lto_output_fn_decl_index (ob->decl_state, ob->main_stream, node->decl);
  LTO_DEBUG_FN_NAME (node->decl);
  lto_set_flag (&flags, node->needed);
  lto_set_flag (&flags, node->local.local);
  lto_set_flag (&flags, node->local.externally_visible);
  lto_set_flag (&flags, node->local.finalized);
  lto_set_flag (&flags, node->local.inlinable);
  lto_set_flag (&flags, node->local.disregard_inline_limits);
  lto_set_flag (&flags, node->local.redefined_extern_inline);
  lto_set_flag (&flags, node->local.for_functions_valid);
  lto_set_flag (&flags, node->local.vtable_method);

  LTO_DEBUG_TOKEN ("flags");
  lto_output_uleb128_stream (ob->main_stream, flags);

  if (tag != LTO_cgraph_unavail_node)
    {
      LTO_DEBUG_TOKEN ("stack_size");
      lto_output_sleb128_stream (ob->main_stream, 
				 node->local.inline_summary.estimated_self_stack_size);
      LTO_DEBUG_TOKEN ("self_insns");
      lto_output_sleb128_stream (ob->main_stream, 
				 node->local.inline_summary.self_insns);
    }

  LTO_DEBUG_UNDENT();

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 0);
#endif

  if (!boundary_p)
    while (callees)
      {
	output_edge (ob, callees);
	callees = callees->next_callee;
      }

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 0);
#endif
}

#ifdef ENABLE_CHECKING
/* Just a little sanity check to keep the cgraph machinery.  At the
   point where we stream out the functions there must only be
   master_clone nodes or nodes that have no function bodies.  */

static void
output_cgraph_verify_node (struct cgraph_node *node)
{
  switch (cgraph_function_body_availability (node))
    {
    case AVAIL_UNSET:
      fprintf (stderr, "found unset function\n.");
      gcc_assert (0);
      break;

    case AVAIL_NOT_AVAILABLE:
      break;

    case AVAIL_OVERWRITABLE:
    case AVAIL_AVAILABLE:
    case AVAIL_LOCAL:
      if  (node != cgraph_master_clone (node, false))
	{
	  fprintf (stderr, "found clone\n.");
	  gcc_assert (0);
	}
      break;
    }
}
#endif

/* Output the cgraph.  */

static void
output_cgraph (cgraph_node_set set)
{
  struct cgraph_node *node;
  struct lto_simple_output_block *ob 
    = lto_create_simple_output_block (LTO_section_cgraph);
  int i = 0;
  cgraph_node_set_iterator csi;
  cgraph_node_set boundary;
  struct cgraph_edge *callee;

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.tag_names = LTO_cgraph_tag_names;
  lto_debug_context.stream_name = "cgraph";
#endif

  /* Compute the boundary nodes. There are nodes not in SET but
     directly reachable from there. */
  boundary = cgraph_node_set_new ();
  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
      for (callee = node->callees; callee; callee = callee->next_callee)
	if (!cgraph_node_in_set_p (callee->callee, set))
	    cgraph_node_set_add (boundary, callee->callee);
    }

  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
#ifdef ENABLE_CHECKING
      output_cgraph_verify_node (node);
#endif
      output_node (ob, node, false /* boundary_p */);
      i++;
    }

  for (csi = csi_start (boundary); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
#ifdef ENABLE_CHECKING
      output_cgraph_verify_node (node);
#endif
      output_node (ob, node, true /* boundary_p */);
    }

  lto_output_uleb128_stream (ob->main_stream, 0);

  /* Create a section to hold the pickled output the cgraph.  */
  lto_destroy_simple_output_block (ob);
}


/* Overwrite the information in NODE based on FILE_DATA, TAG, FLAGS,
   STACK_SIZE and SELF_INSNS.  This is called either to initialize
   NODE or to replace the values in it, for instance becasue the first
   time we saw it, the function body was not available but now it
   is.  */
static void
input_overwrite_node (struct lto_file_decl_data* file_data,
		struct cgraph_node *node,
		enum LTO_cgraph_tags tag,
		unsigned HOST_WIDEST_INT flags,
		unsigned int stack_size,
		unsigned int self_insns)
{
  node->aux = (void *)tag;
  node->local.inline_summary.estimated_self_stack_size = stack_size;
  node->local.inline_summary.self_insns = self_insns;
  node->global.insns = self_insns;
  node->local.lto_file_data = file_data;

  /* This list must be in the reverse order that they are set in
     lto-section-out.c:outout_node.  */
  node->local.vtable_method = lto_get_flag (&flags);
  node->local.for_functions_valid = lto_get_flag (&flags);
  node->local.redefined_extern_inline = lto_get_flag (&flags);
  node->local.disregard_inline_limits = lto_get_flag (&flags);
  node->local.inlinable = lto_get_flag (&flags);
  node->local.finalized = lto_get_flag (&flags);
  node->local.externally_visible = lto_get_flag (&flags);
  node->local.local = lto_get_flag (&flags);
  node->needed = lto_get_flag (&flags);
  node->analyzed = node->local.finalized;
  node->lowered = node->local.finalized;
  if (cgraph_decide_is_function_needed (node))
    cgraph_mark_needed_node (node);
}


/* Input a cgraph from IB using the info in FILE_DATA.  */

static void
input_cgraph_1 (struct lto_file_decl_data* file_data,
		struct lto_input_block *ib)
{
  enum LTO_cgraph_tags tag;
  struct cgraph_node *last_caller = NULL;

  tag = lto_input_uleb128 (ib);
  while (tag)
    {
      LTO_DEBUG_INDENT (tag);

      if (tag == LTO_cgraph_edge)
	{
	  tree callee_decl;
	  struct cgraph_node *callee;
	  struct cgraph_edge *edge;
	  unsigned int stmt_id;
	  unsigned int count;
	  unsigned int freq;
	  unsigned int nest;
	  unsigned int call_stmt_cannot_inline_p;
	  unsigned int decl_idx = lto_input_uleb128 (ib);

	  callee_decl = lto_file_decl_data_get_fn_decl (file_data, decl_idx);
	  LTO_DEBUG_FN_NAME (callee_decl);
	  callee = cgraph_node (callee_decl);
	  
	  LTO_DEBUG_TOKEN ("stmt");
	  stmt_id = lto_input_uleb128 (ib);
	  LTO_DEBUG_TOKEN ("count");
	  count = lto_input_uleb128 (ib);
	  LTO_DEBUG_TOKEN ("frequency");
	  freq = lto_input_uleb128 (ib);
	  LTO_DEBUG_TOKEN ("loop_next");
	  nest = lto_input_uleb128 (ib);
	  LTO_DEBUG_TOKEN ("call_stmt_cannot_inline_p");
	  call_stmt_cannot_inline_p = lto_input_uleb128 (ib);
	  
	  edge = cgraph_create_edge (last_caller, callee, NULL, count,
				     freq, nest);
	  edge->lto_stmt_uid = stmt_id;
	  edge->call_stmt_cannot_inline_p = call_stmt_cannot_inline_p;
	}
      else 
	{
	  tree fn_decl;
	  struct cgraph_node *node;
	  unsigned int flags;
	  int stack_size = 0;
	  int self_insns = 0;
	  unsigned decl_index = lto_input_uleb128 (ib);
	  
	  fn_decl = lto_file_decl_data_get_fn_decl (file_data, decl_index);
	  LTO_DEBUG_FN_NAME (fn_decl);
	  LTO_DEBUG_TOKEN ("flags");
	  flags = lto_input_uleb128 (ib);
	  
	  if (tag == LTO_cgraph_avail_node)
	    {
	      LTO_DEBUG_TOKEN ("stack_size");
	      stack_size = lto_input_sleb128 (ib);
	      LTO_DEBUG_TOKEN ("self_insns");
	      self_insns = lto_input_sleb128 (ib);
	    }
	  
	  node = cgraph_node (fn_decl);
	  
	  switch (tag)
	    {
	    case LTO_cgraph_avail_node:
	      /* We cannot have two avail functions that are the same.  */
	      gcc_assert (((enum LTO_cgraph_tags)(node->aux)) != LTO_cgraph_avail_node);
	      input_overwrite_node (file_data, node, tag, 
				    flags, stack_size, self_insns);
	      break;
	      
	    case LTO_cgraph_unavail_node:
	      /* We only overwrite the node if this is a brand new node.  */
	      if (!node->aux)
		input_overwrite_node (file_data, node, tag, 
				      flags, stack_size, self_insns);
	      break;
	      
	    case LTO_cgraph_overwritable_node:
	      /* FIXME!!!!  This code is written to take the last
		 overwrittable version.  I do not speak linker but if the
		 linker supposed to take the first one, then we need to
		 change the test.  */
	      if (((enum LTO_cgraph_tags)(node->aux)) != LTO_cgraph_avail_node)
		input_overwrite_node (file_data, node, tag, 
				      flags, stack_size, self_insns);
	      break;
	      
	    default:
	      gcc_unreachable ();
	    }
	  
	  /* Set this up so that we can handle the edges which follow and
	     only have the callee in them.  */
	  last_caller = node;
	}
      LTO_DEBUG_UNDENT();
      tag = lto_input_uleb128 (ib);
    }
}


/* Input and merge the cgraph from each of the .o files passed to
   lto1.  */

static void
input_cgraph (void)
{
  struct lto_file_decl_data ** file_data_vec 
    = lto_get_file_decl_data ();
  struct lto_file_decl_data * file_data;
  unsigned int j = 0;
  struct cgraph_node *node;

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.tag_names = LTO_cgraph_tag_names;
  lto_debug_context.stream_name = "cgraph";
#endif

  while ((file_data = file_data_vec[j++]))
    {
      const char *data;
      size_t len;
      struct lto_input_block *ib
	= lto_create_simple_input_block (file_data, 
					 LTO_section_cgraph, 
					 &data, &len);
      input_cgraph_1 (file_data, ib);
      lto_destroy_simple_input_block (file_data, 
				      LTO_section_cgraph, 
				      ib, data, len);
    } 

  /* Clear out the aux field that was used to store enough state to
     tell which nodes should be overwritten.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      gcc_assert (node->local.lto_file_data);
      node->aux = NULL;
    }
}



struct ipa_opt_pass pass_ipa_lto_cgraph =
{
 {
  IPA_PASS,
  "lto_cgraph",	                        /* name */
  gate_lto_out,			        /* gate */
  NULL,		                        /* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_LTO_OUT,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,            			/* todo_flags_start */
  TODO_dump_func                        /* todo_flags_finish */
 },
 NULL,		                        /* generate_summary */
 output_cgraph,				/* write_summary */
 input_cgraph,				/* read_summary */
 NULL,					/* function_read_summary */
 0,					/* TODOs */
 NULL,			                /* function_transform */
 NULL					/* variable_transform */
};

