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

#ifndef GCJX_AOT_AOTFACTORY_HH
#define GCJX_AOT_AOTFACTORY_HH

class aot_class;

class aot_class_factory
{
  /// Map of classes to class wrappers.
  std::map<model_class *, aot_class *> class_map;

  /// Map of elements to their mangled forms.
  std::map<model_element *, std::string> mangle_map;

public:

  aot_class_factory ()
  {
  }

  aot_class *get_class (model_class *);

  // Return the mangled form of a method, field, or class.
  template<typename T>
  std::string get_mangled_form (T *);

};

#endif // GCJX_AOT_AOTFACTORY_HH
