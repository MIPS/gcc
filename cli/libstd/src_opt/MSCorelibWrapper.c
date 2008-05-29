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

#define __LIBSTD_CRT_BUILD

#include "include/__OPT_MSCorlibWrapper.h"

#include "include/__host.h"


PINVOKE_RENAME_HOST_ATTR(int, open, char *path, int oflag);
PINVOKE_RENAME_HOST_ATTR(int, write, int fildes, void *buf, int nbytes);
PINVOKE_RENAME_HOST_ATTR(int, read, int fildes, void *buf, int nbytes);
PINVOKE_RENAME_HOST_ATTR(int, close, int fildes);
PINVOKE_RENAME_HOST_ATTR(long, lseek, int fildes, long offset, int whence);
PINVOKE_RENAME_HOST_ATTR(int, fcntl, int fildes, int cmd, int arg);
PINVOKE_RENAME_HOST_ATTR(int, rename, char *oldname, char *newname);
PINVOKE_RENAME_HOST_ATTR(int, remove, char *filename);
PINVOKE_RENAME_HOST_ATTR(int, link, char *existing, char *anew);
PINVOKE_RENAME_HOST_ATTR(int, unlink, char *path);


PINVOKE_RENAME_HOST_ATTR(void *, malloc, unsigned int size);
PINVOKE_RENAME_HOST_ATTR(void, free, void *ptr);
PINVOKE_RENAME_HOST_ATTR(void *, realloc, void *ptr, unsigned int size);

PINVOKE_RENAME_HOST_ATTR(int *, errno, void);

PINVOKE_RENAME_HOST_ATTR(int, clock, void);
PINVOKE_RENAME_HOST_ATTR(void, exit, int status);

PINVOKE_RENAME_HOST_ATTR(char *, getenv, char *name);

PINVOKE_RENAME_HOST_ATTR(unsigned long, time, void *tptr);

PINVOKE_RENAME_HOST_ATTR(void, gmtime, void *timer,
                                       int *tm_sec,  int *tm_min,  int *tm_hour,
                                       int *tm_mday, int *tm_mon,  int *tm_year,
                                       int *tm_wday, int *tm_yday, int *tm_isdst);

PINVOKE_RENAME_HOST_ATTR(void, gettimeofday, void *tv_sec, void *tv_usec);

///////////////////////////////////////////////////////////////////////////////////////
///  MATH SECTION /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// These functions maybe implemented as macros (e.g. the arm softfloat toolchain).
// Therefore in order to be portable we cannot call it directly. We use the
// mscoreliwrapper_support as a portability layer and pay the cost of an extra call

PINVOKE_RENAME_HOST_ATTR(int, fpclassify, double p0);

PINVOKE_RENAME_HOST_ATTR(double, dbl_epsilon, void);
PINVOKE_RENAME_HOST_ATTR(double, dbl_minval, void);
PINVOKE_RENAME_HOST_ATTR(double, dbl_maxval, void);
PINVOKE_RENAME_HOST_ATTR(double, dbl_hugeval, void);
PINVOKE_RENAME_HOST_ATTR(float, flt_epsilon, void);
PINVOKE_RENAME_HOST_ATTR(float, flt_minval, void);
PINVOKE_RENAME_HOST_ATTR(float, flt_maxval, void);
PINVOKE_RENAME_HOST_ATTR(float, flt_hugeval, void);
PINVOKE_RENAME_HOST_ATTR(float, flt_nan, void);
PINVOKE_RENAME_HOST_ATTR(float, flt_infinity, void);

PINVOKE_RENAME_HOST_ATTR(double, acos, double p0);
PINVOKE_RENAME_HOST_ATTR(float, acosf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, asin, double p0);
PINVOKE_RENAME_HOST_ATTR(float, asinf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, atan, double p0);
PINVOKE_RENAME_HOST_ATTR(float, atanf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, atan2, double p0, double p1);
PINVOKE_RENAME_HOST_ATTR(float, atan2f, float p0, float p1);
PINVOKE_RENAME_HOST_ATTR(double, cos, double p0);
PINVOKE_RENAME_HOST_ATTR(float, cosf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, sin, double p0);
PINVOKE_RENAME_HOST_ATTR(float, sinf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, tan, double p0);
PINVOKE_RENAME_HOST_ATTR(float, tanf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, cosh, double p0);
PINVOKE_RENAME_HOST_ATTR(float, coshf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, sinh, double p0);
PINVOKE_RENAME_HOST_ATTR(float, sinhf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, tanh, double p0);
PINVOKE_RENAME_HOST_ATTR(float, tanhf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, exp, double p0);
PINVOKE_RENAME_HOST_ATTR(float, expf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, exp2, double p0);
PINVOKE_RENAME_HOST_ATTR(float, exp2f, float p0);
PINVOKE_RENAME_HOST_ATTR(double, expm1, double p0);
PINVOKE_RENAME_HOST_ATTR(float, expm1f, float p0);
PINVOKE_RENAME_HOST_ATTR(double, ldexp, double p0, int p1);
PINVOKE_RENAME_HOST_ATTR(float, ldexpf, float p0, int p1);
PINVOKE_RENAME_HOST_ATTR(double, log, double p0);
PINVOKE_RENAME_HOST_ATTR(float, logf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, log10, double p0);
PINVOKE_RENAME_HOST_ATTR(float, log10f, float p0);
PINVOKE_RENAME_HOST_ATTR(double, log1p, double p0);
PINVOKE_RENAME_HOST_ATTR(float, log1pf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, log2, double p0);
PINVOKE_RENAME_HOST_ATTR(float, log2f, float p0);
PINVOKE_RENAME_HOST_ATTR(double, scalbn, double p0, int p1);
PINVOKE_RENAME_HOST_ATTR(float, scalbnf, float p0, int p1);
PINVOKE_RENAME_HOST_ATTR(double, scalbln, double p0, long p1);
PINVOKE_RENAME_HOST_ATTR(float, scalblnf, float p0, long p1);
PINVOKE_RENAME_HOST_ATTR(double, fabs, double p0);
PINVOKE_RENAME_HOST_ATTR(float, fabsf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, hypot, double p0, double p1);
PINVOKE_RENAME_HOST_ATTR(float, hypotf, float p0, float p1);
PINVOKE_RENAME_HOST_ATTR(double, pow, double p0, double p1);
PINVOKE_RENAME_HOST_ATTR(float, powf, float p0, float p1);
PINVOKE_RENAME_HOST_ATTR(double, ceil, double p0);
PINVOKE_RENAME_HOST_ATTR(float, ceilf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, cbrt, double p0);
PINVOKE_RENAME_HOST_ATTR(float, cbrtf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, sqrt, double p0);
PINVOKE_RENAME_HOST_ATTR(float, sqrtf, float p0);
PINVOKE_RENAME_HOST_ATTR(double, floor, double p0);
PINVOKE_RENAME_HOST_ATTR(float, floorf, float p0);
PINVOKE_RENAME_HOST_ATTR(long, lround, double p0);
PINVOKE_RENAME_HOST_ATTR(long, lroundf, float p0);
