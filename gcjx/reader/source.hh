// Read a .java file.

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

#ifndef GCJX_READER_SOURCE_HH
#define GCJX_READER_SOURCE_HH

#include "reader/classinst.hh"
#include "reader/reader.hh"

/// This is a creator class that knows how to create class instances
/// by reading .java source from a byte stream.
class source_file_creator : public class_instance_creator
{
  /// File name.
  std::string file;

  /// Source of bytes.
  reader *the_reader;

public:

  source_file_creator (const std::string &f, reader *rd)
    : file (f),
      the_reader (rd)
  {
  }

  ~source_file_creator ();

  time_t get_mtime ()
  {
    return the_reader->get_mtime ();
  }

  void apply (bool);
};

#endif // GCJX_READER_SOURCE_HH
