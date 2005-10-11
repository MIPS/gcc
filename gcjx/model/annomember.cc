// Represent an annotation type member declaration.

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
model_annotation_member::massage_modifiers (const ref_modifier_list &mods)
{
  // Do nothing: the checks are all done on the corresponding method.
}

bool
model_annotation_member::type_ok_p ()
{
  model_type *t = type->type ();

  // An array type is ok if its element type is.  Arrays of arrays are
  // not permitted.
  if (t->array_p ())
    t = t->element_type ();

  // Valid types are primitive, String, Class, enum types, and
  // annotation types.
  if (t->primitive_p ())
    return true;
  if (t == global->get_compiler ()->java_lang_String ()
      || t->erasure () == global->get_compiler ()->java_lang_Class ())
    return true;
  return t->enum_p () || t->annotation_p ();
}

void
model_annotation_member::resolve (resolution_scope *scope)
{
  model_annotation_value::resolve (scope);
  type->resolve (scope);

  if (! type_ok_p ())
    throw error ("type %1 is invalid for an annotation member")
      % type;
  if (value && ! annotation_commensurate_p (type->type (), value.get ()))
    throw error ("value is not commensurate with type of annotation member");
}

void
model_annotation_member::visit (visitor *v)
{
  v->visit_annotation_member (this, type);
}
