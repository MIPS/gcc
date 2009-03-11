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

#ifndef __MATH_H__
#define __MATH_H__

#include <__cdefs.h>

#include <limits.h>
#include <stdint.h>

#include <bits/libstd_math.h>
#include <bits/libstd_proto.h>

__BEGIN_EXTERN_C

#define HUGE_VAL        (LIBSTD_LNAME(__dbl_hugeval)())

#if defined __IS_ISO_C99

#  define HUGE_VALF       (LIBSTD_LNAME(__flt_hugeval)())
#  define HUGE_VALL       HUGE_VAL

#  define INFINITY        (LIBSTD_LNAME(__flt_infinity)())

#  define NAN             (LIBSTD_LNAME(__flt_nan)())

typedef float float_t;
typedef double double_t;

#  define FP_ILOGB0       (-INT_MAX)
#  define FP_ILOGBNAN     (INT_MAX)

#  define MATH_ERRNO      1
#  define MATH_ERREXCEPT  2

#endif


LIBSTD_LPROTO(double, fabs, double);
LIBSTD_LPROTO(float, fabsf, float);
LIBSTD_LPROTO(long double, fabsl, long double);

LIBSTD_LPROTO(double, ceil, double);
LIBSTD_LPROTO(float, ceilf, float);
LIBSTD_LPROTO(long double, ceill, long double);

LIBSTD_LPROTO(double, floor, double);
LIBSTD_LPROTO(float, floorf, float);
LIBSTD_LPROTO(long double, floorl, long double);


LIBSTD_LPROTO(double, nearbyint, double);
LIBSTD_LPROTO(float, nearbyintf, float);
LIBSTD_LPROTO(long double, nearbyintl, long double);

LIBSTD_LPROTO(double, rint, double);
LIBSTD_LPROTO(float, rintf, float);
LIBSTD_LPROTO(long double, rintl, long double);

LIBSTD_LPROTO(long, lrint, double);
LIBSTD_LPROTO(long, lrintf, float);
LIBSTD_LPROTO(long, lrintl, long double);

LIBSTD_LPROTO(long long, llrint, double);
LIBSTD_LPROTO(long long, llrintf, float);
LIBSTD_LPROTO(long long, llrintl, long double);

LIBSTD_LPROTO(double, round, double);
LIBSTD_LPROTO(float, roundf, float);
LIBSTD_LPROTO(long double, roundl, long double);

LIBSTD_LPROTO(long, lround, double);
LIBSTD_LPROTO(long, lroundf, float);
LIBSTD_LPROTO(long, lroundl, long double);

LIBSTD_LPROTO(long long, llround, double);
LIBSTD_LPROTO(long long, llroundf, float);
LIBSTD_LPROTO(long long, llroundl, long double);

LIBSTD_LPROTO(double, trunc, double);
LIBSTD_LPROTO(float, truncf, float);
LIBSTD_LPROTO(long double, truncl, long double);

LIBSTD_LPROTO(double, fmod, double, double);
LIBSTD_LPROTO(float, fmodf, float, float);
LIBSTD_LPROTO(long double, fmodl, long double, long double);

LIBSTD_LPROTO(double, remainder, double, double);
LIBSTD_LPROTO(float, remainderf, float, float);
LIBSTD_LPROTO(long double, remainderl, long double, long double);

LIBSTD_LPROTO(double, remquo, double, double, int *);
LIBSTD_LPROTO(float, remquof, float, float, int *);
LIBSTD_LPROTO(long double, remquol, long double, long double, int *);

LIBSTD_LPROTO(double, frexp, double, int *);
LIBSTD_LPROTO(float, frexpf, float, int *);
LIBSTD_LPROTO(long double, frexpl, long double, int *);

LIBSTD_LPROTO(double, ldexp, double, int);
LIBSTD_LPROTO(float, ldexpf, float, int);
LIBSTD_LPROTO(long double, ldexpl, long double, int);

LIBSTD_LPROTO(double, modf, double, double *);
LIBSTD_LPROTO(float, modff, float, float *);
LIBSTD_LPROTO(long double, modfl, long double, long double *);

LIBSTD_LPROTO(double, scalbn, double, int);
LIBSTD_LPROTO(float, scalbnf, float, int);
LIBSTD_LPROTO(long double, scalbnl, long double, int);

