// Create a Class instance.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

#ifndef GCC_TREE_CLASSOBJ_HH
#define GCC_TREE_CLASSOBJ_HH

/// This is used to create an instance of a given record.
class record_creator
{
  // Constructor we're building.
  tree constructor;

  // Iterator over fields of class type.
  tree field_iterator;

public:

  record_creator (tree record_type);
  ~record_creator ();

  /// Set the field with the indicated name to the given value.
  /// Fields must be set in the order in which they are declared.
  void set_field (const char *, tree);

  /// Wrap up and return the result.
  tree finish_record ();
};

/// This class handles creating a tree representing the Class object
/// that we write out.
class class_object_creator
{
  // The builtins we use.
  tree_builtins *builtins;

  tree make_decl (tree, tree);
  tree create_one_field_record (model_field *);
  tree create_field_array (model_class *, int &, int &);
  tree create_method_array (model_class *, int &);
  void handle_interfaces (model_class *, tree &, tree &);

public:

  class_object_creator (tree_builtins *b)
    : builtins (b)
  {
  }

  tree create_class_instance (model_class *, tree);
};

#endif // GCC_TREE_CLASSOBJ_HH
