// Per-class wrapper for AOT compilers.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_AOT_AOTCLASS_HH
#define GCJX_AOT_AOTCLASS_HH

class aot_class_factory;

/// These must be kept in sync with libgcj.
typedef enum
{
  JV_ASSERT_END_OF_TABLE = 0,
  JV_ASSERT_TYPES_COMPATIBLE = 1,
  JV_ASSERT_IS_INSTANTIABLE = 2
} type_assertion_type;

/// This class is used to keep track of per-class data for an AOT
/// compiler.
class aot_class
{
  /// This represents one entry in the constant pool.
  struct pool_entry
  {
    /// Type tag.
    classfile_type_constant tag;
    /// String value or class name.
    std::string value;
  };

  /// This is used in type assertions.
  struct type_assertion_entry
  {
    type_assertion_type key;
    model_class *base;
    model_class *derived;
  };

  /// The class factory.  FIXME: require it as a parameter and not
  /// keep it as state?
  aot_class_factory *factory;

  /// The class we wrap.
  model_class *klass;

  /// The constant pool.
  std::vector<pool_entry> pool;

  /// The vtable.
  std::vector<model_method *> vtable;
  /// True if vtable laid out.
  bool vtable_done;

  /// Type assertions.
  std::vector<type_assertion_entry> type_assertions;

  /// All the classes we registered as needing 'new'.
  std::set<model_class *> new_set;

  /// The otable, atable, and itable.
  std::vector<model_element *> otable_map;
  std::vector<model_element *> atable_map;
  std::vector<model_element *> itable_map;


  // Compute the vtable.
  void lay_out_vtable ();

  // Add an item to the constant pool.
  int add_item (const pool_entry &);

  // Helper for putting things into one of the tables.
  int register_something (std::vector<model_element *> &, model_element *);

public:

  aot_class (aot_class_factory *, model_class *);

  /// Add a string to the constant pool.  Returns the index of the
  /// item.
  int add (const std::string &);

  /// Add a Utf8Const to the constant pool.  Returns the index of this
  /// item.
  int add_utf (const std::string &);

  /// Add a class reference to the constant pool.  Returns the index
  /// of the item.
  int add (model_class *);

  /// Add an assertion that BASE is a superclass or superinterface of
  /// DERIVED.
  void add_type_assertion (model_class *base, model_class *derived);

  /// Call this to indicate a class must be instantiable at runtime.
  void note_new (model_class *);

  /// Register a method that is called from the body of this class.
  /// The method is added to the atable (if static) or otable (if not
  /// static).  Returns the index into the table.
  int register_indirect_call (model_method *);

  /// Register an interface method that is called from the body of
  /// this class.  The method is added to the itable.  Returns the
  /// index into the table.
  int register_interface_call (model_method *);

  /// Register a field reference that is made from the body of this
  /// class.  The field is added to the atable or otable as
  /// appropriate.  Returns the index into the table.
  int register_field_reference (model_field *);

  /// Find index of a method in the vtable.
  int find_in_vtable (model_method *);

  /// See whether this class has a finalizer.  This is useful if the
  /// AOT compiler wants to call different allocators depending on
  /// finalizability.
  bool finalizeable_p ();

  /// Return the vtable.
  const std::vector<model_method *> &get_vtable ()
  {
    lay_out_vtable ();
    return vtable;
  }

  const std::vector<model_element *> &get_otable () const
  {
    return otable_map;
  }

  const std::vector<model_element *> &get_atable () const
  {
    return atable_map;
  }

  const std::vector<model_element *> &get_itable () const
  {
    return itable_map;
  }

  model_class *get () const
  {
    return klass;
  }
};

#endif // GCJX_AOT_AOTCLASS_HH
