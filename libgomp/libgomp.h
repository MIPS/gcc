/* Copyright (C) 2005, 2007, 2008 Free Software Foundation, Inc.
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

/* This file contains data types and function declarations that are not
   part of the official OpenMP user interface.  There are declarations
   in here that are part of the GNU OpenMP ABI, in that the compiler is
   required to know about them and use them.

   The convention is that the all caps prefix "GOMP" is used group items
   that are part of the external ABI, and the lower case prefix "gomp"
   is used group items that are completely private to the library.  */

#ifndef LIBGOMP_H 
#define LIBGOMP_H 1

#include "config.h"
#include "gstdint.h"

#include <pthread.h>
#include <stdbool.h>

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility push(hidden)
#endif

#include "sem.h"
#include "mutex.h"
#include "bar.h"


/* This structure contains the data to control one work-sharing construct,
   either a LOOP (FOR/DO) or a SECTIONS.  */

enum gomp_schedule_type
{
  GFS_RUNTIME,
  GFS_STATIC,
  GFS_DYNAMIC,
  GFS_GUIDED,
  GFS_AUTO
};

struct gomp_work_share
{
  /* This member records the SCHEDULE clause to be used for this construct.
     The user specification of "runtime" will already have been resolved.
     If this is a SECTIONS construct, this value will always be DYNAMIC.  */
  enum gomp_schedule_type sched;

  int mode;

  /* This is the chunk_size argument to the SCHEDULE clause.  */
  long chunk_size;

  /* This is the iteration end point.  If this is a SECTIONS construct, 
     this is the number of contained sections.  */
  long end;

  /* This is the iteration step.  If this is a SECTIONS construct, this
     is always 1.  */
  long incr;

  /* The above fields are written once during gomp_loop_init.  Make sure
     the following fields are in a different cache line.  */

  /* This lock protects the update of the following members.  */
  gomp_mutex_t lock __attribute__((aligned (64)));

  union {
    /* This is the next iteration value to be allocated.  In the case of
       GFS_STATIC loops, this the iteration start point and never changes.  */
    long next;

    /* This is the returned data structure for SINGLE COPYPRIVATE.  */
    void *copyprivate;
  };

  /* This is the count of the number of threads that have exited the work
     share construct.  If the construct was marked nowait, they have moved on
     to other work; otherwise they're blocked on a barrier.  The last member
     of the team to exit the work share construct must deallocate it.  */
  unsigned threads_completed;

  /* This is the index into the circular queue ordered_team_ids of the 
     current thread that's allowed into the ordered reason.  */
  unsigned ordered_cur;

  /* This is the number of threads that have registered themselves in
     the circular queue ordered_team_ids.  */
  unsigned ordered_num_used;

  /* This is the team_id of the currently acknoledged owner of the ordered
     section, or -1u if the ordered section has not been acknowledged by
     any thread.  This is distinguished from the thread that is *allowed*
     to take the section next.  */
  unsigned ordered_owner;

  /* This is a circular queue that details which threads will be allowed
     into the ordered region and in which order.  When a thread allocates
     iterations on which it is going to work, it also registers itself at
     the end of the array.  When a thread reaches the ordered region, it
     checks to see if it is the one at the head of the queue.  If not, it
     blocks on its RELEASE semaphore.  */
  unsigned ordered_team_ids[];
};

/* This structure contains all of the thread-local data associated with 
   a thread team.  This is the data that must be saved when a thread
   encounters a nested PARALLEL construct.  */

struct gomp_team_state
{
  /* This is the team of which the thread is currently a member.  */
  struct gomp_team *team;

  /* This is the work share construct which this thread is currently
     processing.  Recall that with NOWAIT, not all threads may be 
     processing the same construct.  This value is NULL when there
     is no construct being processed.  */
  struct gomp_work_share *work_share;

  /* This is the ID of this thread within the team.  This value is
     guaranteed to be between 0 and N-1, where N is the number of
     threads in the team.  */
  unsigned team_id;

  /* Nesting level.  */
  unsigned level;

  /* Active nesting level.  Only active parallel regions are counted.  */
  unsigned active_level;

  /* The work share "generation" is a number that increases by one for
     each work share construct encountered in the dynamic flow of the
     program.  It is used to find the control data for the work share
     when encountering it for the first time.  This particular number
     reflects the generation of the work_share member of this struct.  */
  unsigned work_share_generation;

  /* For GFS_RUNTIME loops that resolved to GFS_STATIC, this is the
     trip number through the loop.  So first time a particular loop
     is encountered this number is 0, the second time through the loop
     is 1, etc.  This is unused when the compiler knows in advance that
     the loop is statically scheduled.  */
  unsigned long static_trip;
};

/* These are the OpenMP 3.0 Internal Control Variables described in
   section 2.3.1.  Those described as having one copy per task are
   stored within the structure; those described as having one copy
   for the whole program are (naturally) global variables.  */

