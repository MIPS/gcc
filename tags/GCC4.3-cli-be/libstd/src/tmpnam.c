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
#include <stdlib.h>
#include <errno.h>

enum { __TMP_MAX_TRY = 64 };

static const char letters_[] ="abcdefghijklmnopqrstuvwxyz0123456789";
static const size_t l_ = sizeof(letters_) - 1;

static char tmpnamb_[L_tmpnam];
LIBSTD_LPROTO_IMPL(char *, tmpnam, char *s)
{
    char* result;

    if (s) result = s;
    else   result = tmpnamb_;

    {
        size_t l           = L_tmpnam;
        const char *dir    = "/tmp";
        const char *prefix = "tmp";

        char *rand_str;
        int c_;
        int serrno = errno;

        LIBSTD_LNAME(sprintf)(result, "%s/%s", dir, prefix);
        rand_str = result + LIBSTD_LNAME(strlen)(result);

        for (c_ = 0; c_ < __TMP_MAX_TRY; ++c_) {
            int rv_;
            FILE* tmpfile_;

            rv_ = rand();

            rand_str[0] = letters_[rv_ % l_];
            rv_ /= 2;
            rand_str[1] = letters_[rv_ % l_];
            rv_ /= 3;
            rand_str[2] = letters_[rv_ % l_];
            rv_ /= 5;
            rand_str[3] = letters_[rv_ % l_];
            rv_ /= 7;
            rand_str[4] = letters_[rv_ % l_];
            rv_ /= 9;
            rand_str[5] = letters_[rv_ % l_];
            rv_ /= 11;
            rand_str[6] = '\0';

            /* Choose the mode to fail on existing files */
            tmpfile_ = LIBSTD_LNAME(fopen)(result, "r");
            if (!tmpfile_) {
                errno = serrno;
                return result;
            } else {
                LIBSTD_LNAME(fclose)(tmpfile_);
            }
        }
        errno = serrno;
    }
    return NULL;
}
