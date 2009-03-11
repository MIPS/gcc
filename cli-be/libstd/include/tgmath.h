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

#ifndef __TGMATH_H__
#define __TGMATH_H__

#include <__cdefs.h>

#include <math.h>

__BEGIN_EXTERN_C

/* This library does not support complex types
 * it should include complex.h
 */

#define __TGMATH__UNARY(Fun, x)         \
   (  sizeof(x) == sizeof(float)        \
    ? Fun ## f (x)                      \
    : sizeof(x) == sizeof(double)       \
    ? Fun (x)                           \
    : Fun ## l (x))

#define __TGMATH__BINARY(Fun, x, y)                                     \
   (  (sizeof(x) == sizeof(float)) && (sizeof(y) == sizeof(float))      \
    ? Fun ## f (x, y)                                                   \
    : (sizeof(x) == sizeof(double)) || (sizeof(y) == sizeof(double))    \
    ? Fun (x, y)                                                        \
    : Fun ## l (x, y))

#define __TGMATH__BINARY_CHECK_FIRST(Fun, x, y)         \
   (  sizeof(x) == sizeof(float)        \
    ? Fun ## f (x, y)                   \
    : sizeof(x) == sizeof(double)       \
    ? Fun (x, y)                                \
    : Fun ## l (x, y))

#define __TGMATH__TERNARY(Fun, x, y, z)                                                                 \
   (  (sizeof(x) == sizeof(float)) && (sizeof(y) == sizeof(float)) && (sizeof(z) == sizeof(float))      \
    ? Fun ## f (x, y, z)                                                                                \
    : (sizeof(x) == sizeof(double)) || (sizeof(y) == sizeof(double)) || (sizeof(z) == sizeof(double))   \
    ? Fun (x, y, z)                                                                                     \
    : Fun ## l (x, y, z))

#define __TGMATH__TERNARY_CHECK_FIRST_TWO(Fun, x, y, z) \
   (  (sizeof(x) == sizeof(float)) && (sizeof(y) == sizeof(float))      \
    ? Fun ## f (x, y, z)                                                        \
    : (sizeof(x) == sizeof(double)) || (sizeof(y) == sizeof(double))    \
    ? Fun (x, y, z)                                                     \
    : Fun ## l (x, y, z))


#define acos(x)         __TGMATH__UNARY(acos, x)
#define acosh(x)        __TGMATH__UNARY(acosh, x)
#define asin(x)         __TGMATH__UNARY(asin, x)
#define asinh(x)        __TGMATH__UNARY(asinh, x)
#define atan(x)         __TGMATH__UNARY(atan, x)
#define atan2(x, y)     __TGMATH__BINARY(atan2, x, y)
#define atanh(x)        __TGMATH__UNARY(atanh, x)
/* carg only complex */
#define cbrt(x)         __TGMATH__UNARY(cbrt, x)
#define ceil(x)         __TGMATH__UNARY(ceil, x)
/* cimag only complex */
/* conj only complex */
#define copysign(x,y)   __TGMATH__BINARY(copysign, x, y)
#define cos(x)          __TGMATH__UNARY(cos, x)
#define cosh(x)         __TGMATH__UNARY(cosh, x)
/* cproj only complex */
/* creal only complex */
#define erf(x)          __TGMATH__UNARY(erf, x)
#define erfc(x)         __TGMATH__UNARY(erfc, x)
#define exp(x)          __TGMATH__UNARY(exp, x)
#define exp2(x)         __TGMATH__UNARY(exp2, x)
#define expm1(x)        __TGMATH__UNARY(expm1, x)
#define fabs(x)         __TGMATH__UNARY(fabs, x)
#define fabs(x)         __TGMATH__UNARY(fabs, x)
#define fdim(x,y)       __TGMATH__BINARY(fdim, x, y)
#define floor(x)        __TGMATH__UNARY(floor, x)
#define fma(x,y,z)      __TGMATH__TERNARY(fma, x, y, z)
#define fmax(x,y)       __TGMATH__BINARY(fmax, x, y)
#define fmin(x,y)       __TGMATH__BINARY(fmin, x, y)
#define fmod(x,y)       __TGMATH__BINARY(fmod, x, y)
#define frexp(x,y)      __TGMATH__BINARY_CHECK_FIRST(frexp, x, y)
#define hypot(x, y)     __TGMATH__BINARY(hypot, x, y)
#define ilogb(x)        __TGMATH__UNARY(ilogb, x)
#define ldexp(x,y)      __TGMATH__BINARY_CHECK_FIRST(ldexp, x, y)
#define lgamma(x)       __TGMATH__UNARY(lgamma, x)
#define llrint(x)       __TGMATH__UNARY(llrint, x)
#define llround(x)      __TGMATH__UNARY(llround, x)
#define log(x)          __TGMATH__UNARY(log, x)
#define log10(x)        __TGMATH__UNARY(log10, x)
#define log1p(x)        __TGMATH__UNARY(log1p, x)
#define log2(x)         __TGMATH__UNARY(log2, x)
#define logb(x)         __TGMATH__UNARY(logb, x)
#define lrint(x)        __TGMATH__UNARY(lrint, x)
#define lround(x)       __TGMATH__UNARY(lround, x)
/* modf is excluded by the C99*/
#define nearbyint(x)    __TGMATH__UNARY(nearbyint, x)
#define nextafter(x,y)  __TGMATH__BINARY(nextafter, x, y)
#define nexttoward(x,y) __TGMATH__BINARY(nexttoward, x, y)
#define pow(x,y)        __TGMATH__BINARY(pow, x, y)
#define remainder(x,y)  __TGMATH__BINARY(remainder, x, y)
#define remquo(x,y,z)   __TGMATH__TERNARY_CHECK_FIRST_TWO(remquo, x, y, z)
#define rint(x)         __TGMATH__UNARY(rint, x)
#define round(x)        __TGMATH__UNARY(round, x)
#define scalbln(x,y)    __TGMATH__BINARY_CHECK_FIRST(scalbln, x, y)
#define scalbn(x,y)     __TGMATH__BINARY_CHECK_FIRST(scalbn, x, y)
#define sin(x)          __TGMATH__UNARY(sin, x)
#define sinh(x)         __TGMATH__UNARY(sinh, x)
#define sqrt(x)         __TGMATH__UNARY(sqrt, x)
#define tan(x)          __TGMATH__UNARY(tan, x)
#define tanh(x)         __TGMATH__UNARY(tanh, x)
#define tgamma(x)       __TGMATH__UNARY(tgamma, x)
#define trunc(x)        __TGMATH__UNARY(trunc, x)

__END_EXTERN_C

#endif /*__TGMATH_H__*/
