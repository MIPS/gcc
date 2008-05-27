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

/* Hash table entry to hold the start offset and length of an lto
   section in a .o file.  */

struct lto_section_slot {
  const char * name;
  unsigned int start;
  unsigned int len;
};


/* One of these is allocated for each object file that being compiled
   by lto.  This structure contains the tables that are needed for the
   by the serialized functions and ipa passes to connect themselves to
   the global types and decls as they are reconstituted.  */
struct lto_file_decl_data
{
  tree *field_decls;            /* The field decls.  */
  tree *fn_decls;               /* The function decls.  */
  tree *var_decls;              /* The global or static var_decls.  */
  tree *type_decls;             /* The type_decls.  */
  tree *namespace_decls;        /* The namespace_decls.  */
  tree *types;                  /* All of the types.  */
  unsigned int num_field_decls; /* The number of field decls.  */
  unsigned int num_fn_decls;    /* The number of function decls.  */
  unsigned int num_var_decls;   /* The number of global or static var_decls.  */
  unsigned int num_type_decls;  /* The number of type_decls.  */
  unsigned int num_namespace_decls; /* The number of namespace_decls.  */
  unsigned int num_types;       /* All number of of the types.  */

  /* The .o file that these offsets relate to.  

  FIXME!!! This will most likely have to be upgraded if the .o files
  have been archived.  */ 
  const char * file_name;

  /* Hash table to contains the location of the lto bodies in file_name. */
  htab_t section_hash_table;
};


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

extern unsigned char lto_input_1_unsigned (struct lto_input_block *);
extern unsigned HOST_WIDE_INT lto_input_uleb128 (struct lto_input_block *);
extern unsigned HOST_WIDEST_INT lto_input_widest_uint_uleb128 (struct lto_input_block *);
extern HOST_WIDE_INT lto_input_sleb128 (struct lto_input_block *);
extern tree lto_input_integer (struct lto_input_block *, tree);
extern char *lto_get_section_data (struct lto_file_decl_data *, enum lto_section_type,
				   const char*);

#ifdef LTO_STREAM_DEBUGGING
extern void lto_debug_in_fun (struct lto_debug_context *, char);
#endif

#endif  /* GCC_LTO_SECTION_IN_H  */
