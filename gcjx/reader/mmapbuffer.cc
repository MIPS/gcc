// Implementation of mmap()-based buffer.

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

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "typedefs.hh"

#include "reader/mmapbuffer.hh"

mmap_byte_buffer::mmap_byte_buffer (int fd)
{
  struct stat stat_buf;
  if (fstat (fd, &stat_buf) != 0 || ! S_ISREG (stat_buf.st_mode))
    // fixme wrong exception, should include perror, etc.
    throw class_file_error (LOCATION_UNKNOWN,
			    "couldn't stat or not a regular file");

  length = stat_buf.st_size;
  data = (uint8 *) mmap (NULL, length, PROT_READ, MAP_SHARED, fd, 0);
  if (data == (uint8 *) -1)
    // fixme wrong exception, should include perror, etc.
    throw class_file_error (LOCATION_UNKNOWN, "couldn't mmap");
}

mmap_byte_buffer::~mmap_byte_buffer ()
{
  munmap (data, length);
}
