// Resource file compilation.

// Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
// Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#include "java/glue.hh"

/* DOS brain-damage */
#ifndef O_BINARY
#define O_BINARY 0 /* MS-DOS brain-damage */
#endif

// Count of all the resources compiled in this invocation.
static int Jr_count = 0;

void
compile_resource_data (const char *name, const char *buffer, int length)
{
  tree rtype, field = NULL_TREE, data_type, rinit, data, decl;
  char buf[60];

  int len = strlen (name) + length;
  tree idx_type = build_index_type (build_int_cst (sizetype, len - 1));
  data_type = build_array_type (type_jubyte, idx_type);

  rtype = make_node (RECORD_TYPE);
  push_field (rtype, field, "name_length", type_juint);
  push_field (rtype, field, "resource_length", type_juint);
  push_field (rtype, field, "data", data_type);
  layout_type (rtype);

  record_creator creator (rtype);
  creator.set_field ("name_length", 
		     build_int_cst (NULL_TREE, strlen (name)));
  creator.set_field ("resource_length", 
		     build_int_cst (NULL_TREE, length));
  data = build_string (strlen(name) + length, buffer);
  TREE_TYPE (data) = data_type;
  creator.set_field ("data", data);

  rinit = creator.finish_record ();
  TREE_CONSTANT (rinit) = 1;
  TREE_INVARIANT (rinit) = 1;

  /* Generate a unique-enough identifier.  */
  sprintf (buf, "_Jr%d", ++Jr_count);

  decl = build_decl (VAR_DECL, get_identifier (buf), rtype);
  TREE_STATIC (decl) = 1;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  TREE_READONLY (decl) = 1;
  TREE_THIS_VOLATILE (decl) = 0;
  DECL_INITIAL (decl) = rinit;
  layout_decl (decl, 0);
  pushdecl (decl);
  rest_of_decl_compilation (decl, true, 0);
  cgraph_varpool_finalize_decl (decl);

  resource_list = tree_cons (NULL_TREE, decl, resource_list);
}

void
write_resource_constructor ()
{
  tree iter, t, register_resource_fn;

  if (resource_list == NULL_TREE)
    return;

  t = build_function_type_list (void_type_node, ptr_type_node, NULL);
  t = build_decl (FUNCTION_DECL, get_identifier ("_Jv_RegisterResource"), t);
  TREE_PUBLIC (t) = 1;
  DECL_EXTERNAL (t) = 1;
  register_resource_fn = t;

  tree body = NULL_TREE;

  /* Write out entries in the same order in which they were defined.  */
  for (iter = nreverse (resource_list); iter ; iter = TREE_CHAIN (iter))
    {
      t = build_fold_addr_expr (TREE_VALUE (iter));
      t = tree_cons (NULL, t, NULL);
      t = build_function_call_expr (register_resource_fn, t);
      append_to_statement_list (t, &body);
    }

  // FIXME: we should share this with the commented-out case in
  // register_classes().
  cgraph_build_static_cdtor ('I', body, DEFAULT_INIT_PRIORITY);
}

/* Generate a byte array representing the contents of FILENAME.  The
   array is assigned a unique local symbol.  The array represents a
   compiled Java resource, which is accessed by the runtime using
   NAME.  */
void
compile_resource_file (const char *name, const char *filename)
{
  struct stat stat_buf;
  int fd;
  char *buffer;

  fd = open (filename, O_RDONLY | O_BINARY);
  if (fd < 0)
    {
      perror ("Failed to read resource file");
      return;
    }
  if (fstat (fd, &stat_buf) != 0
      || ! S_ISREG (stat_buf.st_mode))
    {
      perror ("Could not figure length of resource file");
      return;
    }

  // FIXME: we seem to just leak BUFFER here.
  buffer = new char[strlen (name) + stat_buf.st_size];
  strcpy (buffer, name);
  read (fd, buffer + strlen (name), stat_buf.st_size);
  close (fd);

  compile_resource_data (name, buffer, stat_buf.st_size);
}
