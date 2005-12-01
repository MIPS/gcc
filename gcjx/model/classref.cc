// Class references.

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
model_class_ref::resolve (resolution_scope *scope)
{
  klass->resolve (scope);
  assert (klass->type () != null_type);

  // FIXME:
  // * Should have a better model for compatibility stuff, not just "1.5"
  // * knows too much about the back end -- generating trees won't
  //   need this method!  But what about multi-back-end output?
  if (! global->get_compiler ()->target_15 ()
      && ! klass->type ()->primitive_p ()
      && klass->type () != primitive_void_type)
    {
      // We want to add the method now, before the back ends ask the
      // class for its list of methods.  FIXME: this area is still
      // somewhat bogus.  Seems like it should be simpler to add a
      // method during code generation.  But then we need to handle
      // the situation where back ends differ.
      scope->get_current_class ()->add_class_members ();
    }

  model_class *dest = global->get_compiler ()->java_lang_Class ();
  if (global->get_compiler ()->feature_generics ())
    {
      model_type *boxed = klass->type ();
      if (boxed->primitive_p () || boxed == primitive_void_type)
	boxed = box_primitive_type (boxed);
      model_class *boxed_class = assert_cast<model_class *> (boxed);

      std::list<model_class *> args;
      args.push_back (boxed_class);
      // FIXME: if Class is not parameterized, issue an error about
      // the class library.
      dest = dest->create_instance (this, args);
    }

  set_type (dest);
}

void
model_class_ref::visit (visitor *v)
{
  v->visit_class_ref (this, klass);
}
