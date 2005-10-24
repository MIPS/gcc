// Map type variables to types or other type variables.

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
model_type_map::operator== (const model_type_map &other) const
{
  if (typemap.size () != other.typemap.size ())
    return false;
  for (map_type::const_iterator i = typemap.begin ();
       i != typemap.end ();
       ++i)
    {
      map_type::const_iterator oi = other.typemap.find ((*i).first);
      if (oi == other.typemap.end ()
	  || (*oi).second != (*i).second)
	return false;
    }
  return true;
}

model_class *
model_type_map::find (model_type_variable *var) const
{
  map_type::const_iterator i = typemap.find (var);
  if (i == typemap.end ())
    return NULL;
  return (*i).second;
}
