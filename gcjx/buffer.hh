// Buffer full of bytes.

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

#ifndef GCJX_BUFFER_HH
#define GCJX_BUFFER_HH

/// This is a simple read-only buffer of bytes.  It knows its own
/// length and how to look up bytes like an array.  Concrete
/// subclasses handle allocation and deallocation.
class byte_buffer
{
protected:

  /// The bytes.
  uint8 *data;

  /// The number of valid bytes in data.
  unsigned int length;

  byte_buffer (uint8 *d = NULL, unsigned int l = 0)
    : data (d),
      length (l)
  {
  }

public:

  virtual ~byte_buffer ()
  {
  }

  uint8 operator[] (int x) const
  {
    return data[x];
  }

  operator const uint8 * () const
  {
    return data;
  }

  const uint8 *get () const
  {
    return data;
  }

  unsigned int get_length () const
  {
    return length;
  }
};

#endif // GCJX_BUFFER_HH
