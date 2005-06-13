/* Copyright (C) 2005 Free Software Foundation, Inc.
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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files, some
   of which are compiled with GCC, to produce an executable, this library
   does not by itself cause the resulting executable to be covered by the
   GNU General Public License.  This exception does not however invalidate
   any other reasons why the executable file might be covered by the GNU
   General Public License.  */

/* This file handles the maintainence of threads in response to team
   creation and termination.  */

#include "libgomp.h"
#include <stdlib.h>
#include <string.h>


/* This array manages threads spawned from the top level, which will
   return to the idle loop once the current PARALLEL construct ends.  */
static struct gomp_thread **gomp_threads;
static unsigned gomp_threads_size;
static unsigned gomp_threads_used;

/* This attribute contains PTHREAD_CREATE_DETACHED.  */
static pthread_attr_t gomp_thread_attr;

/* This is the libgomp per-thread data structure.  */
#ifdef HAVE_TLS
__thread struct gomp_thread gomp_tls_data;
#else
pthread_key_t gomp_tls_key;
#endif


/* Initialize and destroy a barrier structure.  */

static inline void
gomp_barrier_init (struct gomp_barrier *b)
{
  gomp_sem_init (&b->waiting, 0);
  gomp_sem_init (&b->release, 0);
}

static inline void
gomp_barrier_destroy (struct gomp_barrier *b)
{
  gomp_sem_destroy (&b->waiting);
  gomp_sem_destroy (&b->release);
}


/* This structure is used to communicate across pthread_create.  */

struct gomp_thread_start_data
{
  gomp_sem_t *initialized;
  struct gomp_thread *thr;
  bool nested;
};


/* This function is a pthread_create entry point.  This contains the idle
   loop in which a thread waits to be called up to become part of a team.  */

static void *
gomp_thread_start (void *xdata)
{
  struct gomp_thread_start_data *data = xdata;
  struct gomp_thread *thr;
  bool local_nested;

#ifdef HAVE_TLS
  thr = &gomp_tls_data;
#else
  struct gomp_thread local_thr;
  thr = &local_thr;
  pthread_setspecific (gomp_tls_key, thr);
#endif
  gomp_barrier_init (&thr->barrier);

  /* Extract what we need from data.  */
  local_nested = data->nested;

  /* Tell the world who we are.  */
  data->thr = thr;
  gomp_sem_post (data->initialized);

  do
    {
      /* Wait for work.  The master thread will fill in all of the team
	 and function data before releasing us.  */
      gomp_sem_wait (&thr->barrier.release);

      /* Work!  */
      thr->fn (thr->data);

      /* Clear out the team and function data.  This is a debugging
	 signal that we're in fact back in the dock.  */
      thr->fn = NULL;
      thr->data = NULL;
      thr->ts.team = NULL;
      thr->ts.work_share = NULL;
      thr->ts.team_id = 0;
      thr->ts.work_share_generation = 0;
      thr->ts.static_trip = 0;

      gomp_sem_post (&thr->barrier.waiting);
    }
  while (!local_nested);

  /* Wait for synchronization from the main thread before exiting.
     Otherwise we'll be freeing data while it's still in use.  */
  gomp_sem_wait (&thr->barrier.release);

  gomp_barrier_destroy (&thr->barrier);

  return NULL;
}


/* Create a new team data structure.  */

static struct gomp_team *
new_team (unsigned nthreads, struct gomp_work_share *work_share)
{
  struct gomp_team *team;

  team = gomp_malloc (sizeof (*team) + nthreads * sizeof (team->threads[0]));
  gomp_mutex_init (&team->work_share_lock);

  team->work_shares = gomp_malloc (4 * sizeof (struct gomp_work_share *));
  team->generation_mask = 3;
  team->oldest_live_gen = work_share == NULL;
  team->num_live_gen = work_share != NULL;
  team->work_shares[0] = work_share;

  team->nthreads = nthreads;

  gomp_barrier_init (&team->master_barrier);
  team->threads[0] = &team->master_barrier;

  return team;
}


/* Free a team data structure.  */

static void
free_team (struct gomp_team *team)
{
  free (team->work_shares);
  gomp_mutex_destroy (&team->work_share_lock);
  gomp_barrier_destroy (&team->master_barrier);
  free (team);
}


/* Launch a team.  */

void
gomp_team_start (void (*fn) (void *), void *data, unsigned nthreads,
		 struct gomp_work_share *work_share)
{
  struct gomp_thread_start_data start_data;
  struct gomp_thread *thr, *nthr;
  struct gomp_team *team;
  bool nested;
  unsigned i;

