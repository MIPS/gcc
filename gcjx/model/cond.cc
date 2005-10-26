// Conditional expression.

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
#include "unify.hh"

void
model_conditional::resolve (resolution_scope *scope)
{
  condition->resolve (scope);
  true_branch->resolve (scope);
  false_branch->resolve (scope);

  model_type *true_type = true_branch->type ();
  model_type *false_type = false_branch->type ();

  if (! boolean_conversion (condition))
    throw condition->error ("conditional expression not of boolean type");

  if (true_type == primitive_void_type)
    throw true_branch->error ("%<void%> type is invalid in conditional");
  if (false_type == primitive_void_type)
    throw false_branch->error ("%<void%> type is invalid in conditional");

  if (true_type == false_type)
    {
      set_type (true_type);
      return;
    }

  // Note that if both arguments have type Boolean, then we do not
  // unwrap.
  if ((true_type == primitive_boolean_type
       && boolean_conversion (false_type))
      || (false_type == primitive_boolean_type
	  && boolean_conversion (true_type)))
    {
      maybe_cast_wrap (primitive_boolean_type, true_branch);
      maybe_cast_wrap (primitive_boolean_type, false_branch);
      set_type (primitive_boolean_type);
      return;
    }

  // Handle null types.
  if (true_type == null_type && false_type->reference_p ())
    {
      set_type (false_type);
      return;
    }
  if (false_type == null_type && true_type->reference_p ())
    {
      set_type (true_type);
      return;
    }

  if (convertible_to_numeric_p (true_type)
      && convertible_to_numeric_p (false_type))
    {
      // Unbox as needed.
      model_type *tun = unboxing_conversion (true_type);
      if (tun != NULL)
	true_type = tun;
      model_type *fun = unboxing_conversion (false_type);
      if (fun != NULL)
	false_type = fun;

      if ((true_type == primitive_byte_type
	   && false_type == primitive_short_type)
	  || (true_type == primitive_short_type
	      && false_type == primitive_byte_type))
	{
	  // We might need explicit unwrapping.
	  maybe_cast_wrap (true_type, true_branch);
	  maybe_cast_wrap (false_type, false_branch);
	  set_type (primitive_short_type);
	}
      else if ((true_type == primitive_byte_type
		|| true_type == primitive_char_type
		|| true_type == primitive_short_type)
	       && false_type == primitive_int_type
	       && false_branch->constant_p ()
	       && representable_p (true_type, false_branch.get ()))
	{
	  // We might need explicit unwrapping.
	  maybe_cast_wrap (true_type, true_branch);
	  maybe_cast_wrap (false_type, false_branch);
	  set_type (true_type);
	}
      else if ((false_type == primitive_byte_type
		|| false_type == primitive_char_type
		|| false_type == primitive_short_type)
	       && true_type == primitive_int_type
	       && true_branch->constant_p ()
	       && representable_p (false_type, true_branch.get ()))
	{
	  // We might need explicit unwrapping.
	  maybe_cast_wrap (true_type, true_branch);
	  maybe_cast_wrap (false_type, false_branch);
	  set_type (false_type);
	}
      else
	set_type (binary_numeric_promotion (true_branch, false_branch));
    }
  else
    {
      // Reference types.
      // Also, this will fail if a primitive type makes it here...
      model_type *merge;
      if (global->get_compiler ()->feature_generics ())
	{
	  // FIXME: we don't have capture conversion yet, but should
	  // run it on the result here.
	  merge = compute_lub (this, assert_cast<model_class *> (true_type),
			       assert_cast<model_class *> (false_type));
	}
      else
	{
	  merge = assignment_conversion (true_type, false_type);
	  if (merge == NULL)
	    merge = assignment_conversion (false_type, true_type);
	}
      if (merge == NULL)
	// fixme bad message
	throw error ("operands of condition have types %1 and %2, "
		     "which are not reconcilable via "
		     "assignment conversion")
	  % true_type % false_type;
      else
	set_type (merge);
    }
}

void
model_conditional::visit (visitor *v)
{
  v->visit_conditional (this, condition, true_branch, false_branch);
}

jvalue
model_conditional::value ()
{
  jboolean cv = jboolean (condition->value ());
  model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
  if (cv)
    return bt->convert (true_branch->type (), true_branch->value ());
  return bt->convert (false_branch->type (), false_branch->value ());
}

std::string
model_conditional::string_value ()
{
  assert (type () == global->get_compiler ()->java_lang_String ());
  jboolean cv = jboolean (condition->value ());
  return cv ? true_branch->string_value () : false_branch->string_value ();
}
