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

#ifndef __LIBSTD_FLOAT_H__
#define __LIBSTD_FLOAT_H__

#if !defined __FLOAT_H__ && !defined __LIBSTD_CRT_BUILD
#  error "Never use <bits/libstd_float.h> directly; include <float.h> instead."
#endif

/* Remember to update values in src/MSCorlibWrapper.cs */

__BEGIN_EXTERN_C

enum {
    __LIBSTD_FLT_RADIX = 2
};

enum {
    __LIBSTD_FLT_EVAL_METHOD = 0
};

enum {
    __LIBSTD_FLT_DIG      =  6,
    __LIBSTD_DBL_DIG      = 15,
    __LIBSTD_FLT_MANT_DIG = 24,
    __LIBSTD_DBL_MANT_DIG = 53,
    __LIBSTD_DECIMAL_DIG  = 17
};

enum {
    __LIBSTD_FLT_MIN_EXP    = (-125),
    __LIBSTD_DBL_MIN_EXP    = (-1021),
    __LIBSTD_FLT_MIN_10_EXP = (-37),
    __LIBSTD_DBL_MIN_10_EXP = (-307)
};

enum {
    __LIBSTD_FLT_MAX_EXP    = 128,
    __LIBSTD_DBL_MAX_EXP    = 1024,
    __LIBSTD_FLT_MAX_10_EXP = 38,
    __LIBSTD_DBL_MAX_10_EXP = 308
};

__END_EXTERN_C

#endif /*__LIBSTD_FLOAT_H__*/
