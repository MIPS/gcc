// A parameterized class instance.

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
model_class_instance::resolve_member_hook (resolution_scope *scope)
{
  parent->resolve_members ();

  // Create member classes.
  for (std::map<std::string, ref_class>::const_iterator i
	 = parent->member_classes.begin ();
       i != parent->member_classes.end ();
       ++i)
    {
      // Don't parameterize static members.
      ref_class mem = (*i).second;
      if (! mem->static_p ())
	mem = mem->apply_type_map (this, type_map);
      member_classes[(*i).first] = mem;
    }

  // Create fields.
  for (std::list<ref_field>::const_iterator i = parent->fields.begin ();
       i != parent->fields.end ();
       ++i)
    {
      // Don't parameterize static members.
      ref_field fld = *i;
      if (! fld->static_p ())
	fld = assert_cast<model_field *> (fld->apply_type_map (type_map,
							       this));
      fields.push_back (fld);
    }

  // Create methods.
  for (std::list<ref_method>::const_iterator i = parent->methods.begin ();
       i != parent->methods.end ();
       ++i)
    {
      // Don't parameterize static members.
      ref_method meth = *i;
      if (! meth->static_p ())
	meth = meth->apply_type_map (type_map, this);
      methods.push_back (meth);
    }
}

void
model_class_instance::get_type_map (std::list<model_class *> &result)
{
  for (std::list<ref_type_variable>::const_iterator i
	 = type_parameters.begin ();
       i != type_parameters.end ();
       ++i)
    result.push_back (type_map.find ((*i).get ()));
}

model_class *
model_class_instance::apply_type_map (model_element *request,
				      const model_type_map &other_type_map)
{
  bool any_changed = false;
  std::list<model_class *> new_params;
  for (std::list<ref_type_variable>::const_iterator i
	 = type_parameters.begin ();
       i != type_parameters.end ();
       ++i)
    {
      model_class *mapping = type_map.find ((*i).get ());
      model_class *xform = mapping->apply_type_map (request, other_type_map);
      if (xform != mapping)
	any_changed = true;
      new_params.push_back (xform);
    }

  // If re-parameterizing didn't change any arguments, then don't
  // bother making a new instance.
  return any_changed ? parent->create_instance (request, new_params) : this;
}

std::string
model_class_instance::get_signature_map_fragment ()
{
  assert (! type_map.empty_p ());
  std::string result = "<";

  for (std::list<ref_type_variable>::const_iterator i
	 = type_parameters.begin ();
       i != type_parameters.end ();
       ++i)
    {
      model_type_variable *var = (*i).get ();
      model_class *k = type_map.find (var);
      assert (k);
      result += k->get_signature ();
    }

  result += ">";
  return result;
}

std::string
model_class_instance::get_pretty_name () const
{
  // FIXME: should share some code with superclass.
  std::string result;
  if (declaring_class)
    result = declaring_class->get_pretty_name () + "$" + get_assigned_name ();
  else
    {
      std::string cu
	= compilation_unit->get_package ()->get_fully_qualified_name ();
      result = (cu.empty ()) ? name : cu + "." + name;
    }

  result += "<";
  bool first = true;
  for (std::list<ref_type_variable>::const_iterator i
	 = type_parameters.begin ();
       i != type_parameters.end ();
       ++i)
    {
      model_class *arg = type_map.find ((*i).get ());
      if (! first)
	result += ", ";
      first = false;
      result += arg->get_pretty_name ();
    }
  result += ">";

  return result;
}

void
model_class_instance::visit (visitor *v)
{
  v->visit_class_instance (this, descriptor, name, parent);
}
