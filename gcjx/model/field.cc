// Fields.

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


model_field::model_field (const ref_variable_decl &vardecl)
  : model_variable_decl (vardecl->get_location (),
			 vardecl->get_name (),
			 vardecl->get_declared_type (),
			 vardecl->get_declaring_class ()),
    IModifiable (),
    state (0)
{
  set_initializer (vardecl->get_initializer ());
}

model_field::model_field (const location &w, const std::string &n,
			  const ref_forwarding_type &t,
			  model_class *decl)
  : model_variable_decl (w, n, t, decl),
    IModifiable (),
    state (0)
{
}

bool
model_field::constant_p ()
{
  require_resolution ();
  return (final
	  && constant_type_p (decltype->type ())
	  && initializer
	  && initializer->constant_p ());
}

bool
model_field::inlineable_p ()
{
  require_resolution ();
  return has_initializer_p () && final_p () && initializer->constant_p ();
}

model_type *
model_field::type () const
{
  if (state < 1
      && ! declaring_class->local_p ()
      && declaring_class->get_compilation_unit ())
    {
      resolution_scope scope;
      declaring_class->push_on_scope (&scope);
      model_field *unconst_this = const_cast<model_field *> (this);
      resolution_scope::push_warnings warn_holder (&scope, unconst_this);
      unconst_this->resolve_classes (&scope);
    }

  return model_variable_decl::type ();
}

void
model_field::resolve_classes (resolution_scope *scope)
{
  if (state < 1)
    {
      state = 1;
      resolution_scope::push_warnings warn_holder (scope, this);
      model_variable_decl::resolve_classes (scope);
    }
}

void
model_field::check_serialization_fields ()
{
  warning_state state
    = global->get_compiler ()->warn_bad_serialization_field ();
  assert (state);

  if (name == "serialPersistentFields")
    {
      model_type *io
	= global->get_compiler ()->java_io_ObjectStreamField ()->array ();
      if (type () != io)
	std::cerr << warn (state, "field %1 should be of type %2")
	  % this % io;
      if ((modifiers & (ACC_PRIVATE | ACC_STATIC | ACC_FINAL))
	  != (ACC_PRIVATE | ACC_STATIC | ACC_FINAL))
	std::cerr << warn (state,
			   "field %1 should be %<private static final%>")
	  % this;
    }

  if (name == "serialVersionUID")
    {
      if (type () != primitive_long_type)
	std::cerr << warn (state, "field %1 should be of type %<long%>")
	  % this;
      if (! declaring_class->interface_p ()
	  && ((modifiers & (ACC_PRIVATE | ACC_STATIC | ACC_FINAL))
	      != (ACC_PRIVATE | ACC_STATIC | ACC_FINAL)))
	std::cerr << warn (state,
			   "field %1 should be %<private static final%>")
	  % this;
    }

  if ((name == "serialPersistentFields" || name == "serialVersionUID")
      && ! global->get_compiler ()->java_io_Serializable ()->assignable_from_p (declaring_class))
    std::cerr << warn (state, "field %1 declared in class which "
		       "is not serializable")
      % this;
}

void
model_field::resolve (resolution_scope *scope)
{
  if (state < 2)
    {
      state = 2;
      resolution_scope::push_warnings warn_holder (scope, this);
      model_variable_decl::resolve (scope);

      if (scope->warn_bad_serialization_field ())
	check_serialization_fields ();
    }
}

void
model_field::check_deprecated (model_element *request)
{
  if (deprecated && global->get_compiler ()->warn_deprecated ())
    // FIXME: include class name ...
    std::cerr << request->warn (global->get_compiler ()->warn_deprecated (),
				"field %1 is deprecated")
      % name;
}

void
model_field::check_referenced (resolution_scope *scope)
{
  warning_scope::push_warnings push (scope, this);
  // We only emit this warning for private fields, as others can be
  // referenced from elsewhere.
  if ((modifiers & ACC_PRIVATE) != 0
      && ! used
      // These private fields are referenced magically by
      // serialization.
      && name != "serialVersionUID"
      && name != "serialPersistentFields"
      && scope->warn_unused_private_members ())
    std::cerr << warn (global->get_compiler ()->warn_unused_private_members (),
		       "private field %1 not used in its scope")
      % this;
}

model_variable_decl *
model_field::apply_type_map (const model_type_map &type_map,
			     model_class *request)
{
  model_type *self_type = type ();
  if (self_type->primitive_p ())
    return this;

  model_class *k = assert_cast<model_class *> (self_type);
  model_class *param = k->apply_type_map (request, type_map);
  // If our type didn't change, don't bother creating a new field.
  if (param == k)
    return this;
  ref_forwarding_type nt
    = new model_forwarding_resolved (get_location (), param);
  return new model_field (get_location (), name, nt, request);
}

model_variable_decl *
model_field::erasure ()
{
  model_type *self_type = type ();
  if (self_type->primitive_p ())
    return this;

  model_class *param = assert_cast<model_class *> (self_type->erasure ());
  // If our type didn't change, don't bother creating a new field.
  if (param == self_type)
    return this;
  ref_forwarding_type nt
    = new model_forwarding_resolved (get_location (), param);
  return new model_field (get_location (), name, nt,
			  assert_cast<model_class *> (declaring_class->erasure ()));
}

void
model_field::require_resolution ()
{
  if (state < 2)
    {
      resolution_scope scope;
      declaring_class->push_on_scope (&scope);

      model_static_context_scope sscope (static_p ());
      resolution_scope::push_iscope holder (&scope, &sscope);
      resolve (&scope);
    }
}

void
model_field::visit (visitor *v)
{
  v->visit_field (this);
}
