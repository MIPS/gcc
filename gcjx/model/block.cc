// Block.

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
model_block::resolve (resolution_scope *scope)
{
  model_block_scope vars (scope);
  resolution_scope::push_iscope var_holder (scope, &vars);

  // Note that a block doesn't require any resolution of its own.
  // This fact is relied on in a couple of places where we create a
  // new block after the body of a method has been resolved.
  ::resolve (scope, statements);
}

void
model_block::visit (visitor *v)
{
  v->visit_block (this, statements);
}

void
model_block::compute_normal_completion (normal_completion_state &state)
{
  normal_completion_pusher push (state, this);
  normal_completion = ::compute_normal_completion (state, statements);
}
