// Represent the if statement.

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

#ifndef GCJX_MODEL_IF_HH
#define GCJX_MODEL_IF_HH

/// This represents an 'if' statement.  It consists of a boolean
/// expression, a statement that is executed when the expression is
/// true, and an optional statement to be expression when the
/// expression is false.
class model_if : public model_stmt
{
  // The condition.
  ref_expression expression;

  // The true branch.
  ref_stmt true_branch;

  // The false branch.  This will be null if there is no 'else'
  // clause.
  ref_stmt false_branch;

public:

  model_if (const location &w)
    : model_stmt (w)
  {
  }

  /// Set the expression.
  void set_expression (const ref_expression &e)
  {
    expression = e;
  }

  /// Set the true branch.
  void set_true_branch (const ref_stmt &b)
  {
    true_branch = b;
  }

  /// Set the false branch.
  void set_false_branch (const ref_stmt &b)
  {
    false_branch = b;
  }

  void resolve (resolution_scope *scope);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_IF_HH
