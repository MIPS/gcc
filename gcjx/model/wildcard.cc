// Type parameter wildcards.

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

bool
model_wildcard::assignable_from_p (model_type *other)
{
  if (other == null_type)
    return true;
  if (! other->reference_p ())
    return false;
  if (! bound)
    return true;
  if (is_super)
    {
      model_class *k = assert_cast<model_class *> (bound->type ());
      return k->assignable_from_p (other);
    }
  model_class *k = assert_cast<model_class *> (other);
  return k->assignable_from_p (bound->type ());
}

bool
model_wildcard::contains_p (model_class *other)
{
  if (! bound)
    // FIXME?  This is done on the theory that no bound == Object.
    return true;
  model_class *k = assert_cast<model_class *> (bound->type ());
  if (other == k)
    return true;
  if (! other->wildcard_p ())
    return false;
  model_wildcard *w = assert_cast<model_wildcard *> (other);
  if (w->super_p () != is_super)
    return false;
  model_class *other_bound = w->get_bound ();
  if (other_bound == NULL)
    other_bound = global->get_compiler ()->java_lang_Object ();
  if (is_super)
    return other_bound->assignable_from_p (k);
  return k->assignable_from_p (other_bound);
}

void
model_wildcard::resolve_classes_hook (resolution_scope *scope)
{
  if (bound)
    {
      bound->resolve (scope);
      if (! bound->type ()->reference_p ())
	throw error ("bound for wildcard parameter must be of reference type");
    }

  // We pretend our superclass is either Object, or the bounds type,
  // depending on our setting.
  if (bound && ! is_super)
    {
      model_class *k = assert_cast<model_class *> (bound->type ());
      if (k->interface_p ())
	{
	  std::list<ref_forwarding_type> ifaces;
	  ifaces.push_back (bound);
	  set_implements (ifaces);
	}
      else
	set_superclass (bound);
    }
}

model_class *
model_wildcard::apply_type_map (model_element *request,
				const model_type_map &typemap)
{
  if (! bound)
    return this;
  model_class *bound_class = assert_cast<model_class *> (bound->type ());
  model_class *applied = bound_class->apply_type_map (request, typemap);
  if (bound_class == applied)
    return this;
  // FIXME: ownership.
  // FIXME: location from request?  Or from 'this'?
  model_wildcard *result = new model_wildcard (request->get_location (),
					       applied, is_super);
  return result;
}

model_type *
model_wildcard::erasure ()
{
  // FIXME: does this even make sense?
  return global->get_compiler ()->java_lang_Object ();
}

std::string
model_wildcard::get_signature ()
{
  std::string result;
  if (! bound)
    result = "*";
  else
    {
      if (is_super)
	result = "-";
      else
	result = "+";
      result += bound->type ()->get_signature ();
    }
  return result;
}

std::string
model_wildcard::get_pretty_name () const
{
  std::string result = "?";
  if (bound)
    result += ((is_super ? " super " : " extends ")
	       + bound->type ()->get_pretty_name ());
  return result;
}

void
model_wildcard::visit (visitor *v)
{
  v->visit_wildcard (this, descriptor, name, is_super, bound);
}
