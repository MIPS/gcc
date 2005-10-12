// "new" operator

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
model_new::determine_enclosing_instance (resolution_scope *scope,
					 model_class *klass,
					 ref_expression &immediate,
					 ref_expression &with_respect_to)
{
  model_class *current = scope->get_current_class ();
  location here = get_location ();

  if (klass->anonymous_p ())
    {
      if (! klass->static_context_p ())
	{
	  immediate = new model_synthetic_this (here, current);
	  immediate->resolve (scope);
	}

      model_class *super = klass->get_superclass ();
      if (super->local_p ())
	{
	  if (! super->static_context_p () && ! super->static_p ())
	    {
	      if (scope->static_p ())
		throw error ("unqualified %<new%> of anonymous class with "
			     "inner superclass in static context");
	      with_respect_to
		= new model_synthetic_this (here,
					    super->get_lexically_enclosing_class ());
	    }
	}
      else if (super->member_p () && ! super->static_p ())
	{
	  if (! expression)
	    {
	      if (scope->static_p ())
		throw error ("unqualified %<new%> of anonymous class with "
			     "inner superclass in static context");
	      with_respect_to
		= new model_synthetic_this (here,
					    super->get_lexically_enclosing_class ());
	    }
	  else
	    with_respect_to = new model_synthetic_this (here,
							expression->type ());
	}
    }
  else if (klass->local_p ())
    {
      if (! klass->static_context_p ())
	{
	  if (scope->static_p ())
	    throw error ("unqualified %<new%> of inner class "
			 "in static context");
	  model_class *outer = klass->get_lexically_enclosing_class ();
	  immediate = new model_synthetic_this (here, outer);
	  immediate->resolve (scope);
	}
    }
  else if (klass->member_p () && ! klass->static_p ())
    {
      if (! expression)
	{
	  if (scope->static_p ())
	    throw error ("unqualified %<new%> of inner "
			 "class in static context");
	  model_class *outer = klass->get_lexically_enclosing_class ();
	  immediate = new model_synthetic_this (here, outer);
	  immediate->resolve (scope);
	}
      else
	{
	  // Qualified class instance creation expression.
	  immediate = expression;
	}
    }
}

void
model_new::finish_search_class (resolution_scope *scope,
				model_class **result,
				model_class **qualifier)
{
  (*result)->check_instantiation (this);

  if ((*result)->anonymous_p ())
    {
      // If we're in a static context and we have an anonymous class,
      // make sure to mark it appropriately before we start digging
      // around.
      if (scope->static_p ())
	(*result)->set_static_context ();
      (*result)->resolve_classes (scope);
    }

  ref_expression immediate;
  ref_expression with_respect_to;
  determine_enclosing_instance (scope, *result, immediate, with_respect_to);

  if ((*result)->anonymous_p ())
    {
      // We must fully resolve an anonymous class here, as it doesn't
      // appear anywhere else.
      (*result)->resolve (scope);
      (*result)->add_anonymous_constructor (scope, with_respect_to, arguments);
    }

  // We don't want to pass this argument to add_anonymous_constructor;
  // the constructor will add the required formal parameter itself.
  if (immediate)
    arguments.push_front (immediate);

  // For local or anonymous classes, we might have captured variables,
  // in which case we must also update the constructor arguments.
  if ((*result)->local_p () || (*result)->anonymous_p ())
    (*result)->add_capturing_arguments (scope, arguments);
}

void
model_new::determine_search_class (resolution_scope *scope,
				   model_class **result,
				   model_class **qualifier)
{
  // This is plain old 'new foo'.
  assert (! expression);
  klass->resolve (scope);
  if (! klass->type ()->reference_p ())
    throw klass->error ("argument to %<new%> must have reference type");
  *result = assert_cast<model_class *> (klass->type ());
  *qualifier = *result;

  finish_search_class (scope, result, qualifier);
}

void
model_new::resolve (resolution_scope *scope)
{
  // FIXME: this is a hack -- all objects should keep track of their
  // own resolution status.  In this particular case we get a crash
  // since this class can't re-resolve itself.
  if (method)
    return;

  handle_resolve (scope);

  // Tell the class about the anonymous class, if there is one.  This
  // makes it a bit easier to decide what classes go through code
  // generation.
  if (anonymous)
    scope->get_current_class ()->add (anonymous);

  set_type (klass->type ());
}

void
model_new::visit (visitor *v)
{
  // Note that if there is an expression, it will already be part of
  // 'arguments'.
  v->visit_new (this, method, klass, arguments);
}



void
model_new_primary::determine_search_class (resolution_scope *scope,
					   model_class **result,
					   model_class **qualifier)
{
  // This is qualified 'primary . new foo'.
  assert (expression);
  // At this point we know 'expression' has been resolved.
  model_type *base = expression->type ();
  if (! base->reference_p ())
    throw error ("qualified %<new%> with expression of %1 type")
      % base;

  model_class *search = assert_cast<model_class *> (base);
  std::set<model_class *> classes;
  // FIXME: search upwards?
  search->find_members (simple_name, classes, scope->get_current_class ());
  int len = classes.size ();
  if (len > 1)
    throw error ("simple name %1 names two accessible "
		 "member classes of type %2")
      % simple_name % search;
  else if (len == 0)
    throw error ("simple name %1 names no accessible "
		 "member classes of type %2")
      % simple_name % search;

  *result = *(classes.begin ());
  *qualifier = search;

  // In this case, we saw 'primary.new Name(...) { ... }'.
  // We've found the name in the primary's class, so use that as
  // the anonymous class' super class and use the anonymous class
  // here.
  if (anonymous)
    {
      if (! (*result)->inner_p ())
	throw error ("simple name %1 does not name an inner class")
	  % simple_name;
      anonymous->set_superclass (new model_forwarding_resolved (get_location (),
								*result));
      *result = anonymous.get ();
    }

  klass = new model_forwarding_resolved (get_location (), *result);

  finish_search_class (scope, result, qualifier);
}

void
model_new_primary::visit (visitor *v)
{
  v->visit_new_primary (this, method, klass, arguments, simple_name,
                        type_params);
}
