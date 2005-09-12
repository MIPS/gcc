// Read a compressed file from a ZIP/JAR archive.

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

#include <time.h>
#include <fcntl.h>

#include <zlib.h>

#include "typedefs.hh"

#include "reader/zereader.hh"
#include "reader/zebuffer.hh"

zip_entry_reader::~zip_entry_reader ()
{
}

byte_buffer *
zip_entry_reader::read_all ()
{
  note_read ();
  uint8 *out_buffer = new uint8[uncompressed_size];
  if (compression_method == Z_NO_COMPRESSION)
    {
      memcpy (out_buffer, compressed_data, uncompressed_size);
    }
  else
    {
      z_stream d_stream;

      d_stream.zalloc = (alloc_func) 0;
      d_stream.zfree = (free_func) 0;
      d_stream.opaque = (voidpf) 0;

      d_stream.next_out = out_buffer;
      d_stream.avail_out = uncompressed_size;

      d_stream.next_in = const_cast<uint8*>(compressed_data);
      d_stream.avail_in = compressed_size;

      inflateInit2 (&d_stream, -MAX_WBITS);
      inflate (&d_stream, Z_FINISH);
      inflateEnd (&d_stream);
    }
  return new zip_entry_byte_buffer (out_buffer, uncompressed_size);
}

/// Converts DOS style time used in a ZIP archive to the time_t format.
static inline time_t
dostime_to_unixtime (uint32 dostime)
{
  // The DOS format appears to have only a 2 second resolution.
  // It is a 32-bit unsigned quantity, and ORs together:
  //
  // (year-1980) << 25
  // month << 21  (month is tm_mon + 1, 1=Jan through 12=Dec)
  // day << 16    (day is tm_mday, 1-31)
  // hour << 11   (hour is tm_hour, 0-23)
  // min << 5	  (min is tm_min, 0-59)
  // sec >> 1	  (sec is tm_sec, 0-59; note that we throw away the LSB)
  //
  // DOS uses local time, so the localtime() call is used to turn the time_t
  // into a struct tm.
  
  time_t now = time (NULL);
  
  // Call localtime to initialize timezone in ltime.
  struct tm ltime = *localtime (&now);
  
  ltime.tm_year = (dostime >> 25) + 80;
  ltime.tm_mon = ((dostime >> 21) & 0x0f) - 1;
  ltime.tm_mday = (dostime >> 16) & 0x1f;
  ltime.tm_hour = (dostime >> 11) & 0x0f;
  ltime.tm_min = (dostime >> 5) & 0x3f;
  ltime.tm_sec = (dostime & 0x1f) << 1;
  
  ltime.tm_wday = -1;
  ltime.tm_yday = -1;
  ltime.tm_isdst = -1;
  
  return mktime (&ltime);
}

time_t
zip_entry_reader::get_mtime ()
{
  if (last_mod_time == ~0)
    last_mod_time = dostime_to_unixtime (dos_mtime);
  return last_mod_time;
}
