/* Copyright (C) 2006-2013 Free Software Foundation, Inc.
   Contributed by Jakub Jelinek <jakub@redhat.com>.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This is a Linux specific implementation of a CPU affinity setting.  */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include "libgomp.h"
#include "proc.h"
#include <stdlib.h>
#include <unistd.h>

#ifdef HAVE_PTHREAD_AFFINITY_NP
static unsigned int affinity_counter;

#ifndef CPU_ALLOC_SIZE
#define CPU_ISSET_S(idx, size, set) CPU_ISSET(idx, set)
#define CPU_ZERO_S(size, set) CPU_ZERO(set)
#define CPU_SET_S(idx, size, set) CPU_SET(idx, set)
#endif

void
gomp_init_affinity (void)
{
  size_t idx, widx;
  unsigned long cpus = 0;
  cpu_set_t *cpusetnewp;

  if (gomp_cpusetp == NULL)
    {
      gomp_error ("could not get CPU affinity set");
      free (gomp_cpu_affinity);
      gomp_cpu_affinity = NULL;
      gomp_cpu_affinity_len = 0;
      return;
    }

#ifdef CPU_ALLOC_SIZE
  cpusetnewp = (cpu_set_t *) gomp_alloca (gomp_cpuset_size);
#else
  cpu_set_t cpusetnew;
  cpusetnewp = &cpusetnew;
#endif

  if (gomp_cpu_affinity_len == 0)
    {
      unsigned long count = gomp_cpuset_popcount (gomp_cpusetp);
      if (count >= 65536)
	count = 65536;
      gomp_cpu_affinity = malloc (count * sizeof (unsigned short));
      if (gomp_cpu_affinity == NULL)
	{
	  gomp_error ("not enough memory to store CPU affinity list");
	  return;
	}
      for (widx = idx = 0; widx < count && idx < 65536; idx++)
	if (CPU_ISSET_S (idx, gomp_cpuset_size, gomp_cpusetp))
	  {
	    cpus++;
	    gomp_cpu_affinity[widx++] = idx;
	  }
    }
  else
    {
      CPU_ZERO_S (gomp_cpuset_size, cpusetnewp);
      for (widx = idx = 0; idx < gomp_cpu_affinity_len; idx++)
	if (gomp_cpu_affinity[idx] < 8 * gomp_cpuset_size
	    && CPU_ISSET_S (gomp_cpu_affinity[idx], gomp_cpuset_size,
			    gomp_cpusetp))
	  {
	    if (! CPU_ISSET_S (gomp_cpu_affinity[idx], gomp_cpuset_size,
			       cpusetnewp))
	      {
		cpus++;
		CPU_SET_S (gomp_cpu_affinity[idx], gomp_cpuset_size,
			   cpusetnewp);
	    }
	  gomp_cpu_affinity[widx++] = gomp_cpu_affinity[idx];
	}
    }

  if (widx == 0)
    {
      gomp_error ("no CPUs left for affinity setting");
      free (gomp_cpu_affinity);
      gomp_cpu_affinity = NULL;
      gomp_cpu_affinity_len = 0;
      return;
    }

  gomp_cpu_affinity_len = widx;
  if (cpus < gomp_available_cpus)
    gomp_available_cpus = cpus;
  CPU_ZERO_S (gomp_cpuset_size, cpusetnewp);
  CPU_SET_S (gomp_cpu_affinity[0], gomp_cpuset_size, cpusetnewp);
  pthread_setaffinity_np (pthread_self (), gomp_cpuset_size,
			  cpusetnewp);
  affinity_counter = 1;
}

void
gomp_init_thread_affinity (pthread_attr_t *attr)
{
  unsigned int cpu;
  cpu_set_t *cpusetp;

#ifdef CPU_ALLOC_SIZE
  cpusetp = (cpu_set_t *) gomp_alloca (gomp_cpuset_size);
#else
  cpu_set_t cpuset;
  cpusetp = &cpuset;
#endif

  cpu = __atomic_fetch_add (&affinity_counter, 1, MEMMODEL_RELAXED);
  cpu %= gomp_cpu_affinity_len;
  CPU_ZERO_S (gomp_cpuset_size, cpusetp);
  CPU_SET_S (gomp_cpu_affinity[cpu], gomp_cpuset_size, cpusetp);
  pthread_attr_setaffinity_np (attr, gomp_cpuset_size, cpusetp);
}

#else

#include "../posix/affinity.c"

#endif
