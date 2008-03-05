/* Write the cgraph to a .o file.

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
#include "tree-vectorizer.h"
#include "timevar.h"
#include "dwarf2asm.h"
#include "dwarf2out.h"
#include "output.h"
#include "lto-cgraph.h"
#include "lto-section-out.h"
#include <ctype.h>


const char * LTO_cgraph_tag_names[LTO_cgraph_last_tag] = 
{"", "avail", "overwrite", "unavail", "edge"};


struct output_block
{
  enum lto_section_type section_type;
  struct lto_out_decl_state *decl_state;

  /* The stream that the main tree codes are written to.  */
  struct lto_output_stream *main_stream;

#ifdef LTO_STREAM_DEBUGGING
  /* The stream that contains the gimple debugging information.  */
  struct lto_output_stream *debug_main_stream;
#endif
};


/* Create the output block and return it.  */

static struct output_block *
create_output_block (void)
{
  struct output_block *ob = xcalloc (1, sizeof (struct output_block));
  ob->section_type = LTO_section_cgraph;
  ob->decl_state = lto_get_out_decl_state ();
  ob->main_stream = xcalloc (1, sizeof (struct lto_output_stream));

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.out = lto_debug_out_fun;
  lto_debug_context.indent = 0;
  lto_debug_context.tag_names = LTO_cgraph_tag_names;
#endif

  return ob;
}


/* Destroy the output block OB.  IS_FUNTION is true if this is for a
   function and false for a constructor.  */

static void
destroy_output_block (struct output_block * ob)
{
  free (ob->main_stream);
  LTO_CLEAR_DEBUGGING_STREAM (debug_main_stream);
  free (ob);
}


/* Output an unsigned LEB128 quantity to OB->main_stream.  */

static void
output_uleb128 (struct output_block *ob, unsigned HOST_WIDE_INT work)
{
  lto_output_uleb128_stream (ob->main_stream, work);
}


/* Output a signed LEB128 quantity to OB->main_stream.  */

static void
output_sleb128 (struct output_block *ob, HOST_WIDE_INT work)
{
  lto_output_sleb128_stream (ob->main_stream, work);
}


static void
output_fn_decl (struct output_block *ob, tree expr)
{
  unsigned int index;
  bool new;
  
  new = lto_output_decl_index (ob->main_stream, 
			       ob->decl_state->fn_decl_hash_table,
			       &ob->decl_state->next_fn_decl_index, 
			       expr, &index);
  if (new)
    VEC_safe_push (tree, heap, ob->decl_state->fn_decls, expr);
}


/* Output the cgraph EDGE to OB.  */

static void
output_edge (struct output_block *ob, struct cgraph_edge *edge)
{
  output_uleb128 (ob, LTO_cgraph_edge);
  LTO_DEBUG_INDENT (LTO_cgraph_edge);
  output_fn_decl (ob, edge->callee->decl);
  LTO_DEBUG_FN_NAME (edge->callee->decl);

  LTO_DEBUG_TOKEN ("stmt");
  output_uleb128 (ob, gimple_stmt_uid (edge->call_stmt));
  LTO_DEBUG_TOKEN ("count");
  output_uleb128 (ob, edge->count);
  LTO_DEBUG_TOKEN ("frequency");
  output_uleb128 (ob, edge->frequency);
  LTO_DEBUG_TOKEN ("loop_next");
  output_uleb128 (ob, edge->loop_nest);
  LTO_DEBUG_UNDENT();
}


/* Add FLAG onto the end of BASE.  */
 
static void
add_flag (unsigned int *base, unsigned int flag)
{
  *base = *base << 1;
  if (flag)
    *base |= 1;
}


/* Output the cgraph NODE to OB.  */

static void
output_node (struct output_block *ob, struct cgraph_node *node)
{
  unsigned int tag;
  unsigned int flags = 0;
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

  output_uleb128 (ob, tag);
  LTO_DEBUG_INDENT (tag);

  output_fn_decl (ob, node->decl);
  LTO_DEBUG_FN_NAME (node->decl);
  add_flag (&flags, node->local.local);
  add_flag (&flags, node->local.externally_visible);
  add_flag (&flags, node->local.finalized);
  add_flag (&flags, node->local.inlinable);
  add_flag (&flags, node->local.disregard_inline_limits);
  add_flag (&flags, node->local.redefined_extern_inline);
  add_flag (&flags, node->local.for_functions_valid);
  add_flag (&flags, node->local.vtable_method);

  LTO_DEBUG_TOKEN ("flags");
  output_uleb128 (ob, flags);

  if (tag != LTO_cgraph_unavail_node)
    {
      LTO_DEBUG_TOKEN ("stack_size");
      output_sleb128 (ob, node->local.estimated_self_stack_size);
      LTO_DEBUG_TOKEN ("self_insns");
      output_sleb128 (ob, node->local.self_insns);
    }

  LTO_DEBUG_UNDENT();

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 0);
#endif

  while (callees)
    {
      output_edge (ob, callees);
      callees = callees->next_callee;
    }

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 0);
#endif
}


/* Produce the section that holds the cgraph.  */

static void
produce_asm (struct output_block *ob)
{
  struct lto_cgraph_header header;
  section *section = lto_get_section (LTO_section_cgraph, NULL);

  memset (&header, 0, sizeof (struct lto_cgraph_header)); 

  /* The entire header is stream computed here.  */
  switch_to_section (section);
  
  /* Write the header which says how to decode the pieces of the
     t.  */
  header.lto_header.major_version = LTO_major_version;
  header.lto_header.minor_version = LTO_minor_version;
  header.lto_header.section_type = LTO_section_cgraph;
  
  header.compressed_size = 0;
  
  header.main_size = ob->main_stream->total_size;
#ifdef LTO_STREAM_DEBUGGING
  header.debug_main_size = ob->debug_main_stream->total_size;
#else
  header.debug_main_size = -1;
#endif

  assemble_string ((const char *)&header, 
		   sizeof (struct lto_cgraph_header));

  lto_write_stream (ob->main_stream);
#ifdef LTO_STREAM_DEBUGGING
  lto_write_stream (ob->debug_main_stream);
#endif
}


/* Output the cgraph.  */

static unsigned int
lto_output_cgraph (void)
{
  struct cgraph_node *node;
  section *saved_section = in_section;
  struct output_block *ob = create_output_block ();

  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);

  /* Turn off some DWARF2 bits.  */
  dwarf2_called_from_lto_p = true;

  for (node = cgraph_nodes; node; node = node->next)
    {
      output_node (ob, node);

#ifdef ENABLE_CHECKING
      /* Just a little sanity check to keep Honza honest.  At the
	 point where we stream out the functions there must only be
	 master_clone nodes or nodes that have no function bodies.  */

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
#endif
    }

  output_uleb128 (ob, 0);

  /* Create a section to hold the pickled output the cgraph.  */
  produce_asm (ob);

  destroy_output_block (ob);

  /* Put back the assembly section that was there before we started
     writing lto info.  */
  if (saved_section)
    switch_to_section (saved_section);

  dwarf2_called_from_lto_p = false;

  return 0;
}

struct tree_opt_pass pass_ipa_lto_cgraph_out =
{
  "lto_cgraph_out",	                /* name */
  gate_lto_out,			        /* gate */
  lto_output_cgraph,		        /* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_LTO_OUT,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,            			/* todo_flags_start */
  TODO_dump_func,                       /* todo_flags_finish */
  0					/* letter */
};

