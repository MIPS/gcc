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

#ifndef GCC_TREE_BUILTINS_HH
#define GCC_TREE_BUILTINS_HH

#include "java/abi.hh"

class tree_builtins
{
  // This maps our types to gcc types.
  std::map<model_type *, tree> typemap;

  // This maps a method to its corresponding gcc tree.
  std::map<model_method *, tree> methodmap;

  // This maps a field to its corresponding gcc tree.
  std::map<model_field *, tree> fieldmap;

  // This maps a variable or parameter to its corresponding gcc tree.
  std::map<model_variable_decl *, tree> varmap;

  // ABI instances.
  cxx_abi old_abi;
  bc_abi new_abi;

  void add (tree, model_method *);
  void add (tree, model_field *);
  tree map_param_or_var (tree_code, tree, model_variable_decl *);
  tree find_atable_slot (model_field *);
  tree find_otable_slot (model_field *);
  void lay_out_vtable (model_class *);


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

  tree map_field_ref (tree, model_field *);
  tree map_field_ref (tree, const std::string &, const std::string &,
		      const std::string &);

  tree map_method_call (tree, tree, model_method *, bool);
  tree map_new (model_class *, tree, tree);

  // Memoize a utf8const.
  tree map_utf8const (const std::string &)
  {
    // FIXME.
    abort ();
  }

  tree build_divide (tree, tree, tree);
  tree build_mod (tree, tree, tree);
  tree build_array_reference (tree, tree, tree, bool = true);
  tree build_exception_object_ref (tree);
  tree build_class_ref (tree);

  tree lay_out_class (model_class *);

  tree check_reference (tree);

  tree find_decl (tree, const char *);

  /// This is called to initialize the type map.  It should only be
  /// called once, before any classes are entered into the type map.
  void initialize_type_map ();

  gcj_abi *find_abi (model_type *);
};

#endif // GCC_TREE_BUILTINS_HH
