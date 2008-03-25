/* Read the cgraph from the memory mapped representation of a a .o file.

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
#include "tree-iterator.h"
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
#include "lto-cgraph.h"
#include "lto.h"
#include "lto-section-in.h"
#include <ctype.h>
#include "cpplib.h"

struct data_in
{
  /* That global decls and types.  */
  struct lto_file_decl_data* file_data;
};


/* Return 0 or 1 based on the last bit of FLAGS and right shift FLAGS
   by 1.  */

static unsigned int
add_flag (unsigned int *flags)
{
  unsigned int result = *flags & 1;
  *flags = *flags >> 1;
  return result;
}

/* Overwrite the information in NODE based on DATA_IN, TAG, FLAGS,
   STACK_SIZE and SELF_INSNS.  This is called either to initialize
   NODE or to replace the values in it, for instance becasue the first
   time we saw it, the function body was not available but now it
   is.  */
static void
overwrite_node (struct data_in *data_in,
		struct cgraph_node *node,
		enum LTO_cgraph_tags tag,
		unsigned int flags,
		unsigned int stack_size,
		unsigned int self_insns)
{
  node->aux = (void *)tag;
  node->local.estimated_self_stack_size = stack_size;
  node->local.self_insns = self_insns;
  if (!node->local.lto_file_data)
    node->local.lto_file_data = xcalloc (1, sizeof (struct lto_file_decl_data));
  
  node->local.lto_file_data = data_in->file_data;

  /* This list must be in the reverse order that they are set in
     lto-section-out.c:outout_node.  */
  node->local.vtable_method = add_flag (&flags);
  node->local.for_functions_valid = add_flag (&flags);
  node->local.redefined_extern_inline = add_flag (&flags);
  node->local.disregard_inline_limits = add_flag (&flags);
  node->local.inlinable = add_flag (&flags);
  node->local.finalized = add_flag (&flags);
  node->local.externally_visible = add_flag (&flags);
  node->local.local = add_flag (&flags);
  node->analyzed = node->local.finalized;
    node->lowered = node->local.finalized;
  if (cgraph_decide_is_function_needed (node))
    cgraph_mark_needed_node (node);
}


/* Input a cgraph from IB using the info in DATA_IN.  */

static void
input_cgraph (struct lto_input_block *ib,
	      struct data_in *data_in)
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

	  callee_decl = data_in->file_data->fn_decls [lto_input_uleb128 (ib)];
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
	  
	  edge = cgraph_create_edge (last_caller, callee, NULL, count, freq, nest);
	  edge->lto_stmt_uid = stmt_id;
	}
      else 
	{
	  tree fn_decl;
	  struct cgraph_node *node;
	  unsigned int flags;
	  int stack_size = 0;
	  int self_insns = 0;
	  
	  fn_decl = data_in->file_data->fn_decls [lto_input_uleb128 (ib)];
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
	      overwrite_node (data_in, node, tag, flags, stack_size, self_insns);
	      break;
	      
	    case LTO_cgraph_unavail_node:
	      /* We only overwrite the node if this is a brand new node.  */
	      if (!node->aux)
		overwrite_node (data_in, node, tag, flags, stack_size, self_insns);
	      break;
	      
	    case LTO_cgraph_overwritable_node:
	      /* FIXME!!!!  This code is written to take the last
		 overwrittable version.  I do not speak linker but if the
		 linker supposed to take the first one, then we need to
		 change the test.  */
	      if (((enum LTO_cgraph_tags)(node->aux)) != LTO_cgraph_avail_node)
		overwrite_node (data_in, node, tag, flags, stack_size, self_insns);
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


/* Read the body form DATA using the symbols in FILE_DATA to
   reconstruct the part of the cgraph associated with FILE_DATA.  */

void 
lto_input_cgraph (struct lto_file_decl_data* file_data, const void *data)
{
  struct lto_cgraph_header * header 
    = (struct lto_cgraph_header *) data;
  struct data_in data_in;
  
  int32_t main_offset = sizeof (struct lto_cgraph_header); 
#ifdef LTO_STREAM_DEBUGGING
  int32_t debug_main_offset = main_offset + header->main_size;
  struct lto_input_block debug_main 
    = {data + debug_main_offset, 0, header->debug_main_size};
#endif
  
  struct lto_input_block ib_main 
    = {data + main_offset, 0, header->main_size};
  
  memset (&data_in, 0, sizeof (struct data_in));
  data_in.file_data          = file_data;
  
#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.current_data = &debug_main;
  lto_debug_context.indent = 0;
  lto_debug_context.stream_name = "cgraph";
  lto_debug_context.tag_names = LTO_cgraph_tag_names;
#endif
  /* Read in the cgraph for this file and merge it into the main
     one.  */
  input_cgraph (&ib_main, &data_in);
}

