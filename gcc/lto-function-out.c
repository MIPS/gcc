/* Write the gimple representation of a function and it's local
   variables to a .o file.

   Copyright 2006 Free Software Foundation, Inc.
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
#include "lto-tags.h"
#include <ctype.h>


sbitmap lto_flags_needed_for;
sbitmap lto_types_needed_for;

#ifdef LTO_STREAM_DEBUGGING
const char *LTO_tag_names[LTO_last_tag];

static struct lto_debug_context lto_debug_context;
static void debug_out_fun (struct lto_debug_context *, char);
#endif


/* The index of the last eh_region seen for an instruction.  The
   eh_region for an instruction is only emitted if it different from
   the last instruction.  */

static int last_eh_region_seen;
static unsigned int expr_to_tag[NUM_TREE_CODES];

struct decl_slot {
  tree t;
  int slot_num;
};

/* Returns a hash code for P.  */

static hashval_t
hash_decl_slot_node (const void *p)
{
  const struct decl_slot *ds = (const struct decl_slot *) p;
  return (hashval_t) DECL_UID (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_decl_slot_node (const void *p1, const void *p2)
{
  const struct decl_slot *ds1 =
    (const struct decl_slot *) p1;
  const struct decl_slot *ds2 =
    (const struct decl_slot *) p2;

  return DECL_UID (ds1->t) == DECL_UID (ds2->t);
}


/* Returns a hash code for P.  */

static hashval_t
hash_type_slot_node (const void *p)
{
  const struct decl_slot *ds = (const struct decl_slot *) p;
  return (hashval_t) TYPE_UID (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_type_slot_node (const void *p1, const void *p2)
{
  const struct decl_slot *ds1 =
    (const struct decl_slot *) p1;
  const struct decl_slot *ds2 =
    (const struct decl_slot *) p2;

  return TYPE_UID (ds1->t) == TYPE_UID (ds2->t);
}


/* Returns a hash code for P.  */

static hashval_t
hash_label_slot_node (const void *p)
{
  const struct decl_slot *ds = (const struct decl_slot *) p;
  return (hashval_t) LABEL_DECL_UID (ds->t);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_label_slot_node (const void *p1, const void *p2)
{
  const struct decl_slot *ds1 =
    (const struct decl_slot *) p1;
  const struct decl_slot *ds2 =
    (const struct decl_slot *) p2;

  return LABEL_DECL_UID (ds1->t) == LABEL_DECL_UID (ds2->t);
}


struct string_slot {
  const char *s;
  int len;
  unsigned int slot_num;
};


/* Returns a hash code for P.  */

static hashval_t
hash_string_slot_node (const void *p)
{
  const struct string_slot *ds = (const struct string_slot *) p;
  return (hashval_t) htab_hash_string (ds->s);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_string_slot_node (const void *p1, const void *p2)
{
  const struct string_slot *ds1 =
    (const struct string_slot *) p1;
  const struct string_slot *ds2 =
    (const struct string_slot *) p2;

  if (ds1->len == ds2->len)
    {
      int i;
      for (i=0; i<ds1->len; i++)
	if (ds1->s[i] != ds2->s[i])
	  return 0;
      return 1;
    }
  else return 0;
}


struct char_ptr_base
{
  char *ptr;
};

/* An incore byte stream to buffer the various parts of the
function. The entire structure should be zeroed when created.  The
record consists of a set of blocks.  The first sizeof (ptr) bytes are
used as a chain, and the rest store the bytes to be written.  */

struct output_stream
{
  /* The pointer to the first block in the stream.  */
  struct char_ptr_base * first_block;
  /* The pointer to the last and current block in the stream.  */
  struct char_ptr_base * current_block;
  /* The pointer to where the next char should be written.  */
  char * current_pointer;
  /* The number of characters left in the current block.  */
  unsigned int left_in_block;
  /* The block size of the last block allocated.  */
  unsigned int block_size;
  /* The total number of characters written.  */
  unsigned int total_size;
};

struct output_block
{
  /* The stream that the main tree codes are written to.  */
  struct output_stream *main_stream;
  /* The stream that contains the indexes for the local name table.  */
  struct output_stream *local_decl_index_stream;
  /* The stream that contains the local name table.  */
  struct output_stream *local_decl_stream;
  /* The stream that contains the names for the named_labels.  */
  struct output_stream *named_label_stream;
  /* The stream that contains the string table.  */
  struct output_stream *string_stream;
  /* The stream that contains the ssa_names table.  */
  struct output_stream *ssa_names_stream;
  /* The stream that contains the cfg.  */
  struct output_stream *cfg_stream;

#ifdef LTO_STREAM_DEBUGGING
  /* The stream that contains the local decls index debugging information.  */
  struct output_stream *debug_decl_index_stream;
  /* The stream that contains the local decls debugging information.  */
  struct output_stream *debug_decl_stream;
  /* The stream that contains the labels debugging information.  */
  struct output_stream *debug_label_stream;
  /* The stream that contains the ssa_names debugging information.  */
  struct output_stream *debug_ssa_names_stream;
  /* The stream that contains the cfg debugging information.  */
  struct output_stream *debug_cfg_stream;
  /* The stream that contains the gimple debugging information.  */
  struct output_stream *debug_main_stream;
#endif

  /* The hash table that contains the set of labels we have seen so
     far and the indexes assigned to them.  */
  htab_t label_hash_table;
  int next_named_label_index;
  int next_unnamed_label_index;
  VEC(tree,heap) *named_labels;

  /* The hash table that contains the set of local parm and var decls
     we have seen so far and the indexes assigned to them.  */
  htab_t local_decl_hash_table;
  unsigned int next_local_decl_index;
  VEC(int,heap) *local_decls_index;
#ifdef LTO_STREAM_DEBUGGING
  VEC(int,heap) *local_decls_index_d;
#endif
  VEC(tree,heap) *local_decls;

  /* The hash table that contains the set of field_decls we have
     seen so far and the indexes assigned to them.  */
  htab_t field_decl_hash_table;
  unsigned int next_field_decl_index;
  VEC(tree,heap) *field_decls;

  /* The hash table that contains the set of function_decls we have
     seen so far and the indexes assigned to them.  */
  htab_t fn_decl_hash_table;
  unsigned int next_fn_decl_index;
  VEC(tree,heap) *fn_decls;

  /* The hash table that contains the set of var_decls we have
     seen so far and the indexes assigned to them.  */
  htab_t var_decl_hash_table;
  unsigned int next_var_decl_index;
  VEC(tree,heap) *var_decls;

  /* The hash table that contains the set of type_decls we have
     seen so far and the indexes assigned to them.  */
  htab_t type_decl_hash_table;
  unsigned int next_type_decl_index;
  VEC(tree,heap) *type_decls;

  /* The hash table that contains the set of strings we have seen so
     far and the indexes assigned to them.  */
  htab_t string_hash_table;
  unsigned int next_string_index;

  /* The hash table that contains the set of type we have seen so far
     and the indexes assigned to them.  */
  htab_t type_hash_table;
  unsigned int next_type_index;
  VEC(tree,heap) *types;

  /* These are the last file and line that were seen in the stream.
     If the current node differs from these, it needs to insert
     something into the stream and fix these up.  */
  const char *current_file;
  int current_line;
#ifdef USE_MAPPED_LOCATION  
  int current_col;
#endif
};


/* The output stream that contains the abbrev table for all of the
   functions in this compilation unit.  */
static void output_expr_operand (struct output_block *, tree);


#ifdef LTO_STREAM_DEBUGGING
#define LTO_SET_DEBUGGING_STREAM(STREAM,CONTEXT)	\
do { \
  ob-> STREAM = xcalloc (1, sizeof (struct output_stream)); \
  lto_debug_context. CONTEXT = ob-> STREAM; \
  lto_debug_context.current_data = ob-> STREAM; \
  gcc_assert (lto_debug_context.indent == 0);  \
} while (0)
#define LTO_CLEAR_DEBUGGING_STREAM(STREAM) \
  free (ob-> STREAM)
#else
#define LTO_SET_DEBUGGING_STREAM(STREAM,CONTEXT)
#define LTO_CLEAR_DEBUGGING_STREAM(STREAM)  (void)0
#endif


/* Clear the line info stored in DATA_IN.  */

static void
clear_line_info (struct output_block *ob)
{
  ob->current_file = NULL;
  ob->current_line = 0;
#ifdef USE_MAPPED_LOCATION  
  ob->current_col = 0;
#endif
}


/* Create the output block and return it.  IS_FUNTION is true if this
   is for a function and false for a constructor.  */

static struct output_block *
create_output_block (bool is_function)
{
  struct output_block *ob = xcalloc (1, sizeof (struct output_block));

  ob->main_stream = xcalloc (1, sizeof (struct output_stream));
  ob->string_stream = xcalloc (1, sizeof (struct output_stream));
  if (is_function)
    {
      ob->local_decl_index_stream = xcalloc (1, sizeof (struct output_stream));
      ob->local_decl_stream = xcalloc (1, sizeof (struct output_stream));
      ob->named_label_stream = xcalloc (1, sizeof (struct output_stream));
      ob->ssa_names_stream = xcalloc (1, sizeof (struct output_stream));
      ob->cfg_stream = xcalloc (1, sizeof (struct output_stream));
    }
#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.out = debug_out_fun;
  lto_debug_context.indent = 0;
#endif

  clear_line_info (ob);

  if (is_function)
    {
      ob->label_hash_table
	= htab_create (37, hash_label_slot_node, eq_label_slot_node, free);
      ob->local_decl_hash_table
	= htab_create (37, hash_decl_slot_node, eq_decl_slot_node, free);
    }

  ob->field_decl_hash_table
    = htab_create (37, hash_decl_slot_node, eq_decl_slot_node, free);
  ob->fn_decl_hash_table
    = htab_create (37, hash_decl_slot_node, eq_decl_slot_node, free);
  ob->var_decl_hash_table
    = htab_create (37, hash_decl_slot_node, eq_decl_slot_node, free);
  ob->type_decl_hash_table
    = htab_create (37, hash_decl_slot_node, eq_decl_slot_node, free);
  ob->string_hash_table
    = htab_create (37, hash_string_slot_node, eq_string_slot_node, free);
  ob->type_hash_table
    = htab_create (37, hash_type_slot_node, eq_type_slot_node, free);

  /* The unnamed labels must all be negative.  */
  ob->next_unnamed_label_index = -1;
  return ob;
}


/* Destroy the output block OB.  IS_FUNTION is true if this is for a
   function and false for a constructor.  */

static void
destroy_output_block (struct output_block * ob, bool is_function)
{
  if (is_function)
    {
      htab_delete (ob->label_hash_table);
      htab_delete (ob->local_decl_hash_table);
    }
  htab_delete (ob->field_decl_hash_table);
  htab_delete (ob->fn_decl_hash_table);
  htab_delete (ob->var_decl_hash_table);
  htab_delete (ob->type_decl_hash_table);
  htab_delete (ob->string_hash_table);
  htab_delete (ob->type_hash_table);

  free (ob->main_stream);
  free (ob->string_stream);
  if (is_function)
    {
      free (ob->local_decl_index_stream);
      free (ob->local_decl_stream);
      free (ob->named_label_stream);
      free (ob->ssa_names_stream);
      free (ob->cfg_stream);
    }

  LTO_CLEAR_DEBUGGING_STREAM (debug_main_stream);
  if (is_function)
    {
      LTO_CLEAR_DEBUGGING_STREAM (debug_label_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_ssa_names_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_cfg_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_decl_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_decl_index_stream);
    }

  if (is_function)
    {
      VEC_free (int, heap, ob->local_decls_index);
      VEC_free (int, heap, ob->local_decls_index_d);
      VEC_free (tree, heap, ob->named_labels);
    }
  VEC_free (tree, heap, ob->local_decls);
  VEC_free (tree, heap, ob->field_decls);
  VEC_free (tree, heap, ob->fn_decls);
  VEC_free (tree, heap, ob->var_decls);
  VEC_free (tree, heap, ob->types);

  free (ob);
}

/* Write all of the chars in OBS to the assembler.  Recycle the blocks
   in obs as this is being done.  */

static void
write_stream (struct output_stream *obs)
{
  unsigned int block_size = 1024;
  unsigned int num_chars;
  struct char_ptr_base *block;
  if (!obs->first_block)
    return;

  block = obs->first_block;
  while (block)
    {
      const char *base = ((char *)block) + sizeof (struct char_ptr_base);
      struct char_ptr_base *old_block = block;
      block = (struct char_ptr_base *)block->ptr;
      /* If there is a next block, then this one is full, if there is
	 not a next block, then the left_in_block field says how many
	 chars there are in this block.  */
      num_chars = block_size - sizeof (struct char_ptr_base);
      if (!block)
	num_chars = num_chars - obs->left_in_block;

      assemble_string (base, num_chars);
      free (old_block);
      block_size *= 2;
    }
}


/* Write a character to the output block.  */

static void
output_1_stream (struct output_stream *obs, char c)
{
  /* No space left.  */
  if (obs->left_in_block == 0)
    {
      struct char_ptr_base *new_block;

      if (obs->first_block == NULL)
	{
	  /* This is the first time the stream has been written
	     into.  */
	  obs->block_size = 1024;
	  new_block = (struct char_ptr_base*) xmalloc (obs->block_size);
	  obs->first_block = new_block;
	}
      else
	{
	  struct char_ptr_base *tptr;
	  /* Get a new block that is twice as big as the last block
	     and link it into the list.  */
	  obs->block_size *= 2;
	  new_block = (struct char_ptr_base*) xmalloc (obs->block_size);
	  /* The first bytes of the block are reserved as a pointer to
	     the next block.  Set the chain of the full block to the
	     pointer to the new block.  */
	  tptr = obs->current_block;
	  tptr->ptr = (char *)new_block;
	}

      /* Set the place for the next char at the first position after the
	 chain to the next block.  */
      obs->current_pointer
	= ((char *)new_block) + sizeof (struct char_ptr_base);
      obs->current_block = new_block;
      /* Null out the newly allocated block's pointer to the next block.  */
      new_block->ptr = NULL;
      obs->left_in_block = obs->block_size - sizeof (struct char_ptr_base);
    }

  /* Write the actual character.  */
  *obs->current_pointer = c;
  obs->current_pointer++;
  obs->total_size++;
  obs->left_in_block--;
}


/* Write a zero to the output stream.  */

static void
output_zero (struct output_block *ob)
{
  LTO_DEBUG_WIDE ("U", 0);
  output_1_stream (ob->main_stream, 0);
}


/* Output an unsigned LEB128 quantity to OBS.  */

static void
output_uleb128_stream (struct output_stream *obs, unsigned HOST_WIDE_INT work)
{
  LTO_DEBUG_WIDE ("U", work);
  do
    {
      unsigned int byte = (work & 0x7f);
      work >>= 7;
      if (work != 0)
	/* More bytes to follow.  */
	byte |= 0x80;

      output_1_stream (obs, byte);
    }
  while (work != 0);

}


/* Output an unsigned LEB128 quantity to OB->main_stream.  */

static void
output_uleb128 (struct output_block *ob, unsigned HOST_WIDE_INT work)
{
  output_uleb128_stream (ob->main_stream, work);
}


/* Output a signed LEB128 quantity.  */

static void
output_sleb128_stream (struct output_stream *obs, HOST_WIDE_INT work)
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

      output_1_stream (obs, byte);
    }
  while (more);
}


/* Output a signed LEB128 quantity to OB->main_stream.  */

static void
output_sleb128 (struct output_block *ob, HOST_WIDE_INT work)
{
  output_sleb128_stream (ob->main_stream, work);
}


/* Output STRING of LEN to the string table in OB.  Then put the index 
   onto the INDEX_STREAM.  */

static void
output_string (struct output_block *ob, 
	       struct output_stream *index_stream,
	       const char *string,
	       unsigned int len)
{
  void **slot;
  struct string_slot s_slot;
  s_slot.s = string;
  s_slot.len = len;

  slot = htab_find_slot (ob->string_hash_table, &s_slot, INSERT);
  if (*slot == NULL)
    {
      struct output_stream *string_stream = ob->string_stream;
      unsigned int start = string_stream->total_size;
      struct string_slot *new_slot
	= xmalloc (sizeof (struct string_slot));
      unsigned int i;

      new_slot->s = string;
      new_slot->slot_num = start;
      *slot = new_slot;
      output_uleb128_stream (index_stream, start);
      output_uleb128_stream (string_stream, len);
      for (i=0; i<len; i++)
	output_1_stream (string_stream, string[i]);
    }
  else
    {
      struct string_slot *old_slot = (struct string_slot *)*slot;
      output_uleb128_stream (index_stream, old_slot->slot_num);

      /* From the debugging protocol's point of view, the entry needs
	 to look the same reguardless of whether this is the first
	 occurence of this string or not.  Thus, we simulate the same
	 debugging info as would be output as if this was a new
	 string.  */
      LTO_DEBUG_WIDE ("U", len);
    }
  LTO_DEBUG_STRING (string, len);
}


/* Put out a real constant.  */

static void
output_real (struct output_block *ob, tree t)
{
  static char real_buffer[1000];
  const REAL_VALUE_TYPE *r = &TREE_REAL_CST (t);

  LTO_DEBUG_TOKEN ("real");
  real_to_hexadecimal (real_buffer, r, 1000, 0, 1);
  output_string (ob, ob->main_stream, real_buffer, strlen (real_buffer));
}


/* Put out a integer constant.  These are stored as two HOST_WIDE_INTS
   so games may have to be played to shift the data from the high to
   the low value.  */

static void
output_integer (struct output_block *ob, tree t)
{
  struct output_stream *obs = ob->main_stream;
  HOST_WIDE_INT low = TREE_INT_CST_LOW (t);
  HOST_WIDE_INT high = TREE_INT_CST_HIGH (t);
  int more, byte;

  /* Of course if the high value is just sign bits for the signed low
     value, we can just punt and call output_sleb128 and be done with
     it.  */
  if (((high == -1) && (low < 0))
      || ((high == 0) && (low >= 0)))
    {
      output_sleb128_stream (obs, low);
      return;
    }

  LTO_DEBUG_INTEGER ("SS", high, low);

  /* This is just a copy of the output_sleb128 code with extra
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

      output_1_stream (obs, byte);
    }
  while (more);
}


/* Lookup NAME in TABLE.  If NAME is not found, create a new entry in
   TABLE for NAME with NEXT_INDEX and increment NEXT_INDEX.  Then
   print the index to OBS.  True is returned if NAME was added to the
   table.  */

static bool
output_decl_index (struct output_stream * obs, htab_t table,
		   unsigned int *next_index, tree name)
{
  void **slot;
  struct decl_slot d_slot;
  d_slot.t = name;

  slot = htab_find_slot (table, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct decl_slot *new_slot = xmalloc (sizeof (struct decl_slot));
      int index = (*next_index)++;

      new_slot->t = name;
      new_slot->slot_num = index;
      *slot = new_slot;
      output_uleb128_stream (obs, index);
      return true;
    }
  else
    {
      struct decl_slot *old_slot = (struct decl_slot *)*slot;
      output_uleb128_stream (obs, old_slot->slot_num);
      return false;
    }
}


/* Build a densely packed word that contains only the flags that are
   used for this type of tree EXPR and write the word in uleb128 to
   the OB.  FORCE_LOC forces the line number to be serialized
   reguardless of the type of tree.  */


static void
output_tree_flags (struct output_block *ob, 
		   enum tree_code code, 
		   tree expr, 
		   bool force_loc)
{
  unsigned HOST_WIDE_INT flags = 0;
  const char *file_to_write = NULL;
  int line_to_write = -1;

  if (code == 0 || TEST_BIT (lto_flags_needed_for, code))
    {

#define START_CLASS_SWITCH()              \
  {                                       \
    enum tree_code code = TREE_CODE (expr); \
                                          \
    switch (TREE_CODE_CLASS (code))       \
    {

#define START_CLASS_CASE(class)    case class:
#define ADD_CLASS_FLAG(flag_name) \
      { flags <<= 1; if (expr->base. flag_name ) flags |= 1; }
#define END_CLASS_CASE(class)      break;
#define END_CLASS_SWITCH()                \
    default:                              \
      gcc_unreachable ();                 \
    }


#define START_EXPR_SWITCH()               \
    switch (code)			  \
    {
#define START_EXPR_CASE(code)    case code:
#define ADD_EXPR_FLAG(flag_name) \
      { flags <<= 1; if (expr->base. flag_name ) flags |= 1; }
#define ADD_DECL_FLAG(flag_name) \
      { flags <<= 1; if (expr->decl_common. flag_name ) flags |= 1; }
#define ADD_VIS_FLAG(flag_name)  \
      { flags <<= 1; if (expr->decl_with_vis. flag_name ) flags |= 1; }
#define ADD_FUNC_FLAG(flag_name) \
      { flags <<= 1; if (expr->function_decl. flag_name ) flags |= 1; }
#define END_EXPR_CASE(class)      break;
#define END_EXPR_SWITCH()                 \
    default:                              \
      gcc_unreachable ();                 \
    }                                     \
  }

#include "lto-tree-flags.def"

#undef START_CLASS_SWITCH
#undef START_CLASS_CASE
#undef ADD_CLASS_FLAG
#undef END_CLASS_CASE
#undef END_CLASS_SWITCH
#undef START_EXPR_SWITCH
#undef START_EXPR_CASE
#undef ADD_EXPR_FLAG
#undef ADD_DECL_FLAG
#undef ADD_VIS_FLAG
#undef ADD_FUNC_FLAG
#undef END_EXPR_CASE
#undef END_EXPR_SWITCH

      flags <<= LTO_SOURCE_LOC_BITS;
      if (expr)
	{
	  const char *current_file = NULL;
	  int current_line = 0;
#ifdef USE_MAPPED_LOCATION
	  int current_col = 0;
#endif
	  if (EXPR_P (expr))
	    {
	      if (EXPR_HAS_LOCATION (expr))
		{
		  current_file = EXPR_FILENAME (expr);
		  current_line = EXPR_LINENO (expr);
#ifdef USE_MAPPED_LOCATION
		/* This will blow up because i cannot find the function
		   that i would have expected to have the name
		   expr_col.  */ 
		  current_col = ????
#endif
		}
	    }
	  else if (force_loc && DECL_P (expr))
	    {
	      expanded_location xloc 
		= expand_location (DECL_SOURCE_LOCATION (expr));

	      current_file = xloc.file;
	      current_line = xloc.line;
#ifdef USE_MAPPED_LOCATION
	      current_col = xloc.column;
#endif
	    }

	  if (current_file
	      && (ob->current_file == NULL
		  || strcmp (ob->current_file, current_file) != 0))
	    {
	      file_to_write = current_file;
	      ob->current_file = current_file;
	      flags |= LTO_SOURCE_FILE;
	    }
	  if (current_line != 0
	      && ob->current_line != current_line)
	    {
	      line_to_write = current_line;
	      ob->current_line = current_line;
	      flags |= LTO_SOURCE_LINE;
	    }
#ifdef USE_MAPPED_LOCATION
	  if (current_loc != 0
	      && ob->current_col != current_col)
	    {
	      col_to_write = current_col;
	      ob->current_clo = current_col;
	      flags |= LTO_SOURCE_COL;
	    }
#endif
	}

      LTO_DEBUG_TOKEN ("flags");
      output_uleb128 (ob, flags);
      LTO_DEBUG_TREE_FLAGS (code, flags);

      if (file_to_write)
	{
	  LTO_DEBUG_TOKEN ("file");
	  output_string (ob, ob->main_stream, 
			 file_to_write, strlen (file_to_write) + 1);
	}
      if (line_to_write != -1)
	{
	  LTO_DEBUG_TOKEN ("line");
	  output_uleb128 (ob, line_to_write);
	}
#ifdef USE_MAPPED_LOCATION
      if (col_to_write != -1)
	{
	  LTO_DEBUG_TOKEN ("col");
	  output_uleb128 (ob, col_to_write);
	}
#endif
    }
}


/* Look up TYPE in the type table and write the uleb128 index for it.
   This is a hack and will be replaced with a real reference to the
   type.  */

static void
output_type_ref (struct output_block *ob, tree node)
{
  bool new;

  LTO_DEBUG_TOKEN ("type");
  new = output_decl_index (ob->main_stream, ob->type_hash_table,
			   &ob->next_type_index, node);

  if (new)
    VEC_safe_push (tree, heap, ob->types, node);
}


/* Look up NAME in the type table and write the uleb128 index for it
   to OB.  */

static void
output_local_decl_ref (struct output_block *ob, tree name)
{
  bool new = output_decl_index (ob->main_stream, ob->local_decl_hash_table,
				&ob->next_local_decl_index, name);
  /* Push the new local var or param onto a vector for later
     processing.  */
  if (new)
    {
      VEC_safe_push (tree, heap, ob->local_decls, name);
      VEC_safe_push (int, heap, ob->local_decls_index, 0);
#ifdef LTO_STREAM_DEBUGGING
      VEC_safe_push (int, heap, ob->local_decls_index_d, 0);
#endif
    }
}

/* Look up LABEL in the type table and write the uleb128 index for it.  */

static void
output_label_ref (struct output_block *ob, tree label)
{
  void **slot;
  struct decl_slot d_slot;
  d_slot.t = label;

  slot = htab_find_slot (ob->label_hash_table, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct decl_slot *new_slot = xmalloc (sizeof (struct decl_slot));

      /* Named labels are given positive integers and unnamed labels are 
	 given negative indexes.  */
      bool named = (DECL_NAME (label) != NULL_TREE);
      int index = named
	? ob->next_named_label_index++ : ob->next_unnamed_label_index--;

      new_slot->t = label;
      new_slot->slot_num = index;
      *slot = new_slot;
      output_sleb128 (ob, index);
      if (named)
	VEC_safe_push (tree, heap, ob->named_labels, label);
    }
  else
    {
      struct decl_slot *old_slot = (struct decl_slot *)*slot;
      output_sleb128 (ob, old_slot->slot_num);
    }
}


/* Output the start of a record with TAG and possibly flags for EXPR,
   and the TYPE for VALUE to OB.  */

static void
output_record_start (struct output_block *ob, tree expr,
		     tree value, unsigned int tag)
{
  output_1_stream (ob->main_stream, tag);
  LTO_DEBUG_INDENT (tag);
  if (expr)
    {
      enum tree_code code = TREE_CODE (expr);
      if (value && TEST_BIT (lto_types_needed_for, code))
	output_type_ref (ob, TREE_TYPE (value));
      output_tree_flags (ob, code, expr, false);
    }
}


/* Output a LIST of TYPE_DECLS.  */

static void
output_type_list (struct output_block *ob, tree list)
{
  tree tl;
  int count = 0;
  if (list)
    {
      gcc_assert (TREE_CODE (list) == TREE_LIST);
      for (tl = list; tl; tl = TREE_CHAIN (tl))
	if (TREE_VALUE (tl) != NULL_TREE)
	  count++;

      output_uleb128 (ob, count);
      for (tl = list; tl; tl = TREE_CHAIN (tl))
	if (TREE_VALUE (tl) != NULL_TREE)
	  output_type_ref (ob, TREE_VALUE (tl));
    }
  else
    output_zero (ob);
}


/* Output a LIST of TAG.  Note that the list may be a NULL_TREE.  */

static void
output_tree_list (struct output_block *ob, tree list)
{
  tree tl;
  int count = 0;
  if (list)
    {
      gcc_assert (TREE_CODE (list) == TREE_LIST);
      for (tl = list; tl; tl = TREE_CHAIN (tl))
	if (TREE_VALUE (tl) != NULL_TREE)
	  count++;

      output_uleb128 (ob, count);
      for (tl = list; tl; tl = TREE_CHAIN (tl))
	if (TREE_VALUE (tl) != NULL_TREE)
	  output_expr_operand (ob, TREE_VALUE (tl));
    }
  else
    output_zero (ob);
}


/* Output an eh_cleanup region with REGION_NUMBER.  HAS_INNER is true if
   there are children of this node and HAS_PEER is true if there are
   siblings of this node.  MAY_CONTAIN_THROW and PREV_TRY are the
   fields of the eh_region.  */

static void
output_eh_cleanup (void *obv,
		   int region_number,
		   bool has_inner, bool has_peer,
		   bool may_contain_throw, int prev_try)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_cleanup0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_sleb128 (ob, prev_try);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output an eh_try region with REGION_NUMBER.  HAS_INNER is true if
   there are children of this node and HAS_PEER is true if there are
   siblings of this node.  MAY_CONTAIN_THROW, CATCH and LAST_CATCH are
   the fields of the eh_region.  */

static void
output_eh_try (void *obv,
	       int region_number,
	       bool has_inner, bool has_peer,
	       bool may_contain_throw, int catch,
	       int last_catch)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_try0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_sleb128 (ob, catch);
  output_sleb128 (ob, last_catch);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output an eh_catch region with REGION_NUMBER.  HAS_INNER is true if
   there are children of this node and HAS_PEER is true if there are
   siblings of this node.  MAY_CONTAIN_THROW, NEXT_CATCH and
   PREV_CATCH, and TYPE_LIST are the fields of the eh_region.  */

static void
output_eh_catch (void *obv,
		 int region_number,
		 bool has_inner, bool has_peer,
		 bool may_contain_throw, int next_catch,
		 int prev_catch, tree type_list)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_catch0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_sleb128 (ob, next_catch);
  output_sleb128 (ob, prev_catch);
  output_type_list (ob, type_list);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}

/* Output an eh_allowed_exceptions region with REGION_NUMBER.
   HAS_INNER is true if there are children of this node and HAS_PEER
   is true if there are siblings of this node.  MAY_CONTAIN_THROW, and
   TYPE_LIST are the fields of the eh_region.  */

static void
output_eh_allowed (void *obv,
		   int region_number,
		   bool has_inner, bool has_peer,
		   bool may_contain_throw, tree type_list)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_allowed0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_type_list (ob, type_list);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output an eh_must_not_throw region with REGION_NUMBER.  HAS_INNER
   is true if there are children of this node and HAS_PEER is true if
   there are siblings of this node.  MAY_CONTAIN_THROW is the field of
   the eh_region.  */

static void
output_eh_must_not_throw (void *obv,
			  int region_number,
			  bool has_inner, bool has_peer,
			  bool may_contain_throw)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_must_not_throw0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output the existing eh_table to OB.  */

static void
output_eh_regions (struct output_block *ob, struct function *fn)
{
  if (0 && fn->eh)
    {
      output_record_start (ob, NULL, NULL, LTO_eh_table);
      output_eh_records (ob, fn,
			 output_eh_cleanup,
			 output_eh_try,
			 output_eh_catch,
			 output_eh_allowed,
			 output_eh_must_not_throw);
      LTO_DEBUG_UNDENT ();
    }
  /* The 0 either terminates the record or indicates that there are no
     eh_records at all.  */
  output_zero (ob);
}


/* Output constructor CTOR to OB.  */

static void
output_constructor (struct output_block *ob, tree ctor)
{
  tree value;
  tree purpose;
  unsigned HOST_WIDE_INT idx;

  output_record_start (ob, ctor, ctor, LTO_constructor);
  output_uleb128 (ob, VEC_length (constructor_elt, CONSTRUCTOR_ELTS (ctor)));

  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (ctor), idx, purpose, value)
    {
      if (purpose && TREE_CODE (purpose) == RANGE_EXPR)
	{
	  output_record_start (ob, NULL, NULL, LTO_constructor_range);
	  /* Need the types here to reconstruct the ranges.  */
	  output_type_ref (ob, TREE_OPERAND (purpose, 0));
	  output_integer (ob, TREE_OPERAND (purpose, 0));
	  output_type_ref (ob, TREE_OPERAND (purpose, 1));
	  output_integer (ob, TREE_OPERAND (purpose, 1));
	  LTO_DEBUG_UNDENT ();
	}

      if (TREE_CODE (value) == CONSTRUCTOR)
	{
	  output_constructor (ob, value);
	  LTO_DEBUG_UNDENT ();
	}
      else 
	output_expr_operand (ob, value);
    }
}

/* Output EXPR to the main stream in OB.  */

static void
output_expr_operand (struct output_block *ob, tree expr)
{
  enum tree_code code;
  enum tree_code_class class;
  unsigned int tag;

  gcc_assert (expr);

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);
  tag = expr_to_tag [code];

  gcc_assert (class != tcc_type);

  switch (code)
    {
    case COMPLEX_CST:
      if (TREE_CODE (TREE_REALPART (expr)) == REAL_CST)
	{
	  output_record_start (ob, expr, TREE_REALPART (expr),
			       LTO_complex_cst1);
	  output_real (ob, TREE_REALPART (expr));
	  output_real (ob, TREE_IMAGPART (expr));
	}
      else
	{
	  output_record_start (ob, expr, TREE_REALPART (expr),
			       LTO_complex_cst0);
	  output_integer (ob, TREE_REALPART (expr));
	  output_integer (ob, TREE_IMAGPART (expr));
	}
      break;

    case INTEGER_CST:
      output_record_start (ob, expr, expr, tag);
      output_integer (ob, expr);
      break;

    case REAL_CST:
      output_record_start (ob, expr, expr, tag);
      output_real (ob, expr);
      break;

    case STRING_CST:
      {
	output_record_start (ob, expr, expr, LTO_string_cst);
	output_string (ob, ob->main_stream, 
		       TREE_STRING_POINTER (expr),
		       TREE_STRING_LENGTH (expr));
      }
      break;

    case VECTOR_CST:
      {
	tree t = TREE_VECTOR_CST_ELTS (expr);
	int len = 1;
	while ((t = TREE_CHAIN (t)) != NULL)
	  len++;
	t = TREE_VECTOR_CST_ELTS (expr);
	if (TREE_CODE (TREE_VALUE(t)) == REAL_CST)
	  {
	    output_record_start (ob, expr, TREE_VALUE (t),
				 LTO_vector_cst1);
	    output_uleb128 (ob, len);
	    output_real (ob, TREE_VALUE (t));
	    while ((t = TREE_CHAIN (t)) != NULL)
	      output_real (ob, TREE_VALUE (t));
	  }
	else
	  {
	    output_record_start (ob, expr, TREE_VALUE (t),
				 LTO_vector_cst0);
	    output_uleb128 (ob, len);
	    output_integer (ob, TREE_VALUE (t));
	    while ((t = TREE_CHAIN (t)) != NULL)
	      output_integer (ob, TREE_VALUE (t));
	  }
      }
      break;

    case CASE_LABEL_EXPR:
      {
	int variant = 0;
	if (CASE_LOW (expr) != NULL_TREE)
	  variant |= 0x1;
	if (CASE_HIGH (expr) != NULL_TREE)
	  variant |= 0x2;
	output_record_start (ob, expr, NULL,
			     LTO_case_label_expr0 + variant);

	if (CASE_LOW (expr) != NULL_TREE)
	  output_expr_operand (ob, CASE_LOW (expr));
	if (CASE_HIGH (expr) != NULL_TREE)
	  output_expr_operand (ob, CASE_HIGH (expr));
	output_label_ref (ob, CASE_LABEL (expr));
      }
      break;

    case CONSTRUCTOR:
      output_constructor (ob, expr);
      break;

    case SSA_NAME:
      output_record_start (ob, expr, expr, LTO_ssa_name);
      output_uleb128 (ob, SSA_NAME_VERSION (expr));
      break;

    case CONST_DECL:
      /* Just ignore these, Mark will make them disappear.  */
      break;

    case FIELD_DECL:
      {
	bool new;
	output_record_start (ob, NULL, NULL, tag);
	
	new = output_decl_index (ob->main_stream, ob->field_decl_hash_table,
				 &ob->next_field_decl_index, expr);
	if (new)
	  VEC_safe_push (tree, heap, ob->field_decls, expr);
      }
      break;

    case FUNCTION_DECL:
      {
	bool new;
	output_record_start (ob, NULL, NULL, tag);
	
	new = output_decl_index (ob->main_stream, ob->fn_decl_hash_table,
				 &ob->next_fn_decl_index, expr);
	if (new)
	  VEC_safe_push (tree, heap, ob->fn_decls, expr);
      }
      break;

    case VAR_DECL:
      if (DECL_CONTEXT (expr) == NULL_TREE)
	{
	  bool new;
	  output_record_start (ob, NULL, NULL, LTO_var_decl1);

	  new = output_decl_index (ob->main_stream, ob->var_decl_hash_table,
				   &ob->next_var_decl_index, expr);
	  if (new)
	    VEC_safe_push (tree, heap, ob->var_decls, expr);
	}
      else
	{
	  output_record_start (ob, NULL, NULL, LTO_var_decl0);
	  output_local_decl_ref (ob, expr);
	}
      break;

    case TYPE_DECL:
      {
	bool new;
	output_record_start (ob, NULL, NULL, tag);
	
	new = output_decl_index (ob->main_stream, ob->type_decl_hash_table,
				 &ob->next_type_decl_index, expr);
	if (new)
	  VEC_safe_push (tree, heap, ob->type_decls, expr);
      }
      break;

    case PARM_DECL:
      gcc_assert (!DECL_RTL_SET_P (expr));
      output_record_start (ob, NULL, NULL, tag);
      output_local_decl_ref (ob, expr);
      break;

    case LABEL_DECL:
      output_record_start (ob, expr, NULL, tag);
      output_label_ref (ob, expr);
      break;

    case LABEL_EXPR:
      output_record_start (ob, expr, NULL, tag);
      output_label_ref (ob, TREE_OPERAND (expr, 0));
      break;

    case COND_EXPR:
      if (TREE_OPERAND (expr, 1))
	{
	  output_record_start (ob, expr, expr, LTO_cond_expr0);
	  output_expr_operand (ob, TREE_OPERAND (expr, 0));
	  output_expr_operand (ob, TREE_OPERAND (expr, 1));
	  output_expr_operand (ob, TREE_OPERAND (expr, 2));
	}
      else 
	{
	  output_record_start (ob, expr, expr, LTO_cond_expr1);
	  output_expr_operand (ob, TREE_OPERAND (expr, 0));
	}
      break;

    case RESULT_DECL:
      output_record_start (ob, expr, expr, tag);
      break;

    case COMPONENT_REF:
      output_record_start (ob, expr, expr, tag);
      output_expr_operand (ob, TREE_OPERAND (expr, 0));
      output_expr_operand (ob, TREE_OPERAND (expr, 1));
      /* Ignore 3 because it can be recomputed.  */
      break;

    case CALL_EXPR:
      {
	unsigned int count = TREE_INT_CST_LOW (TREE_OPERAND (expr, 0));
	unsigned int i;

	/* Operand 2 is the call chain.  */
	if (TREE_OPERAND (expr, 2))
	  {
	    output_record_start (ob, expr, expr, LTO_call_expr1);
	    output_uleb128 (ob, count);
	    output_expr_operand (ob, TREE_OPERAND (expr, 2));
	  }
	else
	  {
	    output_record_start (ob, expr, expr, LTO_call_expr0);
	    output_uleb128 (ob, count);
	  }
	output_expr_operand (ob, TREE_OPERAND (expr, 1));
	for (i = 3; i < count; i++)
	  output_expr_operand (ob, TREE_OPERAND (expr, i));
      }
      break;

    case BIT_FIELD_REF:
      {
	tree op1 = TREE_OPERAND (expr, 1);
	tree op2 = TREE_OPERAND (expr, 2);
	if ((TREE_CODE (op1) == INTEGER_CST)
	    && (TREE_CODE (op2) == INTEGER_CST))
	  {
	    output_record_start (ob, expr, expr,
				 LTO_bit_field_ref1);
	    output_uleb128 (ob, TREE_INT_CST_LOW (op1));
	    output_uleb128 (ob, TREE_INT_CST_LOW (op2));
	    output_expr_operand (ob, TREE_OPERAND (expr, 0));
	  }
	else
	  {
	    output_record_start (ob, expr, expr,
				 LTO_bit_field_ref0);
	    output_expr_operand (ob, TREE_OPERAND (expr, 0));
	    output_expr_operand (ob, op1);
	    output_expr_operand (ob, op2);
	  }
      }
      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
      /* Ignore operands 2 and 3 for ARRAY_REF and ARRAY_RANGE REF
	 because they can be recomputed.  */
      output_record_start (ob, expr, expr, tag);
      output_expr_operand (ob, TREE_OPERAND (expr, 0));
      output_expr_operand (ob, TREE_OPERAND (expr, 1));
      break;


    case ASM_EXPR:
      {
	tree string_cst = ASM_STRING (expr);
	output_record_start (ob, expr, NULL, LTO_asm_expr);
	output_string (ob, ob->main_stream, 
		       TREE_STRING_POINTER (string_cst),
		       TREE_STRING_LENGTH (string_cst));

	output_tree_list (ob, ASM_INPUTS (expr));
	output_tree_list (ob, ASM_OUTPUTS (expr));
	output_tree_list (ob, ASM_CLOBBERS (expr));
      }
      break;

    case RESX_EXPR:
      output_record_start (ob, expr, NULL, tag);
      output_uleb128 (ob, TREE_INT_CST_LOW (TREE_OPERAND (expr, 0)));
      break;

    case RETURN_EXPR:
      {
	tree t = TREE_OPERAND (expr, 0);
	if (t == NULL)
	  {
	    /* Form return.  */
	    output_record_start (ob, expr, expr,
				 LTO_return_expr0);
	  }
	else if (TREE_CODE (t) == MODIFY_EXPR)
	  {
	    /* Form return a = b;  */
	    output_record_start (ob, expr, expr,
				 LTO_return_expr2);
	    output_expr_operand (ob, TREE_OPERAND (t, 0));
	    output_expr_operand (ob, TREE_OPERAND (t, 1));
	  }
	else
	  {
	    /* Form return a; */
	    output_record_start (ob, expr, expr,
				 LTO_return_expr1);
	    output_expr_operand (ob, t);
	  }
      }
      break;

    case GIMPLE_MODIFY_STMT:
      output_record_start (ob, expr, NULL, tag);
      output_expr_operand (ob, GIMPLE_STMT_OPERAND (expr, 0));
      output_expr_operand (ob, GIMPLE_STMT_OPERAND (expr, 1));
      break;

    case SWITCH_EXPR:
      {
	tree label_vec = TREE_OPERAND (expr, 2);
	size_t len = TREE_VEC_LENGTH (label_vec);
	size_t i;
	output_record_start (ob, expr, NULL, tag);
	output_uleb128 (ob, len);
	output_expr_operand (ob, TREE_OPERAND (expr, 0));
	gcc_assert (TREE_OPERAND (expr, 1) == NULL);

	for (i = 0; i < len; ++i)
	  output_expr_operand (ob, TREE_VEC_ELT (label_vec, i));
      }
      break;

      /* This is the default case. All of the cases that can be done
	 completely mechanically are done here.  */
      {
	int i;
#define SET_NAME(a,b)
#define MAP_EXPR_TAG(expr, tag) case expr:
#define TREE_SINGLE_MECHANICAL_TRUE

#include "lto-tree-tags.def"
#undef MAP_EXPR_TAG
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef SET_NAME
	output_record_start (ob, expr, expr, tag);
	for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (expr)); i++)
	  output_expr_operand (ob, TREE_OPERAND (expr, i));
	break;
      }
      /* This is the error case, these are type codes that will either
	 never happen or that we have not gotten around to dealing
	 with are here.  */
    case BIND_EXPR:
    case BLOCK:
    case CATCH_EXPR:
    case EH_FILTER_EXPR:
    case NAME_MEMORY_TAG:
    case OMP_CONTINUE:
    case OMP_CRITICAL:
    case OMP_FOR:
    case OMP_MASTER:
    case OMP_ORDERED:
    case OMP_PARALLEL:
    case OMP_RETURN:
    case OMP_SECTIONS:
    case OMP_SINGLE:
    case STRUCT_FIELD_TAG:
    case SYMBOL_MEMORY_TAG:
    case TARGET_MEM_REF:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    default:
      /* We cannot have forms that are not explicity handled.  So when
	 this is triggered, there is some form that is not being
	 output.  */
      gcc_unreachable ();
    }

  LTO_DEBUG_UNDENT ();
}


