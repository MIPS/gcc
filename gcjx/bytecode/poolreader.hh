// Parsing bytes representing a constant pool.

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

#ifndef GCJX_BYTECODE_POOLREADER_HH
#define GCJX_BYTECODE_POOLREADER_HH

/// This class knows how to parse the constant pool part of a class
/// file.
class constant_pool_reader
{
  // Corresponding code.
  location where;

  // Bytes.
  const uint8 *bytes;

  // Length.
  int length;

  // Current location.
  const uint8 *ptr;

  uint8 read_u1 ();
  uint16 read_u2 ();
  uint32 read_u4 ();
  class_file_error error (const char *) const;

public:

  constant_pool_reader (const location &, const uint8 *, int);

  constant_pool *parse (const uint8 **);
};

#endif // GCJX_BYTECODE_POOLREADER_HH
