// Expression base class.

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
model_expression::set_type (model_type *t)
{
  assert (t);
  assert (! type_cache.set_p ());
  type_cache = t;
}

jvalue
model_expression::value ()
{
  // Most expressions aren't compile-time constants.  However, we
  // don't want to force the subclasses to override this method.
  abort ();
}

std::string
model_expression::string_value ()
{
  jvalue v = value ();
  return type_cache->to_string (v);
}

const format &
operator% (const format &fmt, const model_expression *expr)
{
  model_type *t = expr->type ();
  return fmt % t;
}
