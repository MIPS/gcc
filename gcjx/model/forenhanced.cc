// Enhanced for loop.

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

// A helper function to find a no-argument method in a class.
// Issues error and returns null if no match or ambiguous match.
static model_method *
find_method (resolution_scope *scope, model_element *where, model_class *klass,
	     const std::string &name)
{
  std::set<model_method *> methods;
  klass->find_members (name, methods, scope->get_current_class (), klass);
  if (methods.empty ())
    {
      std::cerr << where->error ("couldn't find method %1 for type %2")
	% name % klass;
      return NULL;
    }

  model_method *candidate = NULL;
  for (std::set<model_method *>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      if ((*i)->get_parameter_count () == 0)
	{
	  if (candidate != NULL)
	    abort ();	// FIXME
	  candidate = *i;
	}
    }
  if (! candidate) abort (); 	// FIXME
  return candidate;
}

void
model_for_enhanced::resolve (resolution_scope *scope)
{
  model_block_scope vscope (scope);
  resolution_scope::push_iscope var_holder (scope, &vscope);

  variable->resolve (scope);
  // FIXME: perhaps we should call use_assignment_conversion here?
  expression->resolve (scope);
  fold (expression);
  if (expression->type ()->array_p ())
    {
      // Ok.  We pull this out so we don't try to load Iterable unless
      // really necessary.  This is a little strange but it does let
      // users iterate over arrays even with an older class library.
      elt_type = expression->type ()->element_type ();
    }
  else
    {
      model_type *iterable
	= global->get_compiler ()->java_lang_Iterable ();
      if (! iterable->assignable_from_p (expression->type ()))
	std::cerr << expression->error ("%<for%> expression has type %1, which"
					" isn't an array and doesn't"
					" implement %<java.lang.Iterable%>")
	  % expression->type ();

      // Find the type of the iterator.  This is fairly roundabout --
      // we find the 'iterator' method, then look at its return type
      // for the 'next' method.  This seems robust, if lengthy.
      model_class *expr_class
	= assert_cast<model_class *> (expression->type ());
      model_method *iterator = find_method (scope, expression.get (),
					    expr_class, "iterator");
      if (iterator)
	{
	  model_type *rtype = iterator->get_return_type ();
	  if (! rtype->reference_p ())
	    {
	      // FIXME
	      abort ();
	    }
	  model_class *rclass = assert_cast<model_class *> (rtype);
	  model_method *next = find_method (scope, expression.get (),
					    rclass, "next");
	  if (next)
	    elt_type = next->get_return_type ();
	}
    }

  // If ELT_TYPE is NULL, we have already issued an error.
  if (elt_type != NULL
      && ! assignment_conversion (variable->type (), elt_type))
    std::cerr << expression->error ("%<for%> expression of type %1"
				    " is not assignment compatible with"
				    " variable of type %2")
      % elt_type
      % variable->type ();

  scope->add_binding (variable.get ());
  body->resolve (scope);
}

void
model_for_enhanced::visit (visitor *v)
{
  v->visit_for_enhanced (this, body, expression, variable);
}

void
model_for_enhanced::compute_normal_completion (normal_completion_state &state)
{
  // The document doesn't specify this very well, but I believe the
  // body should always be reachable, and the extended for completes
  // normally iff the body does.
  normal_completion_pusher push (state, this);
  body->compute_normal_completion (state);
  normal_completion = body->can_complete_normally ();
}
