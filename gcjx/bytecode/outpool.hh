// A constant pool being generated.

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_BYTECODE_OUTPOOL_HH
#define GCJX_BYTECODE_OUTPOOL_HH

#include "bytecode/outstream.hh"

/// This class represents a constant pool that we are generating.
/// Anything that can be put in a constant pool (class, method,
/// string, double, etc) can be added.  This class keeps track of the
/// values that are added and will only add a given item once.  It
/// automatically handles all the processing associated with inner
/// classes.  When finished it can write itself to the output stream.
class output_constant_pool
{
  // Number of elements in the pool.
  int capacity;

  // Offset of saved position in output stream.
  int offset;

  // True if this constant pool has been shut down.
  bool finished;

  // The class whose constant pool this will be.
  model_class *the_class;

  // The result.
  bytecode_stream &bytes;

  // This is used to map a type to an index.
  typedef std::map<model_type *, int> type_map_type;

  // Each of these maps an item in the constant pool to an index in
  // the pool.
  type_map_type type_map;
  std::map<model_method *, type_map_type> method_map;
  std::map<model_field *, type_map_type> field_map;
  std::map<int, int> name_type_map;
  std::map<jint, int> int_map;
  std::map<jlong, int> long_map;
  // Note that these two are indexed by their representation as words,
  // not as float/double.  That way we avoid clashes when bit patterns
  // differ but operator== does not.
  std::map<jint, int> float_map;
  std::map<jlong, int> double_map;
  std::map<std::string, int> string_map;
  std::map<std::string, int> utf_map;

  // This holds all the nested classes referred to.
  std::set<model_class *> nested_classes;

  // If we made a fake compilation unit, this holds a pointer to it.
  model_unit_class *fake_unit;

  template<typename T> int check (std::map<T, int> &, T);
  template<typename T> int check_qual (std::map<T, type_map_type> &,
				       model_type *, T);

  int add_name_and_type (const std::string &, const std::string &);
  void add_if_inner (model_type *);
  std::string get_descriptor (model_type *);
  void increase_capacity (size_t amount);
  void update_descriptor (std::map<std::string, model_class *> &,
			  model_type *);
  void update_descriptor (std::map<std::string, model_class *> &,
			  model_method *);

public:

  output_constant_pool (model_class *, bytecode_stream &);
  ~output_constant_pool ();

  int add_utf (const std::string &);
  int add (model_type *);
  int add (model_type *, model_method *);
  int add (model_type *, model_field *);
  int add (jint);
  int add (jlong);
  int add (jfloat);
  int add (jdouble);
  int add (const std::string &);

  void finish ();

  /// Return true if this constant pool requires an InnerClasses
  /// attribute.
  bool inner_classes_p ();

  /// Write the inner classes attribute.
  void write_inner_classes ();

  /// Create a fake compilation unit object from this outgoing
  /// constant pool.  This can only be called after finish().  This is
  /// used when verifying bytecode after creating it.
  model_unit_class *get_fake_compilation_unit (const location &,
					       model_class *);
};

#endif // GCJX_BYTECODE_OUTPOOL_HH
