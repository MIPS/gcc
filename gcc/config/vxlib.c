/* Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Zack Weinberg <zack@codesourcery.com>

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* Threads compatibility routines for libgcc2 for VxWorks.
   These are out-of-line routines called from gthr-vxworks.h.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

#include "tconfig.h"
#include "tsystem.h"
#include "gthr.h"

#include <vxWorks.h>
#include <vxLib.h>
#include <taskLib.h>
#include <taskHookLib.h>

/* Init-once operation.

   This would be a clone of the implementation from gthr-solaris.h,
   except that we have a bootstrap problem - the whole point of this
   exercise is to prevent double initialization, but if two threads
   are racing with each other, once->mutex is liable to be initialized
   by both.  Then each thread will lock its own mutex, and proceed to
   call the initialization routine.

   So instead we use a bare atomic primitive (vxTas()) to handle
   mutual exclusion.  Threads losing the race then busy-wait, calling
   taskDelay() to yield the processor, until the initialization is
   completed.  Inefficient, but reliable.  */

int
__gthread_once (__gthread_once_t *guard, void (*func)(void))
{
  if (guard->done)
    return 0;

  while (!vxTas ((void *)&guard->busy))
    taskDelay (1);

  /* Only one thread at a time gets here.  Check ->done again, then
     go ahead and call func() if no one has done it yet.  */
  if (!guard->done)
    {
      func ();
      guard->done = 1;
    }

  guard->busy = 0;
  return 0;
}

/* Thread-specific data.

   We reserve a field in the TCB to point to a dynamically allocated
   array which is used to store TSD values.  A TSD key is simply an
   offset in this array.  The exact location of the TCB field is not
   known to this code nor to vxlib.c -- all access to it indirects
   through the routines __gthread_get_tsd_data and
   __gthread_set_tsd_data, which are provided by the VxWorks kernel.

   There is also a global array which records which keys are valid and
   which have destructors.

   A task delete hook is installed to execute key destructors.  The
   routines __gthread_enter_tsd_dtor_context and
   __gthread_leave_tsd_dtor_context, which are also provided by the
   kernel, ensure that it is safe to call free() on memory allocated
   by the task being deleted.  (This is a no-op on VxWorks 5, but
   a major undertaking on AE.)

   Since this interface is used to allocate only a small number of
   keys, the table size is small and static, which simplifies the
   code quite a bit.  Revisit this if and when it becomes necessary.  */

#define MAX_KEYS 4

/* This is the structure pointed to by the pointer returned
   by __gthread_get_tsd_data.  It must match the definition
   in the kernel file defining __gthread_get_tsd_data.  */
struct tsd_data
{
  void *values[MAX_KEYS];
};


/* kernel provided routines */
extern void *__gthread_get_tsd_data (WIND_TCB *tcb);
extern void __gthread_set_tsd_data (WIND_TCB *tcb, void *data);

extern void __gthread_enter_tsd_dtor_context (WIND_TCB *tcb);
extern void __gthread_leave_tsd_dtor_context (WIND_TCB *tcb);

typedef void (*fet_callback_t) (WIND_TCB *, unsigned int);
extern void __gthread_for_all_tasks (fet_callback_t fun, unsigned int number);

/* This is a global structure which records all of the active keys.  A
   key with NULL dtor is valid; a key with dtor == (void (*)(void))-1
   is valid, but has no dtor.  */

typedef void (*tsd_dtor) (void *);
static tsd_dtor tsd_dtors[MAX_KEYS];
#define KEY_INVALID ((tsd_dtor) 0)
#define KEY_NO_DTOR ((tsd_dtor) -1)

static __gthread_once_t tsd_init_guard = __GTHREAD_ONCE_INIT;
static __gthread_mutex_t tsd_lock;

/* Internal routines.  */

/* The task TCB has just been deleted.  Call the destructor
   function for each TSD key that has both a destructor and
   a non-NULL specific value in this thread.  */

static void
tsd_delete_hook (WIND_TCB *tcb)
{
  struct tsd_data *data = __gthread_get_tsd_data (tcb);
  __gthread_key_t key;

  if (data)
    {
      __gthread_enter_tsd_dtor_context (tcb);
      for (key = 0; key < MAX_KEYS; key++)
	{
	  if (tsd_dtors[key] == KEY_INVALID || tsd_dtors[key] == KEY_NO_DTOR)
	    continue;
	  if (data->values[key])
	    tsd_dtors[key] (data->values[key]);
	}
      free (data);
      __gthread_set_tsd_data (tcb, 0);
      __gthread_leave_tsd_dtor_context (tcb);
    }
} 

