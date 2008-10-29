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

You should have received a copy of the GNU General Public License"
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_LTO_TREE_IN_H
#define GCC_LTO_TREE_IN_H

#include "lto-section-in.h"
#include "plugin-api.h"

/* lto-function-in.c */
typedef enum ld_plugin_symbol_resolution ld_plugin_symbol_resolution_t;
DEF_VEC_I(ld_plugin_symbol_resolution_t);
DEF_VEC_ALLOC_I(ld_plugin_symbol_resolution_t, heap);

struct data_in
{
  /* The global decls and types.  */
  struct lto_file_decl_data* file_data;

  /* The offsets to decode the local_decls.  */
  int *local_decls_index;

  /* A table to reconstruct the local_decls.  */
  int *local_decl_indexes;  

#ifdef LTO_STREAM_DEBUGGING
  /* The offsets to decode the local_decls debug info.  */
  int *local_decls_index_d; 
#endif

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

/* Used in lto/lto.c */
extern void lto_static_init_local (void);
extern tree input_tree (struct lto_input_block *, struct data_in *);

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


/* lto-symtab.c */

/* The NEW_VAR (a VAR_DECL) has just been read with resolution RESOLUTION. If
   there is an existing variable with the same name, merge the declaration for
   NEW_VAR with the previous declaration and return the previous
   declaration.  In this case, NEW_VAR must no longer be used by the
   caller.  All other entities referenced from NEW_VAR (including, in
   particular, its type) must already have been merged before this
   function is called.  If the merge fails (due to inconsistencies
   between the declarations), an error message is issued, and
   error_mark_node is returned.  If there is no previous declaration,
   NEW_VAR is returned.  */
extern void lto_symtab_merge_var (tree new_var,
                                  enum ld_plugin_symbol_resolution resolution);

/* Like lto_symtab_merge_var, but for functions.  */
extern void lto_symtab_merge_fn (tree new_fn,
                                 enum ld_plugin_symbol_resolution resolution);

extern tree lto_symtab_prevailing_decl (tree decl);
extern enum ld_plugin_symbol_resolution lto_symtab_get_resolution (tree decl);
extern void lto_symtab_clear_resolution (tree decl);

#endif  /* GCC_LTO_TREE_IN_H  */
