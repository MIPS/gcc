// Represent a list of modifiers.

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
model_modifier_list::check (model_element *request, modifier_t valid,
			    const char *error_format)
{
  for (std::map<modifier_t, location>::const_iterator i = modifiers.begin ();
       i != modifiers.end ();
       ++i)
    {
      if ((valid & (*i).first) == 0)
	std::cerr << request->error (error_format)
	  % name ((*i).first);
    }
}

void
model_modifier_list::access_check (model_element *request,
				   const char *error_format)
{
  modifier_t check = mods & ACC_ACCESS;

  // It is simplest to just write out all the tests.
  if ((check & ACC_PUBLIC) != 0 && (check & ACC_PROTECTED) != 0)
    std::cerr << request->error (error_format)
      % "public" % "protected";
  if ((check & ACC_PUBLIC) != 0 && (check & ACC_PRIVATE) != 0)
    std::cerr << request->error (error_format)
      % "public" % "private";
  if ((check & ACC_PROTECTED) != 0 && (check & ACC_PRIVATE) != 0)
    std::cerr << request->error (error_format)
      % "protected" % "private";
}

void
model_modifier_list::add (token t)
{
  modifier_t m = get_value (t);
  if ((mods & m) != 0)
    {
      // We could indicate where the original modifier is... overkill?
      std::cerr << format (new format_repr (format_error, t,
					    "duplicate modifier %1"))
	% name (m);
    }

  int order = get_order (t);
  if (current_order > order)
    {
      if (global->get_compiler ()->warn_canonical_modifier_order ())
	{
	  std::cerr
	    << format (new format_repr (format_type (global->get_compiler ()->warn_canonical_modifier_order ()),
					t,
					"modifier %1 appears in wrong "
					"position for canonical ordering"))
	    % name (m);
	}
    }
  else
    current_order = order;

  modifiers[m] = t;
  mods |= m;
}
