/* Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License 
   along with libgomp; see the file COPYING.LIB.  If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.  */

/* As a special exception, if you link this library with other files, some
   of which are compiled with GCC, to produce an executable, this library
   does not by itself cause the resulting executable to be covered by the
   GNU General Public License.  This exception does not however invalidate
   any other reasons why the executable file might be covered by the GNU
   General Public License.  */

/* This file defines the OpenMP internal control variables, and arranges
   for them to be initialized from environment variables at startup.  */

#include "libgomp.h"
#include "libgomp_f.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


unsigned long gomp_nthreads_var = 1;
bool gomp_dyn_var = false;
bool gomp_nest_var = false;
enum gomp_schedule_type gomp_run_sched_var = GFS_DYNAMIC;
unsigned long gomp_run_sched_chunk = 1;
unsigned short *gomp_cpu_affinity;
size_t gomp_cpu_affinity_len;
#ifndef HAVE_SYNC_BUILTINS
gomp_mutex_t gomp_remaining_threads_lock;
#endif
unsigned long gomp_available_cpus = 1, gomp_managed_threads = 1;
unsigned long long gomp_spin_count_var, gomp_throttled_spin_count_var;

/* Parse the OMP_SCHEDULE environment variable.  */

static void
parse_schedule (void)
{
  char *env, *end;

  env = getenv ("OMP_SCHEDULE");
  if (env == NULL)
    return;

  while (isspace ((unsigned char) *env))
    ++env;
  if (strncasecmp (env, "static", 6) == 0)
    {
      gomp_run_sched_var = GFS_STATIC;
      env += 6;
    }
  else if (strncasecmp (env, "dynamic", 7) == 0)
    {
      gomp_run_sched_var = GFS_DYNAMIC;
      env += 7;
    }
  else if (strncasecmp (env, "guided", 6) == 0)
    {
      gomp_run_sched_var = GFS_GUIDED;
      env += 6;
    }
  else
    goto unknown;

  while (isspace ((unsigned char) *env))
    ++env;
  if (*env == '\0')
    return;
  if (*env++ != ',')
    goto unknown;
  while (isspace ((unsigned char) *env))
    ++env;
  if (*env == '\0')
    goto invalid;

  gomp_run_sched_chunk = strtoul (env, &end, 10);
  while (isspace ((unsigned char) *end))
    ++end;
  if (*end != '\0')
    goto invalid;
  return;

 unknown:
  gomp_error ("Unknown value for environment variable OMP_SCHEDULE");
  return;

 invalid:
  gomp_error ("Invalid value for chunk size in "
	      "environment variable OMP_SCHEDULE");
  gomp_run_sched_chunk = 1;
  return;
}

/* Parse an unsigned long environment varible.  Return true if one was
   present and it was successfully parsed.  */

static bool
parse_unsigned_long (const char *name, unsigned long *pvalue)
{
  char *env, *end;
  unsigned long value;

  env = getenv (name);
  if (env == NULL)
    return false;

  while (isspace ((unsigned char) *env))
    ++env;
  if (*env == '\0')
    goto invalid;

  value = strtoul (env, &end, 10);
  while (isspace ((unsigned char) *end))
    ++end;
  if (*end != '\0')
    goto invalid;

  *pvalue = value;
  return true;

 invalid:
  gomp_error ("Invalid value for environment variable %s", name);
  return false;
}

/* Parse the GOMP_SPINCOUNT environment varible.  Return true if one was
   present and it was successfully parsed.  */

static bool
parse_spincount (const char *name, unsigned long long *pvalue)
{
  char *env, *end;
  unsigned long long value, mult = 1;

  env = getenv (name);
  if (env == NULL)
    return false;

  while (isspace ((unsigned char) *env))
    ++env;
  if (*env == '\0')
    goto invalid;

  if (strncasecmp (env, "infinite", 8) == 0
      || strncasecmp (env, "infinity", 8) == 0)
    {
      value = ~0ULL;
      end = env + 8;
      goto check_tail;
    }

  errno = 0;
  value = strtoull (env, &end, 10);
  if (errno)
    goto invalid;

  while (isspace ((unsigned char) *end))
    ++end;
  if (*end != '\0')
    {
      switch (tolower (*end))
	{
	case 'k':
	  mult = 1000LL;
	  break;
	case 'm':
	  mult = 1000LL * 1000LL;
	  break;
	case 'g':
	  mult = 1000LL * 1000LL * 1000LL;
	  break;
	case 't':
	  mult = 1000LL * 1000LL * 1000LL * 1000LL;
	  break;
	default:
	  goto invalid;
	}
      ++end;
     check_tail:
      while (isspace ((unsigned char) *end))
	++end;
      if (*end != '\0')
	goto invalid;
    }

  if (value > ~0ULL / mult)
    value = ~0ULL;
  else
    value *= mult;

  *pvalue = value;
  return true;

 invalid:
  gomp_error ("Invalid value for environment variable %s", name);
  return false;
}

/* Parse a boolean value for environment variable NAME and store the 
   result in VALUE.  */

static void
parse_boolean (const char *name, bool *value)
{
  const char *env;

  env = getenv (name);
  if (env == NULL)
    return;

  while (isspace ((unsigned char) *env))
    ++env;
  if (strncasecmp (env, "true", 4) == 0)
    {
      *value = true;
      env += 4;
    }
  else if (strncasecmp (env, "false", 5) == 0)
    {
      *value = false;
      env += 5;
    }
  else
    env = "X";
  while (isspace ((unsigned char) *env))
    ++env;
  if (*env != '\0')
    gomp_error ("Invalid value for environment variable %s", name);
}

