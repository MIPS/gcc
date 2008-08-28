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
#include "lto-section.h"
#include "lto-section-out.h"
#include "lto-tree-out.h"
#include "pointer-set.h"
#include "stdint.h"
#include "lto-symtab.h"
#include "bitmap.h"

static VEC(lto_out_decl_state_ptr, heap) *decl_state_stack;

/* List of out decl states used by functions.  We use this to
   generate the decl directory later. */

static VEC(lto_out_decl_state_ptr, heap) *function_decl_states;

/* Add FLAG onto the end of BASE.  */
 
void
lto_set_flag (unsigned HOST_WIDEST_INT *base, unsigned int flag)
{
  *base = *base << 1;
  if (flag)
    *base |= 1;
}

/* Add FLAGS of WIDTH onto the end of BASE.  */
 
void
lto_set_flags (unsigned HOST_WIDEST_INT *base, unsigned int flag, unsigned int width)
{
  unsigned HOST_WIDEST_INT mask = (((unsigned HOST_WIDEST_INT) 1) << width) - 1;

  *base = *base << width;
  *base |= (flag & mask);
}


/* Returns a hash code for P.  */

hashval_t
lto_hash_decl_slot_node (const void *p)
{
  const struct lto_decl_slot *ds = (const struct lto_decl_slot *) p;

  /*
    return (hashval_t) DECL_UID (ds->t);
  */
  return (hashval_t) TREE_HASH (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

int
lto_eq_decl_slot_node (const void *p1, const void *p2)
{
  const struct lto_decl_slot *ds1 =
    (const struct lto_decl_slot *) p1;
  const struct lto_decl_slot *ds2 =
    (const struct lto_decl_slot *) p2;

  /*
  return DECL_UID (ds1->t) == DECL_UID (ds2->t);
  */
  return ds1->t == ds2->t;
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


/* Returns a hash code for P.  */

hashval_t
lto_hash_global_slot_node (const void *p)
{
  const struct lto_decl_slot *ds = (const struct lto_decl_slot *) p;
  return (hashval_t) TREE_HASH (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

int
lto_eq_global_slot_node (const void *p1, const void *p2)
{
  const struct lto_decl_slot *ds1 =
    (const struct lto_decl_slot *) p1;
  const struct lto_decl_slot *ds2 =
    (const struct lto_decl_slot *) p2;

  return ds1->t == ds2->t;
}


/* Get a section name for a particular type or name.  The NAME field
   is only used if SECTION_TYPE is LTO_section_function_body or
   lto_static_initializer.  For all others it is ignored.  The callee
   of this function is responcible to free the returned name.  */

char *
lto_get_section_name (enum lto_section_type section_type, const char *name)
{
  switch (section_type)
    {
    case LTO_section_function_body:
      return concat (LTO_SECTION_NAME_PREFIX, name, NULL);

    case LTO_section_static_initializer:
      return concat (LTO_SECTION_NAME_PREFIX, ".statics", NULL);

    case LTO_section_symtab:
      return concat (LTO_SECTION_NAME_PREFIX, ".symtab", NULL);

    case LTO_section_decls:
      return concat (LTO_SECTION_NAME_PREFIX, ".decls", NULL);

    case LTO_section_cgraph:
      return concat (LTO_SECTION_NAME_PREFIX, ".cgraph", NULL);

    case LTO_section_ipa_pure_const:
      return concat (LTO_SECTION_NAME_PREFIX, ".pureconst", NULL);

    case LTO_section_ipa_reference:
      return concat (LTO_SECTION_NAME_PREFIX, ".reference", NULL);

    default:
      gcc_unreachable ();
    }
}


/*****************************************************************************
   Output routines shared by all of the serialization passes.
*****************************************************************************/


/* Begin a new output section named NAME.  */

void
lto_begin_section (const char *name)
{
  lang_hooks.lto.begin_section (name);
}


/* End the current output section.  */

void
lto_end_section (void)
{
  lang_hooks.lto.end_section ();
}


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

      lang_hooks.lto.append_data (base, num_chars, old_block);
      block_size *= 2;
    }
}


/* Adds a new block to output stream OBS.  */

static void
append_block (struct lto_output_stream *obs)
{
  struct lto_char_ptr_base *new_block;

  gcc_assert (obs->left_in_block == 0);

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
      tptr->ptr = (char *) new_block;
    }

  /* Set the place for the next char at the first position after the
     chain to the next block.  */
  obs->current_pointer
    = ((char *) new_block) + sizeof (struct lto_char_ptr_base);
  obs->current_block = new_block;
  /* Null out the newly allocated block's pointer to the next block.  */
  new_block->ptr = NULL;
  obs->left_in_block = obs->block_size - sizeof (struct lto_char_ptr_base);
}


/* Write a character to the output block.  */

void
lto_output_1_stream (struct lto_output_stream *obs, char c)
{
  /* No space left.  */
  if (obs->left_in_block == 0)
    append_block (obs);

  /* Write the actual character.  */
  *obs->current_pointer = c;
  obs->current_pointer++;
  obs->total_size++;
  obs->left_in_block--;
}


/* Write raw DATA of length LEN to the output block OB.  */

void
lto_output_data_stream (struct lto_output_stream *obs, const void *data,
			size_t len)
{
  while (len)
    {
      size_t copy;

      /* No space left.  */
      if (obs->left_in_block == 0)
	append_block (obs);

      /* Determine how many bytes to copy in this loop.  */
      if (len <= obs->left_in_block)
	copy = len;
      else
	copy = obs->left_in_block;

      /* Copy the data and do bookkeeping.  */
      memcpy (obs->current_pointer, data, copy);
      obs->current_pointer += copy;
      obs->total_size += copy;
      obs->left_in_block -= copy;
      data = (const char *) data + copy;
      len -= copy;
    }
}


/* Output an unsigned LEB128 quantity to OBS.  */

void
lto_output_uleb128_stream (struct lto_output_stream *obs,
			   unsigned HOST_WIDE_INT work)
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


/* Lookup NAME in ENCODER.  If NAME is not found, create a new entry in
   ENCODER for NAME with the next available index of ENCODER,  then
   print the index to OBS.  True is returned if NAME was added to
   ENCODER.  The resulting index is stored in THIS_INDEX.

   If OBS is NULL, the only action is to add NAME to the encoder. */

bool
lto_output_decl_index (struct lto_output_stream *obs,
		       struct lto_tree_ref_encoder *encoder,
		       tree name, unsigned int *this_index)
{
  void **slot;
  struct lto_decl_slot d_slot;
  int index;
  bool new_entry_p = FALSE;

  d_slot.t = name;
  slot = htab_find_slot (encoder->tree_hash_table, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct lto_decl_slot *new_slot
	= (struct lto_decl_slot *) xmalloc (sizeof (struct lto_decl_slot));
      index = encoder->next_index++;

      new_slot->t = name;
      new_slot->slot_num = index;
      *slot = new_slot;
      VEC_safe_push (tree, heap, encoder->trees, name);
      new_entry_p = TRUE;
    }
  else
    {
      struct lto_decl_slot *old_slot = (struct lto_decl_slot *)*slot;
      index = old_slot->slot_num;
    }

  if (obs)
    lto_output_uleb128_stream (obs, index);
  *this_index = index;
  return new_entry_p;
}

/* Output a field DECL to OBS.  */

void
lto_output_field_decl_index (struct lto_out_decl_state *decl_state,
			     struct lto_output_stream * obs, tree decl)
{
  unsigned int index;
  lto_output_decl_index (obs, &decl_state->streams[LTO_DECL_STREAM_FIELD_DECL],
			 decl, &index);
}

/* Output a function DECL to OBS.  */

void
lto_output_fn_decl_index (struct lto_out_decl_state *decl_state, 
			  struct lto_output_stream * obs, tree decl)
{
  unsigned int index;
  lto_output_decl_index (obs, &decl_state->streams[LTO_DECL_STREAM_FN_DECL],
			 decl, &index);
}

/* Output a namespace DECL to OBS.  */

void
lto_output_namespace_decl_index (struct lto_out_decl_state *decl_state,
				 struct lto_output_stream * obs, tree decl)
{
  unsigned int index;
  lto_output_decl_index (obs,
			 &decl_state->streams[LTO_DECL_STREAM_NAMESPACE_DECL],
			 decl, &index);
}

/* Output a static or extern var DECL to OBS.  */

void
lto_output_var_decl_index (struct lto_out_decl_state *decl_state,
			   struct lto_output_stream * obs, tree decl)
{
  unsigned int index;
  lto_output_decl_index (obs, &decl_state->streams[LTO_DECL_STREAM_VAR_DECL],
			 decl, &index);
}

/* Output a type DECL to OBS.  */

void
lto_output_type_decl_index (struct lto_out_decl_state *decl_state,
			    struct lto_output_stream * obs, tree decl)
{
  unsigned int index;
  lto_output_decl_index (obs, &decl_state->streams[LTO_DECL_STREAM_TYPE_DECL],
			 decl, &index);
}

/* Output a type REF to OBS.  */

void
lto_output_type_ref_index (struct lto_out_decl_state *decl_state,
			   struct lto_output_stream *obs, tree ref)
{
  unsigned int index;
  lto_output_decl_index (obs, &decl_state->streams[LTO_DECL_STREAM_TYPE],
			 ref, &index);
}


/*****************************************************************************
  Convenience routines used by the ipa passes to serialize their information.
*****************************************************************************/

/* Create the output block and return it.  */

struct lto_simple_output_block *
lto_create_simple_output_block (enum lto_section_type section_type)
{
  struct lto_simple_output_block *ob
    = ((struct lto_simple_output_block *)
       xcalloc (1, sizeof (struct lto_simple_output_block)));

  ob->section_type = section_type;
  ob->decl_state = lto_get_out_decl_state ();
  ob->main_stream = ((struct lto_output_stream *)
		     xcalloc (1, sizeof (struct lto_output_stream)));

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.out = lto_debug_out_fun;
  lto_debug_context.indent = 0;
#endif

  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);

  return ob;
}


/* Produce a simple section for one of the ipa passes.  */

void
lto_destroy_simple_output_block (struct lto_simple_output_block *ob)
{
  char *section_name;
  struct lto_simple_header header;
  struct lto_output_stream *header_stream;

  section_name = lto_get_section_name (ob->section_type, NULL);
  lto_begin_section (section_name);
  free (section_name);

  /* Write the header which says how to decode the pieces of the
     t.  */
  memset (&header, 0, sizeof (struct lto_simple_header));
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

  header_stream = ((struct lto_output_stream *)
		   xcalloc (1, sizeof (struct lto_output_stream)));
  lto_output_data_stream (header_stream, &header, sizeof header);
  lto_write_stream (header_stream);
  free (header_stream);

  lto_write_stream (ob->main_stream);
#ifdef LTO_STREAM_DEBUGGING
  lto_write_stream (ob->debug_main_stream);
#endif

  /* Put back the assembly section that was there before we started
     writing lto info.  */
  lto_end_section ();

  free (ob->main_stream);
  LTO_CLEAR_DEBUGGING_STREAM (debug_main_stream);
  free (ob);
}

/* Return a new lto_out_decl_state. */

struct lto_out_decl_state *
lto_new_out_decl_state (void)
{
  struct lto_out_decl_state *state = XCNEW (struct lto_out_decl_state);
  int i;
  htab_hash hash_fn;
  htab_eq eq_fn;

  for (i = 0; i < LTO_N_DECL_STREAMS; i++)
    {
      if (i == LTO_DECL_STREAM_TYPE)
	{
	  hash_fn = lto_hash_type_slot_node;
	  eq_fn = lto_eq_type_slot_node;
	}
      else
	{
	  hash_fn = lto_hash_decl_slot_node;
	  eq_fn = lto_eq_decl_slot_node;
	}
    lto_init_tree_ref_encoder (&state->streams[i], hash_fn, eq_fn);
    }

  return state;
}

/* Delete STATE and components.  */

void
lto_delete_out_decl_state (struct lto_out_decl_state *state)
{
  int i;

  for (i = 0; i < LTO_N_DECL_STREAMS; i++)
    lto_destroy_tree_ref_encoder (&state->streams[i]);

  free (state);
}

/*****************************************************************************
  This part is used to store all of the global decls and types that
  are serialized out in this file so that a table for this file can be
  built that allows the decls and types to be reconnected to the code
  or the ipa summary information.
*****************************************************************************/

/* Get the currently used lto_out_decl_state structure. */

struct lto_out_decl_state *
lto_get_out_decl_state (void)
{
  return VEC_last (lto_out_decl_state_ptr, decl_state_stack);
}

/* Push STATE to top of out decl stack. */

void
lto_push_out_decl_state (struct lto_out_decl_state *state)
{
  VEC_safe_push (lto_out_decl_state_ptr, heap, decl_state_stack, state);
}

/* Pop the currently used out-decl state from top of stack. */

struct lto_out_decl_state *
lto_pop_out_decl_state (void)
{
  return VEC_pop (lto_out_decl_state_ptr, decl_state_stack);
}

/* Record STATE after it has been used in serializing the body of
   FN_DECL.  STATE should no longer be used by the caller.  The ownership
   of it is taken over from this point.  */

void
lto_record_function_out_decl_state (tree fn_decl,
				    struct lto_out_decl_state *state)
{
  int i;

  /* Strip all hash tables to save some memory. */
  for (i = 0; i < LTO_N_DECL_STREAMS; i++)
    if (state->streams[i].tree_hash_table)
      {
	htab_delete (state->streams[i].tree_hash_table);
	state->streams[i].tree_hash_table = NULL;
      }
  state->fn_decl = fn_decl;
  VEC_safe_push (lto_out_decl_state_ptr, heap, function_decl_states, state);
}

/* Return a reference index for tree node T from hash table H.  If
   node T does not already exist in table H, a new entry is created
   for it.  The returned reference is used to identify multiple
   instances of T on the file stream.  This is used in two ways:

   - On the writing side, the first time T is added to H, a new reference
     index is created for T and T is emitted on the stream.  If T
     needs to be emitted again to the stream, instead of pickling it
     again, the reference index is emitted.

   - On the reading side, the first time T is read from the stream,
     it is reconstructed in memory and a new reference index created
     for T.  The reconstructed T is inserted in some array so that 
     when the reference index for T is found in the input stream, it
     can be used to look up into the array to get the reconstructed T.

   *NEXT_REF_P points to the next available reference index to be
   handed out.  If T is inserted into H, this value is updated.

   The reference index for T is returned in *REF_P.  The function
   returns true if T was already in H.  Otherwise, it returns false.  */

bool
get_ref_idx_for (tree t, htab_t h, VEC(tree, heap) **v, unsigned *ref_p)
{
  void **slot;
  struct lto_decl_slot d_slot;
  unsigned next_ref_idx = htab_elements (h);
  bool retval;

  retval = true;
  d_slot.t = t;
  slot = htab_find_slot (h, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct lto_decl_slot *new_slot = XNEW (struct lto_decl_slot);
      new_slot->t = t;
      new_slot->slot_num = next_ref_idx;
      *slot = new_slot;

      if (v)
	{
	  gcc_assert (next_ref_idx == VEC_length (tree, *v));
	  VEC_safe_push (tree, heap, *v, t);
	}

      /* Indicate that the item was not in the table before by
	 returning false.  */
      retval = false;
    }

  if (ref_p)
    *ref_p = ((struct lto_decl_slot *) *slot)->slot_num;

  return retval;
}

/* Record NODE in COMMON_NODES if it is not NULL and is not already in
   SEEN_NODES.  In some cases, also record reachable nodes.  */

static void
lto_record_common_node (tree node, VEC(tree, heap) **common_nodes,
			struct pointer_set_t *seen_nodes)
{
  /* Skip empty slots.  Note that we will be in trouble if
     the empty slots do not match on both the writer and reader side.  */
  if (!node)
    return;

  /* Return if node is already seen.  */
  if (pointer_set_insert (seen_nodes, node))
    return;

  VEC_safe_push (tree, heap, *common_nodes, node);

  /* FIXME lto: In principle, we should perform a walk over all nodes reachable
     from each preloaded node.  This is going to be a lot of work.  At present,
     we catch the case that was causing test failures.  A small step.  */
  if (tree_node_can_be_shared (node))
    {
      if (TYPE_P (node))
	{
	  lto_record_common_node (TYPE_MAIN_VARIANT (node), common_nodes,
				  seen_nodes);
	}
    }
}

/* Generate a vector of common nodes. */

VEC(tree,heap)*
lto_get_common_nodes (void)
{
  unsigned i;
  VEC(tree,heap) *common_nodes = NULL;
  struct pointer_set_t *seen_nodes;

  /* The MAIN_IDENTIFIER_NODE is normally set up by the front-end, but the
     LTO back-end must agree. Currently, the only languages that set this
     use the name "main".  */
  if (main_identifier_node)
    {
      const char *main_name = IDENTIFIER_POINTER (main_identifier_node);
      gcc_assert (strcmp (main_name, "main") == 0);
    }

  gcc_assert (ptrdiff_type_node == integer_type_node);

  /* FIXME lto.  In the C++ front-end, fileptr_type_node is defined as a
     variant copy of of ptr_type_node, rather than ptr_node itself.  The
     distinction should only be relevant to the front-end, so we always
     use the C definition here in lto1.

     These should be assured in free_lang_specifics.  */

  gcc_assert (fileptr_type_node == ptr_type_node);
  gcc_assert (TYPE_MAIN_VARIANT (fileptr_type_node) == ptr_type_node);
  
  seen_nodes = pointer_set_create ();
  
  for (i = 0; i < TI_MAX; i++)
    lto_record_common_node (global_trees[i], &common_nodes, seen_nodes);

  for (i = 0; i < itk_none; i++)
    lto_record_common_node (integer_types[i], &common_nodes, seen_nodes);

  pointer_set_destroy (seen_nodes);
  return common_nodes;
} 

/* Assign an index to tree node T and enter it in the global streamer
   hash table OB->MAIN_HASH_TABLE.  */

void
preload_common_node (tree t, htab_t h, VEC(tree, heap) **v, unsigned *ref_p)
{
  gcc_assert (t);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "Preloading common node: [%s] ",
	   tree_code_name[TREE_CODE (t)]);
  print_generic_expr (stderr, t, 0);
  fprintf (stderr, "\n");
#endif

 get_ref_idx_for (t, h, v, ref_p);
}


