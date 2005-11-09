// Member references.

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

#ifndef GCJX_MODEL_MEMBERREF_HH
#define GCJX_MODEL_MEMBERREF_HH

/// The base class for all kinds of deferred member references.
class model_memberref_base : public model_expression
{
protected:

  // The member reference to which we forward requests.
  ref_expression real;

  model_memberref_base (const location &w)
    : model_expression (w)
  {
  }

public:

  // This is sort of ugly, see unary.cc to see its use.
  // FIXME.
  model_expression *get_real ()
  {
    return real.get ();
  }
};

/// When parsing something like "a.b.c.d" or "a.b.c.d()", we won't know
/// until the analysis phase what we're looking at.  For instance, "a"
/// might be a variable or a type, and likewise for the other elements.
/// This class represents an undetermined expression like the above.
/// It can also represent a simple variable reference.
class model_memberref_forward : public model_memberref_base
{
  // The unresolved identifier sequence.
  std::list<std::string> ids;

  // True if a method call.
  bool is_call;

  // Arguments to a method.
  std::list<ref_expression> arguments;

  // Explicit type parameters.  This list is empty if there are none.
  std::list<ref_forwarding_type> type_parameters;

  // True if left-hand-side of assignment.
  bool is_lhs;
  // True if compound assignment.
  bool is_compound;

  // The type to which assignment conversion will be performed.  If
  // NULL, this member won't be subject to assignment conversion.
  model_type *assign_conv_type;

  bool compute_constant_p ()
  {
    return real->constant_p ();
  }

protected:

  // This constructor is used by the generic template subclass.
  model_memberref_forward (const location &w)
    : model_memberref_base (w),
      is_call (false),
      is_lhs (false),
      is_compound (false),
      assign_conv_type (NULL)
  {
  }

public:

  model_memberref_forward (const location &w, const std::list<std::string> &l)
    : model_memberref_base (w),
      ids (l),
      is_call (false),
      is_lhs (false),
      is_compound (false),
      assign_conv_type (NULL)
  {
  }

  void set_ids (const std::list<std::string> &v)
  {
    ids = v;
  }

  void set_arguments (const std::list<ref_expression> &args)
  {
    arguments = args;
    is_call = true;
  }

  void set_type_parameters (const std::list<ref_forwarding_type> &params)
  {
    type_parameters = params;
  }

  void resolve (resolution_scope *);

  void visit (visitor *v)
  {
    real->visit (v);
  }

  jvalue value ()
  {
    return real->value ();
  }

  std::string string_value ()
  {
    return real->string_value ();
  }

  /// Return true if this is a reference to a method call.
  bool call_p () const
  {
    return is_call;
  }

  void set_left_hand_side (bool compound)
  {
    is_lhs = true;
    is_compound = compound;
  }

  void use_assignment_conversion (model_type *t)
  {
    assert (! real);
    assign_conv_type = t;
  }
};

/// This form of a deferred member reference is used only when reading
/// an enum constant reference from a class file.
class model_memberref_enum : public model_memberref_base
{
  // The enum class.
  ref_forwarding_type base_type;

  // The name of the enum constant.
  std::string field_name;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_memberref_enum (const location &w,
			const ref_forwarding_type &base,
			const std::string &fn)
    : model_memberref_base (w),
      base_type (base),
      field_name (fn)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

#endif // GCJX_MODEL_MEMBERREF_HH
