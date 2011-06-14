/* Copyright (C) 2010 Free Software Foundation, Inc.
   Contributed by Antoniu Pop <antoniu.pop@gmail.com>.

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

/* This implements the stream communication layer for libGOMP.  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "wait.h"
#include "sem.h"
#include "mutex.h"
#include "libgomp.h"

#define AGGREGATION_FACTOR 32

//#define debug_log_init(S, V1, V2) printf (S, V1, V2); fflush (stdout)
#define debug_log_init(S, V1, V2)

//#define debug_log_init3(S, V1, V2, V3) printf (S, V1, V2, V3); fflush (stdout)
#define debug_log_init3(S, V1, V2, V3)

//#define debug_log(S, V1, V2) printf (S, V1, V2); fflush (stdout)
#define debug_log(S, V1, V2)

gomp_barrier_t gomp_stream_tasks_wait_until_connected_barrier;
gomp_barrier_t gomp_stream_tasks_exit_barrier;
unsigned gomp_stream_tasks_count;

/* This structure is used to communicate across pthread_create.  */

struct gomp_stream_thread_start_data
{
  void (*fn) (void *);
  void *fn_data;

  int id;
};

/* Data structures creation and pipeline initialization.  */

/* Allocate and initialize a GOMP_STREAM for data elements of size
   ELEMENT_SIZE using a circular buffer of STREAM_BUFFER_SIZE such
   elements.  Returns a pointer to the newly allocated stream.  The
   user may provide a pointer to pre-allocated memory to be used as
   BUFFER for the stream.  */

void *
GOMP_stream_create_stream (size_t element_size, size_t buffer_size)
{
  gomp_stream_p stream = (gomp_stream_p) gomp_malloc (sizeof (gomp_stream_t));
  debug_log_init ("GOMP_stream_create_stream %zu %zu\n", element_size, buffer_size);

  if (buffer_size < 2)
    gomp_fatal ("GOMP_stream: insufficient stream buffer size.");

  /* Initialize and allocate the data buffer.  We force the
     buffer_size to be a power of 2 for efficient modulo computation
     of the indices in the circular buffer.  */

  /* To avoid excessive multiplication operations, we convert the
     accounting from elements to bytes.  */
  buffer_size *= element_size;
  stream->buffer_size = 1;
  while(stream->buffer_size < buffer_size)
    stream->buffer_size <<= 1;
  stream->buffer_mask = stream->buffer_size - 1;
  stream->buffer =
    (void *) gomp_malloc (stream->buffer_size * 2);

  stream->expected_ready_p = false;
  stream->connected_p = false;
  stream->eos_p = false;
  stream->pre_shift = 0;

  /* Initialize the view_handles.  */
  stream->read_views.current_min = stream->buffer_size;
  stream->read_views.current_max = 0;
  stream->read_views.view_list.views = NULL;
  stream->read_views.view_list.nr_views = 0;
  stream->read_views.view_list.size = 0;
  stream->read_views.nr_expected_views = 0;
  stream->read_views.nr_registered_views = 0;
  stream->read_views.nr_unregistered_views = 0;
  gomp_mutex_init (&stream->read_views.view_list.connect_view_mutex);

  stream->write_views.current_min = 0;
  stream->write_views.current_max = stream->buffer_size;
  stream->write_views.view_list.views = NULL;
  stream->write_views.view_list.nr_views = 0;
  stream->write_views.view_list.size = 0;
  stream->write_views.nr_expected_views = 0;
  stream->write_views.nr_registered_views = 0;
  stream->write_views.nr_unregistered_views = 0;
  gomp_mutex_init (&stream->write_views.view_list.connect_view_mutex);

#ifndef HAVE_SYNC_BUILTINS
  gomp_mutex_init (&stream->stream_mutex);
#endif

  return stream;
}

/* Allocate and initialize a generic GOMP_STREAM_VIEW that can be
   connected to any stream to give either read or write access
   depending on its TYPE.  Returns a pointer to the newly allocated
   view.  This view accesses VIEW_SIZE bytes in the stream and
   commits/releases BURST_SIZE bytes per activation.  */