/* Parse the GOMP_CPU_AFFINITY environment varible.  Return true if one was
   present and it was successfully parsed.  */

static bool
parse_affinity (void)
{
  char *env, *end;
  unsigned long cpu_beg, cpu_end, cpu_stride;
  unsigned short *cpus = NULL;
  size_t allocated = 0, used = 0, needed;

  env = getenv ("GOMP_CPU_AFFINITY");
  if (env == NULL)
    return false;

  do
    {
      while (*env == ' ' || *env == '\t')
	env++;

      cpu_beg = strtoul (env, &end, 0);
      cpu_end = cpu_beg;
      cpu_stride = 1;
      if (env == end || cpu_beg >= 65536)
	goto invalid;

      env = end;
      if (*env == '-')
	{
	  cpu_end = strtoul (++env, &end, 0);
	  if (env == end || cpu_end >= 65536 || cpu_end < cpu_beg)
	    goto invalid;

	  env = end;
	  if (*env == ':')
	    {
	      cpu_stride = strtoul (++env, &end, 0);
	      if (env == end || cpu_stride == 0 || cpu_stride >= 65536)
		goto invalid;

	      env = end;
	    }
	}

      needed = (cpu_end - cpu_beg) / cpu_stride + 1;
      if (used + needed >= allocated)
	{
	  unsigned short *new_cpus;

	  if (allocated < 64)
	    allocated = 64;
	  if (allocated > needed)
	    allocated <<= 1;
	  else
	    allocated += 2 * needed;
	  new_cpus = realloc (cpus, allocated * sizeof (unsigned short));
	  if (new_cpus == NULL)
	    {
	      free (cpus);
	      gomp_error ("not enough memory to store GOMP_CPU_AFFINITY list");
	      return false;
	    }

	  cpus = new_cpus;
	}

      while (needed--)
	{
	  cpus[used++] = cpu_beg;
	  cpu_beg += cpu_stride;
	}

      while (*env == ' ' || *env == '\t')
	env++;

      if (*env == ',')
	env++;
      else if (*env == '\0')
	break;
    }
  while (1);

  gomp_cpu_affinity = cpus;
  gomp_cpu_affinity_len = used;
  return true;

 invalid:
  gomp_error ("Invalid value for enviroment variable GOMP_CPU_AFFINITY");
  return false;
}

static void __attribute__((constructor))
initialize_env (void)
{
  unsigned long stacksize;

  /* Do a compile time check that mkomp_h.pl did good job.  */
  omp_check_defines ();

  parse_schedule ();
  parse_boolean ("OMP_DYNAMIC", &gomp_dyn_var);
  parse_boolean ("OMP_NESTED", &gomp_nest_var);
  gomp_init_num_threads ();
  gomp_available_cpus = gomp_nthreads_var;
  if (!parse_unsigned_long ("OMP_NUM_THREADS", &gomp_nthreads_var))
    gomp_nthreads_var = gomp_available_cpus;
  if (parse_affinity ())
    gomp_init_affinity ();
  if (!parse_spincount ("GOMP_SPINCOUNT", &gomp_spin_count_var))
    {
      /* Using a rough estimation of 100000 spins per msec,
	 use 200 msec blocking.
	 Depending on the CPU speed, this can be e.g. 5 times longer
	 or 5 times shorter.  */
      gomp_spin_count_var = 20000000LL;
    }
  /* gomp_throttled_spin_count_var is used when there are more libgomp
     managed threads than available CPUs.  Use very short spinning.  */
  gomp_throttled_spin_count_var = 100LL;
  if (gomp_throttled_spin_count_var > gomp_spin_count_var)
    gomp_throttled_spin_count_var = gomp_spin_count_var;

  /* Not strictly environment related, but ordering constructors is tricky.  */
  pthread_attr_init (&gomp_thread_attr);
  pthread_attr_setdetachstate (&gomp_thread_attr, PTHREAD_CREATE_DETACHED);

  if (parse_unsigned_long ("GOMP_STACKSIZE", &stacksize))
    {
      int err;

      stacksize *= 1024;
      err = pthread_attr_setstacksize (&gomp_thread_attr, stacksize);

#ifdef PTHREAD_STACK_MIN
      if (err == EINVAL)
	{
	  if (stacksize < PTHREAD_STACK_MIN)
	    gomp_error ("Stack size less than minimum of %luk",
			PTHREAD_STACK_MIN / 1024ul
			+ (PTHREAD_STACK_MIN % 1024 != 0));
	  else
	    gomp_error ("Stack size larger than system limit");
	}
      else
#endif
      if (err != 0)
	gomp_error ("Stack size change failed: %s", strerror (err));
    }
}


/* The public OpenMP API routines that access these variables.  */

void
omp_set_num_threads (int n)
{
  gomp_nthreads_var = n;
}

void
omp_set_dynamic (int val)
{
  gomp_dyn_var = val;
}

int
omp_get_dynamic (void)
{
  return gomp_dyn_var;
}

void
omp_set_nested (int val)
{
  gomp_nest_var = val;
}

int
omp_get_nested (void)
{
  return gomp_nest_var;
}

ialias (omp_set_dynamic)
ialias (omp_set_nested)
ialias (omp_set_num_threads)
ialias (omp_get_dynamic)
ialias (omp_get_nested)
