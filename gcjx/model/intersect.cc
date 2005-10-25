// An intersection type.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

model_intersection_type::model_intersection_type (const location &w,
						  const std::list<model_class *> &classes)
  : model_class (w)
{
  std::list<ref_forwarding_type> ifaces;
  model_class *my_super = NULL;

  for (std::list<model_class *>::const_iterator i = classes.begin ();
       i != classes.end ();
       ++i)
    {
      // FIXME: if K is itself a model_intersection_type, should we
      // crack it open and look at its bounds?  This might be
      // convenient for type inference.
      model_class *k = *i;
      if (k->interface_p ())
	ifaces.push_back (new model_forwarding_resolved (w, k));
      else if (my_super == NULL || k->assignable_from_p (my_super))
	my_super = k;
      else if (! my_super->assignable_from_p (k))
	{
	  // FIXME: do we want an error here?  Consider type
	  // inference... might we not prefer to just silently reject
	  // the particular inference?
	  std::cerr << error ("intersection type with incompatible "
			      "bounds %1 and %2")
	    % my_super % k;
	}
    }
  if (my_super)
    set_superclass (new model_forwarding_resolved (w, my_super));
}

model_type *
model_intersection_type::erasure ()
{
  // FIXME: not sure this is correct.
  if (superclass)
    return superclass->type ()->erasure ();
  if (! interfaces.empty ())
    return interfaces.front ()->type ()->erasure ();
  return global->get_compiler ()->java_lang_Object ();
}

bool
model_intersection_type::assignable_from_p (model_type *other)
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
