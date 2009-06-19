/* Data structures and declarations used for reading and writing
   GIMPLE to a file stream.

   Copyright (C) 2009 Free Software Foundation, Inc.
   Contributed by Doug Kwan <dougkwan@google.com>

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

#ifndef GCC_LTO_STREAMER_H
#define GCC_LTO_STREAMER_H

#include "plugin-api.h"
#include "target.h"
#include "cgraph.h"
#include "vec.h"
#include "vecprim.h"

/* The encoding for a function consists of the following sections:

   1)    The header.
   2)    FIELD_DECLS.
   3)    FUNCTION_DECLS.
   4)    global VAR_DECLS.
   5)    type_decls
   6)    types.
   7)    Names for the labels that have names
   8)    The SSA names.
   9)    The control flow graph.
   10-11)Gimple for local decls.
   12)   Gimple for the function.
   13)   Strings.

   1) THE HEADER.
   2-6) THE GLOBAL DECLS AND TYPES.

      The global decls and types are encoded in the same way.  For each
      entry, there is word with the offset within the section to the
      entry.

   7) THE LABEL NAMES.  

      Since most labels do not have names, this section my be of zero
      length.  It consists of an array of string table references, one
      per label.  In the lto code, the labels are given either
      positive or negative indexes.  the positive ones have names and
      the negative ones do not.  The positive index can be used to
      find the name in this array.

   8) THE ACTIVE SSA NAMES.  

      The SSA_NAME_VERSION in an SSA_NAME in used as an index into
      this table.

   9) THE CFG. 

   10) Index into the local decls.  Since local decls can have local
      decls inside them, they must be read in randomly in order to
      properly restore them.  

   11-12) GIMPLE FOR THE LOCAL DECLS AND THE FUNCTION BODY.

     The gimple consists of a set of records.

       Each record starts with a "LTO_" tag defined in this file.  The
       mapping between gimple tree codes and LTO code described in
       lto-tree-tags.def along with useful macros that allow the
       simple cases to be processed mechanically.

       For the tree types that can be processed mechanically, the form
       of the output is:
         tag          - The LTO_ tag.

	 type         - If the tree code has a bit set in
                        lto_types_needed_for, a reference to the type
			is generated.  This reference is an index into
                        (7).  The index is encoded in uleb128 form.

	 flags        - The set of flags defined for this tree code
		        packed into a word where the low order 2 bits
		        are used to encode the presence or absense of
		        a file and line number respectively. The word
		        is output in uleb128 form. The encoding and
		        decoding of the flags is controlled by
		        lto-tree-flags.def which is designed so that
		        it is useful on both sides.
			
	 file         - If the file of this gimple record is different
                        from the file of the previous record, the file
                        is encoded into the string table and the
                        offset of that entry in uleb128 is output in
                        the stream.  The flags are ored with 0x2 to
                        indicate the presence of this.

	 line number  - If the line number of this gimple record is
                        different from the line number of the previous
			record, the line number in uleb128 is output
			in the stream.  The flags are ored with 0x1 to
			indicate the presence of this.

	 children     - If this gimple record has children defined,
                        they follow here.  For automaticly generated
			gimple forms, TREE_CODE_LENGTH sepcifies the
                        number of children.

     Many gimple forms cannot be automatically processed.  There are
     two reasons for this: either extra fields must be processed
     beyond those defined in the gimple core or some of the children
     are not always there.

     For these cases, the best reference is the code.  The the
     greatest extent possible, the automatic mechanisms are used to
     processes the pieces of such trees.  But the spec is really the
     code and will change as gimple evolves.

     THE FUNCTION
	
     At the top level of (8) is the function. It consists of five
     pieces:

     LTO_function     - The tag.
     eh tree          - This is all of the exception handling regions
                        put out in a post order traversial of the
                        tree.  Siblings are output as lists terminated
			by a 0.  The set of fields matches the fields
			defined in except.c.

     last_basic_block - in uleb128 form.

     basic blocks     - This is the set of basic blocks.

     zero             - The termination of the basic blocks.

     BASIC BLOCKS

     There are two forms of basic blocks depending on if they are
     empty or not.

     The basic block consists of:

     LTO_bb1 or LTO_bb0 - The tag.

     bb->index          - the index in uleb128 form.

     #succs             - The number of successors un uleb128 form.

     the successors     - For each edge, a pair.  The first of the
                          pair is the index of the successor in
                          uleb128 form and the second are the flags in
                          uleb128 form.

     the statements     - A gimple tree, as described above.
                          These are only present for LTO_BB1.
                          Following each statement is an optional
                          exception handling record LTO_set_eh1 or
                          LTO_set_eh0 if the exception handling for
                          this statement differed from the last region
                          output.  LTO_set_eh0 is a special case that
                          sets the region to 0. LTO_set_eh1 contains
			  the region number in sleb128 form.
			
     zero               - This is only present for LTO_BB1 and is used
			  to terminate the statements and exception
			  regions within this block.

   12) STRINGS

     String are represented in the table as pairs, a length in ULEB128
     form followed by the data for the string.  */

