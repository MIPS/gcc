// Represent an enum constant.

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

#ifndef GCJX_MODEL_ENUMCONST_HH
#define GCJX_MODEL_ENUMCONST_HH

/// This represents a constant in an enum class.
class model_enum_constant : public model_class
{
  // Arguments that are provided to the enum constructor.
  std::list<ref_expression> arguments;

  // An ordinal value for this constant that can be used inside the
  // compiler.
  int ordinal;

  // True if this constant has a body, false otherwise.
  bool body;

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_FIELD;
  }

public:

  model_enum_constant (const location &);

  void set_arguments (const std::list<ref_expression> &args)
  {
    arguments = args;
  }

  std::list<ref_expression> get_arguments () const
  {
    return arguments;
  }

  void set_has_body ()
  {
    assert (! body);
    body = true;
  }

  bool has_body_p () const
  {
    return body;
  }

  void set_ordinal (int val)
  {
    assert (ordinal == -1);
    assert (val >= 0);
    ordinal = val;
  }

  int get_ordinal () const
  {
    return ordinal;
  }

  void visit (visitor *v);
};

#endif // GCJX_MODEL_ENUMCONST_HH
