// Buffer that reads from a ZIP/JAR entry.

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

#ifndef GCJX_READER_ZEBUFFER_HH
#define GCJX_READER_ZEBUFFER_HH

#include "buffer.hh"

class zip_entry_byte_buffer : public byte_buffer
{
public:

  /// Create a new zip entry byte buffer given the uncompressed data.
  zip_entry_byte_buffer (uint8 *ucdata, unsigned int ucdata_len);

  /// Destructor for this buffer.
  ~zip_entry_byte_buffer ();
};

#endif // GCJX_READER_ZEBUFFER_HH
