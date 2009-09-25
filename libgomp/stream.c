/* Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Antoniu Pop <antoniu.pop@gmail.com> 
   and Sebastian Pop <sebastian.pop@amd.com>.

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

/* This is a Linux specific implementation of a stream communication
   mechanism for libgomp.  This type is private to the library.  This
   implementation relies on the futex syscall.  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sem.h"
#include "mutex.h"
#include "libgomp.h"


void
debug_stream (void *is)
{
  gomp_stream s = (gomp_stream) is;
  int i;

  printf ("Stream debug info:\n");
  printf ("  capacity           - %zu\n", s->capacity);
  printf ("  size_elt           - %zu\n", s->size_elt);
  printf ("  size_local_buffer  - %zu\n", s->size_local_buffer);
  printf ("  eos_p              - %d\n", s->eos_p);

  printf ("  num_consumers      - %zu\n", s->num_consumers);
  printf ("  num_windows        - %zu\n", s->num_windows);

  printf ("  write_index        - %zu\n", s->write_index);
  printf ("  write_buffer_index - %zu\n", s->write_buffer_index);

  printf ("  read_indexes       - ");
  for (i = 0; i < s->num_consumers; ++i)
    printf ("%zu\t", s->read_index[i]);
  printf ("\n");
  printf ("  read_buffer_index  - ");
  for (i = 0; i < s->num_consumers; ++i)
    printf ("%zu\t", s->read_buffer_index[i]);
  printf ("\n");

  printf ("  write_sem          - ");
  for (i = 0; i < s->num_consumers; ++i)
    printf ("%d\t", (int)s->write_buffer_index_sem[i]);
  printf ("\n");
  printf ("  read_sem           - ");  
  for (i = 0; i < s->num_consumers; ++i)
    printf ("%d\t", (int)s->read_buffer_index_sem[i]);
  printf ("\n\n");
}


static inline size_t
next_window (gomp_stream s, size_t index)
{
  size_t next = index + s->size_local_buffer;
  return ((next >= s->capacity) ? 0 : next);
}

/* Returns a new stream of NUM_WINDOWS * WINDOW_SIZE elements.  Each
   element is of size SIZE_ELT bytes.  Returns NULL when the
   allocation fails or when COUNT is less than 2.  */

void *
GOMP_stream_create (size_t size_elt, size_t num_windows, size_t window_size,
		    size_t num_consumers)
{
  gomp_stream s;
  int i;

  /* There should be enough place for two sliding windows.  */
  if (num_windows < 2)
    return NULL;

  s = (gomp_stream) gomp_malloc (sizeof (struct gomp_stream));

  s->size_elt = size_elt;
  s->num_windows = num_windows;
  s->size_local_buffer = window_size;
  s->num_consumers = num_consumers;

  s->capacity = num_windows * window_size;

  s->write_index = 0;
  s->write_buffer_index = 0;

  s->read_index = (size_t *)
    gomp_malloc (num_consumers * sizeof (size_t));
  s->read_buffer_index = (size_t *)
    gomp_malloc (num_consumers * sizeof (size_t));
  s->read_buffer_index_sem = (gomp_sem_t *)
    gomp_malloc (num_consumers * sizeof (gomp_sem_t));
  s->write_buffer_index_sem = (gomp_sem_t *)
    gomp_malloc (num_consumers * sizeof (gomp_sem_t));
  s->buffer = (char *) gomp_malloc (s->capacity);

  for (i = 0; i < num_consumers; ++i)
    {
      s->read_index[i] = 0;
      s->read_buffer_index[i] = 0;
      gomp_sem_init (&s->read_buffer_index_sem[i], num_windows - 1);
      gomp_sem_init (&s->write_buffer_index_sem[i], 0);
    }
  gomp_mutex_init (&s->lock);

  s->next_consumer = 0;
  s->eos_p = false;

  return s;
}

size_t
GOMP_stream_register_reader (void *vs)
{  
  gomp_stream s = (gomp_stream) vs;
  size_t consumer_id;

  /* Get a new ID for this consumer.  */
  gomp_mutex_lock (&s->lock);
  consumer_id = (s->next_consumer)++;
  gomp_mutex_unlock (&s->lock);

  /* Wait for the writer to leave this first window.  */
  gomp_sem_wait (&s->write_buffer_index_sem[consumer_id]);

  return consumer_id;
}

