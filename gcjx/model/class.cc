// Class representation.

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
#include <iterator>
#include <sstream>



/// This is a scope and a catcher that understands how to forward a
/// 'throws' notification to a particular method.  This is used to
/// implement the special rules for handling field initialization
/// expressions that can throw exceptions.
class field_init_forwarding_catcher : public IScope, public ICatcher
{
  // The method to which the exception is forwarded.
  model_method *method;

public:

  field_init_forwarding_catcher (model_method *m)
    : method (m)
  {
  }

  void note_throw_type (model_type *t)
  {
    method->note_throw_type (t);
  }
};

/// This is a scope and a catcher used to handle 'throws'
/// notifications from a static field initializer.  While we could
/// just do this via the <clinit> method, ensuring that this was
/// created at the right time looked like a pain, and this approach
/// gives a nicer error message.
class static_field_catcher : public IScope, public ICatcher
{
  /// The field we are handling.
  model_field *field;

public:

  static_field_catcher (model_field *f)
    : field (f)
  {
  }

  void note_throw_type (model_type *t)
  {
    if (t->checked_exception_p ())
      std::cerr << field->error ("initializer for %1 "
				 "throws a checked exception of type %2")
	% field % t;
  }
};



void
model_class::compute_super_types (std::list<model_class *> &all_super_types)
{
  // 8.4.6 says that a class inherits methods from its direct
  // superclass and direct superinterfaces.  So, eg, I think a
  // package-private method in p1.A is not inherited in p1.C if the
  // intermediate class is p2.B.

  // Note that we push the superclass on the list first, on purpose.
  // This lets us handle method inheritance properly, in particular
  // the restriction that a concrete method implement all inherited
  // abstract methods.
  if (superclass)
    {
      model_class *super = assert_cast<model_class *> (superclass->type ());
      super->resolve_classes ();
      all_super_types.push_back (super);
    }
  if (interface && interfaces.empty ())
    {
      model_class *obj = global->get_compiler ()->java_lang_Object ();
      // Object might not yet have been resolved.
      obj->resolve_classes ();
      all_super_types.push_back (obj);
    }
  else
    {
      for (std::list<ref_forwarding_type>::const_iterator i
	     = interfaces.begin ();
	   i != interfaces.end ();
	   ++i)
	{
	  model_class *inter = assert_cast<model_class *> ((*i)->type ());
	  inter->resolve_classes ();
	  all_super_types.push_back (inter);
	}
    }
}

// This resolves one method up to the level implied by the class'
// state.  This is used when adding a method after the class has been
// resolved.
void
model_class::resolve_one_method (const ref_method &method)
{
  if (resolution_state < POST_MEMBERS)
    return;

  all_methods.insert (std::make_pair (method->get_name (), method.get ()));
  source_order_methods.push_back (method.get ());

  resolution_scope scope;
  push_on_scope (&scope);

  method->resolve_classes (&scope);
  if (resolution_state == DONE)
    method->resolve (&scope);
}

void
model_class::implicit_modifier (modifier_t mod)
{
  IModifiable::implicit_modifier (mod);
  if (mod == ACC_STRICTFP)
    {
      if (! interface)
	{
	  for (std::list<ref_method>::const_iterator i = methods.begin ();
	       i != methods.end ();
	       ++i)
	    // FIXME: constructors and initializers?
	    (*i)->implicit_modifier (mod);
	}

      for (std::map<std::string, ref_class>::const_iterator i
	     = member_classes.begin ();
	   i != member_classes.end ();
	   ++i)
	(*i).second->implicit_modifier (mod);
    }
}

void
model_class::add (const ref_method &m)
{
  methods.push_back (m);
  if (interface)
    {
      if (! m->static_initializer_p ())
	{
	  // Members of interfaces are public.  Methods are abstract.
	  m->implicitly_public ();
	  m->implicit_modifier (ACC_ABSTRACT);
	}

      // FIXME: modifier or IModifiable should have a method to make
      // this simpler -- must refactor existing ones..?
      if (m->final_p () && ! m->static_initializer_p ())
	std::cerr << m->error ("interface method may not be %<final%>");
      if ((m->get_modifiers () & ACC_STRICTFP) != 0)
	std::cerr << m->error ("interface method may not be %<strictfp%>");
      if ((m->get_modifiers () & ACC_NATIVE) != 0)
	std::cerr << m->error ("interface method may not be %<native%>");
      if ((m->get_modifiers () & ACC_SYNCHRONIZED) != 0)
	std::cerr << m->error ("interface method may not be %<synchronized%>");
    }
  else
    {
      // Not interface.

      // In a strictfp class, methods are implicitly strictfp.
      // FIXME: constructors and initializers?
      if ((modifiers & ACC_STRICTFP) != 0)
	m->implicit_modifier (ACC_STRICTFP);
    }

  // Methods of a final class are implicitly final.  However, this
  // does not apply to static methods or constructors.
  if (final_p ()
      && ! m->static_p ()
      && ! m->constructor_p ())
    m->implicit_modifier (ACC_FINAL);

  // Note: we don't do this check for inner classes here, since a
  // class might be implicitly static and we only discover that later.
  // So, the corresponding inner class check is done when resolving
  // members.
  if (interface && m->static_p () && ! m->static_initializer_p ())
    std::cerr << m->error ("static method invalid in interface");
}

void
model_class::add (std::list<ref_stmt> &statements, const ref_field &f)
{
  // Create an initializer statement and add it to the list of
  // statements.  Later we put this into a special initialization
  // method.
  ref_stmt init = new model_field_initializer (f->get_location (), f.get ());
  statements.push_back (init);
}

bool
model_class::check_init_list (const std::list<ref_stmt> &statements)
{
  for (std::list<ref_stmt>::const_iterator i = statements.begin ();
       i != statements.end ();
       ++i)
    {
      model_stmt *stmt = (*i).get ();
      if (dynamic_cast<model_field_initializer *> (stmt) != NULL)
	{
	  model_field_initializer *fld_init
	    = assert_cast<model_field_initializer *> (stmt);
	  model_field *field = fld_init->get_field ();
	  if (field->static_p () && field->constant_p ())
	    {
	      // Nothing.
	    }
	  else if (field->has_initializer_p ())
	    return true;
	}
      else
	{
	  // We could optimize away empty statements and blocks here.
	  return true;
	}
    }
  return false;
}

void
model_class::add (const ref_field &f)
{
  if (interface)
    {
      // Members of interfaces are public, and fields are also
      // static and final.
      f->implicitly_public ();
      f->implicit_modifier (ACC_STATIC);
      f->implicit_modifier (ACC_FINAL);

      // FIXME: see add(ref_method) -- we need a helper method.
      if ((f->get_modifiers () & ACC_TRANSIENT) != 0)
	std::cerr << f->error ("interface field may not be %<transient%>");
      if ((f->get_modifiers () & ACC_VOLATILE) != 0)
	std::cerr << f->error ("interface field may not be %<volatile%>");

      if (! f->has_initializer_p () && ! f->from_class_p ())
	std::cerr << f->error ("interface field must have an initializer");
    }
  fields.push_back (f);

  // See model_field_initializer to understand why we do this
  // unconditionally.
  add (f->static_p () ? static_inits : instance_inits, f);
}

void
model_class::add (const owner<model_class> &k)
{
  if (k->anonymous_p () || k->local_p ())
    anon_classes.push_back (k);
  else
    {
      std::map< std::string, owner<model_class> >::const_iterator i
	= member_classes.find (k->get_name ());
      if (i != member_classes.end ())
	{
	  // FIXME location of other class.
	  std::cerr << k->error ("type named %1 already defined in %2")
	    % k->get_name () % this;
	}
      else
	member_classes.insert (std::make_pair (k->get_name (), k));
    }
  if (interface)
    {
      // Members of interfaces are public.
      k->implicitly_public ();
    }
  if ((modifiers & ACC_STRICTFP) != 0)
    k->implicit_modifier (ACC_STRICTFP);
  if (interface || k->interface_p ())
    {
      // Member types of interfaces are always static, and interface
      // members are also implicitly static.
      k->implicit_modifier (ACC_STATIC);
    }
}

void
model_class::add_static_initializer (const ref_block &b)
{
  if (interface)
    std::cerr << b->error ("static initializer invalid in interface %1")
      % this;
  else if (inner_p ())
    std::cerr << b->error ("static initializer invalid in inner class %1")
      % this;
  else
    static_inits.push_back (b);
}

void
model_class::add_instance_initializer (const ref_block &b)
{
  if (interface)
    std::cerr << b->error ("instance initializer invalid in interface %1")
      % this;
  else
    instance_inits.push_back (b);
}

std::string
model_class::generate_name ()
{
  // Sigh.
  char buffer[20];
  sprintf (buffer, "%d", counter);
  ++counter;
  return std::string (buffer);
}