/* Output the local var_decls and parm_decls to OB.  */

static void
output_local_vars (struct output_block *ob)
{
  unsigned int index = 0;

  /* We have found MOST of the local vars by scanning the function.
     However, many local vars have other local vars inside them.  So
     this function uses the same mechanism that the function is output
     in order to put out these local_vars.

     The easiest way to get all of this stuff generated is to play
     pointer games with the streams and reuse the code for putting out
     the function bodies for putting out the local decls.  It needs to
     go into a separate stream because the LTO reader will want to
     process the local variables first, rather than have to back patch
     them.
  */
  struct output_stream *tmp_stream = ob->main_stream;
  ob->main_stream = ob->local_decl_stream;

  while (index < VEC_length (tree, ob->local_decls))
    {
      tree decl = VEC_index (tree, ob->local_decls, index);
      unsigned int variant = 0;
      bool is_var = (TREE_CODE (decl) == VAR_DECL);
      bool needs_backing_var
	= (DECL_DEBUG_EXPR_IS_FROM (decl) && DECL_DEBUG_EXPR (decl));

      /* This will either be a local var decl or a parm decl. */
      unsigned int tag;

      gcc_assert (DECL_SIZE (decl));
      variant |= DECL_ATTRIBUTES (decl)      != NULL_TREE ? 0x01 : 0;
      variant |= DECL_SIZE_UNIT (decl)       != NULL_TREE ? 0x02 : 0;
      variant |= needs_backing_var                        ? 0x04 : 0;
      variant |= DECL_ABSTRACT_ORIGIN (decl) != NULL_TREE ? 0x08 : 0;

      tag = (is_var
	     ? LTO_local_var_decl_body0
	     : LTO_parm_decl_body0)
	+ variant;

      VEC_replace (int, ob->local_decls_index, index, ob->main_stream->total_size);
#ifdef LTO_STREAM_DEBUGGING
      VEC_replace (int, ob->local_decls_index_d, index, ob->debug_decl_stream->total_size);
#endif
      index++;
      output_record_start (ob, NULL, NULL, tag);

      /* Put out the name if there is one.  */
      if (DECL_NAME (decl))
	{
	  tree name = DECL_NAME (decl);
	  output_string (ob, ob->main_stream, 
			 IDENTIFIER_POINTER (name), 
			 IDENTIFIER_LENGTH (name));
	}
      else
	output_uleb128 (ob, 0);

      output_type_ref (ob, TREE_TYPE (decl));

      if (!is_var)
	output_type_ref (ob, DECL_ARG_TYPE (decl));

      clear_line_info (ob);
      output_tree_flags (ob, 0, decl, true);

      LTO_DEBUG_TOKEN ("chain");
      if (TREE_CHAIN (decl))
	output_expr_operand (ob, TREE_CHAIN (decl));
      else
	output_uleb128 (ob, 0);

      LTO_DEBUG_TOKEN ("context");
      if (DECL_CONTEXT (decl))
	output_expr_operand (ob, DECL_CONTEXT (decl));
      else 
	output_uleb128 (ob, 0);

      LTO_DEBUG_TOKEN ("align");
      output_uleb128 (ob, DECL_ALIGN (decl));

      /* Put out the subtrees.  */
      LTO_DEBUG_TOKEN ("size");
      output_expr_operand (ob, DECL_SIZE (decl));
      if (DECL_ATTRIBUTES (decl)!= NULL_TREE)
	{
	  LTO_DEBUG_TOKEN ("attributes");
	  output_tree_list (ob, DECL_ATTRIBUTES (decl));
	}
      if (DECL_SIZE_UNIT (decl) != NULL_TREE)
	output_expr_operand (ob, DECL_SIZE_UNIT (decl));
      if (needs_backing_var)
	output_expr_operand (ob, DECL_DEBUG_EXPR (decl));
      if (DECL_ABSTRACT_ORIGIN (decl) != NULL_TREE)
	output_expr_operand (ob, DECL_ABSTRACT_ORIGIN (decl));

      LTO_DEBUG_UNDENT();
    }

  ob->main_stream = tmp_stream;
}


