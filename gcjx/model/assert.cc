// Assert statement.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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
model_assert::resolve (resolution_scope *scope)
{
  expression->resolve (scope);
  if (! boolean_conversion (expression))
    throw expression->error ("%<assert%> expression can't be "
			     "converted to boolean type");

  fold (expression);
  if (result)
    {
      result->resolve (scope);

      if (result->type () == primitive_void_type)
	throw result->error ("second %<assert%> expression cannot "
			     "be of type %<void%>");

      // FIXME: this is pretty bogus, we should search AssertionError
      // for the appropriate constructor and use method conversion.
      if (result->type ()->primitive_p ()
	  && result->type () != primitive_char_type
	  && result->type () != primitive_boolean_type)
	unary_numeric_promotion (result);

      fold (result);
    }
}

void
model_assert::visit (visitor *v)
{
  v->visit_assert (this, expression, result);
}
