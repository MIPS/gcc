// Initialization for all decls.

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

// FIXME: we can't include expr.h due to name clashes, so we declare
// this explicitly.
extern "C" rtx init_one_libfunc (const char *);

// FIXME: do we need this?  Global decl list.
tree all_decls;

// Type of a Utf8Const structure, and pointer to it.
tree type_utf8const;
tree type_utf8const_ptr;

// Type of a vtable and a pointer to it.
tree type_dtable;
tree type_dtable_ptr;

// Type of _Jv_MethodSymbol structure.
tree type_method_symbol;
tree type_method_symbol_ptr;
tree type_method_symbol_array;

// Type of a _Jv_Method and a pointer to it.
tree type_method;
tree type_method_ptr;

// Type of a _Jv_Field and a pointer to it.
tree type_field;
tree type_field_ptr;

// Union used in field reflection info.
tree type_field_info_union;

// Array of _Jv_Field objects.
tree type_field_array;

// Type of _Jv_Constants structure.
tree type_constants;

// Type of java.lang.Object, and type of pointer to it.
tree type_object;
tree type_object_ptr;

// Type of java.lang.Class, and type of pointer to it.
tree type_class;
tree type_class_ptr;

// Offset table, address table, and respective pointer types.
tree type_atable;
tree type_atable_ptr;
tree type_otable;
tree type_otable_ptr;

// Various primitive types and some unsigned variants.
tree type_jint;
tree type_jlong;
tree type_jfloat;
tree type_jdouble;
tree type_jboolean;
tree type_jbyte;
tree type_jchar;
tree type_jshort;
tree type_juint;
tree type_julong;
tree type_jushort;
tree type_jubyte;

// A pointer to native code, and a pointer to that.
tree type_nativecode_ptr;
tree type_nativecode_ptr_ptr;
tree type_nativecode_ptr_array;

// Declarations of various functions provided by the runtime.
tree builtin_Jv_Throw;
tree builtin_Jv_MonitorEnter;
tree builtin_Jv_MonitorExit;
tree builtin_Jv_CheckCast;
tree builtin_Jv_IsInstanceOf;
tree builtin_Jv_AllocObject;
tree builtin_Jv_AllocObjectNoFinalizer;
tree builtin_Jv_AllocObjectNoInitNoFinalizer;
tree builtin_Jv_InitClass;
tree builtin_Jv_NewPrimArray;
tree builtin_Jv_NewObjectArray;
tree builtin_Jv_NewMultiArray;
tree builtin_Jv_ThrowBadArrayIndex;
tree builtin_Jv_ThrowNullPointerException;
tree builtin_Jv_CheckArrayStore;
tree builtin_Jv_LookupInterfaceMethodIdx;
tree builtin_Jv_LookupJNIMethod;
tree builtin_Jv_GetJNIEnvNewFrame;
tree builtin_Jv_JNI_PopSystemFrame;
tree builtin_Jv_divI;
tree builtin_Jv_remI;
tree builtin_Jv_divJ;
tree builtin_Jv_remJ;
tree builtin_fmod;

// Vtables for primitive types.
tree boolean_array_vtable;
tree byte_array_vtable;
tree char_array_vtable;
tree short_array_vtable;
tree int_array_vtable;
tree long_array_vtable;
tree float_array_vtable;
tree double_array_vtable;

// Union type used by .class compiler.
tree type_slot_union;
// Fields of this union.
tree field_slot_i;
tree field_slot_j;
tree field_slot_f;
tree field_slot_d;
tree field_slot_o;



static void
pushdecl (tree x)
{
  all_decls = tree_cons (NULL_TREE, x, all_decls);
}

static void
push_super_field (tree this_class, tree super_class)
{
  tree base_decl;
  /* Don't insert the field if we're just re-laying the class out. */ 
  if (TYPE_FIELDS (this_class) && !DECL_NAME (TYPE_FIELDS (this_class)))
    return;
  base_decl = build_decl (FIELD_DECL, NULL_TREE, super_class);
  DECL_IGNORED_P (base_decl) = 1;
  TREE_CHAIN (base_decl) = TYPE_FIELDS (this_class);
  TYPE_FIELDS (this_class) = base_decl;
  DECL_SIZE (base_decl) = TYPE_SIZE (super_class);
  DECL_SIZE_UNIT (base_decl) = TYPE_SIZE_UNIT (super_class);
}