/* Output the local var_decls index and parm_decls index to OB.  */

static void
output_local_vars_index (struct output_block *ob)
{
  unsigned int index = 0;
  unsigned int stop;

  struct output_stream *tmp_stream = ob->main_stream;
  ob->main_stream = ob->local_decl_index_stream;

  stop = VEC_length (int, ob->local_decls_index);
  for (index = 0; index < stop; index++)
    {
      output_uleb128 (ob, VEC_index (int, ob->local_decls_index, index));
#ifdef LTO_STREAM_DEBUGGING
      output_uleb128 (ob, VEC_index (int, ob->local_decls_index_d, index));
#endif
    }
  ob->main_stream = tmp_stream;
}


/* Output the names in the named labels to the named_label stream.  */

static void
output_named_labels (struct output_block *ob)
{
  unsigned int index = 0;
  clear_line_info (ob);

  while (index < VEC_length (tree, ob->named_labels))
    {
      tree decl = VEC_index (tree, ob->named_labels, index++);
      tree name = DECL_NAME (decl);
      output_string (ob, ob->named_label_stream, 
		     IDENTIFIER_POINTER (name), 
		     IDENTIFIER_LENGTH (name));
   }
}


/* Output all of the active ssa names to the ssa_names stream.  */

static void
output_ssa_names (struct output_block *ob, struct function *fn)
{
  /* Switch streams so we can use output_expr_operand to write the
     SSA_NAME_VAR.  */
  struct output_stream *tmp_stream = ob->main_stream;
  unsigned int i;
  unsigned int len = VEC_length (tree, SSANAMES (fn));

  ob->main_stream = ob->ssa_names_stream;

  output_uleb128 (ob, len);

  for (i = 1; i < len; i++)
    {
      tree ptr = VEC_index (tree, SSANAMES (fn), i);

      if (ptr == NULL_TREE || SSA_NAME_IN_FREE_LIST (ptr))
	continue;

      output_uleb128 (ob, i);
      output_expr_operand (ob, SSA_NAME_VAR (ptr));
      /* Lie about the type of object to get the flags out.  */
      output_tree_flags (ob, 0, ptr, false);
    }

  output_uleb128 (ob, 0);
  ob->main_stream = tmp_stream;
}


