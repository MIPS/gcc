// while statement.

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

#include "typedefs.hh"

void
model_while::compute_normal_completion (normal_completion_state &state)
{
  if (! expression->constant_p ()
      || (jboolean) expression->value () != false)
    {
      // Contained statement is reached.
      normal_completion_pusher push (state, this);
      statement->compute_normal_completion (state);
    }
  else
    statement->unreachable ();

  // We can complete normally if either (1) expression is not a
  // constant "true" expression or (2) if there is a reachable break
  // statement in the body.
  normal_completion = (break_found
		       || ! expression->constant_p ()
		       || (jboolean) expression->value () != true);
}

void
model_while::resolve (resolution_scope *scope)
{
  expression->resolve (scope);
  if (! boolean_conversion (expression))
    std::cerr << expression->error ("%<while%> expression doesn't have "
				    "boolean type");
  statement->resolve (scope);
}

void
model_while::visit (visitor *v)
{
  v->visit_while (this, expression, statement);
}