static inline void 
slide_read_window (gomp_stream s, size_t id)
{
  size_t next = next_window (s, s->read_buffer_index[id]);

  gomp_sem_wait (&s->write_buffer_index_sem[id]);
  s->read_buffer_index[id] = next;
  s->read_index[id] = next;
  gomp_sem_post (&s->read_buffer_index_sem[id]);
}

static inline void
slide_write_window (gomp_stream s)
{
  size_t next = next_window (s, s->write_buffer_index);
  int i;
  
  for (i = 0; i < s->num_consumers; ++i)
    gomp_sem_wait (&s->read_buffer_index_sem[i]);
  s->write_buffer_index = next;
  s->write_index = next;
  for (i = 0; i < s->num_consumers; ++i)
    gomp_sem_post (&s->write_buffer_index_sem[i]);
}

/* Commit the current element to stream S.  */

static inline void
gomp_stream_commit (gomp_stream s)
{
  int written_bytes_in_write_window = s->write_index - s->write_buffer_index;
  if (written_bytes_in_write_window + 2 * s->size_elt > s->size_local_buffer)
    slide_write_window (s);
  else
    s->write_index += s->size_elt;
}

/* Push element ELT to stream S.  This version of push copies the
   element ELT, then commits.  */

static inline void
gomp_stream_push (gomp_stream s, char *elt)
{
  memcpy (s->buffer + s->write_index, elt, s->size_elt);
  gomp_stream_commit (s);
}

/* Release from stream S the next element.  */

static inline void
gomp_stream_pop (gomp_stream s, size_t id)
{
  int read_bytes_in_read_window = s->read_index[id] - s->read_buffer_index[id];
  if (read_bytes_in_read_window + 2 * s->size_elt > s->size_local_buffer)
    slide_read_window (s, id);
  else
    s->read_index[id] += s->size_elt;
}

/* Returns the first element of the stream S.  Don't remove the
   element: for that, a call to gomp_stream_pop is needed.  */

void *
GOMP_stream_head (void *s, size_t id)
{
  return ((gomp_stream) s)->buffer + ((gomp_stream) s)->read_index[id];
}

/* Returns a pointer to the next available location in stream S that
   can hold an element.  Don't commit the element: for that, a call to
   gomp_stream_push is needed.  */

void *
GOMP_stream_tail (void *s)
{
  return ((gomp_stream) s)->buffer + ((gomp_stream) s)->write_index;
}

/* Returns true when there are no more elements to be read from the
   stream S.  Returning false guarantees that at least one element
   will be available for reading.  Unless this function is called
   before accessing the head of the stream, a deadlock is
   possible.  */

bool
GOMP_stream_eos_p (void *s, size_t id)
{
  return (((gomp_stream) s)->eos_p 
	  && (((gomp_stream) s)->read_index[id] 
	      == ((gomp_stream) s)->write_index));
}

/* Producer can set End Of Stream to stream S.  The producer has to
   slide the write window if it wrote something.  */

void
GOMP_stream_set_eos (void *vs)
{
  gomp_stream s = (gomp_stream) vs;
  int i;

  /* Allow the reader to access this same window to finish reading.
     The writer can no longer touch this window.  */
  s->eos_p = true;
  for (i = 0; i < s->num_consumers; ++i)
    gomp_sem_post (&s->write_buffer_index_sem[i]);
}

/* Free stream S.  */

void
GOMP_stream_destroy (void *vs)
{
  gomp_stream s = (gomp_stream) vs;
  int i;
  
  /* FIXME: must ensure only one task destroys this.  */

  gomp_mutex_destroy (&s->lock);
  for (i = 0; i < s->num_consumers; ++i)
    {
      gomp_sem_destroy (&(s->write_buffer_index_sem[i]));
      gomp_sem_destroy (&(s->read_buffer_index_sem[i]));
    }
  free (s->read_index);
  free (s->read_buffer_index);
  free (s->read_buffer_index_sem);
  free (s->write_buffer_index_sem);
  free (s->buffer);
  free (s);
}


void
GOMP_stream_commit (void *s)
{
  gomp_stream_commit ((gomp_stream) s);
}

void
GOMP_stream_push (void *s, void *elt)
{
  gomp_stream_push ((gomp_stream) s, (char *) elt);
}

void
GOMP_stream_pop (void *s, size_t id)
{
  gomp_stream_pop ((gomp_stream) s, id);
}
