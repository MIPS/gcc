/* Input functions for reading lto sections.

   Copyright 2006, 2007, 2008 Free Software Foundation, Inc.
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
#include "lto.h"
#include "lto-section.h"
#include <ctype.h>
#include "cpplib.h"

unsigned char 
lto_input_1_unsigned (struct lto_input_block *ib)
{
  gcc_assert (ib->p < ib->len);
  return (ib->data[ib->p++]);
}


/* Read an ULEB128 Number of IB.  */

unsigned HOST_WIDE_INT 
lto_input_uleb128 (struct lto_input_block *ib)
{
  unsigned HOST_WIDE_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  LTO_DEBUG_WIDE ("U", result);
	  return result;
	}
    }
}

/* HOST_WIDEST_INT version of lto_input_uleb128.  IB is as in lto_input_uleb128.  */

unsigned HOST_WIDEST_INT 
lto_input_widest_uint_uleb128 (struct lto_input_block *ib)
{
  unsigned HOST_WIDEST_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDEST_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  LTO_DEBUG_WIDE ("U", result);
	  return result;
	}
    }
}

/* Read an SLEB128 Number of IB.  */

HOST_WIDE_INT 
lto_input_sleb128 (struct lto_input_block *ib)
{
  HOST_WIDE_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  if ((shift < HOST_BITS_PER_WIDE_INT) && (byte & 0x40))
	    result |= - ((HOST_WIDE_INT)1 << shift);

	  LTO_DEBUG_WIDE ("S", result);
	  return result;
	}
    }
}


/* Input the next integer constant of TYPE in IB.  */

tree
lto_input_integer (struct lto_input_block *ib, tree type)
{
  HOST_WIDE_INT low = 0;
  HOST_WIDE_INT high = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = lto_input_1_unsigned (ib);
      if (shift < HOST_BITS_PER_WIDE_INT - 7)
	/* Working on the low part.  */
	low |= (byte & 0x7f) << shift;
      else if (shift >= HOST_BITS_PER_WIDE_INT)
	/* Working on the high part.  */
	high |= (byte & 0x7f) << (shift - HOST_BITS_PER_WIDE_INT);
      else
	{
	  /* Working on the transition between the low and high parts.  */
	  low |= (byte & 0x7f) << shift;
	  high |= (byte & 0x7f) >> (HOST_BITS_PER_WIDE_INT - shift);
	}

      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  if (byte & 0x40)
	    {
	      /* The number is negative.  */
	      if (shift < HOST_BITS_PER_WIDE_INT)
		{
		  low |= - ((HOST_WIDE_INT)1 << shift);
		  high = -1;
		}
	      else if (shift < (2 * HOST_BITS_PER_WIDE_INT))
		high |= - ((HOST_WIDE_INT)1 << (shift - HOST_BITS_PER_WIDE_INT));
	    }

#ifdef LTO_STREAM_DEBUGGING
	  /* Have to match the quick out in the lto writer.  */
	  if (((high == -1) && (low < 0))
	      || ((high == 0) && (low >= 0)))
	    LTO_DEBUG_WIDE ("S", low);
	  else 
	    LTO_DEBUG_INTEGER ("SS", high, low);
#endif	  
	  return build_int_cst_wide (type, low, high);
	}
    }
}
/*****************************************************************************/
/*  This part is used to recover all of the global decls and types that are  */
/*  serialized out so that a table for this file can be built.               */
/*  that allows the decls and types to be reconnected to the code or the     */
/*  ipa summary information.                                                 */ 
/*****************************************************************************/

/* Load in the global vars and all of the types from the main symbol
   table.  */

