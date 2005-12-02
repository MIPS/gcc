// Member references.

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
model_memberref_base::resolve_as_enum_constant (resolution_scope *scope,
						model_class *klass,
						const std::string &field_name)
{
  std::set<model_field *> result;
  klass->find_members (field_name, result, scope->get_current_class (), NULL);
  if (result.empty ())
    throw error ("no %<enum%> constant named %1 in %2")
      % field_name % klass;
  else if (result.size () > 1)
    throw error ("ambiguous reference to %<enum%> constant named %1 in %2")
      % field_name % klass;

  ref_field_ref rf = new model_field_ref (get_location ());
  rf->set_field (field_name);
  rf->set_field (*(result.begin ()));

  real = rf;
  set_type (klass);
}

void
model_memberref_forward::resolve (resolution_scope *scope)
{
  if (is_call)
    {
      assert (! is_lhs);
      real = classify_expression_name (scope, this, ids, arguments,
				       type_parameters);
      if (ids.size () == 1)
	{
	  model_invocation_base *iv
	    = assert_cast<model_invocation_base *> (real.get ());
	  iv->set_unqualified ();
	}
    }
  else
    {
      assert (type_parameters.empty ());
      real = classify_expression_name (scope, this, ids);
      if (dynamic_cast<model_field_ref *> (real.get ()) != NULL)
	{
	  model_field_ref *ref = assert_cast<model_field_ref *> (real.get ());
	  ref->set_qualified (ids.size () > 1);
	}
      if (is_lhs)
	real->set_left_hand_side (is_compound);
    }
  assert (real);
  if (assign_conv_type)
    real->use_assignment_conversion (assign_conv_type);
  real->resolve (scope);
  set_type (real->type ());
}

void
model_memberref_forward::resolve_as_enum_constant (resolution_scope *scope,
						   model_enum *enum_class)
{
  if (is_call || ids.size () != 1)
    throw error ("%<enum%> case expression must be an identifier");
  assert (type_parameters.empty ());
  model_memberref_base::resolve_as_enum_constant (scope, enum_class,
						  ids.back ());
}

void
model_memberref_enum::resolve (resolution_scope *scope)
{
  base_type->resolve (scope);
  if (! base_type->type ()->enum_p ())
    throw error ("type %1 not an %<enum%>") % base_type->type ();

  model_class *klass = assert_cast<model_class *> (base_type->type ());
  resolve_as_enum_constant (scope, klass, field_name);
}

void
model_memberref_enum::visit (visitor *v)
{
  v->visit_memberref_enum (this, base_type, field_name);
}
