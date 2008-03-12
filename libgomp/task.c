/* Copyright (C) 2007, 2008 Free Software Foundation, Inc.
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

/* This file handles the maintainence of tasks in response to task
   creation and termination.  */

#include "libgomp.h"
#include <stdlib.h>


/* Create a new task data structure.  */

struct gomp_task *
gomp_new_task (struct gomp_task *prev_task, struct gomp_task_icv *prev_icv)
{
  struct gomp_task *task = gomp_malloc (sizeof (struct gomp_task));
  task->prev = prev_task;
  task->icv = *prev_icv;

  return task;
}

/* Clean up and free a task, after completing it.  */

void
gomp_end_task (void)
{
  struct gomp_thread *thr = gomp_thread ();
  struct gomp_task *task = thr->task;

  thr->task = task->prev;
  free (task);
}

/* Called when encountering an explicit task directive.  If IF_CLAUSE is
   true, then we must not delay in executing the task.  If UNTIED is true,
   then the task may be executed by any member of the team.  */

void
GOMP_task (void (*fn) (void *), void *data,
	   bool if_clause __attribute__((unused)),
	   unsigned flags __attribute__((unused)))
{
  struct gomp_thread *thr = gomp_thread ();
  thr->task = gomp_new_task (thr->task, gomp_icv ());

  /* We only implement synchronous tasks at the moment, which means that
     we cannot defer or untie the task.  Which means we execute it now.  */
  fn (data);

  gomp_end_task ();
}

/* Called after a task has been initialized.  Only should be called if
   GOMP_task was called with GOMP_task_flag_explicit_start bit set,
   after all firstprivate etc. copying is done.  The copying will
   happen immediately, in the thread that created the task, afterwards
   it can be suspended and/or moved to another thread, even if not untied.  */

void
GOMP_task_start (void)
{
}

/* Called when encountering a taskwait directive.  */

void
GOMP_taskwait (void)
{
  /* Since we never deferred any tasks, there are none to wait for.  */
}