/* Output the cfg.  */

static void
output_cfg (struct output_block *ob, struct function *fn)
{
  struct output_stream *tmp_stream = ob->main_stream;
  basic_block bb;

  ob->main_stream = ob->cfg_stream;

  /* Output the number of the highest basic block.  */
  LTO_DEBUG_TOKEN ("lastbb");
  output_uleb128 (ob, last_basic_block_for_function(fn));

  FOR_ALL_BB_FN (bb, fn)
    {
      edge_iterator ei;
      edge e;

      LTO_DEBUG_TOKEN ("bbindex");
      output_sleb128 (ob, bb->index);

      /* Output the successors and the edge flags.  */
      LTO_DEBUG_TOKEN ("edgecount");
      output_uleb128 (ob, EDGE_COUNT (bb->succs));
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  LTO_DEBUG_TOKEN ("dest");
	  output_uleb128 (ob, e->dest->index);
	  LTO_DEBUG_TOKEN ("eflags");
	  output_uleb128 (ob, e->flags);
	}
    }

  LTO_DEBUG_TOKEN ("bbindex");
  output_sleb128 (ob, -1);
  ob->main_stream = tmp_stream;
}


/* Output a phi function to the main stream in OB.  */

static void
output_phi (struct output_block *ob, tree expr)
{
  int len = PHI_NUM_ARGS (expr);
  int i;
  
  output_record_start (ob, expr, expr, LTO_phi_node);
  output_uleb128 (ob, SSA_NAME_VERSION (PHI_RESULT (expr)));

  for (i = 0; i < len; i++)
    {
      output_expr_operand (ob, PHI_ARG_DEF (expr, i));
      output_uleb128 (ob, PHI_ARG_EDGE (expr, i)->src->index);
    }
  LTO_DEBUG_UNDENT ();
}


