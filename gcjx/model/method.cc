// Methods.

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
#include "defassign.hh"
#include "dump.hh"
#include "unify.hh"

// FIXME this should probably be a method on model_type.
// FIXME duplicated in throwsclause.cc
static ref_forwarding_type
wrap (model_type *t, model_element *request, const model_type_map &type_map)
{
  if (t->reference_p ())
    {
      model_class *k = assert_cast<model_class *> (t);
      t = k->apply_type_map (request, type_map);
    }
  return new model_forwarding_resolved (request->get_location (), t);
}

model_method::model_method (model_method *other,
			    const model_type_map &type_map,
			    model_class *enclosing)
  : model_element (other),
    IDeprecatable (other),
    IAnnotatable (other),
    IModifiable (other),
    IMember (enclosing),
    method_end (other->method_end)
{
  set_name (other->name);

  // Set our descriptor to be the same as the descriptor of the
  // instance from which we were created.  FIXME: this is kind of a
  // hack, we should perhaps instead just directly track our parent
  // and let users get that information directly.  In this case it
  // would be preferable to make a new model_method_instance subclass.
  descriptor = other->get_descriptor ();

  // The method itself might be generic.
  // FIXME can the type bounds be type variables from the class?
  set_type_parameters (other->type_parameters);
  for (std::list<ref_variable_decl>::const_iterator i
	 = other->parameters.begin ();
       i != other->parameters.end ();
       ++i)
    parameters.push_back ((*i)->apply_type_map (type_map, enclosing));

  // Apply the parameterization to the 'throws' clause.
  throw_decls = model_throws_clause (other->throw_decls, enclosing, type_map);

  set_return_type (wrap (other->return_type->type (), enclosing, type_map));
  // FIXME: what do we do with the body?
  body = NULL;

  varargs = other->varargs;
  used = false;
  overrides = other->overrides;
  is_instance_initializer = other->is_instance_initializer;
}

ref_method
model_method::create_abstract_instance ()
{
  assert (get_declaring_class ()
	  == global->get_compiler ()->java_lang_Object ());
  assert ((get_modifiers () & ACC_PUBLIC) != 0);
  assert (! static_p ());
  assert (type_parameters.empty ());

  ref_method result = new model_abstract_method (this);
  result->set_modifiers ((get_modifiers () | ACC_ABSTRACT) & ~ACC_FINAL);
  result->set_name (name);
  result->set_parameters (parameters);
  result->set_throws (throw_decls);
  // Note that we don't want to create a new forwarder here, since
  // that might involve prematurely creating a class instance.
  result->set_return_type (return_type);

  return result;
}

void
model_method::massage_modifiers (const ref_modifier_list &mods)
{
  // Not all modifiers are valid for a method.
  mods->check (this,
	       (ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE
		| ACC_ABSTRACT | ACC_STATIC | ACC_FINAL
		| ACC_SYNCHRONIZED | ACC_NATIVE | ACC_STRICTFP),
	       "invalid modifier %1 for method declaration");
  // Methods have access modifiers, so check validity of these.
  mods->access_check (this, "method declared %1 also declared %2");

  // Invalid to have an abstract method in conjunction with various
  // other modifiers.
  if ((modifiers & ACC_ABSTRACT) != 0)
    mods->check (this,
		 ~ (ACC_PRIVATE | ACC_STATIC | ACC_FINAL
		    | ACC_NATIVE | ACC_STRICTFP
		    | ACC_SYNCHRONIZED),
		 "%<abstract%> method also declared %1");

  // Can't have a native strictfp method.
  if ((modifiers & ACC_NATIVE) != 0 && (modifiers & ACC_STRICTFP) != 0)
    std::cerr << error ("%<native%> method declared %<strictfp%>");

  // Private methods are implicitly final.  However, this doesn't
  // apply to static methods.
  if ((modifiers & ACC_PRIVATE) != 0 && ! static_p ())
    implicit_modifier (ACC_FINAL);
}


