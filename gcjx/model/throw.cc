// Throw statement.

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
model_throw::resolve (resolution_scope *scope)
{
  expression->resolve (scope);
  fold (expression);
  model_type *thrown_type = expression->type ();
  // 'throw null' will always throw a NullPointerException.
  if (thrown_type == null_type)
    thrown_type = global->get_compiler ()->java_lang_NullPointerException ();
  model_type *thr = global->get_compiler ()->java_lang_Throwable ();
  if (! thr->assignable_from_p (thrown_type))
    std::cerr << expression->error ("%<throw%> statement's expression of "
				    "type %1 doesn't inherit from "
				    "%<java.lang.Throwable%>")
      % thrown_type;
  else if (thrown_type->checked_exception_p ())
    scope->get_current_catcher ()->note_throw_type (thrown_type);
}

void
model_throw::visit (visitor *v)
{
  v->visit_throw (this, expression);
}
