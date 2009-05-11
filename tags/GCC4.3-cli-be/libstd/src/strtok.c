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

static char* pos = NULL;

LIBSTD_LPROTO_IMPL(char *, strtok, char *restrict s1, const char *restrict s2)
{
    if (s1 != NULL)
        pos = s1;

    if (pos != NULL) {
        pos += LIBSTD_LNAME(strspn)(pos, s2);

        if (*pos) {
            char *result = pos;
            pos += LIBSTD_LNAME(strcspn)(pos, s2);

            if (*pos)
                *pos++ = '\0';
            return result;
        }
    }

    return NULL;
}
