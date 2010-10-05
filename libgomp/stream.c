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


/* Data structures creation and pipeline initialization.  */

/* Allocate and initialize a GOMP_STREAM for data elements of size
   ELEMENT_SIZE using a circular buffer of STREAM_BUFFER_SIZE such
   elements.  Returns a pointer to the newly allocated stream.  The
   user may provide a pointer to pre-allocated memory to be used as
   BUFFER for the stream.  */

void *
GOMP_stream_create_stream (size_t element_size,
			   unsigned long long stream_buffer_size,
			   char *buffer)
{
  gomp_stream_p stream = (gomp_stream_p) gomp_malloc (sizeof (gomp_stream_t));

  /* Initialize and allocate the data buffer.  We force the
     buffer_size to be a power of 2 for efficient modulo computation
     of the indices in the circular buffer.  */
  stream->element_size = element_size;
  stream->buffer_size = 1;
  while(stream->buffer_size < stream_buffer_size)
    stream->buffer_size <<= 1;
  stream->buffer_mask = stream->buffer_size - 1;

  /* In case the user provided a pre-allocated buffer, we need to
     ensure it is properly sized.  */
  if (buffer != NULL)
    {
      if (stream->buffer_size != stream_buffer_size)
	gomp_fatal ("GOMP_stream: provided buffer size is not power of 2.");

      stream->buffer = buffer;
    }
  else
    {
      stream->buffer =
	(void *) gomp_malloc (stream->element_size * stream->buffer_size);
    }

  stream->expected_ready_p = false;
  stream->connected_p = false;
  stream->eos_p = false;

  /* Initialize the view_handles.  */
  stream->read_views.current_min = stream->buffer_size;
  stream->read_views.current_max = 0;
  stream->read_views.view_list.views = NULL;
  stream->read_views.view_list.nr_views = 0;
  stream->read_views.view_list.size = 0;
  stream->read_views.nr_expected_views = 0;
  stream->read_views.nr_registered_views = 0;
  stream->read_views.nr_unregistered_views = 0;
  gomp_mutex_init (&stream->read_views.connect_view_mutex);

  stream->write_views.current_min = 0;
  stream->write_views.current_max = stream->buffer_size;
  stream->write_views.view_list.views = NULL;
  stream->write_views.view_list.nr_views = 0;
  stream->write_views.view_list.size = 0;
  stream->write_views.nr_expected_views = 0;
  stream->write_views.nr_registered_views = 0;
  stream->write_views.nr_unregistered_views = 0;
  gomp_mutex_init (&stream->write_views.connect_view_mutex);

#ifndef HAVE_SYNC_BUILTINS
  gomp_mutex_init (&stream->stream_mutex);
#endif

  return stream;
}

/* Allocate and initialize a generic GOMP_STREAM_VIEW that can be
   connected to any stream to give either read or write access
   depending on its TYPE.  Returns a pointer to the newly allocated
   view.  */

static inline void *
gomp_stream_create_view (int type)
{
  gomp_stream_view_p view =
    (gomp_stream_view_p) gomp_malloc (sizeof(gomp_stream_view_t));

  view->lower_index = 0;
  view->upper_index = 0;
  view->stream = NULL;
  view->end_p = false;
  view->type = type;
  view->local_min_value = 0;

  return view;
}

/* Wrapper for creating a READ view.  */

void *
GOMP_stream_create_read_view (void)
{
  return gomp_stream_create_view (READ_VIEW);
}

/* Wrapper for creating a WRITE view.  */

void *
GOMP_stream_create_write_view (void)
{
  return gomp_stream_create_view (WRITE_VIEW);
}

/* Allocate and initialize a GOMP_STREAM_TASK data structure.  */

void *
GOMP_stream_create_task (void)
{
  gomp_stream_task_p task = 
    (gomp_stream_task_p) gomp_malloc (sizeof(gomp_stream_task_t));

  task->read_view_list.views = NULL;
  task->read_view_list.nr_views = 0;
  task->read_view_list.size = 0;

  task->write_view_list.views = NULL;
  task->write_view_list.nr_views = 0;
  task->write_view_list.size = 0;

  return task;
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
  gomp_stream_add_view_to_list (view, task_list);

  /* Connect the view to the stream.  This must be done atomically as
     this data structure is shared with the other producer/consumer
     tasks.  */
  gomp_mutex_lock (&vh->connect_view_mutex);
  gomp_stream_add_view_to_list (view, stream_list);
  gomp_mutex_unlock (&vh->connect_view_mutex);
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
  int num_read_views = task->read_view_list.nr_views;
  int num_write_views = task->write_view_list.nr_views;
  int i;
  bool done;

  do
    {
      done = true;

      for (i = 0; i < num_read_views; ++i)
	if (!gomp_stream_check_connected (task->read_view_list.views[i]->stream))
	  done = false;

      for (i = 0; i < num_write_views; ++i)
	if (!gomp_stream_check_connected (task->write_view_list.views[i]->stream))
	  done = false;
    }
  while (!done);
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

unsigned long long
GOMP_stream_update (void *v, const unsigned long long index)
{
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  view->upper_index = index;

  /* In case another consumer has received permission to read up to a
     yet higher index, then there is no need to check for this one.  */
  if (index > view->stream->read_views.current_max)
    {
      gomp_stream_wait_release (view, &view->stream->write_views, index);

      /* If the producers have finished producing for this stream, we
	 need to ensure we do not give read permission to the view
	 past the highest fully committed index (committed by all
	 producers).  */
      if (view->stream->eos_p)
	{
	  view->stream->write_views.current_min =
	    gomp_stream_compute_lower_min (&view->stream->write_views.view_list);

	  if (index > view->stream->write_views.current_min)
	    {
	      return view->stream->write_views.current_min;
	    }
	}
      view->stream->read_views.current_max = index;
    }
  return index;
}

/* Request write access for the view V to the stream up to INDEX.  */

void
GOMP_stream_stall (void *v, const unsigned long long index)
{
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  view->upper_index = index;

  if (index > view->stream->write_views.current_max)
    {
      gomp_stream_wait_release (view, &view->stream->read_views, index);
      view->stream->write_views.current_max = index;
    }
}

/* Relinquish read access for the view V to the stream up to
   INDEX.  */

void
GOMP_stream_release (void *v, const unsigned long long index)
{
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  view->lower_index = index + view->stream->buffer_size;
}

/* Relinquish write access for the view V to the stream up to
   INDEX.  */

void
GOMP_stream_commit (void *v, const unsigned long long index)
{
  gomp_stream_view_p view = (gomp_stream_view_p) v;
  view->lower_index = index;
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
  int i;

  for (i = 0; i < num_read_views; ++i)
    gomp_stream_unregister_view (task->read_view_list.views[i]);

  for (i = 0; i < num_write_views; ++i)
    gomp_stream_unregister_view (task->write_view_list.views[i]);

  free (task->read_view_list.views);
  free (task->write_view_list.views);
  free (task);
}





