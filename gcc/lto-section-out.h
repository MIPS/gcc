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

#ifndef GCC_LTO_SECTION_OUT_H
#define GCC_LTO_SECTION_OUT_H

#ifdef LTO_STREAM_DEBUGGING
void lto_debug_out_fun (struct lto_debug_context *, char);
#endif

struct lto_char_ptr_base
{
  char *ptr;
};

/* An incore byte stream to buffer the various parts of the
function. The entire structure should be zeroed when created.  The
record consists of a set of blocks.  The first sizeof (ptr) bytes are
used as a chain, and the rest store the bytes to be written.  */

struct lto_output_stream
{
  /* The pointer to the first block in the stream.  */
  struct lto_char_ptr_base * first_block;
  /* The pointer to the last and current block in the stream.  */
  struct lto_char_ptr_base * current_block;
  /* The pointer to where the next char should be written.  */
  char * current_pointer;
  /* The number of characters left in the current block.  */
  unsigned int left_in_block;
  /* The block size of the last block allocated.  */
  unsigned int block_size;
  /* The total number of characters written.  */
  unsigned int total_size;
};

#ifdef LTO_STREAM_DEBUGGING
#define LTO_SET_DEBUGGING_STREAM(STREAM,CONTEXT)	\
do { \
  ob-> STREAM = xcalloc (1, sizeof (struct lto_output_stream)); \
  lto_debug_context. CONTEXT = ob-> STREAM; \
  lto_debug_context.current_data = ob-> STREAM; \
  lto_debug_context.stream_name = #CONTEXT; \
  gcc_assert (lto_debug_context.indent == 0);  \
} while (0)
#define LTO_CLEAR_DEBUGGING_STREAM(STREAM) \
  free (ob-> STREAM)
#else
#define LTO_SET_DEBUGGING_STREAM(STREAM,CONTEXT)
#define LTO_CLEAR_DEBUGGING_STREAM(STREAM)  (void)0
#endif

struct lto_decl_slot {
  tree t;
  int slot_num;
};


/* The structure that holds all of the vectors of global types and
   decls used in lto serialization for this file.  */

struct lto_out_decl_state
{
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

  /* The hash table that contains the set of type we have seen so far
     and the indexes assigned to them.  */
  htab_t type_hash_table;
  unsigned int next_type_index;
  VEC(tree,heap) *types;
};

hashval_t lto_hash_decl_slot_node (const void *);
int lto_eq_decl_slot_node (const void *, const void *);
hashval_t lto_hash_type_slot_node (const void *);
int lto_eq_type_slot_node (const void *, const void *);

struct lto_out_decl_state *lto_get_out_decl_state (void);
void lto_write_stream (struct lto_output_stream *);
void lto_output_1_stream (struct lto_output_stream *, char);
void lto_output_uleb128_stream (struct lto_output_stream *, unsigned HOST_WIDE_INT);
void lto_output_widest_uint_uleb128_stream (struct lto_output_stream *,
					    unsigned HOST_WIDEST_INT);
void lto_output_sleb128_stream (struct lto_output_stream *, HOST_WIDE_INT);
void lto_output_integer_stream (struct lto_output_stream *, tree);
bool lto_output_decl_index (struct lto_output_stream *, htab_t, unsigned int *, tree, unsigned int *);
bool gate_lto_out (void);
#endif  /* GCC_LTO_SECTION_OUT_H  */
