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



/* ------------------------------------------------------------------------ */

static tree mx_flag PARAMS ((tree));

/* XXX: copied from tree-mudflap.c */
static tree
mx_flag (t)
     tree t;
{
  if (!t)
    abort ();
#define MARK_TREE_MUDFLAPPED(tree)  do { TREE_BOUNDED (tree) = 1; } while (0)
  MARK_TREE_MUDFLAPPED(t);
  return t;
}



/* ------------------------------------------------------------------------ */



/* Initialize the global tree nodes that correspond to mf-runtime.h
   declarations.  */
tree
mflang_lookup_decl (name)
     const char* name;
{
  tree decl = lookup_name (get_identifier (name), 1);
  if (decl == NULL_TREE)
    internal_error ("mudflap: cannot find declaration of `%s' from mf-runtime.h", 
		    name);

  return decl;
}



/* Build and return EXPR_STMT for calling __mf_register on the object
   given by the parameters.  One odd thing: the object's address is
   given by the given assembler label string (since that's all we may
   know about a string literal, or the static data thingie may be out
   of the future scope).  To turn that into a validish C tree, we
   create a weird synthetic VAR_DECL node.
*/ 

tree
mflang_register_call (label, regsize, regtype, regname)
     const char* label;
     tree regsize;
     tree regtype;
     tree regname;
{
  tree decltype, decl;
  tree call_params;
  tree call_stmt;

  /* XXX: would be nicer not to duplicate these. */
  tree mf_register_fndecl = mflang_lookup_decl ("__mf_register");

  /* See gcc-checker's c-bounds.c (declare_private_statics)  */
  decltype = build_array_type (char_type_node, build_index_type (integer_zero_node));
  decl = mx_flag (build_decl (VAR_DECL, get_identifier (label), decltype));

  TREE_STATIC (decl) = 1;
  TREE_READONLY (decl) = 1;
  TREE_ASM_WRITTEN (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  DECL_INITIAL (decl) = NULL_TREE;
  layout_decl (decl, 0);
  TREE_USED (decl) = 1;
  SET_DECL_ASSEMBLER_NAME (decl, get_identifier (label));
  DECL_DEFER_OUTPUT (decl) = 1;
  /* XXX: what else? */
  /* rest_of_decl_compilation (decl, build_string (strlen (label) + 1, label), 1, 0); */
  /* make_decl_rtl (decl,  build_string (strlen (label) + 1, label)); */

  call_params = tree_cons (NULL_TREE,
			   convert (ptr_type_node, 
				    mx_flag (build1 (ADDR_EXPR, 
						     build_pointer_type (TREE_TYPE (decl)),
						     decl))),
			   tree_cons (NULL_TREE, 
				      convert (size_type_node, regsize),
				      tree_cons (NULL_TREE,
						 regtype,
						 tree_cons (NULL_TREE,
							    regname,
							    NULL_TREE))));

  call_stmt = build1 (EXPR_STMT, void_type_node,
		      build_function_call (mf_register_fndecl,
					   call_params));

  return call_stmt;
}



/* Emit a synthetic CTOR function for the current file.  Populate it
   from the enqueued __mf_register calls.  Call the RTL expanders
   inline.  */

void
mflang_flush_calls (enqueued_call_stmt_chain)
     tree enqueued_call_stmt_chain;
{
  /* See profile.c (output_func_start_profiler) */
  tree fnname;
  char *nmplus;
  tree fndecl;
  tree body;

  /* Short-circuit!  */
  if (enqueued_call_stmt_chain == NULL_TREE)
    return;

  /* Create the COMPOUND_STMT that becomes the new function's body.  */
  body = make_node (COMPOUND_STMT);
  COMPOUND_BODY (body) = enqueued_call_stmt_chain;
  enqueued_call_stmt_chain = NULL_TREE;

  /* Create a ctor function declaration.  */
  nmplus = concat (IDENTIFIER_POINTER (get_file_function_name ('I')), "_mudflap", NULL);
  fnname = get_identifier (nmplus);
  free (nmplus);
  fndecl = build_decl (FUNCTION_DECL, fnname,
		       build_function_type (void_type_node, NULL_TREE));
  DECL_EXTERNAL (fndecl) = 0;
  TREE_PUBLIC (fndecl) = ! targetm.have_ctors_dtors;
  TREE_USED (fndecl) = 1;
  DECL_RESULT (fndecl) = build_decl (RESULT_DECL, NULL_TREE, void_type_node);

  /* Now compile the sucker as we go.  This is a weird semi-inlined
  form of the guts of the c-parse.y `fndef' production, and a hybrid
  with c_expand_body. */

  /* start_function */
  fndecl = pushdecl (fndecl);
  pushlevel (0);
  rest_of_decl_compilation (fndecl, 0, 1, 0);
  announce_function (fndecl);
  current_function_decl = fndecl;
  DECL_INITIAL (fndecl) = error_mark_node;
  DECL_SAVED_TREE (fndecl) = body;
  make_decl_rtl (fndecl, NULL);

  /* store_parm_decls */
  init_function_start (fndecl, input_filename, lineno);
  cfun->x_whole_function_mode_p = 1;

  /* finish_function */
  poplevel (1, 0, 1);
  BLOCK_SUPERCONTEXT (DECL_INITIAL (fndecl)) = fndecl;

  /* c_expand_body */
  expand_function_start (fndecl, 0);
  expand_stmt (DECL_SAVED_TREE (fndecl));
  if (lang_expand_function_end)
    (*lang_expand_function_end) ();
  expand_function_end (input_filename, lineno, 0);
  rest_of_compilation (fndecl);
  if (! quiet_flag) 
    fflush (asm_out_file);
  current_function_decl = NULL_TREE;

  if (targetm.have_ctors_dtors)
    (* targetm.asm_out.constructor) (XEXP (DECL_RTL (fndecl), 0),
                                     DEFAULT_INIT_PRIORITY);
  else
    static_ctors = tree_cons (NULL_TREE, fndecl, static_ctors);
}