/* Preload the streamer hash table with pointers to well-known objects
   so that they will not be streamed out, and will be replaced with the
   corresponding objects when streamed back in.  */

static void
preload_common_nodes (struct output_block *ob)
{
  unsigned i;
  VEC(tree, heap) *common_nodes = lto_get_common_nodes ();
  tree node;
  
#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloading all common nodes.\n");
#endif

  for (i = 0; VEC_iterate (tree, common_nodes, i, node); i++)
    preload_common_node (node, ob->main_hash_table, NULL, NULL);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloaded %u common nodes\n", i - 1);
#endif

  VEC_free (tree, heap, common_nodes);
}

/* Write each node in encoded by ENCODER to OB, as well as those reachable 
   from it and required for correct representation of its semantics.
   Each node in ENCODER must be a global declaration or a type.  A node
   is written only once, even if it appears multiple times in the
   vector.  Certain transitively-reachable nodes, such as those
   representing expressions, may be duplicated, but such nodes
   must not appear in ENCODER itself.  */

static void
write_global_stream (struct output_block *ob,
		     struct lto_tree_ref_encoder *encoder)
{
  tree t;
  size_t index;
  const size_t size = lto_tree_ref_encoder_size (encoder);

  for (index = 0; index < size; index++)
    {
      void *slot;
      struct lto_decl_slot d_slot;

      t = lto_tree_ref_encoder_get_tree (encoder, index);
      d_slot.t = t;
      slot = htab_find_slot (ob->main_hash_table, &d_slot, NO_INSERT);
      if (slot == NULL)
        output_tree (ob, t);
    }
}


