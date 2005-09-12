// Attribute for bytecode output.

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

#ifndef GCJX_BYTECODE_ATTRIBUTE_HH
#define GCJX_BYTECODE_ATTRIBUTE_HH

class bytecode_generator;

/// This represents a single attribute for output to the generated
/// bytecode.
class bytecode_attribute
{
protected:

  /// Constant pool.
  outgoing_constant_pool *pool;

  /// Name of attribute.
  std::string name;

  bytecode_attribute (outgoing_constant_pool *, const std::string &);

public:

  virtual ~bytecode_attribute ();

  /// This emits the contents of this attribute to the output stream.
  /// The base class implementation emits just the name of the
  /// attribute; subclasses must emit the size of the attribute and
  /// its contents.
  virtual void emit (bytecode_stream &);

  /// Compute the size of this attribute.
  virtual int size () = 0;
};

/// An attribute which is just a tag.
class simple_name_attribute : public bytecode_attribute
{
public:

  simple_name_attribute (outgoing_constant_pool *p, const std::string &n)
    : bytecode_attribute (p, n)
  {
  }

  void emit (bytecode_stream &);

  int size ()
  {
    return 0;
  }
};

/// An attribute that consists of a name and a utf8 value.
class utf8_attribute : public bytecode_attribute
{
  // The value.
  std::string value;

public:

  utf8_attribute (outgoing_constant_pool *, const std::string &,
		  const std::string &);

  void emit (bytecode_stream &);

  int size ()
  {
    return 2;
  }
};

/// An attribute that handles InnerClasses for the constant pool.  It
/// simply delegates the work to the pool.
class inner_classes_attribute : public bytecode_attribute
{
public:

  inner_classes_attribute (outgoing_constant_pool *);

  void emit (bytecode_stream &);

  int size ();
};

/// An attribute that handles exceptions for a method.
class exceptions_attribute : public bytecode_attribute
{
  // The exceptions.
  std::list<ref_forwarding_type> excs;

public:

  exceptions_attribute (outgoing_constant_pool *,
			const std::list<ref_forwarding_type> &);

  void emit (bytecode_stream &);

  int size ()
  {
    return 2 + 2 * excs.size ();
  }
};

/// An attribute that handles the code for a method.
class code_attribute : public bytecode_attribute
{
  // The code generator.
  bytecode_generator *gen;

public:

  code_attribute (outgoing_constant_pool *, bytecode_generator *);

  void emit (bytecode_stream &);

  int size ();
};

/// An attribute that handles the constant value of a field.
class field_value_attribute : public bytecode_attribute
{
  // The index of the value.
  int index;

public:

  field_value_attribute (outgoing_constant_pool *, model_field *);

  void emit (bytecode_stream &);

  int size ()
  {
    return 2;
  }
};

/// This represents annotations that are attached to a class, field,
/// or method.
class annotation_attribute : public bytecode_attribute
{
  // Computed length.
  int len;

  // The annotations.
  std::list<model_annotation *> annos;

public:

  annotation_attribute (outgoing_constant_pool *,
			const std::string &,
			const std::list<model_annotation *> &);

  void emit (bytecode_stream &);

  int size ()
  {
    return len;
  }
};

/// This represents annotations attached to method parameters.
class parameter_attribute : public bytecode_attribute
{
  // Computed length.
  int len;

  // The annotations, one list per parameter.
  std::list< std::list<model_annotation *> > annos;

public:

  parameter_attribute (outgoing_constant_pool *,
		       const std::string &,
		       const std::list< std::list<model_annotation *> > &);

  void emit (bytecode_stream &);

  int size ()
  {
    return len;
  }
};

/// This represents an annotation default value.
class annotation_default_attribute : public bytecode_attribute
{
  // Computed length.
  int len;

  // The attribute's default value.
  model_expression *expr;

public:

  annotation_default_attribute (outgoing_constant_pool *, model_expression *);

  void emit (bytecode_stream &);

  int size ()
  {
    return len;
  }
};

/// This holds a list of attributes.  It owns each attribute object
/// and will delete it when finished.  It handles emission of
/// attributes as well.
class bytecode_attribute_list
{
  // The attributes.
  std::list<bytecode_attribute *> attrs;

public:

  bytecode_attribute_list ();
  ~bytecode_attribute_list ();

  void push_back (bytecode_attribute *attr)
  {
    attrs.push_back (attr);
  }

  void emit (bytecode_stream &);

  // Compute the size needed by all attributes.
  int size ();
};

#endif // GCJX_BYTECODE_ATTRIBUTE_HH