/* Output a basic block BB to the main stream in OB for this FN.  */

static void
output_bb (struct output_block *ob, basic_block bb, struct function *fn)
{
  block_stmt_iterator bsi = bsi_start (bb);

  output_record_start (ob, NULL, NULL,
		       (!bsi_end_p (bsi)) || phi_nodes (bb) ? LTO_bb1 : LTO_bb0);

  /* The index of the basic block.  */
  LTO_DEBUG_TOKEN ("bbindex");
  output_uleb128 (ob, bb->index);

  if ((!bsi_end_p (bsi)) || phi_nodes (bb))
    {
      /* Output the statements.  The list of statements is terminated
	 with a zero.  */
      tree phi;

      for (bsi = bsi_start (bb);
	   !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	
	  LTO_DEBUG_INDENT_TOKEN ("stmt");
	  output_expr_operand (ob, stmt);
	
	  /* We only need to set the region number of the tree that
	     could throw if the region number is different from the
	     last region number we set.  */
	  if (0 && tree_could_throw_p (stmt))
	    {
	      int region = lookup_stmt_eh_region_fn (fn, stmt);
	      if (region != last_eh_region_seen)
		{
		  output_record_start (ob, NULL, NULL,
				       LTO_set_eh0 + region ? 1 : 0);
		  if (region)
		    output_sleb128 (ob, region);
		
		  last_eh_region_seen = region;
		}
	    }
	}

      LTO_DEBUG_INDENT_TOKEN ("stmt");
      output_zero (ob);

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  LTO_DEBUG_INDENT_TOKEN ("phi");
	  output_phi (ob, phi);
	}

      LTO_DEBUG_INDENT_TOKEN ("phi");
      output_zero (ob);
    }

  LTO_DEBUG_UNDENT();

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 1);
#endif
}