static inline void *
gomp_stream_create_view (int type, size_t view_size, size_t burst_size)
{
  gomp_stream_view_p view =
    (gomp_stream_view_p) gomp_malloc (sizeof(gomp_stream_view_t));

  view->lower_index = 0;
  view->upper_index = 0;
  view->stream = NULL;
  view->end_p = false;
  view->type = type;
  view->local_min_value = 0;
  view->view_size = view_size;
  view->burst_size = burst_size;
  view->pxxk_size = view_size - burst_size;

  return view;
}

/* Wrapper for creating a READ view .  */

void *
GOMP_stream_create_read_view (size_t view_size, size_t burst_size)
{
  debug_log_init ("GOMP_stream_create_read_view %zu %zu\n", view_size, burst_size);
  return gomp_stream_create_view (READ_VIEW, view_size, burst_size);
}

/* Wrapper for creating a WRITE view.  */

void *
GOMP_stream_create_write_view (size_t view_size, size_t burst_size)
{
  debug_log_init ("GOMP_stream_create_write_view %zu %zu\n", view_size, burst_size);
  return gomp_stream_create_view (WRITE_VIEW, view_size, burst_size);
}

/* Allocate and initialize a GOMP_STREAM_TASK data structure.  */

void *
GOMP_stream_create_task ()
{
  gomp_stream_task_p task =
    (gomp_stream_task_p) gomp_malloc (sizeof(gomp_stream_task_t));
  debug_log_init3 ("GOMP_stream_create_task %d %d \t %15zu\n", 0, 0, (size_t) task);

  task->read_view_list.views = NULL;
  task->read_view_list.nr_views = 0;
  task->read_view_list.size = 0;
  gomp_mutex_init (&task->read_view_list.connect_view_mutex);

  task->write_view_list.views = NULL;
  task->write_view_list.nr_views = 0;
  task->write_view_list.size = 0;
  gomp_mutex_init (&task->write_view_list.connect_view_mutex);

  task->activation_counter = 0;
  task->termination_flag = false;

  task->first_unassigned_activation_counter = 0;
  task->num_instances = 0;

  __sync_synchronize ();

  return task;
}

volatile void *
GOMP_stream_get_task_activation_counter (void *t)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;

  return &(task->activation_counter);
}

void
GOMP_stream_set_task_termination_flag (void *t)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;

  task->termination_flag = true;
}

void
GOMP_stream_task_add_instance (void *t)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;

  __sync_fetch_and_add (&task->num_instances, 1);
  __sync_synchronize ();
}

/* Declare additional READ_VIEWS and WRITE_VIEWS expected views on
   stream S.  When possible, the thread that creates the streaming
   tasks should declare, for each stream, the number of read/write
   views that will connect to a stream before the streaming tasks are
   started.  If this function is called on a stream, there will be no
   further checks for the number of tasks partaking in the
   initialization synchronization.  */

void
GOMP_stream_add_expected_views (void *s, int read_views, int write_views,
				int final)
{
  gomp_stream_p stream = (gomp_stream_p) s;
  debug_log_init ("GOMP_stream_add_expected_views %d %d\n", read_views, write_views);

  if (stream->expected_ready_p)
    gomp_fatal
      ("GOMP_stream: attempting to modify a final number of expected views.");
  stream->expected_ready_p = final;

#ifdef HAVE_SYNC_BUILTINS
  __sync_fetch_and_add (&stream->read_views.nr_expected_views, read_views);
  __sync_fetch_and_add (&stream->write_views.nr_expected_views, write_views);
#else
  gomp_mutex_lock (&stream->stream_mutex);
  stream->read_views.nr_expected_views += read_views;
  stream->write_views.nr_expected_views += write_views;
  gomp_mutex_unlock (&stream->stream_mutex);
#endif
}

/* Add VIEW to the VIEW_LIST.  We actually use an array as this list
   is only modified in the initialization phase and we never remove
   any items from it.  */

static inline void
gomp_stream_add_view_to_list (gomp_stream_view_p view,
			      gomp_stream_view_list_p view_list)
{
  /* Allocate memory when needed.  */
  if (view_list->views == NULL || view_list->nr_views == view_list->size)
    {
      if (view_list->size == 0)
	view_list->size = 4;
      if (view_list->nr_views == view_list->size)
	view_list->size <<= 1;

      view_list->views = 
	(gomp_stream_view_p *) gomp_realloc (view_list->views,
					     view_list->size * sizeof (gomp_stream_view_p));
    }

  view_list->views[view_list->nr_views] = view;
  view_list->nr_views += 1;
}

