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

#include <string.h>

#include "typedefs.hh"
#include "bytecode/outstream.hh"

// Most class files are less than 5k.
// FIXME: re-run my script to find a more precise tradeoff...?
#define INITIAL_SIZE (5 * 1024)

bytecode_stream::bytecode_stream ()
  : bytes (new jbyte[INITIAL_SIZE]),
    count (0),
    size (INITIAL_SIZE)
{
}

bytecode_stream::~bytecode_stream ()
{
  delete [] bytes;
}

void
bytecode_stream::put (int len, const char *data)
{
  if (count + len >= size)
    {
      size *= 2;
      if (count + len >= size)
	size = count + len;
      jbyte *newbytes = new jbyte[size];
      memcpy (newbytes, bytes, count);
      delete [] bytes;
      bytes = newbytes;
    }
  memcpy (&bytes[count], data, len);
  count += len;
}

void
bytecode_stream::put (int val)
{
  char c = val & 0xff;
  put (1, &c);
}

void
bytecode_stream::put2 (int x)
{
  put (x >> 8);
  put (x & 0xff);
}

void
bytecode_stream::put2_at (int offset, int x)
{
  assert (offset >= 0 && offset < count);
  bytes[offset] = x >> 8;
  bytes[offset + 1] = x & 0xff;
}

void
bytecode_stream::put4 (int x)
{
  put (x >> 24);
  put (x >> 16);
  put (x >>  8);
  put (x & 0xff);
}

void
bytecode_stream::remove ()
{
  assert (count > 0);
  --count;
}
