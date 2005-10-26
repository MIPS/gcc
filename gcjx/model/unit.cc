// Compilation unit.

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
model_unit_source::check_dups (const std::string &sname,
			       model_import *imp,
			       model_class *elt,
			       name_map_type &dups)
{
  name_map_type::const_iterator it
    = dups.find (sname);
  if (it == dups.end ())
    dups[sname] = std::make_pair (imp, elt);
  else
    {
      if (elt == (*it).second.second)
	{
	  if (! global->get_compiler ()->pedantic ()) // FIXME: what flag?
	    {
	      // FIXME: this will be wrong if 'second.first == NULL'
	      // FIXME: put on a warning flag, not 'pedantic'
	      std::cerr << imp->warn ("duplicate simple name %1 "
				      "in single-type import")
		% sname;
	      std::cerr
		<< (*it).second.first->warn ("original declaration is here");
	    }
	  return;
	}
      // FIXME: this will be wrong if 'second.first == NULL'
      std::cerr << error ("duplicate declaration of %1") % sname;
      std::cerr << (*it).second.first->error ("original declaration is here");
    }
}

void
model_unit_source::resolve (resolution_scope *scope)
{
  if (resolved)
    return;

  // Initialize the simple name map.  We will resolve elements from
  // this map on demand.
  for (std::list<ref_import>::const_iterator i = imports.begin ();
       i != imports.end ();
       ++i)
    {
      if ((*i)->single_import_p ())
	simple_name_map.insert (std::make_pair ((*i)->get_simple_name (),
						(*i).get ()));
    }

  resolved = true;
}

void
model_unit_source::resolve (resolution_scope *scope, model_import *imp)
{
  resolving = true;
  imp->resolve (scope);
  resolving = false;
}

void
model_unit_source::look_up_name (const std::string &name,
				 std::set<model_class *> &result,
				 IContext *context,
				 model_class *)
{
  // Check single-type-import declarations, static single-type-import
  // declarations that resolve to a member type, and types defined in
  // this compilation unit.  However, we only want to do if we've been
  // resolved.  If RESOLVING is set then we are resolving an import
  // statement; in this case we don't want to use this code as it
  // would lead to infinite recursion, and anyway other imports should
  // not be in scope.
  if (resolved && ! resolving)
    {
      for (std::multimap<std::string, model_import *>::const_iterator i
	     = simple_name_map.begin ();
	   i != simple_name_map.end ();
	   ++i)
	{
	  if (name != (*i).first)
	    continue;
	  model_import *imp = (*i).second;
	  resolve (&local_scope, imp);
	  model_class *klass = imp->get_class_declaration ();
	  if (accessible_p (klass, context))
	    {
	      imp->set_used ();
	      result.insert (klass);
	      return;
	    }
	}
    }

  // Look at other compilation units in this package.  Note that we
  // don't pass in the context, as we know that these will always be
  // accessible.
  model_class *pk = package->find_class (name);
  if (pk != NULL && accessible_p (pk, context))
    {
      result.insert (pk);
      return;
    }

  // If we haven't been resolved yet, we won't find anything by
  // asking the imports, since they won't have been hooked up.
  if (! resolved || resolving)
    return;

  // Finally look at on-demand imports.
  model_import *result_import = NULL;
  for (std::list<ref_import>::const_iterator i = imports.begin ();
       i != imports.end ();
       ++i)
    {
      resolve (&local_scope, (*i).get ());
      if (! (*i)->single_import_p ())
	{
	  // FIXME: imports should probably cache negative results.
	  model_class *r2 = (*i)->find_class (name);

	  if (r2 && accessible_p (r2, context))
	    {
	      (*i)->set_used ();
	      if (pk != NULL && pk != r2)
		// FIXME: should have the location passed in so we
		// can give a good error.
		// FIXME: should postpone this error for our caller,
		// who already has to handle it anyway.
		throw ambiguous_type_error (get_location (),
					    "reference to ambiguous type %1"
					    " duplicated in packages %2"
					    " and %3")
		  % name % (*i)->get_name () % result_import->get_name ();
	      pk = r2;
	      result_import = (*i).get ();
	    }
	}
    }

  if (pk != NULL)
    result.insert (pk);
}

void
model_unit_source::look_up_name (const std::string &name,
				 std::set<model_variable_decl *> &result,
				 IContext *context,
				 model_class *)
{
  // Look at single imports first, then on-demand imports.
  for (int pass = 0; pass < 2; ++pass)
    {
      for (std::list<ref_import>::const_iterator i = imports.begin ();
	   i != imports.end ();
	   ++i)
	{
	  if ((*i)->single_import_p () == (pass == 1))
	    continue;

	  (*i)->resolve (&local_scope);
	  (*i)->find_field (name, context, result);
	  if (! result.empty ());
	  return;
	}
    }
}

void
model_unit_source::look_up_name (const std::string &name,
				 std::set<model_method *> &accessible,
				 IContext *context,
				 model_class *)
{
  // Look at single imports first, then on-demand imports.
  for (int pass = 0; pass < 2; ++pass)
    {
      for (std::list<ref_import>::const_iterator i = imports.begin ();
	   i != imports.end ();
	   ++i)
	{
	  if ((*i)->single_import_p () == (pass == 1))
	    continue;

	  (*i)->resolve (&local_scope);
	  (*i)->find_method (name, context, accessible);
	  if (! accessible.empty ())
	    return;
	}
    }
}

