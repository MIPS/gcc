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

LIBSTD_SPROTO(void, __libstd__init_fp, void);

static int __local__fp_init = 0;
static double __local__dbl_epsilon;
static double __local__dbl_minval;
static double __local__dbl_maxval;
static double __local__dbl_hugeval;
static float  __local__flt_epsilon;
static float  __local__flt_minval;
static float  __local__flt_maxval;
static float  __local__flt_hugeval;
static float  __local__flt_nan;
static float  __local__flt_infinity;

LIBSTD_SPROTO_IMPL(void, __libstd__init_fp, void)
{
    __local__dbl_epsilon  = LIBSTD_HNAME(dbl_epsilon)();
    __local__dbl_minval   = LIBSTD_HNAME(dbl_minval)();
    __local__dbl_maxval   = LIBSTD_HNAME(dbl_maxval)();
    __local__dbl_hugeval  = LIBSTD_HNAME(dbl_hugeval)();
    __local__flt_epsilon  = LIBSTD_HNAME(flt_epsilon)();
    __local__flt_minval   = LIBSTD_HNAME(flt_minval)();
    __local__flt_maxval   = LIBSTD_HNAME(flt_maxval)();
    __local__flt_hugeval  = LIBSTD_HNAME(flt_hugeval)();
    __local__flt_nan      = LIBSTD_HNAME(flt_nan)();
    __local__flt_infinity = LIBSTD_HNAME(flt_infinity)();
}

LIBSTD_LPROTO_IMPL(double, __dbl_epsilon, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__dbl_epsilon;
}

LIBSTD_LPROTO_IMPL(double, __dbl_minval, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__dbl_minval;
}

LIBSTD_LPROTO_IMPL(double, __dbl_maxval, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__dbl_maxval;
}

LIBSTD_LPROTO_IMPL(double, __dbl_hugeval, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__dbl_hugeval;
}

LIBSTD_LPROTO_IMPL(float, __flt_epsilon, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__flt_epsilon;
}

LIBSTD_LPROTO_IMPL(float, __flt_minval, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__flt_minval;
}

LIBSTD_LPROTO_IMPL(float, __flt_maxval, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__flt_maxval;
}

LIBSTD_LPROTO_IMPL(float, __flt_hugeval, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__flt_hugeval;
}

LIBSTD_LPROTO_IMPL(float, __flt_nan, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__flt_nan;
}

LIBSTD_LPROTO_IMPL(float, __flt_infinity, void)
{
    if (__local__fp_init == 0)
        LIBSTD_SNAME(__libstd__init_fp)();
    return __local__flt_infinity;
}

#define xPROTO_IMPL(a, fname,...)   /* fname implementation missing */

LIBSTD_LPROTO_IMPL(double, fabs, double p0)
{
    return LIBSTD_HNAME(fabs)(p0);
}

LIBSTD_LPROTO_IMPL(float, fabsf, float p0)
{
    return LIBSTD_HNAME(fabsf)(p0);
}

LIBSTD_LPROTO_IMPL(double, ceil, double p0)
{
    return LIBSTD_HNAME(ceil)(p0);
}

LIBSTD_LPROTO_IMPL(float, ceilf, float p0)
{
    return LIBSTD_HNAME(ceilf)(p0);
}

LIBSTD_LPROTO_IMPL(double, floor, double p0)
{
    return LIBSTD_HNAME(floor)(p0);
}

LIBSTD_LPROTO_IMPL(float, floorf, float p0)
{
    return LIBSTD_HNAME(floorf)(p0);
}

xPROTO_IMPL(double, nearbyint, double);
xPROTO_IMPL(float, nearbyintf, float);

xPROTO_IMPL(double, rint, double);
xPROTO_IMPL(float, rintf, float);

xPROTO_IMPL(long, lrint, double);
xPROTO_IMPL(long, lrintf, float);

xPROTO_IMPL(long long, llrint, double);
xPROTO_IMPL(long long, llrintf, float);

xPROTO_IMPL(double, round, double);
xPROTO_IMPL(float, roundf, float);

LIBSTD_LPROTO_IMPL(long, lround, double p0)
{
    return LIBSTD_HNAME(lround)(p0);
}

LIBSTD_LPROTO_IMPL(long, lroundf, float p0)
{
    return LIBSTD_HNAME(lroundf)(p0);
}

