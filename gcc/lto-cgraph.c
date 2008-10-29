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
#include "pointer-set.h"
#include "lto-tree-in.h"
#include <ctype.h>

/* Call-Graph Streamer.

 Originally, the call graph streamer did not handle clone nodes created
 during the IPA-inlining.  The clones complicate things because:

 1. We must write also the master clones.
 2. We must write the master clones before writing the non-master clones
    because non-master clones points to the master clone so they cannot
    exist before the master does.
    
 The streaming algorithms are now changed before of the two constraints.

 For streaming out, we first write out all nodes in the call-cgraph node
 set without the edges.  We take care so that master clones are written
 first.  Then we write out all edges, using the UID of nodes as references.

 For streaming in, we first read in all nodes and then the edges.  We
 keep a mapping of the ordering numbers to the nodes read.  The mapping
 is used to decode references to ndoes in the stream.
 */

struct lto_cgraph_encoder_def
{
  struct pointer_map_t *map;		/* Map nodes to reference number. */
  VEC(cgraph_node_ptr,heap) *nodes;	/* Map reference number to node. */
};
typedef struct lto_cgraph_encoder_def *lto_cgraph_encoder_t;

/* Create a new encoder.  */

static lto_cgraph_encoder_t
lto_cgraph_encoder_new (void)
{
  lto_cgraph_encoder_t encoder = XNEW (struct lto_cgraph_encoder_def);
  encoder->map = pointer_map_create ();
  encoder->nodes = NULL;
  return encoder;
}

/* Delete ENCODER and its components.  */

static void
lto_cgraph_encoder_delete (lto_cgraph_encoder_t encoder)
{
   VEC_free (cgraph_node_ptr, heap, encoder->nodes);
   pointer_map_destroy (encoder->map);
   free (encoder);
}

/* Return the existing reference number of NODE in ENCODER or assign one
   if NODE has not been seen.  */

static void
lto_cgraph_encoder_encode (lto_cgraph_encoder_t encoder,
			   struct cgraph_node *node)
{
  void **slot = pointer_map_contains (encoder->map, node);

  if (!slot)
    {
      intptr_t ref = VEC_length (cgraph_node_ptr, encoder->nodes);
      slot = pointer_map_insert (encoder->map, node);
      *slot = (void *) ref;
      VEC_safe_push (cgraph_node_ptr, heap, encoder->nodes, node);
    }
}

/* Look up NODE in encoder.  Return NODE's reference if it has been encoded
   or LCC_NOT_FOUND if it is not there.  */

static intptr_t
lto_cgraph_encoder_lookup (lto_cgraph_encoder_t encoder,
			   struct cgraph_node *node)
{
  void **slot = pointer_map_contains (encoder->map, node);

  return (slot ? (intptr_t) *slot : LCC_NOT_FOUND);
}

/* Return the cgraph node corresponding to REF using ENCODER. */

static struct cgraph_node *
lto_cgraph_encoder_deref (lto_cgraph_encoder_t encoder, int ref)
{
  if (ref == LCC_NOT_FOUND)
    return NULL;

  return VEC_index (cgraph_node_ptr, encoder->nodes, ref); 
}

/* Return number of encoded nodes in ENCODER.  */

static int
lto_cgraph_encoder_size (lto_cgraph_encoder_t encoder)
{
  return VEC_length (cgraph_node_ptr, encoder->nodes);
}

#ifdef LTO_STREAM_DEBUGGING
const char * LTO_cgraph_tag_names[LTO_cgraph_last_tag] = 
{"", "avail", "overwrite", "unavail", "edge"};
#endif


/* Output the cgraph EDGE to OB using ENCODER.  */

static void
output_edge (struct lto_simple_output_block *ob,
	     struct cgraph_edge *edge, lto_cgraph_encoder_t encoder)
{
  unsigned int uid;
  intptr_t ref;
  unsigned HOST_WIDEST_INT flags = 0;

  lto_output_uleb128_stream (ob->main_stream, LTO_cgraph_edge);
  LTO_DEBUG_INDENT (LTO_cgraph_edge);

