// Represent a constructor.

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
#include "dump.hh"

model_constructor::model_constructor (model_constructor *other,
				      const model_type_map &type_map,
				      model_class *enclosing)
  : model_method (other, type_map, enclosing)
{
  if (other->this0)
    {
      this0 = new model_variable_decl (get_location (),
				       // FIXME: construct new name
				       "arg$this$0",
				       other->this0->get_declared_type (),
				       declaring_class);
    }
}

model_constructor::model_constructor (model_constructor *other,
				      model_class *enclosing)
  : model_method (other, enclosing)
{
  if (other->this0)
    {
      model_type *era = 
	other->this0->get_declared_type ()->type ()->erasure ();
      ref_forwarding_type rtt
	= new model_forwarding_resolved (get_location (), era);
      this0 = new model_variable_decl (get_location (),
				       // FIXME: construct new name
				       "arg$this$0",
				       rtt,
				       declaring_class);
    }
}

model_constructor::model_constructor (model_constructor *other)
  : model_method (other->get_location (), other->get_declaring_class ()),
    other_this (NULL)
{
}

void
model_constructor::massage_modifiers (const ref_modifier_list &mods)
{
  mods->check (this, ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE,
	       "invalid modifier %1 for constructor declaration");
  // Methods have access modifiers, so check validity of these.
  mods->access_check (this, "constructor declared %1 also declared %2");
}

void
model_constructor::resolve_classes (resolution_scope *scope)
{
  // If we're a parameterized instance, we might be created
  // pre-resolved.
  if (this0)
    return;

  if (! from_class
      && declaring_class->inner_p ()
      && ! declaring_class->static_context_p ())
    {
      ref_forwarding_type t
	= new model_forwarding_resolved (get_location (),
					 declaring_class->get_lexically_enclosing_class ());
      this0 = new model_variable_decl (get_location (),
				       // FIXME: construct new name
				       "arg$this$0",
				       t,
				       declaring_class);
      parameters.push_front (this0);

      // Must reset this in case it was used before.
      descriptor = "";
    }

  model_method::resolve_classes (scope);
}

void
model_constructor::resolve (resolution_scope *scope)
{
  if (state == RESOLVED)
    return;

  model_method::resolve (scope);

  if (this0 && other_this == NULL)
    {
      // FIXME: much of this code is duplicated in class.cc.

      // We need to initialize the 'this$0' field.
      ref_field this0f = declaring_class->get_this_0 ();

      ref_field_ref lhs = new model_field_ref (get_location ());
      lhs->set_field (this0f->get_name ());
      lhs->set_field (this0f.get ());

      ref_simple_variable_ref rhs
	= new model_simple_variable_ref (get_location (), this0.get ());

      ref_assignment assign = new model_assignment (get_location ());
      assign->set_lhs (lhs);
      assign->set_rhs (rhs);
      ref_expression_stmt stmt = new model_expression_stmt (get_location (),
							    assign);

      std::list<ref_stmt> statements;
      statements.push_back (stmt);
      prepend_statements (scope, statements);
    }
}

void
model_constructor::prepend_statements (resolution_scope *scope,
				       std::list<ref_stmt> &statements,
				       bool do_resolve)
{
  // Resolve the new statements.
  if (do_resolve)
    {
      resolution_scope::push_warnings warn_holder (scope, this);
      resolution_scope::push_iscope tp_holder (scope, &type_parameters);
      resolution_scope::push_iscope holder (scope, this);

      model_block_scope vscope (scope);
      resolution_scope::push_iscope var_holder (scope, &vscope);

      ::resolve (scope, statements);
    }

  statements.push_back (body);

  // Note that we rely on the fact that a block needs no
  // resolution of its own.
  ref_block new_body = new model_block (get_location ());
  new_body->set_statements (statements);

  // Make sure the normal completion flags are set.
  if (do_resolve)
    {
      normal_completion_state ncs;
      new_body->compute_normal_completion (ncs);
    }

  body = new_body;
}

void
model_constructor::add_parameter (const ref_variable_decl &param)
{
  parameters.push_front (param);
  // Must reset this in case it was used before.
  descriptor = "";
}

void
model_constructor::set_other_this (model_constructor *other,
				   model_element *request)
{
  assert (! other_this);

  if (this == other)
    throw request->error ("recursive %<this%> invocation");

  // We maintain an invariant that there are no cycles in the graph of
  // methods connected by their 'other_this' pointers.  This lets us
  // be relatively naive here without danger.  We also know that if a
  // constructor has not been resolved, its 'other_this' pointer is
  // NULL.  That is ok because if an unresolved constructor
  // potentially contributes to a cycle, that will be detected when it
  // is resolved.
  model_constructor *iter = other;
  while (iter && iter != this)
    iter = iter->other_this;

  if (this == iter)
    // FIXME: we ought to include the whole path here.
    throw error ("recursive %<this%> invocation via other constructors");

  other_this = other;
}

model_method *
model_constructor::apply_type_map (const model_type_map &type_map,
				   model_class *enclosing)
{
  model_method *result = instance_cache.find_instance (type_map);
  if (result == NULL)
    {
      result = new model_constructor (this, type_map, enclosing);
      instance_cache.add_instance (type_map, result);
    }
  return result;
}

model_method *
model_constructor::erasure (model_class *enclosing)
{
  model_method *result = instance_cache.find_erased_instance ();
  if (result == NULL)
    {
      result = new model_constructor (this, enclosing);
      instance_cache.add_erased_instance (result);
    }
  return result;
}

void
model_constructor::visit (visitor *v)
{
  v->visit_constructor (this, parameters, body);
}
