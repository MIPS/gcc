// Method invocation.

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
model_invocation_base::try_method_conversion
  (const std::set<model_method *> &accessible,
   const std::list<model_type *> &actual_types,
   std::set<model_method *> &applicable)
{
  // Construct a new set of applicable methods, implementing the
  // 3-phase process if needed.  There's no point in looping if
  // the feature is disabled, so we check that up front for a
  // small efficiency gain.
  bool should_loop
    = (global->get_compiler ()->feature_varargs ()
       || global->get_compiler ()->feature_boxing_conversion ());
  // FIXME: really this is wrong:
  // * we don't handle the subtyping phase
  // * we don't handle generic methods
  // * it isn't clear immediately whether we compute all
  //   applicable methods first, or whether we do phases as
  //   implemented now
  // * later on, the code for computing most specific must be
  //   updated for varargs
  for (method_phase phase = PHASE_1;
       phase < PHASE_TOO_FAR && applicable.empty ();
       phase = (method_phase) (phase + 1))
    {
      for (std::set<model_method *>::const_iterator i
	     = accessible.begin ();
	   i != accessible.end ();
	   ++i)
	{
	  if ((*i)->method_conversion_p (actual_types, phase))
	    applicable.insert (*i);
	}
      if (! should_loop)
	break;
    }
}

void
model_invocation_base::search_applicable_methods
  (resolution_scope *scope,
   model_class *qualifier,
   const std::list<model_type *> &actual_types,
   std::set<model_method *> &applicable)
{
  std::set<model_method *> accessible;

  // Find all methods with the given name that are accessible.
  while (search)
    {
      search->find_members (name, accessible, scope->get_current_class (),
			    qualifier);

      if (! accessible.empty ())
	{
	  // This is bogus... we need to do the checking in
	  // classify_expression_name, some stuff needs to be
	  // refactored out of this loop, etc.
	  try_method_conversion (accessible, actual_types, applicable);

	  // If there are any candidates, we're done.
	  if (! applicable.empty ())
	    return;
	}

      // Don't search for constructors.
      if (name == "<init>")
	return;

      // Stop searching if we found any method, even if not applicable
      // -- this method hides methods in outer classes.
      if (! accessible.empty ())
	return;

      // Search an outer class.  FIXME: I think this is wrong in some
      // situations.  Note that we intentionally update the 'search'
      // field here.
      search = search->get_lexically_enclosing_class ();
    }

  // Now try static imports, but only for unqualified names.
  if (unqualified)
    {
      model_unit *unit = scope->get_compilation_unit ();
      // FIXME: context argument probably wrong here.
      unit->look_up_name (name, accessible, scope->get_current_class (),
			  NULL);
      if (! accessible.empty ())
	{
	  // All the methods come from the same type, so it doesn't
	  // matter how we set 'search'.
	  search = (*(accessible.begin ()))->get_declaring_class ();
	  try_method_conversion (accessible, actual_types, applicable);
	}
    }
}