bool
model_method::more_specific_p (model_method *other)
{
  // The JLS says to check invocation conversion for the respective
  // declaring classes, but according to Jacks the JLS Clarifications
  // say this is not considered.  And, this makes sense if you think
  // about the inheritance model for methods.  So, there is no code to
  // do that here.

  std::list<ref_variable_decl>::const_iterator this_it
    = parameters.begin ();
  std::list<ref_variable_decl>::const_iterator that_it
    = other->parameters.begin ();

  while (this_it != parameters.end () && that_it != other->parameters.end ())
    {
      if (! method_invocation_conversion ((*that_it)->type (),
					  (*this_it)->type (),
					  PHASE_3))
	return false;
      ++this_it;
      ++that_it;
    }

  assert (this_it == parameters.end () && that_it == other->parameters.end ());

  return true;
}

bool
model_method::potentially_applicable_p (const std::list<model_type *> &args)
{
  int args_size = args.size ();
  int params_size = parameters.size ();
  if ((! varargs && args_size != params_size)
      || (varargs && args_size < params_size - 1))
    return false;
  return true;
}

bool
model_method::potentially_applicable_p (const std::list<model_type *> &args,
					const std::list<ref_forwarding_type> &type_args)
{
  if (type_parameters.size () != type_args.size ())
    return false;
  return potentially_applicable_p (args);
}

model_method *
model_method::do_method_conversion_p (const std::list<model_type *> &args,
				      method_phase phase)
{
  std::list<ref_variable_decl>::const_iterator this_it
    = parameters.begin ();
  std::list<model_type *>::const_iterator args_it = args.begin ();

  while (this_it != parameters.end () && args_it != args.end ())
    {
      model_type *formal_type = (*this_it)->type ();
      model_type *actual_type = *args_it;

      // Increment this here so we can do some special handling of
      // varargs methods.
      ++this_it;

      // We're handling varargs if: we're in phase 3, this is the last
      // argument of a varargs method, and the feature is enabled.
      if (phase == PHASE_3
	  && this_it == parameters.end ()
	  && varargs
	  && global->get_compiler ()->feature_varargs ())
	{
	  // This situation is actually handled below.
	  --this_it;
	  break;
	}

      ++args_it;

      if (! method_invocation_conversion (formal_type, actual_type, phase))
	return NULL;
    }

  // Actually handle varargs.  We do this here because it is valid for
  // there to be no actual arguments corresponding to the final formal
  // parameter.
  if (phase == PHASE_3
      && this_it != parameters.end ()
      && ++this_it == parameters.end ()
      && varargs
      && global->get_compiler ()->feature_varargs ())
    {
      --this_it;
      model_type *formal_type = (*this_it)->type ();

      // Handle varargs.
      assert (formal_type->array_p ());
      formal_type = formal_type->element_type ();

      while (args_it != args.end ())
	{
	  model_type *actual_type = *args_it;
	  if (! method_invocation_conversion (formal_type, actual_type,
					      phase))
	    return NULL;
	  ++args_it;
	}

      return this;
    }

  // Wrong number of arguments.
  if (this_it != parameters.end () || args_it != args.end ())
    return NULL;

  return this;
}

model_method *
model_method::do_method_conversion_p (const model_type_map &typeargs,
				      const std::list<model_type *> &args,
				      method_phase phase)
{
  model_method *new_meth = apply_type_map (typeargs, get_declaring_class ());
  return new_meth->do_method_conversion_p (args, phase);
}

model_method *
model_method::method_conversion_p (const std::list<model_type *> &args,
				   method_phase phase)
{
  if (! type_parameters.empty ())
    {
      model_type_map typeargs;
      // FIXME: return result..?  error detection?
      // FIXME: pass in argument for varargs handling.
      unify (args, this, NULL /* FIXME */, typeargs);
      return do_method_conversion_p (typeargs, args, phase);
    }
  return do_method_conversion_p (args, phase);
}

model_method *
model_method::method_conversion_p (const std::list<model_class *> &typeargs,
				   const std::list<model_type *> &args,
				   method_phase phase)
{
  model_type_map the_map;
  // FIXME: error handling?
  type_parameters.create_type_map (the_map, this /* FIXME */, typeargs);
  return do_method_conversion_p (the_map, args, phase);
}

