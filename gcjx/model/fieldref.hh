// A reference to a field.

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

#ifndef GCJX_MODEL_FIELDREF_HH
#define GCJX_MODEL_FIELDREF_HH

class model_field_ref : public model_expression
{
  // The resolved field.
  model_field *field;

  // The field name.
  std::string name;

  // The expression for 'this', if any.
  ref_expression expression;

  // The qualifying class.  This is set during resolution and will
  // never be NULL after then, even if this is an unqualified
  // reference.
  model_class *qualifier;

  // If not NULL, the lexically enclosing class.  This is used by
  // 'class.super.field'.
  ref_forwarding_type enclosing;

  // Is 'super.field' or 'class.super.field'.
  bool super;

  // True if 'expression' was constructed synthetically.
  bool synthetic_expr;

  // True if this field reference was qualified.
  bool qualified;

  // True if this field is the left hand side of an assignment.
  bool is_left_hand_side;

  // True if this field is in a compound assignment.
  bool is_compound;

  bool compute_constant_p ()
  {
    return (! expression || synthetic_expr) && field->constant_p ();
  }

public:

  model_field_ref (const location &w)
    : model_expression (w),
      field (NULL),
      qualifier (NULL),
      super (false),
      synthetic_expr (false),
      qualified (false),
      is_left_hand_side (false),
      is_compound (false)
  {
  }

  void set_field (const std::string &n)
  {
    name = n;
  }

  void set_field (model_field *f)
  {
    assert (! field);
    field = f;
  }

  void set_super ()
  {
    super = true;
  }

  void set_qualified (bool q)
  {
    qualified = q;
  }

  bool qualified_p () const
  {
    return qualified || (expression && ! synthetic_expr);
  }

  model_field *get_field () const
  {
    return field;
  }

  model_class *get_qualifying_class () const
  {
    return qualifier;
  }

  void set_enclosing_class (const ref_forwarding_type &n)
  {
    enclosing = n;
  }

  void set_expression (const ref_expression &e)
  {
    expression = e;
  }

  void resolve (resolution_scope *);

  jvalue value ();

  std::string string_value ();

  void visit (visitor *);

  void set_left_hand_side (bool cp)
  {
    is_left_hand_side = true;
    is_compound = cp;
  }
};

#endif // GCJX_MODEL_FIELDREF_HH