  LTO_DEBUG_TOKEN ("caller");
  ref = lto_cgraph_encoder_lookup (encoder, edge->caller);
  gcc_assert (ref != LCC_NOT_FOUND); 
  lto_output_sleb128_stream (ob->main_stream, ref);

  LTO_DEBUG_TOKEN ("callee");
  ref = lto_cgraph_encoder_lookup (encoder, edge->callee);
  gcc_assert (ref != LCC_NOT_FOUND); 
  lto_output_sleb128_stream (ob->main_stream, ref);

  LTO_DEBUG_TOKEN ("stmt");
  uid = flag_wpa ? edge->lto_stmt_uid : gimple_uid (edge->call_stmt);
  lto_output_uleb128_stream (ob->main_stream, uid);
  LTO_DEBUG_TOKEN ("inline_failed");
  lto_output_uleb128_stream (ob->main_stream, edge->inline_failed);
  LTO_DEBUG_TOKEN ("count");
  lto_output_uleb128_stream (ob->main_stream, edge->count);
  LTO_DEBUG_TOKEN ("frequency");
  lto_output_uleb128_stream (ob->main_stream, edge->frequency);
  LTO_DEBUG_TOKEN ("loop_next");
  lto_output_uleb128_stream (ob->main_stream, edge->loop_nest);
  LTO_DEBUG_TOKEN ("flags");
  lto_set_flag (&flags, edge->indirect_call);
  lto_set_flag (&flags, edge->call_stmt_cannot_inline_p);
  lto_output_uleb128_stream (ob->main_stream, flags);
  LTO_DEBUG_UNDENT();
}


/* Output the cgraph NODE to OB.  If BOUNDARY_P is true, NODE is a boundary
   of a cgraph_node_set and we pretend NODE just have a decl and no callees.
   WRITTEN_NODES are nodes written so far.  */

static void
output_node (struct lto_simple_output_block *ob, struct cgraph_node *node,
	     lto_cgraph_encoder_t encoder, cgraph_node_set set)
{
  unsigned int tag;
  unsigned HOST_WIDEST_INT flags = 0;
  struct cgraph_node *master_clone = node->master_clone;
  unsigned needed, local, externally_visible, inlinable;
  bool boundary_p = !cgraph_node_in_set_p (node, set);
  bool clone_p = node->master_clone && node != node->master_clone;

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
    {
      gcc_assert (!clone_p);
      tag = LTO_cgraph_unavail_node;
    }

  lto_output_uleb128_stream (ob->main_stream, tag);
  LTO_DEBUG_INDENT (tag);

  needed = node->needed;
  local = node->local.local;
  externally_visible = node->local.externally_visible;
  inlinable = node->local.inlinable;

  /* In the WPA mode, we only output part of the call-graph.  Also, we
     fake cgraph node attributes.  There are two cases that we care.

     Boundary nodes: There are nodes that are not part of SET but are
     called from within SET.  We artificially make them look like externally
     visible nodes with no function body. 

     Cherry-pikced nodes:  These are nodes we pull from other translational
     units into SET during IPA-inlining.  We make them as local static
     nodes and suppress the output of the master clone.  */

  if (boundary_p)
    {
      needed = 0;
      local = 0;
      externally_visible = 1;
      inlinable = 0;
    }
  else if (lto_forced_static_inline_p (node->decl))
    {
      needed = 0;
      local = 1;
      externally_visible = 0;
      inlinable = 1;
    }

  LTO_DEBUG_TOKEN ("clone_p");
  lto_output_uleb128_stream (ob->main_stream, clone_p);

  if (clone_p)
    {
      intptr_t ref = lto_cgraph_encoder_lookup (encoder, master_clone);
      LTO_DEBUG_TOKEN ("master");
      gcc_assert (ref != LCC_NOT_FOUND);
      lto_output_sleb128_stream (ob->main_stream, ref);
    }
  else
    { 
      lto_output_fn_decl_index (ob->decl_state, ob->main_stream, node->decl);
      LTO_DEBUG_FN_NAME (node->decl);
    }

  lto_set_flag (&flags, needed);
  lto_set_flag (&flags, local);
  lto_set_flag (&flags, externally_visible);
  lto_set_flag (&flags, node->local.finalized);
  lto_set_flag (&flags, inlinable);
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

  /* For WPA write also additional inlining information. */
  if (flag_wpa)
    {
      intptr_t ref;

      LTO_DEBUG_TOKEN ("estimated_stack_size");
      lto_output_sleb128_stream (ob->main_stream, 
				 node->global.estimated_stack_size);
      LTO_DEBUG_TOKEN ("stack_frame_offset");
      lto_output_sleb128_stream (ob->main_stream, 
				 node->global.stack_frame_offset);
      LTO_DEBUG_TOKEN ("inlined_to");
      if (node->global.inlined_to)
	{
	  ref = lto_cgraph_encoder_lookup (encoder, node->global.inlined_to);
	  gcc_assert (ref != LCC_NOT_FOUND);
	}
      else
	ref = LCC_NOT_FOUND;
      lto_output_sleb128_stream (ob->main_stream, ref); 

      LTO_DEBUG_TOKEN ("insns");
      lto_output_sleb128_stream (ob->main_stream, node->global.insns);
      LTO_DEBUG_TOKEN ("estimated_growth");
      lto_output_sleb128_stream (ob->main_stream,
				 node->global.estimated_growth);
      LTO_DEBUG_TOKEN ("inlined");
      lto_output_uleb128_stream (ob->main_stream, node->global.inlined);
    }
  
  LTO_DEBUG_UNDENT();

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 0);
#endif

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 0);
#endif
}

