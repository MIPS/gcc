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

#include <stddef.h>
#include <string.h>

LIBSTD_LPROTO_IMPL(void *, memcpy, void *restrict dest, const void *restrict src, size_t n)
{
    void* memcpy_ = dest;
    unsigned char *restrict dest_ = (unsigned char *restrict)dest;
    unsigned const char *restrict src_ = (unsigned const char *restrict)src;

    for ( ; n > 0; --n) {
        *dest_ = *src_;
        ++dest_;
        ++src_;
    }

    return memcpy_;
}

LIBSTD_LPROTO_IMPL(void *, memccpy, void *restrict dest, const void *restrict src, int c, size_t n)
{
    unsigned char *restrict dest_ = (unsigned char *restrict)dest;
    unsigned const char *restrict src_ = (unsigned const char *restrict)src;
    char c_ = (char)c;

    for ( ; n > 0; --n) {
        if ((*dest_ = *src_) == c_) {
            return dest_ + 1;
        }
        ++dest_;
        ++src_;
    }
    return NULL;
}