/* When we get a strongly typed gimple, this flag should be set to 0
   so we do not waste so much space printing out largely redundant
   type codes.  */
#define REDUNDANT_TYPE_SYSTEM 1

/* The string that is the prefix on the section names we make for lto.
   For decls the DECL_ASSEMBLER_NAME is appended to make the section
   name for the functions and static_initializers.  For other types of
   sections a '.' and the section type are appended.  */
#define LTO_SECTION_NAME_PREFIX         ".gnu.lto_"

#define LTO_major_version 1
#define LTO_minor_version 0

#define LTO_DECL_FLAG_DEFINED		(1 << 0)
#define LTO_DECL_FLAG_SUPPRESS_OUTPUT	(1 << 1)

typedef unsigned char	lto_decl_flags_t;

/* Set of section types that are in an LTO file.  This list will grow
   as the number of IPA passes grows since each IPA pass will need its
   own section type to store its summary information.

   When adding a new section type, you must also extend the
   LTO_SECTION_NAME array in lto-section-in.c.  */
enum lto_section_type
{
  LTO_section_decls = 0,
  LTO_section_function_body,
  LTO_section_static_initializer,
  LTO_section_cgraph,
  LTO_section_ipa_pure_const,
  LTO_section_ipa_reference,
  LTO_section_symtab,
  LTO_section_wpa_fixup,
  LTO_section_opts,
  LTO_N_SECTION_TYPES		/* Must be last.  */
};

/* Indices to the various function, type and symbol streams. */
typedef enum
{
  LTO_DECL_STREAM_TYPE = 0,		/* Must be first. */
  LTO_DECL_STREAM_FIELD_DECL,
  LTO_DECL_STREAM_FN_DECL,
  LTO_DECL_STREAM_VAR_DECL,
  LTO_DECL_STREAM_TYPE_DECL,
  LTO_DECL_STREAM_NAMESPACE_DECL,
  LTO_DECL_STREAM_LABEL_DECL,
  LTO_N_DECL_STREAMS
} lto_decl_stream_e_t;

typedef enum ld_plugin_symbol_resolution ld_plugin_symbol_resolution_t;
DEF_VEC_I(ld_plugin_symbol_resolution_t);
DEF_VEC_ALLOC_I(ld_plugin_symbol_resolution_t, heap);


/* Macro to define convenience functions for type and decl streams
   in lto_file_decl_data.  */ 
