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

LIBSTD_LPROTO_IMPL(int, fgetc, FILE *stream)
{
    if (stream && LIBSTD_LNAME(__io_perm_readable)(stream)) {
        if (!_FILE_GET_unget(stream)) {
            if (_FILE_GET_buff(stream) ==  _IONBF) {
                unsigned char _s[1];
                int _read;

                _read = LIBSTD_LNAME(__device_read)(stream->fd, _s, 1);

                if (_read == 0)
                    _FILE_SET_eof(stream);
                else if (_read < 0)
                    _FILE_SET_error(stream);
                else
                    return _s[0];

            } else {

                if (   (!stream->stop || stream->pos == stream->stop)
                    && !(LIBSTD_LNAME(__io_fread)(stream) > 0) )
                    return EOF;
                return (int)*(stream->pos++);
            }
        } else {
            _FILE_UNSET_unget(stream);
            return stream->unget_val;
        }
    }
    return EOF;
}

LIBSTD_LPROTO_IMPL(int, getc, FILE *stream)
{
    return LIBSTD_LNAME(fgetc)(stream);
}

LIBSTD_LPROTO_IMPL(int, getchar, void)
{
    return LIBSTD_LNAME(fgetc)(stdin);
}
