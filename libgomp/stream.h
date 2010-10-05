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

#ifndef GOMP_STREAM_H
#define GOMP_STREAM_H 1

#include <limits.h>

/* Define the type and maximum value for the indices used within the
   stream.  The indices are strictly monotonically increasing
   integers, so we need a type that does not wrap around too fast.  An
   implementation of the wrap-around is necessary for systems where
   this poses a problem.  */
/* typedef unsigned long long int gomp_stream_index_t; */
#define GOMP_STREAM_MAX_INDEX ULLONG_MAX

/*#define LAZY_SYNCH
  #define GUARDED_WAKE*/

/*
typedef enum {
  GOMP_STREAM_INITIALIZED = 1,
  GOMP_STREAM_ALLOCATED   = 2,
  GOMP_STREAM_PRODUCED    = 4,
  GOMP_STREAM_CONSUMED    = 8,
  GOMP_STREAM_ZOMBIE      = 16,
  GOMP_STREAM_STALL       = 32
} gomp_stream_state;
*/

typedef enum
{
  READ_VIEW,
  WRITE_VIEW
} gomp_stream_view_type_t;

struct gomp_stream;
struct gomp_stream_task;


/* GOMP_STREAM_VIEW data structure.  Producer and consumer tasks
   connect to a GOMP_STREAM using views.  */

typedef struct gomp_stream_view
{
  /* The stream accessed through this view.  WARNING: this field needs
     to be first.  */
  struct gomp_stream *stream;

  /* The alignment directives are needed to ensure these
     high-potential false-sharing fields are on their own cache
     lines.  */
  /* Lower and upper bounds accessible in the stream through this
     view.  */
  unsigned long long lower_index __attribute__((aligned (64)));
  unsigned long long upper_index __attribute__((aligned (64)));

  /* The task using this view.  */
  struct gomp_stream_task *task __attribute__((aligned (64)));

  /* Setting this flag means the process accessing the stream through
     this view relinquishes his access to the stream (generally when
     terminating).  */
  bool end_p;

  /* Type of this view (read or write).  */
  gomp_stream_view_type_t type;

  /* In order to avoid excessive accesses to the global minimum index
     (released or consumed), which strongly impacts cache traffic, use
     this duplcated field for an additional level of caching and only
     update when needed.  */
  unsigned long long local_min_value;

} gomp_stream_view_t, *gomp_stream_view_p;

/* List of GOMP_STREAM_VIEWs.  As this list is only modified in the
   initialization phase and we never remove items, we'll use an
   array.  */

typedef struct gomp_stream_view_list
{
  gomp_stream_view_p *views;
  int nr_views;
  int size;
} gomp_stream_view_list_t, *gomp_stream_view_list_p;


/* GOMP_STREAM_VIEW_HANDLE data structure.  This allows the tasks
   interacting through the GOMP_STREAM to which this handle is
   attached to keep updated information global to all similar views on
   this stream.  */

typedef struct gomp_stream_view_handle
{
  /* The alignment directives are needed to ensure these
     high-potential false-sharing fields are on their own cache
     lines.  */
  /* Latest computed value of the min released index and max acquired
     index values across views.  */
  volatile unsigned long long current_min __attribute__((aligned (64)));
  volatile unsigned long long current_max __attribute__((aligned (64)));

  /* Bookkeeping for view connections.  */
  gomp_stream_view_list_t view_list __attribute__((aligned (64)));
  int nr_expected_views;
  int nr_registered_views;
  int nr_unregistered_views;

  /* Enforce atomic connection of the views to the stream.  */
  gomp_mutex_t connect_view_mutex;

} gomp_stream_view_handle_t, *gomp_stream_view_handle_p;


/* GOMP_STREAM data structure.  */

typedef struct gomp_stream
{
  /* WARNING: the first two fields (BUFFER and BUFFER_MASK) need to
     remain in their respective positions.  */

  /* The pointer to the buffer, as well as the subsequent buffer
     description, should be on a single mostly read cache line.  The
     EOS_P flag is only set at the very end of the use of this
     stream.  */
  /* Circular buffer containing the data communicated through this
     stream.  */
  char *buffer __attribute__((aligned (64)));

  /* Size of the buffer and the bitmask used for modulo computation
     for the wrap-around.  The size is expressed in basic elements for
     this stream.  The size in bytes of the buffer is
     BUFFER_SIZE * ELEMENT_SIZE.  */
  unsigned long long buffer_mask;
  unsigned long long buffer_size;
  unsigned long long element_size;

  /* True once all the tasks that should be expected to connect to
     this stream been declared.  */
  bool expected_ready_p;

  /* True once all the tasks expected to connect to this stream have
     done so.  */
  bool connected_p;

  /* End of stream: true when all producers have finished committing
     all the data and are terminating.  */
  bool eos_p;

  /* Handles for read and write views on this stream.  */
  gomp_stream_view_handle_t read_views;
  gomp_stream_view_handle_t write_views;

  /* Barrier used both for waiting for all views to connect to the
     stream and to find the last view disconnecting (and who therefore
     frees this stream).  */
  /*gomp_barrier_t view_handling_barrier;*/

  /* Counter of the number of total unregistered views, both read and
     write, used to determine the last task deconnecting its view.
     The last task will also free the data structures.  */
  int unregistered_views;

#ifndef HAVE_SYNC_BUILTINS
  /* We may need a lock for atomicity if no atomic operations are
     available. */
  gomp_mutex_t stream_mutex;
#endif
} gomp_stream_t, *gomp_stream_p;


/* GOMP_STREAM_TASK data structure.  Runtime node in the task
   graph.  */

typedef struct gomp_stream_task
{
  /* Lists of views on streams used by this task.  */
  gomp_stream_view_list_t read_view_list;
  gomp_stream_view_list_t write_view_list;

} gomp_stream_task_t, *gomp_stream_task_p;

#endif /* GOMP_STREAM_H */
