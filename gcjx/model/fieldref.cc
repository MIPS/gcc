// Field references.

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

jvalue
model_field_ref::value ()
{
  assert (! expression || synthetic_expr);
  return field->value ();
}

std::string
model_field_ref::string_value ()
{
  assert (! expression || synthetic_expr);
  return field->string_value ();
}

void
model_field_ref::resolve (resolution_scope *scope)
{
  // A field reference might be resolved multiple times, since it can
  // also be resolved during name lookup.
  if (type_cache.set_p ())
    return;

  if (expression)
    {
      expression->resolve (scope);
      if (! expression->type ()->reference_p ())
	throw expression->error ("expression qualifying field reference "
				 "must be of reference type");
      fold (expression);
    }

  if (! field)
    {
      model_class *local_qualifier = NULL;
      model_class *search;
      if (super)
	{
	  model_class *enclosing_class;

	  if (enclosing)
	    {
	      enclosing->resolve_classes (scope);
	      if (! enclosing->type ()->reference_p ())
		throw enclosing->error ("enclosing class must be "
					"of reference type");
	      enclosing_class
		= assert_cast<model_class *> (enclosing->type ());

	      if (! enclosing_class->has_as_member_of_p (scope->get_current_class ()))
		throw enclosing->error ("type named in qualified %<super%> "
					"must enclose %1")
		  % scope->get_current_class ();
	    }
	  else
	    enclosing_class = scope->get_current_class ();

	  if (enclosing_class == global->get_compiler ()->java_lang_Object ())
	    throw error ("can't use %<super%> in %<java.lang.Object%>");
	  else if (enclosing_class->interface_p ())
	    throw error ("can't use %<super%> in interface %1")
	      % enclosing_class;
	  else if (scope->static_p ())
	    throw error ("can't use %<super%> in static context");

	  local_qualifier = enclosing_class;
	  search = enclosing_class->get_superclass ();
	  assert (search);
	}
      else
	{
	  // For 'expr.field', use the type of 'expr', which was
	  // resolved above.  Otherwise use the type of the enclosing
	  // class.
	  if (expression)
	    search = assert_cast<model_class *> (expression->type ());
	  else
	    search = scope->get_current_class ();
	}

      field = classify_field_name (this, search, scope->get_current_class (),
				   name, local_qualifier);
      if (! field)
	throw error ("no field named %1 in type %2")
	  % name % search;
    }

  model_class *field_class = field->get_declaring_class ();

  model_class *curr = scope->get_current_class ();
  // If there is no expression, we might either have found an instance
  // field, or we might be using 'super', which also requires a "this"
  // reference -- even if the discovered field is not static.
  if (! expression && (super || ! field->static_p ()))
    {
      // This is strange: we might be accessing a private field via
      // "super.field", in which case we don't want the special
      // 'private' handling for 'this'.
      bool priv = ! super && (field->get_modifiers () & ACC_PRIVATE) != 0;
      expression = model_this::get_synthetic_this (get_location (),
						   curr, field_class,
						   priv);
      assert (expression);
      // Make sure to resolve it as well.
      expression->resolve (scope);
      synthetic_expr = true;
    }

  // FIXME: does this really make sense?  Should we care if a field is
  // only set but never referenced?
  field->set_used ();
  field->check_deprecated (this);

  if (expression && field->static_p ()
      && scope->warn_non_static_context ())
    std::cerr << warn (global->get_compiler ()->warn_non_static_context (),
		       "static field %1 used with non-static qualifier")
      % field;

  // We don't require a trampoline for an inlineable field, as no
  // actual access will be done.
  model_class *accessed;
  if (! field->inlineable_p ()
      && trampoline_required_p (field, scope->get_current_class (), &accessed))
    {
      // Ensure that the required accessor exists.
      // FIXME: only do this if the target environment requires it.
      // What this means is that old-ABI GCC doesn't need an
      // accessor; we should be able to query back ends for a few
      // little details like this.  Or perhaps not... since what if
      // there are multiple back ends with different attributes?
      accessed->get_accessor (field, is_left_hand_side);
      // For compound assignment we need both accessors.
      if (is_compound)
	accessed->get_accessor (field, false);
      if (scope->warn_enclosing_access ())
	{
	  std::cerr << warn (global->get_compiler ()->warn_enclosing_access (),
			     "field %1 requires accessor")
	    % field;
	  std::cerr
	    << field->warn (global->get_compiler ()->warn_enclosing_access (),
			    "field is defined here");
	}
    }

  if (expression)
    qualifier = assert_cast<model_class *> (expression->type ());
  else
    {
      // This is an unqualified reference to a field.  In this case we
      // use the implicit qualifier.  FIXME: if we have a qualified
      // reference like 'Derived.field', but the field is actually
      // declared in 'Base', we will wind up with 'Base.field' here,
      // which is incorrect.  Fixing this requires extracting more
      // information from classify_expression_name().  Even the
      // commented out code here doesn't work, since for something
      // like 'System.out.println()', nothing tells the field reference
      // that it is qualified.
      //       qualifier = (qualified ? field->get_declaring_class ()
      // 		   : scope->get_current_class ());
      qualifier = field->get_declaring_class ();
    }

  set_type (field->type ());
}

void
model_field_ref::visit (visitor *v)
{
  v->visit_field_ref (this, expression, field);
}
