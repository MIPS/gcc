/* Process the ObjC-specific declarations and variables for
   the Objective-C++ compiler.
   Copyright (C) 2001, 2004 Free Software Foundation, Inc.
   Contributed by Ziemowit Laski  <zlaski@apple.com>

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
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "expr.h"
#include "cp-tree.h"
#include "lex.h"
#include "c-common.h"
#include "flags.h"
#include "input.h"
#include "except.h"
#include "output.h"
#include "toplev.h"
#include "cpplib.h"
#include "debug.h"
#include "target.h"
#include "varray.h"

#include "objc-act.h"
#include "objcp-decl.h"

static GTY(()) tree objcp_parmlist = NULL_TREE;

/* Hacks to simulate start_struct() and finish_struct(). */

tree 
objcp_start_struct (enum tree_code code ATTRIBUTE_UNUSED, tree name)
{
  tree s;
  /* The idea here is to mimic the actions that the C++ parser takes when
     constructing 'extern "C" struct NAME {'.  */
  push_lang_context (lang_name_c);
  if (!name)
    name = make_anon_name ();
  s = xref_tag (record_type, name, true, false);
  CLASSTYPE_DECLARED_CLASS (s) = 0;  /* this is a 'struct', not a 'class' */

  return begin_class_definition (s);
}

tree 
objcp_finish_struct (tree t, tree fieldlist, tree attributes ATTRIBUTE_UNUSED)
{
  tree field, next_field;

  for (field = fieldlist; field; field = next_field)
  {
    next_field = TREE_CHAIN (field);      /* insert one field at a time;  */
    TREE_CHAIN (field) = NULL_TREE;       /* otherwise, grokfield croaks. */
    finish_member_declaration (field);
  }
  TYPE_ATTRIBUTES (t) = NULL_TREE;
  t = finish_struct (t, NULL_TREE);
  pop_lang_context ();

  return t;
}

int
objcp_start_function (tree declspecs, tree declarator, tree attributes)
{
  return start_function (declspecs, declarator, attributes, 0);
}

void
objcp_finish_function (void)
{
  /* The C++ flavor of 'finish_function' does not generate RTL -- one has
     to call 'expand_body' to do that.  */
  expand_body (finish_function (0));
}

tree 
objcp_start_decl (tree declarator, tree declspecs, int initialized, tree attributes)
{
  return start_decl (declarator, declspecs, initialized, 
		     attributes, NULL_TREE);
}
			  
void
objcp_finish_decl (tree decl, tree init, tree asmspec)
{
  cp_finish_decl (decl, init, asmspec, 0);
}

tree
objcp_lookup_name (tree name)
{
  return lookup_name (name, -1);
}

/* Hacks to simulate push_parm_decl() and get_parm_info(). */

tree
objcp_push_parm_decl (tree parm)
{
  /* C++ parms are laid out slightly differently from C parms.  Adjust
     for this here.  */
  TREE_VALUE (parm) = TREE_PURPOSE (parm);
  TREE_PURPOSE (parm) = NULL_TREE;   

  if (objcp_parmlist)
    objcp_parmlist = chainon (objcp_parmlist, parm);
  else
    objcp_parmlist = parm;

  return objcp_parmlist;
}

tree
objcp_get_parm_info (int have_ellipsis)
{
  tree parm_info = finish_parmlist (objcp_parmlist, have_ellipsis);

  /* The C++ notion of a parameter list differs slightly from that of
     C.  Adjust for this.  */
  parm_info = build_tree_list (parm_info, NULL_TREE);
  objcp_parmlist = NULL_TREE;

  return parm_info;
}

void 
objcp_store_parm_decls (void)
{
  /* In C++ land, 'start_function' calls 'store_parm_decls'; hence we
     do not need to do anything here.  */
}

tree
objcp_build_function_call (tree function, tree args)
{
  return build_function_call (function, args);
}

tree
objcp_xref_tag (enum tree_code code ATTRIBUTE_UNUSED, tree name)
{
  return xref_tag (record_type, name, true, false);
}

tree
objcp_grokfield (tree declarator, tree declspecs, tree width)
{     
  return (width) ? grokbitfield (declarator, declspecs, width)
		 : grokfield (declarator, declspecs, 0, 0, 0);
}

tree
objcp_build_component_ref (tree datum, tree component)
{
  /* The 'build_component_ref' routine has been removed from the C++
     front-end, but 'finish_class_member_access_expr' seems to be
     a worthy substitute.  */
  return finish_class_member_access_expr (datum, component);
}

tree
objcp_build_compound_expr (tree list)
{
  tree rest = TREE_CHAIN (list);

  if (TREE_CHAIN (rest))
    rest = objcp_build_compound_expr (rest);
  else
    rest = TREE_VALUE (rest);

  return build_compound_expr (TREE_VALUE (list), rest);
}

int
objcp_comptypes (tree type1, tree type2, int flags ATTRIBUTE_UNUSED)
{     
  return comptypes (type1, type2, 0);
}

void
objcp_pushlevel (int flags ATTRIBUTE_UNUSED)
{
  do_pushlevel (sk_block);
}

/* C++'s version of 'builtin_function' winds up placing our precious
   objc_msgSend and friends in namespace std!  This will not do.
   We shall hence duplicate C's 'builtin_function' here instead.   */
   
tree
objcp_builtin_function (const char *name, tree type, int code,
			enum built_in_class class,
			const char *libname ATTRIBUTE_UNUSED, tree attrs)
{
  tree decl = build_decl (FUNCTION_DECL, get_identifier (name), type);
  DECL_EXTERNAL (decl) = 1;
  TREE_PUBLIC (decl) = 1;
  make_decl_rtl (decl, NULL);
  pushdecl (decl);
  DECL_BUILT_IN_CLASS (decl) = class;
  DECL_FUNCTION_CODE (decl) = code;
  DECL_ANTICIPATED (decl) = 1;

  /* Possibly apply some default attributes to this built-in function.  */
  if (attrs)
    decl_attributes (&decl, attrs, ATTR_FLAG_BUILT_IN);
  else
    decl_attributes (&decl, NULL_TREE, 0);

  return decl;
}

#include "gt-objcp-objcp-decl.h"