#ifdef ENABLE_CHECKING
/* Just a little sanity check to keep the cgraph machinery.  At the
   point where we stream out the functions there must only be
   master_clone nodes or nodes that have no function bodies.  */

static void
output_cgraph_verify_node (cgraph_node_set set, struct cgraph_node *node)
{
  struct cgraph_node *master_clone;
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
      master_clone = cgraph_master_clone (node, false);
      if  (node != master_clone
	   && !cgraph_node_in_set_p (master_clone, set))
	{
	  fprintf (stderr, "found clone with no master\n.");
	  gcc_assert (0);
	}
      break;
    }
}
#endif

/* Output the part of the cgraph in SET.  This is a little tricky now as we
   need to handle clones as well.  To simplify things we first write the nodes
   and then the edges.  This make the streamed data a bit larger.  */

static void
output_cgraph (cgraph_node_set set)
{
  struct cgraph_node *node, *master_clone;
  struct lto_simple_output_block *ob 
    = lto_create_simple_output_block (LTO_section_cgraph);
  cgraph_node_set_iterator csi;
  struct cgraph_edge *edge;
  lto_cgraph_encoder_t encoder = lto_cgraph_encoder_new ();
  int i, n_nodes;

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.tag_names = LTO_cgraph_tag_names;
  lto_debug_context.stream_name = "cgraph";
#endif

  /* Go over all the nodes in SET and assign references.  */
  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
      /* Make sure master clone appears before other clones.  */
      master_clone = node->master_clone;
      if (master_clone != NULL && master_clone != node) 
	{
	  gcc_assert (cgraph_node_in_set_p (master_clone, set));
	  lto_cgraph_encoder_encode (encoder, master_clone);
	}
	lto_cgraph_encoder_encode (encoder, node);
    }

  /* Go over all the nodes again to include callees that are not in SET.  */
  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
      for (edge = node->callees; edge; edge = edge->next_callee)
	lto_cgraph_encoder_encode (encoder, edge->callee);
    }

  /* Write out the nodes */
  n_nodes = lto_cgraph_encoder_size (encoder);
  for (i = 0; i < n_nodes; i++)
    {
      node = lto_cgraph_encoder_deref (encoder, i);
#ifdef ENABLE_CHECKING
      output_cgraph_verify_node (set, node);
#endif
      output_node (ob, node, encoder, set);
    }

  /* Go over the nodes in SET again to write edges.  */
  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
      for (edge = node->callees; edge; edge = edge->next_callee)
	output_edge (ob, edge, encoder);
    }

  lto_output_uleb128_stream (ob->main_stream, 0);

  /* Create a section to hold the pickled output the cgraph.  */
  lto_destroy_simple_output_block (ob);
  lto_cgraph_encoder_delete (encoder);
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
     output_node.  */
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
}

