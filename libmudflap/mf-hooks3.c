/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

This file is part of GCC.
XXX: libgcc license?
*/

#include "config.h"

/* These attempt to coax various unix flavours to declare all our
   needed tidbits in the system headers.  */
#define _POSIX_SOURCE
#define _GNU_SOURCE 
#define _XOPEN_SOURCE
#define _BSD_TYPES
#define __EXTENSIONS__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#include "mf-runtime.h"
#include "mf-impl.h"

#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif


/* Multithreading support hooks.  */


#ifdef WRAP_pthreadstuff

#ifndef LIBMUDFLAPTH
#error "pthreadstuff is to be included only in libmudflapth"
#endif



/* Describes a thread (dead or alive). */
struct pthread_info
{
  short used_p;  /* Is this slot in use?  */

  pthread_t self; /* The thread id.  */
  short dead_p;  /* Has thread died?  */

  /* The user's thread entry point and argument.  */
  void * (*user_fn)(void *);
  void *user_arg;

  /* If libmudflapth allocated the stack, store its base/size.  */
  void *stack;
  size_t stack_size;

  int *thread_errno;
  enum __mf_state_enum state;
};


/* To avoid dynamic memory allocation, use static array.
   It is used as a simple hash table.  */
#define LIBMUDFLAPTH_THREADS_MAX 1024
static struct pthread_info __mf_pthread_info[LIBMUDFLAPTH_THREADS_MAX];
#define PTHREAD_HASH(p) ((unsigned) (p) % LIBMUDFLAPTH_THREADS_MAX)
/* XXX: locking required? */


/* Find and return the pthread_info struct for the running thread.  */
struct pthread_info* 
__mf_find_threadinfo ()
{
  pthread_t self = pthread_self ();
  unsigned hash = PTHREAD_HASH (self);
  unsigned i;
  struct pthread_info *result = NULL;

  for (i = hash; 
       (i+1) % LIBMUDFLAPTH_THREADS_MAX != hash; 
       i = (i+1) % LIBMUDFLAPTH_THREADS_MAX)
    {
      struct pthread_info* pi = & __mf_pthread_info [i];
      if (pi->used_p && pi->self == self) 
	{
	  result = pi;
	  break;
	}
    }

  /* XXX: This is too slow for the main thread.  It will not show up
     in the table since it was not created with pthread_create(), but
     will have to search the entire table.  */

  return result;
}


struct pthread_info* 
__mf_create_threadinfo ()
{
  pthread_t self = pthread_self ();
  unsigned hash = PTHREAD_HASH (self);
  unsigned i;
  struct pthread_info *result = NULL;

  for (i = hash; 
       (i+1) % LIBMUDFLAPTH_THREADS_MAX != hash; 
       i = (i+1) % LIBMUDFLAPTH_THREADS_MAX)
    {
      struct pthread_info* pi = & __mf_pthread_info [i];
      if (! pi->used_p)
	{
	  pi->used_p = 1;
	  pi->self = self;
	  result = pi;
	  break;
	}
    }

  return result;
}



enum __mf_state_enum *
__mf_state_perthread ()
{
  static enum __mf_state_enum __mf_state_global = active;
  struct pthread_info *pi = __mf_find_threadinfo ();

  if (pi == NULL)
    return & __mf_state_global;
  else
    return & pi->state;
}


static void 
__mf_pthread_cleanup (void *arg)
{
  struct pthread_info *pi = arg;

  /* XXX: This unregistration is not safe on platforms where distinct
     threads share errno (or at least its virtual address).  */
  if (__mf_opts.heur_std_data)
    __mf_unregister (pi->thread_errno, sizeof (int));
  pi->dead_p = 1;

  /* Some subsequent pthread_create will garbage_collect our stack.  */
}



static void *
__mf_pthread_spawner (void *arg)
{
  struct pthread_info *pi = arg;
  void *result = NULL;

  pi->self = pthread_self ();

  if (__mf_opts.heur_std_data)
    {
      pi->thread_errno = & errno;
      __mf_register (pi->thread_errno, sizeof (int), 
		     __MF_TYPE_GUESS, "errno area (thread)");
      /* NB: we could use __MF_TYPE_STATIC above, but we guess that
	 the thread errno is coming out of some dynamically allocated
	 pool that we already know of as __MF_TYPE_HEAP. */
    }

  /* We considered using pthread_key_t objects instead of these
     cleanup stacks, but they were less cooperative with the
     interposed malloc hooks in libmudflap.  */
  pthread_cleanup_push (& __mf_pthread_cleanup, arg);

  /* Call user thread */
  result = pi->user_fn (pi->user_arg);

  pthread_cleanup_pop (1 /* execute */);

  /* NB: there is a slight race here.  The pthread_info field will now
     say this thread is dead, but it may still be running .. right
     here.  We try to check for this possibility using the
     pthread_kill test below. */

  return result;
}