/* Create the header in the file using OB for t.  */

static void
produce_asm (struct output_block *ob, tree t, bool is_function)
{
  int index;
  tree decl;
  tree type;
  const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (t));
  const char *section_name = concat (LTO_SECTION_NAME_PREFIX, name, NULL);
  struct lto_header header;
  lto_out_ref out_ref = {0, NULL, NULL};

  memset (&header, 0, sizeof (struct lto_header)); 

  /* The entire header is stream computed here.  */
  switch_to_section (get_section (section_name, SECTION_DEBUG, t));
  
  /* Write the header which says how to decode the pieces of the
     t.  */
  header.major_version = LTO_major_version;
  header.minor_version = LTO_minor_version;
  
  header.num_field_decls = VEC_length (tree, ob->field_decls);
  header.num_fn_decls = VEC_length (tree, ob->fn_decls);
  header.num_var_decls = VEC_length (tree, ob->var_decls);
  header.num_type_decls = VEC_length (tree, ob->type_decls);
  header.num_types = VEC_length (tree, ob->types);
  if (is_function)
    {
      header.num_local_decls = VEC_length (tree, ob->local_decls);
      header.num_named_labels = ob->next_named_label_index;
      header.num_unnamed_labels = -ob->next_unnamed_label_index;
    }
  header.compressed_size = 0;
  
  if (is_function)
    {
      header.named_label_size = ob->named_label_stream->total_size;
      header.ssa_names_size = ob->ssa_names_stream->total_size;
      header.cfg_size = ob->cfg_stream->total_size;
      header.local_decls_index_size = ob->local_decl_index_stream->total_size;
      header.local_decls_size = ob->local_decl_stream->total_size;
    }
  header.main_size = ob->main_stream->total_size;
  header.string_size = ob->string_stream->total_size;
