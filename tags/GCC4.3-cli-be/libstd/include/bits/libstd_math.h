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

#ifndef __LIBSTD_MATH_H
#define __LIBSTD_MATH_H

#if !defined __MATH_H__ && !defined __LIBSTD_CRT_BUILD
#  error "Never use <bits/libstd_math.h> directly; include <math.h> instead."
#endif

/* Remember to update values in src/MSCorlibWrapper.cs */

__BEGIN_EXTERN_C

enum {
    __LIBSTD_FP_INFINITE  = 0,
    __LIBSTD_FP_NAN       = 1,
    __LIBSTD_FP_NORMAL    = 2,
    __LIBSTD_FP_SUBNORMAL = 3,
    __LIBSTD_FP_ZERO      = 4
};

__END_EXTERN_C

#endif /* __LIBSTD_MATH_H */
