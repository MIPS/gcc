// Reader that uncompresses and reads a ZIP/JAR file entry.

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

#ifndef GCJX_READER_ZEREADER_HH
#define GCJX_READER_ZEREADER_HH

#include <time.h>

#include "reader/reader.hh"

class zip_entry_reader : public reader
{
private:

  /// The compressed data.
  const uint8 *compressed_data;

  /// The compression method used for the file.
  int compression_method;

  /// The compressed size of the file inside the archive.
  uint32 compressed_size;

  /// The uncompressed size of the original file.
  uint32 uncompressed_size;

  /// The last modified time of the file as stored within the archive.
  uint32 dos_mtime;
  /// The last modified time of the file, corrected.
  time_t last_mod_time;

public:

  zip_entry_reader (const std::string &fn, const uint8 *data, int cmethod,
		    uint32 csize, uint32 ucsize, uint32 dos_mtime)
    : reader (fn),
      compressed_data (data),
      compression_method (cmethod),
      compressed_size (csize),
      uncompressed_size (ucsize),
      dos_mtime (dos_mtime),
      last_mod_time (~0)
  {
  }

  ~zip_entry_reader ();

  byte_buffer *read_all ();

  time_t get_mtime ();
};

#endif // GCJX_READER_ZEREADER_HH
