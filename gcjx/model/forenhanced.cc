// Enhanced for loop.

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
model_for_enhanced::resolve (resolution_scope *scope)
{
  model_block_scope vscope (scope);
  resolution_scope::push_iscope var_holder (scope, &vscope);

  variable->resolve (scope);
  expression->resolve (scope);
  fold (expression);
  if (expression->type ()->array_p ())
    {
      // Ok.  We pull this out so we don't try to load Iterable unless
      // really necessary.  This is a little strange but it does let
      // users iterate over arrays even with an older class library.

      if (! assignment_conversion (variable->type (),
				   expression->type ()->element_type ()))
	std::cerr << expression->error ("%<for%> expression of type %1"
					" is not assignment compatible with"
					" variable of type %2")
	  % expression->type ()->element_type ()
	  % variable->type ();
    }
  else
    {
      model_type *iterable
	= global->get_compiler ()->java_lang_Iterable ();
      if (! iterable->assignable_from_p (expression->type ()))
	std::cerr << expression->error ("%<for%> expression has type %1, which"
					" isn't an array and doesn't"
					" implement %<java.lang.Iterable%>")
	  % expression->type ();

      if (! variable->type ()->reference_p ())
	std::cerr << variable->error ("%<for%> variable has primitive type");

      // FIXME: generics: if expression has parameterized type, the
      // variable must be compatible with the parameter.
    }

  scope->add_binding (variable.get ());
  body->resolve (scope);
}

void
model_for_enhanced::visit (visitor *v)
{
  v->visit_for_enhanced (this, body, expression, variable);
}

void
model_for_enhanced::compute_normal_completion (normal_completion_state &state)
{
  // The document doesn't specify this very well, but I believe the
  // body should always be reachable, and the extended for completes
  // normally iff the body does.
  normal_completion_pusher push (state, this);
  body->compute_normal_completion (state);
  normal_completion = body->can_complete_normally ();
}