// This is a helper method that is used when building a record type.
// It appends a new field to the type.  It is invalid to call this
// after layout_type has been called on the type.  FIELD is updated to
// be the new field, it can be modified after return.
static void
push_field (tree record, tree &field, const char *name,
	    tree field_type, bool is_private = false)
{
  assert (record != NULL_TREE);
  assert (field_type != NULL_TREE);
  tree new_field = build_decl (FIELD_DECL, get_identifier (name), field_type);
  if (TYPE_FIELDS (record) == NULL_TREE)
    TYPE_FIELDS (record) = new_field;
  else
    TREE_CHAIN (field) = new_field;
  DECL_CONTEXT (new_field) = record;
  DECL_ARTIFICIAL (new_field) = 1;
//   if (is_private)
//     FIELD_PRIVATE (new_field) = 1;
  field = new_field;
}

static void
start_type_initialization ()
{
  type_object = make_node (RECORD_TYPE);
  type_object_ptr = build_pointer_type (type_object);
  type_class = make_node (RECORD_TYPE);
  type_class_ptr = build_pointer_type (type_class);
  type_method = make_node (RECORD_TYPE);
  type_method_ptr = build_pointer_type (type_method);
  type_field = make_node (RECORD_TYPE);
  type_field_ptr = build_pointer_type (type_field);
}

static void
build_utf8_record ()
{
  tree field = NULL_TREE;
  tree type_utf8const = make_node (RECORD_TYPE);
  push_field (type_utf8const, field, "hash", type_jushort);
  push_field (type_utf8const, field, "length", type_jushort);
  push_field (type_utf8const, field, "data",
	      build_array_type (char_type_node, integer_type_node));
  layout_type (type_utf8const);

  type_utf8const_ptr = build_pointer_type (type_utf8const);
}

static void
build_method_symbol_type ()
{
  tree field = NULL_TREE;
  type_method_symbol = make_node (RECORD_TYPE);
  push_field (type_method_symbol, field, "clname", type_utf8const_ptr);
  push_field (type_method_symbol, field, "name", type_utf8const_ptr);
  // A misnomer as of 1.5.
  push_field (type_method_symbol, field, "signature", type_utf8const_ptr);
  layout_type (type_method_symbol);
  type_method_symbol_ptr = build_pointer_type (type_method_symbol);

  tree one_elt_array_domain_type = build_index_type (integer_one_node);
  type_method_symbol_array = build_array_type (type_method_symbol,
					       one_elt_array_domain_type);
}

static void
build_constant_type ()
{
  tree field = NULL_TREE;
  type_constants = make_node (RECORD_TYPE);
  push_field (type_constants, field, "size", type_juint);
  push_field (type_constants, field, "tags", ptr_type_node);
  push_field (type_constants, field, "data", ptr_type_node);
  layout_type (type_constants);
}

static void
build_method_type ()
{
  tree field = NULL_TREE;
  push_field (type_method, field, "name", type_utf8const_ptr);
  // A misnomer as of 1.5.
  push_field (type_method, field, "signature", type_utf8const_ptr);
  push_field (type_method, field, "accflags", type_jushort);
  push_field (type_method, field, "index", type_jushort);
  push_field (type_method, field, "ncode", type_nativecode_ptr);
  push_field (type_method, field, "throws", ptr_type_node);
  layout_type (type_method);
}

static void
build_oa_table_types ()
{
  tree one_elt_array_domain_type = build_index_type (integer_one_node);

  type_atable = build_array_type (ptr_type_node, one_elt_array_domain_type);
  TYPE_NONALIASED_COMPONENT (type_atable) = 1;
  type_atable_ptr = build_pointer_type (type_atable);

  type_otable = build_array_type (integer_type_node,
				  one_elt_array_domain_type);
  TYPE_NONALIASED_COMPONENT (type_otable) = 1;
  type_otable_ptr = build_pointer_type (type_otable);
}

static void
build_field_type ()
{
  tree field = NULL_TREE;
  type_field_info_union = make_node (UNION_TYPE);
  push_field (type_field_info_union, field, "boffset", type_jint);
  push_field (type_field_info_union, field, "addr", ptr_type_node);
  layout_type (type_field_info_union);

  field = NULL_TREE;
  push_field (type_field, field, "name", type_utf8const_ptr);
  push_field (type_field, field, "type", type_class_ptr);
  push_field (type_field, field, "accflags", type_jushort);
  push_field (type_field, field, "bsize", type_jushort);
  push_field (type_field, field, "info", type_field_info_union);
  layout_type (type_field);
}