LIBSTD_LPROTO(double, scalbln, double, long);
LIBSTD_LPROTO(float, scalblnf, float, long);
LIBSTD_LPROTO(long double, scalblnl, long double, long);

LIBSTD_LPROTO(double, exp, double);
LIBSTD_LPROTO(float, expf, float);
LIBSTD_LPROTO(long double, expl, long double);

LIBSTD_LPROTO(double, exp2, double);
LIBSTD_LPROTO(float, exp2f, float);
LIBSTD_LPROTO(long double, exp2l, long double);

LIBSTD_LPROTO(double, expm1, double);
LIBSTD_LPROTO(float, expm1f, float);
LIBSTD_LPROTO(long double, expm1l, long double);

LIBSTD_LPROTO(double, log, double);
LIBSTD_LPROTO(float, logf, float);
LIBSTD_LPROTO(long double, logl, long double);

LIBSTD_LPROTO(double, log10, double);
LIBSTD_LPROTO(float, log10f, float);
LIBSTD_LPROTO(long double, log10l, long double);

LIBSTD_LPROTO(double, log1p, double);
LIBSTD_LPROTO(float, log1pf, float);
LIBSTD_LPROTO(long double, log1pl, long double);

LIBSTD_LPROTO(double, log2, double);
LIBSTD_LPROTO(float, log2f, float);
LIBSTD_LPROTO(long double, log2l, long double);

LIBSTD_LPROTO(double, logb, double);
LIBSTD_LPROTO(float, logbf, float);
LIBSTD_LPROTO(long double, logbl, long double);

LIBSTD_LPROTO(int, ilogb, double);
LIBSTD_LPROTO(int, ilogbf, float);
LIBSTD_LPROTO(int, ilogbl, long double);

LIBSTD_LPROTO(double, cbrt, double);
LIBSTD_LPROTO(float, cbrtf, float);
LIBSTD_LPROTO(long double, cbrtl, long double);

LIBSTD_LPROTO(double, hypot, double, double);
LIBSTD_LPROTO(float, hypotf, float, float);
LIBSTD_LPROTO(long double, hypotl, long double, long double);

LIBSTD_LPROTO(double, fma, double, double, double);
LIBSTD_LPROTO(float, fmaf, float, float, float);
LIBSTD_LPROTO(long double, fmal, long double, long double, long double);

LIBSTD_LPROTO(double, pow, double, double);
LIBSTD_LPROTO(float, powf, float, float);
LIBSTD_LPROTO(long double, powl, long double, long double);

LIBSTD_LPROTO(double, sqrt, double);
LIBSTD_LPROTO(float, sqrtf, float);
LIBSTD_LPROTO(long double, sqrtl, long double);

LIBSTD_LPROTO(double, cos, double);
LIBSTD_LPROTO(float, cosf, float);
LIBSTD_LPROTO(long double, cosl, long double);

LIBSTD_LPROTO(double, sin, double);
LIBSTD_LPROTO(float, sinf, float);
LIBSTD_LPROTO(long double, sinl, long double);

LIBSTD_LPROTO(double, tan, double);
LIBSTD_LPROTO(float, tanf, float);
LIBSTD_LPROTO(long double, tanl, long double);

LIBSTD_LPROTO(double, cosh, double);
LIBSTD_LPROTO(float, coshf, float);
LIBSTD_LPROTO(long double, coshl, long double);

LIBSTD_LPROTO(double, sinh, double);
LIBSTD_LPROTO(float, sinhf, float);
LIBSTD_LPROTO(long double, sinhl, long double);

LIBSTD_LPROTO(double, tanh, double);
LIBSTD_LPROTO(float, tanhf, float);
LIBSTD_LPROTO(long double, tanhl, long double);

LIBSTD_LPROTO(double, acos, double);
LIBSTD_LPROTO(float, acosf, float);
LIBSTD_LPROTO(long double, acosl, long double);

LIBSTD_LPROTO(double, asin, double);
LIBSTD_LPROTO(float, asinf, float);
LIBSTD_LPROTO(long double, asinl, long double);

LIBSTD_LPROTO(double, atan, double);
LIBSTD_LPROTO(float, atanf, float);
LIBSTD_LPROTO(long double, atanl, long double);

