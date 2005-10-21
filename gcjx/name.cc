// Meaning of a name.

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

model_package *
classify_package_name (model_element *request,
		       const std::list<std::string> &name)
{
  assert (name.begin () != name.end ());

  model_package *base = global->get_compiler ()->get_primordial_package ();
  std::list<std::string> partial;
  for (std::list<std::string>::const_iterator i = name.begin ();
       i != name.end ();
       ++i)
    {
      partial.push_back (*i);
      model_package *p2 = base->find_package (*i);
      model_class *k = base->find_class (*i);

      if (k != NULL)
	{
	  std::string ename = join (partial, '.');
	  // FIXME we need to rethink error strategy!
	  throw duplicate_declaration_error (request->get_location (),
					     "package %1 already "
					     "declared as a class")
	    % ename;
	  // k->error ("original declaration is here");
	}
      else if (p2 == NULL)
	{
	  // Create a new package.
	  ref_package np (new model_package (request->get_location ()));
	  np->set_name (partial);
	  base->add (np);
	  p2 = np.get ();
	}
      base = p2;
    }
  return base;
}

Iname *
classify_package_or_type_name (resolution_scope *scope,
			       model_element *request,
			       const std::list<std::string> &name,
			       model_class *context)
{
  assert (name.begin () != name.end ());

  try
    {
      model_class *k = classify_type_name (scope, request, name, context);
      return k;
    }
  catch (ambiguous_type_error &)
    {
      // This is a legitimate error.
      // FIXME who prints the message?
      throw;
    }
  catch (type_not_found_error &)
    {
      // We don't care, this just means that the name refers to a
      // package.
    }
  assert (! context);
  return classify_package_name (request, name);
}

static model_class *
check_deprecated (model_class *klass, model_element *request)
{
  assert (klass);
  klass->check_deprecated (request);
  return klass;
}

model_class *
classify_type_name (resolution_scope *scope,
		    model_element *request,
		    const std::string &name)
{
  assert (! name.empty ());

  std::set<model_class *> memtypes;
  scope->search_for_type_name (name, memtypes, scope->get_current_context (),
			       NULL);

  if (memtypes.size () > 1)
    {
      // FIXME rethink error reporting!
      throw ambiguous_type_error (request->get_location (),
				  "ambiguous name %1")
	% name;
//       for (std::set<model_class *>::const_iterator i = memtypes.begin ();
// 	   i != memtypes.end ();
// 	   ++i)
// 	(*i)->error ("could resolve to %1") % (*i);
    }
  else if (memtypes.size () == 1)
    return check_deprecated (*(memtypes.begin ()), request);

  throw type_not_found_error (request->get_location (),
			      "type named %1 is undefined")
    % name;
}

model_class *
classify_type_name (resolution_scope *scope,
		    model_element *request,
		    const std::list<std::string> &name,
		    model_class *context)
{
  assert (name.begin () != name.end ());

  if (! context && simple_name_p (name))
    return classify_type_name (scope, request, name.front ());

  // Qualified name.
  Iname *qual;
  if (simple_name_p (name))
    qual = context;
  else
    qual = classify_package_or_type_name (scope, request,
					  drop_last_element (name),
					  context);

  model_class *result = NULL;
  std::string base = get_simple_name (name);
  if (qual->package_p ())
    {
      model_package *pack = assert_cast<model_package *> (qual);
      result = pack->find_class (base);
      IContext *ctxt = context;
      if (ctxt == NULL)
	ctxt = scope->get_compilation_unit ()->get_package ();
      if (result && ! accessible_p (result, ctxt))
	result = NULL;
    }
  else
    {
      std::set<model_class *> memtypes;
      model_class *qualk = assert_cast<model_class *> (qual);
      IContext *ctxt = scope->get_current_class ();
      if (ctxt == NULL)
	ctxt = scope->get_compilation_unit ()->get_package ();
      qualk->find_members (base, memtypes, ctxt);

      if (memtypes.size () == 1)
	result = *(memtypes.begin ());
      else if (memtypes.size () > 1)
	{
	  // FIXME list all ambiguous types.
	  throw ambiguous_type_error (request->get_location (),
				      "ambiguous name %1")
	    % join (name, '.');
	}
    }

  if (result == NULL)
    {
      std::string qualname = join (name, '.');
      throw type_not_found_error (request->get_location (),
				  "type named %1 is undefined")
	% qualname;
    }

  return check_deprecated (result, request);
}

