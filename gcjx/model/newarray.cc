// 'new' of array.

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
model_new_array::resolve (resolution_scope *scope)
{
  klass->resolve (scope);
  ::resolve (scope, sizes);

  model_type *atype = klass->type ();
  for (std::list<ref_expression>::iterator i = sizes.begin ();
       i != sizes.end ();
       ++i)
    {
      atype = atype->array ();

      fold (*i);
      model_type *promoted = unary_numeric_promotion (*i);
      if (promoted == NULL)
	throw (*i)->error ("array dimension expression must have "
			   "numeric type");
      else if (! promoted->integral_p ())
	throw (*i)->error ("array dimension expression must have "
			   "integral type");
      else if (promoted == primitive_long_type)
	throw (*i)->error ("array dimension expression must not "
			   "be of type %<long%>");

      if (scope->warn_array_size () && (*i)->constant_p ())
	{
	  assert (promoted == primitive_int_type);
	  jint val = (*i)->value ();
	  // It is valid to have a negative constant size; it throws
	  // an exception at runtime.  It may seem bogus to check for
	  // this, but it could be the result of an expression.
	  if (val < 0)
	    std::cerr
	      << (*i)->warn (global->get_compiler ()->warn_array_size (),
			     "negative array dimension");
	}
    }

  if (initializer)
    initializer->resolve (scope);
  else if (sizes.empty ())
    throw error ("at least one array dimension required");

  set_type (atype);
}

void
model_new_array::visit (visitor *v)
{
  v->visit_new_array (this, klass, sizes, initializer);
}

void
model_new_array::already_resolved ()
{
  assert (klass->type () != NULL);
  set_type (klass->type ());
}