/* Initialize global data used by the TSD system.  */
static void
tsd_init (void)
{
  taskDeleteHookAdd ((FUNCPTR)tsd_delete_hook);
  __GTHREAD_MUTEX_INIT_FUNCTION (&tsd_lock);
}

/* External interface */

/* Store in KEYP a value which can be passed to __gthread_setspecific/
   __gthread_getspecific to store and retrive a value which is
   specific to each calling thread.  If DTOR is not NULL, it will be
   called when a thread terminates with a non-NULL specific value for
   this key, with the value as its sole argument.  */

int
__gthread_key_create (__gthread_key_t *keyp, tsd_dtor dtor)
{
  __gthread_key_t key;

  __gthread_once (&tsd_init_guard, tsd_init);

  if (__gthread_mutex_lock (&tsd_lock) == ERROR)
    return errno;

  for (key = 0; key < MAX_KEYS; key++)
    if (tsd_dtors[key] == KEY_INVALID)
      goto found_slot;

  /* no room */
  __gthread_mutex_unlock (&tsd_lock);
  return EAGAIN;

 found_slot:
  tsd_dtors[key] = dtor ? dtor : KEY_NO_DTOR;
  *keyp = key;
  __gthread_mutex_unlock (&tsd_lock);
  return 0;
}

/* Subroutine of __gthread_key_delete.  Clear the thread-specific
   value associated with key INDEX in TCB.  */

static void
key_delete_iter (WIND_TCB *tcb, unsigned int index)
{
  struct tsd_data *data = __gthread_get_tsd_data (tcb);
  if (data)
    data->values[index] = 0;
}

/* Invalidate KEY; it can no longer be used as an argument to
   setspecific/getspecific.  Note that this does NOT call destructor
   functions for any live values for this key.  */
int
__gthread_key_delete (__gthread_key_t key)
{
  if (key >= MAX_KEYS)
    return EINVAL;

  __gthread_once (&tsd_init_guard, tsd_init);

  if (__gthread_mutex_lock (&tsd_lock) == ERROR)
    return errno;

  if (tsd_dtors[key] == KEY_INVALID)
    {
      __gthread_mutex_unlock (&tsd_lock);
      return EINVAL;
    }

  __gthread_for_all_tasks (key_delete_iter, key);

  tsd_dtors[key] = KEY_INVALID;

  __gthread_mutex_unlock (&tsd_lock);
  return 0;
}

/* Retrieve the thread-specific value for KEY.  If it has never been
   set in this thread, or KEY is invalid, returns NULL.

   It does not matter if this function races with key_create or
   key_delete; the worst that can happen is you get a value other than
   the one that a serialized implementation would have provided.  */

void *
__gthread_getspecific (__gthread_key_t key)
{
  struct tsd_data *data;

  if (key >= MAX_KEYS || tsd_dtors[key] == KEY_INVALID)
    return 0;

  data = __gthread_get_tsd_data (taskTcb (taskIdSelf ()));

  if (data)
    return data->values[key];
  else
    return 0;
}

/* Set the thread-specific value for KEY.  If KEY is invalid, or
   memory allocation fails, returns -1, otherwise 0.

   It does matter if this function races with key_create/key_delete,
   so we have to take the lock, which should be okay since this is
   called much less often than getspecific (usually once per key
   per thread).  */

int
__gthread_setspecific (__gthread_key_t key, void *value)
{
  struct tsd_data *data;
  WIND_TCB *tcb;

  if (key >= MAX_KEYS)
    return EINVAL;

  __gthread_once (&tsd_init_guard, tsd_init);

  if (__gthread_mutex_lock (&tsd_lock) == ERROR)
    return errno;

  if (tsd_dtors[key] == KEY_INVALID)
    {
      __gthread_mutex_unlock (&tsd_lock);
      return EINVAL;
    }

  tcb = taskTcb (taskIdSelf ());
  data = __gthread_get_tsd_data (tcb);
  if (!data)
    {
      data = calloc (MAX_KEYS, sizeof (void *));
      if (!data)
	{
	  __gthread_mutex_unlock (&tsd_lock);
	  return ENOMEM;
	}
      __gthread_set_tsd_data (tcb, data);
    }

  data->values[key] = value;

  __gthread_mutex_unlock (&tsd_lock);
  return 0;
}



