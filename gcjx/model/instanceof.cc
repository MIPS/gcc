// instanceof operator.

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
model_instanceof::resolve (resolution_scope *scope)
{
  expr->resolve (scope);
  type->resolve (scope);

  model_type *etype = expr->type ();
  if (etype != null_type && ! etype->reference_p ())
    throw expr->error ("expression argument to %<instanceof%> must "
		       "have reference type");
  if (! type->type ()->reference_p ())
    throw type->error ("type argument to %<instanceof%> must have "
		       "reference type");
  if (! casting_conversion (type->type (), etype))
    throw error ("expression of type %1 cannot possibly be instance of"
		 " type %2")
      % type->type () % etype;

  if (type->type ()->assignable_from_p (etype)
      && scope->warn_unneeded_instanceof ())
    std::cerr << warn (global->get_compiler ()->warn_unneeded_instanceof (),
		       "%<instanceof%> could be replaced with "
		       "check against %<null%>");
  if (etype == null_type && scope->warn_unneeded_instanceof ())
    std::cerr << warn (global->get_compiler ()->warn_unneeded_instanceof (),
		       "argument to %<instanceof%> is <%null%>");

  set_type (primitive_boolean_type);
}

void
model_instanceof::visit (visitor *v)
{
  v->visit_instanceof (this, expr, type);
}
