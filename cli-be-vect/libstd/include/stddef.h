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

#ifndef __STDDEF_H__
#define __STDDEF_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

#if !defined(_PTRDIFF_T)
  typedef long ptrdiff_t;
#  define _PTRDIFF_T
#endif


#if !defined(_SIZE_T)
  typedef unsigned int size_t;
#  define _SIZE_T
#endif

#if !defined(_WCHAR_T) && !defined(__cplusplus)
  typedef unsigned short wchar_t;
#  define _WCHAR_T
#endif


#if !defined(NULL)
#  define NULL    (0)
#endif


#if defined(__GNUC__)
#  define offsetof(TYPE, MEMBER) \
          __builtin_offsetof (TYPE, MEMBER)
#else
#  define __offset(PTR_TYPE, FIELD) \
          ((size_t)(((char *)(&(((PTR_TYPE)NULL)->FIELD))) - ((char *) NULL)))
#  define offsetof(TYPE, MEMBER) \
          __offset (TYPE*, MEMBER)
#endif

__END_EXTERN_C

#endif /*__STDDEF_H__*/