#ifdef LTO_STREAM_DEBUGGING
  if (is_function)
    {
      header.debug_decl_index_size = ob->debug_decl_index_stream->total_size;
      header.debug_decl_size = ob->debug_decl_stream->total_size;
      header.debug_label_size = ob->debug_label_stream->total_size;
      header.debug_ssa_names_size = ob->debug_ssa_names_stream->total_size;
      header.debug_cfg_size = ob->debug_cfg_stream->total_size;
    }
  header.debug_main_size = ob->debug_main_stream->total_size;
#else
  header.debug_decl_index_size = -1;
  header.debug_decl_size = -1;
  header.debug_label_size = -1;
  header.debug_ssa_names_size = -1;
  header.debug_cfg_size = -1;
  header.debug_main_size = -1;
#endif

  assemble_string ((const char *)&header, 
		   sizeof (struct lto_header));

  /* Write the global field references.  */
  for (index = 0; VEC_iterate(tree, ob->field_decls, index, decl); index++)
    {
      lto_field_ref (decl, &out_ref);
      dw2_asm_output_data (8, out_ref.section, " ");
      dw2_asm_output_delta (8, out_ref.label,
			    out_ref.base_label, " ");
    }

  /* Write the global function references.  */
  for (index = 0; VEC_iterate(tree, ob->fn_decls, index, decl); index++)
    {
      lto_fn_ref (decl, &out_ref);
      dw2_asm_output_data (8, out_ref.section, " ");
      dw2_asm_output_delta (8, out_ref.label,
			    out_ref.base_label, " ");
    }

  /* Write the global var references.  */
  for (index = 0; VEC_iterate(tree, ob->var_decls, index, decl); index++)
    {
      lto_var_ref (decl, &out_ref);
      dw2_asm_output_data (8, out_ref.section, " ");
      dw2_asm_output_delta (8, out_ref.label,
			    out_ref.base_label, " ");
    }

  /* Write the global type_decl references.  */
  for (index = 0; VEC_iterate(tree, ob->type_decls, index, decl); index++)
    {
      lto_typedecl_ref (decl, &out_ref);
      dw2_asm_output_data (8, out_ref.section, " ");
      dw2_asm_output_delta (8, out_ref.label,
			    out_ref.base_label, " ");
    }

  /* Write the global type references.  */
  for (index = 0; VEC_iterate(tree, ob->types, index, type); index++)
    {
      lto_type_ref (type, &out_ref);
      dw2_asm_output_data (8, out_ref.section, " ");
      dw2_asm_output_delta (8, out_ref.label,
			    out_ref.base_label, " ");
    }

  /* Put all of the gimple and the string table out the asm file as a
     block of text.  */
  if (is_function)
    {
      write_stream (ob->named_label_stream);
      write_stream (ob->ssa_names_stream);
      write_stream (ob->cfg_stream);
      write_stream (ob->local_decl_index_stream);
      write_stream (ob->local_decl_stream);
    }
  write_stream (ob->main_stream);
  write_stream (ob->string_stream);
#ifdef LTO_STREAM_DEBUGGING
  if (is_function)
    {
      write_stream (ob->debug_decl_index_stream);
      write_stream (ob->debug_decl_stream);
      write_stream (ob->debug_label_stream);
      write_stream (ob->debug_ssa_names_stream);
      write_stream (ob->debug_cfg_stream);
    }
  write_stream (ob->debug_main_stream);
#endif
}


static bool initialized = false;
static bool initialized_local = false;


/* Static initialization for both the lto reader and the lto
   writer.  */

