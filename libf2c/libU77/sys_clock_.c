/* Copyright (C) 1996, 1998, 2001 Free Software Foundation, Inc.
This file is part of GNU Fortran libU77 library.

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Library General Public License as published
by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GNU Fortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with GNU Fortran; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <sys/types.h>
#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_SYS_TIMES_H
#  include <sys/times.h>
#endif
#include <limits.h>
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if defined (_WIN32)
#  include <windows.h>
#  undef min
#  undef max
#endif
#include <errno.h>		/* for ENOSYS */
#include "f2c.h"

int
G77_system_clock_0 (integer * count, integer * count_rate,
		    integer * count_max)
{
#if defined (_WIN32)
  /* Windows32 provides a slew of interfaces for getting time information;
     we use the high resolution performance counters available on all ix86 
     and Alphas. The downside is that all the information is 64bit. We
     could use GetTickCount or timeGetTime interface, but then the resolution
     is only 1.0e-3s. */

  unsigned long long clock_freq;
  unsigned long long cnt;
  LARGE_INTEGER counter_val;
  LARGE_INTEGER freq;
  if (! QueryPerformanceFrequency (&freq))
    {
      errno = ENOSYS;
      return -1;
    }

#ifdef _GNU_H_WINDOWS32_STRUCTURES
  clock_freq = ((unsigned long long) freq.HighPart << 32)
	       + ((unsigned) freq.LowPart);
#else
  clock_freq = freq.QuadPart;
#endif
  if (count_rate) 
    {
       /* the following cast is obviously dubious, but there is only so much
          one can do with 32bit quantities. */
      *count_rate = (unsigned long) clock_freq;
    }
  if (count_max)		/* optional arg present? */
    *count_max = INT_MAX;		/* dubious */

  if (! QueryPerformanceCounter (&counter_val))
    {
      errno = ENOSYS;
      return -1;
    }

#ifdef _GNU_H_WINDOWS32_STRUCTURES
  cnt = ((unsigned long long) counter_val.HighPart << 32)
	+ ((unsigned) counter_val.LowPart);
#else
  cnt = counter_val.QuadPart;
#endif
  
  /* another dubious cast. */
  *count = (unsigned long) cnt;
  return 0;
#elif defined (HAVE_TIMES)
  struct tms buffer;
  unsigned long cnt;
  if (count_rate)
    {
#ifdef _SC_CLK_TCK
      *count_rate = sysconf (_SC_CLK_TCK);
#elif defined CLOCKS_PER_SECOND
      *count_rate = CLOCKS_PER_SECOND;
#elif defined CLK_TCK
      *count_rate = CLK_TCK;
#elif defined HZ
      *count_rate = HZ;
#else
#error Dont know clock tick length
#endif
    }
  if (count_max)		/* optional arg present? */
    *count_max = INT_MAX;	/* dubious */
  cnt = times (&buffer);
  if (cnt > (unsigned long) (INT_MAX))
    *count = INT_MAX;		/* also dubious */
  else
    *count = cnt;
  return 0;
#else /* ! HAVE_TIMES */
  errno = ENOSYS;
  return -1;
#endif /* ! HAVE_TIMES */
}