/* Write a sequence of indices into the globals vector corresponding
   to the trees in ENCODER.  These are used by the reader to map the
   indices used to refer to global entities within function bodies to
   their referents.  */

static void
write_global_references (struct output_block *ob,
			 struct lto_output_stream *ref_stream,
 			 struct lto_tree_ref_encoder *encoder)
{
  tree t;
  int32_t index;
  const int32_t size = lto_tree_ref_encoder_size (encoder);

  /* Write size as 32-bit unsigned. */
  lto_output_data_stream (ref_stream, &size, sizeof (int32_t));

  for (index = 0; index < size; index++)
    {
      void **slot;
      struct lto_decl_slot d_slot;
      struct lto_decl_slot *old_slot;
      int32_t slot_num;

      t = lto_tree_ref_encoder_get_tree (encoder, index);
      d_slot.t = t;
      slot = htab_find_slot (ob->main_hash_table, &d_slot, NO_INSERT);
      gcc_assert (slot);
      old_slot = (struct lto_decl_slot *)*slot;
      slot_num = old_slot->slot_num;
      lto_output_data_stream (ref_stream, &slot_num, sizeof slot_num);
    }
}

/* Write all the streams in an lto_out_decl_state STATE using
   output block OB and output stream OUT_STREAM.  */

static void
lto_output_decl_state_streams (struct output_block *ob,
			       struct lto_out_decl_state *state)
{
  int i;

