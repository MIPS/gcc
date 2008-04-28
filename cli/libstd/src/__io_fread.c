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

#include "__io.h"

LIBSTD_LPROTO_IMPL(int, __io_fread, FILE *fp)
{
    if (fp) {
        int read;
        size_t size = fp->end - fp->base;

        if (_FILE_GET_buff(fp) == _IOLBF)
            LIBSTD_LNAME(__io_flush)(fp);

        read = LIBSTD_LNAME(__device_read)(fp->fd, fp->base, (int)size);

        fp->pos  = fp->base;
        fp->stop = fp->base + read;

        if (read == 0)
            _FILE_SET_eof(fp);
        else if (read < 0)
            _FILE_SET_error(fp);

        return read;
    }

    return EOF;
}
