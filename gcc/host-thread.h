/* Host thread abstraction for GCC.
   Copyright (C) 2007
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_HOST_THREAD_H
#define GCC_HOST_THREAD_H

/* For now, just pthreads.  */
#include <pthread.h>

typedef pthread_mutex_t host_mutex;
typedef pthread_cond_t host_condition;

static inline host_mutex *
host_mutex_create (void)
{
  host_mutex *result = XNEW (host_mutex);
  pthread_mutex_init (result, NULL);
  return result;
}

static inline void
host_mutex_lock (host_mutex *m)
{
  pthread_mutex_lock (m);
}

static inline void
host_mutex_unlock (host_mutex *m)
{
  pthread_mutex_unlock (m);
}

static inline host_condition *
host_condition_create (void)
{
  host_condition *result = XNEW (host_condition);
  pthread_cond_init (result, NULL);
  return result;
}

static inline void
host_condition_wait (host_condition *c, host_mutex *m)
{
  pthread_cond_wait (c, m);
}

static inline void
host_condition_broadcast (host_condition *c)
{
  pthread_cond_broadcast (c);
}

static inline void
host_thread_create (void *(*func) (void *), void *arg)
{
  pthread_t thr;
  pthread_attr_t attr;
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
  /* FIXME: error handling.  */
  pthread_create (&thr, &attr, func, arg);
  pthread_attr_destroy (&attr);
}

#endif /* GCC_HOST_THREAD_H */