  for (i = 0;  i < LTO_N_DECL_STREAMS; i++)
    write_global_stream (ob, &state->streams[i]);
}

/* Write all the references in an lto_out_decl_state STATE using
   output block OB and output stream OUT_STREAM.  */

static void
lto_output_decl_state_refs (struct output_block *ob,
			    struct lto_output_stream *out_stream,
			    struct lto_out_decl_state *state)
{
  unsigned i;
  int32_t ref;
  void **slot;
  struct lto_decl_slot d_slot;
  struct lto_decl_slot *old_slot;
  tree decl;
  
  /* Write reference to FUNCTION_DECL.  If there is not function,
     write reference to void_type_node. */
  decl = (state->fn_decl) ? state->fn_decl : void_type_node;
  d_slot.t = decl;
  slot = htab_find_slot (ob->main_hash_table, &d_slot, NO_INSERT);
  gcc_assert (slot);
  old_slot = (struct lto_decl_slot *)*slot;
  ref = old_slot->slot_num;
  lto_output_data_stream (out_stream, &ref, sizeof (int32_t));

  for (i = 0;  i < LTO_N_DECL_STREAMS; i++)
    write_global_references (ob, out_stream, &state->streams[i]);
}

/* Return the written size of STATE. */

static size_t
lto_out_decl_state_written_size (struct lto_out_decl_state *state)
{
  int i;
  size_t size;

  size = sizeof (int32_t);	/* fn_ref. */
  for (i = 0; i < LTO_N_DECL_STREAMS; i++)
    {
      size += sizeof (int32_t); /* vector size. */
      size += (lto_tree_ref_encoder_size (&state->streams[i])
	       * sizeof (int32_t));
    }
  return size;
}

