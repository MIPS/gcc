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

LIBSTD_SPROTO(char *, __fgets, char *, int, FILE *, int);

LIBSTD_SPROTO_IMPL(char *, __fgets, char *s, int size, FILE *stream, int fgetscaller)
{
    if (   s && size > 1
        && stream && LIBSTD_LNAME(__io_perm_readable)(stream) && !_FILE_GET_eof(stream)) {
        char *_s = s;

        if (_FILE_GET_buff(stream) == _IONBF) {
            int _read   = 0;
            int _toread = size-1;

            do {
                _s += _read;
                _read = LIBSTD_LNAME(__device_read)(stream->fd, _s, 1);
                _toread -= _read;
            } while (   _toread > 0
                     && _read   > 0
                     && *_s != '\n');

            if (_read > 0) {
                _s[1] = '\0';
                return s;
            }
        } else {
            unsigned char *nl_ = NULL;
            size_t left_ = size - 1;
            size_t snl_ = 0;
            int    eof  = 0;
            /* size_t toread_ ; */

            while ((left_ > 0 || fgetscaller) && !nl_ && !_FILE_GET_eof(stream)) {
                size_t toread_;

                if ((stream->pos == stream->stop) || !stream->stop)
                    LIBSTD_LNAME(__io_fread)(stream);

                toread_ = stream->stop - stream->pos;

                nl_ = (unsigned char *)LIBSTD_LNAME(memchr)(stream->pos, '\n', toread_);

                if (nl_)
                    toread_ = nl_ - stream->pos + 1;

                if (fgetscaller) {
                    if (nl_) {
                        --toread_;
                        snl_ = 1;
                    }
                }
                else
                    toread_ = MIN(left_, toread_);

                LIBSTD_LNAME(memcpy)(_s, stream->pos, toread_);
                stream->pos += toread_ + snl_;

                left_ -= toread_;
                _s += toread_;
            }

            if (s != _s) {
                _s[0] = '\0';
                _FILE_UNSET_unget(stream);
                return s;
            }
        }
    }
    return NULL;
}

LIBSTD_LPROTO_IMPL(char *, fgets, char *restrict s, int n, FILE *restrict stream)
{
    return LIBSTD_SNAME(__fgets)(s, n, stream, 0);
}

LIBSTD_LPROTO_IMPL(char *, gets, char * s)
{
    return LIBSTD_SNAME(__fgets)(s, 1, stdin, 1);
}
