// Catch clauses.

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
model_catch::resolve (resolution_scope *scope)
{
  // It would be nice if we didn't have to push a block here and then
  // another one immediately in the real block.  An empty block is
  // pretty cheap, though.
  model_block_scope vscope (scope);
  resolution_scope::push_iscope var_holder (scope, &vscope);

  assert (vars.size () == 1);
  model_variable_stmt::resolve (scope);

  model_type *thr = global->get_compiler ()->java_lang_Throwable ();
  ref_variable_decl var = vars.front ();
  if (! thr->assignable_from_p (var->type ()))
    std::cerr << var->error ("parameter of exception handler has "
			     "type %1, which is not a subclass "
			     "of %<java.lang.Throwable%>")
      % var->type ();

  block->resolve (scope);
}

void
model_catch::visit (visitor *v)
{
  assert (vars.size () == 1);
  v->visit_catch (this, vars.front (), block);
}

void
model_catch::compute_normal_completion (normal_completion_state &state)
{
  normal_completion_pusher push (state, this);
  block->compute_normal_completion (state);
  normal_completion = block->can_complete_normally ();
}