void
model_class::massage_modifiers (const ref_modifier_list &mods)
{
  // Not all modifiers are valid for a class.
  if (! declaring_class)
    {
      // Top-level class or interface.
      if (interface)
	mods->check (this, ACC_PUBLIC | ACC_ABSTRACT | ACC_STRICTFP,
		     "invalid modifier %1 for top-level "
		     "interface declaration");
      else
	mods->check (this, (ACC_PUBLIC | ACC_ABSTRACT
			    | ACC_FINAL | ACC_STRICTFP),
		     "invalid modifier %1 for top-level class declaration");
    }
  else if (local)
    {
      // Local class or interface.
      if (interface)
	mods->check (this, (ACC_PUBLIC | ACC_ABSTRACT | ACC_STATIC
			    | ACC_STRICTFP),
		   "invalid modifier %1 for local interface declaration");
      else
	mods->check (this, (ACC_PUBLIC | ACC_ABSTRACT | ACC_STATIC | ACC_FINAL
			    | ACC_STRICTFP),
		     "invalid modifier %1 for local class declaration");
    }
  else if (! anonymous)
    {
      // Member class or interface.
      if (interface)
	mods->check (this, (ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE
			    | ACC_ABSTRACT | ACC_STATIC | ACC_STRICTFP),
		     "invalid modifier %1 for member interface declaration");
      else
	mods->check (this, (ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE
			    | ACC_ABSTRACT | ACC_STATIC | ACC_FINAL
			    | ACC_STRICTFP),
		     "invalid modifier %1 for member class declaration");
    }
  else
    {
      // Anonymous class.
      mods->check (this, ACC_FINAL | ACC_STRICTFP,
		   "invalid modifier %1 for anonymous class declaration");
    }

  if (interface)
    {
      implicit_modifier (ACC_ABSTRACT);
      modifiers |= ACC_INTERFACE;
    }

  // Classes have access modifiers, so check validity of these.
  mods->access_check (this, "class declared %1 also declared %2");

  if (final_p () && (modifiers & ACC_ABSTRACT) != 0)
    std::cerr << error ("%<final%> class %1 also declared %<abstract%>")
      % this;
}

void
model_class::compute_descriptor ()
{
  descriptor = "L" + get_fully_qualified_name_with_slashes () + ";";
}

std::string
model_class::get_assigned_name () const
{
  if (assigned_name.empty ())
    {
      if (local || anonymous)
	assigned_name = get_top_level_class ()->generate_name ();
      else
	assigned_name = name;
    }
  return assigned_name;
}

std::string
model_class::get_output_name () const
{
  if (declaring_class)
    return declaring_class->get_name () + "$" + get_assigned_name ();
  return name;
}

std::string
model_class::get_signature_fragment ()
{
  std::string result;
  if (declaring_class)
    result = declaring_class->get_signature_fragment () + ".";
  result += get_assigned_name () + get_signature_map_fragment ();
  return result;
}

std::string
model_class::get_signature ()
{
  std::string result = get_package ()->get_signature_fragment ();
  if (! result.empty ())
    result += '/';
  return "L" + result + get_signature_fragment () + ";";
}

std::string
model_class::get_class_signature ()
{
  std::string result;
  result = type_parameters.get_signature ();
  model_type *sup = NULL;
  if (interface)
    sup = global->get_compiler ()->java_lang_Object ();
  else if (superclass)
    sup = superclass->type ();
  if (sup)
    result += sup->get_signature ();
  for (std::list<ref_forwarding_type>::const_iterator i = interfaces.begin ();
       i != interfaces.end ();
       ++i)
    result += (*i)->type ()->get_signature ();
  return result;
}

std::string
model_class::get_fully_qualified_name ()
{
  std::string result;
  if (declaring_class)
    result = (declaring_class->get_fully_qualified_name ()
	      + "$"
	      + get_assigned_name ());
  else
    {
      result = compilation_unit->get_package ()->get_fully_qualified_name ();
      if (! result.empty ())
	result += ".";
      result += name;
    }
  return result;
}

std::string
model_class::get_fully_qualified_name_with_slashes ()
{
  std::string name (get_fully_qualified_name ());
  replace_all (name, '.', '/');
  return name;
}

bool
model_class::assignable_from_p (model_type *other)
{
  resolve_classes ();
  if (other->primitive_p ())
    return false;
  return widening_reference_conversion (this, other);
}

model_package *
model_class::get_package () const
{
  return compilation_unit->get_package ();
}

Iname *
model_class::find_name (const std::string &n)
{
  std::map< std::string, owner<model_class> >::const_iterator i
    = member_classes.find (n);
  return i == member_classes.end () ? NULL : (*i).second.get ();
}

bool
model_class::has_as_member_of_p (model_class *member)
{
  while (member && member != this)
    member = member->get_lexically_enclosing_class ();
  return member == this;
}

void
model_class::add_method (const ref_method &method)
{
  methods.push_back (method);
  all_methods.insert (std::make_pair (method->get_name (), method));
  source_order_methods.push_back (method.get ());
}

void
model_class::add_anonymous_constructor (resolution_scope *scope,
					const ref_expression &qualifier,
					const std::list<ref_expression> &exprs)
{
  assert (anonymous);

  // FIXME: add assertion to make sure this isn't called twice.

  ref_method init = new model_constructor (get_location (), this);
  init->set_name ("<init>");
  init->set_modifiers (0);
  init->set_synthetic ();

  // Create formal parameters for the method.  The type of each
  // parameter is the type of the corresponding expression.  We also
  // construct the actual arguments to the 'super' call here.
  std::list<ref_variable_decl> formal_parameters;
  std::list<ref_expression> actual_parameters;
  int arg_no = 0;
  for (std::list<ref_expression>::const_iterator i = exprs.begin ();
       i != exprs.end ();
       ++i)
    {
      model_type *arg_type = (*i)->type ();
      ref_forwarding_type type_wrap
	= new model_forwarding_resolved (get_location (), arg_type);
      // Do we even need to give these parameters names?  Perhaps it
      // will make debugging simpler in some obscure case.
      std::string name = std::string ("arg$") + arg_no;
      ++arg_no;
      ref_variable_decl param = new model_parameter_decl (get_location (),
							  name, type_wrap,
							  this);
      formal_parameters.push_back (param);

      ref_simple_variable_ref actual_arg
	= new model_simple_variable_ref (get_location ());
      actual_arg->set_name (name);
      actual_arg->set_variable (param.get ());
      actual_parameters.push_back (actual_arg);
    }
  init->set_parameters (formal_parameters);

  // Construct the 'super(...)' call and make it the method body.
  std::list<ref_stmt> block_stmts;
  ref_super_invocation super_call
    = new model_super_invocation (get_location ());
  super_call->set_enclosing_class (this);
  super_call->set_arguments (actual_parameters);
  if (qualifier)
    super_call->set_expression (qualifier);
  super_call->set_synthetic ();
  block_stmts.push_back (new model_expression_stmt (get_location (),
						    super_call));
  ref_block body = new model_block (get_location (), block_stmts);
  init->set_body (body);

  init->set_return_type (new model_forwarding_resolved (get_location (),
							primitive_void_type));

  // Resolve classes on the method, add the needed capturing
  // arguments, and then resolve the method itself.
  resolution_scope::push_iscope holder (scope, this);
  resolution_scope::push_warnings warn_holder (scope, this);
  init->resolve_classes (scope);

  add_captured_constructor_arguments (scope, init, false);
  init->resolve (scope);
  init->check_definite_assignment ();

  add_method (init);
}

ref_field
model_class::get_this_0 ()
{
  if (! this_0)
    {
      std::string name = generate_synthetic_field_name ("this$0");
      this_0
	= new model_field (get_location (), name,
			   new model_forwarding_resolved (get_location (),
							  declaring_class),
			   this);
      // Note that it might be possible for us to inherit this$0
      // from our superclass.  We could do this if we made it
      // package-private, and if our superclass was an inner class
      // of our declaring class.  FIXME?
      this_0->set_modifiers (ACC_PRIVATE | ACC_FINAL);
      this_0->set_synthetic ();

      assert (resolution_state >= POST_MEMBERS);
      resolution_scope scope;
      push_on_scope (&scope);
      this_0->resolve (&scope);

      ambiguous_field_map[name] = this_0.get ();
      field_map.insert (std::make_pair (name, this_0.get ()));
      fields.push_back (this_0);
    }
  return this_0;
}