struct lto_file_decl_data*
lto_read_decls (lto_info_fd *fd,
		lto_context *context,
		const void *data)
{
  struct lto_decl_header * header 
    = (struct lto_decl_header *) data;
  struct lto_file_decl_data *data_in = xmalloc (sizeof (struct lto_file_decl_data));

  int32_t fields_offset = sizeof (struct lto_decl_header); 
  int32_t fns_offset 
    = fields_offset + (header->num_field_decls * sizeof (lto_ref));
  int32_t vars_offset 
    = fns_offset + (header->num_fn_decls * sizeof (lto_ref));
  int32_t type_decls_offset 
    = vars_offset + (header->num_var_decls * sizeof (lto_ref));
  int32_t types_offset 
    = type_decls_offset + (header->num_type_decls * sizeof (lto_ref));

  lto_ref *in_field_decls = (lto_ref*)(data + fields_offset);
  lto_ref *in_fn_decls    = (lto_ref*)(data + fns_offset);
  lto_ref *in_var_decls   = (lto_ref*)(data + vars_offset);
  lto_ref *in_type_decls  = (lto_ref*)(data + type_decls_offset);
  lto_ref *in_types       = (lto_ref*)(data + types_offset);
  int i;

  data_in->field_decls = xcalloc (header->num_field_decls, sizeof (tree*));
  data_in->fn_decls    = xcalloc (header->num_fn_decls, sizeof (tree*));
  data_in->var_decls   = xcalloc (header->num_var_decls, sizeof (tree*));
  data_in->type_decls  = xcalloc (header->num_type_decls, sizeof (tree*));
  data_in->types       = xcalloc (header->num_types, sizeof (tree*));

  for (i=0; i<header->num_field_decls; i++)
    data_in->field_decls[i] 
      = lto_resolve_field_ref (fd, context, &in_field_decls[i]);
  data_in->num_field_decls = header->num_field_decls;

  for (i=0; i<header->num_fn_decls; i++)
    data_in->fn_decls[i] 
      = lto_resolve_fn_ref (fd, context, &in_fn_decls[i]);
  data_in->num_fn_decls = header->num_fn_decls;

  for (i=0; i<header->num_var_decls; i++)
    data_in->var_decls[i] 
      = lto_resolve_var_ref (fd, context, &in_var_decls[i]);
  data_in->num_var_decls = header->num_var_decls;

  for (i=0; i<header->num_type_decls; i++)
    data_in->type_decls[i] 
      = lto_resolve_typedecl_ref (fd, context, &in_type_decls[i]);
  data_in->num_type_decls = header->num_type_decls;

  for (i=0; i<header->num_types; i++)
    data_in->types[i] = lto_resolve_type_ref (fd, context, &in_types[i]);
  data_in->num_types = header->num_types;

  return data_in;
}


/*****************************************************************************/
/* Stream debugging support code.                                            */
/*****************************************************************************/

#ifdef LTO_STREAM_DEBUGGING

/* Dump the debug STREAM, and two characters B and C.  */

void 
dump_debug_stream (struct lto_input_block *stream, char b, char c)
{
  unsigned int i = 0;
  bool new_line = true;
  int chars = 0;
  int hit_pos = -1;
  fprintf (stderr, 
	   "stream failure: looking for a '%c'[0x%x] in the debug stream.\nHowever the data translated into a '%c'[0x%x]at position%d\n\n",
	   c, c, b, b, stream->p);
  
  while (i < stream->len)
    {
      char x;
      
      if (new_line)
	{
	  if (hit_pos >= 0)
	    {
	      int j;
	      fprintf (stderr, "             ");
	      for (j=0; j<hit_pos; j++)
		fputc (' ', stderr);
	      fprintf (stderr, "^\n             ");
	      for (j=0; j<hit_pos; j++)
		fputc (' ', stderr);
	      fprintf (stderr, "|\n");
	      hit_pos = -1;
	    }
	  
	  fprintf (stderr, "%6d   -->>", i);
	  new_line = false;
	  chars = 0;
	}
      
      x = stream->data[i++];
      if (x == '\n')
	{
	  fprintf (stderr, "<<--\n");
	  new_line = true;
	}
      else 
	fputc (x, stderr);
      
      if (i == stream->p)
	hit_pos = chars;
      chars++;
    }
}


/* The low level output routine to for a single character.  Unlike the
   version on the writing side, this does interesting processing.

   This call checks that the debugging information generated by
   lto-function-out matches the debugging information generated by the
   reader. Each character is checked and a call to abort is generated
   when the first mismatch is found.  
  */

void
lto_debug_in_fun (struct lto_debug_context *context, char c)
{
  struct lto_input_block *stream = (struct lto_input_block *)context->current_data;
  char b = lto_input_1_unsigned (stream);

  if (b != c)
    {
      dump_debug_stream (stream, b, c);
      gcc_unreachable ();
    }
}
 
#endif