void
model_invocation_base::find_method (resolution_scope *scope)
{
  // Determine class or interface to search.
  model_class *qualifier = NULL;
  determine_search_class (scope, &search, &qualifier);
  assert (search);

  std::set<model_method *> applicable;

  // Compute the actual types of the parameters, used for determining
  // applicability.
  std::list<model_type *> actual_types;
  for (std::list<ref_expression>::const_iterator i = arguments.begin ();
       i != arguments.end ();
       ++i)
    actual_types.push_back ((*i)->type ());

  model_class *err_search = search;
  search_applicable_methods (scope, qualifier, actual_types, applicable);

  if (applicable.size () == 1)
    {
      // Found it.
      method = *(applicable.begin ());
      method->set_used ();
      return;
    }

  if (applicable.empty ())
    {
      // Consider sharing this code with
      // model_method::get_pretty_name().
      std::string sig = "(";
      bool first = true;
      for (std::list<model_type *>::const_iterator i = actual_types.begin ();
	   i != actual_types.end ();
	   ++i)
	{
	  if (! first)
	    sig += ", ";
	  sig += (*i)->get_pretty_name ();
	  first = false;
	}
      sig += ")";
      throw error ("no applicable, accessible method named %1 with signature "
		   "matching %2 in type %3")
	% name % sig % err_search;
    }

  // There is more than one applicable, accessible method, so we must
  // determine maximally specific methods.  We also do some other
  // bookkeeping so we can issue the proper error messages.
  std::set<model_method *> maximal;
  std::string descriptor;
  bool ambiguous = false;
  model_method *non_abstract = NULL;
  model_method *non_interface = NULL;
  for (std::set<model_method *>::const_iterator outer = applicable.begin ();
       outer != applicable.end ();
       ++outer)
    {
      bool is_maximal = true;
      for (std::set<model_method *>::const_iterator inner
	     = applicable.begin ();
	   inner != applicable.end ();
	   ++inner)
	{
	  // Make sure not to compare against ourself.
	  if (*inner == *outer)
	    continue;

	  // Note how we do the check here.  A method is maximally
	  // specific if it is more specific than all the other
	  // methods.  However, this relationship isn't necessarily
	  // one way, as two methods with identical argument types are
	  // both "more specific" than each other.  In a situation
	  // like that we want all the methods to end up on our
	  // maximal list.
	  if (! (*outer)->more_specific_p (*inner))
	    {
	      is_maximal = false;
	      break;
	    }
	}

      if (is_maximal)
	{
	  maximal.insert (*outer);
	  // We need the classes of the target method to be resolved
	  // before we can ask for its descriptor.
	  (*outer)->get_declaring_class ()->resolve_classes ();
	  std::string newsig = (*outer)->get_descriptor ();
	  if (descriptor.empty ())
	    descriptor = newsig;
	  else if (descriptor != newsig)
	    ambiguous = true;
	  if (! (*outer)->abstract_p ())
	    non_abstract = *outer;
	  else if (! (*outer)->get_declaring_class ()->interface_p ())
	    non_interface = *outer;
	}
    }

  if (ambiguous || maximal.empty ())
    {
      // FIXME: would like to include all the info...
      throw error ("ambiguous call to %1") % name;
//       std::cerr << error ("candidates are:");
//       for (std::set<model_method *>::const_iterator i = maximal.begin ();
// 	   i != maximal.end ();
// 	   ++i)
// 	std::cerr << (*i)->error ("%1") % *i;
    }

  if (non_abstract)
    method = non_abstract;
  else if (maximal.size () == 1)
    method = *(maximal.begin ());
  else
    {
      // We choose a method arbitrarily.  It is a little better to
      // choose a method declared in a class, as opposed to an
      // interface, since invokeinterface is usually slower.
      model_method *chosen = (non_interface
			      ? non_interface
			      : *(maximal.begin ()));

      phony = new model_abstract_method (chosen);
      phony->set_name (chosen->get_name ());
      phony->set_modifiers (chosen->get_modifiers ());
      // FIXME
      // phony->set_type_parameters (chosen->get_type_parameters ());
      phony->set_parameters (chosen->get_parameters ());
      // FIXME: creating new object for no reason...
      phony->set_return_type
	(new model_forwarding_resolved (chosen->get_location (),
					chosen->get_return_type ()));
      if (chosen->varargs_p ())
	phony->set_varargs ();

      // Now merge the 'throws' clauses.  The new method has a checked
      // exception only if it is declared in all the methods.
      model_throws_clause new_throws (maximal);
      phony->set_throws (new_throws);

      method = phony.get ();
    }

  method->set_used ();
}

void
model_invocation_base::handle_resolve (resolution_scope *scope,
				       bool is_special_static_context)
{
  if (expression)
    {
      // A false argument means "whatever the surrounding scope says".
      // A true argument means "the qualifier can be 'this'".
      model_static_context_scope stat (is_special_static_context ? false
				       : scope->static_p ());
      resolution_scope::push_iscope holder (scope, &stat);

      expression->resolve (scope);
      if (! expression->type ()->reference_p ())
	throw error ("expression qualifying method call must have "
		     "reference type");
      // Expression might have constants requiring folding, or could
      // even be a constant string expression itself.
      fold (expression);
    }
  ::resolve (scope, arguments);

  find_method (scope);
  assert (method);
  method->check_deprecated (this);

  if (expression && method->static_p () && scope->warn_non_static_context ())
    std::cerr << warn (global->get_compiler ()->warn_non_static_context (),
		       "static method %1 used with non-static qualifier")
      % method;

  // Convert arguments as needed.
  method->method_conversion (arguments);

  if (! expression && ! method->static_p () && ! method->constructor_p ())
    {
      model_class *curr = scope->get_current_class ();
      // This is strange: we might be accessing a private method via
      // "super.method()", in which case we don't want the special
      // 'private' handling for 'this'.
      bool priv = (! super_call_p ()
		   && (method->get_modifiers () & ACC_PRIVATE) != 0);
      expression
	= model_this::get_synthetic_this (get_location (),
					  curr,
					  method->get_declaring_class (),
					  priv);
      expression->resolve (scope);
    }

  model_class *accessed;
  if (trampoline_required_p (method, scope->get_current_class (), &accessed))
    {
      // Ensure that the required accessor exists.
      model_method *accessor = accessed->get_accessor (method);
      if (scope->warn_enclosing_access ())
	{
	  std::cerr << warn (global->get_compiler ()->warn_enclosing_access (),
			     "method %1 requires forwarding call")
	    % method;
	  std::cerr
	    << method->warn (global->get_compiler ()->warn_enclosing_access (),
			     "method is defined here");
	}
      if (method->constructor_p ())
	{
	  // Constructors are handled by adding extra 'boolean'
	  // arguments.
	  int extra = (accessor->get_parameter_count ()
		       - method->get_parameter_count ());
	  // It is ok to share substructure here.
	  ref_expression narg = new model_boolean_literal (get_location (),
							   true);
	  narg->resolve (scope);
	  for (int i = 0; i < extra; ++i)
	    arguments.push_back (narg);
	}
    }

  method->propagate_throws (scope);
}