xPROTO_IMPL(long long, llround, double);
xPROTO_IMPL(long long, llroundf, float);

xPROTO_IMPL(double, trunc, double);
xPROTO_IMPL(float, truncf, float);

xPROTO_IMPL(double, fmod, double, double);
xPROTO_IMPL(float, fmodf, float, float);

xPROTO_IMPL(double, remainder, double, double);
xPROTO_IMPL(float, remainderf, float, float);

xPROTO_IMPL(double, remquo, double, double, int *);
xPROTO_IMPL(float, remquof, float, float, int *);

LIBSTD_LPROTO_IMPL(double, frexp, double num, int* exp)
{
    double res;
    double pos_num;

    if (LIBSTD_HNAME(fpclassify)(num) == FP_NAN) {
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
    /* compute e = 1+floor(log2(num)) */
    else if (pos_num >= 1.0) {
        int e = (int)LIBSTD_HNAME(log2)(pos_num) + 1;
        *exp = e;
        res = num / (double)(1 << e);
    }
    else {
        int e = 1 + (int)LIBSTD_HNAME(floor)(LIBSTD_HNAME(log2)(pos_num));
        *exp = e;
        res = num * (double)(1 << -e);
    }
    return res;
}

LIBSTD_LPROTO_IMPL(float, frexpf, float num, int* exp)
{
    float res;
    float pos_num;

    if (LIBSTD_HNAME(fpclassify)(num) == FP_NAN) {
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
    /* compute e = 1+floor(log2f(num)) */
    else if (pos_num >= 1.0) {
        int e = (int)LIBSTD_HNAME(log2f)(pos_num) + 1;
        *exp = e;
        res = num / (double)(1 << e);
    }
    else {
        int e = 1 + (int)LIBSTD_HNAME(floor)(LIBSTD_HNAME(log2f)(pos_num));
        *exp = e;
        res = num * (double)(1 << -e);
    }
    return res;
}

LIBSTD_LPROTO_IMPL(double, ldexp, double p0, int p1)
{
    return LIBSTD_HNAME(ldexp)(p0,p1);
}

LIBSTD_LPROTO_IMPL(float, ldexpf, float p0, int p1)
{
    return LIBSTD_HNAME(ldexpf)(p0,p1);
}

xPROTO_IMPL(double, modf, double, double *);
xPROTO_IMPL(float, modff, float, float *);

LIBSTD_LPROTO_IMPL(double, scalbn, double p0, int p1)
{
    return LIBSTD_HNAME(scalbn)(p0,p1);
}

LIBSTD_LPROTO_IMPL(float, scalbnf, float p0, int p1)
{
    return LIBSTD_HNAME(scalbnf)(p0,p1);
}

LIBSTD_LPROTO_IMPL(double, scalbln, double p0, long p1)
{
    return LIBSTD_HNAME(scalbn)(p0,p1);
}

LIBSTD_LPROTO_IMPL(float, scalblnf, float p0, long p1)
{
    return LIBSTD_HNAME(scalblnf)(p0,p1);
}

LIBSTD_LPROTO_IMPL(double, exp, double p0)
{
    return LIBSTD_HNAME(exp)(p0);
}

LIBSTD_LPROTO_IMPL(float, expf, float p0)
{
    return LIBSTD_HNAME(expf)(p0);
}

LIBSTD_LPROTO_IMPL(double, exp2, double p0)
{
    return LIBSTD_HNAME(exp2)(p0);
}

LIBSTD_LPROTO_IMPL(float, exp2f, float p0)
{
    return LIBSTD_HNAME(exp2f)(p0);
}

LIBSTD_LPROTO_IMPL(double, expm1, double p0)
{
    return LIBSTD_HNAME(expm1)(p0);
}

LIBSTD_LPROTO_IMPL(float, expm1f, float p0)
{
    return LIBSTD_HNAME(expm1f)(p0);
}

LIBSTD_LPROTO_IMPL(double, log, double p0)
{
    return LIBSTD_HNAME(log)(p0);
}

LIBSTD_LPROTO_IMPL(float, logf, float p0)
{
    return LIBSTD_HNAME(logf)(p0);
}

LIBSTD_LPROTO_IMPL(double, log10, double p0)
{
    return LIBSTD_HNAME(log10)(p0);
}

LIBSTD_LPROTO_IMPL(float, log10f, float p0)
{
    return LIBSTD_HNAME(log10f)(p0);
}

LIBSTD_LPROTO_IMPL(double, log1p, double p0)
{
    return LIBSTD_HNAME(log1p)(p0);
}

LIBSTD_LPROTO_IMPL(float, log1pf, float p0)
{
    return LIBSTD_HNAME(log1pf)(p0);
}

LIBSTD_LPROTO_IMPL(double, log2, double p0)
{
    return LIBSTD_HNAME(log2)(p0);
}

LIBSTD_LPROTO_IMPL(float, log2f, float p0)
{
    return LIBSTD_HNAME(log2f)(p0);
}

xPROTO_IMPL(double, logb, double);
xPROTO_IMPL(float, logbf, float);

xPROTO_IMPL(int, ilogb, double);
xPROTO_IMPL(int, ilogbf, float);

LIBSTD_LPROTO_IMPL(double, cbrt, double p0)
{
    return LIBSTD_HNAME(cbrt)(p0);
}

LIBSTD_LPROTO_IMPL(float, cbrtf, float p0)
{
    return LIBSTD_HNAME(cbrtf)(p0);
}

LIBSTD_LPROTO_IMPL(double, hypot, double p0, double p1)
{
    return LIBSTD_HNAME(hypot)(p0, p1);
}

LIBSTD_LPROTO_IMPL(float, hypotf, float p0, float p1)
{
    return LIBSTD_HNAME(hypotf)(p0, p1);
}

xPROTO_IMPL(double, fma, double, double, double);
xPROTO_IMPL(float, fmaf, float, float, float);

LIBSTD_LPROTO_IMPL(double, pow, double p0, double p1)
{
    return LIBSTD_HNAME(pow)(p0, p1);
}

LIBSTD_LPROTO_IMPL(float, powf, float p0, float p1)
{
    return LIBSTD_HNAME(powf)(p0, p1);
}


LIBSTD_LPROTO_IMPL(double, sqrt, double p0)
{
    return LIBSTD_HNAME(sqrt)(p0);
}

LIBSTD_LPROTO_IMPL(float, sqrtf, float p0)
{
    return LIBSTD_HNAME(sqrtf)(p0);
}

LIBSTD_LPROTO_IMPL(double, cos, double p0)
{
    return LIBSTD_HNAME(cos)(p0);
}

LIBSTD_LPROTO_IMPL(float, cosf, float p0)
{
    return LIBSTD_HNAME(cosf)(p0);
}

LIBSTD_LPROTO_IMPL(double, sin, double p0)
{
    return LIBSTD_HNAME(sin)(p0);
}

LIBSTD_LPROTO_IMPL(float, sinf, float p0)
{
    return LIBSTD_HNAME(sinf)(p0);
}

LIBSTD_LPROTO_IMPL(double, tan, double p0)
{
    return LIBSTD_HNAME(tan)(p0);
}

LIBSTD_LPROTO_IMPL(float, tanf, float p0)
{
    return LIBSTD_HNAME(tanf)(p0);
}

LIBSTD_LPROTO_IMPL(double, cosh, double p0)
{
    return LIBSTD_HNAME(cosh)(p0);
}

LIBSTD_LPROTO_IMPL(float, coshf, float p0)
{
    return LIBSTD_HNAME(coshf)(p0);
}

LIBSTD_LPROTO_IMPL(double, sinh, double p0)
{
    return LIBSTD_HNAME(sinh)(p0);
}

LIBSTD_LPROTO_IMPL(float, sinhf, float p0)
{
    return LIBSTD_HNAME(sinhf)(p0);
}

LIBSTD_LPROTO_IMPL(double, tanh, double p0)
{
    return LIBSTD_HNAME(tanh)(p0);
}

LIBSTD_LPROTO_IMPL(float, tanhf, float p0)
{
    return LIBSTD_HNAME(tanhf)(p0);
}

LIBSTD_LPROTO_IMPL(double, acos, double p0)
{
    return LIBSTD_HNAME(acos)(p0);
}

LIBSTD_LPROTO_IMPL(float, acosf, float p0)
{
    return LIBSTD_HNAME(acosf)(p0);
}

LIBSTD_LPROTO_IMPL(double, asin, double p0)
{
    return LIBSTD_HNAME(asin)(p0);
}

LIBSTD_LPROTO_IMPL(float, asinf, float p0)
{
    return LIBSTD_HNAME(asinf)(p0);
}

LIBSTD_LPROTO_IMPL(double, atan, double p0)
{
    return LIBSTD_HNAME(atan)(p0);
}

LIBSTD_LPROTO_IMPL(float, atanf, float p0)
{
    return LIBSTD_HNAME(atanf)(p0);
}

LIBSTD_LPROTO_IMPL(double, atan2, double p0, double p1)
{
    return LIBSTD_HNAME(atan2)(p0, p1);
}

LIBSTD_LPROTO_IMPL(float, atan2f, float p0, float p1)
{
    return LIBSTD_HNAME(atan2f)(p0, p1);
}

xPROTO_IMPL(double, acosh, double);
xPROTO_IMPL(float, acoshf, float);

xPROTO_IMPL(double, asinh, double);
xPROTO_IMPL(float, asinhf, float);

xPROTO_IMPL(double, atanh, double);
xPROTO_IMPL(float, atanhf, float);

xPROTO_IMPL(double, fdim, double, double);
xPROTO_IMPL(float, fdimf, float, float);

xPROTO_IMPL(double, fmax, double, double);
xPROTO_IMPL(float, fmaxf, float, float);

xPROTO_IMPL(double, fmin, double, double);
xPROTO_IMPL(float, fminf, float, float);

xPROTO_IMPL(double, erf, double);
xPROTO_IMPL(float, erff, float);

xPROTO_IMPL(double, erfc, double);
xPROTO_IMPL(float, erfcf, float);

xPROTO_IMPL(double, lgamma, double);
xPROTO_IMPL(float, lgammaf, float);

xPROTO_IMPL(double, tgamma, double);
xPROTO_IMPL(float, tgammaf, float);

LIBSTD_LPROTO_IMPL(double, copysign, double p0, double p1)
{
    double res;
    /* The signbit cannot be properly computed in CIL, so
     * this function does not conform to the standard as
     * it cannot recognize if p1 is -0.0
     */
    if (p1 < 0.0) {
        res = (p0 < 0.0 ? p0 : -p0);
    }
    else {
        res = (p0 < 0.0 ? -p0 : p0);
    }
    return res;
}

LIBSTD_LPROTO_IMPL(float, copysignf, float p0, float p1)
{
    float res;
    if (p1 < 0.0f) {
        res = (p0 < 0.0f ? p0 : -p0);
    }
    else {
        res = (p0 < 0.0f ? -p0 : p0);
    }
    return res;
}

xPROTO_IMPL(double, nan, const char *);
xPROTO_IMPL(float, nanf, const char *);

xPROTO_IMPL(double, nextafter, double, double);
xPROTO_IMPL(float, nextafterf, float, float);

xPROTO_IMPL(double, nexttoward, double, long double);
xPROTO_IMPL(float, nexttowardf, float, long double);


/* This function is used by the libc and not just by libm,
 * so we make it rely on __host.c
 **/
LIBSTD_LPROTO_IMPL(int, fpclassify, double p0)
{
    return LIBSTD_HNAME(fpclassify)(p0);
}

LIBSTD_LPROTO_IMPL(int, isfinite, double p0)
{
    int temp = LIBSTD_HNAME(fpclassify)(p0);
    if (temp != FP_NAN && temp != FP_INFINITE)
        return 1;
    else
        return 0;
}

LIBSTD_LPROTO_IMPL(int, isinf, double p0)
{
    // New glibc return +1 for +inf and -1 for -inf, but this is not
    // required to C99. Libstd implements C99 semantics
    if (LIBSTD_HNAME(fpclassify)(p0) == FP_INFINITE)
        return 1;
    else
        return 0;
}

LIBSTD_LPROTO_IMPL(int, isnan, double p0)
{
    if (LIBSTD_HNAME(fpclassify)(p0) == FP_NAN)
        return 1;
    else
        return 0;
}

LIBSTD_LPROTO_IMPL(int, isnormal, double p0)
{
    if (LIBSTD_HNAME(fpclassify)(p0) == FP_NORMAL)
        return 1;
    else
        return 0;
}

/* This function is used by the libc and not just by libm,
 * so we make it rely on __host.c
 **/
LIBSTD_LPROTO_IMPL(int, signbit, double p0)
{
    /* The signbit cannot be properly computed in CIL, so
     * this function does not conform to the standard as
     * it cannot recognize -0.0
     */
  return (p0 < 0.0);
}