/* Connect a VIEW to a STREAM and also to the TASK which will use it.
   This effectively builds the runtime task graph.  */

void
GOMP_stream_connect_view (void *t, void *s, void *v)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;
  gomp_stream_p stream = (gomp_stream_p) s;
  gomp_stream_view_p view = (gomp_stream_view_p) v;

  gomp_stream_view_handle_p vh = (view->type == READ_VIEW) ?
    &stream->read_views : &stream->write_views;
  gomp_stream_view_list_p stream_list = &vh->view_list;
  gomp_stream_view_list_p task_list = (view->type == READ_VIEW) ?
    &task->read_view_list : &task->write_view_list;

  view->stream = stream;

  /* A read view's lower index is shifted by the buffer_size as the
     stream is initially empty. This is equivalent to releasing the
     original buffer_size elements.  A write view will start with
     buffer_size free space.  */
  if (view->type == READ_VIEW)
    view->lower_index = stream->buffer_size;
  else
    view->local_min_value = stream->buffer_size;

  /* Register the view with the TASK to which it belongs.  This
     operation is local to the task, so there is no need to
     synchronize.  */
  gomp_mutex_lock (&task_list->connect_view_mutex);
  gomp_stream_add_view_to_list (view, task_list);
  gomp_mutex_unlock (&task_list->connect_view_mutex);

  /* Connect the view to the stream.  This must be done atomically as
     this data structure is shared with the other producer/consumer
     tasks.  */
  gomp_mutex_lock (&vh->view_list.connect_view_mutex);
  gomp_stream_add_view_to_list (view, stream_list);
  gomp_mutex_unlock (&vh->view_list.connect_view_mutex);
  __sync_fetch_and_add (&vh->nr_registered_views, 1);
}

/* Check whether all the expected views on STREAM have already
   connected.  */

static inline bool
gomp_stream_check_connected (gomp_stream_p stream)
{
  if (!stream->expected_ready_p)
    return false;

  if (stream->connected_p)
    return true;

  if ((stream->read_views.view_list.nr_views
       == stream->read_views.nr_expected_views)
      && (stream->write_views.view_list.nr_views
	  == stream->write_views.nr_expected_views))
    {
      stream->connected_p = true;
      return true;
    }
  return false;
}

/* Wait until all the streams to which TASK connects are ready and
   connected to all producer/consumer tasks.  */

void
GOMP_stream_wait_until_connected (void *t)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;
  bool done;
  int i;

  do
    {
      done = true;

      gomp_mutex_lock (&task->read_view_list.connect_view_mutex);
      for (i = 0; i < task->read_view_list.nr_views; ++i)
	if (!gomp_stream_check_connected (task->read_view_list.views[i]->stream))
	  done = false;
      gomp_mutex_unlock (&task->read_view_list.connect_view_mutex);

      gomp_mutex_lock (&task->write_view_list.connect_view_mutex);
      for (i = 0; i < task->write_view_list.nr_views; ++i)
	if (!gomp_stream_check_connected (task->write_view_list.views[i]->stream))
	  done = false;
      gomp_mutex_unlock (&task->write_view_list.connect_view_mutex);
    }
  while (!done);
  debug_log_init ("GOMP_stream_wait_until_connected %zu %zu\n", (size_t) task, (size_t) task);
}

/* Stream communication/synchronization.  */

/* Compute the minimum of the LOWER_INDEX fields of all views in the
   LIST of views.  This is used during the termination phase to give
   access to the readers up to the highest committed index.  This is
   only useful when producers, not too far apart in indices they
   committed, forget to commit up to the last index that should appear
   in the stream.  */

static inline unsigned long long
gomp_stream_compute_lower_max (gomp_stream_view_list_p list)
{
  unsigned long long local_max = 0;
  gomp_stream_view_p *views = list->views;
  int i;

  for (i = 0; i < list->nr_views; ++i)
    if (views[i]->lower_index > local_max)
      local_max = views[i]->lower_index;

  return local_max;
}

