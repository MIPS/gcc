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

#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <__cdefs.h>

/* Included for NULL, size_t, wchar_t */
#include <stddef.h>

__BEGIN_EXTERN_C

/* Storage Allocation */

LIBSTD_LPROTO(void *, malloc, size_t);
LIBSTD_LPROTO(void *, calloc, size_t, size_t);
LIBSTD_LPROTO(void *, realloc, void *, size_t);
LIBSTD_LPROTO(void, free, void *);

/* Random Number Generation */

#define RAND_MAX        32767

LIBSTD_LPROTO(int, rand, void);
LIBSTD_LPROTO(void, srand, unsigned int);

/* Numeric Conversions and Integer Arithmetic */

LIBSTD_LPROTO(double, atof, const char *);
LIBSTD_LPROTO(int, atoi, const char *);
LIBSTD_LPROTO(long, atol, const char *);
LIBSTD_LPROTO(long long, atoll, const char *);

LIBSTD_LPROTO(double, strtod, const char *restrict, char **restrict);
LIBSTD_LPROTO(float, strtof,const char *restrict, char **restrict);
LIBSTD_LPROTO(long double, strtold,const char *restrict, char **restrict);

LIBSTD_LPROTO(long, strtol, const char *restrict, char **restrict, int);
LIBSTD_LPROTO(long long, strtoll, const char *restrict, char **restrict, int);
LIBSTD_LPROTO(unsigned long, strtoul,const char *restrict, char **restrict, int);
LIBSTD_LPROTO(unsigned long long, strtoull,const char *restrict, char **restrict, int);

/* Environment Comunication */

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0

enum {
/* The implementation must support the registration of at least 32 atexit functions */
    __ATEXIT_MAX = 32
};
#define __ATEXIT_MAX __ATEXIT_MAX
typedef void (*__atexit_fun) (void);

LIBSTD_LPROTO(void, exit, int);
LIBSTD_LPROTO(void, _Exit, int);
LIBSTD_LPROTO(void, abort, void);
LIBSTD_LPROTO(int, atexit, __atexit_fun);

LIBSTD_LPROTO(char *, getenv, const char *);

LIBSTD_LPROTO(int, system, const char *);

/* Serching And Storing */

LIBSTD_LPROTO(void *, bsearch, const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
LIBSTD_LPROTO(void, qsort, void *, size_t, size_t, int (*)(const void *, const void *));


LIBSTD_LPROTO(int, abs, int);
LIBSTD_LPROTO(long, labs, long);
LIBSTD_LPROTO(long long, llabs, long long);

typedef struct div_t {
  int quot;
  int rem;
} div_t;

typedef struct ldiv_t {
  long quot;
  long rem;
} ldiv_t;

typedef struct lldiv_t {
  long long quot;
  long long rem;
} lldiv_t;


LIBSTD_LPROTO(div_t, div, int, int);
LIBSTD_LPROTO(ldiv_t, ldiv, long, long);
LIBSTD_LPROTO(lldiv_t, lldiv, long long, long long);

/* Multibyte, Wide-Character and String Conversions */

#define MB_CUR_MAX      sizeof(wchar_t)

LIBSTD_LPROTO(int, mblen, const char *, size_t);
LIBSTD_LPROTO(int, mbtowc, wchar_t *restrict, const char *restrict, size_t);
LIBSTD_LPROTO(int, wctomb, char *, wchar_t);

LIBSTD_LPROTO(size_t, mbstowcs, wchar_t *restrict, const char *restrict, size_t);
LIBSTD_LPROTO(size_t, wcstombs, char *restrict, const wchar_t *restrict, size_t);

__END_EXTERN_C

#endif /*__STDLIB_H__*/
