// Represent the do statement.

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

#ifndef GCJX_MODEL_DO_HH
#define GCJX_MODEL_DO_HH

/// This represents a "do" loop.  It consists of a boolean expression
/// and a loop body.
class model_do : public model_loop
{
  // The condition.
  ref_expression expression;

  // The statement.
  ref_stmt statement;

public:

  model_do (const location &w)
    : model_loop (w)
  {
  }

  /// Set the expression.
  void set_expression (const ref_expression &e)
  {
    expression = e;
  }

  /// Set the body of the loop.
  void set_statement (const ref_stmt &b)
  {
    statement = b;
  }

  void resolve (resolution_scope *scope);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_DO_HH