ref_field
model_class::add_captured_variable (model_variable_decl *var)
{
  assert (local || anonymous);

  std::map<model_variable_decl *, ref_field>::const_iterator it
    = synthetic_fields.find (var);
  if (it != synthetic_fields.end ())
    return (*it).second;

  // FIXME: perhaps use location of the variable reference, if we can
  // find it?
  std::string name = generate_synthetic_field_name ("copy$"
						    + var->get_name ());
  ref_field field = new model_field (get_location (),
				     name,
				     var->get_declared_type (),
				     this);
  // It is final since that makes the most sense.  It is also
  // package-private since that makes it somewhat simpler for nested
  // classes to access it.
  field->set_modifiers (ACC_FINAL);
  field->set_synthetic ();

  // Resolve the field now, because our class might already have been
  // resolved.
  {
    resolution_scope scope;
    push_on_scope (&scope);
    field->resolve (&scope);
  }

  synthetic_fields[var] = field;
  // Push on the front so we walk in reverse order later.
  captured_variables.push_front (var);

  // Ensure users can find out about the field.
  fields.push_back (field);
  field_map.insert (std::make_pair (name, field.get ()));

  return field;
}

void
model_class::add_capturing_arguments (resolution_scope *scope,
				      std::list<ref_expression> &arguments)
{
  assert (local || anonymous);

  for (std::list<model_variable_decl *>::const_iterator i
	 = captured_variables.begin ();
       i != captured_variables.end ();
       ++i)
    {
      // FIXME: suppose we have a local class X that captures a
      // variable, then a local subclass Y that doesn't explicitly
      // capture it?  We must still pass in the variables via the
      // subclass constructor.  And we probably shouldn't pass in a
      // given variable more than once.

      // FIXME: what requestor makes sense?
      ref_expression expr = create_variable_reference (scope, this, *i);
      // We have to resolve here because the arguments were already
      // resolved in the caller.
      expr->resolve (scope);
      arguments.push_front (expr);
    }
}

void
model_class::add_captured_constructor_arguments (resolution_scope *scope,
						 const ref_method &constructor,
						 bool do_resolve)
{
  std::list<ref_stmt> statements;

  if (captured_variables.empty ())
    return;

  for (std::list<model_variable_decl *>::const_iterator i
	 = captured_variables.begin ();
       i != captured_variables.end ();
       ++i)
    {
      // Create the new parameter and add it to the constructor.
      location here = (*i)->get_location ();
      ref_variable_decl decl
	= new model_parameter_decl (here,
				    "capture$" + (*i)->get_name (),
				    (*i)->get_declared_type (),
				    this);
      constructor->add_parameter (decl);
      // Create a new assignment to initialize the field.
      ref_field cfield = synthetic_fields[*i];

      ref_field_ref lhs = new model_field_ref (here);
      lhs->set_field (cfield->get_name ());
      lhs->set_field (cfield.get ());

      ref_simple_variable_ref rhs
	= new model_simple_variable_ref (here, decl.get ());

      ref_assignment assign = new model_assignment (here);
      assign->set_lhs (lhs);
      assign->set_rhs (rhs);
      ref_expression_stmt stmt = new model_expression_stmt (here, assign);
      statements.push_front (stmt);
    }

  // Add new statements to the constructor.
  model_constructor *cons
    = assert_cast<model_constructor *> (constructor.get ());
  cons->prepend_statements (scope, statements, do_resolve);
}

std::string
model_class::generate_synthetic_field_name (const std::string &prefix)
{
  std::string name (prefix);
  while (field_map.find (name) != field_map.end ())
    name += "$";
  return name;
}

std::string
model_class::generate_synthetic_method_name (const std::string &prefix,
                                             ref_method &method)
{
  std::string name (prefix);
  std::string descriptor (method->get_descriptor ());
  while (has_method_with_descriptor_p (name, descriptor))
    name += "$";
  return name;
}

model_field *
model_class::add_assert_members ()
{
  if (! dollar_assertionsDisabled)
    {
      // This can only be called for the first time when resolving a
      // method body of a method declared in this class.
      assert (resolution_state == POST_MEMBERS);

      std::string name = generate_synthetic_field_name ("$assertionsDisabled");

      // Create a new '$assertionsDisabled' field.
      ref_field result
        = new model_field (LOCATION_UNKNOWN, name,
                           new model_forwarding_resolved (LOCATION_UNKNOWN,
                                                          primitive_boolean_type),
                           this);
      result->set_modifiers (ACC_PRIVATE | ACC_STATIC | ACC_FINAL);
      result->set_synthetic ();

      // Add a static initializer for the field.  FIXME: should assertions
      // be enabled during static blocks?  If so we would need to handle
      // this differently.
      // Call '<this class>.desiredAssertionStatus()'.
      ref_method_invocation call
        = new model_method_invocation (LOCATION_UNKNOWN);
      call->set_expression (new model_class_ref (LOCATION_UNKNOWN, this));
      call->set_method ("desiredAssertionStatus");
      // Now invert the value.
      ref_unary expr = new model_logical_not (LOCATION_UNKNOWN);
      expr->set_expression (call);

      result->set_initializer (expr);

      // Put in various places it might be needed.
      // FIXME: clean up these internal adds.
      ambiguous_field_map[name] = result.get ();
      field_map.insert (std::make_pair (name, result.get ()));
      fields.push_back (result);
      // Set up static initializer.
      add (static_inits, result);
      
      dollar_assertionsDisabled = result;

      // Now make sure the field is resolved.  This works by side
      // effect.  FIXME: just have a real method on model_field.
      result->constant_p ();
    }
  return dollar_assertionsDisabled.get ();
}

ref_method
model_class::add_implicit_constructor ()
{
  std::list<ref_stmt> stmt_list;

  // FIXME: it would be nice if we could annotate the location with
  // "compiler-generated" somehow.
  // Use the location of the class so that the user can have at least
  // some clue about where the method comes from.
  location where = get_location ();

  if (this != global->get_compiler ()->java_lang_Object ())
    {
      ref_super_invocation super = new model_super_invocation (where);
      super->set_enclosing_class (this);
      super->set_synthetic ();
      ref_stmt statement = new model_expression_stmt (where, super);

      stmt_list.push_back (statement);
    }

  ref_block block = new model_block (where, stmt_list);

  ref_method init = new model_constructor (where, this);
  init->set_name ("<init>");
  init->set_modifiers (modifiers & ACC_ACCESS);
  // According to the class file format documentation, the default
  // constructor is not marked 'synthetic'.
  init->set_body (block);
  init->set_return_type (new model_forwarding_resolved (where,
							primitive_void_type));

  return init;
}

bool
model_class::has_method_with_descriptor_p (const std::string &name,
                                           const std::string &descriptor) const
{
  std::pair<AllMethods::const_iterator, AllMethods::const_iterator> its;
  its = all_methods.equal_range (name);
  for (AllMethods::const_iterator i = its.first; i != its.second; ++i)
    {
      model_method *m = i->second.get ();
      if (m->get_descriptor () == descriptor)
        return true;
    }
  return false;
}

bool
model_class::method_inheritable_p (model_method *meth, model_class *other)
{
  std::string this_name = meth->get_name ();
  // Don't inherit constructors or private methods.  Note that
  // accessible_p() might return 'true' for a private method if we
  // extend our own outer class.  (Also, although our <clinit> methods
  // are always private, ones generated by javac never are.)
  if (this_name == "<clinit>" || this_name == "<init>"
      || (meth->get_modifiers () & ACC_PRIVATE) != 0
      // We check access as if the method was declared in OTHER.  This
      // is correct because we are inheriting the method from this
      // class, not from its actual declaring class.
      || ! accessible_p (meth, this, NULL, other))
    return false;

  std::pair<AllMethods::const_iterator, AllMethods::const_iterator> its;
  its = all_methods.equal_range (this_name);
  for (AllMethods::const_iterator i = its.first; i != its.second; ++i)
    {
      if (i->second->hides_or_overrides_p (meth, this))
	return false;
    }

  return true;
}

template <typename IteratorT>
void
model_class::inherit_methods (const IteratorT &begin, const IteratorT &end,
                              model_class *other, bool default_access)
{
  for (IteratorT i = begin; i != end; ++i)
    {
      if (default_access && ((*i)->get_modifiers () & ACC_ACCESS) != 0)
	continue;

      if (method_inheritable_p ((*i).get (), other))
	{
	  // FIXME if OTHER == Object && this.interface_p(), then add
	  // a new abstract method -- ?
	  all_methods.insert (std::make_pair ((*i)->get_name (), *i));
	  source_order_methods.push_back ((*i).get ());

	  if (! abstract_p () && (*i)->abstract_p ())
	    // FIXME: locations.
	    std::cerr << error ("method %1 is %<abstract%> but "
				"not implemented here")
	      % (*i).get ();
	}
    }
}

void
model_class::inherit_methods (model_class *other, bool default_access)
{
  if (interface && ! other->interface)
    {
      // Handle abstract methods inherited from Object.
      std::list<ref_method> ms;
      ms = global->get_compiler ()->get_object_interface_methods ();
      inherit_methods (ms.begin (), ms.end (), other, default_access);
    }
  else
    {
      inherit_methods (other->begin_all_methods (), other->end_all_methods (),
                       other, default_access);
    }
}

