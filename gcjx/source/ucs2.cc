// Implementation of readers.

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

#include <langinfo.h>

#include "source/ucs2.hh"
#include "source/iconv.hh"
#include "buffer.hh"

ucs2_reader::ucs2_reader (byte_buffer *in)
  : input (in),
    begin (in->get ()),
    curr (in->get ()),
    end (in->get () + in->get_length ()),
    posn (0),
    max_posn (in->get_length ())
{
}

ucs2_reader::~ucs2_reader ()
{
  delete input;
}

int
ucs2_reader::here ()
{
  return curr - input->get ();
}



unicode_w_t
fallback_ucs2_reader::get ()
{
  int c = get_uint8 ();

  if (c == -1)
    return UNICODE_EOF;
  if (c < 128)
    return (unicode_w_t) c;

  if ((c & 0xe0) == 0xc0)
    {
      int c1 = get_uint8 ();
      if ((c1 & 0xc0) == 0x80)
	{
	  unicode_w_t r = (unicode_w_t) (((c & 0x1f) << 6) + (c1 & 0x3f));
	  // Check for valid 2-byte characters.  We explicitly
	  // allow \0 because this encoding is common in the Java
	  // world.
	  if (r == 0 || (r >= 0x80 && r <= 0x7ff))
	    return r;
	}
    }
  else if ((c & 0xf0) == 0xe0)
    {
      int c1 = get_uint8 ();
      if ((c1 & 0xc0) == 0x80)
	{
	  int c2 = get_uint8 ();
	  if ((c2 & 0xc0) == 0x80)
	    {
	      unicode_w_t r =  (unicode_w_t) (((c & 0xf) << 12) + 
					      (( c1 & 0x3f) << 6)
					      + (c2 & 0x3f));
	      // Check for valid 3-byte characters.
	      // Don't allow surrogate, \ufffe or \uffff.
	      if (in_range_p (r, 0x800, 0xffff)
		  && ! in_range_p (r, 0xd800, 0xdfff)
		  && r != 0xfffe && r != 0xffff)
		return r;
	    }
	}
    }

  // We simply don't support invalid characters.  We also don't
  // support 4-, 5-, or 6-byte UTF-8 sequences, as these cannot be
  // valid Java characters.  Note that we don't know our location, but
  // our caller can fill that in as needed.
  throw conversion_error (LOCATION_UNKNOWN,
			  "malformed UTF-8 character at byte %1 in file")
    % here ();
}



std::string
compute_default_encoding ()
{
  char *result;

  // FIXME: configury.
  setlocale (LC_CTYPE, "");
  result = nl_langinfo (CODESET);

  if (result == NULL || *result == '\0')
    result = "UTF-8";

  return result;
}

ucs2_reader *
create_ucs2_reader (byte_buffer *buffer, const std::string &encoding)
{
  // FIXME: configury: only use iconv reader if available.
  // FIXME: throw the proper exception on error.
  // FIXME: construct a fallback_ucs2_reader if the iconv creation
  // fails.
  return new iconv_ucs2_reader (buffer, encoding);
}
