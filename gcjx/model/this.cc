// 'this' expression.

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
model_this::check_outer (model_class *current)
{
  model_type *self = outer->type ();
  while (current && current != self)
    current = current->get_lexically_enclosing_class ();
  if (current == NULL)
    throw error ("%<this%> does not refer to any enclosing class");
}

void
model_this::resolve (resolution_scope *scope)
{
  outer->resolve (scope);

  model_class *inner = scope->get_current_class ();
  check_outer (inner);

  model_type *outt = outer->type ();
  if (! outt->reference_p ())
    throw outer->error ("reference type required");
  model_class *memtype = assert_cast<model_class *> (outt);

  if (scope->static_p () && check_match (memtype, inner))
    throw error ("%<this%> cannot be used in a static context");

  // A static inner class can't access inner fields or methods of an
  // outer class.
  for (model_class *curr = inner;
       curr != NULL && ! check_match (memtype, curr);
       curr = curr->get_lexically_enclosing_class ())
    {
      if (curr->static_p ())
	throw error ("static class %1 cannot access instance member "
		     "of an outer class")
	  % curr;
      if (curr->static_context_p ())
	throw error ("class %1 in static context cannot access "
		     "instance member of an outer class")
	  % curr;
    }

  set_type (outer->type ());
}

void
model_this::visit (visitor *v)
{
  v->visit_this (this);
}

model_this *
model_this::get_synthetic_this (const location &where, model_class *referrer,
				model_class *declarer, bool is_private)
{
  assert (referrer);

  // This is tricky: for a given member, we might refer to it via some
  // "this" pointer, or via some "Outer.this".  The difference depends
  // on our access path: did we inherit the member, or is it only
  // visible via some outer path?  The subtlety here is that we might
  // inherit from the member's declaring class, but if the member is
  // private in that class, then we can't use "this" to access it.
  // So, for private members, we require an exact class match, not
  // just assignability.  (Another way to handle this would be for
  // name lookup to record the path it chose.  This would probably be
  // more reliable in the face of other changes.)
  if (is_private)
    {
      while (referrer && referrer != declarer)
	referrer = referrer->get_lexically_enclosing_class ();
    }
  else
    {
      while (referrer && ! declarer->assignable_from_p (referrer))
	referrer = referrer->get_lexically_enclosing_class ();
    }
  if (! referrer)
    return NULL;
  // If private, we make a synthetic "this" to force the choice of
  // the outer class.
  return new model_this (where, referrer);
}



void
model_synthetic_this::visit (visitor *v)
{
  v->visit_synthetic_this (this);
}

bool
model_synthetic_this::check_match (model_class *self, model_class *target)
{
  return self->assignable_from_p (target);
}



void
model_this_outer::visit (visitor *v)
{
  v->visit_this_outer (this);
}
