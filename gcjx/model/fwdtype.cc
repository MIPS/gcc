// Forwarding type implementation.

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

owner<model_forwarding_type>
model_forwarding_type::array ()
{
  owner<model_forwarding_type> result (new model_forwarding_array (this));
  return result;
}

void
model_forwarding_type::resolve_classes (resolution_scope *scope)
{
  // Resolving this object will find the underlying type.  Then we
  // resolve the classes related to that type.
  resolve (scope);
  if (! resolved_type->primitive_p ())
    {
      model_class *k = assert_cast<model_class *> (resolved_type);
      k->resolve_classes ();
    }
}

void
model_forwarding_type::visit (visitor *v)
{
  v->visit_forwarding_type (this, resolved_type);
}



void
model_forwarding_resolved::visit (visitor *v)
{
  v->visit_forwarding_resolved (this, resolved_type);
}



void
model_forwarding_owned::visit (visitor *v)
{
  v->visit_forwarding_owned (this, ref);
}



void
model_forwarding_simple::resolve (resolution_scope *scope)
{
  // We've already been resolved.
  if (resolved_type)
    return;

  // FIXME null return
  resolved_type = classify_type_name (scope, this, name);
}

void
model_forwarding_simple::visit (visitor *v)
{
  v->visit_forwarding_simple (this, name);
}



void
model_forwarding_array::resolve (resolution_scope *scope)
{
  element->resolve (scope);
  resolved_type = element->type ()->array ();
}

void
model_forwarding_array::visit (visitor *v)
{
  v->visit_forwarding_array (this, element);
}



void
model_forwarding_element::resolve (resolution_scope *scope)
{
  array_type->resolve (scope);

  model_type *ary = array_type->type ();
  if (! ary->array_p ())
    throw error ("expected array type, but found %1")
      % ary;

  resolved_type = ary->element_type ();
}

void
model_forwarding_element::visit (visitor *v)
{
  v->visit_forwarding_element (this, array_type);
}



void
model_forwarding_full::resolve (resolution_scope *scope)
{
  if (resolved_type == NULL)
    {
      resolved_type = classify_type_descriptor (scope, this, name);
      if (resolved_type == NULL)
	throw error ("couldn't find type named %1")
	  % name;
    }
}

void
model_forwarding_full::visit (visitor *v)
{
  v->visit_forwarding_full (this, name);
}



void
model_forwarding_inner::resolve (resolution_scope *scope)
{
  if (resolved_type)
    return;

  model_class *context = NULL;
  // It is ok for parent to be null, as a convenience to make the
  // parser a little simpler.
  if (parent)
    {
      parent->resolve (scope);
      context = assert_cast<model_class *> (parent->type ());
    }

  // FIXME null return
  model_type *r = classify_type_name (scope, this, name, context);
  if (! r->reference_p ())
    throw error ("reference type expected");
  resolved_type = r;
}

void
model_forwarding_inner::visit (visitor *v)
{
  v->visit_forwarding_inner (this, name, parent);
}



void
model_forwarding_parameterized::resolve (resolution_scope *scope)
{
  if (resolved_type)
    return;

  base->resolve (scope);
  model_type *r = base->type ();
  if (! r->reference_p ())
    throw error ("reference type expected");

  std::list<model_class *> rp;
  for (std::list<ref_forwarding_type>::const_iterator i
	 = parameters.begin ();
       i != parameters.end ();
       ++i)
    {
      (*i)->resolve (scope);
      model_type *one = (*i)->type ();
      if (! one->reference_p ())
	throw error ("reference type expected");
      model_class *onek = assert_cast<model_class *> (one);
      if (dynamic_cast<model_wildcard *> (onek) != NULL)
	onek->resolve_classes (scope);
      rp.push_back (onek);
    }

  model_class *k = assert_cast<model_class *> (r);

  model_raw_class *rc = dynamic_cast<model_raw_class *> (k);
  if (rc)
    k = rc->get_parent ();

  resolved_type = k->create_instance (this, rp);
}

void
model_forwarding_parameterized::visit (visitor *v)
{
  v->visit_forwarding_parameterized (this, base, parameters);
}



const format &
operator% (const format &fmt, const owner<model_forwarding_type> &fwd)
{
  return fmt % fwd->type ();
}
