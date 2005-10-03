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

#include "typedefs.hh"
#include "source/ucs2.hh"
#include "source/iconv.hh"
#include "buffer.hh"

#include <iconv.h>
#include <errno.h>

// We use this adapter because iconv() has different prototypes on
// different platforms.  This is taken from libgcj.
template<typename T>
static inline size_t
iconv_adapter (size_t (*iconv_f) (iconv_t, T, size_t *, char **, size_t *),
	       iconv_t handle, char **inbuf, size_t *inavail,
	       char **outbuf, size_t *outavail)
{
  return (*iconv_f) (handle, (T) inbuf, inavail, outbuf, outavail);
}

iconv_ucs2_reader::iconv_ucs2_reader (byte_buffer *in,
				      const std::string &inp_enc)
  : ucs2_reader (in),
    input_encoding (inp_enc)
{
  handle = iconv_open ("UCS-2", input_encoding.c_str ());
  // FIXME: throw the proper exception in this case.
  assert (handle != (iconv_t) -1);

  last = 0;
}

iconv_ucs2_reader::~iconv_ucs2_reader ()
{
  if (handle != (iconv_t) -1)
    iconv_close (handle);
}

void
iconv_ucs2_reader::refill ()
{
  size_t in_rem = end - curr;
  char *out_loc = (char *) translated;
  size_t out_rem = sizeof (translated);

  size_t result = iconv_adapter (iconv, handle, (char **) &curr, &in_rem,
				 &out_loc, &out_rem);
  if (result == (size_t) -1)
    {
      if (errno == E2BIG)
	{
	  // This actually isn't an error, it just means we have more
	  // input than we have space for in our output buffer.
	}
      else
	{
	  // Other choice is EBADF, which "really can't happen".
	  assert (errno == EILSEQ || errno == EINVAL);
	  // Our caller will fill in the real location, as we don't know
	  // it.
	  throw conversion_error (LOCATION_UNKNOWN, "malformed %1 character")
	    % input_encoding;
	}
    }

  // A UCS-2 character is always 2 bytes.
  assert ((out_loc - (char *) translated) % 2 == 0);
  last = (out_loc - (char *) translated) / 2;
}

jchar *
iconv_ucs2_reader::get (int &size)
{
  refill ();
  size = last;
  return translated;
}
