// Class declaration statement.

// Copyright (C) 2004 Free Software Foundation, Inc.
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
model_class_decl_stmt::resolve (resolution_scope *scope)
{
  if (scope->static_p ())
    decl->set_static_context ();

  // Add to binding first, as the body of the class is in the scope of
  // its name.
  scope->add_binding (decl.get ());

  decl->resolve_classes (scope);
  decl->resolve (scope);

  // Make sure our containing class knows about this local class, for
  // code generation.  Also this lets method body cleanup work
  // properly.
  decl->get_declaring_class ()->add (decl);
}

void
model_class_decl_stmt::visit (visitor *v)
{
  v->visit_class_decl_stmt (this, decl);
}
