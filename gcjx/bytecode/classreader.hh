// Read a class and put it in the model.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_BYTECODE_CLASSREADER_HH
#define GCJX_BYTECODE_CLASSREADER_HH

#include "buffer.hh"

typedef enum
{
  ATTR_UNKNOWN = 0,
  ATTR_SOURCEFILE = 1,
  ATTR_CONSTANTVALUE = 2,
  ATTR_CODE = 4,
  ATTR_EXCEPTIONS = 8,
  ATTR_LINENUMBERTABLE = 16,
  ATTR_LOCALVARIABLETABLE = 32,
  ATTR_DEPRECATED = 64,
  ATTR_SYNTHETIC = 128,
  ATTR_ENCLOSINGMETHOD = 256,
  ATTR_SIGNATURE = 512,
  ATTR_INNERCLASSES = 1024,
  ATTR_LOCALVARIABLETYPETABLE = 2048,
  ATTR_RUNTIMEVISIBLEANNOTATIONS = 4096,
  ATTR_RUNTIMEINVISIBLEANNOTATIONS = 8192,
  ATTR_RUNTIMEVISIBLEPARAMETERANNOTATIONS = 16384,
  ATTR_RUNTIMEINVISIBLEPARAMETERANNOTATIONS = 32768,
  ATTR_ANNOTATIONDEFAULT = 65536
} class_known_attribute;

/// This is used by the compiler to read classes and define parts of
/// the model.  It knows how to read a single .class file and return
/// the corresponding model_class object.
class class_reader
{
  /// The underlying bytes.
  byte_buffer *buffer;

  /// Pointer to current location.
  const uint8 *current;

  /// Location to use in error messages.
  location where;

  /// The constant pool.
  constant_pool *pool;

  /// List of all interfaces of this class.
  std::list<ref_forwarding_type> interfaces;

  /// Unsplit name of this class.
  std::string unsplit_name;

  /// Fully qualified name of this class.
  std::list<std::string> self_name;

  /// True if this class is discovered to be a member class.
  bool is_member;

  /// Name of our outer class, only valid if this is a member class.
  std::string outer_name;

  /// The class we are going to return.
  ref_class result;

  /// If non-null, field we are currently parsing.
  ref_field current_field;

  /// If non-null, method we are currently parsing.
  ref_method current_method;

  /// If non-null, current bytecode block.
  ref_bytecode_block current_block;

  /// If non-null, current annotation member.
  ref_annotation_member current_annotation_member;

  /// Current list of annotations.
  std::list<ref_annotation> current_annotations;

  /// This maps inner class names to their corresponding containing
  /// class.
  std::map<std::string, model_unit_class::inner_class> inner_infos;

  /// The access flags for the class.
  uint16 access_flags;

  /// Some information about the current method.
  std::list<ref_forwarding_type> current_exceptions;
  std::list<ref_forwarding_type> current_method_arguments;


  // Some internal helper methods.
  void parse_constant_value ();
  uint8 parse_constant_value (ref_expression &, bool = false);
  ref_expression parse_simple_annotation_value (uint8);
  ref_expression parse_annotation_value ();
  ref_annotation parse_annotation ();
  void parse_annotations ();
  void parse_parameter_annotations ();
  void parse_annotation_default ();
  void parse_synthetic ();
  void parse_deprecated ();
  void parse_exception_table ();
  void parse_code ();
  void parse_inner_classes ();
  void parse_exceptions ();
  void parse_enclosing_method ();
  void parse_signature ();
  void parse_attributes (int);
  void parse_method ();
  void parse_field ();
  bool parse_header ();
  void parse_constant_pool ();
  void parse_self ();
  void parse_methods ();
  void parse_fields ();
  void parse_class_attributes ();
  void create_class ();

  uint8 read_u1 ();
  uint16 read_u2 ();
  uint32 read_u4 ();

  class_file_error error (const char *) const;
  class_known_attribute find_attribute_type (const std::string &);

  ref_forwarding_type parse_field_descriptor (uint16);
  ref_forwarding_type one_type (const std::string &, unsigned int &);
  ref_forwarding_type parse_method_descriptor (const std::string &,
					       std::list<ref_forwarding_type> &);
  model_class *find_outer_class (const std::string &);

public:

  /// Create a new class_reader.
  /// @param buf The buffer holding the bytes of the class
  class_reader (byte_buffer *);

  /// Destroy this class reader.
  ~class_reader ();

  /// Parse the .class file.  Argument is the name of the class file.
  /// @return the corresponding model_class object
  ref_unit parse (const std::string &);
};

#endif // GCJX_BYTECODE_CLASSREADER_HH