/// Two interfaces conflict if they each have a method with the same name,
/// same argument types, but a different return type.
bool
model_class::has_any_conflicting_interface_method_p (model_class *other) const
{
  AllMethodsIterator end = end_all_methods ();
  for (AllMethodsIterator i = begin_all_methods (); i != end; ++i)
    {
      std::pair<AllMethods::const_iterator, AllMethods::const_iterator> its;
      its = other->all_methods.equal_range ((*i)->get_name ());
      for (AllMethods::const_iterator j = its.first; j != its.second; ++j)
        {
          if (! (*i)->same_arguments_p (j->second.get ()))
            continue;
          if ((*i)->get_return_type () != j->second->get_return_type ())
            return true;
        }
    }
  return false;
}

void
model_class::inherit_fields (model_class *other, bool default_access)
{
  for (std::multimap<std::string, model_field *>::iterator i
	 = other->field_map.begin ();
       i != other->field_map.end ();
       ++i)
    {
      if (default_access && ((*i).second->get_modifiers () & ACC_ACCESS) != 0)
	continue;

      std::string name = (*i).first;

      // Don't inherit a field if it is hidden by a local field.
      std::multimap<std::string, model_field *>::const_iterator it
	= field_map.find (name);
      if (it != field_map.end ()
	  && (*it).second->get_declaring_class () == this)
	continue;

      // Check accessibility of inherited field.
      model_field *fld = (*i).second;
      // Note that accessible_p() will return true for a private field
      // in some cases, but private fields are never inherited.
      if ((fld->get_modifiers () & ACC_PRIVATE) != 0
	  || ! accessible_p (fld, this, NULL, other))
	continue;

      // Update the field map.
      field_map.insert (std::make_pair (name, fld));

      // Update the ambiguous field map.
      std::map<std::string, model_field_base *>::const_iterator ambit
	= ambiguous_field_map.find (name);
      if (ambit == ambiguous_field_map.end ())
	ambiguous_field_map[name] = fld;
      else if ((*ambit).second != fld)
	{
	  model_ambiguous_field *amb;
	  if (! (*ambit).second->ambiguous_p ())
	    {
	      model_field *fld
		= assert_cast<model_field *> ((*ambit).second);
	      amb = new model_ambiguous_field (fld);
	      ambiguous_field_map[name] = amb;
	    }
	  else
	    amb = assert_cast<model_ambiguous_field *> ((*ambit).second);
	  amb->add ((*i).second);
	}
    }
}

void
model_class::inherit_types (model_class *other, bool default_access)
{
  for (std::multimap<std::string, model_class *>::iterator i
	 = other->all_member_classes.begin ();
       i != other->all_member_classes.end ();
       ++i)
    {
      if (default_access && ((*i).second->get_modifiers () & ACC_ACCESS) != 0)
	continue;

      std::string name = (*i).first;

      // Don't inherit a class if it is hidden by a local class.
      if (member_classes.find (name) != member_classes.end ())
	continue;

      // Check accessibility of inherited type.
      model_class *klass = (*i).second;
      if ((klass->get_modifiers () & ACC_PRIVATE) != 0
	  || ! accessible_p (klass, this, NULL, other))
	continue;

      all_member_classes.insert (std::make_pair (name, klass));
    }
}

void
model_class::inherit_package_private_members ()
{
  // If our superclass is in a different package from us, then we have
  // to find the nearest superclass that is in our package and handle
  // inheritance of package-private members from it.  We don't need to
  // look any higher than that, since that class will already have all
  // the visible package-private members.  Also, we needn't consider
  // members with other visibility, since the visible ones are already
  // handled by our direct superclass.

  if (! superclass || array_p () || interface)
    return;
  model_class *iter = dynamic_cast<model_class *> (superclass->type ());
  if (! iter || get_package () == iter->get_package ())
    {
      // Nothing to do in this case.
      return;
    }

  while (iter && get_package () != iter->get_package ())
    {
      if (! iter->superclass)
	iter = NULL;
      else
	iter = dynamic_cast<model_class *> (iter->superclass->type ());
    }
  if (! iter)
    return;

  inherit_methods (iter, true);
  inherit_fields (iter, true);
  inherit_types (iter, true);
}

void
model_class::check_uninherited_abstract_methods ()
{
  // This method is used to check a special case: we might have
  // inherited some package-private abstract methods from some
  // superclass.  We only have to check our nearest ancestor that is
  // in a different package; other situations are already handled.
  // For instance, if the method is overridden, it will not have been
  // inherited by that class.  We only need to examine the concrete
  // hierarchy as interface methods are public.

  assert (! abstract_p ());

  // Find the ancestor.
  model_package *our_package = get_package ();
  model_class *ancestor;
  for (ancestor = this;
       ancestor && ancestor->get_package () == our_package;
       ancestor = ancestor->get_superclass ())
    ;

  // We might not find such an ancestor.
  if (ancestor)
    {
      for (AllMethods::const_iterator i = ancestor->all_methods.begin ();
	   i != ancestor->all_methods.end ();
	   ++i)
	{
	  if (! i->second->abstract_p ()
	      || (i->second->get_modifiers () & ACC_ACCESS) != 0)
	    continue;

	  // Found one.
	  std::cerr << error ("ancestor %1 of non-abstract class %2 "
			      "declares package-private abstract method "
			      "%3, which cannot be overridden here")
	    % ancestor % this % i->second.get ();
	}
    }
}

void
model_class::do_resolve_classes (resolution_scope *scope)
{
  // First make sure the compilation unit has been resolved.
  if (compilation_unit)
    compilation_unit->resolve (scope);

  resolve_hook (scope);

  resolve_annotation_classes (scope);

  // Put the type parameters in scope.  Note that we don't resolve
  // them here, as that can lead to false circularity reports.
  resolution_scope::push_iscope tp_pusher (scope, &type_parameters);
  resolution_scope::push_warnings warn_holder (scope, this);

  model_class *obj = global->get_compiler ()->java_lang_Object ();
  bool was_null = ! superclass;
  if (this == obj)
    {
      // We treat errors related to processing Object as relatively
      // fatal.  It is simpler to just require the user to fix these
      // before continuing.
      if (interface)
	throw error ("%<java.lang.Object%> must be a class");
      if (superclass)
	throw error ("%<java.lang.Object%> must not have a superclass");
      if (! interfaces.empty ())
	throw error ("%<java.lang.Object%> must not implement any interfaces");
    }
  else if (interface)
    {
      if (superclass)
	{
	  std::cerr << error ("interface %1 must not have a superclass")
	    % this;
	  superclass = NULL;
	}
    }
  else if (! superclass)
    superclass = new model_forwarding_resolved (LOCATION_UNKNOWN, obj);

  if (superclass)
    {
      superclass->resolve_classes (scope);
      // If there was an explicit 'extends' clause, and this class
      // isn't an array (we supply the extends clause for arrays) and
      // not anonymous (new Object() { } is just fine), and not a
      // wildcard, and not a type variable, and not a class instance
      // (we would emit the warning when handling the parent class)
      // then warn if we are extending Object, as that is redundant.
      // Don't bother warning if the class came from a .class file --
      // those are always completely explicit.
      if (! was_null && ! array_p () && ! from_class
 	  && ! anonymous && superclass->type () == obj
	  && dynamic_cast<model_wildcard *> (this) == NULL
	  && ! type_variable_p ()
	  && ! parameterized_p ()
	  && scope->warn_unneeded_extends ())
	std::cerr << warn (global->get_compiler ()->warn_unneeded_extends (),
			   "explicit %<extends java.lang.Object%>");

      if (superclass->type ()->final_p ())
	std::cerr << error ("class %1 extends %<final%> class %2")
	  % this % superclass->type ();

      if (superclass->type ()->interface_p ())
	{
	  if (anonymous)
	    {
	      // We always set the superclass of an anonymous class.
	      // If it turns out to be an interface, we rewrite things
	      // here.
	      assert (interfaces.empty ());
	      interfaces.push_back (superclass);
	      superclass
		= new model_forwarding_resolved (superclass->get_location (),
						 obj);
	      superclass->resolve_classes (scope);
	    }
	  else
	    std::cerr << error ("class %1 extends interface %2")
	      % this % superclass->type ();
	}
    }

  {
    std::set<model_type *> iface_set;
    for (std::list<ref_forwarding_type>::const_iterator i
	   = interfaces.begin ();
	 i != interfaces.end ();
	 ++i)
      {
	(*i)->resolve_classes (scope);
	model_type *iface = (*i)->type ();

	if (! iface->interface_p ())
	  // Report error against the location in the implements clause.
	  std::cerr << (*i)->error ("type %1 in %<implements%> clause is "
				    "not an interface")
	    % iface;

	if (iface_set.find (iface) != iface_set.end ())
	  std::cerr << (*i)->error ("type %1 appears twice in "
				    "%<implements%> clause")
	    % iface;
	else
	  iface_set.insert (iface);
      }
  }

  // Also check to ensure we're following the rules for generic
  // interface implementation.
  {
    std::map<model_class *, model_class *> track;
    check_interface_instances (this, track);
  }

  resolution_state = PRE_MEMBERS;

  // Subclasses of Throwable cannot be parameterized.
  if (! type_parameters.empty ()
      && global->get_compiler ()->java_lang_Throwable ()->assignable_from_p (this))
    std::cerr << error ("subclasses of %<java.lang.Throwable%> "
			"cannot be generic");

  resolution_scope::push_iscope self_holder (scope, this);

  // Insert our local classes into our map, then resolve the class
  // envelope of each one.  FIXME: this is bogus, but it is needed
  // since our scoping approach is somewhat broken.  See the IScope
  // idea in TODO.
  // Double FIXME: we have IScope.. now what?
  for (std::map<std::string, ref_class>::const_iterator i
	 = member_classes.begin ();
       i != member_classes.end ();
       ++i)
    {
      all_member_classes.insert (std::make_pair ((*i).first,
						 (*i).second.get ()));
    }

  // Note we also handle inheritance before resolving.  This allows
  // name lookup when resolving member types to work properly.
  std::list<model_class *> all_super_types;
  compute_super_types (all_super_types);
  for (std::list<model_class *>::const_iterator class_it
	 = all_super_types.begin ();
       class_it != all_super_types.end ();
       ++class_it)
    inherit_types (*class_it);

  ::resolve_classes (scope, member_classes);
}

