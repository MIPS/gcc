// Local variable handling.

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
#include "bytecode/locals.hh"
#include "bytecode/insns.hh"
#include "bytecode/byteutil.hh"

void
locals::push_scope (const model_stmt *newscope)
{
  scope.push_back (newscope);
}

int
locals::request (model_variable_decl *decl)
{
  assert (decl == NULL || indexes.find (decl) == indexes.end ());
  int n;
  int delta = 0;
  if (wide_p (decl ? decl->type () : NULL))
    delta = 1;

  // This loop is written a little strangely so that it will always
  // terminate with N == MAX if we didn't find a large enough empty
  // space.
  for (n = 0; n < max; ++n)
    {
      if (! used[n] && (n + delta < max && ! used[n + delta]))
	break;
    }

  if (n >= max)
    {
      max += delta + 1;
      used.resize (max);
    }
  used[n] = true;
  used[n + delta] = true;

  if (decl)
    {
      indexes[decl] = n;
      scope_map[decl] = scope.back ();
    }
  return n;
}

int
locals::get_index (model_variable_decl *decl)
{
  // It would be way easier to have a field on the variable.
  std::map<model_variable_decl *, int>::const_iterator i
    = indexes.find (decl);
  assert (i != indexes.end ());
  return (*i).second;
}

void
locals::remove (model_variable_decl *decl)
{
  std::map<model_variable_decl *, int>::iterator i
    = indexes.find (decl);
  assert (i != indexes.end ());
  used[(*i).second] = false;

  if (wide_p (decl ? decl->type () : NULL))
    used[(*i).second + 1] = false;

  indexes.erase (i);
}

void
locals::remove (int n)
{
  // This should never be called for wide variables, but at the moment
  // we have no way of asserting that.
  used[n] = false;
}

void
locals::remove (const model_stmt *stmt)
{
  std::list<model_variable_decl *> dels;
  for (std::map<model_variable_decl *, int>::iterator i
	 = indexes.begin ();
       i != indexes.end ();
       ++i)
    {
      if (scope_map[(*i).first] == stmt)
	{
	  used[(*i).second] = false;
	  if (wide_p ((*i).first->type ()))
	    used[(*i).second + 1] = false;
	  dels.push_back ((*i).first);
	}
    }

  for (std::list<model_variable_decl *>::const_iterator i = dels.begin ();
       i != dels.end ();
       ++i)
    indexes.erase (*i);
}
