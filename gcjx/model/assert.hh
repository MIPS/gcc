// Represent an assert statement.

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

#ifndef GCJX_MODEL_ASSERT_HH
#define GCJX_MODEL_ASSERT_HH

// FIXME: give a warning if the expressions have side effects.
// Optionally anyway.
class model_assert : public model_stmt
{
  // The expression to evaluate.
  ref_expression expression;

  // The expression to print, if any.  This is null if there is no
  // message clause in the assert statement.
  ref_expression result;

public:

  model_assert (const location &w)
    : model_stmt (w)
  {
  }

  void set_expression (const ref_expression &e)
  {
    expression = e;
  }

  void set_message (const ref_expression &m)
  {
    result = m;
  }

  void resolve (resolution_scope *scope);

  void compute_normal_completion (normal_completion_state &)
  {
    normal_completion = true;
  }

  void visit (visitor *v);
};

#endif // GCJX_MODEL_ASSERT_HH
