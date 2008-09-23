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

#include "lto-section.h"

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
  ob-> STREAM = (struct lto_output_stream *) xcalloc (1, sizeof (struct lto_output_stream)); \
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


/* The lto_tree_ref_encoder struct is used to encode trees into indices. */

struct lto_tree_ref_encoder
{
  htab_t tree_hash_table;	/* Maps pointers to indices. */
  unsigned int next_index;	/* Next available index. */
  VEC(tree,heap) *trees;	/* Maps indices to pointers. */
};

/* The structure that holds all of the vectors of global types and
   decls used in lto serialization for this file.  */

struct lto_out_decl_state
{
  /* The buffers contain the sets of decls of various kinds and types we have
     seen so far and the indexes assigned to them.  */
  struct lto_tree_ref_encoder streams[LTO_N_DECL_STREAMS];

  /* If this out-decl state belongs to a function, fn_decl points to that
     function.  Otherwise, it is NULL. */
  tree fn_decl;
};

typedef struct lto_out_decl_state *lto_out_decl_state_ptr;

DEF_VEC_P(lto_out_decl_state_ptr);
DEF_VEC_ALLOC_P(lto_out_decl_state_ptr, heap);

/* A simple output block.  This can be used for simple ipa passes that
   do not need more than one stream.  */
struct lto_simple_output_block
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

/* Iniitialize an lto_out_decl_buffer ENCODER.  */

static inline void
lto_init_tree_ref_encoder (struct lto_tree_ref_encoder *encoder,
			   htab_hash hash_fn, htab_eq eq_fn)
{
  encoder->tree_hash_table = htab_create (37, hash_fn, eq_fn, free);
  encoder->next_index = 0;
  encoder->trees = NULL;
}

/* Destory an lto_tree_ref_encoder ENCODER by freeing its contents.  The
   memory used by ENCODER is not freed by this function.  */

static inline void
lto_destroy_tree_ref_encoder (struct lto_tree_ref_encoder *encoder)
{
  /* Hash table may be delete already.  */
  if (encoder->tree_hash_table)
    htab_delete (encoder->tree_hash_table);
  VEC_free (tree, heap, encoder->trees);
}

/* Return the number of trees encoded in ENCODER. */

static inline unsigned int
lto_tree_ref_encoder_size (struct lto_tree_ref_encoder *encoder)
{
  return VEC_length (tree, encoder->trees);
}

/* Return the IDX-th tree in ENCODER. */

static inline tree
lto_tree_ref_encoder_get_tree (struct lto_tree_ref_encoder *encoder,
			       unsigned int idx)
{
  return VEC_index (tree, encoder->trees, idx);
}
 
void lto_set_flag (unsigned HOST_WIDEST_INT *, unsigned int);
void lto_set_flags (unsigned HOST_WIDEST_INT *, unsigned int, unsigned int);
hashval_t lto_hash_decl_slot_node (const void *);
int lto_eq_decl_slot_node (const void *, const void *);
hashval_t lto_hash_type_slot_node (const void *);
int lto_eq_type_slot_node (const void *, const void *);
hashval_t lto_hash_global_slot_node (const void *);
int lto_eq_global_slot_node (const void *, const void *);

void lto_begin_section (const char *);
void lto_end_section (void);
void lto_write_stream (struct lto_output_stream *);
void lto_output_1_stream (struct lto_output_stream *, char);
void lto_output_data_stream (struct lto_output_stream *, const void *, size_t);
void lto_output_uleb128_stream (struct lto_output_stream *,
				unsigned HOST_WIDE_INT);
void lto_output_widest_uint_uleb128_stream (struct lto_output_stream *,
					    unsigned HOST_WIDEST_INT);
void lto_output_sleb128_stream (struct lto_output_stream *, HOST_WIDE_INT);
void lto_output_integer_stream (struct lto_output_stream *, tree);
bool lto_output_decl_index (struct lto_output_stream *,
			    struct lto_tree_ref_encoder *,
			    tree, unsigned int *);
void lto_output_field_decl_index (struct lto_out_decl_state *,
				  struct lto_output_stream *, tree);
void lto_output_fn_decl_index (struct lto_out_decl_state *,
			       struct lto_output_stream *, tree);
void lto_output_namespace_decl_index (struct lto_out_decl_state *,
				      struct lto_output_stream *, tree);
void lto_output_var_decl_index (struct lto_out_decl_state *,
				struct lto_output_stream *, tree);
void lto_output_type_decl_index (struct lto_out_decl_state *,
				 struct lto_output_stream *, tree);
void lto_output_type_ref_index (struct lto_out_decl_state *,
				struct lto_output_stream *, tree);

struct lto_simple_output_block *lto_create_simple_output_block (enum lto_section_type);
void lto_destroy_simple_output_block (struct lto_simple_output_block * ob);

struct lto_out_decl_state *lto_new_out_decl_state (void);
void lto_delete_out_decl_state (struct lto_out_decl_state *);
struct lto_out_decl_state *lto_get_out_decl_state (void);
void lto_push_out_decl_state (struct lto_out_decl_state *);
struct lto_out_decl_state *lto_pop_out_decl_state (void);
void lto_record_function_out_decl_state (tree, struct lto_out_decl_state*);

VEC(tree,heap)* lto_get_common_nodes (void);

bool gate_lto_out (void);

void preload_common_node (tree, htab_t, VEC(tree, heap) **, unsigned *);
bool get_ref_idx_for (tree, htab_t, VEC(tree, heap) **, unsigned *);

void lto_new_static_inline_states (void);
void lto_delete_static_inline_states (void);
void lto_force_function_static_inline (tree fn_decl);
void lto_force_functions_static_inline (bitmap decls);
bool lto_forced_static_inline_p (tree fn_decl);

#endif  /* GCC_LTO_SECTION_OUT_H  */
