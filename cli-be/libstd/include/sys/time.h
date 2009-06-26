/*

   Copyright (C) 2008-2009 Free Software Foundation, Inc.
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

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include <__cdefs.h>

__BEGIN_EXTERN_C

#include <time.h>

typedef unsigned long suseconds_t;

struct timeval {
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
};

struct timezone {
    int tz_minuteswest;     /* minutes west of Greenwich */
    int tz_dsttime;         /* type of DST correction */
};

LIBSTD_LPROTO(int, gettimeofday, struct timeval *tv, struct timezone *tz);
LIBSTD_LPROTO(int, settimeofday, const struct timeval *tv , const struct timezone *tz);

__END_EXTERN_C

#endif