/* Read a node from input_block IB.  TAG is the node's tag just read. 
   Return the node read or overwriten.  NODES points to a vector of nodes
   read so far.  */
 
static struct cgraph_node *
input_node (struct lto_file_decl_data* file_data,
	    struct lto_input_block *ib,
	    enum LTO_cgraph_tags tag,
	    VEC(cgraph_node_ptr, heap) *nodes)
{
  tree fn_decl;
  struct cgraph_node *node, *master_clone;
  unsigned int flags;
  int stack_size = 0;
  int self_insns = 0;
  unsigned decl_index;
  bool clone_p;
  int estimated_stack_size = 0;
  int stack_frame_offset = 0;
  int ref = LCC_NOT_FOUND;
  int insns = 0;
  int estimated_growth = 0;
  bool inlined = false;

  LTO_DEBUG_TOKEN ("clone_p");
  clone_p = lto_input_uleb128 (ib);

  if (clone_p)
    {
      LTO_DEBUG_TOKEN ("master");
      master_clone = VEC_index (cgraph_node_ptr, nodes, lto_input_sleb128 (ib));
      gcc_assert (master_clone);
      node = cgraph_clone_input_node (master_clone);
    }
  else
    {
      decl_index = lto_input_uleb128 (ib);
      fn_decl = lto_file_decl_data_get_fn_decl (file_data, decl_index);
      LTO_DEBUG_FN_NAME (fn_decl);
      node = cgraph_node (fn_decl);
    }

  LTO_DEBUG_TOKEN ("flags");
  flags = lto_input_uleb128 (ib);
  
  if (tag == LTO_cgraph_avail_node)
    {
      LTO_DEBUG_TOKEN ("stack_size");
      stack_size = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("self_insns");
      self_insns = lto_input_sleb128 (ib);
    }
	  
  /* Read additional global data for LTRANS. */
  if (flag_ltrans)
    {
      LTO_DEBUG_TOKEN ("estimated_stack_size");
      estimated_stack_size = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("stack_frame_offset");
      stack_frame_offset = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("inlined_to");
      ref = lto_input_sleb128 (ib);

      LTO_DEBUG_TOKEN ("insns");
      insns = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("estimated_growth");
      estimated_growth = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("inlined");
      inlined = lto_input_uleb128 (ib);
    }

  gcc_assert (!node->aux);

  input_overwrite_node (file_data, node, tag, flags, stack_size,
			self_insns);
  if (flag_ltrans)
    {
      node->global.estimated_stack_size = estimated_stack_size;
      node->global.stack_frame_offset = stack_frame_offset;
      node->global.insns = insns;

      /* Store a reference for now, and fix up later to be a pointer.  */
      node->global.inlined_to = (cgraph_node_ptr) (intptr_t) ref;

      node->global.estimated_growth = estimated_growth;
      node->global.inlined = inlined;
    }

  return node;
}

/* Read an edge from IB.  NODES points to a vector of previously read
   nodes for decoding caller and callee of the edge to be read.  */

