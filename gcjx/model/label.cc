// Label.

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

model_loop *
model_label::get_continue_target ()
{
  // A label might directly wrap a loop.
  label_used = true;
  return dynamic_cast<model_loop *> (statement.get ());
}

void
model_label::resolve (resolution_scope *scope)
{
  statement->resolve (scope);
  if (! label_used && scope->warn_unused_label ())
    std::cerr << warn (global->get_compiler ()->warn_unused_label (),
		       "label %1 defined but never used")
      % name;
}

void
model_label::visit (visitor *v)
{
  v->visit_label (this, statement);
}

void
model_label::compute_normal_completion (normal_completion_state &state)
{
  normal_completion_pusher push (state, this);
  statement->compute_normal_completion (state);

  normal_completion = statement->can_complete_normally () || break_found;
}