void
lto_static_init (void)
{
  if (initialized)
    return;

  initialized = true;

  lto_flags_needed_for = sbitmap_alloc (NUM_TREE_CODES);
  sbitmap_ones (lto_flags_needed_for);
  RESET_BIT (lto_flags_needed_for, FIELD_DECL);
  RESET_BIT (lto_flags_needed_for, FUNCTION_DECL);
  RESET_BIT (lto_flags_needed_for, PARM_DECL);
  RESET_BIT (lto_flags_needed_for, SSA_NAME);
  RESET_BIT (lto_flags_needed_for, VAR_DECL);

  lto_types_needed_for = sbitmap_alloc (NUM_TREE_CODES);

#if REDUNDANT_TYPE_SYSTEM
  /* These forms never need types.  */
  sbitmap_ones (lto_types_needed_for);
  RESET_BIT (lto_types_needed_for, ASM_EXPR);
  RESET_BIT (lto_types_needed_for, CASE_LABEL_EXPR);
  RESET_BIT (lto_types_needed_for, FIELD_DECL);
  RESET_BIT (lto_types_needed_for, FUNCTION_DECL);
  RESET_BIT (lto_types_needed_for, GIMPLE_MODIFY_STMT);
  RESET_BIT (lto_types_needed_for, LABEL_DECL);
  RESET_BIT (lto_types_needed_for, LABEL_EXPR);
  RESET_BIT (lto_types_needed_for, MODIFY_EXPR);
  RESET_BIT (lto_types_needed_for, PARM_DECL);
  RESET_BIT (lto_types_needed_for, PHI_NODE);
  RESET_BIT (lto_types_needed_for, RESX_EXPR);
  RESET_BIT (lto_types_needed_for, SSA_NAME);
  RESET_BIT (lto_types_needed_for, SWITCH_EXPR);
  RESET_BIT (lto_types_needed_for, VAR_DECL);
#else
  /* These forms will need types, even when the type system is fixed.  */
  SET_BIT (lto_types_needed_for, COMPLEX_CST);
  SET_BIT (lto_types_needed_for, CONSTRUCTOR);
  SET_BIT (lto_types_needed_for, CONVERT_EXPR);
  SET_BIT (lto_types_needed_for, FIXED_CONVERT_EXPR);
  SET_BIT (lto_types_needed_for, FIXED_CST);
  SET_BIT (lto_types_needed_for, INTEGER_CST);
  SET_BIT (lto_types_needed_for, NOP_EXPR);
  SET_BIT (lto_types_needed_for, REAL_CST);
  SET_BIT (lto_types_needed_for, VECTOR_CST );
  SET_BIT (lto_types_needed_for, VIEW_CONVERT_EXPR);
#endif

#ifdef LTO_STREAM_DEBUGGING
#define LTO_STREAM_DEBUGGING_INIT_NAMES
#define SET_NAME(index,value) LTO_tag_names[index] = value;
#include "lto-tree-tags.def"
#undef SET_NAME
#undef LTO_STREAM_DEBUGGING_INIT_NAMES
#endif
}


/* Static initialization for the lto writer.  */

static void
lto_static_init_local (void)
{
  if (initialized_local)
    return;

  initialized_local = true;

  /* Initialize the expression to tag mapping.  */
#define MAP_EXPR_TAG(expr,tag)   expr_to_tag [expr] = tag;
#define TREE_SINGLE_MECHANICAL_TRUE
#define TREE_SINGLE_MECHANICAL_FALSE
#include "lto-tree-tags.def"

#undef MAP_EXPR_TAG
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef TREE_SINGLE_MECHANICAL_FALSE

  lto_static_init ();
}


#ifdef FILE_PER_FUNCTION
/* The once per compilation unit initialization flag.  */
static int function_num;
#endif

/* Generate complete DWARF information for the function now so that we
   don't run into missing or incomplete information later.  */

static void
generate_early_dwarf_information (tree function)
{
  /* Don't bother with frame information, since we have no RTL.  */
  dwarf2_generate_frame_info_p = false;

  dwarf2out_decl (function);

  /* Later passes, however, will need frame information.  */
  dwarf2_generate_frame_info_p = true;
}

/* Output FN.  */

static void
output_function (tree function)
{
  struct function *fn = DECL_STRUCT_FUNCTION (function);
  basic_block bb;
  struct output_block *ob = create_output_block (true);

  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);
  clear_line_info (ob);

  gcc_assert (!current_function_decl && !cfun);

  /* Set current_function_decl to what the dwarf2 machinery expects.  */
  current_function_decl = function;
  cfun = fn;

  /* Generate debugging info as early as we can.  */
  generate_early_dwarf_information (function);

  /* Make string 0 be a NULL string.  */
  output_1_stream (ob->string_stream, 0);

  last_eh_region_seen = 0;

  output_record_start (ob, NULL, NULL, LTO_function);

  /* Output any exception-handling regions.  */
  output_eh_regions (ob, fn);

  /* Output the argument array.  */
  LTO_DEBUG_INDENT_TOKEN ("decl_arguments");
  if (DECL_ARGUMENTS (function))
    output_expr_operand (ob, DECL_ARGUMENTS (function));
  else
    output_zero (ob);
    
  /* Output the code for the function.  */
  FOR_ALL_BB_FN (bb, fn)
    output_bb (ob, bb, fn);

  /* The terminator for this function.  */
  output_zero (ob);
  LTO_DEBUG_UNDENT();

  LTO_SET_DEBUGGING_STREAM (debug_ssa_names_stream, ssa_names_data);
  output_ssa_names (ob, fn);

  LTO_SET_DEBUGGING_STREAM (debug_cfg_stream, cfg_data);
  output_cfg (ob, fn);

  LTO_SET_DEBUGGING_STREAM (debug_decl_stream, decl_data);
  output_local_vars (ob);

  LTO_SET_DEBUGGING_STREAM (debug_decl_index_stream, decl_index_data);
  output_local_vars_index (ob);

  LTO_SET_DEBUGGING_STREAM (debug_label_stream, label_data);
  output_named_labels (ob);

  /* Create a file to hold the pickled output of this function.  This
     is a temp standin until we start writing sections.  */
  produce_asm (ob, function, true);

  destroy_output_block (ob, true);

  current_function_decl = NULL;
  cfun = NULL;
}


/* Output constructor for VAR.  */

static void
output_constructor_or_init (tree var)
{
  struct output_block *ob = create_output_block (false);

  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);
  clear_line_info (ob);

  /* Make string 0 be a NULL string.  */
  output_1_stream (ob->string_stream, 0);

  LTO_DEBUG_INDENT_TOKEN ("init");
  output_expr_operand (ob, DECL_INITIAL (var));

  /* The terminator for the constructor.  */
  output_zero (ob);

  /* Create a file to hold the pickled output of this function.  This
     is a temp standin until we start writing sections.  */
  produce_asm (ob, var, false);

  destroy_output_block (ob, false);
}


/* Main entry point from the pass manager.  */

static unsigned int
lto_output (void)
{
  struct cgraph_node *node;
  struct varpool_node *vnode;

  section *saved_section = in_section;

  lto_static_init_local ();

  /* Process only the fuctions with bodies and only process the master
     ones of them.  */
  for (node = cgraph_nodes; node; node = node->next)
    if (node->analyzed && cgraph_is_master_clone (node, false))
      output_function (node->decl);

  /* Process the global static vars that have initializers or
     constructors.  */
  FOR_EACH_STATIC_INITIALIZER (vnode)
    if (DECL_CONTEXT (vnode->decl) == NULL_TREE)
      output_constructor_or_init (vnode->decl);

  /* Put back the assembly section that was there before we started
     writing lto info.  */
  if (saved_section)
    switch_to_section (saved_section);

  return 0;
}

static bool
gate_lto_out (void)
{
  return (flag_generate_lto
	  /* Don't bother doing anything if the program has errors.  */
	  && !(errorcount || sorrycount));
}

struct tree_opt_pass pass_ipa_lto_out =
{
  "lto-function-out",	                /* name */
  gate_lto_out,			        /* gate */
  lto_output,		        	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_LTO_OUT,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  TODO_dump_func,			/* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0					/* letter */
};


#ifdef LTO_STREAM_DEBUGGING

/* The low level output routine to print a single character to the
   debugging stream.  */

static void
debug_out_fun (struct lto_debug_context *context, char c)
{
  struct output_stream * stream 
    = (struct output_stream *)context->current_data;
  output_1_stream (stream, c);
}
/* Print the tree flags to the debugging stream.  */
   
void 
lto_debug_tree_flags (struct lto_debug_context *context, 
		      enum tree_code code, 
		      unsigned HOST_WIDE_INT flags)
{
#define CLEAROUT (HOST_BITS_PER_WIDE_INT - 1)

#define START_CLASS_SWITCH()              \
  {                                       \
                                          \
    switch (TREE_CODE_CLASS (code))       \
    {

#define START_CLASS_CASE(class)    case class:
#define ADD_CLASS_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define END_CLASS_CASE(class)      break;
#define END_CLASS_SWITCH()                \
    default:                              \
      gcc_unreachable ();                 \
    }


#define START_EXPR_SWITCH()               \
    switch (code)			  \
    {
#define START_EXPR_CASE(code)    case code:
#define ADD_EXPR_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_DECL_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_VIS_FLAG(flag_name)  \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_FUNC_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define END_EXPR_CASE(class)      break;
#define END_EXPR_SWITCH()                 \
    default:                              \
      gcc_unreachable ();                 \
    }                                     \
  }

#include "lto-tree-flags.def"

#undef START_CLASS_SWITCH
#undef START_CLASS_CASE
#undef ADD_CLASS_FLAG
#undef END_CLASS_CASE
#undef END_CLASS_SWITCH
#undef START_EXPR_SWITCH
#undef START_EXPR_CASE
#undef ADD_EXPR_FLAG
#undef ADD_DECL_FLAG
#undef ADD_VIS_FLAG
#undef ADD_FUNC_FLAG
#undef END_EXPR_CASE
#undef END_EXPR_SWITCH
}
#endif
