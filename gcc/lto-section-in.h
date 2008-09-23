/* LTO input code.
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

#ifndef GCC_LTO_SECTION_IN_H
#define GCC_LTO_SECTION_IN_H

#include "lto-header.h"

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

/* An lto_tree_reference_table is a mapping from indices to tree. */

struct lto_tree_ref_table {
  tree *trees;		/* Array of referenced trees . */
  unsigned int size;	/* Size of array. */
};

/* Structure to hold states of input scope. */
struct lto_in_decl_state {
  /* Array of lto_in_decl_buffers to store type and decls streams. */
  struct lto_tree_ref_table	streams[LTO_N_DECL_STREAMS];

  /* If this in-decl state is associated with a function. FN_DECL
     point to the FUNCTION_DECL. */
  tree fn_decl;
};

typedef struct lto_in_decl_state *lto_in_decl_state_ptr;

/* One of these is allocated for each object file that being compiled
   by lto.  This structure contains the tables that are needed by the
   serialized functions and ipa passes to connect themselves to the
   global types and decls as they are reconstituted.  */
struct lto_file_decl_data
{
  /* Decl state currently used. */
  struct lto_in_decl_state	*current_decl_state;

  /* Decl state corresponding to regions outside of any functions
     in the compilation unit. */
  struct lto_in_decl_state	*global_decl_state;

  /* Hash table maps lto-related section names to location in file.  */
  htab_t function_decl_states;

  /* The .o file that these offsets relate to.  

  FIXME!!! This will most likely have to be upgraded if the .o files
  have been archived.  */ 
  const char *file_name;

  /* If the file is open, this is the fd of the mapped section.  This
     is -1 if the file has not yet been opened.  */
  int fd;

  /* Hash table maps lto-related section names to location in file.  */
  htab_t section_hash_table;

  /* Hash new name of renamed global declaration to its original name.  */
  htab_t renaming_hash_table;
};

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

DEFINE_DECL_STREAM_FUNCS (TYPE, type)
DEFINE_DECL_STREAM_FUNCS (FIELD_DECL, field_decl)
DEFINE_DECL_STREAM_FUNCS (FN_DECL, fn_decl)
DEFINE_DECL_STREAM_FUNCS (VAR_DECL, var_decl)
DEFINE_DECL_STREAM_FUNCS (TYPE_DECL, type_decl)
DEFINE_DECL_STREAM_FUNCS (NAMESPACE_DECL, namespace_decl)

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

extern struct lto_input_block *
lto_create_simple_input_block (struct lto_file_decl_data *, 
			       enum lto_section_type, const char **, size_t *);
extern void
lto_destroy_simple_input_block (struct lto_file_decl_data *, 
				enum lto_section_type,
				struct lto_input_block *, const char *, size_t);


/* lto-function-in.c */

/* FN is a FUNCTION_DECL.  DATA is the LTO data written out during
   ordinary compilation, encoding the body of FN.  FILE_DATA are the
   tables holding all of the global types and decls used by FN.  Upon
   return, DECL_SAVED_TREE for FN contains the reconstituted body of
   FN and DECL_INITIAL contains the BLOCK tree for the function.
   However, it is not this function's responsibility to provide FN to
   the optimizers or code-generators; that will be done by the
   caller.  */
extern void
lto_input_function_body (struct lto_file_decl_data* file_data,
			tree fn, const char *data);

/* DATA is the LTO data written out during ordinary compilation,
   encoding the initializers for the static and external vars.
   FILE_DATA are the tables holding all of the global types and decls
   used in that file.  */
extern void
lto_input_constructors_and_inits (struct lto_file_decl_data* file_data,
				  const char *data);

/* DATA is the LTO data written out during ordinary compilation,
   encoding the initializers for the static and external vars.
   FILE_DATA are the tables holding all of the global types and decls
   used un that file.   */
extern void
lto_input_cgraph (struct lto_file_decl_data* file_data, const char *data);

/* lto-section-in.c */

extern unsigned int lto_get_flag (unsigned HOST_WIDEST_INT *);
extern unsigned int lto_get_flags (unsigned HOST_WIDEST_INT *, unsigned int);
extern void lto_set_in_hooks (struct lto_file_decl_data **, 
			      lto_get_section_data_f*,
			      lto_free_section_data_f*);
extern struct lto_file_decl_data **lto_get_file_decl_data (void);
extern const char* lto_get_section_data (struct lto_file_decl_data *, enum lto_section_type,
					 const char *, size_t *);
extern void lto_free_section_data (struct lto_file_decl_data *, enum lto_section_type,
				   const char *, const char *, size_t);
extern unsigned char lto_input_1_unsigned (struct lto_input_block *);
extern unsigned HOST_WIDE_INT lto_input_uleb128 (struct lto_input_block *);
extern unsigned HOST_WIDEST_INT lto_input_widest_uint_uleb128 (struct lto_input_block *);
extern HOST_WIDE_INT lto_input_sleb128 (struct lto_input_block *);
extern tree lto_input_integer (struct lto_input_block *, tree);

extern htab_t lto_create_renaming_table (void);
extern void lto_record_renamed_decl (struct lto_file_decl_data *,
				     const char *, const char *);
extern const char *lto_original_decl_name (struct lto_file_decl_data *,
					   const char *);
extern struct lto_in_decl_state *lto_new_in_decl_state (void);
extern void lto_delete_in_decl_state (struct lto_in_decl_state *);
extern hashval_t lto_hash_in_decl_state (const void *);
extern int lto_eq_in_decl_state (const void *, const void *);

extern struct lto_in_decl_state *
  lto_get_function_in_decl_state (struct lto_file_decl_data *, tree);

#ifdef LTO_STREAM_DEBUGGING
extern void lto_debug_in_fun (struct lto_debug_context *, char);
#endif

extern void lto_mark_function_body_in_file (tree);
extern bool lto_function_body_in_file_p (tree);

#endif  /* GCC_LTO_SECTION_IN_H  */
