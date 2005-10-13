// Array initializers.

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
model_array_initializer::resolve (resolution_scope *scope)
{
  element_type->resolve (scope);
  model_type *ct = element_type->type ();
  for (std::list<ref_expression>::iterator i = initializers.begin ();
       i != initializers.end ();
       ++i)
    {
      (*i)->resolve (scope);
      if (assignment_conversion (ct, *i) == NULL)
	std::cerr << (*i)->error ("expression in array initializer of "
				  "type %1 can't be converted to element "
				  "type %2 by assignment")
	  % (*i)->type () % ct;
      fold (*i);
    }
  set_type (ct->array ());
}

void
model_array_initializer::visit (visitor *v)
{
  v->visit_array_initializer (this, element_type, initializers);
}

void
model_array_initializer::already_resolved ()
{
  assert (element_type->type () != NULL);
  set_type (element_type->type ());
}



void
model_annotation_initializer::resolve (resolution_scope *scope)
{
  // Just resolve and fold the elements.  Whether the elements are
  // actually commensurate with the annotation type is handled
  // elsewhere.
  for (std::list<ref_expression>::iterator i = initializers.begin ();
       i != initializers.end ();
       ++i)
    {
      (*i)->resolve (scope);
      fold (*i);
    }
}

void
model_annotation_initializer::visit (visitor *v)
{
  v->visit_annotation_initializer (this, element_type, initializers);
}

void
model_annotation_initializer::set_actual_type (model_type *t)
{
  set_type (t);
}
