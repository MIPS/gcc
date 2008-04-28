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
    char str[2];
    str[0] = c;
    str[1] = '\0';
    LIBSTD_LNAME(strcat)(p->where_.s, str);
    return 1;
}

LIBSTD_SPROTO_IMPL(int, __proxy_os, __out_proxy *p, char *s)
{
    LIBSTD_LNAME(strcat)(p->where_.s, s);
    return (int)LIBSTD_LNAME(strlen)(s);
}

LIBSTD_LPROTO_IMPL(int, vsprintf, char *restrict s, const char *restrict format, va_list arg)
{
    if (s && format) {
        __out_proxy out_ ;
        char *f_ = (char *)format;
        out_.oc_ = LIBSTD_SNAME(__proxy_oc);
        out_.os_ = LIBSTD_SNAME(__proxy_os);
        out_.where_.s = s;
        *s = '\0' ; /* So we can strcat */
        return LIBSTD_LNAME(__io_fout)(&f_, arg, &out_);
    }
    return -1;
}