static void
input_edge (struct lto_input_block *ib, VEC(cgraph_node_ptr, heap) *nodes)
{
  struct cgraph_node *caller, *callee;
  struct cgraph_edge *edge;
  unsigned int stmt_id;
  unsigned int count;
  unsigned int freq;
  unsigned int nest;
  cgraph_inline_failed_t inline_failed;
  unsigned HOST_WIDEST_INT flags;
  tree prevailing;
  enum ld_plugin_symbol_resolution caller_resolution;

  LTO_DEBUG_TOKEN ("caller");
  caller = VEC_index (cgraph_node_ptr, nodes, lto_input_sleb128 (ib));
  gcc_assert (caller);
  gcc_assert (caller->decl);

  LTO_DEBUG_TOKEN ("callee");
  callee = VEC_index (cgraph_node_ptr, nodes, lto_input_sleb128 (ib));
  gcc_assert (callee);
  gcc_assert (callee->decl);

  caller_resolution = lto_symtab_get_resolution (caller->decl);

  /* FIXME lto: The following assert would currently fail for  extern inline
     functions. */

/*   gcc_assert (caller_resolution == LDPR_PREVAILING_DEF */
/* 	      || caller_resolution == LDPR_PREVAILING_DEF_IRONLY */
/* 	      || caller_resolution == LDPR_PREEMPTED_REG */
/* 	      || caller_resolution == LDPR_PREEMPTED_IR); */

  LTO_DEBUG_TOKEN ("stmt");
  stmt_id = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("inline_failed");
  inline_failed = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("count");
  count = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("frequency");
  freq = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("loop_next");
  nest = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("flags");
  flags = lto_input_uleb128 (ib);

  /* If the caller was preempted, don't create the edge. */
  if (caller_resolution == LDPR_PREEMPTED_REG
      || caller_resolution == LDPR_PREEMPTED_IR)
      return;

  /* Make sure the callee is the prevailing decl. */
  prevailing = lto_symtab_prevailing_decl (callee->decl);

  /* FIXME lto: remove this once extern inline in handled in lgen. */
  if (caller_resolution != LDPR_PREVAILING_DEF
      && caller_resolution != LDPR_PREVAILING_DEF_IRONLY
      && caller_resolution != LDPR_PREEMPTED_REG
      && caller_resolution != LDPR_PREEMPTED_IR)
    {
      /* If we have a extern inline, make sure it is the prevailing. */
      gcc_assert (prevailing == callee->decl);
    }

  if (prevailing != callee->decl)
    {
      /* We cannot replace a clone! */
      gcc_assert (callee == cgraph_node (callee->decl));


      callee = cgraph_node (prevailing);
      gcc_assert (callee);
    }

  edge = cgraph_create_edge (caller, callee, NULL, count, freq, nest);
  edge->lto_stmt_uid = stmt_id;
  edge->inline_failed = inline_failed;

  /* This list must be in the reverse order that they are set in
     output_edge.  */
  edge->call_stmt_cannot_inline_p = lto_get_flag (&flags);
  edge->indirect_call = lto_get_flag (&flags);
}

/* Input a cgraph from IB using the info in FILE_DATA.  */

static void
input_cgraph_1 (struct lto_file_decl_data* file_data,
		struct lto_input_block *ib)
{
  enum LTO_cgraph_tags tag;
  VEC(cgraph_node_ptr, heap) *nodes = NULL;
  struct cgraph_node *node;
  unsigned i;

  tag = lto_input_uleb128 (ib);
  while (tag)
    {
      LTO_DEBUG_INDENT (tag);

      if (tag == LTO_cgraph_edge)
        input_edge (ib, nodes);
      else 
	{
	  node = input_node (file_data, ib, tag, nodes);
	  gcc_assert (node);
	  gcc_assert (node->decl);
	  VEC_safe_push (cgraph_node_ptr, heap, nodes, node);
	}

      LTO_DEBUG_UNDENT();
      tag = lto_input_uleb128 (ib);
    }

  if (flag_ltrans)
    {
      for (i = 0; VEC_iterate (cgraph_node_ptr, nodes, i, node); i++)
        {
          const int ref = (int) (intptr_t) node->global.inlined_to;

          /* Fixup inlined_to from reference to pointer.  */
          if (ref != LCC_NOT_FOUND)
            node->global.inlined_to = VEC_index (cgraph_node_ptr, nodes, ref);
          else
            node->global.inlined_to = NULL;
        }
    }

  for (i = 0; VEC_iterate (cgraph_node_ptr, nodes, i, node); i++)
    {
      tree prevailing = lto_symtab_prevailing_decl (node->decl);

      if (prevailing != node->decl)
	{
	  cgraph_remove_node (node);
	  VEC_replace (cgraph_node_ptr, nodes, i, NULL);
	}
    }

  for (i = 0; VEC_iterate (cgraph_node_ptr, nodes, i, node); i++)
    if (node && cgraph_decide_is_function_needed (node))
      cgraph_mark_needed_node (node);

  VEC_free (cgraph_node_ptr, heap, nodes);
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

