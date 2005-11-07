// Raw types.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

model_raw_class::model_raw_class (const location &w, model_class *p)
  : model_class (w),
    parent (p)
{
  assert (! parent->get_type_parameters ().empty ());
}

std::string
model_raw_class::get_pretty_name () const
{
  // FIXME: should share some code with superclass.
  std::string result;
  if (declaring_class)
    result = declaring_class->get_pretty_name () + "$" + get_assigned_name ();
  else
    {
      std::string cu
	= compilation_unit->get_package ()->get_fully_qualified_name ();
      result = (cu.empty ()) ? name : cu + "." + name;
    }
  return result;
}

void
model_raw_class::ensure_classes_inherited (resolution_scope *)
{
  parent->resolve_classes ();

  // Create our variants of member classes.
  // Note that this is more or less duplicated in classinst.cc, maybe
  // we could share with a functor.
  for (std::map<std::string, ref_class>::const_iterator i
	 = parent->member_classes.begin ();
       i != parent->member_classes.end ();
       ++i)
    {
      ref_class mem = (*i).second;
      if (! mem->static_p ())
	mem = assert_cast<model_class *> (mem->erasure ());
      member_classes[(*i).first] = mem;
    }

  for (std::multimap<std::string, model_class *>::const_iterator i
	 = parent->all_member_classes.begin ();
       i != parent->all_member_classes.end ();
       ++i)
    {
      model_class *mem = (*i).second;
      if (! mem->static_p ())
	mem = assert_cast<model_class *> (mem->erasure ());
      all_member_classes.insert (std::make_pair ((*i).first, mem));
    }
}

void
model_raw_class::resolve_member_hook (resolution_scope *scope)
{
  parent->resolve_members ();
  
  // Also could share with classinst.

  // Create fields.
  for (std::list<ref_field>::const_iterator i = parent->fields.begin ();
       i != parent->fields.end ();
       ++i)
    {
      ref_field fld = *i;
      if (! fld->static_p ())
	fld = assert_cast<model_field *> (fld->erasure ());
      fields.push_back (fld);
    }

  // Create methods.
  for (std::list<ref_method>::const_iterator i = parent->methods.begin ();
       i != parent->methods.end ();
       ++i)
    {
      ref_method meth = *i;
      if (! meth->static_p ())
	meth = meth->erasure (this);
      methods.push_back (meth);
    }
}
