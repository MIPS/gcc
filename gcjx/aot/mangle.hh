// Name mangler that is compatible with g++.

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

#ifndef GCJX_AOT_MANGLE_HH
#define GCJX_AOT_MANGLE_HH

class mangler
{
  // The mangled form.
  std::string result;

  struct cache_entry
  {
    model_element *element;
    bool is_pointer;
  };

  std::vector<cache_entry> compression_table;


  char get_type_name (model_type *);
  void emit_saved (int);
  int find_compression (model_element *, bool);
  void insert (model_element *, bool);
  void update (const std::string &);
  void update (model_package *);
  void update_array (model_array_type *);
  void update (model_type *, bool);
  void update_cxx (const std::string &);

public:

  mangler (model_type *);

  mangler (model_method *);

  mangler (model_field *);

  // This is a way to create a mangled name for a non-existing field,
  // given its declaring class.
  mangler (model_class *, const char *);

  /// This is slightly bogus: the bool argument is ignored; this
  /// constructor is used to create the name of the class' vtable.
  mangler (model_class *, bool);

  std::string get () const
  {
    return result;
  }
};

#endif // GCJX_AOT_MANGLE_HH
