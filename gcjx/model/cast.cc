// Casts.

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

bool
model_cast::compute_constant_p ()
{
  return constant_type_p (type_cache) && expr->constant_p ();
}

jvalue
model_cast::value ()
{
  model_type *t = target->type ();
  assert (t->primitive_p ());
  model_primitive_base *baset = assert_cast<model_primitive_base *> (t);
  return baset->convert (expr->type (), expr->value ());
}

std::string
model_cast::string_value ()
{
  return expr->string_value ();
}

void
model_cast::resolve (resolution_scope *scope)
{
  expr->resolve (scope);
  target->resolve (scope);
  if (! casting_conversion (target->type (), expr->type ()))
    throw error ("expression of type %1 can't be cast to type %2")
      % expr->type () % target->type ();
  set_type (target->type ());
}

void
model_cast::already_resolved ()
{
  assert (expr->type () != NULL);
  set_type (target->type ());
}

void
model_cast::visit (visitor *v)
{
  v->visit_cast (this, target, expr);
}
