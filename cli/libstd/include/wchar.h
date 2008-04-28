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

#ifndef __WCHAR_H__
#define __WCHAR_H__

#include <__cdefs.h>

/* Included for NULL, size_t and wchar_t */
#include <stddef.h>

/* Included for va_list */
#include <stdarg.h>

__BEGIN_EXTERN_C

#error  "<wchar.h> : Sorry : Wide Characters not supported yet. "

typedef unsigned int wint_t;

typedef struct mbstate_t {
  union mbstate_t_v_ {
      wint_t wch_;
      unsigned char wc_[4];
  } v_;
  int c_;
} mbstate_t;

/* struct tm is declared as incomplete */
struct tm;

#define WCHAR_MIN	0
#define WCHAR_MAX	(UINT16_MAX)

#define WEOF ((wint_t)0xFFFFFFFFU)



/* Conversions Between Wide and Multybyte Characters */

LIBSTD_LPROTO(size_t, mbrlen, const char *restrict, size_t, struct mbstate_t *restrict);
LIBSTD_LPROTO(wint_t, btowc, int);
LIBSTD_LPROTO(size_t, mbrtowc, wchar_t *restrict, const char * restrict, size_t, struct mbstate_t *restrict);
LIBSTD_LPROTO(int, wctob, wint_t);
LIBSTD_LPROTO(size_t, wcrtomb, char *restrict, wchar_t, struct mbstate_t * restrict);
LIBSTD_LPROTO(int, mbsinit, const struct mbstate_t *);

/* Conversions Between Wide and Multibytes Strings*/

LIBSTD_LPROTO(size_t, mbsrtowcs, wchar_t *restrict, const char **restrict, size_t, struct mbstate_t *restrict);
LIBSTD_LPROTO(size_t, wcsrtombs, char *restrict, const char **restrict, size_t, struct mbstate_t *restrict);

/* Conversions to Arithmetic Types*/

LIBSTD_LPROTO(double, wcstod, const wchar_t *restrict, wchar_t **restrict);
LIBSTD_LPROTO(float, wcstof, const wchar_t *restrict, wchar_t **restrict);
LIBSTD_LPROTO(long double, wcstold, const wchar_t *restrict, wchar_t **restrict);
LIBSTD_LPROTO(long, wcstol, const wchar_t *restrict, wchar_t **restrict, int);
LIBSTD_LPROTO(int64_t, wcstoll, const wchar_t *restrict, wchar_t **restrict, int);
LIBSTD_LPROTO(unsigned long, wcstoul, const wchar_t *restrict, wchar_t **restrict, int);
LIBSTD_LPROTO(uint64_t, wcstoull, const wchar_t *restrict, wchar_t **restrict, int);


/* Wide Input/Output Functions */


LIBSTD_LPROTO(wint_t, fgetwc, FILE *);
LIBSTD_LPROTO(wint_t, fgetws, wchar_t *restrict s, int, FILE * restrict);
LIBSTD_LPROTO(wint_t, fputwc, wchar_t, FILE *);
LIBSTD_LPROTO(wint_t, fputws, const wchar_t *restrict, FILE *);
LIBSTD_LPROTO(int, fwide, FILE *, int);
LIBSTD_LPROTO(int, fwprintf, FILE *restrict, const wchar_t *restrict, ...);
LIBSTD_LPROTO(int, fwscanf, FILE *restrict, const wchar_t *restrict, ...);
LIBSTD_LPROTO(wint_t, getwc, FILE *);
LIBSTD_LPROTO(wint_t, getwchar, void);
LIBSTD_LPROTO(wint_t, putwc, wchar_t, FILE *);
LIBSTD_LPROTO(wint_t, putwchar, wchar_t);
LIBSTD_LPROTO(int, swprintf, FILE *restrict, const wchar_t *restrict, ...);
LIBSTD_LPROTO(int, swscanf, FILE *restrict, const wchar_t *restrict, ...);
LIBSTD_LPROTO(wint_t, ungetwc, wint_t, FILE *);

LIBSTD_LPROTO(int, vfwprintf, FILE *restrict, const wchar_t *restrict, va_list);
LIBSTD_LPROTO(int, vfwscanf, FILE *restrict, const wchar_t *restrict, va_list);
LIBSTD_LPROTO(int, vswprintf, FILE *restrict, size_t, const wchar_t *restrict, va_list);
LIBSTD_LPROTO(int, vswscanf, FILE *restrict, const wchar_t *restrict, va_list);
LIBSTD_LPROTO(int, vwprintf, const wchar_t *restrict, va_list);
LIBSTD_LPROTO(int, vwscanf, const wchar_t *restrict, va_list);
LIBSTD_LPROTO(int, wprintf, const wchar_t *restrict, ...);
LIBSTD_LPROTO(int, wscanf, const wchar_t *restrict, ...);

/* Wide String Functions */

LIBSTD_LPROTO(wchar_t *, wcscat, wchar_t *restrict, const wchar_t *restrict);
LIBSTD_LPROTO(wchar_t *, wcschr, const wchar_t *, wchar_t);
LIBSTD_LPROTO(int, wcscmp, const wchar_t *, const wchar_t *);
LIBSTD_LPROTO(int, wcscoll, const wchar_t *, const wchar_t *);
LIBSTD_LPROTO(wchar_t *, wcscpy, wchar_t *restrict, const wchar_t *restrict);
LIBSTD_LPROTO(size_t, wcscspn, const wchar_t *, const wchar_t *);
LIBSTD_LPROTO(size_t, wcslen, const wchar_t *);
LIBSTD_LPROTO(wchar_t *, wcsncat, wchar_t *restrict, const wchar_t *restrict, size_t);
LIBSTD_LPROTO(int, wcsncmp, const wchar_t *, const wchar_t *, size_t);
LIBSTD_LPROTO(wchar_t *, wcsncpy, wchar_t *restrict, const wchar_t *restrict, size_t);
LIBSTD_LPROTO(wchar_t *, wcspbrk, const wchar_t *, const wchar_t *);

LIBSTD_LPROTO(wchar_t *, wcsrchr, const wchar_t *, wchar_t);
LIBSTD_LPROTO(size_t, wcsspn, const wchar_t *, const wchar_t *);
LIBSTD_LPROTO(wchar_t *, wcsstr, const wchar_t *, const wchar_t *);
LIBSTD_LPROTO(wchar_t *, wcstok, const wchar_t * restrict, const wchar_t * restrict, wchar_t ** restrict);
LIBSTD_LPROTO(size_t, wcsxfrm, wchar_t * restrict, const wchar_t * restrict, size_t);
LIBSTD_LPROTO(wchar_t *, wmemchr, const wchar_t *, wchar_t, size_t);
LIBSTD_LPROTO(wchar_t *, wmemcpy, wchar_t *restrict, const wchar_t *restrict, size_t);
LIBSTD_LPROTO(wchar_t *, wmemmove, wchar_t *, const wchar_t *, size_t);
LIBSTD_LPROTO(wchar_t *, wmemset, wchar_t *, wchar_t, size_t);

LIBSTD_LPROTO(size_t, wcsftime,wchar_t * restrict , size_t, const wchar_t *restrict, const struct tm * restrict);

__END_EXTERN_C

#endif /*__WCHAR_H__*/