/* Helper function of write_symbols_of_kind.  HASH is the hash with the
   decl -> slot_num mapping. STREAM is the stream where to write the table.
   V is a vector with the DECLs that should be on the table.  SEEN is
   a bitmap of symbols written so far.  */

static void
write_symbol_vec (htab_t hash, struct lto_output_stream *stream,
		  VEC(tree,heap) *v, bitmap seen)
{
  tree t;
  int index;

  for (index = 0; VEC_iterate(tree, v, index, t); index++)
    {
      const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (t));
      enum gcc_plugin_symbol_kind kind;
      enum gcc_plugin_symbol_visibility visibility;
      struct lto_decl_slot d_slot;
      int slot_num;
      void **slot;
      uint64_t size;
      const char *comdat;

      d_slot.t = t;
      slot = htab_find_slot (hash, &d_slot, NO_INSERT);
      gcc_assert (slot != NULL);
      slot_num = ((struct lto_decl_slot *)*slot)->slot_num;

      /* Avoid duplicate symbols. */
      if (bitmap_bit_p (seen, slot_num))
	continue;
      else
        bitmap_set_bit (seen, slot_num);

      if (DECL_EXTERNAL (t))
	{
	  if (DECL_WEAK (t))
	    kind = GCCPK_WEAKUNDEF;
	  else
	    kind = GCCPK_UNDEF;
	}
      else
	{
	  if (DECL_WEAK (t))
	    kind = GCCPK_WEAKDEF;
	  else if (DECL_COMMON (t))
	    kind = GCCPK_COMMON;
	  else
	    kind = GCCPK_DEF;
	}

      switch (DECL_VISIBILITY(t))
	{
	case VISIBILITY_DEFAULT:
	  visibility = GCCPV_DEFAULT;
	  break;
	case VISIBILITY_PROTECTED:
	  visibility = GCCPV_PROTECTED;
	  break;
	case VISIBILITY_HIDDEN:
	  visibility = GCCPV_HIDDEN;
	  break;
	case VISIBILITY_INTERNAL:
	  visibility = GCCPV_INTERNAL;
	  break;
	}

      if (kind == GCCPK_COMMON)
	size = ((uint64_t) TREE_INT_CST_HIGH (DECL_SIZE (t))) << 32
	  | TREE_INT_CST_LOW (DECL_SIZE (t));
      else
	size = 0;

      if (DECL_COMDAT (t))
	comdat = lang_hooks.decls.comdat_group (t);
      else
	comdat = "";

      lto_output_data_stream (stream, name, strlen (name) + 1);
      lto_output_data_stream (stream, comdat, strlen (comdat) + 1);
      lto_output_data_stream (stream, &kind, 1);
      lto_output_data_stream (stream, &visibility, 1);
      lto_output_data_stream (stream, &size, 8);
      lto_output_data_stream (stream, &slot_num, 4);
    }
}

