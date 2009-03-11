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

#ifndef __LIMITS_H__
#define __LIMITS_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

#define CHAR_BIT        8

#define SCHAR_MIN      (-0x7F-1)
#define SCHAR_MAX       (0x7F)
#define UCHAR_MAX       (0xFFU)

#if defined(__Char_Is_Unsigned__)
#  define CHAR_MIN      0
#  define CHAR_MAX      UCHAR_MAX
#elif defined (__Char_Is_Signed__)
#  define CHAR_MIN      SCHAR_MIN
#  define CHAR_MAX      SCHAR_MAX
#else
#  error "<limits.h> : Sorry : either __Char_Is_Unsigned__ or __Char_Is_Signed__ must be compiler-defined."
#endif

/*
 * Maximum number of bytes in a multibyte character, for any supported locale.
*/
#define MB_LEN_MAX      1

#define SHRT_MIN       (-0x7FFF-1)
#define SHRT_MAX        (0x7FFF)
#define USHRT_MAX       (0xFFFFU)

#define INT_MIN        (-0x7FFFFFFF-1)
#define INT_MAX         (0x7FFFFFFF)
#define UINT_MAX        (0xFFFFFFFFU)

#define LONG_MIN       (-0x7FFFFFFFL-1)
#define LONG_MAX        (0x7FFFFFFFL)
#define ULONG_MAX       (0xFFFFFFFFUL)

#define LLONG_MIN      (-0x7FFFFFFFFFFFFFFFLL-1)
#define LLONG_MAX       (0x7FFFFFFFFFFFFFFFLL)
#define ULLONG_MAX      (0xFFFFFFFFFFFFFFFFULL)

__END_EXTERN_C

#endif /*__LIMITS_H__*/
