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

#ifndef __STDINT_H__
#define __STDINT_H__

#include <__cdefs.h>

#include <limits.h>

__BEGIN_EXTERN_C

typedef signed char      int8_t;
typedef signed short     int16_t;
typedef signed int       int32_t;
typedef signed long long int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef int8_t  int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef int8_t  int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;

typedef uint8_t  uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;

#define INT8_MIN                SCHAR_MIN
#define INT8_MAX                SCHAR_MAX
#define UINT8_MAX               UCHAR_MAX

#define INT16_MIN               SHRT_MIN
#define INT16_MAX               SHRT_MAX
#define UINT16_MAX              USHRT_MAX

#define INT32_MIN               INT_MIN
#define INT32_MAX               INT_MAX
#define UINT32_MAX              UINT_MAX

#define INT64_MIN               LLONG_MIN
#define INT64_MAX               LLONG_MAX
#define UINT64_MAX              ULLONG_MAX

#define INT_LEAST8_MIN          INT8_MIN
#define INT_LEAST8_MAX          INT8_MAX
#define UINT_LEAST8_MAX         UINT8_MAX

#define INT_LEAST16_MIN         INT16_MIN
#define INT_LEAST16_MAX         INT16_MAX
#define UINT_LEAST16_MAX        UINT16_MAX

#define INT_LEAST32_MIN         INT32_MIN
#define INT_LEAST32_MAX         INT32_MAX
#define UINT_LEAST32_MAX        UINT32_MAX

#define INT_LEAST64_MIN         INT64_MIN
#define INT_LEAST64_MAX         INT64_MAX
#define UINT_LEAST64_MAX        UINT64_MAX

#define INT_FAST8_MIN           INT8_MIN
#define INT_FAST8_MAX           INT8_MAX
#define UINT_FAST8_MAX          UINT8_MAX

#define INT_FAST16_MIN          INT16_MIN
#define INT_FAST16_MAX          INT16_MAX
#define UINT_FAST16_MAX         UINT16_MAX

#define INT_FAST32_MIN          INT32_MIN
#define INT_FAST32_MAX          INT32_MAX
#define UINT_FAST32_MAX         UINT32_MAX

#define INT_FAST64_MIN          INT64_MIN
#define INT_FAST64_MAX          INT64_MAX
#define UINT_FAST64_MAX         UINT64_MAX

#define INTPTR_MIN              LONG_MIN
#define INTPTR_MAX              LONG_MAX
#define UINTPTR_MAX             ULONG_MAX

#define PTRDIFF_MIN             LONG_MIN
#define PTRDIFF_MAX             LONG_MAX

#define SIZE_MAX                UINT_MAX

#define WCHAR_MIN               (0)
#define WCHAR_MAX               UINT16_MAX

#define WINT_MIN                (0)
#define WINT_MAX                UINT_MAX

#define SIG_ATOMIC_MIN          INT32_MIN
#define SIG_ATOMIC_MAX          INT32_MAX


#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#define INT8_C(v)       (v)
#define INT16_C(v)      (v)
#define INT32_C(v)      (v)
#define INT64_C(v)      (v ## LL)

#define UINT8_C(v)      (v ## U)
#define UINT16_C(v)     (v ## U)
#define UINT32_C(v)     (v ## U)
#define UINT64_C(v)     (v ## ULL)

#define INTMAX_C(v)     INT64_C(v)
#define UINTMAX_C(v)    UINT64_C(v)

#endif

__END_EXTERN_C

#endif /*__STDINT_H__*/
