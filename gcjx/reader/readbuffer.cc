// Implementation of a buffer that read()s its contents.

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

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "typedefs.hh"
#include "reader/readbuffer.hh"

read_byte_buffer::read_byte_buffer (const location &w, int fd)
  : where (w)
{
  struct stat stat_buf;
  if (fstat (fd, &stat_buf) != 0 || ! S_ISREG (stat_buf.st_mode))
    // FIXME should include perror.
    throw class_file_error (where, "couldn't stat or not a regular file");

  length = stat_buf.st_size;
  data = new uint8[length];
  if ((unsigned long) read (fd, data, length) != length)
    // FIXME should include perror.
    throw class_file_error (where, "read error");
}

read_byte_buffer::~read_byte_buffer ()
{
  delete [] data;
}