LIBSTD_LPROTO(double, atan2, double, double);
LIBSTD_LPROTO(float, atan2f, float, float);
LIBSTD_LPROTO(long double, atan2l, long double, long double);

LIBSTD_LPROTO(double, acosh, double);
LIBSTD_LPROTO(float, acoshf, float);
LIBSTD_LPROTO(long double, acoshl, long double);

LIBSTD_LPROTO(double, asinh, double);
LIBSTD_LPROTO(float, asinhf, float);
LIBSTD_LPROTO(long double, asinhl, long double);

LIBSTD_LPROTO(double, atanh, double);
LIBSTD_LPROTO(float, atanhf, float);
LIBSTD_LPROTO(long double, atanhl, long double);

LIBSTD_LPROTO(double, fdim, double, double);
LIBSTD_LPROTO(float, fdimf, float, float);
LIBSTD_LPROTO(long double, fdiml, long double, long double);

LIBSTD_LPROTO(double, fmax, double, double);
LIBSTD_LPROTO(float, fmaxf, float, float);
LIBSTD_LPROTO(long double, fmaxl, long double, long double);

LIBSTD_LPROTO(double, fmin, double, double);
LIBSTD_LPROTO(float, fminf, float, float);
LIBSTD_LPROTO(long double, fminl, long double, long double);

LIBSTD_LPROTO(double, erf, double);
LIBSTD_LPROTO(float, erff, float);
LIBSTD_LPROTO(long double, erfl, long double);

LIBSTD_LPROTO(double, erfc, double);
LIBSTD_LPROTO(float, erfcf, float);
LIBSTD_LPROTO(long double, erfcl, long double);

LIBSTD_LPROTO(double, lgamma, double);
LIBSTD_LPROTO(float, lgammaf, float);
LIBSTD_LPROTO(long double, lgammal, long double);

LIBSTD_LPROTO(double, tgamma, double);
LIBSTD_LPROTO(float, tgammaf, float);
LIBSTD_LPROTO(long double, tgammal, long double);

LIBSTD_LPROTO(double, copysign, double, double);
LIBSTD_LPROTO(float, copysignf, float, float);
LIBSTD_LPROTO(long double, copysignl, long double, long double);

LIBSTD_LPROTO(double, nan, const char *);
LIBSTD_LPROTO(float, nanf, const char *);
LIBSTD_LPROTO(long double, nanl, const char *);

LIBSTD_LPROTO(double, nextafter, double, double);
LIBSTD_LPROTO(float, nextafterf, float, float);
LIBSTD_LPROTO(long double, nextafterl, long double, long double);

LIBSTD_LPROTO(double, nexttoward, double, long double);
LIBSTD_LPROTO(float, nexttowardf, float, long double);
LIBSTD_LPROTO(long double, nexttowardl, long double, long double);

#define FP_INFINITE  __LIBSTD_FP_INFINITE
#define FP_NAN       __LIBSTD_FP_NAN
#define FP_NORMAL    __LIBSTD_FP_NORMAL
#define FP_SUBNORMAL __LIBSTD_FP_SUBNORMAL
#define FP_ZERO      __LIBSTD_FP_ZERO

LIBSTD_LPROTO(int, fpclassify, double p0);

LIBSTD_LPROTO(int, isfinite, double p0);
LIBSTD_LPROTO(int, isinf, double p0);
LIBSTD_LPROTO(int, isnan, double p0);
LIBSTD_LPROTO(int, isnormal, double p0);
LIBSTD_LPROTO(int, signbit, double p0);

#ifndef isgreater
#define isgreater(x,y) (!isunordered((x),(y)) && ((x)>(y)))
#endif

#ifndef isgreaterequal
#define isgreaterequal(x,y) (!isunordered((x),(y)) && ((x)>=(y)))
#endif

#ifndef isless
#define isless(x,y) (!isunordered((x),(y)) && ((x)<(y)))
#endif

#ifndef islessequal
#define islessequal(x,y) (!isunordered((x),(y)) && ((x)<=(y)))
#endif

#ifndef islessgreater
#define islessgreater(x,y) (!isunordered((x),(y)) && ((x)<(y)||(x)>(y)))
#endif

#ifndef isunordered
#define isunordered(x,y) ((fpclassify(x) == FP_NAN) || (fpclassify(y) == FP_NAN))
#endif

__END_EXTERN_C

#endif /*__MATH_H__*/
