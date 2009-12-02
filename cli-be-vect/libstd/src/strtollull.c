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

#include <inttypes.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#include "__ctype.h"
enum {
    __STRTOLL_SIGNED = 0,
    __STRTOLL_UNSIGNED = 1
};

typedef union __strtollull_t {
    long long _ll;
    unsigned long long _ull;
} __strtollull_t;

LIBSTD_SPROTO(int, __strtollull_is_digit, int, int);
LIBSTD_SPROTO(__strtollull_t, __strtollull,const char *restrict nptr, char **restrict endptr, int base, int sflags);

LIBSTD_LPROTO_IMPL(long long, atoll, const char * nptr)
{
    __strtollull_t result = LIBSTD_SNAME(__strtollull)(nptr, (char**)NULL, 10, __STRTOLL_SIGNED);
    return result._ll;
}

LIBSTD_LPROTO_IMPL(long long, strtoll, const char *restrict nptr, char **restrict endptr, int base)
{
    __strtollull_t result = LIBSTD_SNAME(__strtollull)(nptr, endptr, base, __STRTOLL_SIGNED);
    return result._ll;
}

LIBSTD_LPROTO_IMPL(unsigned long long, strtoull, const char *restrict nptr, char **restrict endptr, int base)
{
    __strtollull_t result = LIBSTD_SNAME(__strtollull)(nptr, endptr, base, __STRTOLL_UNSIGNED);
    return result._ull;
}

LIBSTD_LPROTO_IMPL(intmax_t, strtoimax, const char *restrict nptr, char **restrict endptr, int base)
{
    __strtollull_t result = LIBSTD_SNAME(__strtollull)(nptr, endptr, base, __STRTOLL_SIGNED);
  return (intmax_t)result._ll;

}

LIBSTD_LPROTO_IMPL(uintmax_t, strtoumax, const char *restrict nptr, char **restrict endptr, int base)
{
    __strtollull_t result = LIBSTD_SNAME(__strtollull)(nptr, endptr, base, __STRTOLL_UNSIGNED);
  return (uintmax_t)result._ull;

}

LIBSTD_SPROTO_IMPL(int, __strtollull_is_digit, int c_, int base)
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

LIBSTD_SPROTO_IMPL(__strtollull_t, __strtollull, const char *restrict nptr, char **restrict endptr, int base, int sflags)
{
    __strtollull_t result;
    int neg_  = 0;
    int ov_   = 0;
    int read_ = 0;

    /* prepare default result */
    result._ull = 0L;
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
        unsigned long long overflow_      = ULLONG_MAX / base;
        unsigned long long digitoverflow_ = ULLONG_MAX % base;
        int digit_;
        unsigned long long ull_read = 0;

        for ( digit_ = LIBSTD_SNAME(__strtollull_is_digit)(nptr[0], base);
              digit_ < base;
              digit_ = LIBSTD_SNAME(__strtollull_is_digit)((++nptr)[0], base)) {
            read_ = 1;
            if (   ull_read < overflow_
                || (   ull_read == overflow_
                    && ((unsigned long long)digit_ <= digitoverflow_))) {
                ull_read = ull_read * base + digit_;
            } else {
                ov_ = 1;
            }
        }
        result._ull = ull_read;
    }

    if (!read_) {
        return result;
    } else if (   ov_
               || ( !(sflags == __STRTOLL_UNSIGNED)
                  && (   (  neg_ && (result._ull > (unsigned long long)-(LLONG_MIN+1)+1 ))
                      || ( !neg_ && (result._ull >  LLONG_MAX) ) ) ) ) {

        errno = ERANGE;

        if ( sflags == __STRTOLL_UNSIGNED ) result._ull = ULLONG_MAX;
        else if ( neg_ )                    result._ll  = LLONG_MIN;
        else                                result._ll  = LLONG_MAX;

    } else if (neg_) {
        result._ll = -(long long)result._ull;
    }

    if (endptr)
        *endptr = (char *)nptr;

    return result;
}
