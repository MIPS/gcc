/* Mudflap: narrow-pointer bounds-checking by tree rewriting:
   C++ front-end interface.

   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */


#include "config.h"
#include "errors.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-inline.h"
#include "cp-tree.h"
#include "c-common.h"
#include "diagnostic.h"
#include "output.h"
#include "varray.h"
#include "tree-mudflap.h"
#include "target.h"
#include "flags.h"
#include "rtl.h"
#include "toplev.h"


/* Initialize the global tree nodes that correspond to mf-runtime.h
   declarations.  */
tree
mflang_lookup_decl (const char* name)
{
  tree decl = lookup_name (get_identifier (name), 1);
  if (decl == NULL_TREE)
    internal_error ("mudflap: cannot find declaration of `%s' from mf-runtime.h",
		    name);

  return decl;
}



/* Build and return an EXPR for calling __mf_register on the object
   given by the parameters.  One odd thing: the object's address is
   given by the given assembler label string (since that's all we may
   know about a string literal, or the static data thingie may be out
   of the future scope).  To turn that into a validish C tree, we
   create a weird synthetic VAR_DECL node.  */

tree
mflang_register_call (const char* label, tree regsize, tree regtype,
		      tree regname)
{
  tree decltype, decl, params, call, t;

  /* XXX: would be nicer not to duplicate these. */
  tree mf_register_fndecl = mflang_lookup_decl ("__mf_register");

  /* See gcc-checker's c-bounds.c (declare_private_statics)  */
  decltype = build_array_type (char_type_node, 0);
  decl = mf_mark (build_decl (VAR_DECL, get_identifier (label), decltype));

  TREE_STATIC (decl) = 1;
  TREE_READONLY (decl) = 1;
  TREE_ASM_WRITTEN (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  DECL_INITIAL (decl) = NULL_TREE;
  layout_decl (decl, 0);
  TREE_USED (decl) = 1;
  SET_DECL_ASSEMBLER_NAME (decl, get_identifier (label));
  DECL_DEFER_OUTPUT (decl) = 1;

  params = tree_cons (NULL_TREE, regname, NULL_TREE);
  params = tree_cons (NULL_TREE, regtype, params);

  t = convert (size_type_node, regsize);
  params = tree_cons (NULL_TREE, t, params);

  t = build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (decl)), decl);
  t = mf_mark (t);
  t = convert (ptr_type_node, t);
  params = tree_cons (NULL_TREE, t, params);

  call = build_function_call (mf_register_fndecl, params);

  return call;
}



/* Emit a synthetic CTOR function for the current file.  Populate it from
   the enqueued __mf_register calls.  Register it with the constructors.  */

void
mflang_flush_calls (tree enqueued_call_stmt_chain)
{
  tree fnname, fndecl, body;

  /* Short-circuit!  */
  if (enqueued_call_stmt_chain == NULL_TREE)
    return;

  /* Create a ctor function declaration.  */
  fnname = get_identifier ("__static_initialization_and_destruction_mudflap");

  start_function (void_list_node,
		  make_call_declarator (fnname, void_list_node, NULL_TREE,
					NULL_TREE),
		  NULL_TREE, SF_DEFAULT);

  TREE_PUBLIC (current_function_decl) = 0;
  DECL_ARTIFICIAL (current_function_decl) = 1;

  /* Generate the body, one statement at a time.  */
  body = begin_compound_stmt (/*has_no_scope=*/false);

  while (enqueued_call_stmt_chain)
    {
      tree next = TREE_CHAIN (enqueued_call_stmt_chain);
      finish_expr_stmt (enqueued_call_stmt_chain);
      enqueued_call_stmt_chain = next;
    }

  finish_compound_stmt (body);
  fndecl = finish_function (0);

  /* NB: We cannot call expand_or_defer_fn here, since that goes through
     the callgraph queue.  This queue will have already been processed by the
     time this function is running.  */
  expand_body (fndecl);
  if (targetm.have_ctors_dtors)
    (* targetm.asm_out.constructor) (XEXP (DECL_RTL (fndecl), 0),
                                     DEFAULT_INIT_PRIORITY);
  else
    /* By this time, it's too late to do this:
       static_ctors = tree_cons (NULL_TREE, fndecl, static_ctors); */
    abort ();
}
