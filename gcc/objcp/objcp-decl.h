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

#ifndef GCC_OBJCP_DECL_H
#define GCC_OBJCP_DECL_H

extern tree objcp_start_struct (enum tree_code, tree);
extern tree objcp_finish_struct (tree, tree, tree);
extern int objcp_start_function (tree, tree, tree);
extern void objcp_finish_function (void);
extern tree objcp_start_decl (tree, tree, int, tree);
extern void objcp_finish_decl (tree, tree, tree);
extern tree objcp_lookup_name (tree);
extern tree objcp_push_parm_decl (tree);
extern tree objcp_get_parm_info (int);
extern void objcp_store_parm_decls (void);
extern tree objcp_build_function_call (tree, tree);
extern tree objcp_xref_tag (enum tree_code, tree);
extern tree objcp_grokfield (tree, tree, tree);
extern tree objcp_build_component_ref (tree, tree);
extern tree objcp_build_compound_expr (tree);
extern int objcp_comptypes (tree, tree, int);
extern void objcp_pushlevel (int);
extern tree objcp_builtin_function (const char *, tree, int, 
				    enum built_in_class, const char *, tree);
						 
/* Now "cover up" the corresponding C++ functions if required (NB: the 
   OBJCP_ORIGINAL_FUNCTION macro, shown below, can still be used to
   invoke the original C++ functions if needed).  */
#ifdef OBJCP_REMAP_FUNCTIONS

#define start_struct(code, name) \
	objcp_start_struct (code, name)
#define finish_struct(t, fieldlist, attributes) \
	objcp_finish_struct (t, fieldlist, attributes)
#define start_function(declspecs, declarator, attributes) \
	objcp_start_function (declspecs, declarator, attributes)
#define finish_function() \
	objcp_finish_function ()
#define start_decl(declarator, declspecs, initialized, attributes) \
	objcp_start_decl (declarator, declspecs, initialized, attributes)
#define finish_decl(decl, init, asmspec) \
	objcp_finish_decl (decl, init, asmspec)
#define lookup_name(name) \
	objcp_lookup_name (name)
#define push_parm_decl(parm) \
	objcp_push_parm_decl (parm)
#define get_parm_info(void_at_end) \
	objcp_get_parm_info (void_at_end)
#define store_parm_decls() \
	objcp_store_parm_decls ()
#define build_function_call(function, args) \
	objcp_build_function_call (function, args)
#define xref_tag(code, name) \
	objcp_xref_tag (code, name)
#define grokfield(declarator, declspecs, width) \
	objcp_grokfield (declarator, declspecs, width)
#define build_component_ref(datum, component) \
        objcp_build_component_ref (datum, component)
#define build_compound_expr(list) \
        objcp_build_compound_expr (list)
#define comptypes(type1, type2, flags) \
	objcp_comptypes (type1, type2, flags)
#define pushlevel(flags) \
	objcp_pushlevel (flags)
#define builtin_function(name, type, code, class, libname, attr) \
	objcp_builtin_function(name, type, code, class, libname, attr)

#undef OBJC_TYPE_NAME
#define OBJC_TYPE_NAME(type) \
  (TYPE_NAME (type) && TREE_CODE (TYPE_NAME (type)) == TYPE_DECL \
   ? DECL_NAME (TYPE_NAME (type)) \
   : TYPE_NAME (type))
#undef OBJC_SET_TYPE_NAME
#define OBJC_SET_TYPE_NAME(type, name) \
  if(TYPE_NAME (type) && TREE_CODE (TYPE_NAME (type)) == TYPE_DECL) \
    DECL_NAME (TYPE_NAME (type)) = name;\
  else \
    TYPE_NAME (type) = name;

#define OBJCP_ORIGINAL_FUNCTION(name, args) 	(name)args

/* C++ marks ellipsis-free function parameters differently from C.  */
#undef OBJC_VOID_AT_END
#define OBJC_VOID_AT_END        void_list_node

#endif  /* OBJCP_REMAP_FUNCTIONS */

#endif /* ! GCC_OBJCP_DECL_H */
