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
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#include "__ctype.h"

enum {
    __STRTOL_SIGNED = 0,
    __STRTOL_UNSIGNED = 1
};

typedef union __strtolul_t {
    long _l;
    unsigned long _ul;
} __strtolul_t;

static int __strtolul_is_digit(int, int);
LIBSTD_SPROTO(__strtolul_t, __strtolul,const char *restrict, char **restrict, int, int);

LIBSTD_LPROTO_IMPL(int, atoi, const char *nptr)
{
    __strtolul_t result = LIBSTD_SNAME(__strtolul)(nptr, (char**)NULL, 10, __STRTOL_SIGNED);
    return (int)result._l;
}

LIBSTD_LPROTO_IMPL(long, atol, const char * nptr)
{
    __strtolul_t result = LIBSTD_SNAME(__strtolul)(nptr, (char**)NULL, 10, __STRTOL_SIGNED);
    return result._l;
}

LIBSTD_LPROTO_IMPL(long, strtol, const char *restrict nptr, char **restrict endptr, int base)
{
    __strtolul_t result = LIBSTD_SNAME(__strtolul)(nptr, endptr, base, __STRTOL_SIGNED);
    return result._l;
}

LIBSTD_LPROTO_IMPL(unsigned long, strtoul,const char *restrict nptr, char **restrict endptr, int base)
{
    __strtolul_t result = LIBSTD_SNAME(__strtolul)(nptr, endptr, base, __STRTOL_UNSIGNED);
    return result._ul;
}

static int __strtolul_is_digit(int c_, int base)
{
    int result;
    if (__isdigit(c_))
        result = c_ - '0';
    else if (__isalpha(c_))
        result = __toupper(c_) - 'A' + 10;
    else
        result = base;
    return result;
}

LIBSTD_SPROTO_IMPL(__strtolul_t, __strtolul,const char *restrict nptr, char **restrict endptr, int base, int sflags)
{
    __strtolul_t result;
    int neg_  = 0;
    int ov_   = 0;
    int read_ = 0;

    /* prepare default result */
    result._ul = 0L;
    if (endptr)
        *endptr = (char *)nptr;

    for ( ; __isspace((int)nptr[0]); ++nptr) {
        /* Skip space */
    }

    switch (nptr[0]) {
    case '-':
        neg_ = 1;
        /* Fall through */
    case '+':
        ++nptr;
        break;
    default:
        break;
    }

    /* Check base */
    if (base < 0 || base == 1 || base > 36) {
        return result;
    }
    else if (base == 0) {
        if (nptr[0] != '0')
            base = 10;
        else if (nptr[1] == 'x' || nptr[1] == 'X')
            base = 16;
        else
            base = 8;
    }

    if (base == 16 && (nptr[0] == '0' && (nptr[1] =='x' || nptr[1] == 'X' )))
        nptr = nptr + 2;

    {
        unsigned long overflow_      = ULONG_MAX / base;
        unsigned long digitoverflow_ = ULONG_MAX % base;
        int digit_;
        unsigned long ul_read = 0;

        for ( digit_ = __strtolul_is_digit(nptr[0], base);
              digit_ < base;
              digit_ = __strtolul_is_digit((++nptr)[0], base)) {
            read_ = 1;
            if (   ul_read < overflow_
                || (   ul_read == overflow_
                    && ((unsigned long)digit_ <= digitoverflow_))) {
                ul_read = ul_read * base + digit_;
            } else {
                ov_ = 1;
            }
        }
        result._ul = ul_read;
    }

    if (!read_) {
        return result;
    } else if (   ov_
               || ( !(sflags == __STRTOL_UNSIGNED)
                  && (   (  neg_ && (-(long long)result._ul < (long long)LONG_MIN) )
                      || ( !neg_ && (result._ul >  LONG_MAX) ) ) ) ) {

        errno = ERANGE;

        if ( sflags == __STRTOL_UNSIGNED ) result._ul = ULONG_MAX;
        else if ( neg_ )                   result._l  = LONG_MIN;
        else                               result._l  = LONG_MAX;

    } else if (neg_) {
        result._l = -(long)result._ul;
    }

    if (endptr)
        *endptr = (char *)nptr;

    return result;
}
