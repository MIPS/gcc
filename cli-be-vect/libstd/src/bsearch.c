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

LIBSTD_LPROTO_IMPL(void *, bsearch, const void *key, const void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *) )
{
    size_t low;
    size_t high;
    size_t mid;
    const void* where;
    int cmp;

    low  = 0;
    high = nmemb;
    while (low < high) {
        mid = (low + high) / 2;
        where = (const void*)((const char*)base + (mid * size));
        cmp = (*compar) (key, where);

        if (cmp == 0)      return (void*)where;
        else if (cmp < 0)  high = mid;
        else               low = mid + 1;
    }

    return NULL;
}
