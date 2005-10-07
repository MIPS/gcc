// Package representation.

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
model_package::add (const ref_class &klass)
{
  std::string base = klass->get_name ();
  std::map<std::string, owner<model_package> >::const_iterator f
    = subpackages.find (base);
  if (f != subpackages.end ())
    // FIXME: does the location of a package make sense?
    throw (*f).second->error ("class %1 already defined as a package")
      % klass.get ();

  std::map<std::string, ref_class>::const_iterator k
    = types.find (base);
  if (k != types.end ())
    throw (*k).second->error ("class %1 already defined")
      % klass.get ();

  types[base] = klass;
}

Iname *
model_package::find_name (const std::string &n)
{
  std::map<std::string, ref_class>::const_iterator ti
    = types.find (n);
  if (ti != types.end ())
    return (*ti).second.get ();

  std::map<std::string, owner<model_package> >::const_iterator pi
    = subpackages.find (n);
  if (pi != subpackages.end ())
    return (*pi).second.get ();

  return NULL;
}

model_package *
model_package::find_package (const std::string &n)
{
  std::map<std::string, owner<model_package> >::const_iterator pi
    = subpackages.find (n);
  if (pi != subpackages.end ())
    return (*pi).second.get ();
  return NULL;
}

model_class *
model_package::find_class (const std::string &n)
{
  std::map<std::string, model_class *>::const_iterator iter
    = name_cache.find (n);
  if (iter != name_cache.end ())
    return (*iter).second;

  std::list<std::string> full = name;
  full.push_back (n);
  model_class *result = global->get_compiler ()->find_class (full);
  name_cache[n] = result;
  return result;
}

void
model_package::visit (visitor *v)
{
  v->visit_package (this, name);
}

const format &
operator% (const format &fmt, model_package *pkg)
{
  return fmt % pkg->get_fully_qualified_name ();
}
