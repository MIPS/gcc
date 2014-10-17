/* OpenACC Runtime - internal declarations

   Copyright (C) 2013-2014 Free Software Foundation, Inc.

   Contributed by Mentor Embedded.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This file contains data types and function declarations that are not
   part of the official OpenACC user interface.  There are declarations
   in here that are part of the GNU OpenACC ABI, in that the compiler is
   required to know about them and use them.

   The convention is that the all caps prefix "GOACC" is used group items
   that are part of the external ABI, and the lower case prefix "goacc"
   is used group items that are completely private to the library.  */

#ifndef _OACC_INT_H
#define _OACC_INT_H 1

#include "openacc.h"
#include "config.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility push(hidden)
#endif

typedef struct ACC_dispatch_t
{
  /* open or close a device instance.  */
  void *(*open_device_func) (int n);
  int (*close_device_func) (void *h);

  /* set or get the device number.  */
  int (*get_device_num_func) (void);
  void (*set_device_num_func) (int);

  /* availability */
  bool (*avail_func) (void);

  /* execute */
  void (*exec_func) (void (*) (void *), size_t, void **, void **, size_t *,
		     unsigned short *, int, int, int, int, void *);

  /* asynchronous routines  */
  int (*async_test_func) (int);
  int (*async_test_all_func) (void);
  void (*async_wait_func) (int);
  void (*async_wait_async_func) (int, int);
  void (*async_wait_all_func) (void);
  void (*async_wait_all_async_func) (int);
  void (*async_set_async_func) (int);

  /* NVIDIA target specific routines  */
  struct {
    void *(*get_current_device_func) (void);
    void *(*get_current_context_func) (void);
    void *(*get_stream_func) (int);
    int (*set_stream_func) (int, void *);
  } cuda;
} ACC_dispatch_t;

typedef enum ACC_dispatch_f
  {
    ACC_unified_mem_f = 1 << 0,
  }
ACC_dispatch_f;

struct gomp_device_descr;

void ACC_register (struct gomp_device_descr const *) __GOACC_NOTHROW;

/* Memory routines.  */
struct memmap_t *ACC_mem_open (void *, struct memmap_t *, int) __GOACC_NOTHROW;
bool ACC_mem_close (void *, struct memmap_t *) __GOACC_NOTHROW;
struct gomp_device_descr *ACC_resolve_device(int) __GOACC_NOTHROW;

/* Current dispatcher */
extern struct gomp_device_descr const *ACC_dev;

/* Device handle for current thread.  */
extern __thread void *ACC_handle;

typedef struct memmap_t
{
  unsigned live;
  struct target_mem_desc *tlist;
  struct gomp_memory_mapping mem_map;
} memmap_t;

/* Memory mapping */
extern __thread struct memmap_t *ACC_memmap;

void ACC_runtime_initialize (void);
void ACC_save_and_set_bind (acc_device_t);
void ACC_restore_bind (void);
void ACC_lazy_initialize (void);

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility pop
#endif

#endif /* _OACC_INT_H */
