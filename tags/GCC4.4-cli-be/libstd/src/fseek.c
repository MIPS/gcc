/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD

#include <stdio.h>
#include <errno.h>

#include "__io.h"

LIBSTD_LPROTO_IMPL(int, fseek, FILE *stream, long offset, int whence)
{
    if (whence==SEEK_SET || whence==SEEK_CUR || whence==SEEK_END ) {
        if (stream && (stream->fd != -1)) {

            if ((whence == SEEK_CUR) && _FILE_GET_r(stream))
                offset -=  (long)(stream->stop - stream->pos);

            LIBSTD_LNAME(__io_flush)(stream);

            _FILE_UNSET_unget(stream);
            _FILE_UNSET_eof(stream);
            if (_FILE_GET_rw(stream)) {
                _FILE_UNSET_r(stream);
                _FILE_UNSET_w(stream);
            }

            if (LIBSTD_LNAME(__device_lseek)(stream->fd, offset, whence) != -1)
                return 0;
        }
    }
    else {
        errno = EINVAL;
    }

    return -1;
}