void
model_method::method_conversion (std::list<ref_expression> &args)
{
  std::list<ref_variable_decl>::const_iterator this_it
    = parameters.begin ();
  std::list<ref_expression>::iterator args_it = args.begin ();

  bool can_do_varargs = varargs;
  while (this_it != parameters.end () && args_it != args.end ())
    {
      model_type *formal = (*this_it)->type ();
      ref_expression &expr (*args_it);

      ++this_it;
      ++args_it;

      // Special handling for varargs: we wrap the final arguments in
      // a new array expression.  Note that if the final actual
      // argument is compatible with the vararg parameter, then we
      // treat it as an ordinary parameter and do not wrap it in
      // another array.  The logic here differs from
      // method_conversion_p because we do not have (or need) phase
      // information.
      if (this_it == parameters.end ()
	  && varargs
	  && global->get_compiler ()->feature_varargs ())
	{
	  if (args_it == args.end ()
	      && method_invocation_conversion (formal, expr->type (), PHASE_3))
	    {
	      // Don't do any special varargs processing when we leave
	      // the loop.
	      can_do_varargs = false;
	    }
	  else
	    {
	      // Go to the varargs part, but with the argument pointer
	      // set correctly.
	      --this_it;
	      --args_it;
	      break;
	    }
	}

      method_invocation_conversion (formal, expr);
    }

  // Handle varargs.  The condition is written strangely as we must
  // handle the case where there are no actual arguments corresponding
  // to the final varargs parameter.
  if (can_do_varargs
      && this_it != parameters.end ()
      && ++this_it == parameters.end ()
      && global->get_compiler ()->feature_varargs ())
    {
      --this_it;
      model_type *formal = (*this_it)->type ()->element_type ();

      ref_forwarding_type elt_type
	= new model_forwarding_resolved (LOCATION_UNKNOWN, formal);

      // FIXME: should have ref_ class for this.
      // FIXME: location is wrong.
      std::list<ref_expression> inits (args_it, args.end ());
      for (std::list<ref_expression>::iterator i = inits.begin ();
	   i != inits.end ();
	   ++i)
	method_invocation_conversion (formal, *i);
      model_array_initializer *init
	= new model_array_initializer (LOCATION_UNKNOWN, elt_type, inits);
      init->already_resolved ();

      // FIXME: location should come from arguments.
      ref_new_array var_param = new model_new_array (LOCATION_UNKNOWN);
      var_param->set_element_type (elt_type);
      var_param->set_initializer (init);
      var_param->already_resolved ();

      args.erase (args_it, args.end ());
      args.push_back (var_param);
    }
}

bool
model_method::same_arguments_p (model_method *other) const
{
  // Map our type variables to OTHER's.
  model_type_map type_map;
  // True if we might have the same descriptor as OTHER.
  // Skipping this step if there are no type parameters is a little
  // more efficient.
  bool maybe_same = ! type_parameters.empty ();
  if (maybe_same)
    maybe_same = type_parameters.create_type_map (type_map,
						  other->type_parameters);

  std::list<ref_variable_decl>::const_iterator this_it
    = parameters.begin ();
  std::list<ref_variable_decl>::const_iterator other_it
    = other->parameters.begin ();

  while (this_it != parameters.end () && other_it != other->parameters.end ())
    {
      model_type *this_type = (*this_it)->type ();
      model_type *other_type = (*other_it)->type ();

      if (maybe_same
	  && this_type->reference_p ()
	  && other_type->reference_p ())
	{
	  // We might still be checking to see if we have the same
	  // argument types.  We don't want to do this check
	  // spuriously since it requires applying a type map, which
	  // we might not have been able to construct.
	  model_class *this_class = assert_cast<model_class *> (this_type);
	  model_class *other_class = assert_cast<model_class *> (other_type);

	  // FIXME: using the wrong kinds of cast here.
	  this_class = this_class->apply_type_map ((model_element *) this,
						   type_map);
	  other_class = other_class->apply_type_map ((model_element *) this,
						     type_map);
	}

      if (this_type->erasure () != other_type->erasure ())
	return false;
      ++this_it;
      ++other_it;
    }

  // We might have the wrong number of arguments.
  if (this_it != parameters.end () || other_it != other->parameters.end ())
    return false;

  return true;
}

