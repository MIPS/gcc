// Represent a statement.

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

bool
normal_completion_state::can_goto_p (model_stmt *stmt) const
{
  for (std::list<model_stmt *>::const_reverse_iterator i
	 = statements.rbegin ();
       i != statements.rend ();
       ++i)
    {
      if (stmt == *i)
	return true;
      model_try *t = dynamic_cast<model_try *> (*i);
      if (t != NULL && ! t->finally_can_complete_normally ())
	return false;
    }
  // We have to find the statement.
  abort ();
}

bool
compute_normal_completion (normal_completion_state &state,
			   std::list< owner<model_stmt> > &statements)
{
  for (std::list< owner<model_stmt> >::iterator i = statements.begin ();
       i != statements.end ();
       ++i)
    {
      // This lets us avoid recomputing state when we add statements
      // to a constructor.
      if ((*i)->normal_completion.set_p ())
	return (*i)->can_complete_normally ();

      (*i)->compute_normal_completion (state);
      if (! (*i)->can_complete_normally ())
	{
	  if (++i != statements.end ())
	    {
	      (*i)->unreachable ();
	      // Remove all unreachable statements.  This makes
	      // things conceptually simpler when walking the tree
	      // later.  (Note that unreachable code is an error, so
	      // this doesn't affect code generation.)
	      statements.erase (i, statements.end ());
	    }
	  return false;
	}
    }
  return true;
}