void
model_class::resolve_classes (resolution_scope *scope)
{
  // This can also be called for type variables and wildcards.
  // assert (local || anonymous);
  if (resolution_state >= PRE_MEMBERS)
    return;

  resolution_state = DOING_SUPERCLASS;

  // This is pretty ugly: if this class came from a .class file, then
  // it doesn't make sense to resolve it in some outer scope.  The
  // reason for this is that each such class will have its own
  // compilation unit, which is the only way to properly look up inner
  // classes by name.
  if (from_class_p ())
    {
      resolution_scope special_scope;
      // We need to push our compilation unit, but also all our outer
      // classes in order.
      compilation_unit->push_on_scope (&special_scope);
      if (declaring_class)
	declaring_class->push_on_scope_for_class (&special_scope);
      do_resolve_classes (&special_scope);
    }
  else
    do_resolve_classes (scope);
}

void
model_class::resolve_classes ()
{
  if (resolution_state >= PRE_MEMBERS)
    return;

  // An inner class cannot be resolved until its containing class has
  // been.  Resolution might start anywhere, though, so we have to
  // check and delegate here.
  if (declaring_class && declaring_class->resolution_state < PRE_MEMBERS)
    {
      declaring_class->resolve_classes ();
      // Resolving our containing class should have resolved us as
      // well.
      assert (resolution_state >= PRE_MEMBERS);
      return;
    }

  if (resolution_state == DOING_SUPERCLASS)
    throw error ("circular dependency involving %1 detected") % this;
  resolution_state = DOING_SUPERCLASS;

  // Make sure the compilation unit has been resolved and pushed into
  // our scope.  Note that array types don't have a compilation unit
  // (or a reason to be resolved, really -- fixme?).
  resolution_scope scope;
  if (declaring_class)
    declaring_class->push_on_scope (&scope);
  else if (compilation_unit)
    compilation_unit->push_on_scope (&scope);

  do_resolve_classes (&scope);
}

void
model_class::compute_object_interface_methods ()
{
  // This computes the special abstract methods that are inherited by
  // interfaces.

  std::list<ref_method> result;
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      // We're only interested in public instance methods.
      if ((*i)->static_p ()
	  || ((*i)->get_modifiers () & ACC_PUBLIC) == 0
	  || (*i)->constructor_p ())
	continue;

      ref_method m = (*i)->create_abstract_instance ();
      result.push_back (m);
    }

  global->get_compiler ()->set_object_interface_methods (result);
}

void
model_class::resolve_members (resolution_scope *scope)
{
  if (resolution_state < PRE_MEMBERS)
    resolve_classes (scope);
  if (resolution_state >= DOING_MEMBERS)
    return;
  assert (resolution_state == PRE_MEMBERS);
  resolution_state = DOING_MEMBERS;

  // If we are processing 'Object', then compute the abstract methods
  // needed for interfaces.
  if (! superclass && ! interface)
    compute_object_interface_methods ();

  resolve_annotations (scope);

  // Let the subclass modify behavior a little.  This must come after
  // compute_object_interface_methods().
  resolve_member_hook (scope);

  // Put type parameters in scope, then resolve them.  This is done
  // here and not during resolve_classes() to avoid false circularity
  // reports.  It isn't done in resolve() because this is part of the
  // class envelope.
  resolution_scope::push_iscope tp_pusher (scope, &type_parameters);
  resolution_scope::push_warnings warn_holder (scope, this);
  type_parameters.resolve_classes (scope);

  // Resolve types of methods and fields.
  ::resolve_classes (scope, methods);
  ::resolve_classes (scope, fields);

  std::list<model_class *> all_super_types;
  compute_super_types (all_super_types);
  for (std::list<model_class *>::const_iterator class_it
	 = all_super_types.begin ();
       class_it != all_super_types.end ();
       ++class_it)
    (*class_it)->resolve_members ();

  // Insert our methods into the list of methods.
  for (std::list<ref_method>::const_iterator it = methods.begin ();
       it != methods.end ();
       ++it)
    {
      all_methods.insert (std::make_pair ((*it)->get_name (), *it));
      source_order_methods.push_back ((*it).get ());
    }

  // Insert our fields into the field map first.
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    {
      std::string n = (*i)->get_name ();
      if (ambiguous_field_map.find (n) != ambiguous_field_map.end ())
	{
	  // FIXME: should be able to show other locations.
	  throw (*i)->error ("duplicate declaration of field %1")
	    % n;
	}
      ambiguous_field_map[n] = (*i).get ();
      field_map.insert (std::make_pair (n, (*i).get ()));
    }

  // Inherit fields and methods.  Classes were handled elsewhere.
  for (std::list<model_class *>::const_iterator class_it
	 = all_super_types.begin ();
       class_it != all_super_types.end ();
       ++class_it)
    {
      inherit_methods (*class_it);
      inherit_fields (*class_it);
    }

  // If our parent is in a different package, we need to look higher
  // in the class hierarchy to handle package-private members we might
  // inherit.
  inherit_package_private_members ();

  // A few checks on methods.
  bool constructor_found = false;
  std::set<std::string> descriptors;
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      if ((*i)->constructor_p ())
	constructor_found = true;

      std::string n = (*i)->get_name ();
      // We construct a string that includes the method name and
      // descriptor and that won't have false clashes.
      n += (*i)->get_descriptor ();
      if (descriptors.find (n) != descriptors.end ())
	// FIXME: show the duplicate as well.
	throw (*i)->error ("duplicate method %1 in class %2")
	  % n % this;
      descriptors.insert (n);

      if (! abstract_p () && (*i)->abstract_p ())
	std::cerr << (*i)->error ("method %1 is %<abstract%> but in "
				  "concrete class")
	  % (*i).get ();

      if (inner_p () && (*i)->static_p () && ! (*i)->static_initializer_p ())
	std::cerr << (*i)->error ("static method invalid in inner class");

      (*i)->check_override ();
    }

  if (! abstract_p () && ! array_p ())
    check_uninherited_abstract_methods ();

  // We add an anonymous class' constructor later.
  if (! constructor_found && ! interface && ! anonymous && ! array_p ())
    {
      ref_method init = add_implicit_constructor ();
      init->resolve_classes (scope);

      add_method (init);
    }

  resolution_state = POST_MEMBERS;
}

void
model_class::resolve_members ()
{
  if (resolution_state < PRE_MEMBERS)
    resolve_classes ();
  if (resolution_state >= DOING_MEMBERS)
    return;
  assert (resolution_state == PRE_MEMBERS);

  // Local and anonymous classes require more context than
  // push_on_scope can provide, so they had better not ever get here.
  assert (! anonymous && ! local);
  resolution_scope scope;
  push_on_scope (&scope);
  resolve_members (&scope);
}

bool
model_class::create_clinit_method ()
{
  if (static_inits.empty () || ! check_init_list (static_inits))
    return false;

  location loc = get_location ();
  ref_method clinit = new model_method (loc, this);
  clinit->set_name ("<clinit>");
  clinit->set_modifiers (ACC_STATIC | ACC_PRIVATE);
  // According to the class file format documentation, <clinit> is not
  // marked 'synthetic'.
  clinit->set_body (new model_block (loc, static_inits));
  clinit->set_return_type (new model_forwarding_resolved (loc,
							  primitive_void_type));

  add (clinit);
  return true;
}

