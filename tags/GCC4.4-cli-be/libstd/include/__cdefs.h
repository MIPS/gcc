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

#ifndef ____CDEFS_H__
#define ____CDEFS_H__

/* Define this library signature */
#define __LIBSTD__

#if defined(__cplusplus)
#  define __BEGIN_EXTERN_C extern "C" {
#  define __END_EXTERN_C   }
#else
#  define __BEGIN_EXTERN_C /* Not in C++ */
#  define __END_EXTERN_C   /* Not in C++ */
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__>= 199901L)
#  define __IS_ISO_C99
#endif

/*
 * Define the characterisitics of the target that are needed for proper include handling and compilation
 * This includes various types and size, but also target endiannes
*/

#define __Char_Is_Signed__
/* wchar_t is of type uint16_t */

#if defined __IS_ISO_C99
    /* restrict is ok */
#elif defined(__GNUC__)
#  define restrict __restrict
#else
#  define restrict /* no restrict */
#endif


#if !defined(__LIBSTD_CRT_BUILD)

#  if defined __GNU_CIL__
#    define LIBSTD_LNAME(_name) _name
#    define LIBSTD_LPROTO(type, name, ...) \
       extern type LIBSTD_LNAME(name)(__VA_ARGS__)
#  elif defined __GNUC__
#    define LIBSTD_LNAME(_name) Libstd_##_name
#    define LIBSTD_LPROTO(type, name, ...) \
       extern type name(__VA_ARGS__) asm ("Libstd_"#name)
#  else
#    error "Target not supported."
#  endif

#else
#  include "__cdefs_build.h"
#endif

#endif /*____CDEFS_H__*/
