// Factory for creating class wrappers.

// Copyright (C) 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#include "typedefs.hh"
#include "aot/aotfactory.hh"
#include "aot/aotclass.hh"
#include "aot/mangle.hh"

aot_class *
aot_class_factory::get_class (model_class *klass)
{
  std::map<model_class *, aot_class *>::const_iterator it
    = class_map.find (klass);
  if (it != class_map.end ())
    return (*it).second;

  aot_class *result = new aot_class (this, klass);
  class_map[klass] = result;
  return result;
}

template<typename T>
std::string
aot_class_factory::get_mangled_form (T *val)
{
  std::map<model_element *, std::string>::const_iterator i
    = mangle_map.find (val);
  if (i != mangle_map.end ())
    return (*i).second;

  mangler mg (val);
  mangle_map[val] = mg.get ();
  return mg.get ();
}



// Instantiations.

template std::string aot_class_factory::get_mangled_form (model_field *);
template std::string aot_class_factory::get_mangled_form (model_method *);