/* Compute the minimum of the LOWER_INDEX fields of all views in the
   LIST of views.  For a LIST of read views, this minimum represents
   the highest index released by all read views on a stream (i.e. the
   index of elements that all consumers have already discarded) and
   therefore the highest index a write view will be allowed to acquire
   for writing.  For a LIST of write views, this minimum represents
   the highest index all producers have committed and thus the highest
   index available for reading.  */

static inline unsigned long long
gomp_stream_compute_lower_min (gomp_stream_view_list_p list)
{
  unsigned long long local_min = GOMP_STREAM_MAX_INDEX;
  gomp_stream_view_p *views = list->views;
  int i;

  for (i = 0; i < list->nr_views; ++i)
    if (views[i]->lower_index < local_min)
      local_min = views[i]->lower_index;

  return local_min;
}

/* Compute the minimum of the UPPER_INDEX fields of all views in the
   LIST of views.  Similar to the above, but this is only a hint on
   the resources that another producer (resp. consumer) has already
   acquired.  If a producer (resp. consumer) has successfully acquired
   an index with GOMP_stream_stall (resp. GOMP_stream_update) for
   writing (resp. reading), then all other producers (resp. consumers)
   on the same stream can access up to the same index without further
   verification.  */

static inline unsigned long long
gomp_stream_compute_upper_min (gomp_stream_view_list_p list)
{
  unsigned long long local_min = GOMP_STREAM_MAX_INDEX;
  gomp_stream_view_p *views = list->views;
  int i;

  for (i = 0; i < list->nr_views; ++i)
    if (views[i]->upper_index < local_min)
      local_min = views[i]->upper_index;

  return local_min;
}

/* Wait until the producers (resp. consumers) on this stream have
   committed (resp. released) up to the INDEX position in the stream.
   When that hapens, the consumer (resp. producer) connected to the
   stream through VIEW is allowed to access the elements up to
   INDEX.  */

static inline void
gomp_stream_wait_release (gomp_stream_view_p view,
			  gomp_stream_view_handle_p vh,
			  const unsigned long long index)
{
  /* Test whether someone already got a hold of a bigger index
     yet.  */

  if (view->local_min_value < index)
    {
      while (vh->current_min < index && !view->stream->eos_p)
	{
	  unsigned long long local_min =
	    gomp_stream_compute_lower_min (&vh->view_list);

	  if (vh->current_min == local_min)
	    __asm volatile ("pause" : : : "memory");
	  else
	    vh->current_min = local_min;
	}
      view->local_min_value = vh->current_min;
    }
}

/* Request read access for the view V to the stream up to INDEX.  In
   case the producers have finished and there is not enough data, the
   returned value is the highest index to which the view is allowed to
   access the stream.  */

void *
GOMP_stream_update (void *v, const unsigned long long act_start,
		    const unsigned long long act_end)
{
  unsigned long long low_idx, up_idx;
  size_t low_idx_loc, up_idx_loc;
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  gomp_stream_p stream = view->stream;
  void *buffer_pointer;

  debug_log ("GOMP_stream_update [in] %llu %llu\n", act_start, act_end);

  /* This update requests access to the buffer in [low_idx,up_idx[.
     We will release up to low_idx-1 and acquire up to up_idx-1.  */
  low_idx = act_start * view->burst_size;
  up_idx = act_end * view->burst_size + view->pxxk_size - 1;

  if (up_idx - low_idx > stream->buffer_size)
    gomp_fatal ("GOMP_stream: update requested access to more than buffer_size data.");


  view->lower_index = low_idx + view->stream->buffer_size;
  view->upper_index = up_idx;

  /* In case another consumer has received permission to read up to a
     yet higher index, then there is no need to check for this one.  */
  if (up_idx > view->stream->read_views.current_max)
    {
      gomp_stream_wait_release (view, &view->stream->write_views, up_idx);
      view->stream->read_views.current_max = up_idx;
    }

  low_idx_loc = low_idx & stream->buffer_mask;
  up_idx_loc = up_idx & stream->buffer_mask;

  /* Once we know enough data is available for reading, we need to
     check whether the data between the lower and upper buonds is
     contiguous or if the buffer wrap-around occurs in the middle.  */
  if (low_idx_loc > up_idx_loc)
    {
      /* FIXME: does this require synchronization or is concurrent
	 overwriting acceptable as long as enough data has been copied
	 at the end?  */
      memcpy (stream->buffer + stream->buffer_size, stream->buffer,
	      up_idx_loc + 1);
      //printf ("Update copy: (%llu,%llu) %llu - %llu | %zu - %zu (size: %zu)\n ", act_start, act_end, low_idx, up_idx, low_idx_loc, up_idx_loc, up_idx_loc + 1);
    }

  /* We return a pointer to a contiguous array where this view is
     guaranteed access to all the requested data.  */
  buffer_pointer = stream->buffer + low_idx_loc;

  debug_log ("GOMP_stream_update [out] %llu %llu\n", act_start, act_end);

  return buffer_pointer;
}

