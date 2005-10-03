// iconv-based reader.

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

#ifndef GCJX_SOURCE_ICONV_HH
#define GCJX_SOURCE_ICONV_HH

#include <iconv.h>

class iconv_ucs2_reader : public ucs2_reader
{
  // The iconv() handle.
  iconv_t handle;

  // A buffer of ucs-2 characters we supply to our caller.
  jchar translated[1000];

  // One past the last valid character in TRANSLATED.
  int last;

  // Name of the input encoding.
  std::string input_encoding;

  // Helper function to refill TRANSLATED when needed.
  void refill ();

public:

  iconv_ucs2_reader (byte_buffer *, const std::string &);

  ~iconv_ucs2_reader ();

  jchar *get (int &);
};

#endif // GCJX_SOURCE_ICONV_HH
