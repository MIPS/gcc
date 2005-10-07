// Represent a variable.

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
model_variable_decl::resolve_classes (resolution_scope *scope)
{
  decltype->resolve (scope);
  resolve_annotation_classes (scope);
}

void
model_variable_decl::resolve (resolution_scope *scope)
{
  resolve_annotations (scope);
  decltype->resolve (scope);  // fixme redundant for fields...
  if (initializer)
    {
      initializer->resolve (scope);
      if (! assignment_conversion (decltype->type (), initializer))
	// FIXME error message!
	throw initializer->error ("initializer for %1 "
				  "can't be converted from type "
				  "%2 to type %3")
	  % name % initializer->type () % decltype->type ();
      // We fold after assignment conversion, so that casts can be
      // pushed into the literal.  Besides being cleaner, this lets
      // us generate somewhat better bytecode more easily.
      fold (initializer);
    }
}

bool
model_variable_decl::constant_p ()
{
  return (final
	  && constant_type_p (decltype->type ())
	  && initializer
	  && initializer->constant_p ());
}

model_variable_decl *
model_variable_decl::apply_type_map (const model_type_map &type_map,
				     model_class *enclosing)
{
  model_type *self_type = type ();
  if (self_type->primitive_p ())
    return this;

  model_class *k = assert_cast<model_class *> (self_type);
  model_class *new_type = k->apply_type_map (enclosing, type_map);
  // If our type didn't change, don't bother creating a new variable.
  if (new_type == k)
    return this;
  ref_forwarding_type fw = new model_forwarding_resolved (get_location (),
							  new_type);
  model_variable_decl *result = new model_variable_decl (get_location (),
							 name, fw,
							 enclosing);
  if (final)
    result->set_final ();
  return result;
}

std::string
model_variable_decl::get_signature ()
{
  return type ()->get_signature ();
}

void
model_variable_decl::check_referenced (resolution_scope *scope)
{
  warning_scope::push_warnings push (scope, this);
  if (! used && scope->warn_unused_variable ())
    std::cerr << warn (global->get_compiler ()->warn_unused_variable (),
		       "variable %1 unused")
      % this->get_name ();
}

void
model_variable_decl::visit (visitor *v)
{
  v->visit_variable_decl (this, name, decltype, initializer, final, used);
}

void
model_parameter_decl::check_referenced (resolution_scope *scope)
{
  warning_scope::push_warnings push (scope, this);
  if (! used && scope->warn_unused_parameter ())
    std::cerr << warn (global->get_compiler ()->warn_unused_parameter (),
		       "parameter %1 unused")
      % this->get_name ();
}

void
model_parameter_decl::visit (visitor *v)
{
  v->visit_parameter_decl (this, name, decltype, initializer, final, used);
}

void
model_catch_decl::visit (visitor *v)
{
  v->visit_catch_decl (this, name, decltype, initializer, final, used);
}