  thr = gomp_thread ();
  nested = thr->ts.team != NULL;

  team = new_team (nthreads, work_share);

  /* Always save the previous state, even if this isn't a nested team.
     In particular, we should save any work share state from an outer
     orphaned work share construct.  */
  team->prev_ts = thr->ts;

  thr->ts.team = team;
  thr->ts.work_share = work_share;
  thr->ts.team_id = 0;
  thr->ts.work_share_generation = 0;
  thr->ts.static_trip = 0;

  if (nthreads == 1)
    return;

  i = 1;

  /* We only allow the reuse of idle threads for non-nested PARALLEL
     regions.  This appears to be implied by the semantics of
     threadprivate variables, but perhaps that's reading too much into
     things.  Certainly it does prevent any locking problems, since
     only the initial program thread will modify gomp_threads.  */
  if (!nested)
    {
      unsigned n = gomp_threads_used;

      if (n < nthreads)
	gomp_threads_used = nthreads;
      else
	n = nthreads;

      /* Release existing idle threads.  */
      for (; i < n; ++i)
	{
	  nthr = gomp_threads[i];
	  nthr->ts.team = team;
	  nthr->ts.work_share = work_share;
	  nthr->ts.team_id = i;
	  nthr->ts.work_share_generation = 0;
	  nthr->ts.static_trip = 0;
	  nthr->fn = fn;
	  nthr->data = data;
	  team->threads[i] = &nthr->barrier;
	}

      /* ??? Should we terminate additional threads here?  If the number
	 of threads has been reduced because of dyn-var, terminating 
	 threads can free up resources...  */
      if (i == nthreads)
	goto do_release;

      /* If necessary, expand the size of the gomp_threads array.  It is
	 expected that changes in the number of threads is rare, thus we
	 make no effort to expand gomp_threads_size geometrically.  */
      if (nthreads >= gomp_threads_size)
	{
	  gomp_threads_size = nthreads + 1;
	  gomp_threads
	    = gomp_realloc (gomp_threads,
			    gomp_threads_size
			    * sizeof (struct gomp_thread_data *));
	}
    }

  start_data.initialized = &team->master_barrier.release;
  start_data.nested = nested;

  /* Launch new threads.  */
  for (; i < nthreads; ++i)
    {
      pthread_t pt;
      int err;

      err = pthread_create (&pt, &gomp_thread_attr,
			    gomp_thread_start, &start_data);
      if (err != 0)
	gomp_fatal ("Thread creation failed: %s", strerror (err));

      gomp_sem_wait (&team->master_barrier.release);

      nthr = start_data.thr;
      nthr->ts.team = team;
      nthr->ts.work_share = work_share;
      nthr->ts.team_id = i;
      nthr->ts.work_share_generation = 0;
      nthr->ts.static_trip = 0;
      nthr->fn = fn;
      nthr->data = data;
      team->threads[i] = &nthr->barrier;
      if (!nested)
	gomp_threads[i] = nthr;
    }

 do_release:
  for (i = 1; i < nthreads; ++i)
    gomp_sem_post (&team->threads[i]->release);
}


/* Terminate the current team.  This is only to be called by the master
   thread.  We assume that we must wait for the other threads.  */

void
gomp_team_end (void)
{
  struct gomp_thread *thr = gomp_thread ();
  struct gomp_team *team = thr->ts.team;
  unsigned i, n = team->nthreads;

  /* Wait for all other threads to terminate.  */
  for (i = 1; i < n; ++i)
    gomp_sem_wait (&team->threads[i]->waiting);

  thr->ts = team->prev_ts;

  /* If this was a nested team, the other threads are paused waiting
     to exit.  Release them.  */
  if (thr->ts.team)
    {
      unsigned i, n = team->nthreads;
      for (i = 1; i < n; ++i)
	gomp_sem_post (&team->threads[i]->release);
    }

  free_team (team);
}


/* Constructors for this file.  */

static void __attribute__((constructor))
initialize_team (void)
{
  struct gomp_thread *thr;

#ifndef HAVE_TLS
  pthread_key_create (&gomp_tls_key, free);
#endif

#ifdef HAVE_TLS
  thr = &gomp_tls_data;
#else
  thr = gomp_malloc_cleared (sizeof (*thr));
  pthread_setspecific (gomp_tls_key, thr);
#endif
  gomp_barrier_init (&thr->barrier);

  pthread_attr_init (&gomp_thread_attr);
  pthread_attr_setdetachstate (&gomp_thread_attr, PTHREAD_CREATE_DETACHED);
}
