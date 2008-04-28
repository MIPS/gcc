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

#ifndef __TIME_H__
#define __TIME_H__

#include <__cdefs.h>

/* Included for NULL, size_t */
#include <stddef.h>

__BEGIN_EXTERN_C

#define  CLOCKS_PER_SEC         1000000

typedef unsigned long clock_t;

typedef unsigned long time_t;

struct tm {
  int tm_sec;   /* Seconds after the minute [0,60] (Sic) */
  int tm_min;   /* Minutes after the hour [0,59] */
  int tm_hour;  /* Hours since midnight [0,23] */
  int tm_mday;  /* Day of the month [1,31] */
  int tm_mon;   /* Months since January [0,11] */
  int tm_year;  /* Years since 1900 */
  int tm_wday;  /* Days since Sunday [0,6] */
  int tm_yday;  /* Days since January, the 1st [0, 365] */
  int tm_isdst; /* Daylight saving times flag */
};

LIBSTD_LPROTO(clock_t, clock, void);

LIBSTD_LPROTO(double, difftime, time_t, time_t);

LIBSTD_LPROTO(time_t, mktime, struct tm *);

LIBSTD_LPROTO(time_t, time, time_t *);

LIBSTD_LPROTO(char *, asctime, const struct tm *);

LIBSTD_LPROTO(char *, ctime, const time_t *);

LIBSTD_LPROTO(struct tm *, gmtime, const time_t *);

LIBSTD_LPROTO(struct tm *, localtime, const time_t *);

LIBSTD_LPROTO(size_t, strftime, char *restrict, size_t, const char *restrict,
                        const struct tm *restrict);

__END_EXTERN_C

#endif /*__TIME_H__*/
