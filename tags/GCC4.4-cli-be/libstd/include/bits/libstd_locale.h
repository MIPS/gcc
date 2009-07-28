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

#ifndef __LIBSTD_LOCALE_H__
#define __LIBSTD_LOCALE_H__

#if !defined __LOCALE_H__ && !defined __LIBSTD_CRT_BUILD
#  error "Never use <bits/libstd_locale.h> directly; include <locale.h> instead."
#endif

__BEGIN_EXTERN_C

enum {
    __LIBSTD_LC_ALL      = 0,
    __LIBSTD_LC_COLLATE  = 1,
    __LIBSTD_LC_CTYPE    = 2,
    __LIBSTD_LC_MONETARY = 3,
    __LIBSTD_LC_NUMERIC  = 4,
    __LIBSTD_LC_TIME     = 5
};

__END_EXTERN_C

#endif /*__LIBSTD_LOCALE_H__*/
