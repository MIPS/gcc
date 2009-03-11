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

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD

#include <__cdefs.h>

#include <__host.h>
#include <time.h>

LIBSTD_LPROTO_IMPL(clock_t, clock, void)
{
    return LIBSTD_HNAME(clock)();
}

LIBSTD_LPROTO_IMPL(double, difftime, time_t time1, time_t time0)
{
    return (double)(time1 - time0);
}

LIBSTD_LPROTO_IMPL(time_t, mktime, struct tm *timeptr)
{
    /*If the calendar time cannot be represented, the function returns the value (time_t)(-1) */
    time_t time_t_ = (time_t)(-1);
    return time_t_;
}

LIBSTD_LPROTO_IMPL(time_t, time, time_t *tptr)
{
    return LIBSTD_HNAME(time)((int *)tptr);
}

static struct tm _gmtime;

LIBSTD_LPROTO_IMPL(struct tm *, gmtime, const time_t *tptr)
{
    LIBSTD_HNAME(gmtime)((int *)tptr,
                               &_gmtime.tm_sec,
                               &_gmtime.tm_min,
                               &_gmtime.tm_hour,
                               &_gmtime.tm_mday,
                               &_gmtime.tm_mon,
                               &_gmtime.tm_year,
                               &_gmtime.tm_wday,
                               &_gmtime.tm_yday,
                               &_gmtime.tm_isdst);
    return &_gmtime;
}

LIBSTD_LPROTO_IMPL(struct tm *, localtime, const time_t *timer)
{
    struct tm *localtime_ = NULL;
    /* Not implemented yet.*/
    return localtime_;
}