/* Write IL symbols of KIND. HASH is the hash with the decl -> slot_num
   mapping.  SEEN is a bitmap of symbols written so far.  */

static void
write_symbols_of_kind (lto_decl_stream_e_t kind, htab_t hash, bitmap seen)
{
  struct lto_out_decl_state *out_state;
  struct lto_output_stream stream;
  unsigned num_fns = VEC_length (lto_out_decl_state_ptr, function_decl_states);
  unsigned idx;

  memset (&stream, 0, sizeof (stream));
  out_state = lto_get_out_decl_state ();
  write_symbol_vec (hash, &stream, out_state->streams[kind].trees, seen);

  for (idx = 0; idx < num_fns; idx++)
    {
      out_state =
	VEC_index (lto_out_decl_state_ptr, function_decl_states, idx);
      write_symbol_vec (hash, &stream, out_state->streams[kind].trees, seen);
    }

  lto_write_stream (&stream);
}

/* Write an IL symbol table. HASH is the hash with the decl -> slot_num
   mapping.  */

static void
produce_symtab (htab_t hash)
{
  char *section_name = lto_get_section_name (LTO_section_symtab, NULL);
  bitmap seen;

  lto_begin_section (section_name);
  free(section_name);

  seen = BITMAP_ALLOC (NULL);
  write_symbols_of_kind (LTO_DECL_STREAM_FN_DECL, hash, seen);
  write_symbols_of_kind (LTO_DECL_STREAM_VAR_DECL, hash, seen);

  BITMAP_FREE (seen);
  lto_end_section ();
}

