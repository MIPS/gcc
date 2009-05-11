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

#include <stdlib.h>

LIBSTD_LPROTO_IMPL(size_t, wcstombs, char *restrict s, const wchar_t *restrict pwcs, size_t n)
{
    if (pwcs != NULL) {
        size_t count = n;

        while (*pwcs > 0 && *pwcs <= 255 && count > 0) {
            *s = (char) *pwcs;
            ++pwcs;
            ++s;
            --count;
        }
        if (*pwcs == 0 || count == 0) {
            if (count > 0)
                *s = '\0';
            return n-count;
        }
    }
    return -1;
}
