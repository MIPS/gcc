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
#include <sys/times.h>

LIBSTD_LPROTO_IMPL(clock_t, times, struct tms *buf)
{
  unsigned long clks, tms_utime, tms_stime, tms_cutime, tms_cstime;

  if (LIBSTD_HNAME(gettimes) (&clks, &tms_utime, &tms_stime, &tms_cutime, &tms_cstime) == (clock_t)-1)
    return (clock_t)-1;
  
  
  buf->tms_utime = tms_utime;
  buf->tms_stime = tms_stime;
  buf->tms_cutime = tms_cutime;
  buf->tms_cstime = tms_cstime;
  
  return clks;
}