model_field *
classify_field_name (model_element *request, model_class *search,
		     model_class *context, const std::string &name,
		     model_class *qualifier)
{
  if (qualifier == NULL)
    qualifier = search;

  std::set<model_field *> memfields;
  search->find_members (name, memfields, context, qualifier);
  if (memfields.size () > 1)
    throw request->error ("ambiguous reference to field named %1")
      % name;
  else if (memfields.size () == 1)
    return *(memfields.begin ());
  return NULL;
}

ref_expression
create_variable_reference (resolution_scope *scope,
			   model_element *request,
			   model_variable_decl *vdecl)
{
  ref_expression expr;
  if (vdecl->get_declaring_class () != scope->get_current_class ())
    {
      // We have a reference to a variable defined in the enclosing
      // method, so we need to capture the variable and add a
      // reference to it.

      if (! vdecl->final_p ())
	throw request->error ("variable %1 referenced in inner class "
			      "but not %<final%>")
	  % vdecl->get_name ();

      model_class *k = scope->get_current_class ();
      ref_field f = k->add_captured_variable (vdecl);
      model_field_ref *fref
	= new model_field_ref (request->get_location ());
      fref->set_field (f->get_name ());
      fref->set_field (f.get ());
      expr = fref;
    }
  else
    {
      model_simple_variable_ref *vref
	= new model_simple_variable_ref (request->get_location (), vdecl);
      expr = vref;
    }

  return expr;
}

static void
classify_ambiguous_name (resolution_scope *scope,
			 model_element *request,
			 const std::list<std::string> &name,
			 model_package *&package_result,
			 model_class *&class_result,
			 ref_expression &expr_result)
{
  std::list<std::string>::const_iterator iter = name.begin ();
  assert (iter != name.end ());

  model_package *package_int = NULL;
  model_class *class_int = NULL;
  ref_expression expr_int;

  std::list<std::string> partial;
  partial.push_back (*iter);

  // Handle the simple name at the beginning specially.
  std::set<model_variable_decl *> vars;
  scope->search_for_field_name (*iter, vars, scope->get_current_class (),
				NULL);
  if (vars.size () > 1)
    {
      // FIXME: error, ambiguous.
      abort ();
    }
  else if (vars.size () == 1)
    {
      model_variable_decl *var_or_field = *(vars.begin ());

      model_field_base *fbase
	= dynamic_cast<model_field_base *> (var_or_field);
      if (! fbase)
	expr_int = create_variable_reference (scope, request, var_or_field);
      else
	{
	  if (fbase->ambiguous_p ())
	    {
	      // fixme
	      abort ();
	    }

	  model_field_ref *fref
	    = new model_field_ref (request->get_location ());
	  fref->set_field (assert_cast<model_field * > (fbase));
	  expr_int = fref;
	}
    }
  else
    {
      Iname *result = classify_package_or_type_name (scope, request, partial);
      assert (result);
      if (result->package_p ())
	package_int = assert_cast<model_package *> (result);
      else
	class_int = assert_cast<model_class *> (result);
    }

  for (++iter; iter != name.end (); ++iter)
    {
      partial.push_back (*iter);

      assert (package_int || class_int || expr_int);

      if (package_int)
	{
	  // FIXME this API is really bogus, since in all places we
	  // use it we end up dynamic_cast<>ing the result.
	  Iname *what
	    = classify_package_or_type_name (scope, request, partial);
	  assert (what);
	  if (what->package_p ())
	    package_int = assert_cast<model_package *> (what);
	  else
	    {
	      package_int = NULL;
	      class_int = assert_cast<model_class *> (what);
	    }
	  continue;
	}

      model_class *base_class;
      if (class_int)
	base_class = class_int;
      else
	{
	  assert (expr_int);
	  expr_int->resolve (scope);
	  model_type *t = expr_int->type ();

	  if (! t->reference_p ())
	    throw expr_int->error ("qualifier for field reference "
				   "must have reference type");

	  base_class = assert_cast<model_class *> (t);
	}
      model_field *field = classify_field_name (request, base_class,
						scope->get_current_class (),
						*iter);

      // fixme this whole section is duplicated above.
      if (field)
	{
	  if (! field->static_p () && class_int)
	    throw request->error ("can't use type name as qualifier for "
				  "non-static field reference");

	  model_field_ref *fref
	    = new model_field_ref (request->get_location ());
	  fref->set_field (*iter);
	  fref->set_field (field);
	  fref->set_expression (expr_int);
	  expr_int = fref;
	  class_int = NULL;
	}
      else
	{
	  // FIXME this is incorrect since once we've found a field we
	  // can't find a class again...

	  Iname *result = classify_package_or_type_name (scope, request,
							 partial);
	  assert (result);
	  if (result->package_p ())
	    package_int = assert_cast<model_package *> (result);
	  else
	    class_int = assert_cast<model_class *> (result);
	}
    }

  package_result = package_int;
  class_result = class_int;
  expr_result = expr_int;
}

