// Parameters for a generic method or class.

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
model_parameters::resolve_classes (resolution_scope *scope)
{
  for (std::list<ref_type_variable>::const_iterator i
	 = type_parameters.begin ();
       i != type_parameters.end ();
       ++i)
    (*i)->resolve (scope);
}

void
model_parameters::look_up_name (const std::string &name,
				std::set<model_class *> &result,
				IContext *,
				model_class *)
{
  for (std::list<ref_type_variable>::const_iterator i
	 = type_parameters.begin ();
       i != type_parameters.end ();
       ++i)
    {
      if (name == (*i)->get_name ())
	{
	  result.insert ((*i).get ());
	  break;
	}
    }
}

bool
model_parameters::create_type_map (model_type_map &result,
				   const model_parameters &other) const
{
  std::list<ref_type_variable>::const_iterator this_it
    = type_parameters.begin ();
  std::list<ref_type_variable>::const_iterator other_it
    = other.type_parameters.begin ();

  while (this_it != type_parameters.end ()
	 && other_it != other.type_parameters.end ())
    {
      result.add ((*this_it).get (), (*other_it).get ());
      ++this_it;
      ++other_it;
    }

  return (this_it == type_parameters.end ()
	  && other_it == other.type_parameters.end ());
}

std::string
model_parameters::get_pretty_name () const
{
  std::string result = "<";
  for (std::list<ref_type_variable>::const_iterator it
	 = type_parameters.begin ();
       it != type_parameters.end ();
       ++it)
    {
      if (it != type_parameters.begin ())
        result += ",";
      result += (*it)->get_pretty_name ();
    }
  result += ">";
  return result;
}

std::string
model_parameters::get_signature ()
{
  std::string result;

  if (! type_parameters.empty ())
    {
      result = "<";
      for (std::list<ref_type_variable>::const_iterator it
	     = type_parameters.begin ();
	   it != type_parameters.end ();
	   ++it)
	result += (*it)->get_formal_signature ();
      result += ">";
    }
  return result;
}
