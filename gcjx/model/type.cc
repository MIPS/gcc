// Implementation of type base class.

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

model_array_type *
model_type::array ()
{
  if (! array_type)
    array_type = new model_array_type (this);
  return array_type.get ();
}

bool
model_type::checked_exception_p ()
{
  model_type *thr
    = global->get_compiler ()->java_lang_Throwable ();
  model_type *err
    = global->get_compiler ()->java_lang_Error ();
  model_type *rte
    = global->get_compiler ()->java_lang_RuntimeException ();
  
  return (thr->assignable_from_p (this)
          && ! err->assignable_from_p (this)
          && ! rte->assignable_from_p (this));
}

model_type *
model_type::erasure ()
{
  // In many cases the erasure of a type is the type itself.
  return this;
}

void
model_type::visit (visitor *v)
{
  v->visit_type (this, descriptor);
}

const format &
operator% (const format &fmt, const model_type *t)
{
  return fmt % t->get_pretty_name ();
}
