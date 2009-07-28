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

#ifndef __CTYPE_H__
#define __CTYPE_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

LIBSTD_LPROTO(int, isalnum, int);
LIBSTD_LPROTO(int, isalpha, int);
LIBSTD_LPROTO(int, isblank, int);
LIBSTD_LPROTO(int, iscntrl, int);
LIBSTD_LPROTO(int, isdigit, int);
LIBSTD_LPROTO(int, isgraph, int);
LIBSTD_LPROTO(int, islower, int);
LIBSTD_LPROTO(int, isprint, int);
LIBSTD_LPROTO(int, ispunct, int);
LIBSTD_LPROTO(int, isspace, int);
LIBSTD_LPROTO(int, isupper, int);
LIBSTD_LPROTO(int, isxdigit, int);
LIBSTD_LPROTO(int, tolower, int);
LIBSTD_LPROTO(int, toupper, int);

__END_EXTERN_C

#endif /*__CTYPE_H__*/
