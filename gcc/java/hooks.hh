// Declare functions we use in lang hooks.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

#ifndef GCC_JAVA_HOOKS_HH
#define GCC_JAVA_HOOKS_HH

// See decl.cc for documentation of these globals.
extern GTY (()) tree all_decls;
extern GTY (()) tree type_utf8const;
extern GTY (()) tree type_utf8const_ptr;
extern GTY (()) tree type_dtable;
extern GTY (()) tree type_dtable_ptr;
extern GTY (()) tree type_method_symbol;
extern GTY (()) tree type_method_symbol_ptr;
extern GTY (()) tree type_method_symbol_array;
extern GTY (()) tree type_method;
extern GTY (()) tree type_method_ptr;
extern GTY (()) tree type_field;
extern GTY (()) tree type_field_ptr;
extern GTY (()) tree type_field_info_union;
extern GTY (()) tree type_constants;
extern GTY (()) tree type_catch_class;
extern GTY (()) tree type_object;
extern GTY (()) tree type_object_ptr;
extern GTY (()) tree type_class;
extern GTY (()) tree type_class_ptr;
extern GTY (()) tree type_atable;
extern GTY (()) tree type_atable_ptr;
extern GTY (()) tree type_otable;
extern GTY (()) tree type_otable_ptr;
extern GTY (()) tree type_jint;
extern GTY (()) tree type_jlong;
extern GTY (()) tree type_jfloat;
extern GTY (()) tree type_jdouble;
extern GTY (()) tree type_jboolean;
extern GTY (()) tree type_jbyte;
extern GTY (()) tree type_jchar;
extern GTY (()) tree type_jshort;
extern GTY (()) tree type_juint;
extern GTY (()) tree type_julong;
extern GTY (()) tree type_jushort;
extern GTY (()) tree type_jubyte;
extern GTY (()) tree type_nativecode_ptr;
extern GTY (()) tree type_nativecode_ptr_ptr;
extern GTY (()) tree type_nativecode_ptr_array;
extern GTY (()) tree builtin_Jv_Throw;
extern GTY (()) tree builtin_Jv_MonitorEnter;
extern GTY (()) tree builtin_Jv_MonitorExit;
extern GTY (()) tree builtin_Jv_CheckCast;
extern GTY (()) tree builtin_Jv_IsInstanceOf;
extern GTY (()) tree builtin_Jv_AllocObject;
extern GTY (()) tree builtin_Jv_AllocObjectNoFinalizer;
extern GTY (()) tree builtin_Jv_AllocObjectNoInitNoFinalizer;
extern GTY (()) tree builtin_Jv_InitClass;
extern GTY (()) tree builtin_Jv_NewPrimArray;
extern GTY (()) tree builtin_Jv_NewObjectArray;
extern GTY (()) tree builtin_Jv_NewMultiArray;
extern GTY (()) tree builtin_Jv_ThrowBadArrayIndex;
extern GTY (()) tree builtin_Jv_ThrowNullPointerException;
extern GTY (()) tree builtin_Jv_ThrowAbstractMethodError;
extern GTY (()) tree builtin_Jv_CheckArrayStore;
extern GTY (()) tree builtin_Jv_LookupInterfaceMethod;
extern GTY (()) tree builtin_Jv_LookupInterfaceMethodIdx;
extern GTY (()) tree builtin_Jv_LookupJNIMethod;
extern GTY (()) tree builtin_Jv_GetJNIEnvNewFrame;
extern GTY (()) tree builtin_Jv_JNI_PopSystemFrame;
extern GTY (()) tree builtin_Jv_divI;
extern GTY (()) tree builtin_Jv_remI;
extern GTY (()) tree builtin_Jv_divJ;
extern GTY (()) tree builtin_Jv_remJ;
extern GTY (()) tree builtin_Jv_ResolvePoolEntry;
extern GTY (()) tree boolean_array_vtable;
extern GTY (()) tree byte_array_vtable;
extern GTY (()) tree char_array_vtable;
extern GTY (()) tree short_array_vtable;
extern GTY (()) tree int_array_vtable;
extern GTY (()) tree long_array_vtable;
extern GTY (()) tree float_array_vtable;
extern GTY (()) tree double_array_vtable;
extern GTY (()) tree type_slot_union;
extern GTY (()) tree field_slot_i;
extern GTY (()) tree field_slot_j;
extern GTY (()) tree field_slot_f;
extern GTY (()) tree field_slot_d;
extern GTY (()) tree field_slot_o;
extern GTY (()) tree gcj_abi_version;
extern GTY (()) tree class_list;
extern GTY (()) tree resource_list;


extern tree build_address_of (tree);
extern void push_field (tree record, tree &field, const char *name,
			tree field_type, bool is_private = false);
extern void pushdecl (tree);
extern void compile_resource_data (const char *, const char *, int);
extern void compile_resource_file (const char *, const char *);


namespace gcjx
{
  // Driver functions: setup, options, parsing, shutdown.
  int handle_option (size_t, const char *, int);
  unsigned int init_options (unsigned int, const char **);
  bool post_options (const char **);
  bool init ();
  void finish ();
  void parse_file (int);

  // Other stuff.
  tree type_for_mode (enum machine_mode, int);
  tree type_for_size (unsigned, int);
  tree signed_or_unsigned_type (int, tree);
  tree signed_type (tree);
  tree unsigned_type (tree);
  bool mark_addressable (tree);
  tree truthvalue_conversion (tree);
  bool decl_ok_for_sibcall (tree);
  tree builtin_function (const char *name,
			 tree type,
			 int function_code,
			 enum built_in_class cl,
			 const char *library_name,
			 tree);
  void expand_function (tree);
  tree getdecls ();

  void initialize_decls ();
};

#endif // GCC_JAVA_HOOKS_HH
