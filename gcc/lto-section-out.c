/* LTO output code.
   Copyright (C) 2007, 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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
#include "lto-section.h"
#include "lto-section-out.h"
#include <ctype.h>


/* Returns a hash code for P.  */

hashval_t
lto_hash_decl_slot_node (const void *p)
{
  const struct lto_decl_slot *ds = (const struct lto_decl_slot *) p;
  return (hashval_t) DECL_UID (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

int
lto_eq_decl_slot_node (const void *p1, const void *p2)
{
  const struct lto_decl_slot *ds1 =
    (const struct lto_decl_slot *) p1;
  const struct lto_decl_slot *ds2 =
    (const struct lto_decl_slot *) p2;

  return DECL_UID (ds1->t) == DECL_UID (ds2->t);
}


/* Returns a hash code for P.  */

hashval_t
lto_hash_type_slot_node (const void *p)
{
  const struct lto_decl_slot *ds = (const struct lto_decl_slot *) p;
  return (hashval_t) TYPE_UID (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

int
lto_eq_type_slot_node (const void *p1, const void *p2)
{
  const struct lto_decl_slot *ds1 =
    (const struct lto_decl_slot *) p1;
  const struct lto_decl_slot *ds2 =
    (const struct lto_decl_slot *) p2;

  return TYPE_UID (ds1->t) == TYPE_UID (ds2->t);
}


/* Get a section name for a particular type or name.  The NAME field
   is only used if SECTION_TYPE is LTO_section_function_body or
   lto_static_initializer.  For all others it is ignored.  The callee
   of this function is responcible to free the returned name.  */
const char *
lto_get_section_name (enum lto_section_type section_type, const char * name)
{
  switch (section_type)
    {
    case LTO_section_function_body:
      return concat (LTO_SECTION_NAME_PREFIX, name, NULL);

    case LTO_section_static_initializer:
      return concat (LTO_SECTION_NAME_PREFIX, ".statics", NULL);

    case LTO_section_decls:
      return concat (LTO_SECTION_NAME_PREFIX, ".decls", NULL);

    case LTO_section_cgraph:
      return concat (LTO_SECTION_NAME_PREFIX, ".cgraph", NULL);

    default:
      gcc_unreachable ();
    }
}


/* Get a section for particular type or name.  The NAME field is only
   used if SECTION_TYPE is LTO_section_function_body or
   lto_static_initializer.  */
section *
lto_get_section (enum lto_section_type section_type, const char * name)
{
  const char *section_name = lto_get_section_name (section_type, name);
  section *section = get_section (section_name, SECTION_DEBUG, NULL);
  free ((char *)section_name);
  return section;
}


/*****************************************************************************/
/* Output routines shared by all of the serialization passes.                */
/*****************************************************************************/


/* Write all of the chars in OBS to the assembler.  Recycle the blocks
   in obs as this is being done.  */

void
lto_write_stream (struct lto_output_stream *obs)
{
  unsigned int block_size = 1024;
  unsigned int num_chars;
  struct lto_char_ptr_base *block;
  if (!obs->first_block)
    return;

  block = obs->first_block;
  while (block)
    {
      const char *base = ((char *)block) + sizeof (struct lto_char_ptr_base);
      struct lto_char_ptr_base *old_block = block;
      block = (struct lto_char_ptr_base *)block->ptr;
      /* If there is a next block, then this one is full, if there is
	 not a next block, then the left_in_block field says how many
	 chars there are in this block.  */
      num_chars = block_size - sizeof (struct lto_char_ptr_base);
      if (!block)
	num_chars = num_chars - obs->left_in_block;

      assemble_string (base, num_chars);
      free (old_block);
      block_size *= 2;
    }
}


/* Write a character to the output block.  */

void
lto_output_1_stream (struct lto_output_stream *obs, char c)
{
  /* No space left.  */
  if (obs->left_in_block == 0)
    {
      struct lto_char_ptr_base *new_block;

      if (obs->first_block == NULL)
	{
	  /* This is the first time the stream has been written
	     into.  */
	  obs->block_size = 1024;
	  new_block = (struct lto_char_ptr_base*) xmalloc (obs->block_size);
	  obs->first_block = new_block;
	}
      else
	{
	  struct lto_char_ptr_base *tptr;
	  /* Get a new block that is twice as big as the last block
	     and link it into the list.  */
	  obs->block_size *= 2;
	  new_block = (struct lto_char_ptr_base*) xmalloc (obs->block_size);
	  /* The first bytes of the block are reserved as a pointer to
	     the next block.  Set the chain of the full block to the
	     pointer to the new block.  */
	  tptr = obs->current_block;
	  tptr->ptr = (char *)new_block;
	}

      /* Set the place for the next char at the first position after the
	 chain to the next block.  */
      obs->current_pointer
	= ((char *)new_block) + sizeof (struct lto_char_ptr_base);
      obs->current_block = new_block;
      /* Null out the newly allocated block's pointer to the next block.  */
      new_block->ptr = NULL;
      obs->left_in_block = obs->block_size - sizeof (struct lto_char_ptr_base);
    }

  /* Write the actual character.  */
  *obs->current_pointer = c;
  obs->current_pointer++;
  obs->total_size++;
  obs->left_in_block--;
}


/* Output an unsigned LEB128 quantity to OBS.  */

void
lto_output_uleb128_stream (struct lto_output_stream *obs, unsigned HOST_WIDE_INT work)
{
  LTO_DEBUG_WIDE ("U", work);
  do
    {
      unsigned int byte = (work & 0x7f);
      work >>= 7;
      if (work != 0)
	/* More bytes to follow.  */
	byte |= 0x80;

      lto_output_1_stream (obs, byte);
    }
  while (work != 0);
}

/* Identical to output_uleb128_stream above except using unsigned 
   HOST_WIDEST_INT type.  For efficiency on host where unsigned HOST_WIDEST_INT
   is not native, we only use this if we know that HOST_WIDE_INT is not wide
   enough.  */

void
lto_output_widest_uint_uleb128_stream (struct lto_output_stream *obs,
				       unsigned HOST_WIDEST_INT work)
{
  LTO_DEBUG_WIDE ("U", work);
  do
    {
      unsigned int byte = (work & 0x7f);
      work >>= 7;
      if (work != 0)
	/* More bytes to follow.  */
	byte |= 0x80;

      lto_output_1_stream (obs, byte);
    }
  while (work != 0);
}


/* Output a signed LEB128 quantity.  */

void
lto_output_sleb128_stream (struct lto_output_stream *obs, HOST_WIDE_INT work)
{
  int more, byte;
  LTO_DEBUG_WIDE ("S", work);
  do
    {
      byte = (work & 0x7f);
      /* arithmetic shift */
      work >>= 7;
      more = !((work == 0 && (byte & 0x40) == 0)
	       || (work == -1 && (byte & 0x40) != 0));
      if (more)
	byte |= 0x80;

      lto_output_1_stream (obs, byte);
    }
  while (more);
}


/* Put out a integer constant.  These are stored as two HOST_WIDE_INTS
   so games may have to be played to shift the data from the high to
   the low value.  */

void
lto_output_integer_stream (struct lto_output_stream *obs, tree t)
{
  HOST_WIDE_INT low = TREE_INT_CST_LOW (t);
  HOST_WIDE_INT high = TREE_INT_CST_HIGH (t);
  int more, byte;

  /* Of course if the high value is just sign bits for the signed low
     value, we can just punt and call lto_output_sleb128 and be done with
     it.  */
  if (((high == -1) && (low < 0))
      || ((high == 0) && (low >= 0)))
    {
      lto_output_sleb128_stream (obs, low);
      return;
    }

  LTO_DEBUG_INTEGER ("SS", high, low);

  /* This is just a copy of the lto_output_sleb128 code with extra
     operations to transfer the low 7 bits of the high value to the
     top 7 bits of the low value, shift the high down by 7 and then do
     a slightly more complex exit test.  */
  do
    {
      unsigned HOST_WIDE_INT transfer = (high & 0x7f);
      high = ((unsigned HOST_WIDE_INT) high) >> 7;
      transfer <<= (HOST_BITS_PER_WIDE_INT - 7);

      byte = (low & 0x7f);

      /* Logical shift.  */
      low = ((unsigned HOST_WIDE_INT) low) >> 7;
      low |= transfer;
      more = !((high == 0 && low == 0 && (byte & 0x40) == 0)
	       || (high == -1 && low == -1 && (byte & 0x40) != 0));
      if (more)
	byte |= 0x80;

      lto_output_1_stream (obs, byte);
    }
  while (more);
}


/* Lookup NAME in TABLE.  If NAME is not found, create a new entry in
   TABLE for NAME with NEXT_INDEX and increment NEXT_INDEX.  Then
   print the index to OBS.  True is returned if NAME was added to the
   table.  The resulting index is store in THIS_INDEX.

   If OBS is NULL, the only action is to add NAME to the table. */

bool
lto_output_decl_index (struct lto_output_stream * obs, htab_t table,
		       unsigned int *next_index, tree name, 
		       unsigned int *this_index)
{
  void **slot;
  struct lto_decl_slot d_slot;
  d_slot.t = name;

  slot = htab_find_slot (table, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct lto_decl_slot *new_slot = xmalloc (sizeof (struct lto_decl_slot));
      int index = (*next_index)++;

      new_slot->t = name;
      new_slot->slot_num = index;
      *this_index = index;
      *slot = new_slot;
      if (obs)
	lto_output_uleb128_stream (obs, index);
      return true;
    }
  else
    {
      struct lto_decl_slot *old_slot = (struct lto_decl_slot *)*slot;
      *this_index = old_slot->slot_num;
      if (obs)
	lto_output_uleb128_stream (obs, old_slot->slot_num);
      return false;
    }
}


/*****************************************************************************/
/*  This part is used to store all of the global decls and types that are    */
/*  serialized out in this file so that a table for this file can be built   */
/*  that allows the decls and types to be reconnected to the code or the     */
/*  ipa summary information.                                                 */ 
/*****************************************************************************/
static struct lto_out_decl_state *out_state;

struct lto_out_decl_state *
lto_get_out_decl_state (void)
{
  if (!out_state)
    {
      out_state = xcalloc (1, sizeof (struct lto_out_decl_state));

      out_state->field_decl_hash_table
	= htab_create (37, lto_hash_decl_slot_node, lto_eq_decl_slot_node, free);
      out_state->fn_decl_hash_table
	= htab_create (37, lto_hash_decl_slot_node, lto_eq_decl_slot_node, free);
      out_state->type_hash_table
	= htab_create (37, lto_hash_type_slot_node, lto_eq_type_slot_node, free);
      out_state->type_decl_hash_table
	= htab_create (37, lto_hash_decl_slot_node, lto_eq_decl_slot_node, free);
      out_state->var_decl_hash_table
	= htab_create (37, lto_hash_decl_slot_node, lto_eq_decl_slot_node, free);
    }
  return out_state;
}

/* Write the references for the objects in V to section SEC in the
   assembly file.  Use REF_FN to compute the reference.  */

static void
write_references (VEC(tree,heap) *v, section *sec,
		  void (*ref_fn) (tree, lto_out_ref *))
{
  int index;
  tree t;
  lto_out_ref out_ref = {0, NULL, NULL};

  for (index = 0; VEC_iterate(tree, v, index, t); index++)
    {
      ref_fn (t, &out_ref);
      /* We always call switch_to_section as the act of creating a
	 handle we can reference may have dumped some bits into the
	 assembly.  */
      switch_to_section (sec);
      dw2_asm_output_data (8, out_ref.section, " ");
      dw2_asm_output_delta (8, out_ref.label, out_ref.base_label, " ");
    }
}

/* This pass is run after all of the functions are serialized and all
   of the ipa passes have written their serialized forms.  This pass
   cases the vector of all of the global decls and types used from
   this file to be written in to a section that can then be read in to
   recover these on other side.  */
static unsigned int
produce_asm_for_decls (void)
{
  struct lto_decl_header header;
  section *section = lto_get_section (LTO_section_decls, NULL);
  struct lto_out_decl_state *out_state = lto_get_out_decl_state ();

  memset (&header, 0, sizeof (struct lto_decl_header)); 

  /* The entire header is stream computed here.  */
  switch_to_section (section);
  
  header.lto_header.major_version = LTO_major_version;
  header.lto_header.minor_version = LTO_minor_version;
  header.lto_header.section_type = LTO_section_decls;

  header.num_field_decls = VEC_length (tree, out_state->field_decls);
  header.num_fn_decls = VEC_length (tree, out_state->fn_decls);
  header.num_var_decls = VEC_length (tree, out_state->var_decls);
  header.num_type_decls = VEC_length (tree, out_state->type_decls);
  header.num_types = VEC_length (tree, out_state->types);

  assemble_string ((const char *)&header, 
		   sizeof (struct lto_decl_header));

  /* Write the global field references.  */
  write_references (out_state->field_decls, section, lto_field_ref);

  /* Write the global function references.  */
  write_references (out_state->fn_decls, section, lto_fn_ref);

  /* Write the global var references.  */
  write_references (out_state->var_decls, section, lto_var_ref);

  /* Write the global type_decl references.  */
  write_references (out_state->type_decls, section, lto_typedecl_ref);

  /* Write the global type references.  */
  write_references (out_state->types, section, lto_type_ref);

  htab_delete (out_state->field_decl_hash_table);
  htab_delete (out_state->fn_decl_hash_table);
  htab_delete (out_state->var_decl_hash_table);
  htab_delete (out_state->type_decl_hash_table);
  htab_delete (out_state->type_hash_table);

  VEC_free (tree, heap, out_state->field_decls);
  VEC_free (tree, heap, out_state->fn_decls);
  VEC_free (tree, heap, out_state->var_decls);
  VEC_free (tree, heap, out_state->type_decls);
  VEC_free (tree, heap, out_state->types);

  free (out_state);

  return 0;
}


/* Gate function for all lto streaming passes.  */
bool
gate_lto_out (void)
{
  return (flag_generate_lto
	  /* Don't bother doing anything if the program has errors.  */
	  && !(errorcount || sorrycount));
}


struct tree_opt_pass pass_ipa_lto_finish_out =
{
  "lto_decls_out",	                /* name */
  gate_lto_out,			        /* gate */
  produce_asm_for_decls,        	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_LTO_OUT,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,            			/* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0					/* letter */
};



#ifdef LTO_STREAM_DEBUGGING

struct lto_debug_context lto_debug_context;

/* The low level output routine to print a single character to the
   debugging stream.  */

void
lto_debug_out_fun (struct lto_debug_context *context, char c)
{
  struct lto_output_stream * stream 
    = (struct lto_output_stream *)context->current_data;
  lto_output_1_stream (stream, c);
}

#endif
