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

#ifndef __WCTYPE_H__
#define __WCTYPE_H__

#include <__cdefs.h>

/* Included for NULL, wchar_t */
#include <stddef.h>

/* Included for wint_t, mbstate_t, WEOF */
#include <wchar.h>

__BEGIN_EXTERN_C

#error  "<wctype.h> : Sorry : Wide Characters not supported yet. "

typedef unsigned int wctype_t;
typedef const int * wctrans_t;

LIBSTD_LPROTO(wctype_t, wctype, const char *);

LIBSTD_LPROTO(wctrans_t, wctrans,const char *);
LIBSTD_LPROTO(wint_t, towctrans,wint_t, wctrans_t);

/* Wide-Character Classification and Mapping Functions */

LIBSTD_LPROTO(int, iswalnum, wint_t);
LIBSTD_LPROTO(int, iswalpha, wint_t);
LIBSTD_LPROTO(int, iswblank, wint_t);
LIBSTD_LPROTO(int, iswcntrl, wint_t);
LIBSTD_LPROTO(int, iswctype, wint_t, wctype_t);
LIBSTD_LPROTO(int, iswdigit, wint_t);
LIBSTD_LPROTO(int, iswgraph, wint_t);
LIBSTD_LPROTO(int, iswlower, wint_t);
LIBSTD_LPROTO(int, iswprint, wint_t);
LIBSTD_LPROTO(int, iswpunct, wint_t);
LIBSTD_LPROTO(int, iswspace, wint_t);
LIBSTD_LPROTO(int, iswupper, wint_t);
LIBSTD_LPROTO(int, iswxdigit, wint_t);
LIBSTD_LPROTO(wint_t, towlower,wint_t);
LIBSTD_LPROTO(wint_t, towupper,wint_t);
LIBSTD_LPROTO(wctrans_t, wctrans,const char *);

__END_EXTERN_C

#endif /*__WCTYPE_H__*/