void
model_class::create_finit_method ()
{
  assert (! bool (finit_));

  if (instance_inits.empty () || ! check_init_list (instance_inits))
    return;

  location loc = get_location ();
  finit_ = new model_method (loc, this);
  finit_->set_instance_initializer ();
  finit_->set_modifiers (ACC_PRIVATE | ACC_FINAL);
  finit_->set_synthetic ();
  finit_->set_body (new model_block (loc, instance_inits));
  finit_->set_return_type (new model_forwarding_resolved (loc,
							  primitive_void_type));
  // Note that the 'throws' clause for this method is handled
  // specially during resolution of the method.
  finit_->set_name (generate_synthetic_method_name ("finit$", finit_));

  add_method (finit_);
}

model_method *
model_class::add_class_members ()
{
  if (! class_)
    {
      location loc = get_location ();

      class_ = new model_method (loc, this);
      class_->set_modifiers (ACC_STATIC | ACC_PRIVATE | ACC_FINAL);
      class_->set_synthetic ();
      class_->set_return_type (new model_forwarding_resolved (loc,
							      global->get_compiler ()->java_lang_Class ()));
      class_->set_used ();

      // This method accepts a string and a boolean as arguments.  The
      // idea comes from jikes.  The boolean controls whether we want
      // the type of the argument or its element type; in all cases we
      // pass an array name to this class.  This lets us avoid
      // initializing the class when 'foo.class' is evaluated.
      //   static Class class$ (String name, boolean isArray)
      //   {
      //     try
      //       {
      //         Class result = Class.forName (name);
      //         return isArray ? result : result.getComponentType ();
      //       }
      //     catch (ClassNotFoundException exc)
      //       {
      //         throw (Error) new NoClassDefFoundError ().initCause (exc);
      //       }
      //   }

      assert (! global->get_compiler ()->target_15 ());

      // FIXME: we could really clean up this code by adding some new
      // constructors and helper methods here and there.
      // FIXME: we have un-ref'd objects here, could be a memory
      // leak if a constructor throws an exception.

      std::list<ref_expression> args;

      ref_variable_decl name_arg
	= new model_parameter_decl (loc, "name",
				    new model_forwarding_resolved (loc,
								   global->get_compiler ()->java_lang_String ()),
				    this);
      ref_variable_decl isarray_arg
	= new model_parameter_decl (loc, "isArray",
				    new model_forwarding_resolved (loc,
								   primitive_boolean_type),
				    this);
      std::list<ref_variable_decl> params;
      params.push_back (name_arg);
      params.push_back (isarray_arg);
      class_->set_parameters (params);

      ref_variable_decl exc
	= new model_variable_decl (loc, "exc",
				   new model_forwarding_resolved (loc,
								  global->get_compiler ()->java_lang_ClassNotFoundException ()),
				   this);

      ref_simple_variable_ref varref
	= new model_simple_variable_ref (loc, exc.get ());
      ref_expression throw_expr
	= new model_new (loc, global->get_compiler ()->java_lang_NoClassDefFoundError ());

      if (global->get_compiler ()->target_14 ())
	{
	  // In 1.4 we can chain the exception.
	  ref_method_invocation initcause = new model_method_invocation (loc);
	  initcause->set_method ("initCause");
	  args.push_back (varref);
	  initcause->set_arguments (args);
	  initcause->set_expression (throw_expr);
	  throw_expr
	    = new model_cast (loc,
			      new model_forwarding_resolved (loc,
							     global->get_compiler ()->java_lang_Error ()),
			      initcause);
	}

      ref_throw throw_stmt = new model_throw (loc);
      throw_stmt->set_expression (throw_expr);

      ref_variable_decl result
	= new model_variable_decl (loc, "result",
				   new model_forwarding_resolved (loc,
								  global->get_compiler ()->java_lang_Class ()),
				   this);

      args.clear ();
      args.push_back (new model_simple_variable_ref (loc, name_arg.get ()));
      model_type_qualified_invocation *forname
	= new model_type_qualified_invocation (loc);
      forname->set_method ("forName");
      forname->set_arguments (args);
      forname->set_class (new model_forwarding_resolved (loc,
							 global->get_compiler ()->java_lang_Class ()));
      result->set_initializer (forname);

      ref_method_invocation getcomp = new model_method_invocation (loc);
      getcomp->set_expression (new model_simple_variable_ref (loc,
							      result.get ()));
      getcomp->set_method ("getComponentType");
      ref_expression cond
	= new model_conditional (loc,
				 new model_simple_variable_ref (loc,
								isarray_arg.get ()),
				 new model_simple_variable_ref (loc,
								result.get ()),
				 getcomp);

      std::list<ref_stmt> statements;
      statements.push_back (throw_stmt);
      ref_block block = new model_block (loc, statements);

      ref_catch catcher = new model_catch (loc);
      catcher->set_formal_parameter (exc);
      catcher->set_block (block);

      model_return *retstmt = new model_return (loc);
      retstmt->set_expression (cond);

      std::list<ref_variable_decl> vars;
      vars.push_back (result);
      ref_variable_stmt resultdecl = new model_variable_stmt (loc);
      resultdecl->set_variables (vars);

      statements.clear ();
      statements.push_back (resultdecl);
      statements.push_back (retstmt);

      block = new model_block (loc, statements);

      ref_try trystmt = new model_try (loc);
      trystmt->set_block (block);
      std::list<ref_catch> catchers;
      catchers.push_back (catcher);
      trystmt->set_catchers (catchers);

      statements.clear ();
      statements.push_back (trystmt);
      block = new model_block (loc, statements);
      class_->set_body (block);

      class_->set_name (generate_synthetic_method_name ("class$", class_));

      add_method (class_);
    }

  return class_.get ();
}

void
model_class::check_members ()
{
  const char *msg
    = (local ? "local class %1 in static context cannot have static member"
       : "anonymous class %1 in static context cannot have static member");

  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      // FIXME: is checking for synthetic here really ok?
      // Or should class$ be called in our outermost class?
      // ... and be package-private?
      if ((*i)->static_p () && ! (*i)->synthetic_p ())
	std::cerr << (*i)->error (msg) % this;
    }

  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    {
      if ((*i)->static_p () && ! (*i)->constant_p ())
	std::cerr << (*i)->error (msg) % this;
    }

  for (std::map< std::string, owner<model_class> >::const_iterator i
	 = member_classes.begin ();
       i != member_classes.end ();
       ++i)
    {
      if ((*i).second->static_p ())
	std::cerr << (*i).second->error (msg) % this;
    }
}

void
model_class::check_serialization_fields ()
{
  if (! global->get_compiler ()->java_io_Serializable ()->assignable_from_p (this))
    return;

  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    {
      if ((*i)->get_name () == "serialVersionUID")
	return;
    }

  std::cerr << warn (global->get_compiler ()->warn_bad_serialization_field (),
		     "class %1 is serializable but does not declare %<serialVersionUID%>")
    % this;
}

void
model_class::resolve (resolution_scope *scope)
{
  if (resolution_state == DONE)
    return;

  resolution_scope::push_warnings warn_holder (scope, this);
  resolution_scope::push_iscope holder (scope, this);
  resolution_scope::push_iscope tp_pusher (scope, &type_parameters);

  if (resolution_state < POST_MEMBERS)
    resolve_members (scope);
  assert (resolution_state == POST_MEMBERS);

  ::resolve (scope, member_classes);

  // Create this before resolving fields, so it can be used as a
  // 'throws' target.
  create_finit_method ();

  // Resolve fields declared in this class.
  for (std::list< owner<model_field> >::const_iterator i
	 = fields.begin ();
       i != fields.end ();
       ++i)
    {
      ref_field f = *i;

      model_static_context_scope static_holder (f->static_p ());
      resolution_scope::push_iscope holder (scope, &static_holder);
      if (! f->static_p ())
	{
	  // If a field's initializer can throw, the thrown exception
	  // types must be forwarded to finit$ for special handling.
	  field_init_forwarding_catcher catcher (finit_.get ());
	  resolution_scope::push_iscope catch_holder (scope, &catcher);
	  f->resolve (scope);
	}
      else
	{
	  static_field_catcher catcher (f.get ());
	  resolution_scope::push_iscope catch_holder (scope, &catcher);
	  f->resolve (scope);
	}

      if (inner_p () && f->static_p () && ! f->constant_p ())
	std::cerr << f->error ("%<static%> field of inner class must "
			       "be a compile-time constant");
    }

  bool found_clinit;
  if (from_class_p ())
    found_clinit = has_method_with_descriptor_p ("<clinit>", "()V");
  else
    found_clinit = create_clinit_method ();

  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    (*i)->resolve (scope);

  if (static_context)
    {
      assert (local || anonymous);
      check_members ();
    }

  // After resolving the methods and initializers, we go back and add
  // hidden arguments to the constructors.  This lets us correctly
  // capture final variables from the enclosing scope.
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      if ((*i)->constructor_p ())
	add_captured_constructor_arguments (scope, *i);

      // Now that all modifications to the method have been made,
      // check definite assignment.
      (*i)->check_definite_assignment ();
    }

  // If we didn't see any static initializer, then nothing has done
  // definite assignment checking for all our blank final static
  // fields.
  if (! found_clinit)
    {
      for (std::list< owner<model_field> >::const_iterator i = fields.begin ();
	   i != fields.end ();
	   ++i)
	{
	  if ((*i)->static_p () && (*i)->final_p ()
	      && ! (*i)->has_initializer_p ())
	    std::cerr << (*i)->error ("blank final field %1 must be assigned")
	      % (*i).get ();
	}
    }

  // Do last-minute warning checks.
  if (scope->warn_bad_serialization_field ())
    check_serialization_fields ();

  // Check whether private members have been referenced.  We can only
  // do this once the outermost class has been resolved.
  if (! declaring_class)
    check_referenced (scope);

  resolution_state = DONE;
}

