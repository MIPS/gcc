/* Definitions for <stdint.h> types on systems using AIX.
   Copyright (C) 2009, 2010 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define SIG_ATOMIC_TYPE itk_int

#define INT8_TYPE itk_signed_char
#define INT16_TYPE itk_short
#define INT32_TYPE itk_int
#define INT64_TYPE (LONG_TYPE_SIZE == 64 ? itk_long : itk_long_long)
#define UINT8_TYPE itk_unsigned_char
#define UINT16_TYPE itk_unsigned_short
#define UINT32_TYPE itk_unsigned_int
#define UINT64_TYPE \
  (LONG_TYPE_SIZE == 64 ? itk_unsigned_long : itk_unsigned_long_long)

#define INT_LEAST8_TYPE itk_signed_char
#define INT_LEAST16_TYPE itk_short
#define INT_LEAST32_TYPE itk_int
#define INT_LEAST64_TYPE (LONG_TYPE_SIZE == 64 ? itk_long : itk_long_long)
#define UINT_LEAST8_TYPE itk_unsigned_char
#define UINT_LEAST16_TYPE itk_unsigned_short
#define UINT_LEAST32_TYPE itk_unsigned_int
#define UINT_LEAST64_TYPE \
  (LONG_TYPE_SIZE == 64 ? itk_unsigned_long : itk_unsigned_long_long)

#define INT_FAST8_TYPE itk_signed_char
#define INT_FAST16_TYPE itk_short
#define INT_FAST32_TYPE itk_int
#define INT_FAST64_TYPE (LONG_TYPE_SIZE == 64 ? itk_long : itk_long_long)
#define UINT_FAST8_TYPE itk_unsigned_char
#define UINT_FAST16_TYPE itk_unsigned_short
#define UINT_FAST32_TYPE itk_unsigned_int
#define UINT_FAST64_TYPE (LONG_TYPE_SIZE == 64 ? itk_unsigned_long : itk_unsigned_long_long)

#define INTPTR_TYPE itk_long
#define UINTPTR_TYPE itk_unsigned_long