struct gomp_task_icv
{
  unsigned long nthreads_var;
  enum gomp_schedule_type run_sched_var;
  int run_sched_modifier;
  bool dyn_var;
  bool nest_var;
};

extern struct gomp_task_icv gomp_global_icv;
extern unsigned long gomp_thread_limit_var;
extern unsigned long gomp_remaining_threads_count;
#ifndef HAVE_SYNC_BUILTINS
extern gomp_mutex_t gomp_remaining_threads_lock;
#endif
extern unsigned long gomp_max_active_levels_var;
extern bool gomp_active_wait_policy;
extern unsigned long long gomp_spin_count_var;

/* This structure describes a "task" to be run by a thread.  At present
   we implement only synchronous tasks, i.e. no tasks are deferred or
   untied.  As such, all we need is the state of the ICVs.  */

struct gomp_task
{
  struct gomp_task *prev;
  struct gomp_task_icv icv;
};

/* This structure describes a "team" of threads.  These are the threads
   that are spawned by a PARALLEL constructs, as well as the work sharing
   constructs that the team encounters.  */

struct gomp_team
{
  /* This lock protects access to the following work shares data structures.  */
  gomp_mutex_t work_share_lock;

  /* This is a dynamically sized array containing pointers to the control
     structs for all "live" work share constructs.  Here "live" means that
     the construct has been encountered by at least one thread, and not
     completed by all threads.  */
  struct gomp_work_share **work_shares;

  /* The work_shares array is indexed by "generation & generation_mask".
     The mask will be 2**N - 1, where 2**N is the size of the array.  */
  unsigned generation_mask;

  /* These two values define the bounds of the elements of the work_shares
     array that are currently in use.  */
  unsigned oldest_live_gen;
  unsigned num_live_gen;

  /* This is the number of threads in the current team.  */
  unsigned nthreads;

  /* This is the saved team state that applied to a master thread before
     the current thread was created.  */
  struct gomp_team_state prev_ts;

  /* This semaphore should be used by the master thread instead of its
     "native" semaphore in the thread structure.  Required for nested
     parallels, as the master is a member of two teams.  */
  gomp_sem_t master_release;

  /* This points to an array with pointers to the release semaphore
     of the threads in the team.  */
  gomp_sem_t **ordered_release;

  struct gomp_work_share *init_work_shares[4];

  /* This barrier is used for most synchronization of the team.  */
  gomp_barrier_t barrier;

  /* This array contains structures for implicit tasks.  */
  struct gomp_task implicit_task[];
};

/* This structure contains all data that is private to libgomp and is
   allocated per thread.  */

struct gomp_thread
{
  /* This is the function that the thread should run upon launch.  */
  void (*fn) (void *data);
  void *data;

  /* This is the current team state for this thread.  The ts.team member
     is NULL only if the thread is idle.  */
  struct gomp_team_state ts;

  /* This is the task that the thread is currently executing.  */
  struct gomp_task *task;

  /* This semaphore is used for ordered loops.  */
  gomp_sem_t release;
};

/* ... and here is that TLS data.  */

#ifdef HAVE_TLS
extern __thread struct gomp_thread gomp_tls_data;
static inline struct gomp_thread *gomp_thread (void)
{
  return &gomp_tls_data;
}
#else
extern pthread_key_t gomp_tls_key;
static inline struct gomp_thread *gomp_thread (void)
{
  return pthread_getspecific (gomp_tls_key);
}
#endif

/* Here's how to access the current copy of the ICVs.  */

static inline struct gomp_task_icv *gomp_icv(void)
{
  struct gomp_task *task = gomp_thread()->task;
  if (task)
    return &task->icv;
  else
    return &gomp_global_icv;
}

/* The attributes to be used during thread creation.  */
extern pthread_attr_t gomp_thread_attr;

/* Other variables.  */

extern unsigned short *gomp_cpu_affinity;
extern size_t gomp_cpu_affinity_len;

/* Function prototypes.  */

/* affinity.c */

extern void gomp_init_affinity (void);
extern void gomp_init_thread_affinity (pthread_attr_t *);

/* alloc.c */

extern void *gomp_malloc (size_t) __attribute__((malloc));
extern void *gomp_malloc_cleared (size_t) __attribute__((malloc));
extern void *gomp_realloc (void *, size_t);

/* Avoid conflicting prototypes of alloca() in system headers by using
   GCC's builtin alloca().  */
#define gomp_alloca(x)  __builtin_alloca(x)

/* error.c */

extern void gomp_error (const char *, ...)
	__attribute__((format (printf, 1, 2)));
extern void gomp_fatal (const char *, ...)
	__attribute__((noreturn, format (printf, 1, 2)));

/* iter.c */

extern int gomp_iter_static_next (long *, long *);
extern bool gomp_iter_dynamic_next_locked (long *, long *);
extern bool gomp_iter_guided_next_locked (long *, long *);

