// A type variable.

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
model_type_variable::resolve_classes_hook (resolution_scope *scope)
{
  ref_forwarding_type bounds_class;
  std::list<ref_forwarding_type> ifaces;
  bool first = true;
  for (std::list<ref_forwarding_type>::const_iterator i = bounds.begin ();
       i != bounds.end ();
       ++i)
    {
      (*i)->resolve (scope);

      model_type *itype = (*i)->type ();
      if (! itype->reference_p ())
	throw (*i)->error ("type variable bound is not of reference type");
      if (itype->interface_p ())
	ifaces.push_back (*i);
      else if (! first)
	throw (*i)->error ("type variable bound is not an interface but "
			   "is not first bound");
      else
	{
	  if (bounds_class)
	    {
	      // FIXME: would be nice to include the location of the
	      // previous bound as well.  We can easily keep a
	      // reference to the forwarder here...
	      throw (*i)->error ("type variable bound is not an interface, "
				 "but class bound %1 already seen")
		% bounds_class;
	    }
	  bounds_class = *i;
	}
      first = false;
    }

  if (bounds_class)
    set_superclass (bounds_class);
  set_implements (ifaces);
}

void
model_type_variable::resolve (resolution_scope *scope)
{
  // FIXME: should be able to handle setting resolution_state.
  // This area needs some work.
  resolve_classes (scope);
}

model_type *
model_type_variable::erasure ()
{
  if (superclass)
    return superclass->type ()->erasure ();
  if (! interfaces.empty ())
    return interfaces.front ()->type ()->erasure ();
  return global->get_compiler ()->java_lang_Object ();
}

void
model_type_variable::validate (model_element *request, model_type *type)
{
  if (! type->reference_p ())
    throw request->error ("type parameter must be reference type");

  // FIXME: really implement capture conversion.
  if (dynamic_cast<model_wildcard *> (type))
    return;

  if (superclass && ! superclass->type ()->assignable_from_p (type))
    throw request->error ("type parameter does not extend %1")
      % superclass;

  for (std::list<ref_forwarding_type>::const_iterator i = interfaces.begin ();
       i != interfaces.end ();
       ++i)
    {
      if (! (*i)->type ()->assignable_from_p (type))
	throw request->error ("type parameter does not implement %1")
	  % *i;
    }
}

bool
model_type_variable::assignable_from_p (model_type *other)
{
  if (other == null_type)
    return true;
  if (! other->reference_p ())
    return false;
  // The type must satisfy all our bounds.
  if (superclass && ! superclass->type ()->assignable_from_p (other))
    return false;
  for (std::list<ref_forwarding_type>::const_iterator i = interfaces.begin ();
       i != interfaces.end ();
       ++i)
    {
      if (! (*i)->type ()->assignable_from_p (other))
	return false;
    }
  // Any reference type that got to this point is ok.  If we don't
  // have any bounds, then our erasure is Object, and the assignment
  // is fine.
  return true;
}

model_class *
model_type_variable::apply_type_map (model_element *,
				     const model_type_map &type_map)
{
  model_class *result = type_map.find (this);
  return result ? result : this;
}

void
model_type_variable::check_instantiation (model_element *request)
{
  throw request->error ("can't create instance of type variable %1")
    % this;
}

std::string
model_type_variable::get_formal_signature ()
{
  std::string result = name;
  bool is_first = true;
  for (std::list<ref_forwarding_type>::const_iterator i = bounds.begin ();
       i != bounds.end ();
       ++i)
    {
      model_type *bound = (*i)->type ();
      if (is_first && bound->interface_p ())
	{
	  // The first element must always name the class bound, and
	  // in this case there is none.
	  result += ":";
	}
      result += ":" + bound->get_signature ();
    }
  // Handle the case where there were no bounds at all.
  if (bounds.empty ())
    result += ":";
  return result;
}

std::string
model_type_variable::get_signature ()
{
  return "T" + name + ";";
}

void
model_type_variable::visit (visitor *v)
{
  v->visit_type_variable (this, descriptor, name, bounds);
}
