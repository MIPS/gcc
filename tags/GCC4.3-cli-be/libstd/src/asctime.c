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

#include <stdio.h>
#include <time.h>

static const char wday_name[7][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
static const char mon_name[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char asctime_[30];

LIBSTD_LPROTO_IMPL(char *, asctime, const struct tm *timeptr)
{
    LIBSTD_LNAME(sprintf)(asctime_,
                         "%.3s %.3s %3d %.2d:%.2d:%.2d %d\n",
                         wday_name[timeptr->tm_wday],
                         mon_name[timeptr->tm_mon],
                         timeptr->tm_mday,
                         timeptr->tm_hour,
                         timeptr->tm_min,
                         timeptr->tm_sec,
                         1900 + timeptr->tm_year);
    return asctime_;
}