void
model_invocation_base::resolve (resolution_scope *scope)
{
  // FIXME: these casts are bogus, could be done another way.
  bool is_static = (dynamic_cast<model_super_invocation *> (this)
		    || dynamic_cast<model_this_invocation *> (this));
  handle_resolve (scope, is_static);
  set_type (method->get_return_type ());
}

model_class *
model_invocation_base::get_qualifying_class () const
{
  model_class *qualifier = search;
  // From the binary compatibility spec: if the method's declaring
  // class is Object, then that is the qualifier.
  if (method->get_declaring_class ()
      == global->get_compiler ()->java_lang_Object ())
    qualifier = method->get_declaring_class ();
  return qualifier;
}



void
model_method_invocation::determine_search_class (resolution_scope *scope,
						 model_class **result,
						 model_class **qualifier)
{
  if (! expression)
    {
      // This is an unqualified method call.
      *result = scope->get_current_class ();
    }
  else
    {
      // This is a method invocation qualified by a primary or a field
      // name.
      assert (! expression->type ()->primitive_p ());
      *result = assert_cast<model_class *> (expression->type ());
    }
  *qualifier = *result;
}

void
model_method_invocation::visit (visitor *v)
{
  v->visit_method_invocation (this, method, expression, arguments);
}



void
model_type_qualified_invocation::determine_search_class
    (resolution_scope *scope,
     model_class **result,
     model_class **qualifier)
{
  if (klass)
    {
      klass->resolve_classes (scope);
      if (! klass->type ()->reference_p ())
	throw klass->error ("qualifier must have reference type");
      *result = assert_cast<model_class *> (klass->type ());
    }
  else
    *result = scope->get_current_class ();
  *qualifier = *result;

  if (super)
    {
      (*result)->ensure_enclosing (scope->get_current_class ());

      if (*result == global->get_compiler ()->java_lang_Object ())
	throw error ("%<super%>-qualified invocation invalid for "
		     "type %<java.lang.Object%>")
	  % *result;
      if ((*result)->interface_p ())
	throw error ("argument type %1 to %<super%> must not have "
		     "interface type")
	  % *result;
      if (scope->static_p ())
	throw error ("can't use %<super%> in static context");

      *result = (*result)->get_superclass ();
    }
}

void
model_type_qualified_invocation::resolve (resolution_scope *scope)
{
  model_invocation_base::resolve (scope);

  if (super && method->abstract_p ())
    throw error ("%<super%> call resolves to %<abstract%> method %1")
      % method;

  if (! super && ! method->static_p ())
    throw error ("type qualified call must not resolve to "
		 "non-%<static%> method %1")
      % method;
}

void
model_type_qualified_invocation::visit (visitor *v)
{
  v->visit_type_qualified_invocation (this, method, arguments, super);
}