/* This pass is run after all of the functions are serialized and all
   of the IPA passes have written their serialized forms.  This pass
   causes the vector of all of the global decls and types used from
   this file to be written in to a section that can then be read in to
   recover these on other side.  */

static void
produce_asm_for_decls (cgraph_node_set set ATTRIBUTE_UNUSED)
{
  struct lto_out_decl_state *out_state = lto_get_out_decl_state ();
  struct lto_out_decl_state *fn_out_state;
  struct lto_decl_header header;
  char *section_name;
  struct output_block *ob = create_output_block (LTO_section_decls);
  struct lto_output_stream *header_stream, *decl_state_stream;
  unsigned idx, num_fns;
  size_t decl_state_size;
  int32_t num_decl_states;

  ob->global = true;
  ob->main_hash_table = htab_create (37, lto_hash_global_slot_node,
				     lto_eq_global_slot_node, free);

  /* Assign reference indices for predefined trees.  These need not be
     serialized.  */
  preload_common_nodes (ob);

  memset (&header, 0, sizeof (struct lto_decl_header)); 

  section_name = lto_get_section_name (LTO_section_decls, NULL);
  lto_begin_section (section_name);
  free (section_name);

  /* Make string 0 be a NULL string.  */
  lto_output_1_stream (ob->string_stream, 0);

  /* Write the global var decls.  */
  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);
  num_fns = VEC_length (lto_out_decl_state_ptr, function_decl_states);
  lto_output_decl_state_streams (ob, out_state);
  for (idx = 0; idx < num_fns; idx++)
    {
      fn_out_state =
	VEC_index (lto_out_decl_state_ptr, function_decl_states, idx);
      lto_output_decl_state_streams (ob, fn_out_state);
    }

  header.lto_header.major_version = LTO_major_version;
  header.lto_header.minor_version = LTO_minor_version;
  header.lto_header.section_type = LTO_section_decls;

  /* Currently not used.  This field would allow us to preallocate
     the globals vector, so that it need not be resized as it is extended.  */
  header.num_nodes = -1;

  /* Compute the total size of all decl out states. */
  decl_state_size = sizeof (int32_t); /* number of decl_states. */
  decl_state_size += lto_out_decl_state_written_size (out_state);
  for (idx = 0; idx < num_fns; idx++)
    {
      fn_out_state =
	VEC_index (lto_out_decl_state_ptr, function_decl_states, idx);
      decl_state_size += lto_out_decl_state_written_size (fn_out_state);
    }
  header.decl_state_size = decl_state_size;

  header.main_size = ob->main_stream->total_size;
  header.string_size = ob->string_stream->total_size;
