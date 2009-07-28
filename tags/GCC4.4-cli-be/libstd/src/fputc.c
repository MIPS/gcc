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

LIBSTD_LPROTO_IMPL(int, fputc, int c, FILE *stream)
{
    if (stream && LIBSTD_LNAME(__io_perm_writable)(stream)) {
        unsigned char _c = (unsigned char) c;

        if (_FILE_GET_buff(stream) == _IONBF) {
            int write_ = LIBSTD_LNAME(__device_write)(stream->fd, &_c, 1);
            if (write_ == -1 ) {
                _FILE_SET_error(stream);
                return EOF;
            }
        } else {
            *(stream->pos++) = _c;
            if ((stream->pos == stream->end) || ( (_FILE_GET_buff(stream) == _IOLBF) && (_c == '\n'))) {
                if (LIBSTD_LNAME(__io_flush)(stream) == EOF) {
                    _FILE_SET_error(stream);
                    return EOF;
                }
            }
        }
        return _c;
    }
    return EOF;
}

LIBSTD_LPROTO_IMPL(int, putc, int c, FILE *stream)
{
    return LIBSTD_LNAME(fputc)(c, stream);
}

LIBSTD_LPROTO_IMPL(int, putchar, int c)
{
    return LIBSTD_LNAME(fputc)(c, stdout);
}
