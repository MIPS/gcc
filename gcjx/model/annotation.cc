// Represent an annotation.

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
model_annotation::resolve_classes (resolution_scope *scope)
{
  // FIXME: Resolution methods seem backwards on
  // model_forwarding_type.
  name->resolve (scope);
  // The reason we call set_type() here is that we need to know the
  // annotation's type when handling SuppressWarnings.  This is
  // moderately ugly.
  set_type (name->type ());
}

void
model_annotation::resolve (resolution_scope *scope)
{
  if (! name->type ()->annotation_p ())
    throw name->error ("type %1 isn't an annotation type")
      % name;
  model_annotation_type *anno_type
    = assert_cast<model_annotation_type *> (name->type ());

  ::resolve (scope, args);

  anno_type->check_completeness (args, this);
}

void
model_annotation::visit (visitor *v)
{
  v->visit_annotation (this, name, args);
}

model_annotation_value *
model_annotation::find_annotation (const char *name)
{
  for (std::list<ref_annotation_value>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    {
      if ((*i)->get_name () == name)
	return (*i).get ();
    }
  return NULL;
}
