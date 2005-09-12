// The enhanced for statement.

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

#ifndef GCJX_MODEL_FORENHANCED_HH
#define GCJX_MODEL_FORENHANCED_HH

class model_for_enhanced : public model_for_base
{
  // Expression that yields an Iterable.
  ref_expression expression;

  // Variable declaration.
  ref_variable_decl variable;

  // Type of the elements of the collection.
  model_type *elt_type;

public:

  model_for_enhanced (const location &w,
		      const ref_variable_decl &v,
		      const ref_expression &e)
    : model_for_base (w),
      expression (e),
      variable (v),
      elt_type (NULL)
  {
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);

  model_type *get_element_type () const
  {
    return elt_type;
  }
};

#endif // GCJX_MODEL_FORENHANCED_HH
