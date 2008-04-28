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

LIBSTD_LPROTO_IMPL(size_t, fwrite, const void * restrict ptr, size_t size, size_t nmemb, FILE *restrict stream)
{
    if (size != 0 && nmemb != 0 && ptr
        && stream && LIBSTD_LNAME(__io_perm_writable)(stream)) {
        size_t left_ = size * nmemb;
        size_t written_ = 0;

        switch (_FILE_GET_buff(stream)) {
        case _IONBF:
            written_ = (size_t)LIBSTD_LNAME(__device_write)(stream->fd, ptr, (int)left_);
            if (written_ != left_) {
                _FILE_SET_error(stream);
                if ((int)written_ == -1)
                    written_ = 0;
            }
            break;

        case _IOLBF:
            {
                size_t bufsize_ = stream->end - stream->base;
                size_t room_    = stream->end - stream->pos;

                while (left_ > 0) {
                    size_t towrite_;
                    size_t nl_;
                    {
                        unsigned char *pnl_ = (unsigned char*)LIBSTD_LNAME(memchr)(ptr, '\n', left_);
                        if (pnl_) {
                            nl_ = (pnl_ - (unsigned char *)ptr) + 1;
                            towrite_ = (nl_ < room_) ? nl_ : (left_ < room_) ? left_ : room_;
                        }
                        else if (left_ < room_) {
                            LIBSTD_LNAME(memcpy)(stream->pos, ptr, left_);
                            stream->pos += left_;
                            written_     += left_;
                            return (written_ / size);
                        } else {
                            nl_ = room_ + 1;
                            towrite_ = room_;
                        }
                    }

                    LIBSTD_LNAME(memcpy)(stream->pos, ptr, towrite_);
                    stream->pos += towrite_;
                    room_        -= towrite_;

                    if (room_ == 0 || towrite_ == nl_) {
                        if (LIBSTD_LNAME(__io_flush)(stream) == 0) {
                            room_ = bufsize_;
                            _FILE_SET_w(stream);
                        } else {
                            _FILE_SET_error(stream);
                            return (written_ / size);
                        }
                    }

                    written_ += towrite_;
                    ptr      = (unsigned char *)ptr + towrite_;
                    left_    -= towrite_;

                }
            }
            break;
        case _IOFBF:
            {
                size_t bufsize_ = stream->end - stream->base;
                size_t room_    = stream->end - stream->pos;

                while (left_ > 0 ) {

                    size_t towrite_ = MIN(room_, left_);

                    LIBSTD_LNAME(memcpy)(stream->pos, ptr, towrite_);
                    stream->pos += towrite_;
                    room_        -= towrite_;

                    if (room_ == 0) {
                        if (LIBSTD_LNAME(__io_flush)(stream) == 0) {
                            room_ = bufsize_;
                            _FILE_SET_w(stream);
                        } else {
                            _FILE_SET_error(stream);
                            return (written_ / size);
                        }
                    }

                    written_ += towrite_;
                    ptr      = (unsigned char *)ptr + towrite_;
                    left_    -= towrite_;

                }
            }
            break;

        default:
            break;
        }

        return (written_ / size);
    }

    return 0;
}
