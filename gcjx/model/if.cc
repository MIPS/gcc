// if statement.

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
model_if::resolve (resolution_scope *scope)
{
  expression->resolve (scope);
  fold (expression);
  true_branch->resolve (scope);
  if (false_branch)
    false_branch->resolve (scope);

  if (! boolean_conversion (expression))
    std::cerr << expression->error ("%<if%> expression doesn't "
				    "have boolean type");
}

void
model_if::visit (visitor *v)
{
  v->visit_if (this, expression, true_branch, false_branch);
}

void
model_if::compute_normal_completion (normal_completion_state &state)
{
  normal_completion_pusher push (state, this);

  // Always do this for reachability of children.
  true_branch->compute_normal_completion (state);

  if (! false_branch)
    normal_completion = true;
  else
    {
      false_branch->compute_normal_completion (state);
      normal_completion = (true_branch->can_complete_normally ()
			   || false_branch->can_complete_normally ());

      if (! true_branch->can_complete_normally ()
	  && global->get_compiler ()->warn_unneeded_else ())
	std::cerr
	  << false_branch->warn (global->get_compiler ()->warn_unneeded_else (),
				 "unnecessary %<else%>; %<then%> clause "
				 "can't complete normally");
    }
}