void
model_class::find_members (const std::string &memname,
			   std::set<model_class *> &result,
			   IContext *request)
{
  resolve_classes ();

  for (std::multimap<std::string, model_class *>::const_iterator i
	 = all_member_classes.begin ();
       i != all_member_classes.end ();
       ++i)
    {
      model_class *klass = (*i).second;
      if ((*i).first == memname
	  // FIXME: should have qualifier here.
	  && accessible_p (klass, request, this, this))
	result.insert (klass);
    }
}

void
model_class::find_members (const std::string &memname,
			   std::set<model_field *> &result,
			   IContext *request,
			   model_class *qualifier)
{
  // We need to ensure that this class' "envelope" has been resolved
  // before we can look up members.
  resolve_members ();

  for (std::multimap<std::string, model_field *>::const_iterator i
	 = field_map.begin ();
       i != field_map.end ();
       ++i)
    {
      model_field *fld = (*i).second;
      if (memname == fld->get_name ()
	  && accessible_p (fld, request, qualifier, this))
	result.insert (fld);
    }
}

void
model_class::find_members (const std::string &memname,
			   std::set<model_method *> &result,
			   IContext *request,
			   model_class *qualifier)
{
  resolve_members ();
  std::pair<AllMethods::const_iterator, AllMethods::const_iterator> its;
  its = all_methods.equal_range (memname);
  for (AllMethods::const_iterator i = its.first; i != its.second; ++i)
    {
      if (accessible_p (i->second.get (), request, qualifier, this))
	result.insert (i->second.get ());
    }
}

std::string
model_class::get_pretty_name () const
{
  std::string result;
  if (declaring_class)
    result = declaring_class->get_pretty_name () + "$" + get_assigned_name ();
  else
    {
      std::string cu
	= compilation_unit->get_package ()->get_fully_qualified_name ();
      result = (cu.empty ()) ? name : cu + "." + name;
    }
  if (! type_parameters.empty ())
    result += type_parameters.get_pretty_name ();
  return result;
}

void
model_class::check_deprecated (model_element *request)
{
  if (deprecated && global->get_compiler ()->warn_deprecated ())
    std::cerr << request->warn (global->get_compiler ()->warn_deprecated (),
				"class %1 is deprecated")
      % get_pretty_name ();
}

void
model_class::check_referenced (resolution_scope *scope)
{
  warning_scope::push_warnings push (scope, this);

  if (! scope->warn_unused_private_members ())
    return;

  // We only emit this warning for private classes, as others can be
  // referenced from elsewhere.
  if ((modifiers & ACC_PRIVATE) != 0 && ! used)
    std::cerr << warn (global->get_compiler ()->warn_unused_private_members (),
		       "private class %1 not used in its scope")
      % this;

  // Check all member classes.
  for (std::map<std::string, ref_class>::const_iterator i
	 = member_classes.begin ();
       i != member_classes.end ();
       ++i)
    (*i).second->check_referenced (scope);

  // Check fields.
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    (*i)->check_referenced (scope);

  // Check methods.
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    (*i)->check_referenced (scope);
}

void
model_class::ensure_enclosing (model_class *inner)
{
  for (model_class *k = inner; k; k = k->get_lexically_enclosing_class ())
    {
      if (k == this)
	return;
    }

  throw inner->error ("%1 does not lexically enclose %2")
    % this % inner;
}

void
model_class::check_interface_instances (model_class *base,
					std::map<model_class *, model_class *> &track)
{
  if (base->interface_p ())
    {
      if (base->parameterized_p ())
	{
	  model_class_instance *inst
	    = assert_cast<model_class_instance *> (base);
	  model_class *parent = inst->get_parent ();
	  if (track.find (parent) == track.end ())
	    track[parent] = base;
	  else if (track[parent] != base)
	    throw error ("interface %1 implemented twice with different "
			 "parameterizations")
	      % parent;
	}
    }
  else
    {
      // Ordinary class, so check superclass.
      if (base->get_superclass () != NULL)
	check_interface_instances (base->get_superclass (), track);
    }

  std::list<ref_forwarding_type> ifaces = base->get_interfaces ();
  for (std::list<ref_forwarding_type>::const_iterator i = ifaces.begin ();
       i != ifaces.end ();
       ++i)
    check_interface_instances (assert_cast<model_class *> ((*i)->type ()),
			       track);
}

model_class *
model_class::apply_type_map (model_element *, const model_type_map &)
{
  assert (! anonymous);
  assert (! static_context);

  // The model_class object represents the class declaration and also
  // the raw type.  Applying a type map to the raw type does nothing,
  // hence we simply return 'this'.  Creating a parameterized instance
  // is done with create_instance.
  return this;
}

model_class *
model_class::create_instance (model_element *request,
			      const std::list<model_class *> &params)
{
  model_type_map type_map;
  type_parameters.create_type_map (type_map, request, params);

  if (type_parameters.empty () || type_map.empty_p ())
    return this;

  // See if this instance has been cached.
  model_class_instance *cache = instance_cache.find_instance (type_map);
  if (cache != NULL)
    return cache;

  // We need to have our superclasses installed before we can proceed.
  resolve_classes ();

  // Create a new parameterization of the class and fill in
  // superclass and interface information.
  ref_class_instance result
    = new model_class_instance (request->get_location (), this, type_map);
  result->set_name (name);
  // FIXME?
  result->set_declaring_class (declaring_class);
  result->set_compilation_unit (compilation_unit);

  // FIXME: set annotations
  result->set_deprecated (deprecated);
  result->set_modifiers (modifiers);
  // We consider this class as synthetic since we are constructing it
  // internally, it doesn't really get written out, and since we want
  // to suppress warnings when, say, its superclass is Object.
  result->set_synthetic ();

  // FIXME: should xform here
  result->set_type_parameters (type_parameters);

  // We might be an interface.
  if (superclass)
    {
      model_class *sck = assert_cast<model_class *> (superclass->type ());
      result->set_superclass
	(new model_forwarding_resolved (request->get_location (),
					sck->apply_type_map (request,
							     type_map)));
    }

  std::list<ref_forwarding_type> ifaces;
  for (std::list<ref_forwarding_type>::const_iterator i = interfaces.begin ();
       i != interfaces.end ();
       ++i)
    {
      model_class *itype = assert_cast<model_class *> ((*i)->type ());
      itype = itype->apply_type_map (request, type_map);
      ifaces.push_back (new model_forwarding_resolved (request->get_location (),
						       itype));
    }
  result->set_implements (ifaces);

  if (interface)
    result->set_interface ();

  // FIXME: the type of this will be wrong -- but does it matter?
  // There are also other fields we're omitting here.
  if (this_0)
    result->set_this_0 (this_0);

  instance_cache.add_instance (type_map, result);
  return result.get ();
}

void
model_class::clean_up ()
{
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    (*i)->clean_up ();
}

void
model_class::check_instantiation (model_element *request)
{
  if (interface_p ())
    throw request->error ("can't create instance of interface %1")
      % this;
  if (abstract_p ())
    throw request->error ("can't create instance of %<abstract%> class %1")
      % this;
}

std::list<model_field *>
model_class::get_all_fields () const
{
  std::list<model_field *> result;

  for (std::map<std::string, model_field_base *>::const_iterator
	 i = ambiguous_field_map.begin ();
       i != ambiguous_field_map.end ();
       ++i)
    {
      model_field_base *fb = (*i).second;
      if (dynamic_cast<model_field *> (fb) != NULL)
	result.push_back (dynamic_cast<model_field *> (fb));
    }

  return result;
}

