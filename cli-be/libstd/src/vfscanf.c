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

#include "__ctype.h"
#include "__io.h"

LIBSTD_SPROTO(char, __proxy_ic, __in_proxy *);
LIBSTD_SPROTO(void, __proxy_uc, __in_proxy *, char);
LIBSTD_SPROTO(int,  __proxy_ma, __in_proxy *, char **, size_t *);


LIBSTD_SPROTO_IMPL(char, __proxy_ic, __in_proxy *p)
{
    return LIBSTD_LNAME(fgetc)(p->where_.f);
}

LIBSTD_SPROTO_IMPL(void, __proxy_uc, __in_proxy *p, char c)
{
    LIBSTD_LNAME(ungetc)(c, p->where_.f);
}

LIBSTD_SPROTO_IMPL(int,  __proxy_ma, __in_proxy *p, char **f, size_t *r)
{
    signed char c_;
    /* Match the input along with the format spec until % found*/
    /* First skip white space in the format statement itself*/
    while (__isspace(**f)) {
        for (c_ = LIBSTD_LNAME(fgetc)(p->where_.f); __isspace(c_); c_ = LIBSTD_LNAME(fgetc)(p->where_.f))
            (*r)++;
        LIBSTD_LNAME(ungetc)(c_, p->where_.f);
        (*f)++;
    }
    while (**f && (**f!='%'))
        if ( (*((*f)++) != (c_ = LIBSTD_LNAME(fgetc)(p->where_.f)) )) {
            LIBSTD_LNAME(ungetc)(c_, p->where_.f);
            if (c_ == EOF)
                return EOF;
            else
                return 0;
        }
        else {
            (*r)++;
        }
    if (**f== '%')
        return 1;

    return 0;
}

LIBSTD_LPROTO_IMPL(int, vfscanf, FILE *restrict stream, const char *restrict format, va_list arg)
{
    if (stream && (stream->fd != -1) && format) {
        __in_proxy in_;
        in_.ic_ = LIBSTD_SNAME(__proxy_ic);
        in_.uc_ = LIBSTD_SNAME(__proxy_uc);
        in_.ma_ = LIBSTD_SNAME(__proxy_ma);
        in_.where_.f = stream;
        return LIBSTD_LNAME(__io_fin)(format, arg, &in_);
    }
    return -1;
}
