// Member references.

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
model_memberref_forward::resolve (resolution_scope *scope)
{
  if (is_call)
    {
      assert (! is_lhs);
      real = classify_expression_name (scope, this, ids, arguments);
      if (ids.size () == 1)
	{
	  model_invocation_base *iv
	    = assert_cast<model_invocation_base *> (real.get ());
	  iv->set_unqualified ();
	}
    }
  else
    {
      real = classify_expression_name (scope, this, ids);
      if (dynamic_cast<model_field_ref *> (real.get ()) != NULL)
	{
	  model_field_ref *ref = assert_cast<model_field_ref *> (real.get ());
	  ref->set_qualified (ids.size () > 1);
	}
      if (is_lhs)
	real->set_left_hand_side (is_compound);
    }
  assert (real);
  real->resolve (scope);
  set_type (real->type ());
}
