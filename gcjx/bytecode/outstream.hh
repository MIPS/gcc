// Bytecode output stream.

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

#ifndef GCJX_BYTECODE_OUTSTREAM_HH
#define GCJX_BYTECODE_OUTSTREAM_HH

class bytecode_stream
{
  // The bytes we're collecting.
  jbyte *bytes;

  // The number of valid bytes in the array.
  int count;

  // The size of the array.
  int size;

public:

  bytecode_stream ();

  ~bytecode_stream ();

  void put (int);
  void put2 (int);
  void put4 (int);
  void remove ();

  void put (int, const char *);
  void put2_at (int, int);

  int get_offset () const
  {
    return count;
  }

  jbyte *get_bytes () const
  {
    return bytes;
  }
};

#endif // GCJX_BYTECODE_OUTSTREAM_HH
