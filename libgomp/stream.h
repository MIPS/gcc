/* Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Antoniu Pop <apop@cri.ensmp.fr>
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

#ifndef GOMP_STREAM_H
#define GOMP_STREAM_H 1

/* This structure represents a stream between tasks.  Special care
   needs to be taken to ensure proper cache behaviour.  We need to
   separate groups of fields on independent cache lines according to
   the usage of each group.  In the case of SMPs, this is a critical
   requirement.  */

typedef struct gomp_stream
{
  /* Read-only group.  These fields will be initialized or redefined
     *very* sparsely (i.e., change of strategy in the runtime).
     Therefore, the behaviour of the cache protocol for the cache
     lines holding the following fields will be to stay in the Shared
     state.  */

  /* Circular buffer.  */
  char *buffer;

  /* Number of bytes in the circular buffer.  */
  size_t capacity;

  /* Size in bytes of an element in the stream.  */
  size_t size_elt;

  /* Size in bytes of sub-buffers for unsynchronized reads and writes.  */
  size_t size_local_buffer;

  /* Number of buffers/windows in the stream.  */
  size_t num_windows;

  /* Number of consumers of this stream.  */
  size_t num_consumers;

  /* Counter for assigning IDs to the consumers.  */
  size_t next_consumer;

  /* End of stream: true when producer has finished inserting elements.  */
  bool eos_p;

  /* Mutex used to ensure atomicity of access to all shared data (from
     the group above).  */
  gomp_mutex_t lock;

  /* Producer private group.  The following fields are only touched by
     the producer (read and written).  They should be on a separate
     cache line, which will ensure that the line stays in the Modified
     (or Exclusive) state and avoids ping-pong.  */

  /* Offset in bytes of the first empty element in the stream.  */
  size_t write_index __attribute__((aligned (64)));

  /* Consumer private group.  The following fields are only touched by
     the consumer (read and written).  Same remark as for the consumer
     private group.  */

  /* Offset in bytes of the first used element in the stream.  */
  size_t *read_index;

  /* Shared/Modified groups.  As a stream implicitely synchronizes
     tasks/threads, it is natural that some fields will be
     read/written by both producer and consumer.  It is therefore
     critical to separate those on individual cache lines (obviously
     same situation as locks).  */

  /* Offset in bytes of the sliding writing window.  Writing window is
     of size LOCAL_BUFFER_SIZE bytes.  Read/Written by producer, Read
     by consumer.  */
  size_t write_buffer_index __attribute__((aligned (64)));

  /* Offset in bytes of the sliding reading window.  Read window is of
     size LOCAL_BUFFER_SIZE bytes.  Read/Written by consumer, Read by
     producer.  */
  size_t *read_buffer_index;

  /* Semaphore for write_buffer_index.  The producer posts it to
     signal the consumer that additional data is available in the
     stream.  Initial value is 0 as no data is in the stream.  */
  gomp_sem_t *write_buffer_index_sem;

  /* Semaphore for read_buffer_index.  The consumer posts it to signal
     the producer that additional free space is available in the
     stream.  Initial value is the maximum of sliding windows as the
     whole steram data buffer is empty.  */
  gomp_sem_t *read_buffer_index_sem;

} *gomp_stream;

#endif /* GOMP_STREAM_H */
