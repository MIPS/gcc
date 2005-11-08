// return statement.

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
model_return::resolve (resolution_scope *scope)
{
  model_method *meth = scope->get_current_method ();

  if (meth->static_initializer_p ())
    std::cerr << error ("%<return%> statement is invalid in "
			"static initializer");
  else if (meth->instance_initializer_p ())
    std::cerr << error ("%<return%> statement is invalid in "
			"instance initializer");
  else
    {
      model_type *method_type = meth->get_return_type ();
      if (! expression)
	{
	  if (method_type != primitive_void_type)
	    std::cerr << error ("%<return%> statement with no expression "
				"in method whose return type is not %<void%>");
	}
      else
	{
	  if (method_type != primitive_void_type)
	    expression->use_assignment_conversion (method_type);
	  expression->resolve (scope);
	  fold (expression);
	  if (method_type == primitive_void_type)
	    std::cerr << expression->error ("%<return%> statement with "
					    "expression in method whose "
					    "return type is %<void%>");
	  else if (! assignment_conversion (method_type, expression))
	    std::cerr << expression->error ("%<return%> expression of type %1 "
					    "isn't assignment compatible "
					    "with method's return type")
	      % expression->type ();
	}
    }
}

void
model_return::visit (visitor *v)
{
  v->visit_return (this, expression);
}
