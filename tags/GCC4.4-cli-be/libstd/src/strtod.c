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
#include <limits.h>
#include <float.h>
#include <math.h>
#include <errno.h>

#include "__ctype.h"

LIBSTD_LPROTO_IMPL(double, atof, const char *restrict nptr)
{
    return LIBSTD_LNAME(strtod)(nptr, (char**)NULL);
}

LIBSTD_LPROTO_IMPL(float, strtof, const char *restrict nptr, char **restrict endptr)
{
    return (float)LIBSTD_LNAME(strtod)(nptr, endptr);
}

LIBSTD_LPROTO_IMPL(long double, strtold,const char *restrict nptr, char **restrict endptr)
{
    return (long double)LIBSTD_LNAME(strtod)(nptr, endptr);
}

static const double  pw10_[] = { 1.e+1, 1.e+2, 1.e+4, 1.e+8,1.e+16, 1.e+32, 1.e+64, 1.e+128, 1.e+256 };
static const double ipw10_[] = { 1.e-1, 1.e-2, 1.e-4, 1.e-8,1.e-16, 1.e-32, 1.e-64, 1.e-128, 1.e-256 };

LIBSTD_LPROTO_IMPL(double, strtod, const char * restrict s, char ** restrict endptr)
{
    double strtod_ = 0;
    int exp_ = 0;
    int sign_ = 0;
    int success_ = 0;

    /* prepare default result */
    if (endptr)
        *endptr = (char *)s;

    for ( ; __isspace((int)s[0]); ++s) {
        /* Skip space */
    }

    switch (s[0]) {
    case '-':
        sign_ = 1;
        /* Fall through */
    case '+':
        ++s;
        break;
    default:
        break;
    }

    /* Read frac. part, until exp, counting digits after . */
    while (__isdigit((int)s[0])) {
        strtod_ = strtod_ * 10 + (s[0] - '0');
        success_  = 1;
        ++s;
    }

    if (s[0] == '.') {
        ++s;
        while (__isdigit((int)s[0]))  {
            strtod_ = strtod_ * 10 + (s[0] - '0');
            success_ = 1;
            --exp_;
            ++s;
        }
    }

    if (sign_)
        strtod_ = -strtod_;

    if (success_ && (s[0] == 'e' || s[0]=='E')) {
        int rsign = 0; /* 0= not specified, 1= negative, 2= positive */
        ++s;

        switch (s[0]) {
        case '-':
            rsign = 1;
            ++s;
        case '+':
            rsign = 2;
            ++s;
            break;
        default:
            break;
        }

        if (__isdigit(s[0])) {
            int count = 0;
            for ( ; __isdigit(s[0]); ++s) {
                if ((INT_MAX - (exp_ < 0 ? -exp_ : exp_) - (s[0] - '0')) / 10 > count) {
                    count = count * 10 + (s[0] - '0');
                } else {
                    count = INT_MAX - exp_;
                    break;
                }
            }
            if (rsign==1)
                exp_ -= count;
            else
                exp_ += count;
        }
        else if (rsign)
            s -= 2;
        else
            s -= 1;
    }

    /* Adjust number by power of 10.*/
    if (strtod_ != 0.0) {
        /* ceil(log2(DBL_MAX_10_EXP)) is 9, also for ceil(log2(abs(DBL_MIN_10_EXP)))  */
        if (exp_ > DBL_MAX_10_EXP) {
            errno = ERANGE;
            strtod_ = (strtod_ < 0) ? -HUGE_VAL : HUGE_VAL;
        }
        else if (exp_ < DBL_MIN_10_EXP) {
            errno = ERANGE;
            strtod_ = 0.0;
        }
        else if (exp_ < 0) {
            int count = 0;
            exp_ = -exp_;
            for (; exp_; ++count, exp_ >>= 1) {
                if (exp_ & 1)
                    strtod_ *= ipw10_[count];
            }
        }
        else {
            int count = 0;
            for (; exp_; ++count, exp_ >>= 1) {
                if (exp_ & 1)
                    strtod_ *= pw10_[count];
            }
        }
    }

    /* Change only if endptr valid and value was translated */
    if (endptr && success_)
        *endptr = (char *)s;

    return strtod_;
}