#ifdef LTO_STREAM_DEBUGGING
  header.debug_main_size = ob->debug_main_stream->total_size;
#endif

  header_stream = ((struct lto_output_stream *)
		   xcalloc (1, sizeof (struct lto_output_stream)));
  lto_output_data_stream (header_stream, &header, sizeof header);
  lto_write_stream (header_stream);
  free (header_stream);
 
  /* Write the main out-decl state, followed by out-decl states of
     functions. */
  decl_state_stream = ((struct lto_output_stream *)
		       xcalloc (1, sizeof (struct lto_output_stream)));
  num_decl_states = num_fns + 1;
  lto_output_data_stream (decl_state_stream, &num_decl_states,
			  sizeof (num_decl_states));
  lto_output_decl_state_refs (ob, decl_state_stream, out_state);
  for (idx = 0; idx < num_fns; idx++)
    {
      fn_out_state =
	VEC_index (lto_out_decl_state_ptr, function_decl_states, idx);
      lto_output_decl_state_refs (ob, decl_state_stream, fn_out_state);
    }
  lto_write_stream (decl_state_stream);
  free(decl_state_stream); 

  lto_write_stream (ob->main_stream);
  lto_write_stream (ob->string_stream);

#ifdef LTO_STREAM_DEBUGGING
  lto_write_stream (ob->debug_main_stream);
#endif

  lto_end_section ();

  VEC_free (lto_out_decl_state_ptr, heap, function_decl_states);
  function_decl_states = NULL;

  /* Write the symbol table. */
  produce_symtab (ob->main_hash_table);

  /* Deallocate memory and clean up.  */
  destroy_output_block (ob);
}


/* Gate function for all lto streaming passes.  */

bool
gate_lto_out (void)
{
  return (flag_generate_lto
	  /* Don't bother doing anything if the program has errors.  */
	  && !(errorcount || sorrycount));
}

struct ipa_opt_pass pass_ipa_lto_finish_out =
{
 {
  IPA_PASS,
  "lto_decls_out",	                /* name */
  gate_lto_out,			        /* gate */
  NULL,        	                        /* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_LTO_OUT,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,            			/* todo_flags_start */
  0                                     /* todo_flags_finish */
 },
 NULL,		                        /* generate_summary */
 produce_asm_for_decls,			/* write_summary */
 NULL,		         		/* read_summary */
 NULL,					/* function_read_summary */
 0,					/* TODOs */
 NULL,			                /* function_transform */
 NULL					/* variable_transform */
};

#ifdef LTO_STREAM_DEBUGGING
struct lto_debug_context lto_debug_context;

/* Print character C to the debugging stream in CONTEXT.  */

void
lto_debug_out_fun (struct lto_debug_context *context, char c)
{
  struct lto_output_stream *stream 
    = (struct lto_output_stream *)context->current_data;
  lto_output_1_stream (stream, c);
}
#endif
