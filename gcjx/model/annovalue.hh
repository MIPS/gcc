// Represent an annotation value.

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

#ifndef GCJX_MODEL_ANNOVALUE_HH
#define GCJX_MODEL_ANNOVALUE_HH

/// This represents a single annotation value.  It is a name-value
/// pair.  This same class is used to represent an annotation type
/// member declaration.
class model_annotation_value : public model_element
{
protected:

  // The name of the annotation.
  std::string name;

  // The value.  For an annotation this is never NULL.  For an
  // annotation type member declaration, it is NULL if there is no
  // default value.
  ref_expression value;

public:

  model_annotation_value (const location &w, const std::string &n,
			  const ref_expression &ex)
    : model_element (w),
      name (n),
      value (ex)
  {
  }

  virtual void resolve (resolution_scope *scope)
  {
    if (value)
      value->resolve (scope);
  }

  std::string get_name () const
  {
    return name;
  }

  void set_value (const ref_expression &e)
  {
    assert (! value);
    value = e;
  }

  model_expression *get_value () const
  {
    return value.get ();
  }

  bool has_value_p () const
  {
    return value;
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_ANNOVALUE_HH
