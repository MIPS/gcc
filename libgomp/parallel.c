/* Copyright (C) 2005, 2007 Free Software Foundation, Inc.
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

/* This file handles the (bare) PARALLEL construct.  */

#include "libgomp.h"


/* Determine the number of threads to be launched for a PARALLEL construct.
   This algorithm is explicitly described in OpenMP 3.0 section 2.4.1.
   SPECIFIED is a combination of the NUM_THREADS clause and the IF clause.
   If the IF clause is false, SPECIFIED is forced to 1.  When NUM_THREADS
   is not present, SPECIFIED is 0.  */

unsigned
gomp_resolve_num_threads (unsigned specified)
{
  struct gomp_thread *thread = gomp_thread();
  struct gomp_task_icv *icv;
  unsigned threads_requested, max_num_threads;
  unsigned threads_busy, max_threads_remaining;
  
  icv = gomp_icv();

  if (specified == 1)
    return 1;
  else if (thread->ts.level >= 1 && !icv->nest_var)
    return 1;
  else if (thread->ts.level >= gomp_max_active_levels_var)
    return 1;

  /* If NUM_THREADS not specified, use nthreads_var.  */
  if (specified == 0)
    threads_requested = icv->nthreads_var;
  else
    threads_requested = specified;

  /* ??? FIXME: We probably need a global variable that contains this
     value; walking the entire team tree doesn't sound like a Good Idea.  */
  threads_busy = 1;

  max_threads_remaining = gomp_thread_limit_var - threads_busy + 1;
  if (threads_requested <= max_threads_remaining)
    max_num_threads = threads_requested;
  else
    max_num_threads = max_threads_remaining;

  /* If dynamic threads are enabled, bound the number of threads
     that we launch.  */
  if (icv->dyn_var)
    {
      unsigned dyn = gomp_dynamic_max_threads ();
      if (dyn < max_num_threads)
	return dyn;
    }

  return max_num_threads;
}

void
GOMP_parallel_start (void (*fn) (void *), void *data, unsigned num_threads)
{
  num_threads = gomp_resolve_num_threads (num_threads);
  gomp_team_start (fn, data, num_threads, NULL);
}

void
GOMP_parallel_end (void)
{
  gomp_team_end ();
}


/* The public OpenMP API for thread and team related inquiries.  */

int
omp_get_num_threads (void)
{
  struct gomp_team *team = gomp_thread ()->ts.team;
  return team ? team->nthreads : 1;
}

int
omp_get_thread_num (void)
{
  return gomp_thread ()->ts.team_id;
}

/* This wasn't right for OpenMP 2.5.  Active region used to be non-zero
   when the IF clause doesn't evaluate to false, starting with OpenMP 3.0
   it is non-zero with more than one thread in the team.  */

int
omp_in_parallel (void)
{
  struct gomp_team *team = gomp_thread ()->ts.team;

  while (team)
    {
      if (team->nthreads > 1)
	return true;
      team = team->prev_ts.team;
    }

  return false;
}

int
omp_get_level (void)
{
  return gomp_thread ()->ts.level;
}

int
omp_get_ancestor_thread_num (int level)
{
  struct gomp_team_state *ts = &gomp_thread ()->ts;
  if (level < 0 || level > ts->level)
    return -1;
  for (level = ts->level - level; level > 0; --level)
    ts = &ts->team->prev_ts;
  return ts->team_id;
}

int
omp_get_team_size (int level)
{
  struct gomp_team_state *ts = &gomp_thread ()->ts;
  if (level < 0 || level > ts->level)
    return -1;
  for (level = ts->level - level; level > 0; --level)
    ts = &ts->team->prev_ts;
  if (ts->team == NULL)
    return 1;
  else
    return ts->team->nthreads;
}

int
omp_get_active_level (void)
{
  struct gomp_team *team = gomp_thread ()->ts.team;
  int active = 0;

  while (team)
    {
      if (team->nthreads > 1)
	++active;
      team = team->prev_ts.team;
    }

  return active;
}

ialias (omp_get_num_threads)
ialias (omp_get_thread_num)
ialias (omp_in_parallel)
ialias (omp_get_level)
ialias (omp_get_ancestor_thread_num)
ialias (omp_get_team_size)
ialias (omp_get_active_level)
