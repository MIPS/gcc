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

#ifndef __FLOAT_H__
#define __FLOAT_H__

#include <__cdefs.h>

#include <bits/libstd_float.h>
#include <bits/libstd_proto.h>

__BEGIN_EXTERN_C

#define FLT_RADIX               __LIBSTD_FLT_RADIX

#define FLT_ROUNDS              (LIBSTD_LNAME(__flt_rounds)())

#define FLT_EVAL_METHOD         __LIBSTD_FLT_EVAL_METHOD


#define FLT_EPSILON             (LIBSTD_LNAME(__flt_epsilon)())
#define DBL_EPSILON             (LIBSTD_LNAME(__dbl_epsilon)())
#define LDBL_EPSILON            DBL_EPSILON

#define FLT_DIG                 __LIBSTD_FLT_DIG
#define DBL_DIG                 __LIBSTD_DBL_DIG
#define LDBL_DIG                DBL_DIG

#define FLT_MANT_DIG            __LIBSTD_FLT_MANT_DIG
#define DBL_MANT_DIG            __LIBSTD_DBL_MANT_DIG
#define LDBL_MANT_DIG           DBL_MANT_DIG

#define DECIMAL_DIG             __LIBSTD_DECIMAL_DIG

#define FLT_MIN                 (LIBSTD_LNAME(__flt_minval)())
#define DBL_MIN                 (LIBSTD_LNAME(__dbl_minval)())
#define LDBL_MIN                DBL_MIN

#define FLT_MIN_EXP             __LIBSTD_FLT_MIN_EXP
#define DBL_MIN_EXP             __LIBSTD_DBL_MIN_EXP
#define LDBL_MIN_EXP            DBL_MIN_EXP

#define FLT_MIN_10_EXP          __LIBSTD_FLT_MIN_10_EXP
#define DBL_MIN_10_EXP          __LIBSTD_FLT_MIN_10_EXP
#define LDBL_MIN_10_EXP         DBL_MIN_10_EXP

#define FLT_MAX                 (LIBSTD_LNAME(__flt_maxval)())
#define DBL_MAX                 (LIBSTD_LNAME(__dbl_maxval)())
#define LDBL_MAX                DBL_MAX

#define FLT_MAX_EXP             __LIBSTD_FLT_MAX_EXP
#define DBL_MAX_EXP             __LIBSTD_DBL_MAX_EXP
#define LDBL_MAX_EXP            DBL_MAX_EXP

#define FLT_MAX_10_EXP          __LIBSTD_FLT_MAX_10_EXP
#define DBL_MAX_10_EXP          __LIBSTD_DBL_MAX_10_EXP
#define LDBL_MAX_10_EXP         DBL_MAX_10_EXP

__END_EXTERN_C

#endif /*__FLOAT_H__*/
