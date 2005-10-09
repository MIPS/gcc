// Represent a literal.

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

#ifndef GCJX_MODEL_LITERAL_HH
#define GCJX_MODEL_LITERAL_HH

#include "model/primitive.hh"

class model_literal_base : public model_expression
{
  bool compute_constant_p ()
  {
    return true;
  }

protected:

  model_literal_base (const location &w)
    : model_expression (w)
  {
  }
};

// We need '**RT' since we have to use the address of the pointer to
// the type.
template<typename T, model_type **RT>
class model_simple_lit : public model_literal_base
{
protected:

  // The value.
  T val;

public:

  model_simple_lit (const location &w, const T &v)
    : model_literal_base (w),
      val (v)
  {
  }

  jvalue value ()
  {
    return jvalue (val);
  }

  std::string string_value ()
  {
    // Let the type handle it, as it already needs the machinery to
    // handle string conversion for expressions.
    return (*RT)->to_string (jvalue (val));
  }

  void resolve (resolution_scope *)
  {
    set_type (*RT);
  }

  void visit (visitor *);
};


typedef model_simple_lit<jchar, &primitive_char_type> model_char_literal;
typedef model_simple_lit<jbyte, &primitive_byte_type> model_byte_literal;
typedef model_simple_lit<jshort, &primitive_short_type> model_short_literal;
typedef model_simple_lit<jint, &primitive_int_type> model_int_literal;
typedef model_simple_lit<jlong, &primitive_long_type> model_long_literal;
typedef model_simple_lit<jfloat, &primitive_float_type> model_float_literal;
typedef model_simple_lit<jdouble, &primitive_double_type> model_double_literal;
typedef model_simple_lit<jboolean, &primitive_boolean_type>
  model_boolean_literal;


class model_string_literal : public model_literal_base
{
protected:

  // The value.
  std::string val;

public:

  model_string_literal (const location &w, const std::string &v)
    : model_literal_base (w),
      val (v)
  {
  }

  jvalue value ()
  {
    // A string doesn't have an ordinary representation.
    abort ();
  }

  std::string string_value ()
  {
    return val;
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};


/// Given an constant expression, return a new literal expression with
/// the same value.
owner<model_expression> create_literal (const owner<model_expression> &);

#endif // GCJX_MODEL_LITERAL_HH
