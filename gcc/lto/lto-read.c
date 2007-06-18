/* Read the gimple representation of a function and it's local
   variables from the memory mapped representation of a a .o file.

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
#include "timevar.h"
#include "dwarf2asm.h"
#include "dwarf2out.h"
#include "output.h"
#include "lto-tags.h"
#include "lto.h"
#include <ctype.h>

struct fun_in
{
  tree *field_decls;
  tree *fn_decls;
  tree *var_decls;
  tree *types;
};


/* Load in the global vars and all of the types from the main symbol
   table.  */
static void
load_globals (struct lto_function_header * header,
	      lto_info_fd *fd,
	      lto_context *context,	
	      struct fun_in *fun_in, 
	      lto_ref in_field_decls[] ATTRIBUTE_UNUSED,
	      lto_ref in_fn_decls[],
	      lto_ref in_var_decls[],
	      lto_ref in_types[])
{
  int i;
  fun_in->field_decls = xcalloc (header->num_field_decls, sizeof (tree*));
  fun_in->fn_decls    = xcalloc (header->num_fn_decls, sizeof (tree*));
  fun_in->var_decls   = xcalloc (header->num_var_decls, sizeof (tree*));
  fun_in->types       = xcalloc (header->num_types, sizeof (tree*));

  /* FIXME: The test for zero section can go away when everything gets
     working.  */
  /*
  for (i=0; i<header->num_field_decls; i++)
    if (in_field_decls[i].section)
      fun_in->field_decls[i] 
        = lto_resolve_field_ref (fd, context, in_field_decls[i]);
  */

  for (i=0; i<header->num_fn_decls; i++)
    if (in_fn_decls[i].section)
      fun_in->fn_decls[i] 
	= lto_resolve_fn_ref (fd, context, &in_fn_decls[i]);

  for (i=0; i<header->num_var_decls; i++)
    if (in_var_decls[i].section)
      fun_in->var_decls[i] 
	= lto_resolve_var_ref (fd, context, &in_var_decls[i]);

  for (i=0; i<header->num_types; i++)
    if (in_types[i].section)
      fun_in->types[i]
	= lto_resolve_type_ref (fd, context, &in_types[i]);
}


void 
lto_read_function_body (lto_info_fd *fd,
			lto_context *context,
			tree fn ATTRIBUTE_UNUSED,
			const void *data)
{
  struct lto_function_header * header 
    = (struct lto_function_header *) data;
  struct fun_in fun_in;
  int32_t fields_offset = sizeof (struct lto_function_header); 
  int32_t fns_offset 
    = fields_offset + 
    (header->num_field_decls * sizeof (lto_ref));
  int32_t vars_offset 
    = fns_offset + 
    (header->num_fn_decls * sizeof (lto_ref));
  int32_t types_offset 
    = vars_offset + 
    (header->num_var_decls * sizeof (lto_ref));

#if 0
  int32_t locals_offset 
    = types_offset + 
    (header->num_types * sizeof (lto_ref));
  int32_t main_offset = locals_offset + header->local_decls_size;
  int32_t string_offset = main_offset + header->main_size;
#endif

  lto_ref *in_field_decls = (lto_ref*)(data + fields_offset);
  lto_ref *in_fn_decls    = (lto_ref*)(data + fns_offset);
  lto_ref *in_var_decls   = (lto_ref*)(data + vars_offset);
  lto_ref *in_types       = (lto_ref*)(data + types_offset);

#if 0
  const char * local_decls = data + locals_offset;
  const char * main_gimple = data + main_offset;
  const char * strings     = data + string_offset;
#endif

  /* No upward compatibility here.  */
  gcc_assert (header->major_version == LTO_major_version);
  gcc_assert (header->minor_version == LTO_minor_version);

  load_globals (header, fd, context, &fun_in, 
		in_field_decls, in_fn_decls, 
		in_var_decls, in_types);
}

#if 0
void
lto_read_function_body (lto_info_fd *fd ATTRIBUTE_UNUSED,
			lto_context *context ATTRIBUTE_UNUSED,
			tree fn,
			const void *data ATTRIBUTE_UNUSED)
{
  tree return_expr;
  tree stmt_list;
  tree_stmt_iterator i;
  tree body;

  /* ??? For now, just pretend the function returns immediately.  */
  return_expr = build1 (RETURN_EXPR, void_type_node, NULL_TREE);
  stmt_list = alloc_stmt_list ();
  i = tsi_start (stmt_list);
  tsi_link_after (&i, return_expr, TSI_NEW_STMT);
  body = build3 (BIND_EXPR, NULL_TREE, NULL_TREE, stmt_list, NULL_TREE);
  DECL_SAVED_TREE (fn) = body;
  DECL_INITIAL (fn) = make_node (BLOCK);

  return;
}
#endif
