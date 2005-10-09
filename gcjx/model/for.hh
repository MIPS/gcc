// Represent the for statement.

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

#ifndef GCJX_MODEL_FOR_HH
#define GCJX_MODEL_FOR_HH

/// This is the base class for both kinds of 'for' loop.  It is not
/// instantiated directly, only subclassed.
class model_for_base : public model_loop
{
protected:

  // Body.
  ref_stmt body;

public:

  model_for_base (const location &w)
    : model_loop (w)
  {
  }

  void set_body (const ref_stmt &b)
  {
    body = b;
  }
};

/// This is an "old-style" for loop.
class model_for : public model_for_base
{
  // Initialization.  This is null if there is no initializer
  // statement.
  ref_stmt initializer;

  // Condition.  This is null if there is no condition expression;
  // this should be taken to mean 'true'.
  ref_expression cond_expr;

  // Update.  This is null if there is no update statement.
  ref_stmt update;

public:

  model_for (const location &w)
    : model_for_base (w)
  {
  }

  void set_initializer (const ref_stmt &s)
  {
    initializer = s;
  }

  void set_expression (const ref_expression &exp)
  {
    cond_expr = exp;
  }

  void set_update (const ref_stmt &s)
  {
    update = s;
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_FOR_HH