static void
build_object_type ()
{
  tree field = NULL_TREE;
  push_field (type_object, field, "vtable", type_dtable_ptr, true);
  // If we don't have hash synchronization, then we have an extra slot
  // in the object used for synchronization.
  if (! flag_hash_synchronization)
    push_field (type_object, field, "sync_info", ptr_type_node, true);
  layout_type (type_object);
}

static void
build_class_type ()
{
  tree field = NULL_TREE;
  push_field (type_class, field, "next", type_class_ptr, true);
  push_field (type_class, field, "name", type_utf8const_ptr, true);
  push_field (type_class, field, "accflags", type_jushort, true);
  push_field (type_class, field, "superclass", type_class_ptr, true);
  push_field (type_class, field, "constants", type_constants, true);
  push_field (type_class, field, "methods", type_method_ptr, true);
  push_field (type_class, field, "method_count", type_jshort, true);
  push_field (type_class, field, "vtable_method_count", type_jshort, true);
  push_field (type_class, field, "fields", type_field_ptr, true);
  push_field (type_class, field, "size_in_bytes", type_jint, true);
  push_field (type_class, field, "field_count", type_jshort, true);
  push_field (type_class, field, "static_field_count", type_jshort, true);
  push_field (type_class, field, "vtable", type_dtable_ptr, true);
  push_field (type_class, field, "otable", type_otable_ptr, true);
  push_field (type_class, field, "otable_syms", 
	      build_pointer_type (type_method_symbol_array), true);
  push_field (type_class, field, "atable", type_atable_ptr, true);
  push_field (type_class, field, "atable_syms", 
	      build_pointer_type (type_method_symbol_array), true);
  push_field (type_class, field, "catch_classes", ptr_type_node, true);
  push_field (type_class, field, "interfaces",
	      build_pointer_type (type_class_ptr), true);
  push_field (type_class, field, "loader", ptr_type_node, true);
  push_field (type_class, field, "interface_count", type_jshort, true);
  push_field (type_class, field, "state", type_jbyte, true);
  push_field (type_class, field, "thread", ptr_type_node, true);
  push_field (type_class, field, "depth", type_jshort, true);
  push_field (type_class, field, "ancestors", ptr_type_node, true);
  push_field (type_class, field, "idt", ptr_type_node, true);  
  push_field (type_class, field, "arrayclass", ptr_type_node, true);  
  push_field (type_class, field, "protectionDomain", ptr_type_node, true);
  push_field (type_class, field, "hack_signers", ptr_type_node, true);
  push_field (type_class, field, "chain", ptr_type_node, true);
  push_field (type_class, field, "aux_info", ptr_type_node, true);
  push_super_field (type_class, type_object);
  layout_type (type_class);
}

// Build the union used for representing stack and local variables in
// class files.
static void
build_class_union ()
{
  tree field = NULL_TREE;
  type_slot_union = make_node (UNION_TYPE);
  // Names are chosen to match 'jvalue'.  However we omit those which
  // we don't need -- all types are promoted and all references use
  // Object.
  push_field (type_slot_union, field, "i", type_jint);
  field_slot_i = field;
  push_field (type_slot_union, field, "j", type_jlong);
  field_slot_j = field;
  push_field (type_slot_union, field, "f", type_jfloat);
  field_slot_f = field;
  push_field (type_slot_union, field, "d", type_jdouble);
  field_slot_d = field;
  push_field (type_slot_union, field, "o", type_object_ptr);
  field_slot_o = field;
  layout_type (type_slot_union);
}

static tree
identity_function (tree x)
{
  return x;
}

// Initialize the types and some other things needed by the non-java
// parts of gcc.
static void
initialize_gcc_decls ()
{
  // Note that we don't care whether 'char' is signed, as we don't
  // use it.
  build_common_tree_nodes (false, false);

  size_type_node = make_unsigned_type (POINTER_SIZE);
  set_sizetype (size_type_node);

  // Note that we don't care whether 'double' is short, as we don't
  // use it.
  build_common_tree_nodes_2 (false);

  // Initialize variables for except.c.
  {
    eh_personality_libfunc = init_one_libfunc (USING_SJLJ_EXCEPTIONS
					       ? "__gcj_personality_sj0"
					       : "__gcj_personality_v0");
    lang_eh_runtime_type = identity_function;
  }
}

