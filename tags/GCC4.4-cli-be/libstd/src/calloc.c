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

#include <errno.h>
#include <stdlib.h>
#include <string.h>

LIBSTD_LPROTO_IMPL(void *, calloc, size_t nmemb, size_t size)
{
    if (nmemb > 0 && size > 0) {
        size_t msize = nmemb*size;
        void* _calloc = LIBSTD_LNAME(malloc)(msize);
        if (_calloc != NULL)
            LIBSTD_LNAME(memset)(_calloc, 0, msize);
        return _calloc;
    }
    errno = ENOMEM;
    return NULL;
}
