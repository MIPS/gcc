// Represent a method's 'throws' clause.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

// FIXME this should probably be a method on model_type.
// FIXME duplicated in method.cc
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

model_throws_clause::model_throws_clause (const model_throws_clause &other,
					  model_element *request,
					  const model_type_map &type_map)
{
  for (std::list<ref_forwarding_type>::const_iterator i = other.decls.begin ();
       i != other.decls.end ();
       ++i)
    decls.push_back (wrap ((*i)->type (), request, type_map));
}

model_throws_clause::model_throws_clause (const std::set<model_method *> &methods)
{
  // The rule in the JLS says that the same exception must be declared
  // in each method.  However, we follow the Java Spec Report here,
  // which points out that it suffices for some superclass to be
  // included in all the 'throws' clauses.  Unfortunately this is more
  // expensive to compute.

  std::set<model_type *> intersection;
  std::set<model_type *> checked;
  for (std::set<model_method *>::const_iterator iter = methods.begin ();
       iter != methods.end ();
       ++iter)
    {
      std::set<model_type *> next_throws = (*iter)->get_throws_as_set ();
	  
      for (std::set<model_type *>::const_iterator it = next_throws.begin ();
	   it != next_throws.end ();
	   ++it)
	{
	  if (checked.find (*it) != checked.end ())
	    continue;
	  checked.insert (*it);

	  bool ok = true;
	  for (std::set<model_method *>::const_iterator inner
		 = methods.begin ();
	       inner != methods.end ();
	       ++inner)
	    {
	      if (! (*inner)->exception_handled_p (*it))
		{
		  ok = false;
		  break;
		}
	    }

	  if (ok)
	    intersection.insert (*it);
	}
    }

  for (std::set<model_type *>::const_iterator i = intersection.begin ();
       i != intersection.end ();
       ++i)
    {
      ref_forwarding_type t
	= new model_forwarding_resolved (LOCATION_UNKNOWN, *i);
      decls.push_back (t);
    }
}

std::set<model_type *>
model_throws_clause::get_as_set () const
{
  std::set<model_type *> result;
  for (std::list<ref_forwarding_type>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      model_type *throw_type = (*i)->type ();
      if (throw_type->checked_exception_p ())
	result.insert (throw_type);
    }
  return result;
}

bool
model_throws_clause::handled_p (model_type *exc_type) const
{
  for (std::list<ref_forwarding_type>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      model_type *throw_type = (*i)->type ();
      if (throw_type->assignable_from_p (exc_type))
	return true;
    }
  return false;
}

bool
model_throws_clause::subset_p (const model_throws_clause &other) const
{
  std::set<model_type *> oset = other.get_as_set ();
  for (std::list<ref_forwarding_type>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      model_type *throw_type = (*i)->type ();
      if (! throw_type->checked_exception_p ()
	  || oset.find (throw_type) != oset.end ())
	continue;

      // Check to see if this is a subclass of some existing class.
      bool found = false;
      for (std::set<model_type *>::const_iterator j = oset.begin ();
	   j != oset.end ();
	   ++j)
	{
	  if ((*j)->assignable_from_p (throw_type))
	    {
	      found = true;
	      break;
	    }
	}
      if (! found)
	return false;
    }
  return true;
}

void
model_throws_clause::propagate_throws (resolution_scope *scope)
{
  for (std::list<ref_forwarding_type>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      model_type *throw_type = (*i)->type ();
      if (throw_type->checked_exception_p ())
	scope->get_current_catcher ()->note_throw_type (throw_type);
    }
}

std::string
model_throws_clause::get_signature ()
{
  std::string result;
  for (std::list<ref_forwarding_type>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    result += "^" + (*i)->type ()->get_signature ();
  return result;
}

void
model_throws_clause::resolve (resolution_scope *scope)
{
  model_type *thr = global->get_compiler ()->java_lang_Throwable ();
  bool warn = scope->warn_unchecked ();

  for (std::list<ref_forwarding_type>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      (*i)->resolve (scope);
      if (! thr->assignable_from_p ((*i)->type ()))
	std::cerr << (*i)->error ("type in %<throws%> clause not derived from "
				  "%<java.lang.Throwable%>");
      else if (warn && ! (*i)->type ()->checked_exception_p ())
	std::cerr << (*i)->warn (global->get_compiler ()->warn_unchecked (),
				 "unnecessary use of unchecked exception"
				 " in %<throws%> clause");
    }
}
