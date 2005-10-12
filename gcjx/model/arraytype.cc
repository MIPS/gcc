// Array types.

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

model_array_type::model_array_type (model_type *elt)
  : model_class (LOCATION_UNKNOWN),
    element (elt)
{
  descriptor = "[" + elt->get_descriptor ();

  model_class *obj = global->get_compiler ()->java_lang_Object ();
  model_forwarding_resolved *res_obj
    = new model_forwarding_resolved (LOCATION_UNKNOWN, obj);
  set_superclass (res_obj);

  // Add the implemented interfaces.
  model_class *ser
    = global->get_compiler ()->java_io_Serializable ();
  model_class *cloneable
    = global->get_compiler ()->java_lang_Cloneable ();
  // fixme we should cache the forwarding types for things like
  // this (pre-resolved) so we can more easily dole them out...
  // there will be lots of arrays and it is silly to make new wrappers.
  ref_forwarding_type sert (new model_forwarding_resolved (LOCATION_UNKNOWN,
							   ser));
  ref_forwarding_type clonet (new model_forwarding_resolved (LOCATION_UNKNOWN,
							     cloneable));
  interfaces.push_back (clonet);
  interfaces.push_back (sert);

  // Add the 'length' field.
  ref_forwarding_type length_type
    = new model_forwarding_resolved (LOCATION_UNKNOWN, primitive_int_type);
  ref_field length = new model_field (LOCATION_UNKNOWN, "length",
				      length_type, this);
  length->set_modifiers (ACC_PUBLIC | ACC_FINAL);
  add (length);

  model_method *clone = new model_method (LOCATION_UNKNOWN, this);
  clone->set_name ("clone");
  clone->set_return_type (res_obj);
  clone->set_modifiers (ACC_PUBLIC);
  add (ref_method (clone));
}

std::string
model_array_type::get_pretty_name () const
{
  return element->get_pretty_name () + "[]";
}

model_type *
model_array_type::erasure ()
{
  return element->erasure ()->array ();
}

void
model_array_type::visit (visitor *v)
{
  v->visit_array_type (this, descriptor, name, element);
}