void
model_super_invocation::determine_search_class (resolution_scope *scope,
						model_class **result,
						model_class **qualifier)
{
  if (expression)
    {
      *qualifier = scope->get_current_class ();
      *result = (*qualifier)->get_superclass ();
    }
  else
    {
      assert (enclosing);
      enclosing->resolve_classes ();
      *qualifier = enclosing;
      *result = enclosing->get_superclass ();
    }

  model_class *s = *result;
  if (! s->inner_p () || s->static_context_p ())
    {
      if (expression)
	throw error ("qualified %<super%> not valid in static context");
    }
  else if (expression)
    {
      model_class *o = s->get_lexically_enclosing_class ();
      if (! o->assignable_from_p (expression->type ()))
	throw error ("expression type %1 not assignable to "
		     "enclosing class %2 ")
	  % expression->type () % o;
    }
  else if (s->local_p ())
    {
      // The implicit 'this' is valid so we need a special scope.
      model_static_context_scope stat (false);
      resolution_scope::push_iscope holder (scope, &stat);
      expression = new model_synthetic_this (get_location (),
					     s->get_lexically_enclosing_class ());
      expression->resolve (scope);
    }
  else
    {
      if (s->get_lexically_enclosing_class () == NULL)
	throw error ("no enclosing class");
      // The implicit 'this' is valid so we need a special scope.
      model_static_context_scope stat (false);
      resolution_scope::push_iscope holder (scope, &stat);
      expression = new model_synthetic_this (get_location (),
					     s->get_lexically_enclosing_class ());
      expression->resolve (scope);
    }

  if (expression)
    arguments.push_front (expression);
}

void
model_super_invocation::resolve (resolution_scope *scope)
{
  {
    // The 'super' call is a static context.
    model_static_context_scope static_holder (true);
    resolution_scope::push_iscope holder (scope, &static_holder);
    model_invocation_base::resolve (scope);
  }

  // An explicit 'super' in an enum class is invalid.
  model_class *current = scope->get_current_class ();
  if (! synthetic && current->enum_p ())
    throw error ("explicit %<super%> invocation is invalid in %<enum%> class");

  // An explicit 'super' is invalid in Object.
  if (current == global->get_compiler ()->java_lang_Object ())
    throw error ("%<super%> invocation invalid in %<java.lang.Object%>");

  // Create the 'finit$' call if needed.
  model_method *finit_m = current->get_finit_method ();
  if (finit_m)
    {
      ref_method_invocation nfm
	= new model_method_invocation (get_location ());
      nfm->set_method (finit_m->get_name ());

      finit = nfm;
      finit->resolve (scope);
    }
}

void
model_super_invocation::visit (visitor *v)
{
  v->visit_super_invocation (this, method, arguments, finit);
}



void
model_this_invocation::determine_search_class (resolution_scope *,
					       model_class **result,
					       model_class **qualifier)
{
  assert (! bool (expression));
  assert (enclosing);
  *result = enclosing;
  *qualifier = enclosing;
}

void
model_this_invocation::visit (visitor *v)
{
  v->visit_this_invocation (this, method, arguments);
}

void
model_this_invocation::resolve (resolution_scope *scope)
{
  // The 'this' call is a static context.
  model_static_context_scope static_holder (true);
  resolution_scope::push_iscope holder (scope, &static_holder);

  model_class *curr = scope->get_current_class ();
  model_constructor *curr_cons
    = assert_cast<model_constructor *> (scope->get_current_method ());

  if (curr->inner_p () && ! curr->static_context_p ())
    {
      // We need to also pass the 'this$0' parameter to the other
      // constructor.
      model_variable_decl *this0 = curr_cons->get_this0_parameter ();

      ref_simple_variable_ref ref
	= new model_simple_variable_ref (get_location (), this0);
      arguments.push_front (ref);
    }

  model_invocation_base::resolve (scope);

  model_constructor *cons = assert_cast<model_constructor *> (method);
  curr_cons->set_other_this (cons, this);
}



template<typename T>
model_generic_invocation<T>::model_generic_invocation (const location &w,
						       const std::list<ref_forwarding_type> &params)
  : T (w),
    actual_type_params (params)
{
}



// Instantiations.

template class model_generic_invocation<model_type_qualified_invocation>;
template class model_generic_invocation<model_super_invocation>;
template class model_generic_invocation<model_this_invocation>;
template class model_generic_invocation<model_new>;
template class model_generic_invocation<model_new_primary>;

void
model_generic_invocation<model_type_qualified_invocation>::visit (visitor *v)
{
  v->visit_generic_invocation (this, method, arguments, super,
                               actual_type_params);
}

void
model_generic_invocation<model_super_invocation>::visit (visitor *v)
{
  v->visit_generic_invocation (this, method, arguments, finit,
                               actual_type_params);
}

void
model_generic_invocation<model_this_invocation>::visit (visitor *v)
{
  v->visit_generic_invocation (this, method, arguments, actual_type_params);
}

void
model_generic_invocation<model_new>::visit (visitor *v)
{
  v->visit_generic_invocation (this, method, klass, arguments,
                               actual_type_params);
}

void
model_generic_invocation<model_new_primary>::visit (visitor *v)
{
  v->visit_generic_invocation (this, method, klass, arguments, simple_name,
                               type_params, actual_type_params);
}