ref_expression
classify_expression_name (resolution_scope *scope,
			  model_element *request,
			  const std::list<std::string> &name)
{
  assert (name.begin () != name.end ());

  model_package *package = NULL;
  model_class *klass = NULL;
  ref_expression expr;
  classify_ambiguous_name (scope, request, name,  package, klass, expr);
  if (package)
    throw semantic_error (request->get_location (),
			  "expected expression name but found "
			  "package name %1")
      % join (name, '.');
  if (klass)
    throw semantic_error (request->get_location (),
			  "expected expression name but found "
			  "class name %1")
      % join (name, '.');
  return expr;
}

ref_invocation_base
classify_expression_name (resolution_scope *scope,
			  model_element *request,
			  const std::list<std::string> &name,
			  const std::list<ref_expression> &args,
			  const std::list<ref_forwarding_type> &type_args)
{
  assert (name.begin () != name.end ());

  ref_invocation_base result;
  if (simple_name_p (name))
    {
      location w = request->get_location ();
      result = (type_args.empty ()
		? new model_method_invocation (w)
		: new model_generic_method_invocation (w, type_args));
      // result->set_qualifying_class (scope->get_current_class ());
    }
  else
    {
      model_package *package = NULL;
      model_class *klass = NULL;
      ref_expression expr;
      std::list<std::string> dropname (drop_last_element (name));
      classify_ambiguous_name (scope, request, dropname, package, klass, expr);
      if (package)
	throw semantic_error (request->get_location (),
			      // fixme this message sucks
			      "expected expression or class name but found "
			      "package name %1")
	  % join (dropname, '.');
      if (klass)
	{
	  location where = request->get_location ();
	  model_type_qualified_invocation *tqi
	    = (type_args.empty ()
	       ? new model_type_qualified_invocation (where)
	       : new model_generic_type_qualified_invocation (where,
							      type_args));
	  tqi->set_class (new model_forwarding_resolved (where, klass));
	  result = tqi;
	}
      else
	{
	  result = (type_args.empty ()
		    ? new model_method_invocation (request->get_location ())
		    : new model_generic_method_invocation (request->get_location (),
							   type_args));
	  result->set_expression (expr);
	}
    }

  result->set_method (get_simple_name (name));
  result->set_arguments (args);
  return result;
}

model_type *
classify_primitive_type_descriptor (const std::string &descriptor)
{
  // This isn't exactly clean, but at least it is simple.
  if (descriptor == primitive_boolean_type->get_descriptor ())
    return primitive_boolean_type;
  if (descriptor == primitive_byte_type->get_descriptor ())
    return primitive_byte_type;
  if (descriptor == primitive_char_type->get_descriptor ())
    return primitive_char_type;
  if (descriptor == primitive_short_type->get_descriptor ())
    return primitive_short_type;
  if (descriptor == primitive_int_type->get_descriptor ())
    return primitive_int_type;
  if (descriptor == primitive_long_type->get_descriptor ())
    return primitive_long_type;
  if (descriptor == primitive_float_type->get_descriptor ())
    return primitive_float_type;
  if (descriptor == primitive_double_type->get_descriptor ())
    return primitive_double_type;
  if (descriptor == primitive_void_type->get_descriptor ())
    return primitive_void_type;
  return NULL;
}

model_type *
classify_type_descriptor (resolution_scope *scope,
			  model_element *request,
			  const std::string &descriptor)
{
  model_unit *unit = scope->get_compilation_unit ();
  model_class *result = unit->find_class_from_descriptor (scope, request,
							  descriptor);
  if (result)
    result->check_deprecated (request);
  return result;
}
