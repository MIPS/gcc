// Various built-in functions.

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

#ifndef GCC_JAVA_BUILTINS_HH
#define GCC_JAVA_BUILTINS_HH

#include "java/abi.hh"

class tree_builtins : public aot_class_factory
{
  // This maps our types to gcc types.
  std::map<model_type *, tree> typemap;

  // This maps classes to their class objects.
  // FIXME: perhaps this should be attached to the gcc type somehow?
  std::map<model_class *, tree> classobj_map;

  // This maps classes to their vtable decls
  std::map<model_class *, tree> vtable_map;

  // This maps classes to their constant pool decls.
  std::map<model_class *, tree> cpool_map;

  // This maps a method to its corresponding gcc tree.
  std::map<model_method *, tree> methodmap;

  // This maps a field to its corresponding gcc tree.
  std::map<model_field *, tree> fieldmap;

  // This maps a variable or parameter to its corresponding gcc tree.
  std::map<model_variable_decl *, tree> varmap;

  // This maps a length to a concrete Utf8Const type.
  std::map<int, tree> utf8typemap;

  // This maps a string to a gcc tree representing the corresponding
  // Utf8Const.
  std::map<std::string, tree> utf8map;

  // Registry of otable, atable, and itable decls.
  std::map<model_class *, tree> atable_map;
  std::map<model_class *, tree> otable_map;
  std::map<model_class *, tree> itable_map;

  // This maps a containing class to a sub-map, which maps exception
  // classes to individual DECLs for _Jv_CatchClass.
  typedef std::map<model_class *, std::map<model_class *, tree> >
    catch_map_type;
  catch_map_type catch_map;


  // Used when creating symbol names.
  int symbol_count;

  // ABI instances.
  cxx_abi old_abi;
  bc_abi new_abi;

  void add (tree, model_method *);
  void add (tree, model_field *);
  tree map_param_or_var (tree_code, tree, model_variable_decl *);
  void lay_out_vtable (model_class *);
  tree build_utf8const_type (int);
  int hash_utf8 (const char *, int);


  friend class cxx_abi;
  friend class bc_abi;

public:

  tree_builtins ();
  ~tree_builtins ();

  /// Return an identifier node corresponding to a string.
  tree map_identifier (const std::string &);

  /// return ... fixme
  tree map_type (model_type *);
  tree map_parameter (tree, model_variable_decl *);
  tree map_variable (tree, model_variable_decl *);
  tree map_field (model_field *);
  tree map_method (model_method *);

  tree map_field_ref (aot_class *, tree, model_field *);
  tree map_field_ref (aot_class *,
		      tree, const std::string &, const std::string &,
		      const std::string &);

  tree map_method_call (aot_class *, tree, tree, model_method *, bool);
  tree map_method_call (aot_class *, tree, tree, const std::string &,
			const std::string &, const std::string &);

  tree map_new (aot_class *, model_class *, model_method *, tree);
  tree map_class_object (model_class *);

  /// Memoize a utf8const.
  tree map_utf8const (const std::string &);

  tree lay_out_class (model_class *);

  /// Generate code to check a null reference.  REF is the reference
  /// to check.  By default, this only generates a check if the target
  /// architecture requires explicit checks.  If OVERRIDE is true,
  /// then the check will be generated regardless of the target
  /// platform.
  tree check_reference (tree ref, bool override = false);

  tree find_decl (tree, const char *);

  /// This is called to initialize the type map.  It should only be
  /// called once, before any classes are entered into the type map.
  void initialize_type_map ();

  gcj_abi *find_abi ();

  /// Return the mangled name of the class object for a given class.
  std::string get_class_object_name (model_class *);

  /// Return a new unique symbol name.
  tree get_symbol ();

  /// Return the decl for a given class' vtable.
  tree get_vtable_decl (model_class *);

  /// Return the decl for a given class' constant pool.
  tree get_constant_pool_decl (model_class *);

  /// Return some code to initialize a class.
  tree build_class_init (model_class *);

  tree get_atable_decl (model_class *);
  tree get_otable_decl (model_class *);
  tree get_itable_decl (model_class *);

  tree map_catch_class (model_class *, model_class *);

  std::map<model_class *, tree> *get_catch_map (model_class *);

  // Like the get_descriptor() methods, but returns the format wanted
  // by libgcj.  FIXME: why do we have these?
  static std::string get_descriptor (model_type *t)
  {
    // Why do we do this?
    return join (split (t->get_descriptor (), '/'), '.');
  }

  static std::string get_descriptor (model_method *m)
  {
    // Why do we do this?
    return join (split (m->get_descriptor (), '/'), '.');
  }
};

#endif // GCC_JAVA_BUILTINS_HH