model_method *
model_class::get_accessor (model_field *fld, bool writing)
{
  assert (fld->get_declaring_class ()->assignable_from_p (this));
  std::map<IMember *, ref_method> &mem_map (writing ? settors : accessors);
  if (mem_map.find (fld) == mem_map.end ())
    {
      ref_method meth = new model_method (fld->get_location (), this);

      // An accessor must always be static so that it can't be
      // accidentally overridden by a subclass.
      meth->set_modifiers (ACC_STATIC);
      meth->set_synthetic ();

      ref_field_ref fref = new model_field_ref (fld->get_location ());
      fref->set_field (fld);

      std::list<ref_variable_decl> args;
      // If we are handling an instance field we must pass in an
      // object reference.
      if (! fld->static_p ())
	{
	  // Note that we use 'this' and not the field's declaring
	  // class here.  Using the declaring class could result in a
	  // violation of the 'protected' access rules.
	  ref_forwarding_type fldt
	    = new model_forwarding_resolved (fld->get_location (), this);
	  ref_variable_decl arg
	    = new model_parameter_decl (fld->get_location (), "ref",
					fldt, this);
	  args.push_back (arg);

	  fref->set_expression (new model_simple_variable_ref (fld->get_location (),
							       arg.get ()));
	}

      std::list<ref_stmt> statements;
      if (writing)
	{
	  // The method has a single argument and the body looks like:
	  //   field = arg;
	  // This simplifies code generation when the field reference
	  // occurs mid-expression.

	  meth->set_return_type (new model_forwarding_resolved (fld->get_location (),
								primitive_void_type));

	  ref_variable_decl arg
	    = new model_parameter_decl (fld->get_location (),
					"arg",
					fld->get_declared_type (),
					this);
	  args.push_back (arg);

	  ref_assignment assign = new model_assignment (fld->get_location ());
	  assign->set_lhs (fref);
	  assign->set_rhs (new model_simple_variable_ref (fld->get_location (),
							  arg.get ()));

	  statements.push_back (new model_expression_stmt (fld->get_location (),
							   assign));
	}
      else
	{
	  meth->set_return_type (fld->get_declared_type ());

	  // The body is 'return FREF'.
	  statements.push_back (new model_return (fld->get_location (), fref));
	}

      meth->set_parameters (args);

      ref_block block = new model_block (fld->get_location (), statements);
      meth->set_body (block);

      std::string name =
        std::string (writing ? "set$" : "get$") + fld->get_name ();
      meth->set_name (generate_synthetic_method_name (name, meth));

      mem_map[fld] = meth;
      add (meth);
      resolve_one_method (meth);
    }
  return mem_map[fld].get ();
}

model_method *
model_class::get_accessor (model_method *meth)
{
  if (accessors.find (meth) == accessors.end ())
    {
      location where = meth->get_location ();

      std::string name;
      ref_method accm;
      // This is only used in the constructor case.
      int added_args = 0;

      // Formal and actual arguments to the new method.
      std::list<ref_variable_decl> args;
      std::list<ref_expression> actual;
      // Object reference, if needed.
      ref_variable_decl arg;

      // Note that constructors are special since we can't create one
      // with a new name.  So, we are forced to add dummy arguments to
      // differentiate our constructors from ones that the user might
      // declare.
      if (meth->constructor_p ())
	{
	  model_constructor *cons = assert_cast<model_constructor *> (meth);
	  name = "<init>";
	  std::string descriptor = cons->get_descriptor ();
	  std::string::size_type it = descriptor.find(')');

	  // Unlike other compilers, we simply add 'boolean' arguments
	  // until the result is disambiguated.  This can be defeated
	  // by having a constructor with many arguments, but cases
	  // like that are pathological anyway.
	  do
	    {
	      ++added_args;
	      descriptor.insert (it, "Z");
	      ++it;
	    }
	  while (has_method_with_descriptor_p (name, descriptor));

	  accm = new model_constructor (cons);
	  accm->set_modifiers (0);
	}
      else
	{
	  accm = new model_method (where, this);
	  // An accessor must always be static so that it can't be
	  // accidentally overridden by a subclass.
	  accm->set_modifiers (ACC_STATIC);

	  if (! meth->static_p ())
	    {
	      // Note that we use 'this' and not the method's
	      // declaring class here.  Using the declaring class
	      // could result in a violation of the 'protected' access
	      // rules.
	      ref_forwarding_type declt
		= new model_forwarding_resolved (meth->get_location (), this);
	      arg = new model_parameter_decl (meth->get_location (), "ref",
					      declt, this);
	      args.push_back (arg);
	    }
	}

      accm->set_synthetic ();
      accm->set_return_type (new model_forwarding_resolved (where,
							    meth->get_return_type ()));

      // Compute the new formal arguments and actual arguments to the
      // forwarding method call we create.
      std::list<ref_variable_decl> old_args = meth->get_parameters ();
      for (std::list<ref_variable_decl>::const_iterator i = old_args.begin ();
	   i != old_args.end ();
	   ++i)
	{
	  model_variable_decl *other = (*i).get ();
	  ref_variable_decl arg
	    = new model_variable_decl (where,
				       other->get_name (),
				       other->get_declared_type (),
				       this);
	  args.push_back (arg);
	  actual.push_back (new model_simple_variable_ref (where, arg.get ()));
	}

      // For a constructor we added arguments, so add those to the
      // actual parameters.
      if (added_args)
	{
	  assert (meth->constructor_p ());
	  ref_forwarding_type booltype
	    = new model_forwarding_resolved (where, primitive_boolean_type);
	  for (int i = 0; i < added_args; ++i)
	    {
	      char buffer[10];
	      sprintf (buffer, "%d", i);
	      ref_variable_decl arg
		= new model_variable_decl (where,
					   // FIXME: duplicate name?
					   "added$" + std::string (buffer),
					   booltype,
					   this);
	      args.push_back (arg);
	    }

	  // If the class is an inner class, then resolving the
	  // 'this()' invocation will push a new 'this$0' argument at
	  // the front.  So, eliminate that here.
	  if (inner_p () && ! static_context_p ())
	    {
	      args.pop_front ();
	      // FIXME: this is a hack: we pop the this$0 parameter if
	      // the original constructor has been resolved.  There
	      // has to be a better way.
	      model_constructor *c = assert_cast<model_constructor *> (meth);
	      if (c->get_this0_parameter ())
		actual.pop_front ();
	    }
	}

      ref_invocation_base inv;
      if (meth->constructor_p ())
	{
	  // Generate 'this(actual args)'.
	  inv = new model_this_invocation (where);
	  // Sigh.
	  model_this_invocation *thi
	    = assert_cast<model_this_invocation *> (inv.get ());
	  thi->set_enclosing_class (this);
	}
      else
	{
	  inv = new model_method_invocation (where);

	  if (arg)
	    inv->set_expression (new model_simple_variable_ref (meth->get_location (),
								arg.get ()));
	}

      inv->set_arguments (actual);
      inv->set_method (meth->get_name ());

      std::list<ref_stmt> statements;
      if (meth->get_return_type () == primitive_void_type)
	statements.push_back (new model_expression_stmt (where, inv));
      else
	statements.push_back (new model_return (where, inv));

      ref_block block = new model_block (where, statements);
      accm->set_body (block);

      accm->set_parameters (args);
      accm->set_throws (meth->get_throws ());
      // FIXME: copy type parameters?

      if (! meth->constructor_p ())
	name = generate_synthetic_method_name ("call$" + meth->get_name (),
					       accm);
      accm->set_name (name);

      add (accm);
      resolve_one_method (accm);
      accessors[meth] = accm;
    }
  return accessors[meth].get ();
}

void
model_class::look_up_name (const std::string &name,
			   std::set<model_class *> &result,
			   IContext *request,
			   model_class *qualifier)
{
  find_members (name, result, request);
}

void
model_class::look_up_name (const std::string &name,
			   std::set<model_variable_decl *> &result,
			   IContext *request,
			   model_class *qualifier)
{
  std::set<model_field *> fr;
  find_members (name, fr, request, qualifier);
  for (std::set<model_field *>::const_iterator i = fr.begin ();
       i != fr.end ();
       ++i)
    result.insert (*i);
}

void
model_class::look_up_name (const std::string &name,
			   std::set<model_method *> &result,
			   IContext *request,
			   model_class *qualifier)
{
  find_members (name, result, request, qualifier);
}

void
model_class::push_on_scope (resolution_scope *scope)
{
  // assert (! local && ! anonymous);
  if (declaring_class)
    declaring_class->push_on_scope (scope);
  else
    {
      // An array class won't have a compilation unit.
      if (compilation_unit)
	compilation_unit->push_on_scope (scope);
    }
  scope->push_warning_context (this);
  scope->push_scope (this);
  scope->push_scope (&type_parameters);
}

void
model_class::push_on_scope_for_class (resolution_scope *scope)
{
  if (declaring_class)
    declaring_class->push_on_scope_for_class (scope);
  scope->push_warning_context (this);
  scope->push_scope (this);
}

void
model_class::visit (visitor *v)
{
  v->visit_class (this, descriptor, name);
}
