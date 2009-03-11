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
#include <stdlib.h>

#include "__io.h"

LIBSTD_LPROTO_IMPL(int, fclose, FILE *stream)
{
    if (stream && (stream->fd != -1)) {
        int flush_result;
        int close_result;

        flush_result = LIBSTD_LNAME(__io_flush)(stream);
        close_result = LIBSTD_LNAME(__device_close)(stream->fd);

        if (_FILE_GET_alloc(stream)) {
            _FILE_UNSET_alloc(stream);
            LIBSTD_LNAME(free)(stream->base);
        }

        if (_FILE_GET_tmp(stream) && stream->tmpnam) {
            LIBSTD_LNAME(remove)(stream->tmpnam);
            LIBSTD_LNAME(free)(stream->tmpnam);
        }

        LIBSTD_LNAME(__io_ftable_release_file)(stream);

        if ((flush_result != EOF) && (close_result != -1))
            return 0;
    }
    return EOF;
}