bool
model_method::return_type_substitutable_p (model_type *base,
					   model_type *derived) const
{
  if (base == primitive_void_type || base->primitive_p ())
    return base == derived;
  // FIXME: if methods have same signature, see if base==derived
  // or if we can use unchecked conversion.  Otherwise fall back to
  // what we have now.
  return widening_reference_conversion (base, derived);
  // return base->erasure ()->assignable_from_p (derived->erasure ());
}

bool
model_method::hides_or_overrides_p (model_method *other, model_class *asker)
{
  if (! same_arguments_p (other))
    return false;

  if (static_p () && ! other->static_p ())
    throw error ("static method hides non-static method %1")
      % other;
  if (! static_p () && other->static_p ())
    throw error ("non-static method overrides static method %2")
      % other;

  if (global->get_compiler ()->feature_generics ())
    {
      if (! return_type_substitutable_p (other->return_type->type (),
					 return_type->type ()))
	throw error ("method's return type is not convertible"
		     " from the return type of %1")
	  % other;
    }
  else
    {
      // FIXME: we have a jacks failure here related to covariant
      // methods from class files.
      if (return_type->type () != other->return_type->type ())
	// FIXME this message sucks -- look in JLS.
	throw error ("method has different return type from %1")
	  % other;
    }

  // If we are inheriting two abstract methods, then we needn't check
  // the 'throws' clauses or the modifiers and, in fact, we know we
  // will inherit both of them.  So, we return 'false' as one does not
  // override the other.
  assert (asker != other->declaring_class);
  if (asker != declaring_class && abstract_p () && other->abstract_p ())
    return false;

  // Check the modifiers.
  modifier_t this_mods = modifiers & ACC_ACCESS;
  modifier_t other_mods = other->get_modifiers () & ACC_ACCESS;
  // We never inherit private methods.
  assert (other_mods != ACC_PRIVATE);
  if (this_mods == ACC_PRIVATE
      || (other_mods == ACC_PROTECTED && this_mods == 0)
      || (other_mods == ACC_PUBLIC && (this_mods == 0
				       || this_mods == ACC_PROTECTED)))
    throw error ("method overriding %1 must have equal or greater access")
      % other;

  // 'throws' clauses must be compatible.
  if (! throw_decls.subset_p (other->throw_decls))
    // FIXME it would be nice to list the precise exceptions...
    throw error ("method overriding %1 must have compatible %<throws%> clause")
      % other;

  if ((other->get_modifiers () & ACC_FINAL) != 0)
    throw error ("cannot override %<final%> method %1")
      % other;

  if (! from_class_p ())
    other->check_deprecated (this);

  if (! static_p () && ! other->declaring_class->interface_p ())
    overrides = true;

  return true;
}

std::string
model_method::get_descriptor ()
{
  if (descriptor.empty ())
    {
      descriptor = "(";
      for (std::list<ref_variable_decl>::const_iterator i
	     = parameters.begin ();
	   i != parameters.end ();
	   ++i)
	descriptor += (*i)->type ()->erasure ()->get_descriptor ();
      descriptor += ")" + return_type->type ()->erasure ()->get_descriptor ();
    }
  return descriptor;
}

std::string
model_method::get_signature ()
{
  std::string result = type_parameters.get_signature () + "(";
  for (std::list<ref_variable_decl>::const_iterator i
	 = parameters.begin ();
       i != parameters.end ();
       ++i)
    result += (*i)->get_signature ();
  result = (result + ")" + return_type->type ()->get_signature ()
	    + throw_decls.get_signature ());
  return result;
}

