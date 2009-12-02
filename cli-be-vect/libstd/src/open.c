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
#include <fcntl.h>

#include "__io.h"

LIBSTD_LPROTO_IMPL(int, open, const char *name, int flags, ...)
{
    FILE* fp = LIBSTD_LNAME(__io_ftable_new_file)();

    if (fp != NULL) {

        if ((flags & O_BINARY) != 0)
            _FILE_SET_bin(fp);

        if ((flags & (O_RDWR|O_APPEND|O_WRONLY)) != 0)
            flags |= O_CREAT | O_TRUNC;

        if ((flags & O_RDWR) != 0 || (flags & O_APPEND) != 0)
            _FILE_SET_rw(fp);
        else if ((flags & O_WRONLY) != 0)
            _FILE_SET_w(fp);
        else if  ((flags & O_RDONLY) != 0)
            _FILE_SET_r(fp);

        /* Call the device specific low-level function */
        fp->fd = LIBSTD_LNAME(__device_open)(name, flags);
    }

    if (fp == NULL || fp->fd < 0) {
        /* we failed, clean up this entry */
        LIBSTD_LNAME(__io_ftable_release_file)(fp);
        return -1;
    }

    return fp->fd;
}