/* Request write access for the view V to the stream up to INDEX.  */

void *
GOMP_stream_stall (void *v, const unsigned long long act_start,
		   const unsigned long long act_end)
{
  unsigned long long low_idx, up_idx;
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  gomp_stream_p stream = view->stream;
  void *buffer_pointer;

  debug_log ("GOMP_stream_stall [in] %llu %llu\n", act_start, act_end);

  /* This update requests access to the buffer in [low_idx,up_idx[.
     We will release up to low_idx-1 and acquire up to up_idx-1.  */
  low_idx = act_start * view->burst_size + stream->pre_shift;
  up_idx = act_end * view->burst_size + view->pxxk_size + stream->pre_shift - 1;

  if (up_idx - low_idx > stream->buffer_size)
    {
      fprintf (stderr, "Requesting data from low: %llu to up: %llu act [%llu,%llu] for burst:%zu size: %zu\n", low_idx, up_idx, act_start, act_end, view->burst_size, view->view_size);
      gomp_fatal ("GOMP_stream: stall requested access to more than buffer_size data.");
    }
  /* We do not need to worry about wrap-around copying as this
     "commit" only means that we do not want to write to those
     indices below low_idx.  */
  view->lower_index = low_idx;
  view->upper_index = up_idx;

  if (up_idx > stream->write_views.current_max)
    {
      gomp_stream_wait_release (view, &stream->read_views, up_idx);
      stream->write_views.current_max = up_idx;
    }

  buffer_pointer = stream->buffer + (low_idx & stream->buffer_mask);

  debug_log ("GOMP_stream_stall [out] %llu %llu\n", act_start, act_end);

  return buffer_pointer;
}

/* Relinquish read access for the view V to the stream up to
   INDEX.  */

void
GOMP_stream_release (void *v, const unsigned long long act_idx)
{
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  view->lower_index = act_idx * view->burst_size + view->stream->buffer_size - 1;
  debug_log ("GOMP_stream_release %llu %llu\n", act_idx, act_idx);
}

/* Relinquish write access for the view V to the stream up to
   INDEX.  */

void
GOMP_stream_commit (void *v, const unsigned long long act_idx)
{
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  gomp_stream_p stream = view->stream;
  unsigned long long up_idx = act_idx * view->burst_size + stream->pre_shift - 1;
  size_t low_idx_loc, up_idx_loc;

  low_idx_loc = view->lower_index & stream->buffer_mask;
  up_idx_loc = up_idx & stream->buffer_mask;

  /* Once we know enough data is available for reading, we need to
     check whether the data between the lower and upper buonds is
     contiguous or if the buffer wrap-around occurs in the middle.  */
  if (low_idx_loc > up_idx_loc)
    {
      /* FIXME: does this require synchronization or is concurrent
	 overwriting acceptable as long as enough data has been copied
	 at the end?  */
      memcpy (stream->buffer, stream->buffer + stream->buffer_size,
	      up_idx_loc + 1);

      //printf ("Commit copy: (%llu) %llu - %llu | %zu - %zu (size: %zu)\n ", act_idx, view->lower_index, up_idx, low_idx_loc, up_idx_loc, up_idx_loc + 1);
    }

  view->lower_index = up_idx;
  debug_log ("GOMP_stream_commit %llu %llu\n", act_idx, act_idx);
}

