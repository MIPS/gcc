/* Definitions for <stdint.h> types on systems using newlib.
   Copyright (C) 2008, 2009, 2010 Free Software Foundation, Inc.

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

/* newlib uses 32-bit long in certain cases for all non-SPU
   targets.  */
#ifndef STDINT_LONG32
#define STDINT_LONG32 (LONG_TYPE_SIZE == 32)
#endif

#define SIG_ATOMIC_TYPE itk_int

/* The newlib logic actually checks for sizes greater than 32 rather
   than equal to 64 for various 64-bit types.  */

#define INT8_TYPE (CHAR_TYPE_SIZE == 8 ? itk_signed_char : itk_none)
#define INT16_TYPE (SHORT_TYPE_SIZE == 16 ? itk_short : INT_TYPE_SIZE == 16 ? itk_int : CHAR_TYPE_SIZE == 16 ? itk_signed_char : itk_none)
#define INT32_TYPE (STDINT_LONG32 ? itk_long : INT_TYPE_SIZE == 32 ? itk_int : SHORT_TYPE_SIZE == 32 ? itk_short : CHAR_TYPE_SIZE == 32 ? itk_signed_char : itk_none)
#define INT64_TYPE (LONG_TYPE_SIZE == 64 ? itk_long : LONG_LONG_TYPE_SIZE == 64 ? itk_long_long : INT_TYPE_SIZE == 64 ? itk_int : itk_none)
#define UINT8_TYPE (CHAR_TYPE_SIZE == 8 ? itk_unsigned_char : itk_none)
#define UINT16_TYPE (SHORT_TYPE_SIZE == 16 ? itk_unsigned_short : INT_TYPE_SIZE == 16 ? itk_unsigned_int : CHAR_TYPE_SIZE == 16 ? itk_unsigned_char : itk_none)
#define UINT32_TYPE (STDINT_LONG32 ? itk_unsigned_long : INT_TYPE_SIZE == 32 ? itk_unsigned_int : SHORT_TYPE_SIZE == 32 ? itk_unsigned_short : CHAR_TYPE_SIZE == 32 ? itk_unsigned_char : itk_none)
#define UINT64_TYPE (LONG_TYPE_SIZE == 64 ? itk_unsigned_long : LONG_LONG_TYPE_SIZE == 64 ? itk_unsigned_long_long: INT_TYPE_SIZE == 64 ? itk_unsigned_int : itk_none)

#define INT_LEAST8_TYPE (INT8_TYPE != itk_none ? INT8_TYPE : INT16_TYPE != itk_none ? INT16_TYPE : INT32_TYPE != itk_none ? INT32_TYPE : INT64_TYPE != itk_none ? INT64_TYPE : itk_none)
#define INT_LEAST16_TYPE (INT16_TYPE != itk_none ? INT16_TYPE : INT32_TYPE != itk_none ? INT32_TYPE : INT64_TYPE != itk_none ? INT64_TYPE : itk_none)
#define INT_LEAST32_TYPE (INT32_TYPE != itk_none ? INT32_TYPE : INT64_TYPE != itk_none ? INT64_TYPE : itk_none)
#define INT_LEAST64_TYPE INT64_TYPE
#define UINT_LEAST8_TYPE (UINT8_TYPE != itk_none ? UINT8_TYPE : UINT16_TYPE != itk_none ? UINT16_TYPE : UINT32_TYPE != itk_none ? UINT32_TYPE : UINT64_TYPE != itk_none ? UINT64_TYPE : itk_none)
#define UINT_LEAST16_TYPE (UINT16_TYPE != itk_none ? UINT16_TYPE : UINT32_TYPE != itk_none ? UINT32_TYPE : UINT64_TYPE != itk_none ? UINT64_TYPE : itk_none)
#define UINT_LEAST32_TYPE (UINT32_TYPE != itk_none ? UINT32_TYPE : UINT64_TYPE != itk_none ? UINT64_TYPE : itk_none)
#define UINT_LEAST64_TYPE UINT64_TYPE

#define INT_FAST8_TYPE (INT_TYPE_SIZE >= 8 ? itk_int : INT_LEAST8_TYPE)
#define INT_FAST16_TYPE (INT_TYPE_SIZE >= 16 ? itk_int : INT_LEAST16_TYPE)
#define INT_FAST32_TYPE (INT_TYPE_SIZE >= 32 ? itk_int : INT_LEAST32_TYPE)
#define INT_FAST64_TYPE (INT_TYPE_SIZE >= 64 ? itk_int : INT_LEAST64_TYPE)
#define UINT_FAST8_TYPE (INT_TYPE_SIZE >= 8 ? itk_unsigned_int : UINT_LEAST8_TYPE)
#define UINT_FAST16_TYPE (INT_TYPE_SIZE >= 16 ? itk_unsigned_int : UINT_LEAST16_TYPE)
#define UINT_FAST32_TYPE (INT_TYPE_SIZE >= 32 ? itk_unsigned_int : UINT_LEAST32_TYPE)
#define UINT_FAST64_TYPE (INT_TYPE_SIZE >= 64 ? itk_unsigned_int : UINT_LEAST64_TYPE)

/* Newlib uses the unsigned type corresponding to ptrdiff_t for
   uintptr_t; this is the same as size_t for most newlib-using
   targets.  */
#define INTPTR_TYPE PTRDIFF_TYPE
#ifndef UINTPTR_TYPE
#define UINTPTR_TYPE SIZE_TYPE
#endif
