// Scopes.

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



resolution_scope::resolution_scope ()
  : warning_scope (static_cast<warning_scope *> (global->get_compiler ())),
    current_class (NULL)
{
}

resolution_scope::~resolution_scope ()
{
}

model_unit *
resolution_scope::get_compilation_unit () const
{
  if (scope_stack.empty ())
    return NULL;
  // The first thing better be a compilation unit.
  return assert_cast<model_unit *> (scope_stack.front ());
}

model_method *
resolution_scope::get_current_method () const
{
  for (std::deque<IScope *>::const_reverse_iterator i = scope_stack.rbegin ();
       i != scope_stack.rend ();
       ++i)
    {
      model_method *r = dynamic_cast<model_method *> (*i);
      if (r != NULL)
	return r;
    }
  return NULL;
}

void
resolution_scope::update_cache ()
{
  // Note that this is called before the last item is popped from the
  // stack, due how push_iscope is implemented.
  std::deque<IScope *>::reverse_iterator i = scope_stack.rbegin (); 
  // Return early if stack is empty or if we aren't popping the
  // current class.
  if (i == scope_stack.rend () || *i != current_class)
    return;
  current_class = NULL;
  for (++i; i != scope_stack.rend (); ++i)
    {
      model_class *r = dynamic_cast<model_class *> (*i);
      if (r != NULL)
	{
	  current_class = r;
	  break;
	}
    }
}

ICatcher *
resolution_scope::get_current_catcher () const
{
  for (std::deque<IScope *>::const_reverse_iterator i = scope_stack.rbegin ();
       i != scope_stack.rend ();
       ++i)
    {
      ICatcher *r = dynamic_cast<ICatcher *> (*i);
      if (r != NULL)
	return r;
    }
  return NULL;
}

void
resolution_scope::search_for_type_name (const std::string &name,
					std::set<model_class *> &result,
					IContext *request,
					model_class *qualifier)
{
  for (std::deque<IScope *>::reverse_iterator i = scope_stack.rbegin ();
       result.empty () && i != scope_stack.rend ();
       ++i)
    (*i)->look_up_name (name, result, request, qualifier);
}

void
resolution_scope::search_for_field_name (const std::string &name,
					 std::set<model_variable_decl *> &result,
					 IContext *request,
					 model_class *qualifier)
{
  for (std::deque<IScope *>::reverse_iterator i = scope_stack.rbegin ();
       result.empty () && i != scope_stack.rend ();
       ++i)
    (*i)->look_up_name (name, result, request, qualifier);
}

bool
resolution_scope::static_p () const
{
  for (std::deque<IScope *>::const_reverse_iterator i = scope_stack.rbegin ();
       i != scope_stack.rend ();
       ++i)
    {
      static_result r = (*i)->is_static_scope ();
      if (r != DONT_KNOW)
	return r == STATIC_CONTEXT;
    }
  return false;
}

void
resolution_scope::look_up_name (const std::string &name,
				std::set<model_class *> &result,
				IContext *request,
				model_class *qualifier)
{
  // FIXME is this method needed?
  return scope_stack.back ()->look_up_name (name, result, request, qualifier);
}

void
resolution_scope::look_up_name (const std::string &name,
				std::set<model_variable_decl *> &result,
				IContext *request,
				model_class *qualifier)
{
  // FIXME is this method needed?
  return scope_stack.back ()->look_up_name (name, result, request, qualifier);
}

void
resolution_scope::look_up_name (const std::string &name,
				std::set<model_method *> &result,
				IContext *request,
				model_class *qualifier)
{
  // FIXME is this method needed?
  return scope_stack.back ()->look_up_name (name, result, request, qualifier);
}

IContext *
resolution_scope::get_current_context () const
{
  for (std::deque<IScope *>::const_reverse_iterator i = scope_stack.rbegin ();
       i != scope_stack.rend ();
       ++i)
    {
      IContext *r = dynamic_cast<IContext *> (*i);
      if (r != NULL)
	return r;

      model_unit *unit = dynamic_cast<model_unit *> (*i);
      if (unit != NULL)
	return unit->get_package ();
    }
  return NULL;
}

void
resolution_scope::add_binding (model_variable_decl *var)
{
  // See if we have a redeclaration.  The search stops when we hit a
  // class or a method on the scope stack.
  std::set<model_variable_decl *> hold;
  std::string name = var->get_name ();
  for (std::deque<IScope *>::reverse_iterator i = scope_stack.rbegin ();
       i != scope_stack.rend ();
       ++i)
    {
      if (dynamic_cast<model_method *> (*i) != NULL
	  || dynamic_cast<model_class *> (*i) != NULL)
	break;

      (*i)->look_up_name (name, hold, NULL, NULL);
      if (! hold.empty ())
	{
	  // FIXME: location might be wrong.
	  // FIXME: put other locations in error message.
	  throw var->error ("duplicate declaration of variable %1")
	    % var->get_name ();
	}
    }

  scope_stack.back ()->add_binding (var);
}

void
resolution_scope::add_binding (model_class *klass)
{
  // See if we have a redeclaration.  The search stops when we hit a
  // class or a method on the scope stack.
  std::set<model_class *> hold;
  std::string name = klass->get_name ();
  for (std::deque<IScope *>::reverse_iterator i = scope_stack.rbegin ();
       i != scope_stack.rend ();
       ++i)
    {
      if (dynamic_cast<model_method *> (*i) != NULL
	  || dynamic_cast<model_class *> (*i) != NULL)
	break;

      (*i)->look_up_name (name, hold, NULL, NULL);
      if (! hold.empty ())
	{
	  // FIXME: location might be wrong.
	  // FIXME: put other locations in error message.
	  throw klass->error ("duplicate declaration of local class %1")
	    % klass;
	}
    }

  scope_stack.back ()->add_binding (klass);
}

void
resolution_scope::push_scope (IScope *i)
{
  scope_stack.push_back (i);
  model_class *r = dynamic_cast<model_class *> (i);
  if (r != NULL)
    current_class = r;
}

resolution_scope::push_iscope::push_iscope (resolution_scope *scope,
					    IScope *is)
  : stack_temporary<IScope *> (scope->scope_stack, is),
    save (scope)
{
  model_class *r = dynamic_cast<model_class *> (is);
  if (r != NULL)
    scope->current_class = r;
}
