// Ordinary for loops.

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
model_for::resolve (resolution_scope *scope)
{
  model_block_scope vscope (scope);
  resolution_scope::push_iscope var_holder (scope, &vscope);

  if (initializer)
    initializer->resolve (scope);
  if (cond_expr)
    {
      cond_expr->resolve (scope);
      fold (cond_expr);
    }
  if (update)
    update->resolve (scope);
  body->resolve (scope);

  if (cond_expr && ! boolean_conversion (cond_expr))
    std::cerr << cond_expr->error ("%<for%> condition expression "
				   "doesn't have boolean type");
}

void
model_for::visit (visitor *v)
{
  v->visit_for (this, initializer, cond_expr, update, body);
}

void
model_for::compute_normal_completion (normal_completion_state &state)
{
  if (! cond_expr
      || ! cond_expr->constant_p ()
      || (jboolean) cond_expr->value () != false)
    {
      // In this case, the body is reachable.  We don't care about
      // the result though.
      normal_completion_pusher push (state, this);
      body->compute_normal_completion (state);
    }
  else
    body->unreachable ();

  // FIXME: expression handling for try blocks.
  // With -gnu have a condition for when the for update is unreachable.

  if (cond_expr
      && (! cond_expr->constant_p ()
	  || (jboolean) cond_expr->value () == false))
    normal_completion = true;
  else if (break_found)
    normal_completion = true;
  else
    normal_completion =false;
}
