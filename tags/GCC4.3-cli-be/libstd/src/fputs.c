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

LIBSTD_LPROTO_IMPL(int, fputs, const char *restrict s, FILE *restrict stream)
{
    if (stream && LIBSTD_LNAME(__io_perm_writable)(stream)) {
        size_t str_len;

        if ( s == NULL || s[0] == '\0')
            return 0;

        str_len = LIBSTD_LNAME(strlen)(s);

        if (_FILE_GET_buff(stream) == _IONBF)
            return LIBSTD_LNAME(__device_write)(stream->fd, s, (int)str_len);
        else {
            size_t room = stream->end - stream->pos;
            size_t n = str_len;
            void* nl = NULL;

            while (n > 0) {
                size_t towrite = n > room ? room : n;

                if (_FILE_GET_buff(stream) == _IOLBF) {
                    nl = LIBSTD_LNAME(memchr)(s, '\n', towrite);
                    if (nl != NULL)
                        towrite = (char*)nl - s + 1;
                }

                LIBSTD_LNAME(memcpy)(stream->pos, s, towrite);
                stream->pos += towrite;

                s    += towrite;
                n    -= towrite;
                room -= towrite;

                if (nl != NULL || room == 0) {
                    if (!LIBSTD_LNAME(__io_flush)(stream)) {
                        room = stream->end - stream->pos;
                        _FILE_SET_w(stream);
                    } else {
                        return EOF;
                    }
                }
            }
            return (int)str_len;
        }
    }
    return EOF;
}

LIBSTD_LPROTO_IMPL(int, puts, const char * s)
{
    FILE *stream = stdout;
    int puts_s_ = LIBSTD_LNAME(fputs)(s, stream);
    int puts_eol_ = LIBSTD_LNAME(fputc)('\n', stream);
    return (puts_s_== EOF) || (puts_eol_ == EOF) ? EOF : puts_s_ + puts_eol_;
}
