// Base class for plain-byte input streams.

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

#ifndef GCJX_READER_READER_HH
#define GCJX_READER_READER_HH

#include <time.h>

class byte_buffer;

class reader
{
private:

  // Name of underlying file.
  std::string filename;

protected:

  reader (const std::string &fn)
    : filename (fn)
  {
  }

  void note_read ()
  {
    global->get_compiler ()->note_file_read (filename);
  }

public:

  virtual ~reader ()
  {
  }

  /// Read everything in the input stream.  This cannot be called if
  /// any bytes were read.  And once it is called, the stream is in an
  /// invalid state and must be closed.
  /// @return a new buffer
  virtual byte_buffer *read_all () = 0;

  /// Return the last modified time of the underlying resource.
  virtual time_t get_mtime () = 0;
};

#endif // GCJX_READER_READER_HH