/* Finalization and destruction of the streaming data structures.  */

/* Disconnects VIEW from the stream to which it is connected and free
   the stream if it was the last task to disconnect.  */

static inline void
gomp_stream_unregister_view (gomp_stream_view_p view)
{
  gomp_stream_p stream = view->stream;
  gomp_stream_view_handle_p vh =
    (view->type == READ_VIEW) ? &stream->read_views : &stream->write_views;
  int unregistered_views;

  __sync_fetch_and_add (&(vh->nr_unregistered_views), 1);
  unregistered_views = __sync_add_and_fetch (&(stream->unregistered_views), 1);


  /* Make sure that when multiple views access a stream, the finished
     views do not hinder the others in the min computation.  */
  if (view->type == READ_VIEW)
    GOMP_stream_release (view, GOMP_STREAM_MAX_INDEX);
  /* The last producer exiting will set the eos_p flag and allow the
     consumers to read up to the highest committed index.  */
  else if (vh->nr_unregistered_views == vh->nr_registered_views)
    {
      stream->eos_p = true;
      vh->current_min = gomp_stream_compute_lower_max (&vh->view_list);
    }

  /* If all known views arre accounted for, this is the last one
     unregistering.  It frees the memory allocated for the stream as
     well as all the views on this stream.  */
  if (unregistered_views == (stream->read_views.nr_registered_views
			     + stream->write_views.nr_registered_views))
    {
      gomp_stream_view_list_p read_view_list = &stream->read_views.view_list;
      gomp_stream_view_list_p write_view_list = &stream->write_views.view_list;
      int i;

      for (i = 0; i < read_view_list->nr_views; ++i)
	free (read_view_list->views[i]);

      for (i = 0; i < write_view_list->nr_views; ++i)
	free (write_view_list->views[i]);

      free (stream->buffer);
      free (read_view_list->views);
      free (write_view_list->views);
      free (stream);
    }
}

/* Invoked before terminating a stream TASK, this disconnects all the
   views and for all streams for which it is the last one to
   disconnect from, it frees up all data structures.  */

void
GOMP_stream_task_exit (void *t)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;
  int num_read_views = task->read_view_list.nr_views;
  int num_write_views = task->write_view_list.nr_views;
  int i, res;
  debug_log_init ("GOMP_stream_task_exit %zu %zu\n", (size_t) t, (size_t) t);

  res = __sync_sub_and_fetch (&task->num_instances, 1);

  if (res == 0)
    {
      for (i = 0; i < num_read_views; ++i)
	gomp_stream_unregister_view (task->read_view_list.views[i]);

      for (i = 0; i < num_write_views; ++i)
	gomp_stream_unregister_view (task->write_view_list.views[i]);

      free (task->read_view_list.views);
      free (task->write_view_list.views);
      free (task);
    }
}

/* Get info on the amount of work/data available in the stream
   starting from INDEX and considering a constant burst of size BURST.
   This function does not wait, except in case there is no work yet,
   but the EOS flag has not yet been set.  The function only returns 0
   on termination.  */
unsigned long long
GOMP_stream_get_available_work (void *t, unsigned long long *start_idx)
{
  gomp_stream_task_p task = (gomp_stream_task_p) t;
  long result = 0;
  unsigned long long start;

  /* Atomically acquire a range, then wait until the range is either
     fully available or termination occurs.  */
  start = __sync_fetch_and_add (&task->first_unassigned_activation_counter,
				AGGREGATION_FACTOR);
  result = task->activation_counter - start;
  debug_log_init3 ("GOMP_stream_get_available_work [entry] %llu %llu \t %15zu\n", start, task->activation_counter, (size_t) task);

  if (result >= AGGREGATION_FACTOR)
    {
      *start_idx = start;
      return AGGREGATION_FACTOR;
    }

  while (result < AGGREGATION_FACTOR)
    {
      if (task->termination_flag)
	{
	  __sync_synchronize ();
	  result = task->activation_counter - start;
	  debug_log ("GOMP_stream_get_available_work [final] %llu %ld\n", start, result);
	  *start_idx = start;
	  if (result > AGGREGATION_FACTOR)
	    return AGGREGATION_FACTOR;
	  return (result > 0) ? result : 0;
	}
      result = task->activation_counter - start;
    }
  debug_log ("GOMP_stream_get_available_work %llu %ld\n", start, result);

  *start_idx = start;
  return AGGREGATION_FACTOR;
}


