// ucs-2 readers.

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

#ifndef GCJX_SOURCE_UCS2_HH
#define GCJX_SOURCE_UCS2_HH

class byte_buffer;

class ucs2_reader
{
protected:

  /// The buffer holding all input data.
  byte_buffer *input;

  /// Current pointer.
  const uint8 *curr;

  /// Limit pointer.
  const uint8 *limit;

  /// Return the next uint8, or -1 on EOF.
  int get_uint8 ()
  {
    return curr == limit ? -1 : *curr++;
  }

  int here ();

  ucs2_reader (byte_buffer *);

public:

  virtual ~ucs2_reader ();

  /// @return the next UCS-2 character.  Return UNICODE_EOF on
  /// end-of-file.  Throws conversion_error on any kind of character
  /// conversion error; the caller is expected to set the location on
  /// this exception.
  virtual unicode_w_t get () = 0;
};

// Assume the input is utf-8.
class fallback_ucs2_reader : public ucs2_reader
{
  // Just a dumb helper.
  bool in_range_p (unicode_w_t v, unicode_w_t min, unicode_w_t max)
  {
    return v >= min && v <= max;
  }

public:

  fallback_ucs2_reader (byte_buffer *in)
    : ucs2_reader (in)
  {
  }

  unicode_w_t get ();
};


// This is a helper function that knows how to compute the default
// encoding for this platform and current environment.
std::string compute_default_encoding ();

// This is a factory function that constructs the proper UCS-2 reader,
// given a buffer and the encoding.
ucs2_reader *create_ucs2_reader (byte_buffer *, const std::string &);

#endif // GCJX_SOURCE_UCS2_HH
