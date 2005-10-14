// Constant pool.

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

#ifndef GCJX_BYTECODE_CPOOL_HH
#define GCJX_BYTECODE_CPOOL_HH

#include "bytecode/constants.hh"

// FIXME this is bogus
typedef uint32 jword;

class constant_pool
{
  // Number of elements in the pool.
  int capacity;

  // Location.
  location where;

  // Tag for each element.
  uint8 *tags;

  // Data for each element.
  jword *data;

  // Pointer to utf-8 constants.
  const std::string *utf_data;


  void get_name_and_type (uint16, std::string &, std::string &);
  void get_indices (uint16, uint16 &, uint16 &);

public:

  constant_pool (const location &w, int size, uint8 *ts,
		 jword *ds, const std::string *ud)
    : capacity (size),
      tags (ts),
      data (ds),
      utf_data (ud)
  {
  }

  ~constant_pool ()
  {
    delete [] tags;
    delete [] data;
    delete [] utf_data;
  }

  uint8 tag (uint16 n)
  {
    return tags[n];
  }

  bool valid_p (uint16 n)
  {
    return n < capacity;
  }

  int size () const
  {
    return capacity;
  }

  jword value (uint16 n)
  {
    return data[n];
  }


  /// Return a string for the UTF-8 constant at the given index.
  /// Throws an exception if the index is invalid or if the type is
  /// incorrect.
  std::string get_utf8 (uint16);

  std::string get_string (uint16);
  std::string get_class (uint16);

  void get_fieldref (uint16, std::string &, std::string &, std::string &);
  void get_methodref (uint16, std::string &, std::string &, std::string &);
};

#endif // GCJX_BYTECODE_CPOOL_HH