/* Initialize streaming in this region.  */

void
GOMP_stream_init ()
{
  /* Add self to ensure at least one member of the team barrier will
     be waiting for the streaming tasks.  */
  gomp_stream_tasks_count = 1;
  gomp_barrier_init (&gomp_stream_tasks_exit_barrier, gomp_stream_tasks_count);
}

/* Wait until all streaming threads complete.  */

void
GOMP_stream_exit ()
{
  gomp_barrier_wait (&gomp_stream_tasks_exit_barrier);
}

/* Request SIZE bytes for a PRE operator on stream S.  Return a
   pointer where data should be stored.  */
void *
GOMP_stream_pre (void *s, const unsigned long long size)
{
  gomp_stream_p stream = (gomp_stream_p) s;

  debug_log_init ("GOMP_stream_pre  %zu \t %llu\n", (size_t) s, size);

  stream->pre_shift = size;
  stream->write_views.current_min = size;

  return stream->buffer;
}


/* This function is a pthread_create entry point for streaming
   tasks.  */

static void *
gomp_stream_thread_start (void *xdata)
{
  struct gomp_stream_thread_start_data *data = xdata;
  void (*local_fn) (void *);
  void *local_data;

  local_fn = data->fn;
  local_data = data->fn_data;

  local_fn (local_data);

  gomp_barrier_wait_last (&gomp_stream_tasks_exit_barrier);
  debug_log_init ("** exiting task: %d (%u)\n", data->id, gomp_stream_tasks_count);

  return NULL;
}

/* Called for starting a streaming task.  These tasks do not partake
   in existing thread teams and are not subject to scheduling
   points.  */

void
GOMP_stream_task (void (*fn) (void *), void *data,
		  void (*cpyfn) (void *, void *),
		  long arg_size, long arg_align,
		  long num_instances, bool auto_replicable)
{
  pthread_attr_t thread_attr, *attr;
  pthread_t pt;
  int err, i, base_id;
  char *arg, *buf;

  base_id = __sync_fetch_and_add (&gomp_stream_tasks_count, num_instances);
  gomp_barrier_reinit (&gomp_stream_tasks_exit_barrier,
		       gomp_stream_tasks_count);

  debug_log_init ("** adding tasks: %ld (%u)\n", num_instances, gomp_stream_tasks_count);

  attr = &gomp_thread_attr;
  if (__builtin_expect (gomp_cpu_affinity != NULL, 0))
    {
      size_t stacksize;
      pthread_attr_init (&thread_attr);
      pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED);
      if (! pthread_attr_getstacksize (&gomp_thread_attr, &stacksize))
	pthread_attr_setstacksize (&thread_attr, stacksize);
      attr = &thread_attr;
#if 0 /* This should be handled separately ... we will have to build a
	 stream mapping and prevent other OMP threads from touching
	 the cores running streaming tasks.  */
      gomp_init_thread_affinity (attr);
#endif
    }

  for (i = 0; i < num_instances; ++i)
    {
      struct gomp_stream_thread_start_data *start_data;

      buf = (char *) gomp_malloc (arg_size + arg_align - 1);
      arg = (char *) (((uintptr_t) (buf) + arg_align - 1)
		      & ~(uintptr_t) (arg_align - 1));
      if (cpyfn)
	cpyfn (arg, data);
      else
	memcpy (arg, data, arg_size);

      start_data = gomp_malloc (sizeof (struct gomp_stream_thread_start_data));
      start_data->fn = fn;
      start_data->fn_data = arg;
      start_data->id = base_id + i;

      err = pthread_create (&pt, attr, gomp_stream_thread_start, start_data);
      if (err != 0)
	gomp_fatal ("Thread creation failed: %s", strerror (err));
    }

  if (__builtin_expect (gomp_cpu_affinity != NULL, 0))
    pthread_attr_destroy (&thread_attr);
}
