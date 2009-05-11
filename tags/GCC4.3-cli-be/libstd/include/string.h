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

#ifndef __STRING_H__
#define __STRING_H__

#include <__cdefs.h>

/* Included for NULL, size_t */
#include <stddef.h>

__BEGIN_EXTERN_C

LIBSTD_LPROTO(void *, memccpy, void *, const void *, int, size_t);

LIBSTD_LPROTO(void *, memchr, const void *, int, size_t);

LIBSTD_LPROTO(int, memcmp, const void *, const void *, size_t);

LIBSTD_LPROTO(void *, memcpy, void *restrict, const void *restrict, size_t);

LIBSTD_LPROTO(void *, memmove, void *, const void *, size_t);

LIBSTD_LPROTO(void *, memset, void *, int, size_t);


LIBSTD_LPROTO(char *, strcat, char *restrict, const char *restrict);

LIBSTD_LPROTO(char *, strncat, char *restrict, const char *restrict, size_t);

LIBSTD_LPROTO(int, strcmp, const char *, const char *);

LIBSTD_LPROTO(int, strncmp, const char *, const char *, size_t);

LIBSTD_LPROTO(char *, strcpy, char *restrict, const char *restrict);

LIBSTD_LPROTO(char *, strncpy, char *restrict, const char *restrict, size_t);

LIBSTD_LPROTO(size_t, strlen, const char *);

LIBSTD_LPROTO(char *, strchr, const char *, int);

LIBSTD_LPROTO(char *, strrchr, const char *, int);

LIBSTD_LPROTO(size_t, strspn, const char*, const char *);

LIBSTD_LPROTO(size_t, strcspn, const char *, const char *);

LIBSTD_LPROTO(char *, strpbrk, const char *, const char *);

LIBSTD_LPROTO(char *, strtok, char *restrict, const char *restrict);

LIBSTD_LPROTO(char *, strstr, const char *, const char *);

LIBSTD_LPROTO(int, strcoll, const char *, const char *);

LIBSTD_LPROTO(size_t, strxfrm, char *restrict, const char *restrict, size_t);


LIBSTD_LPROTO(char *, strerror, int);

__END_EXTERN_C

#endif /*__STRING_H__*/