void
model_unit_source::check_imports ()
{
  name_map_type local_name_map;

  // Resolve the imports.
  for (std::list<ref_import>::const_iterator i = imports.begin ();
       i != imports.end ();
       ++i)
    {
      resolve (&local_scope, (*i).get ());
      // This will be NULL if the import doesn't resolve to a single
      // class.
      model_class *k = (*i)->get_class_declaration ();
      if (k != NULL && accessible_p (k, package))
	check_dups (k->get_name (), (*i).get (), k, local_name_map);
    }

  // Check for duplicates among the types.
  model_class *pub = NULL;
  for (std::list<ref_class>::const_iterator i = types.begin ();
       i != types.end ();
       ++i)
    {
      std::string sname = (*i)->get_name ();
      check_dups (sname, NULL, (*i).get (), local_name_map);
      if (((*i)->get_modifiers () & ACC_PUBLIC) != 0)
	{
	  if (pub)
	    std::cerr << (*i)->error ("%1 cannot be public in a compilation "
	                              "unit already containing "
				      "public class %2")
	      % (*i)->get_pretty_name () % pub->get_pretty_name ();
	  else
	    pub = (*i).get ();
	}
    }

  // Check to see if each import is used.
  if (! global->get_compiler ()->warn_unused_import ())
    return;

  for (std::list<ref_import>::const_iterator i = imports.begin ();
       i != imports.end ();
       ++i)
    (*i)->check_referenced ();
}

void
model_unit_source::visit (visitor *v)
{
  v->visit_unit_source (this, package, types, filename, resolved, imports);
}



model_class *
model_unit_class::find_top_level_class (resolution_scope *scope,
					const std::string &descriptor,
					bool side_effects)
{
  // In a .class file names are separated with "/".
  std::list<std::string> parts = split (descriptor, '/');

  model_package *pkg;
  if (parts.size () > 1)
    pkg = classify_package_name (this, drop_last_element (parts));
  else
    pkg = global->get_compiler ()->get_primordial_package ();

  // Some compilers will omit things from InnerClasses, so we can wind
  // up with a spurious failure here.  We attempt to detect that
  // situation and cope.
  std::string simple = get_simple_name (parts);
  model_class *result = pkg->find_class (simple);
  if (side_effects || result != NULL)
    return result;

  // A '$' denotes an inner class.
  parts = split (simple, '$');
  if (parts.size () <= 1)
    return NULL;

  IContext *ctxt = scope->get_current_class ();
  if (ctxt == NULL)
    ctxt = scope->get_compilation_unit ()->get_package ();

  bool first = true;
  for (std::list<std::string>::const_iterator i = parts.begin ();
       i != parts.end ();
       ++i)
    {
      if (first)
	{
	  result = pkg->find_class (*i);
	  first = false;
	}
      else
	{
	  // FIXME: duplicated code with find_class_from_descriptor().
	  std::set<model_class *> members;
	  result->find_members (*i, members, ctxt);

	  if (members.size () == 0)
	    result = NULL;
	  else if (members.size () == 1)
	    result = *(members.begin ());
	  else
	    throw error ("reference to class descriptor %1 is ambiguous")
	      % descriptor;
	}

      if (result == NULL)
	break;
    }

  return result;
}

void
model_unit_class::resolve (resolution_scope *scope)
{
  if (! resolved)
    {
      resolved = true;

      assert (types.size () == 1);
      model_class *our_class = types.front ().get ();
      std::string our_name (our_class->get_fully_qualified_name_with_slashes ());

      // Load every class file that references us as an outer class.
      // These will be added to our class as a side effect.
      for (std::map<std::string, inner_class>::const_iterator i
	     = inners.begin ();
	   i != inners.end ();
	   ++i)
	{
	  const inner_class &ic = (*i).second;
	  if (! ic.plain.empty () && ic.outer == our_name)
	    {
	      // Invoke this for side effects.
	      find_top_level_class (scope, (*i).first, true);
	    }
	}
    }
}

model_class *
model_unit_class::find_class_from_descriptor (resolution_scope *scope,
					      model_element *request,
					      const std::string &descriptor)
{
  if (inners.find (descriptor) == inners.end ())
    return find_top_level_class (scope, descriptor);

  inner_class &rec (inners[descriptor]);

  if (rec.plain.empty () || rec.outer.empty ())
    {
      // Anonymous or local class.  In this case it should be true
      // that the class we're looking for must come from the same
      // source compilation unit as the current class.  Or, so we
      // hope.  On this theory we don't look for a Java source file
      // (what would it be named?) but instead look for a class file
      // directly.
      abort ();
    }
  else
    {
      // Member class.  Find the containing class, then ask it.
      model_class *outer = find_class_from_descriptor (scope, request,
						       rec.outer);

      IContext *ctxt = scope->get_current_class ();
      if (ctxt == NULL)
	ctxt = scope->get_compilation_unit ()->get_package ();

      std::set<model_class *> result;
      outer->find_members (rec.plain, result, ctxt);

      if (result.size () == 0)
	return NULL;
      if (result.size () == 1)
	return *(result.begin ());

      throw request->error ("reference to class descriptor %1 is ambiguous")
	% descriptor;
    }
}

void
model_unit_class::visit (visitor *v)
{
  v->visit_unit_class (this, package, types, filename, resolved);
}



model_class *
model_unit_fake::find_class_from_descriptor (resolution_scope *,
					     model_element *request,
					     const std::string &descriptor)
{
  std::map<std::string, model_class *>::const_iterator i
    = descriptor_map.find (descriptor);
  if (i == descriptor_map.end ())
    throw request->error ("couldn't find class with descriptor %1")
      % descriptor;
  return (*i).second;
}

void
model_unit_fake::visit (visitor *v)
{
  v->visit_unit_fake (this, package, types, filename, resolved);
}
