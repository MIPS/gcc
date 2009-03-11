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

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

#undef assert

#ifdef NDEBUG

#  define assert(ignore) ((void)0)

#else /* !NDEBUG */

LIBSTD_LPROTO(void, __assert, const char *, const char *, const char *, int);


#  if defined __IS_ISO_C99

#    define assert(scalar_expression) \
            (void)( (scalar_expression) || (__assert(#scalar_expression, __func__, __FILE__, __LINE__), 0))

#  else /* !__IS_ISO_C99 */

#   define assert(scalar_expression) \
           (void)( (scalar_expression) || (__assert(#scalar_expression, /*NULL*/0, __FILE__, __LINE__), 0))

#  endif /* __IS_ISO_C99 */


#endif /* NDEBUG */


__END_EXTERN_C


#endif
