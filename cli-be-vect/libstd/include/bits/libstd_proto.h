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

#ifndef __LIBSTD_PROTO_H__
#define __LIBSTD_PROTO_H__

#include <bits/libstd_types.h>

__BEGIN_EXTERN_C

/* used in errno.h */
LIBSTD_LPROTO(int *, __errno__get_ptr, void);

/* used in float.h */
LIBSTD_LPROTO(int, __flt_rounds, void);
LIBSTD_LPROTO(float, __flt_nan, void);
LIBSTD_LPROTO(float, __flt_infinity, void);
LIBSTD_LPROTO(float, __flt_epsilon, void);
LIBSTD_LPROTO(float, __flt_minval, void);
LIBSTD_LPROTO(float, __flt_maxval, void);
LIBSTD_LPROTO(float, __flt_hugeval, void);
LIBSTD_LPROTO(double, __dbl_epsilon, void);
LIBSTD_LPROTO(double, __dbl_minval, void);
LIBSTD_LPROTO(double, __dbl_maxval, void);
LIBSTD_LPROTO(double, __dbl_hugeval, void);

/* used in stdio.h */
LIBSTD_LPROTO(FILE *, __io_ftable_get_entry, int);

__END_EXTERN_C

#endif /*__LIBSTD_PROTO_H__*/