#define DEFINE_DECL_STREAM_FUNCS(UPPER_NAME, name) \
static inline tree \
lto_file_decl_data_get_ ## name (struct lto_file_decl_data *data, \
				 unsigned int idx) \
{ \
  struct lto_in_decl_state *state = data->current_decl_state; \
  gcc_assert (idx < state->streams[LTO_DECL_STREAM_## UPPER_NAME].size); \
  return state->streams[LTO_DECL_STREAM_## UPPER_NAME].trees[idx]; \
} \
\
static inline unsigned int \
lto_file_decl_data_num_ ## name ## s (struct lto_file_decl_data *data) \
{ \
  struct lto_in_decl_state *state = data->current_decl_state; \
  return state->streams[LTO_DECL_STREAM_## UPPER_NAME].size; \
}


/* Return a char pointer to the start of a data stream for an lto pass
   or function.  The first parameter is the file data that contains
   the information.  The second parameter is the type of information
   to be obtained.  The third parameter is the name of the function
   and is only used when finding a function body; otherwise it is 
   NULL.  The fourth parameter is the length of the data returned.  */
typedef const char* (lto_get_section_data_f) (struct lto_file_decl_data *, 
					      enum lto_section_type,
					      const char *, 
					      size_t *);

/* Return the data found from the above call.  The first three
   parameters are the same as above.  The fourth parameter is the data
   itself and the fifth is the lenght of the data. */
typedef void (lto_free_section_data_f) (struct lto_file_decl_data *, 
					enum lto_section_type,
					const char *,
					const char *,
					size_t);

/* Structure used as buffer for reading an LTO file.  */
struct lto_input_block 
{
  const char *data;
  unsigned int p;
  unsigned int len;
};

#define LTO_INIT_INPUT_BLOCK(BASE,D,P,L)   \
  do {                                     \
    BASE.data = D;                         \
    BASE.p = P;                            \
    BASE.len = L;                          \
  } while (0)

#define LTO_INIT_INPUT_BLOCK_PTR(BASE,D,P,L) \
  do {                                       \
    BASE->data = D;                          \
    BASE->p = P;                             \
    BASE->len = L;                           \
  } while (0)


/* The is the first part of the record for a function or constructor
   in the .o file.  */
struct lto_header
{
  int16_t major_version;
  int16_t minor_version;
  enum lto_section_type section_type;
};

/* The header for a function body.  */
struct lto_function_header
{
  /* The header for all types of sections. */
  struct lto_header lto_header;

  /* Number of local VAR_DECLS and PARM_DECLS.  */
  int32_t num_local_decls;

  /* Number of labels with names.  */
  int32_t num_named_labels;

  /* Number of labels without names.  */
  int32_t num_unnamed_labels;

  /* Size compressed or 0 if not compressed.  */
  int32_t compressed_size;

  /* Size of names for named labels.  */
  int32_t named_label_size;

  /* Size of the SSA_NAMES table.  */
  int32_t ssa_names_size;

  /* Size of the cfg.  */
  int32_t cfg_size;

  /* Size of local parm and var decl index region. */
  int32_t local_decls_index_size;

  /* Size of local parm and var decl region. */
  int32_t local_decls_size;

  /* Size of main gimple body of function.  */
  int32_t main_size;

  /* Size of the string table.  */
  int32_t string_size;
};


/* Structure describing a symbol section.  */
struct lto_decl_header
{
  /* The header for all types of sections. */
  struct lto_header lto_header;

  /* Size of region for decl state. */
  int32_t decl_state_size;

  /* Number of nodes in globals stream.  */
  int32_t num_nodes;

  /* Size of region for expressions, decls, types, etc. */
  int32_t main_size;

  /* Size of the string table.  */
  int32_t string_size;
};


/* Statistics gathered during LTO, WPA and LTRANS.  */
struct lto_stats_d
{
  unsigned HOST_WIDE_INT num_input_cgraph_nodes;
  unsigned HOST_WIDE_INT num_output_cgraph_nodes;
  unsigned HOST_WIDE_INT num_input_files;
  unsigned HOST_WIDE_INT num_output_files;
  unsigned HOST_WIDE_INT num_cgraph_partitions;
  unsigned HOST_WIDE_INT section_size[LTO_N_SECTION_TYPES];
  unsigned HOST_WIDE_INT num_function_bodies;
  unsigned HOST_WIDE_INT num_trees[NUM_TREE_CODES];
  unsigned HOST_WIDE_INT num_output_il_bytes;
  unsigned HOST_WIDE_INT num_compressed_il_bytes;
  unsigned HOST_WIDE_INT num_input_il_bytes;
  unsigned HOST_WIDE_INT num_uncompressed_il_bytes;
};

/* Encoder data structure used to stream callgraph nodes.  */
struct lto_cgraph_encoder_d
{
  /* Map nodes to reference number. */
  struct pointer_map_t *map;

  /* Map reference number to node. */
  VEC(cgraph_node_ptr,heap) *nodes;
};

typedef struct lto_cgraph_encoder_d *lto_cgraph_encoder_t;

/* Mapping from indices to trees.  */
struct lto_tree_ref_table
{
  /* Array of referenced trees . */
  tree *trees;

  /* Size of array. */
  unsigned int size;
};


/* Mapping between trees and slots in an array.  */
struct lto_decl_slot
{
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


/* Structure to hold states of input scope.  */
struct lto_in_decl_state
{
  /* Array of lto_in_decl_buffers to store type and decls streams. */
  struct lto_tree_ref_table streams[LTO_N_DECL_STREAMS];

  /* If this in-decl state is associated with a function. FN_DECL
     point to the FUNCTION_DECL. */
  tree fn_decl;
};

typedef struct lto_in_decl_state *lto_in_decl_state_ptr;


/* The structure that holds all of the vectors of global types,
   decls and cgraph nodes used in the serialization of this file.  */
struct lto_out_decl_state
{
  /* The buffers contain the sets of decls of various kinds and types we have
     seen so far and the indexes assigned to them.  */
  struct lto_tree_ref_encoder streams[LTO_N_DECL_STREAMS];

  /* Encoder for cgraph nodes.  */
  lto_cgraph_encoder_t cgraph_node_encoder;

  /* If this out-decl state belongs to a function, fn_decl points to that
     function.  Otherwise, it is NULL. */
  tree fn_decl;
};

typedef struct lto_out_decl_state *lto_out_decl_state_ptr;

DEF_VEC_P(lto_out_decl_state_ptr);
DEF_VEC_ALLOC_P(lto_out_decl_state_ptr, heap);

/* One of these is allocated for each object file that being compiled
   by lto.  This structure contains the tables that are needed by the
   serialized functions and ipa passes to connect themselves to the
   global types and decls as they are reconstituted.  */
struct lto_file_decl_data
{
  /* Decl state currently used. */
  struct lto_in_decl_state *current_decl_state;

  /* Decl state corresponding to regions outside of any functions
     in the compilation unit. */
  struct lto_in_decl_state *global_decl_state;

  /* Table of cgraph nodes present in this file.  */
  lto_cgraph_encoder_t cgraph_node_encoder;

  /* Hash table maps lto-related section names to location in file.  */
  htab_t function_decl_states;

  /* The .o file that these offsets relate to.  */
  const char *file_name;

  /* Nonzero if this file should be recompiled with LTRANS.  */
  unsigned needs_ltrans_p : 1;

  /* If the file is open, this is the fd of the mapped section.  This
     is -1 if the file has not yet been opened.  */
  int fd;

  /* Hash table maps lto-related section names to location in file.  */
  htab_t section_hash_table;

  /* Hash new name of renamed global declaration to its original name.  */
  htab_t renaming_hash_table;
};

struct lto_char_ptr_base
{
  char *ptr;
};

/* An incore byte stream to buffer the various parts of the function.
   The entire structure should be zeroed when created.  The record
   consists of a set of blocks.  The first sizeof (ptr) bytes are used
   as a chain, and the rest store the bytes to be written.  */
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

/* The is the first part of the record in an LTO file for many of the
   IPA passes.  */
struct lto_simple_header
{
  /* The header for all types of sections. */
  struct lto_header lto_header;

  /* Size of main gimple body of function.  */
  int32_t main_size;

  /* Size of main stream when compressed.  */
  int32_t compressed_size;
};

/* A simple output block.  This can be used for simple IPA passes that
   do not need more than one stream.  */
struct lto_simple_output_block
{
  enum lto_section_type section_type;
  struct lto_out_decl_state *decl_state;

  /* The stream that the main tree codes are written to.  */
  struct lto_output_stream *main_stream;
};

/* Data structure holding all the data and descriptors used when writing
   an LTO file.  */
struct output_block
{
  enum lto_section_type section_type;
  struct lto_out_decl_state *decl_state;

  /* The stream that the main tree codes are written to.  */
  struct lto_output_stream *main_stream;

  /* The stream that contains the indexes for the local name table.  */
  struct lto_output_stream *local_decl_index_stream;

  /* The stream that contains the local name table.  */
  struct lto_output_stream *local_decl_stream;

  /* The stream that contains the names for the named_labels.  */
  struct lto_output_stream *named_label_stream;

  /* The stream that contains the string table.  */
  struct lto_output_stream *string_stream;

  /* The stream that contains the ssa_names table.  */
  struct lto_output_stream *ssa_names_stream;

  /* The stream that contains the cfg.  */
  struct lto_output_stream *cfg_stream;

  /* The hash table that contains the set of labels we have seen so
     far and the indexes assigned to them.  */
  htab_t label_hash_table;
  int next_named_label_index;
  int next_unnamed_label_index;
  VEC(tree,heap) *named_labels;

  /* The out decl buffer that contains the set of local parm and var decls
     we have seen so far and the indexes assigned to them.  */
  struct lto_tree_ref_encoder local_decl_encoder;

  /* The local_decls_index and the local_decls_index_d are the indexes
     in the local var stream where a particular local var is located.
     This allows the local vars to be read in random order.  */ 
  VEC(int,heap) *local_decls_index;

  /* Index in local_decls so that list can be reconstructed
     properly.  */
  VEC(int,heap) *unexpanded_local_decls_index;

  /* The hash table that contains the set of strings we have seen so
     far and the indexes assigned to them.  */
  htab_t string_hash_table;

  /* The current cgraph_node that we are currently serializing.  Null
     if we are serializing something else.  */
  struct cgraph_node *cgraph_node;

  /* These are the last file and line that were seen in the stream.
     If the current node differs from these, it needs to insert
     something into the stream and fix these up.  */
  const char *current_file;
  int current_line;
  int current_col;

  /* True if writing globals and types.  */
  bool global;

  /* Map global decls and types to indices in the main stream.  */
  htab_t main_hash_table;
};

/* Data and descriptors used when reading from an LTO file.  */
struct data_in
{
  /* The global decls and types.  */
  struct lto_file_decl_data *file_data;

  /* The offsets to decode the local_decls.  */
  int *local_decls_index;

  /* A table to reconstruct the local_decls.  */
  int *local_decl_indexes;  

  /* The local var_decls and the parm_decls.  */
  tree *local_decls;

  /* All of the labels.  */
  tree *labels;

  /* The string table.  */
  const char * strings;

  /* The length of the string table.  */
  unsigned int strings_len;

  /* Number of named labels.  Used to find the index of unnamed labels
     since they share space with the named labels.  */
  unsigned int num_named_labels;  

  /* Number of unnamed labels.  */
  unsigned int num_unnamed_labels;

  const char *current_file;
  int current_line;
  int current_col;

  /* Context for reading global variables and types.
     When a new variable or type is read in from the stream,
     it is pushed onto this vector.  Subsequent references
     to the variable or type specify the vector index.  */
  VEC(tree,heap) *globals_index;

  /* Maps each reference number to the resolution done by the linker. */
  VEC(ld_plugin_symbol_resolution_t,heap) *globals_resolution;
};


/* In lto-section-in.c  */
extern struct lto_input_block * lto_create_simple_input_block (
			       struct lto_file_decl_data *, 
			       enum lto_section_type, const char **, size_t *);
extern void
lto_destroy_simple_input_block (struct lto_file_decl_data *, 
				enum lto_section_type,
				struct lto_input_block *, const char *, size_t);
extern unsigned int lto_get_flag (unsigned HOST_WIDEST_INT *);
extern unsigned int lto_get_flags (unsigned HOST_WIDEST_INT *, unsigned int);
extern void lto_set_in_hooks (struct lto_file_decl_data **, 
			      lto_get_section_data_f *,
			      lto_free_section_data_f *);
extern struct lto_file_decl_data **lto_get_file_decl_data (void);
extern const char *lto_get_section_data (struct lto_file_decl_data *,
					 enum lto_section_type,
					 const char *, size_t *);
extern void lto_free_section_data (struct lto_file_decl_data *,
				   enum lto_section_type,
				   const char *, const char *, size_t);
extern unsigned char lto_input_1_unsigned (struct lto_input_block *);
extern unsigned HOST_WIDE_INT lto_input_uleb128 (struct lto_input_block *);
extern unsigned HOST_WIDEST_INT lto_input_widest_uint_uleb128 (
						struct lto_input_block *);
extern HOST_WIDE_INT lto_input_sleb128 (struct lto_input_block *);
extern tree lto_input_integer (struct lto_input_block *, tree);
extern htab_t lto_create_renaming_table (void);
extern void lto_record_renamed_decl (struct lto_file_decl_data *,
				     const char *, const char *);
extern const char *lto_get_decl_name_mapping (struct lto_file_decl_data *,
					      const char *);
extern struct lto_in_decl_state *lto_new_in_decl_state (void);
extern void lto_delete_in_decl_state (struct lto_in_decl_state *);
extern hashval_t lto_hash_in_decl_state (const void *);
extern int lto_eq_in_decl_state (const void *, const void *);
extern struct lto_in_decl_state *lto_get_function_in_decl_state (
				      struct lto_file_decl_data *, tree);

/* In lto-section-out.c  */
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
struct lto_simple_output_block *lto_create_simple_output_block (
				enum lto_section_type);
void lto_destroy_simple_output_block (struct lto_simple_output_block *ob);
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
void lto_new_extern_inline_states (void);
void lto_delete_extern_inline_states (void);
void lto_force_functions_extern_inline (bitmap decls);
bool lto_forced_extern_inline_p (tree fn_decl);


/* In lto-streamer.c.  */
extern bitmap lto_bitmap_alloc (void);
extern void lto_bitmap_free (bitmap);
extern lto_decl_flags_t lto_get_decl_flags (tree);
extern void lto_set_decl_flags (tree, lto_decl_flags_t);
extern char *lto_get_section_name (int, const char *);
extern void print_lto_report (void);


/* In lto-streamer-in.c */
extern void lto_input_function_body (struct lto_file_decl_data *, tree,
				     const char *);
extern void lto_input_constructors_and_inits (struct lto_file_decl_data *,
					      const char *);
extern void lto_input_cgraph (struct lto_file_decl_data *, const char *);
extern void lto_init_reader (void);
extern tree input_tree (struct lto_input_block *, struct data_in *);
extern void lto_input_function_body (struct lto_file_decl_data *, tree,
				     const char *);
extern void lto_input_constructors_and_inits (struct lto_file_decl_data *,
					      const char *);


/* In lto-streamer-out.c  */
extern void lto_register_decl_definition (tree, struct lto_file_decl_data *);
struct output_block *create_output_block (enum lto_section_type);
void destroy_output_block (struct output_block *);
extern void output_constructors_and_inits (struct cgraph_node_set_def *);
extern void output_tree (struct output_block *, tree);


/* In lto-cgraph.c  */
struct cgraph_node *lto_cgraph_encoder_deref (lto_cgraph_encoder_t, int);
int lto_cgraph_encoder_lookup (lto_cgraph_encoder_t, struct cgraph_node *);
lto_cgraph_encoder_t lto_cgraph_encoder_new (void);
int lto_cgraph_encoder_encode (lto_cgraph_encoder_t, struct cgraph_node *);
void lto_cgraph_encoder_delete (lto_cgraph_encoder_t encoder);
void output_cgraph (cgraph_node_set);
void input_cgraph (void);


/* In lto-symtab.c.  */
extern void lto_symtab_merge_var (tree, enum ld_plugin_symbol_resolution);
extern void lto_symtab_merge_fn (tree, enum ld_plugin_symbol_resolution,
                                 struct lto_file_decl_data *);
extern tree lto_symtab_prevailing_decl (tree decl);
extern enum ld_plugin_symbol_resolution lto_symtab_get_resolution (tree decl);
struct lto_file_decl_data *lto_symtab_get_file_data (tree decl);
extern void lto_symtab_clear_resolution (tree decl);


/* In lto-opts.c.  */
extern void lto_register_user_option (size_t, const char *, int, int);
extern void lto_read_file_options (struct lto_file_decl_data *);
extern void lto_write_options (void);
extern void lto_reissue_options (void);
void lto_clear_user_options (void);
void lto_clear_file_options (void);


/* In lto-wpa-fixup.c  */
void lto_mark_nothrow_fndecl (tree);
void lto_fixup_nothrow_decls (void);


/* Statistics gathered during LTO, WPA and LTRANS.  */
extern struct lto_stats_d lto_stats;

/* Section names corresponding to the values of enum lto_section_type.  */
extern const char *lto_section_name[];

/* Holds all the out decl states of functions output so far in the
   current output file.  */
extern VEC(lto_out_decl_state_ptr, heap) *lto_function_decl_states;

/* Return true if FILE needs to be compiled with LTRANS.  */
static inline bool
lto_file_needs_ltrans_p (struct lto_file_decl_data *file)
{
  return file->needs_ltrans_p != 0;
}


/* Mark FILE to be compiled with LTRANS.  */
static inline void
lto_mark_file_for_ltrans (struct lto_file_decl_data *file)
{
  file->needs_ltrans_p = 1;
}


/* Return true if any files in node set SET need to be compiled
   with LTRANS.  */
static inline bool
cgraph_node_set_needs_ltrans_p (cgraph_node_set set)
{
  cgraph_node_set_iterator csi;

  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    if (lto_file_needs_ltrans_p (csi_node (csi)->local.lto_file_data))
      return true;

  return false;
}


/* Initialize an lto_out_decl_buffer ENCODER.  */
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


/* Return true if LABEL should be emitted in the global context.  */
static inline bool
emit_label_in_global_context_p (tree label)
{
  return DECL_NONLOCAL (label) || FORCED_LABEL (label);
}

DEFINE_DECL_STREAM_FUNCS (TYPE, type)
DEFINE_DECL_STREAM_FUNCS (FIELD_DECL, field_decl)
DEFINE_DECL_STREAM_FUNCS (FN_DECL, fn_decl)
DEFINE_DECL_STREAM_FUNCS (VAR_DECL, var_decl)
DEFINE_DECL_STREAM_FUNCS (TYPE_DECL, type_decl)
DEFINE_DECL_STREAM_FUNCS (NAMESPACE_DECL, namespace_decl)
DEFINE_DECL_STREAM_FUNCS (LABEL_DECL, label_decl)

#endif /* !GCC_LTO_STREAMER_H */
