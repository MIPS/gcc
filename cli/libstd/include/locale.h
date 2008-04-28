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

#ifndef __LOCALE_H__
#define __LOCALE_H__

#include <__cdefs.h>

/* Included for NULL */
#include <stddef.h>

#include <bits/libstd_locale.h>

__BEGIN_EXTERN_C

/* The standard wants macros */
#define LC_ALL      __LIBSTD_LC_ALL
#define LC_COLLATE  __LIBSTD_LC_COLLATE
#define LC_CTYPE    __LIBSTD_LC_CTYPE
#define LC_MONETARY __LIBSTD_LC_MONETARY
#define LC_NUMERIC  __LIBSTD_LC_NUMERIC
#define LC_TIME     __LIBSTD_LC_TIME

struct lconv {
  char *decimal_point;     /* "." */
  char *thousands_sep;     /* "" */
  char *grouping;          /* "" */
  char *int_curr_symbol;   /* "" */
  char *currency_symbol;   /* "" */
  char *mon_decimal_point; /* "" */
  char *mon_thousands_sep; /* "" */
  char *mon_grouping;      /* "" */
  char *positive_sign;     /* "" */
  char *negative_sign;     /* "" */
  char int_frac_digits;    /* CHAR_MAX */
  char frac_digits;        /* CHAR_MAX */
  char p_cs_precedes;      /* CHAR_MAX */
  char p_sep_by_space;     /* CHAR_MAX */
  char n_cs_precedes;      /* CHAR_MAX */
  char n_sep_by_space;     /* CHAR_MAX */
  char p_sign_posn;        /* CHAR_MAX */
  char n_sign_posn;        /* CHAR_MAX */
};

LIBSTD_LPROTO(char *, setlocale, int, const char *);

LIBSTD_LPROTO(struct lconv *, localeconv, void);

__END_EXTERN_C

#endif /*__LOCALE_H__*/