#if PIC
/* A special bootstrap variant. */
int
__mf_0fn_pthread_create (pthread_t *thr, pthread_attr_t *attr, 
			 void * (*start) (void *), void *arg)
{
  return -1;
}
#endif


#undef pthread_create
WRAPPER(int, pthread_create, pthread_t *thr, const pthread_attr_t *attr, 
	 void * (*start) (void *), void *arg)
{
  DECLARE(void, free, void *p);
  DECLARE(void *, malloc, size_t c);
  DECLARE(int, pthread_create, pthread_t *thr, const pthread_attr_t *attr, 
	  void * (*start) (void *), void *arg);
  int result;
  struct pthread_info *pi;
  pthread_attr_t override_attr;
  void *override_stack;
  size_t override_stacksize;
  unsigned i;

  TRACE ("pthread_create\n");

  /* Garbage collect dead thread stacks.  */
  for (i = 0; i < LIBMUDFLAPTH_THREADS_MAX; i++)
    {
      pi = & __mf_pthread_info [i];
      if (pi->used_p
	  && pi->dead_p 
	  && !pthread_kill (pi->self, 0)) /* Really dead?  XXX: safe?  */ 
	{
	  if (pi->stack != NULL)
	    CALL_REAL (free, pi->stack);

	  pi->stack = NULL;
	  pi->stack_size = 0;
	  pi->used_p = 0;
	}
    }

  /* Find a slot in __mf_pthread_info to track this thread.  */
  pi = __mf_create_threadinfo ();
  if (pi == NULL) /* no slots free - simulated out-of-memory.  */
    {
      errno = EAGAIN;
      return -1;
    }

  /* Let's allocate a stack for this thread, if one is not already
     supplied by the caller.  We don't want to let e.g. the
     linuxthreads manager thread do this allocation.  */
  if (attr != NULL)
    override_attr = *attr;
  else
    pthread_attr_init (& override_attr);

  /* Get supplied attributes, if any.  */
  /* XXX: consider using POSIX2K attr_getstack() */
  if (pthread_attr_getstackaddr (& override_attr, & override_stack) != 0 ||
      pthread_attr_getstacksize (& override_attr, & override_stacksize) != 0)
    {
      override_stack = NULL;
      override_stacksize = 0;
    }

  /* Do we need to allocate the new thread's stack?  */
  if (override_stack == NULL)
    {
      uintptr_t alignment = 256; /* power of two */

      /* Perturb the initial stack addresses slightly, to encourage
	 threads to have nonconflicting entries in the lookup cache
	 for their tracked stack objects.  */
      static unsigned perturb = 0;
      const unsigned perturb_delta = 0; /* 32; */
      const unsigned perturb_count = 16;
      perturb += perturb_delta;
      if (perturb > perturb_delta*perturb_count) perturb = 0;

      /* Use glibc x86 defaults */
      if (override_stacksize < alignment)
/* Should have been defined in <limits.h> */
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 65536
#endif
	override_stacksize = max (PTHREAD_STACK_MIN, 2 * 1024 * 1024);

      override_stack = CALL_REAL (malloc, override_stacksize);
      if (override_stack == NULL)
	{
	  errno = EAGAIN;
	  pi->used_p = 0;
	  return -1;
	}

      pi->stack = override_stack;
      pi->stack_size = override_stacksize;

      /* The stackaddr pthreads attribute is a candidate stack pointer.
	 It must point near the top or the bottom of this buffer, depending
	 on whether stack grows downward or upward, and suitably aligned.
	 On the x86, it grows down, so we set stackaddr near the top.  */
      override_stack = (void *)
	(((uintptr_t) override_stack + override_stacksize - alignment - perturb)
	 & (~(uintptr_t)(alignment-1)));
      
      /* XXX: consider using POSIX2K attr_setstack() */
      if (pthread_attr_setstackaddr (& override_attr, override_stack) != 0 ||
	  pthread_attr_setstacksize (& override_attr, 
				     override_stacksize - alignment - perturb) != 0)
	{
	  /* This should not happen.  */
	  CALL_REAL (free, pi->stack);
	  pi->stack = NULL;
	  errno = EAGAIN;
	  pi->used_p = 0;
	  return -1;
	}

  }

  /* Fill in remaining fields.  */
  pi->user_fn = start;
  pi->user_arg = arg;
  pi->dead_p = 0;
  pi->state = active;

  /* Actually create the thread.  */
  result = CALL_REAL (pthread_create, thr, & override_attr,
		      & __mf_pthread_spawner, (void *) pi);
  
  /* May need to clean up if we created a pthread_attr_t of our own.  */
  if (attr == NULL)
    pthread_attr_destroy (& override_attr); /* NB: this shouldn't deallocate stack */

  return result;
}


#endif /* pthreadstuff */
