// Represent a block of bytecode.

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
#include "verify.h"

void
model_bytecode_block::resolve (resolution_scope *scope)
{
  // If we did't want to verify the method, we would have created a
  // model_phony_block instead.
  model_method *m = scope->get_current_method ();
  model_unit_class *unit
    = assert_cast<model_unit_class *> (scope->get_compilation_unit ());
  verify (m, unit);
}

void
model_bytecode_block::verify (model_method *container, model_unit_class *unit)
{
  if (! verified)
    {
      resolution_scope scope;
      model_class *decl = container->get_declaring_class ();
      decl->push_on_scope (&scope);

      if (unit == NULL)
	{
	  model_class *klass = container->get_declaring_class ();
	  unit
	    = assert_cast<model_unit_class *> (klass->get_compilation_unit ());
	}

      if (global->get_compiler ()->verbose ())
	std::cout << "[verifying method "
		  << decl->get_fully_qualified_name ()
		  << "."
		  << container->get_name ()
		  << "]" << std::endl;

      vfy_method m (container, this, &scope, unit);
      _Jv_VerifyMethod (&m);
      // FIXME: this API isn't ideal if we verify multiple times and
      // get an error each time.  Instead we could catch the error and
      // have a true/false return value...?
      verified = true;
    }
}

void
model_bytecode_block::visit (visitor *v)
{
  v->visit_bytecode_block (this, max_stack, max_locals, length, bytes);
}



void
model_phony_block::visit (visitor *v)
{
  v->visit_phony_block (this, statements);
}
