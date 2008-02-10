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
  tree *types;                  /* All of the types.  */
  unsigned int num_field_decls; /* The number of field decls.  */
  unsigned int num_fn_decls;    /* The number of function decls.  */
  unsigned int num_var_decls;   /* The number of global or static var_decls.  */
  unsigned int num_type_decls;  /* The number of type_decls.  */
  unsigned int num_types;       /* All number of of the types.  */
};

unsigned char lto_input_1_unsigned (struct lto_input_block *);
unsigned HOST_WIDE_INT lto_input_uleb128 (struct lto_input_block *);
unsigned HOST_WIDEST_INT lto_input_widest_uint_uleb128 (struct lto_input_block *);
HOST_WIDE_INT lto_input_sleb128 (struct lto_input_block *);
tree lto_input_integer (struct lto_input_block *, tree);

#ifdef LTO_STREAM_DEBUGGING
void lto_debug_in_fun (struct lto_debug_context *, char);
void dump_debug_stream (struct lto_input_block *, char, char);
#endif

#endif  /* GCC_LTO_SECTION_IN_H  */