void
model_method::note_throw_type (model_type *exc_type)
{
  if (throw_decls.handled_p (exc_type))
    {
      // The exception is declared, so we're ok.
      return;
    }

  // If we're in an anonymous class constructor or instance
  // initializer, then we simply add the checked exception to the list
  // of such exceptions for this method.
  // FIXME: there is an ordering problem here; must propagate
  // exceptions from finit$ to <init>.
  if (declaring_class->anonymous_p ()
      && (constructor_p () || instance_initializer_p ()))
    {
      // FIXME: we may end up with a given type listed here several
      // times.  In this case we should unique-ify the list as we
      // construct it.
      ref_forwarding_type e
	= new model_forwarding_resolved (LOCATION_UNKNOWN, exc_type);
      throw_decls.add (e);
    }
  else
    {
      // An instance initializer in a non-anonymous class can throw an
      // exception if all its constructors declare it.
      if (instance_initializer_p ())
	{
	  bool ok = true;
	  std::list<ref_method> methods = declaring_class->get_methods ();
	  for (std::list<ref_method>::const_iterator i = methods.begin ();
	       i != methods.end ();
	       ++i)
	    {
	      if (! (*i)->constructor_p ())
		continue;
	      if (! (*i)->throw_decls.handled_p (exc_type))
		{
		  ok = false;
		  break;
		}
	    }

	  if (ok)
	    {
	      ref_forwarding_type e
		= new model_forwarding_resolved (LOCATION_UNKNOWN, exc_type);
	      throw_decls.add (e);
	      return;
	    }
	}

      // FIXME location of thrown object... requires minor API change
      // here.  We could also consider using this in the synthetic
      // throws clause above...?
      std::cerr << error ("checked exception of type %1 must be caught "
			  "or declared in %<throws%>")
	% exc_type;
    }
}

void
model_method::resolve (resolution_scope *scope)
{
  resolve_annotations (scope);

  resolution_scope::push_warnings warn_holder (scope, this);

  // Push the type parameters.
  resolution_scope::push_iscope type_holder (scope, &type_parameters);

  // Push the method itself on the scope.
  resolution_scope::push_iscope this_holder (scope, this);

  // Create and push an ordinary block.
  // FIXME: we could handle this ourselves without an extra object
  // and without the possibility of an incorrect add_binding().
  model_block_scope method_block (scope);
  resolution_scope::push_iscope param_holder (scope, &method_block);

  for (std::list<ref_variable_decl>::const_iterator i = parameters.begin ();
       i != parameters.end ();
       ++i)
    {
      scope->add_binding ((*i).get ());
      // Mark parameters of native or abstract methods as used.
      if ((modifiers & (ACC_NATIVE | ACC_ABSTRACT)) != 0)
	(*i)->set_used ();
    }

  if ((modifiers & ACC_NATIVE))
    {
      if (body)
	{
	  std::cerr << error ("%<native%> method can't have body");
	  body = NULL;
	}
    }
  else if ((modifiers & ACC_ABSTRACT))
    {
      if (body)
	{
	  std::cerr << error ("%<abstract%> method can't have body");
	  body = NULL;
	}
    }
  else if (! body)
    std::cerr << error ("non-%<native%> and non-%<abstract%> method "
			"must have body");
  else  
    {
      body->resolve (scope);

      normal_completion_state ncs;
      body->compute_normal_completion (ncs);

      // This is a valid implementation because if we see a break or
      // continue we'll generate an error earlier, and otherwise only
      // throw and return will be trapped.
      if (body->can_complete_normally ()
	  && return_type->type () != primitive_void_type)
	std::cerr << error ("method body must end with %<return%> or %<throw%>");
    }

  if (name == declaring_class->get_name ()
      && scope->warn_constructor_name ())
    std::cerr << warn (global->get_compiler ()->warn_constructor_name (),
		       "method %1 has constructor's name")
      % this;
}

void
model_method::check_definite_assignment ()
{
  // Just a convenient place to dump... all modifications to this
  // method have been completed.
  if (global->get_compiler ()->dump_methods ())
    dump_method (this);

  if (body)
    ::check_definite_assignment (this);
}

