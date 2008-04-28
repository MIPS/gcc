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
#include <string.h>

#include "__io.h"


LIBSTD_LPROTO_IMPL(size_t, fread, void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream)
{
    if (   ptr && size > 0 && nmemb > 0
        && stream && LIBSTD_LNAME(__io_perm_readable)(stream)) {
        size_t left = size * nmemb;
        size_t readed = 0;

        _FILE_UNSET_unget(stream);

        if (_FILE_GET_buff(stream) == _IONBF) {
            readed = LIBSTD_LNAME(__device_read)(stream->fd, ptr, (int)left);
            if (readed != left) {
                if (readed == 0)
                    _FILE_SET_eof(stream);
                else
                    _FILE_SET_error(stream);
                if (readed == EOF)
                    readed = 0;
            }
        }
        else {
            while (left > 0) {
                size_t toread;

                if (stream->pos == stream->stop || !stream->stop) {
                    int __io_fread_ = LIBSTD_LNAME(__io_fread)(stream);
                    if (__io_fread_ == 0 || __io_fread_ == EOF)
                        return readed / size;
                }

                toread = stream->stop - stream->pos;
                toread = MIN(toread, left);

                LIBSTD_LNAME(memcpy)(ptr, stream->pos, toread);

                ptr = (void *restrict)((char *)ptr + toread);
                stream->pos += toread;
                readed       += toread;
                left         -= toread;
            }
        }
        return readed / size;
    }

    return 0;
}
