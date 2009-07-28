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
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "__io.h"

LIBSTD_SPROTO(int, __proxy_oc, __out_proxy *, char);
LIBSTD_SPROTO(int, __proxy_os, __out_proxy *, char *);

LIBSTD_SPROTO_IMPL(int, __proxy_oc, __out_proxy *p, char  c)
{
    if (p->maxlen > 0) {
        char str[2];
        str[0] = c;
        str[1] = '\0';
        LIBSTD_LNAME(strcat)(p->where_.s, str);
        p->maxlen -= 1;
        return 1;
    }
    return 0;
}

LIBSTD_SPROTO_IMPL(int, __proxy_os, __out_proxy *p, char *s)
{
    if (p->maxlen > 0) {
        size_t towrite = MIN(LIBSTD_LNAME(strlen)(s), p->maxlen);
        LIBSTD_LNAME(strncat)(p->where_.s, s, towrite);
        p->maxlen -= towrite;
        return (int)towrite;
    }
    return 0;
}

LIBSTD_LPROTO_IMPL(int, vsnprintf, char *restrict s, size_t n, const char *restrict format, va_list arg)
{
    if (n == 0)
        return 0;

    if (s && n > 0 && format) {
        __out_proxy out_proxy;
        char *f_ = (char *)format;
        out_proxy.maxlen = n;
        out_proxy.oc_ = LIBSTD_SNAME(__proxy_oc);
        out_proxy.os_ = LIBSTD_SNAME(__proxy_os);
        out_proxy.where_.s = s;
        *s = '\0' ; /* So we can strcat */
        return LIBSTD_LNAME(__io_fout)(&f_, arg, &out_proxy);
    }
    return -1;
}
