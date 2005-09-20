// A buffer that mmap()s its contents.

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

#ifndef GCJX_READER_MMAPBUFFER_HH
#define GCJX_READER_MMAPBUFFER_HH

#include "buffer.hh"

class mmap_byte_buffer : public byte_buffer
{
  // Location for error reporting.
  location where;

public:

  /// Create a new mmap buffer given a file descriptor.
  /// fixme document the exception thrown on error.
  mmap_byte_buffer (const location &, int);

  /// Destructor for this buffer.
  ~mmap_byte_buffer ();
};

#endif // GCJX_READER_MMAPBUFFER_HH
