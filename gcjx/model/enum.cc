// Represent an enum class.

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

model_enum::model_enum (const location &w)
  : model_class (w),
    ordinal (0)
{
  implicit_modifier (ACC_STATIC);
}

void
model_enum::massage_modifiers (const ref_modifier_list &mods)
{
  if (! declaring_class)
    {
      mods->check (this, (ACC_PUBLIC | ACC_STRICTFP),
		   "invalid modifier %1 for top-level enum declaration");
    }
  else
    {
      mods->check (this, (ACC_PUBLIC | ACC_PROTECTED | ACC_STATIC
			  | ACC_PRIVATE | ACC_STRICTFP),
		   "invalid modifier %1 for member enum declaration");
      implicit_modifier (ACC_STATIC);
    }

  if (local)
    std::cerr << error ("local enum is invalid");

  // This seems to suffice to get us the required semantics for
  // abstract methods.  FIXME: but it shows up in the class file.
  // FIXME: and we could have ACC_FINAL as well -- a contradiction.
  implicit_modifier (ACC_ABSTRACT);

  bool has_body = false;
  for (std::list<ref_enum_constant>::const_iterator i = constants.begin ();
       i != constants.end ();
       ++i)
    {
      if ((*i)->has_body_p ())
	{
	  has_body = true;
	  break;
	}
    }

  // FIXME
//   if (! has_body)
//     implicit_modifier (ACC_FINAL);

  mods->access_check (this, "class declared %1 also declared %2");
}

void
model_enum::add_enum (const ref_enum_constant &new_constant)
{
  ref_forwarding_type self_type
    = new model_forwarding_resolved (get_location (), this);

  new_constant->set_declaring_class (this);
  new_constant->set_superclass (self_type);
  new_constant->set_ordinal (ordinal);
  constants.push_back (new_constant);
  ++ordinal;

  // An enum constant is just a field with an initializer.  It is
  // simplest to just directly model it as such.
  ref_field field = new model_field (new_constant->get_location (),
				     new_constant->get_name (),
				     self_type,
				     this);
  field->set_modifiers (ACC_PUBLIC | ACC_STATIC | ACC_FINAL);

  ref_expression name
    = new model_string_literal (new_constant->get_location (),
				new_constant->get_name ());
  ref_expression ord
    = new model_int_literal (new_constant->get_location (),
			     jint (new_constant->get_ordinal ()));

  model_class *what = (new_constant->has_body_p () ?
		       (model_class *) new_constant.get ()
		       : (model_class *) this);
  ref_new init = new model_new_enum (new_constant->get_location (), what);
  if (new_constant->has_body_p ())
    init->set_anonymous (new_constant);

  std::list<ref_expression> init_args = new_constant->get_arguments ();
  init_args.push_front (ord);
  init_args.push_front (name);
  init->set_arguments (init_args);

  field->set_initializer (init);

  add (field);
}

void
model_enum::check_instantiation (model_element *request)
{
  throw request->error ("can't create instance of enum type %1")
    % this;
}

void
model_enum::resolve_classes_hook (resolution_scope *scope)
{
  if (declaring_class && declaring_class->inner_p ())
    std::cerr << error ("enum invalid in inner class %1")
      % declaring_class;

  location w = get_location ();

  set_superclass (new model_forwarding_resolved (w,
						 global->get_compiler ()->java_lang_Enum ()));

  // FIXME: add values() and valueOf().

  // Add the 'VALUES' field.
  model_class *list = global->get_compiler ()->java_util_List ();
  ref_forwarding_type list_wrap = new model_forwarding_resolved (w, list);
  ref_field values = new model_field (w, "VALUES", list_wrap, this);
  // fixme final as well?
  // fixme an "immutable List", so we need our own subclass...
  values->set_modifiers (ACC_STATIC);
  add (values);
}

void
model_enum::add_enum_arguments (model_constructor *cons)
{
  location where = get_location ();
  // FIXME: name of synthetic variable should not clash.
  ref_variable_decl name
    = new model_parameter_decl (where, "$name",
				new model_forwarding_resolved (where,
							       global->get_compiler ()->java_lang_String ()),
				this);
  // FIXME: name of synthetic variable should not clash.
  ref_variable_decl ordinal
    = new model_parameter_decl (where, "$ordinal",
				new model_forwarding_resolved (where,
							       primitive_int_type),
				this);

  cons->add_parameter (ordinal);
  cons->add_parameter (name);
}

ref_method
model_enum::add_implicit_constructor ()
{
  ref_method result = model_class::add_implicit_constructor ();

  // For an enum, the implicit constructor is always private.
  modifier_t mods = result->get_modifiers ();
  mods &= ~ACC_ACCESS;
  mods |= ACC_PRIVATE;
  result->set_modifiers (mods);

  add_enum_arguments (assert_cast<model_constructor *> (result.get ()));
  return result;
}

void
model_enum::resolve_member_hook (resolution_scope *scope)
{
  // Add the special arguments to each constructor in the class.
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      if ((*i)->constructor_p ())
	add_enum_arguments (assert_cast<model_constructor *> ((*i).get ()));
    }
}

void
model_enum::visit (visitor *v)
{
  v->visit_enum (this, descriptor, name, constants);
}