#ifdef HAVE_SYNC_BUILTINS
extern bool gomp_iter_dynamic_next (long *, long *);
extern bool gomp_iter_guided_next (long *, long *);
#endif

/* ordered.c */

extern void gomp_ordered_first (void);
extern void gomp_ordered_last (void);
extern void gomp_ordered_next (void);
extern void gomp_ordered_static_init (void);
extern void gomp_ordered_static_next (void);
extern void gomp_ordered_sync (void);

/* parallel.c */

extern unsigned gomp_resolve_num_threads (unsigned, unsigned);

/* proc.c (in config/) */

extern void gomp_init_num_threads (void);
extern unsigned gomp_dynamic_max_threads (void);

/* task.c */

extern void gomp_init_task (struct gomp_task *, struct gomp_task *,
			    struct gomp_task_icv *);
extern void gomp_end_task (void);

/* team.c */

extern void gomp_team_start (void (*) (void *), void *, unsigned,
			     struct gomp_work_share *);
extern void gomp_team_end (void);

/* work.c */

extern struct gomp_work_share * gomp_new_work_share (bool, unsigned);
extern bool gomp_work_share_start (bool);
extern void gomp_work_share_end (void);
extern void gomp_work_share_end_nowait (void);

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility pop
#endif

/* Now that we're back to default visibility, include the globals.  */
#include "libgomp_g.h"

/* Include omp.h by parts.  */
#include "omp-lock.h"
#define _LIBGOMP_OMP_LOCK_DEFINED 1
#include "omp.h.in"

#if !defined (HAVE_ATTRIBUTE_VISIBILITY) \
    || !defined (HAVE_ATTRIBUTE_ALIAS) \
    || !defined (PIC)
# undef LIBGOMP_GNU_SYMBOL_VERSIONING
#endif

#ifdef LIBGOMP_GNU_SYMBOL_VERSIONING
extern void gomp_init_lock_30 (omp_lock_t *) __GOMP_NOTHROW;
extern void gomp_destroy_lock_30 (omp_lock_t *) __GOMP_NOTHROW;
extern void gomp_set_lock_30 (omp_lock_t *) __GOMP_NOTHROW;
extern void gomp_unset_lock_30 (omp_lock_t *) __GOMP_NOTHROW;
extern int gomp_test_lock_30 (omp_lock_t *) __GOMP_NOTHROW;
extern void gomp_init_nest_lock_30 (omp_nest_lock_t *) __GOMP_NOTHROW;
extern void gomp_destroy_nest_lock_30 (omp_nest_lock_t *) __GOMP_NOTHROW;
extern void gomp_set_nest_lock_30 (omp_nest_lock_t *) __GOMP_NOTHROW;
extern void gomp_unset_nest_lock_30 (omp_nest_lock_t *) __GOMP_NOTHROW;
extern int gomp_test_nest_lock_30 (omp_nest_lock_t *) __GOMP_NOTHROW;

extern void gomp_init_lock_25 (omp_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_destroy_lock_25 (omp_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_set_lock_25 (omp_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_unset_lock_25 (omp_lock_25_t *) __GOMP_NOTHROW;
extern int gomp_test_lock_25 (omp_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_init_nest_lock_25 (omp_nest_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_destroy_nest_lock_25 (omp_nest_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_set_nest_lock_25 (omp_nest_lock_25_t *) __GOMP_NOTHROW;
extern void gomp_unset_nest_lock_25 (omp_nest_lock_25_t *) __GOMP_NOTHROW;
extern int gomp_test_nest_lock_25 (omp_nest_lock_25_t *) __GOMP_NOTHROW;

# define strong_alias(fn, al) \
  extern __typeof (fn) al __attribute__ ((alias (#fn)));
# define omp_lock_symver(fn) \
  __asm (".symver g" #fn "_30, " #fn "@@OMP_3.0"); \
  __asm (".symver g" #fn "_25, " #fn "@OMP_1.0");
#else
# define gomp_init_lock_30 omp_init_lock
# define gomp_destroy_lock_30 omp_destroy_lock
# define gomp_set_lock_30 omp_set_lock
# define gomp_unset_lock_30 omp_unset_lock
# define gomp_test_lock_30 omp_test_lock
# define gomp_init_nest_lock_30 omp_init_nest_lock
# define gomp_destroy_nest_lock_30 omp_destroy_nest_lock
# define gomp_set_nest_lock_30 omp_set_nest_lock
# define gomp_unset_nest_lock_30 omp_unset_nest_lock
# define gomp_test_nest_lock_30 omp_test_nest_lock
#endif

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# define attribute_hidden __attribute__ ((visibility ("hidden")))
#else
# define attribute_hidden
#endif

#ifdef HAVE_ATTRIBUTE_ALIAS
# define ialias(fn) \
  extern __typeof (fn) gomp_ialias_##fn \
    __attribute__ ((alias (#fn))) attribute_hidden;
#else
# define ialias(fn)
#endif

#endif /* LIBGOMP_H */
