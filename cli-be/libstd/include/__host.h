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

#ifndef ____HOST_H__
#define ____HOST_H__

#if !defined(__LIBSTD_CRT_BUILD)
#error "__host.h : Sorry : This header is intended for building C runtime only."
#endif

__BEGIN_EXTERN_C

#include "bits/libstd_errno.h"
#include "bits/libstd_fcntl.h"
#include "bits/libstd_math.h"
#include "bits/libstd_stdio.h"

LIBSTD_HPROTO(void *, malloc, unsigned int size);
LIBSTD_HPROTO(void, free, void *ptr);
LIBSTD_HPROTO(void *, realloc, void *ptr, unsigned int size);

LIBSTD_HPROTO(int *, errno, void);

LIBSTD_HPROTO(char *, getenv, char *name);

/* Close a file descriptor. */
LIBSTD_HPROTO(int, close, int fildes);

/* Return file size. */
LIBSTD_HPROTO(int, filesize, char *path);

/* Called when process exit, to signa IOs _cleanup to host. */
LIBSTD_HPROTO(void, exit, int status);

/* Control over an open file descriptor.*/
LIBSTD_HPROTO(int, fcntl, int fildes, int cmd, int arg);

/* Manages file pointers.  */
LIBSTD_HPROTO(long, lseek, int fildes, long offset, int whence);

/* Create a directory entry.  */
LIBSTD_HPROTO(int, link, char *existing, char *anew);

/* Opens a file descriptor on a file. */
LIBSTD_HPROTO(int, open, char *path, int oflag);

/* Attempts to read bytes from stream.  */
LIBSTD_HPROTO(int, read, int fildes, void *buf, int nbytes);

/* Renames a file. */
LIBSTD_HPROTO(int, rename, char *oldname, char *newname);

/* Removes a file. */
LIBSTD_HPROTO(int, remove, char *filename);

/* Removes a directory entry.  */
LIBSTD_HPROTO(int, unlink, char *path);

/* Attempts to write bytes into stream.    */
LIBSTD_HPROTO(int, write, int fildes, void *buf, int nbytes);

/* Getting clock */
LIBSTD_HPROTO(int, clock, void);

/* Getting the host time */
LIBSTD_HPROTO(unsigned long, time, void *tptr);

LIBSTD_HPROTO(void, gmtime, void *timer,
                         int *tm_sec,  int *tm_min,  int *tm_hour,
                         int *tm_mday, int *tm_mon,  int *tm_year,
                         int *tm_wday, int *tm_yday, int *tm_isdst);

LIBSTD_HPROTO(void, gettimeofday, void *tv_sec, void *tv_usec);

/* Floating point classification */
LIBSTD_HPROTO(int, fpclassify, double p0);

LIBSTD_HPROTO(double, dbl_epsilon, void);
LIBSTD_HPROTO(double, dbl_minval, void);
LIBSTD_HPROTO(double, dbl_maxval, void);
LIBSTD_HPROTO(double, dbl_hugeval, void);
LIBSTD_HPROTO(float, flt_epsilon, void);
LIBSTD_HPROTO(float, flt_minval, void);
LIBSTD_HPROTO(float, flt_maxval, void);
LIBSTD_HPROTO(float, flt_hugeval, void);
LIBSTD_HPROTO(float, flt_nan, void);
LIBSTD_HPROTO(float, flt_infinity, void);

/*********************** Math functions *******************************/

LIBSTD_HPROTO(double, acos, double p0);
LIBSTD_HPROTO(float, acosf, float p0);
LIBSTD_HPROTO(double, asin, double p0);
LIBSTD_HPROTO(float, asinf, float p0);
LIBSTD_HPROTO(double, atan, double p0);
LIBSTD_HPROTO(float, atanf, float p0);
LIBSTD_HPROTO(double, atan2, double p0, double p1);
LIBSTD_HPROTO(float, atan2f, float p0, float p1);
LIBSTD_HPROTO(double, cos, double p0);
LIBSTD_HPROTO(float, cosf, float p0);
LIBSTD_HPROTO(double, sin, double p0);
LIBSTD_HPROTO(float, sinf, float p0);
LIBSTD_HPROTO(double, tan, double p0);
LIBSTD_HPROTO(float, tanf, float p0);
LIBSTD_HPROTO(double, cosh, double p0);
LIBSTD_HPROTO(float, coshf, float p0);
LIBSTD_HPROTO(double, sinh, double p0);
LIBSTD_HPROTO(float, sinhf, float p0);
LIBSTD_HPROTO(double, tanh, double p0);
LIBSTD_HPROTO(float, tanhf, float p0);
LIBSTD_HPROTO(double, exp, double p0);
LIBSTD_HPROTO(float, expf, float p0);
LIBSTD_HPROTO(double, exp2, double p0);
LIBSTD_HPROTO(float, exp2f, float p0);
LIBSTD_HPROTO(double, expm1, double p0);
LIBSTD_HPROTO(float, expm1f, float p0);
LIBSTD_HPROTO(double, ldexp, double p0, int p1);
LIBSTD_HPROTO(float, ldexpf, float p0, int p1);
LIBSTD_HPROTO(double, log, double p0);
LIBSTD_HPROTO(float, logf, float p0);
LIBSTD_HPROTO(double, log10, double p0);
LIBSTD_HPROTO(float, log10f, float p0);
LIBSTD_HPROTO(double, log1p, double p0);
LIBSTD_HPROTO(float, log1pf, float p0);
LIBSTD_HPROTO(double, log2, double p0);
LIBSTD_HPROTO(float, log2f, float p0);
LIBSTD_HPROTO(double, scalbn, double p0, int p1);
LIBSTD_HPROTO(float, scalbnf, float p0, int p1);
LIBSTD_HPROTO(double, scalbln, double p0, long p1);
LIBSTD_HPROTO(float, scalblnf, float p0, long p1);
LIBSTD_HPROTO(double, cbrt, double p0);
LIBSTD_HPROTO(float, cbrtf, float p0);
LIBSTD_HPROTO(double, fabs, double p0);
LIBSTD_HPROTO(float, fabsf, float p0);
LIBSTD_HPROTO(double, hypot, double p0, double p1);
LIBSTD_HPROTO(float, hypotf, float p0, float p1);
LIBSTD_HPROTO(double, pow, double p0, double p1);
LIBSTD_HPROTO(float, powf, float p0, float p1);
LIBSTD_HPROTO(double, sqrt, double p0);
LIBSTD_HPROTO(float, sqrtf, float p0);
LIBSTD_HPROTO(double, ceil, double p0);
LIBSTD_HPROTO(float, ceilf, float p0);
LIBSTD_HPROTO(double, floor, double p0);
LIBSTD_HPROTO(float, floorf, float p0);
LIBSTD_HPROTO(long, lround, double p0);
LIBSTD_HPROTO(long, lroundf, float p0);

__END_EXTERN_C

#endif /*____HOST_H__*/