void
model_method::resolve_classes (resolution_scope *scope)
{
  // Resolve annotations here since they seem like part of the
  // envelope.
  resolve_annotation_classes (scope);

  resolution_scope::push_warnings warn_holder (scope, this);

  // Put the type parameters in scope, then resolve them.
  resolution_scope::push_iscope tp_pusher (scope, &type_parameters);
  type_parameters.resolve_classes (scope);

  return_type->resolve (scope);
  ::resolve (scope, parameters);
  throw_decls.resolve (scope);

  if (return_type->type () == primitive_void_type
      && parameters.empty ()
      // FIXME: type parameters must be empty???
      && type_parameters.empty ()
      // Some folks accidentally use the UK spelling.
      && name == "finalise"
      && scope->warn_finalize_spelling ())
    std::cerr << warn (global->get_compiler ()->warn_finalize_spelling (),
		       "did you mean to override %<finalize%>?");
}

void
model_method::visit (visitor *v)
{
  // FIXME: is this enough?
  v->visit_method (this, parameters, body);
}

void
model_method::check_deprecated (const model_element *request)
{
  // FIXME: also need to check we're not in a deprecated context.

  if (deprecated && global->get_compiler ()->warn_deprecated ())
    {
      if (constructor_p ())
	std::cerr << request->warn (global->get_compiler ()->warn_deprecated (),
				    "constructor %1 is deprecated")
	  % this;
      else
	std::cerr << request->warn (global->get_compiler ()->warn_deprecated (),
				    "method %1 is deprecated")
	  % this;
    }
}

void
model_method::check_referenced (resolution_scope *scope)
{
  warning_scope::push_warnings push (scope, this);
  // We only emit this warning for private methods, as others can be
  // referenced from elsewhere.
  if ((modifiers & ACC_PRIVATE) != 0
      && ! used
      && scope->warn_unused_private_members ()
      // Special serialization methods.
      && name != "readObject"
      && name != "writeObject"
      // <clinit> will appear to be unused to the compiler.
      && ! static_initializer_p ())
    std::cerr << warn (global->get_compiler ()->warn_unused_private_members (),
		       "private method %1 not used in its scope")
      % this;
}

void
model_method::check_override ()
{
  // This is a little tricky: if there are no annotations, skip the
  // rest so that we don't try to read annotation-related classes if
  // we're compiling against a pre-1.5 library.
  if (! has_annotations_p ())
    return;

  if (find_annotation (global->get_compiler ()->java_lang_Override ()))
    {
      // Check @Override.
      if (static_p ())
	std::cerr << error ("%<@Override%> applied to static method");
      else if (declaring_class->interface_p ())
	std::cerr << error ("%<@Override%> applied to interface method");
      else if (! overrides)
	std::cerr << error ("method marked %<@Override%> does not override "
			    "another method");
    }
}

std::string
model_method::get_pretty_name ()
{
  // This method is only used for error reporting, so for the time
  // being a relatively inefficient implementation is acceptable.
  std::string result = (declaring_class->get_pretty_name ()
			+ "." + name
			+ " (");
  bool first = true;
  for (std::list<ref_variable_decl>::const_iterator i = parameters.begin ();
       i != parameters.end ();
       ++i)
    {
      if (! first)
	result += ", ";
      result += (*i)->type ()->get_pretty_name ();
      first = false;
    }
  result += ")";
  return result;
}

void
model_method::clean_up ()
{
  body = NULL;
}

model_method *
model_method::apply_type_map (const model_type_map &type_map,
			      model_class *enclosing)
{
  // FIXME: if argument and return types don't change, perhaps we
  // should just return 'this'?

  model_method *cache = instance_cache.find_instance (type_map);
  if (cache != NULL)
    return cache;

  model_method *result = new model_method (this, type_map, enclosing);
  instance_cache.add_instance (type_map, result);
  return result;
}



void
model_abstract_method::visit (visitor *v)
{
  v->visit_abstract_method (this, parameters, body, original);
}



const format &
operator% (const format &fmt, model_method *meth)
{
  return fmt % meth->get_pretty_name ();
}
