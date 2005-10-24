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

#ifndef GCJX_MODEL_TYPEMAP_HH
#define GCJX_MODEL_TYPEMAP_HH

class model_type_variable;

/// A type map maps type variables onto types or other type variables.
/// So, for instance, if a class is declared as ClassName<X>, a
/// particular instantiation of that class, e.g. ClassName<String>,
/// will be created by means of a type map that maps X to String.
class model_type_map
{
  typedef std::map<model_type_variable *, model_class *> map_type;

  map_type typemap;

public:

  model_type_map ()
  {
  }

  ~model_type_map ()
  {
  }

  /// Add a mapping to the type map.
  void add (model_type_variable *var, model_class *val)
  {
    typemap[var] = val;
  }

  /// Get a mapping from the type map.  Returns NULL if it isn't
  /// found.
  model_class *find (model_type_variable *) const;

  /// Return true if this type map is equivalent to another type map.
  bool operator== (const model_type_map &) const;

  bool empty_p () const
  {
    return typemap.empty ();
  }
};

#endif // GCJX_MODEL_TYPEMAP_HH
