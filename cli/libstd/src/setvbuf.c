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

LIBSTD_LPROTO_IMPL(int, setvbuf, FILE *restrict stream, char *restrict buf, int mode, size_t size)
{
    if (   stream != NULL
        && stream->fd != -1
        && size >  0
        && size <= BUFSIZ) {

        /* Set new mode (if 0 used for mode, default is _IOFBF) */
        if (mode == 0)
            mode = _IOFBF;

        /* Maybe a stream buffer already existed, take care to clean-up */
        if (stream->base) {
            if (_FILE_GET_alloc(stream))
                LIBSTD_LNAME(free)(stream->base);
            _FILE_UNSET_alloc(stream);
            stream->base = NULL;
            stream->end  = NULL;
            stream->pos  = NULL;
            stream->stop = NULL;
        }

        _FILE_SET_buff(stream,mode);

        /* Wire buffer, if none is provided, allocate it*/
        if (buf != NULL) {
            _FILE_UNSET_alloc(stream);
            stream->base = (unsigned char *)buf;
            stream->end  = stream->base + size;
            stream->pos  = (unsigned char *)buf;
            stream->stop = NULL;
            return 0;
        }
        else {
            stream->base = (unsigned char *)LIBSTD_LNAME(malloc)(size);
            if (stream->base != NULL) {
                _FILE_SET_alloc(stream);
                stream->end  = stream->base + size;
                stream->pos  = stream->base;
                stream->stop = NULL;
                return 0;
            }
            else
                return -1;
        }
    }
    return -1;
}
