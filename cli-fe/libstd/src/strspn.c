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

#define __LIBSTD_CRT_BUILD

#include <string.h>

LIBSTD_LPROTO_IMPL(size_t, strspn, const char* s1, const char *s2)
{
    const char *s1_start = s1;
    const char *_s2;

    for ( ; *s1 != '\0'; ++s1) {
        for (_s2 = s2; *_s2 != '\0'; ++_s2)
            if (*s1 == *_s2)
                break;
        if (*_s2 == '\0')
            return s1 - s1_start;
    }

    return s1 - s1_start;
}
