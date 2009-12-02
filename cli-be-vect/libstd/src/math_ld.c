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

#include <__cdefs.h>

#include <__host.h>
#include <math.h>

#define xLIBSTD_LPROTO_IMPL(a, fname,...)   /* fname implementation missing */

LIBSTD_LPROTO_IMPL(long double, fabsl, long double p0)
{
    return (long double)LIBSTD_HNAME(fabs)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, ceill, long double p0)
{
    return (long double)LIBSTD_HNAME(ceil)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, floorl, long double p0)
{
    return (long double)LIBSTD_HNAME(floor)((double)p0);
}

xLIBSTD_LPROTO_IMPL(long double, nearbyintl, long double);

xLIBSTD_LPROTO_IMPL(long double, rintl, long double);

xLIBSTD_LPROTO_IMPL(long, lrintl, long double);

xLIBSTD_LPROTO_IMPL(long long, llrintl, long double);

xLIBSTD_LPROTO_IMPL(long double, roundl, long double);

LIBSTD_LPROTO_IMPL(long, lroundl, long double p0)
{
    return LIBSTD_HNAME(lround)((double)p0);
}


xLIBSTD_LPROTO_IMPL(long long, llroundl, long double);

xLIBSTD_LPROTO_IMPL(long double, truncl, long double);

xLIBSTD_LPROTO_IMPL(long double, fmodl, long double, long double);

xLIBSTD_LPROTO_IMPL(long double, remainderl, long double, long double);

xLIBSTD_LPROTO_IMPL(long double, remquol, long double, long double, int *);

LIBSTD_LPROTO_IMPL(long double, frexpl, long double num, int* exp)
{
    long double res;
    long double pos_num;

    if (isnan(num)) {
        return num;
    }

    pos_num = (num >= 0.0 ? num : -num);
    if (num == 0.0) {
        *exp = 0;
        res = num;
    }
    else if ((pos_num >= 0.5) && (pos_num < 1.0)) {
        /* num already matches the constraints */
        *exp = 0;
        res = num;
    }
    /* compute e = 1+floor(log2l(num)) */
    else if (pos_num >= 1.0) {
        int e = (int)log2l(pos_num) + 1; /* e >= 0 */
        *exp = e;
        res = num / (double)(1 << e);
    }
    else {
        int e = 1 + (int)floor(log2l(pos_num));
        *exp = e;
        res = num * (double)(1 << -e);
    }
    return res;
}

LIBSTD_LPROTO_IMPL(long double, ldexpl, long double p0, int p1)
{
    return (long double)LIBSTD_LNAME(ldexp)((double)p0, p1);
}

xLIBSTD_LPROTO_IMPL(long double, modfl, long double, long double *);

LIBSTD_LPROTO_IMPL(long double, scalbnl, long double p0, int p1)
{
    return (long double)LIBSTD_LNAME(scalbn)((double)p0, p1);
}

LIBSTD_LPROTO_IMPL(long double, scalblnl, long double p0, long p1)
{
    return (long double)LIBSTD_LNAME(scalbln)((double)p0, p1);
}


LIBSTD_LPROTO_IMPL(long double, expl, long double p0)
{
    return (long double)LIBSTD_HNAME(exp)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, exp2l, long double p0)
{
    return (long double)LIBSTD_HNAME(exp2)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, expm1l, long double p0)
{
    return (long double)LIBSTD_HNAME(expm1)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, logl, long double p0)
{
    return (long double)LIBSTD_HNAME(log)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, log10l, long double p0)
{
    return (long double)LIBSTD_HNAME(log10)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, log1pl, long double p0)
{
    return (long double)LIBSTD_HNAME(log1p)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, log2l, long double p0)
{
    return (long double)LIBSTD_HNAME(log2)((double)p0);
}

xLIBSTD_LPROTO_IMPL(long double, logbl, long double);

xLIBSTD_LPROTO_IMPL(int, ilogbl, long double);

LIBSTD_LPROTO_IMPL(long double, cbrtl, long double p0)
{
    return (long double)LIBSTD_HNAME(cbrt)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, hypotl, long double p0, long double p1)
{
    return (long double)LIBSTD_LNAME(hypot)((double)p0, (double)p1);
}

xLIBSTD_LPROTO_IMPL(long double, fmal, long double, long double, long double);

LIBSTD_LPROTO_IMPL(long double, powl, long double p0, long double p1)
{
    return (long double)LIBSTD_HNAME(pow)((double)p0, (double)p1);
}

LIBSTD_LPROTO_IMPL(long double, sqrtl, long double p0)
{
    return (long double)LIBSTD_HNAME(sqrt)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, cosl, long double p0)
{
    return (long double)LIBSTD_HNAME(cos)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, sinl, long double p0)
{
    return (long double)LIBSTD_HNAME(sin)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, tanl, long double p0)
{
    return (long double)LIBSTD_HNAME(tan)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, coshl, long double p0)
{
    return (long double)LIBSTD_HNAME(cosh)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, sinhl, long double p0)
{
    return (long double)LIBSTD_HNAME(sinh)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, tanhl, long double p0)
{
    return (long double)LIBSTD_HNAME(tanh)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, acosl, long double p0)
{
    return (long double)LIBSTD_HNAME(acos)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, asinl, long double p0)
{
    return (long double)LIBSTD_HNAME(asin)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, atanl, long double p0)
{
    return (long double)LIBSTD_HNAME(atan)((double)p0);
}

LIBSTD_LPROTO_IMPL(long double, atan2l, long double p0, long double p1)
{
    return (long double)LIBSTD_HNAME(atan2)((double)p0, (double)p1);
}

xLIBSTD_LPROTO_IMPL(long double, acoshl, long double);

xLIBSTD_LPROTO_IMPL(long double, asinhl, long double);

xLIBSTD_LPROTO_IMPL(long double, atanhl, long double);

xLIBSTD_LPROTO_IMPL(long double, fdiml, long double, long double);

xLIBSTD_LPROTO_IMPL(long double, fmaxl, long double, long double);

xLIBSTD_LPROTO_IMPL(long double, fminl, long double, long double);

xLIBSTD_LPROTO_IMPL(long double, erfl, long double);

xLIBSTD_LPROTO_IMPL(long double, erfcl, long double);

xLIBSTD_LPROTO_IMPL(long double, lgammal, long double);

xLIBSTD_LPROTO_IMPL(long double, tgammal, long double);

LIBSTD_LPROTO_IMPL(long double, copysignl, long double p0, long double p1)
{
    return (long double)LIBSTD_LNAME(copysign)((double)p0, (double)p1);
}

xLIBSTD_LPROTO_IMPL(long double, nanl, const char *);

xLIBSTD_LPROTO_IMPL(long double, nextafterl, long double, long double);

xLIBSTD_LPROTO_IMPL(long double, nexttowardl, long double, long double);