static void
initialize_gcc_primitive_types ()
{
  // We can just use the generic boolean.
  type_jboolean = boolean_type_node;

  // FIXME: pushdecl seems bogus, do we need it?;

  type_jbyte = make_signed_type (8);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("byte"), type_jbyte));

  type_jubyte = make_unsigned_type (8);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("unsigned byte"),
			type_jubyte));

  type_jchar = make_node (CHAR_TYPE);
  TYPE_PRECISION (type_jchar) = 16;
  fixup_unsigned_type (type_jchar);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("char"), type_jchar));

  type_jshort = make_signed_type (16);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("short"), type_jshort));

  type_jushort = make_unsigned_type (16);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("unsigned short"),
			type_jushort));

  type_jint = make_signed_type (32);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("int"), type_jint));

  type_juint = make_unsigned_type (32);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("unsigned int"),
			type_juint));

  type_jlong = make_signed_type (64);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("long"), type_jlong));

  type_julong = make_unsigned_type (64);
  pushdecl (build_decl (TYPE_DECL, get_identifier ("unsigned long"),
			type_julong));

  type_jfloat = make_node (REAL_TYPE);
  TYPE_PRECISION (type_jfloat) = 32;
  pushdecl (build_decl (TYPE_DECL, get_identifier ("float"), type_jfloat));
  layout_type (type_jfloat);

  type_jdouble = make_node (REAL_TYPE);
  TYPE_PRECISION (type_jdouble) = 64;
  pushdecl (build_decl (TYPE_DECL, get_identifier ("double"), type_jdouble));
  layout_type (type_jdouble);
}

// Return tree that represents a vtable for a primitive array.
static tree
create_primitive_vtable (const char *name)
{
  char buf[50];
  sprintf (buf, "_Jv_%sVTable", name);
  tree r = build_decl (VAR_DECL, get_identifier (buf), ptr_type_node);
  DECL_EXTERNAL (r) = 1;
  return r;
}

static void
initialize_primitive_vtables ()
{
  boolean_array_vtable = create_primitive_vtable ("boolean");
  byte_array_vtable = create_primitive_vtable ("byte");
  char_array_vtable = create_primitive_vtable ("char");
  short_array_vtable = create_primitive_vtable ("short");
  int_array_vtable = create_primitive_vtable ("int");
  long_array_vtable = create_primitive_vtable ("long");
  float_array_vtable = create_primitive_vtable ("float");
  double_array_vtable = create_primitive_vtable ("double");
}

static void
initialize_misc_types ()
{
  // This is just an alias with a "more clear" name.
  type_nativecode_ptr = ptr_type_node;

  // A pointer to a pointer to native code.
  type_nativecode_ptr_ptr = build_pointer_type (type_nativecode_ptr);

  tree one_elt_array_domain_type = build_index_type (integer_one_node);
  type_nativecode_ptr_array = build_array_type (type_nativecode_ptr,
						one_elt_array_domain_type);

  type_dtable = make_node (RECORD_TYPE);
  tree field = NULL_TREE;
  push_field (type_dtable, field, "class", type_class_ptr);
  push_field (type_dtable, field, "methods", type_nativecode_ptr_array);
  layout_type (type_dtable);

  type_dtable_ptr = build_pointer_type (type_dtable);
}

tree
build_address_of (tree fn)
{
  return build1 (ADDR_EXPR, build_pointer_type (TREE_TYPE (fn)), fn);
}

