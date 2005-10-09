// Represent new of an array.

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

#ifndef GCJX_MODEL_NEWARRAY_HH
#define GCJX_MODEL_NEWARRAY_HH

/// This represents a 'new' expression which creates an array.  Any
/// dimension of array is handled here, as are initialized arrays.
class model_new_array : public model_expression
{
  // The element type.
  ref_forwarding_type klass;

  // Expressions for array sizes.
  std::list<ref_expression> sizes;

  // The initializer, if any.  This will be null if there is no
  // initializer.
  ref_expression initializer;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_new_array (const location &w)
    : model_expression (w)
  {
  }

  void set_element_type (const ref_forwarding_type &t)
  {
    klass = t;
  }

  void set_initializer (const ref_expression &expr)
  {
    initializer = expr;
  }

  void set_size_expressions (const std::list<ref_expression> &szs)
  {
    sizes = szs;
  }

  void add (const ref_expression &expr)
  {
    sizes.push_back (expr);
  }

  void resolve (resolution_scope *scope);

  void visit (visitor *);

  /// Indicate that this element does not need to be resolved.  This
  /// is only used when creating a synthetic new array expression when
  /// handling varargs.
  void already_resolved ();
};

#endif // GCJX_MODEL_NEWARRAY_HH
