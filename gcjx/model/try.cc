// try statement.

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

void
model_try::compute_normal_completion (normal_completion_state &state)
{
  // We do this in a certain order to maintain reachability
  // constraints.  First we handle the 'finally' clause, so that code
  // in the try and catch blocks can query to see if it can complete
  // normally.
  if (finally)
    {
      // Note that we don't push this statement on the state when
      // evaluating the 'finally' clause.
      finally->compute_normal_completion (state);
    }
  bool finally_ccn = finally ? finally->can_complete_normally () : true;

  {
    normal_completion_pusher push (state, this);
    flag = false;
    block->compute_normal_completion (state);
    flag = true;
  }

  bool try_ccn = block->can_complete_normally ();

  model_class *thrt = global->get_compiler ()->java_lang_Throwable ();
  model_class *errt = global->get_compiler ()->java_lang_Error ();
  model_class *rtet = global->get_compiler ()->java_lang_RuntimeException ();
  model_class *exct = global->get_compiler ()->java_lang_Exception ();

  std::list<model_type *> previous_exceptions;
  std::list<ref_catch>::const_iterator i;
  for (i = catchers.begin (); i != catchers.end (); ++i)
    {
      model_type *t = (*i)->get_parameter_type ();
      bool reachable = true;

      // If the clause handles a checked exception, first see if any
      // such exception is actually thrown.  As a special case, we
      // treat Throwable and Exception as if they were unchecked.
      if (reachable)
	{
	  if (t != thrt && t != exct && ! errt->assignable_from_p (t)
	      && ! rtet->assignable_from_p (t))
	    {
	      reachable = false;
	      for (std::set<model_type *>::const_iterator j
		     = checked_exceptions.begin ();
		   j != checked_exceptions.end ();
		   ++j)
		{
		  // This is sort of strange, but the user might catch
		  // just some subclass of an exception that is
		  // thrown.  Since exception handlers are found
		  // dynamically, and an expression might throw some
		  // subclass of what it actually declares, this makes
		  // sense.  Perhaps we should have a warning in here
		  // somewhere; see TODO.
		  if (t->assignable_from_p (*j) || (*j)->assignable_from_p (t))
		    {
		      reachable = true;
		      break;
		    }
		}
	    }
	}

      // Now check for 'catch' shadowing -- if the type of a catch
      // clause parameter is shadowed by some earlier type, then the
      // catch clause is unreachable.
      if (reachable)
	{
	  for (std::list<model_type *>::iterator j
		 = previous_exceptions.begin ();
	       j != previous_exceptions.end ();
	       ++j)
	    {
	      model_type *prev = (*j);
	      if (prev->assignable_from_p (t))
		reachable = false;
	    }
	}

      if (reachable)
	{
	  normal_completion_pusher push (state, this);
	  (*i)->compute_normal_completion (state);
	  if ((*i)->can_complete_normally ())
	    try_ccn = true;
	  previous_exceptions.push_back (t);
	}
      else
	(*i)->unreachable ();
    }

  if (! finally_ccn
      && global->get_compiler ()->warn_finally_abnormal_completion ())
    std::cerr << finally->warn (global->get_compiler ()->warn_finally_abnormal_completion (),
				"%<finally%> block cannot complete normally");

  normal_completion = try_ccn && finally_ccn;

  propagate_exceptions ();
}

void
model_try::propagate_exceptions ()
{
  // We must propagate any exceptions we didn't handle.  But first we
  // look at an unusual case.  If we have a 'finally' clause, and that
  // clause cannot complete normally, then exceptions in the 'try'
  // will not propagate.
  if (finally && ! finally->can_complete_normally ())
    return;

  for (std::set<model_type *>::const_iterator i
	 = checked_exceptions.begin ();
       i != checked_exceptions.end ();
       ++i)
    {
      bool found = false;
      for (std::list<ref_catch>::const_iterator j = catchers.begin ();
	   j != catchers.end ();
	   ++j)
	{
	  model_type *clause_type = (*j)->get_parameter_type ();
	  if (clause_type->assignable_from_p (*i))
	    {
	      found = true;
	      break;
	    }
	}

      if (! found)
	enclosing_catcher->note_throw_type (*i);
    }

  for (std::set<model_type *>::const_iterator i
	 = catch_block_exceptions.begin ();
       i != catch_block_exceptions.end ();
       ++i)
    enclosing_catcher->note_throw_type (*i);
}

void
model_try::resolve (resolution_scope *scope)
{
  enclosing_catcher = scope->get_current_catcher ();

  {
    // We push ourselves on the scope as the current catcher.
    resolution_scope::push_iscope holder (scope, this);
    assert (! flag);
    block->resolve (scope);

    flag = true;
    ::resolve (scope, catchers);
  }

  if (finally)
    finally->resolve (scope);
}

void
model_try::visit (visitor *v)
{
  v->visit_try (this, block, catchers, finally);
}
