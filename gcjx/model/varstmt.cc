// Variable declaration statements.

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
model_variable_stmt::resolve (resolution_scope *scope)
{
  for (std::list<ref_variable_decl>::const_iterator i = vars.begin ();
       i != vars.end ();
       ++i)
    {
      std::set<model_variable_decl *> found;
      scope->look_up_name ((*i)->get_name (), found, NULL, NULL);
      model_variable_decl *var = NULL;
      if (found.size () == 1)
	var = *(found.begin ());
      // We are allowed to shadow fields.
      if (var && dynamic_cast<model_field *> (var) == NULL)
	{
	  model_variable_decl *decl = assert_cast<model_variable_decl *> (var);
	  if (decl->get_declaring_class () == scope->get_current_class ())
	    // FIXME: location of earlier decl.
	    throw (*i)->error ("variable %1 shadows earlier declaration")
	      // FIXME: should have its own operator?
	      % (*i)->get_name ();
	}

      // Push variable into scope immediately, as a variable's
      // initializer is in the scope of its declaration.
      scope->add_binding ((*i).get ());
      (*i)->resolve (scope);
    }
}

void
model_variable_stmt::visit (visitor *v)
{
  v->visit_variable_stmt (this, vars);
}