static void
initialize_builtin_functions ()
{
  // I wonder why this isn't created by GCC itself.
  void_list_node = build_tree_list (NULL_TREE, void_type_node);

  tree t = tree_cons (NULL_TREE, type_class_ptr,
		      tree_cons (NULL_TREE, type_jint, void_list_node));
  builtin_Jv_AllocObject
    = gcjx::builtin_function ("_Jv_AllocObject",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_MALLOC (builtin_Jv_AllocObject) = 1;
  builtin_Jv_AllocObject = build_address_of (builtin_Jv_AllocObject);

  builtin_Jv_AllocObjectNoFinalizer
    = gcjx::builtin_function ("_Jv_AllocObjectNoFinalizer",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_MALLOC (builtin_Jv_AllocObjectNoFinalizer) = 1;
  builtin_Jv_AllocObjectNoFinalizer
    = build_address_of (builtin_Jv_AllocObjectNoFinalizer);

  t = tree_cons (NULL_TREE, ptr_type_node, void_list_node);
  builtin_Jv_InitClass
    = gcjx::builtin_function ("_Jv_InitClass",
			      build_function_type (void_type_node,
						   t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_InitClass
    = build_address_of (builtin_Jv_InitClass);

  builtin_Jv_Throw
    = gcjx::builtin_function ("_Jv_Throw",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  // Mark throw_nodes as 'noreturn' functions with side effects.
  TREE_THIS_VOLATILE (builtin_Jv_Throw) = 1;
  TREE_SIDE_EFFECTS (builtin_Jv_Throw) = 1;
  builtin_Jv_Throw
    = build_address_of (builtin_Jv_Throw);

  t = build_function_type (type_jint, void_list_node);
  builtin_Jv_MonitorEnter 
    = gcjx::builtin_function ("_Jv_MonitorEnter", t, 0, NOT_BUILT_IN,
			      NULL, NULL_TREE);
  builtin_Jv_MonitorEnter
    = build_address_of (builtin_Jv_MonitorEnter);
  builtin_Jv_MonitorExit 
    = gcjx::builtin_function ("_Jv_MonitorExit", t, 0, NOT_BUILT_IN,
			      NULL, NULL_TREE);
  builtin_Jv_MonitorExit
    = build_address_of (builtin_Jv_MonitorExit);

  t = tree_cons (NULL_TREE, type_jint, 
		 tree_cons (NULL_TREE, type_jint, void_list_node));
  builtin_Jv_NewPrimArray
    = gcjx::builtin_function ("_Jv_NewPrimArray",
			      build_function_type(ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_MALLOC (builtin_Jv_NewPrimArray) = 1;
  builtin_Jv_NewPrimArray
    = build_address_of (builtin_Jv_NewPrimArray);

  t = tree_cons (NULL_TREE, type_jint,
		 tree_cons (NULL_TREE, type_class_ptr,
			    tree_cons (NULL_TREE, type_object_ptr,
				       void_list_node)));
  builtin_Jv_NewObjectArray
    = gcjx::builtin_function ("_Jv_NewObjectArray",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_MALLOC (builtin_Jv_NewObjectArray) = 1;
  builtin_Jv_NewObjectArray
    = build_address_of (builtin_Jv_NewObjectArray);

  // There is no void_list_node here because _Jv_NewMultiArray is a
  // varargs function.
  t = tree_cons (NULL_TREE, ptr_type_node,
		 tree_cons (NULL_TREE, type_jint, NULL_TREE));
  builtin_Jv_NewMultiArray
    = gcjx::builtin_function ("_Jv_NewMultiArray",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_MALLOC (builtin_Jv_NewMultiArray) = 1;
  builtin_Jv_NewMultiArray
    = build_address_of (builtin_Jv_NewMultiArray);

  t = build_function_type (void_type_node, 
			   tree_cons (NULL_TREE, type_jint, void_list_node));
  builtin_Jv_ThrowBadArrayIndex
    = gcjx::builtin_function ("_Jv_ThrowBadArrayIndex", t, 
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  // Mark this as a 'noreturn' function with side effects.
  TREE_THIS_VOLATILE (builtin_Jv_ThrowBadArrayIndex) = 1;
  TREE_SIDE_EFFECTS (builtin_Jv_ThrowBadArrayIndex) = 1;
  builtin_Jv_ThrowBadArrayIndex
    = build_address_of (builtin_Jv_ThrowBadArrayIndex);

  builtin_Jv_ThrowNullPointerException
    = gcjx::builtin_function ("_Jv_ThrowNullPointerException",
			      build_function_type (void_type_node,
						   void_list_node),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  // Mark this as a 'noreturn' function with side effects.
  TREE_THIS_VOLATILE (builtin_Jv_ThrowNullPointerException) = 1;
  TREE_SIDE_EFFECTS (builtin_Jv_ThrowNullPointerException) = 1;
  builtin_Jv_ThrowNullPointerException
    = build_address_of (builtin_Jv_ThrowNullPointerException);

  t = tree_cons (NULL_TREE, type_class_ptr,
		 tree_cons (NULL_TREE, type_object_ptr, void_list_node));
  builtin_Jv_CheckCast
    = gcjx::builtin_function ("_Jv_CheckCast",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_CheckCast
    = build_address_of (builtin_Jv_CheckCast);

  t = tree_cons (NULL_TREE, type_object_ptr,
		 tree_cons (NULL_TREE, type_class_ptr, void_list_node));
  builtin_Jv_IsInstanceOf
    = gcjx::builtin_function ("_Jv_IsInstanceOf",
			      build_function_type (type_jboolean, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_PURE (builtin_Jv_IsInstanceOf) = 1;
  builtin_Jv_IsInstanceOf
    = build_address_of (builtin_Jv_IsInstanceOf);

  t = tree_cons (NULL_TREE, type_object_ptr,
		 tree_cons (NULL_TREE, type_object_ptr, void_list_node));
  builtin_Jv_CheckArrayStore
    = gcjx::builtin_function ("_Jv_CheckArrayStore",
			      build_function_type (void_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_CheckArrayStore
    = build_address_of (builtin_Jv_CheckArrayStore);

  t = tree_cons (NULL_TREE, ptr_type_node,
		 tree_cons (NULL_TREE, ptr_type_node,
			    tree_cons (NULL_TREE, type_jint, void_list_node)));
  builtin_Jv_LookupInterfaceMethodIdx 
    = gcjx::builtin_function ("_Jv_LookupInterfaceMethodIdx",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  DECL_IS_PURE (builtin_Jv_LookupInterfaceMethodIdx) = 1;
  builtin_Jv_LookupInterfaceMethodIdx
    = build_address_of (builtin_Jv_LookupInterfaceMethodIdx);

  t = tree_cons (NULL_TREE, type_object_ptr,
		 tree_cons (NULL_TREE, ptr_type_node,
			    tree_cons (NULL_TREE, ptr_type_node, 
			               tree_cons (NULL_TREE, type_jint, 
				                  void_list_node))));
  builtin_Jv_LookupJNIMethod
    = gcjx::builtin_function ("_Jv_LookupJNIMethod",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_LookupJNIMethod
    = build_address_of (builtin_Jv_LookupJNIMethod);

  t = tree_cons (NULL_TREE, ptr_type_node, void_list_node);
  builtin_Jv_GetJNIEnvNewFrame
    = gcjx::builtin_function ("_Jv_GetJNIEnvNewFrame",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_GetJNIEnvNewFrame
    = build_address_of (builtin_Jv_GetJNIEnvNewFrame);

  builtin_Jv_JNI_PopSystemFrame
    = gcjx::builtin_function ("_Jv_JNI_PopSystemFrame",
			      build_function_type (ptr_type_node, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_JNI_PopSystemFrame
    = build_address_of (builtin_Jv_JNI_PopSystemFrame);

  builtin_Jv_divI
    = gcjx::builtin_function ("_Jv_divI",
			      build_function_type (type_jint, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_divI
    = build_address_of (builtin_Jv_divI);

  builtin_Jv_remI
    = gcjx::builtin_function ("_Jv_remI",
			      build_function_type (type_jint, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_remI
    = build_address_of (builtin_Jv_remI);

  builtin_Jv_divJ
    = gcjx::builtin_function ("_Jv_divJ",
			      build_function_type (type_jlong, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_divJ
    = build_address_of (builtin_Jv_divJ);

  builtin_Jv_remJ
    = gcjx::builtin_function ("_Jv_remJ",
			      build_function_type (type_jlong, t),
			      0, NOT_BUILT_IN, NULL, NULL_TREE);
  builtin_Jv_remJ
    = build_address_of (builtin_Jv_remJ);

  // FIXME: initialize builtin_fmod.
}

tree
gcjx::builtin_function (const char *name,
			tree type,
			int function_code,
			enum built_in_class cl,
			const char *library_name,
			tree)
{
  tree decl = build_decl (FUNCTION_DECL, get_identifier (name), type);
  DECL_EXTERNAL (decl) = 1;
  TREE_PUBLIC (decl) = 1;
  if (library_name)
    SET_DECL_ASSEMBLER_NAME (decl, get_identifier (library_name));
  make_decl_rtl (decl);
  pushdecl (decl);
  DECL_BUILT_IN_CLASS (decl) = cl;
  DECL_FUNCTION_CODE (decl) = (built_in_function) function_code;
  return decl;
}

void
gcjx::initialize_decls ()
{
  all_decls = NULL_TREE;

  initialize_gcc_decls ();
  initialize_gcc_primitive_types ();
  initialize_primitive_vtables ();

  start_type_initialization ();
  initialize_misc_types ();

  build_oa_table_types ();
  build_utf8_record ();
  build_method_symbol_type ();
  build_method_type ();
  build_field_type ();
  build_constant_type ();
  build_object_type ();
  build_class_type ();
  build_class_union ();
  initialize_builtin_functions ();
}

#include "gt-java-hooks.h"
#include "gtype-java.h"
