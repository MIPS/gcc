// break statement.

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
model_break::resolve (resolution_scope *scope)
{
  // We already issued an error message if there is one.
  if (target)
    actual = target->get_break_target ();
}

void
model_break::visit (visitor *v)
{
  v->visit_break (this, actual);
}

void
model_break::compute_normal_completion (normal_completion_state &state)
{
  if (target)
    {
      if (state.can_goto_p (dynamic_cast<model_stmt *> (target)))
	{
	  target->break_reached ();
	  if (actual)
	    actual->break_reached ();
	}
    }
  else
    assert (! actual);
  normal_completion = false;
}
