/* Stub functions for Objective-C and Objective-C++ routines
   that are called from within the C and C++ front-ends,
   respectively.
   Copyright (C) 1991, 1995, 1997, 1998,
   1999, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
#include "tree.h"
#include "c-common.h"

tree
objc_is_class_name (tree arg ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_is_id (tree arg ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_is_object_ptr (tree arg ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_lookup_ivar (tree arg ATTRIBUTE_UNUSED)
{
  return 0;
}

void
objc_check_decl (tree decl ATTRIBUTE_UNUSED)
{
}
   
int
objc_is_reserved_word (tree ident ATTRIBUTE_UNUSED)
{
  return 0;
}

int
objc_comptypes (tree lhs ATTRIBUTE_UNUSED, tree rhs ATTRIBUTE_UNUSED,
                int reflexive ATTRIBUTE_UNUSED)
{
  return -1;
}

tree
objc_message_selector (void)
{ 
  return 0;
}

void
objc_declare_alias (tree alias ATTRIBUTE_UNUSED, tree orig ATTRIBUTE_UNUSED)
{
}

void
objc_declare_class (tree list ATTRIBUTE_UNUSED)
{
}

void
objc_declare_protocols (tree list ATTRIBUTE_UNUSED)
{
}

void
objc_start_protocol (tree proto ATTRIBUTE_UNUSED,
		     tree protorefs ATTRIBUTE_UNUSED)
{
}

void
objc_start_class_interface (tree name ATTRIBUTE_UNUSED,
			    tree super ATTRIBUTE_UNUSED,
			    tree protos ATTRIBUTE_UNUSED)
{
}

void
objc_start_category_interface (tree name ATTRIBUTE_UNUSED,
			       tree categ ATTRIBUTE_UNUSED,
			       tree protos ATTRIBUTE_UNUSED)
{
}

void
objc_continue_interface (void)
{
}

void
objc_finish_interface (void)
{
}

void
objc_add_instance_variable (tree decl ATTRIBUTE_UNUSED)
{
}

void
objc_set_visibility (int vis ATTRIBUTE_UNUSED)
{
}

void
objc_set_method_type (enum tree_code code ATTRIBUTE_UNUSED)
{
}

void
objc_start_class_implementation (tree name ATTRIBUTE_UNUSED,
				 tree super ATTRIBUTE_UNUSED)
{
}

void
objc_start_category_implementation (tree name ATTRIBUTE_UNUSED,
				    tree categ ATTRIBUTE_UNUSED)
{
}

void
objc_continue_implementation (void)
{
}

void
objc_clear_super_receiver (void)
{
}

void
objc_finish_implementation (void)
{
}

void
objc_add_method_declaration (tree signature ATTRIBUTE_UNUSED)
{
}

void
objc_start_method_definition (tree signature ATTRIBUTE_UNUSED)
{
}

void
objc_finish_method_definition (tree fndecl ATTRIBUTE_UNUSED)
{
}

tree
objc_build_keyword_decl (tree selector ATTRIBUTE_UNUSED,
			 tree typename ATTRIBUTE_UNUSED,
			 tree identifier ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_build_method_signature (tree rettype ATTRIBUTE_UNUSED,
			     tree selectors ATTRIBUTE_UNUSED,
			     tree optparms ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_build_encode_expr (tree expr ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_build_protocol_expr (tree expr ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_build_selector_expr (tree expr ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_build_message_expr (tree expr ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_build_string_object (tree str ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_get_class_reference (tree name ATTRIBUTE_UNUSED)
{
  return 0;
}

tree
objc_get_protocol_qualified_type (tree name ATTRIBUTE_UNUSED,
				  tree protos ATTRIBUTE_UNUSED)
{
  return 0;
}

int objc_static_init_needed_p (void)
{
  return 0;
}

tree
objc_generate_static_init_call (tree ctors ATTRIBUTE_UNUSED)
{
  return 0;
}

int
objc_is_public (tree expr ATTRIBUTE_UNUSED, tree identifier ATTRIBUTE_UNUSED)
{
  return 1;
}
