// Import statements.

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



bool
model_import::java_lang_p (Iname *name)
{
  // This seems simplest.
  return name == global->get_compiler ()->java_lang_Object ()->get_package ();
}

void
model_import::check_referenced ()
{
  if (! used && global->get_compiler ()->warn_unused_import ())
    // FIXME: the text of the import...?
    std::cerr << warn (global->get_compiler ()->warn_unused_import (),
		       "unused %<import%> statement");
}



void
model_import_single::resolve (resolution_scope *scope)
{
  if (resolved_type)
    return;

  resolved_type = classify_type_name (scope, this, name);
  if (! resolved_type)
    throw error ("%1 does not name a class or interface type") % name;

  if (scope->warn_java_lang_import ()
      && java_lang_p (resolved_type->get_package ()))
    std::cerr << warn (global->get_compiler ()->warn_java_lang_import (),
		       "explicit import of %<java.lang%>");
}

model_class *
model_import_single::find_class (const std::string &n)
{
  // Should never be called.
  abort ();
}

void
model_import_single::visit (visitor *v)
{
  v->visit_import_single (this, name, resolved_type);
}



void
model_import_on_demand::resolve (resolution_scope *scope)
{
  if (! resolved_type)
    resolved_type = classify_package_or_type_name (scope, this, name);

  if (! implicit
      && scope->warn_java_lang_import ()
      && java_lang_p (resolved_type))
    std::cerr << warn (global->get_compiler ()->warn_java_lang_import (),
		       "explicit import of %<java.lang%>");
}

model_class *
model_import_on_demand::find_class (const std::string &n)
{
  // FIXME: this is super bogus, we need a new method on Iname.
  // Or perhaps Iname::find_name should mean "try to load", but that
  // probably implies changes elsewhere...
  model_class *result;
  if (resolved_type->package_p ())
    {
      model_package *p = assert_cast<model_package *> (resolved_type);
      result = p->find_class (n);
    }
  else
    {
      Iname *r = resolved_type->find_name (n);

      if (! r)
	return NULL;

      if (r->package_p ())
	{
	  std::cerr << error ("expected class but found package named %1")
	    % assert_cast<model_package *> (r);
	  return NULL;
	}

      result = assert_cast<model_class *> (r);
    }

  return result;
}

void
model_import_on_demand::visit (visitor *v)
{
  v->visit_import_on_demand (this, name, resolved_type, implicit);
}



// FIXME: reuse model_import_single::resolve
void
model_static_import_base::resolve (resolution_scope *scope)
{
  if (resolved_type)
    return;

  resolved_type = classify_type_name (scope, this, name);
  if (! resolved_type)
    throw error ("%1 does not name a class or interface type")
      % join (name, '.');
}

model_class *
model_static_import_base::search_for_class (const std::string &name) const
{
  std::set<model_class *> members;
  resolved_type->find_members (name, members, NULL);

  std::set<model_class *> static_members;
  for (std::set<model_class *>::const_iterator i = members.begin ();
       i != members.end ();
       ++i)
    {
      if ((*i)->static_p ())
	static_members.insert (*i);
    }

  // FIXME: what if there is more than one class available for import?
  if (static_members.size () != 1)
    return NULL;
  return *(static_members.begin ());
}

void
model_static_import_base::find_field (const std::string &name,
				      IContext *context,
				      std::set<model_variable_decl *> &result)
{
  std::set<model_field *> members;
  resolved_type->find_members (name, members, context, NULL);

  for (std::set<model_field *>::const_iterator i = members.begin ();
       i != members.end ();
       ++i)
    {
      if ((*i)->static_p ())
	result.insert (*i);
    }
}

void
model_static_import_base::find_method (const std::string &name,
				       IContext *context,
				       std::set<model_method *> &accessible)
{
  std::set<model_method *> members;
  resolved_type->find_members (name, members, context, NULL);

  for (std::set<model_method *>::const_iterator i = members.begin ();
       i != members.end ();
       ++i)
    {
      if ((*i)->static_p ())
	accessible.insert (*i);
    }
}



model_class *
model_static_import_single::get_class_declaration () const
{
  return search_for_class (member_name);
}

void
model_static_import_single::resolve (resolution_scope *scope)
{
  model_static_import_base::resolve (scope);

  // Make sure we imported something.
  // FIXME: don't we need to use the canonical name?

  {
    std::set<model_variable_decl *> fields;
    find_field (member_name, scope->get_current_context (), fields);
    if (! fields.empty ())
      return;
  }

  {
    std::set<model_method *> methods;
    find_method (member_name, scope->get_current_context (), methods);
    if (! methods.empty ())
      return;
  }

  if (get_class_declaration () != NULL)
    return;

  // If we got here, there's no member.
  throw error ("%1 does not name a static member in %2")
    % member_name % resolved_type;
}

model_class *
model_static_import_single::find_class (const std::string &name)
{
  // Should never be called.
  abort ();
}

void
model_static_import_single::find_field (const std::string &name,
					IContext *context,
					std::set<model_variable_decl *> &result)
{
  if (name == member_name)
    model_static_import_base::find_field (name, context, result);
}

void
model_static_import_single::find_method (const std::string &name,
					 IContext *context,
					 std::set<model_method *> &accessible)
{
  if (name == member_name)
    model_static_import_base::find_method (name, context, accessible);
}

void
model_static_import_single::visit (visitor *v)
{
  v->visit_static_import_single (this, name, resolved_type, member_name);
}



model_class *
model_static_import_on_demand::find_class (const std::string &name)
{
  return search_for_class (name);
}

void
model_static_import_on_demand::visit (visitor *v)
{
  v->visit_static_import_on_demand (this, name, resolved_type);
}
