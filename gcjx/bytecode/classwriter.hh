// Write a class file.

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

#ifndef GCJX_BYTECODE_CLASSWRITER_HH
#define GCJX_BYTECODE_CLASSWRITER_HH

#include "bytecode/outpool.hh"
#include "directory.hh"

class model_method;
class bytecode_generator;
class bytecode_attribute_list;

class class_writer
{
  // The class we're writing.
  model_class *the_class;

  // The output stream.
  bytecode_stream writer;

  // The constant pool for this class.
  outgoing_constant_pool *pool;

  // True if we're targeting 1.5.
  bool target_15;

  void classify_annotations (const std::list<ref_annotation> &,
			     std::list<model_annotation *> &,
			     std::list<model_annotation *> &);
  void maybe_write_annotations (IAnnotatable *, bytecode_attribute_list *);
  void maybe_write_parameter_annotations (model_method *,
					  bytecode_attribute_list *);
  void write_method (model_method *, bytecode_attribute_list *, bool = false);
  void write_field (model_field *, bytecode_attribute_list *);

public:

  class_writer (model_class *);
  ~class_writer ();

  void write (directory_cache &);

  // Convenience method for checking number of dimensions of an array.
  static void check_type (model_element *, model_type *);
};

#endif // GCJX_BYTECODE_CLASSWRITER_HH
