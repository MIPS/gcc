// Array initializers.

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

#ifndef GCJX_MODEL_ARRAYINIT_HH
#define GCJX_MODEL_ARRAYINIT_HH

class model_array_initializer : public model_expression
{
protected:

  // The list of initializer expressions.
  std::list<ref_expression> initializers;

  // The component type of the resulting array.
  ref_forwarding_type element_type;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_array_initializer (const location &w,
			   const ref_forwarding_type &elt_type,
			   const std::list<ref_expression> &inits)
    : model_expression (w),
      initializers (inits),
      element_type (elt_type)
  {
  }

  void set_initializers (const std::list<ref_expression> &inits)
  {
    initializers = inits;
  }

  std::list<ref_expression> get_initializers () const
  {
    return initializers;
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  /// Indicate that this element does not need to be resolved.  This
  /// is only used when creating a synthetic new array expression when
  /// handling varargs.
  void already_resolved ();
};

/// This is an array initializer that is used to represent the value
/// of an annotation when it has array form.
class model_annotation_initializer : public model_array_initializer
{
public:

  model_annotation_initializer (const location &w,
				const std::list<ref_expression> &inits)
    : model_array_initializer (w, NULL, inits)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  /// This is used to set the known type of the initializer according
  /// to what is required by the annotation.  This might not really be
  /// correct (if there is user error), but is done so that later code
  /// can use the type() method without crashing.  This is safe as
  /// user errors will prevent code generation.
  void set_actual_type (model_type *);
};

#endif // GCJX_MODEL_ARRAYINIT_HH
