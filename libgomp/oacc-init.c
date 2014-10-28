/* OpenACC Runtime initialization routines

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

#include "libgomp.h"
#include "target.h"
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <stdio.h>

gomp_mutex_t acc_device_lock;

/* Current dispatcher, and how it was initialized */
static acc_device_t init_key = _ACC_device_hwm;

/* The dispatch table for the current accelerator device.  This is currently
   global, so you can only have one type of device open at any given time in a
   program.  */
struct gomp_device_descr const *ACC_dev;

/* Handle for current thread.  */
__thread  void *ACC_handle;
static __thread int handle_num = -1;

/* This context structure associates the handle for a physical device with
   memory-mapping information for that device, and is used to associate new
   host threads with previously-opened devices.  Note that it's not directly
   connected with the CUDA "context" concept as used by the NVidia plugin.  */
struct ACC_context {
  struct memmap_t *ACC_memmap;
  void *ACC_handle;

  struct ACC_context *next;
};

static struct ACC_context *ACC_contexts;

static struct gomp_device_descr const *dispatchers[_ACC_device_hwm] = { 0 };

void
ACC_register (struct gomp_device_descr const *disp)
{
  gomp_mutex_lock (&acc_device_lock);

  assert (acc_device_type (disp->type) != acc_device_none
	  && acc_device_type (disp->type) != acc_device_default
	  && acc_device_type (disp->type) != acc_device_not_host);
  assert (!dispatchers[disp->type]);
  dispatchers[disp->type] = disp;

  gomp_mutex_unlock (&acc_device_lock);
}

static void
close_handle (void)
{
  if (ACC_memmap)
    {
      if (ACC_mem_close (ACC_handle, ACC_memmap))
        {
          if (ACC_dev->openacc.close_device_func (ACC_handle) < 0)
            gomp_fatal ("failed to close device");
        }

      ACC_memmap = 0;
    }
}

static struct gomp_device_descr const *
resolve_device (acc_device_t d)
{
  acc_device_t d_arg = d;

  switch (d)
    {
    case acc_device_default:
      {
	if (goacc_device_type)
	  {
	    /* Lookup the named device.  */
	    while (++d != _ACC_device_hwm)
	      if (dispatchers[d]
		  && !strcasecmp (goacc_device_type, dispatchers[d]->name)
		  && dispatchers[d]->openacc.avail_func ())
		goto found;

	    gomp_fatal ("device type %s not supported", goacc_device_type);
	  }

	/* No default device specified, so start scanning for any non-host
	   device that is available.  */
	d = acc_device_not_host;
      }
      /* FALLTHROUGH */

    case acc_device_not_host:
      /* Find the first available device after acc_device_not_host.  */
      while (++d != _ACC_device_hwm)
	if (dispatchers[d] && dispatchers[d]->openacc.avail_func ())
	  goto found;
      if (d_arg == acc_device_default)
	{	  
	  d = acc_device_host;
	  goto found;
	}
      gomp_fatal ("no device found");
      break;

    case acc_device_host:
      break;

    default:
      if (d > _ACC_device_hwm)
	gomp_fatal ("device %u out of range", (unsigned)d);
      break;
    }
 found:

  assert (d != acc_device_none
	  && d != acc_device_default
	  && d != acc_device_not_host);

  return dispatchers[d];
}

static struct gomp_device_descr const *
_acc_init (acc_device_t d)
{
  struct gomp_device_descr const *acc_dev;

  if (ACC_dev)
    gomp_fatal ("device already active");

  init_key = d;  /* We need to remember what we were intialized as, to
		    check shutdown etc.  */

  acc_dev = resolve_device (d);
  if (!acc_dev || !acc_dev->openacc.avail_func ())
    gomp_fatal ("device %u not supported", (unsigned)d);

  if (!acc_dev->is_initialized)
    gomp_init_device ((struct gomp_device_descr *) acc_dev);

  return acc_dev;
}

/* Open the ORD'th device of the currently-active type (ACC_dev must be
   initialised before calling).  If ORD is < 0, open the default-numbered
   device (set by the ACC_DEVICE_NUM environment variable or a call to
   acc_set_device_num), or leave any currently-opened device as is.  "Opening"
   consists of  calling the device's open_device_func hook, and either creating
   a new memory mapping or associating a new thread with an existing such
   mapping (that matches ACC_handle, i.e. which corresponds to the same
   physical device).  */

static void
lazy_open (int ord)
{
  struct ACC_context *acc_ctx;

  if (ACC_memmap)
    {
      assert (ord < 0 || ord == handle_num);
      return;
    }

  assert (ACC_dev);

  if (ord < 0)
    ord = goacc_device_num;

  ACC_handle = ACC_dev->openacc.open_device_func (ord);
  handle_num = ord;

  for (acc_ctx = ACC_contexts; acc_ctx != NULL; acc_ctx = acc_ctx->next)
    {
      if (acc_ctx->ACC_handle == ACC_handle)
        {
          ACC_memmap = acc_ctx->ACC_memmap;
	  ACC_dev->openacc.async_set_async_func (acc_async_sync);

          return;
        }
    }

  ACC_memmap = ACC_mem_open (ACC_handle, NULL, handle_num);

  ACC_dev->openacc.async_set_async_func (acc_async_sync);

  acc_ctx = gomp_malloc (sizeof (struct ACC_context));
  acc_ctx->ACC_handle = ACC_handle;
  acc_ctx->ACC_memmap = ACC_memmap;

  if (!ACC_memmap->mem_map.is_initialized)
    gomp_init_tables (ACC_dev, &ACC_memmap->mem_map);

  acc_ctx->next = ACC_contexts;
  ACC_contexts = acc_ctx;
}

/* OpenACC 2.0a (3.2.12, 3.2.13) doesn't specify whether the serialization of
   init/shutdown is per-process or per-thread.  We choose per-process.  */

void
acc_init (acc_device_t d)
{
  if (!ACC_dev)
    gomp_init_targets_once ();

  gomp_mutex_lock (&acc_device_lock);

  ACC_dev = _acc_init (d);

  lazy_open (-1);

  gomp_mutex_unlock (&acc_device_lock);
}

ialias (acc_init)

void
_acc_shutdown (acc_device_t d)
{
  /* We don't check whether d matches the actual device found, because
     OpenACC 2.0 (3.2.12) says the parameters to the init and this
     call must match (for the shutdown call anyway, it's silent on
     others).  */

  if (!ACC_dev)
    gomp_fatal ("no device initialized");
  if (init_key != d)
    gomp_fatal ("device %u(%u) is initialized",
	       (unsigned)init_key, (unsigned)ACC_dev->type);

  close_handle ();

  while (ACC_contexts != NULL)
    {
      struct ACC_context *c = ACC_contexts;
      ACC_contexts = ACC_contexts->next;
      free (c);
    }

  gomp_fini_device ((struct gomp_device_descr *) ACC_dev);

  ACC_dev = 0;
  ACC_handle = 0;
  handle_num = -1;
}

void
acc_shutdown (acc_device_t d)
{
  gomp_mutex_lock (&acc_device_lock);

  _acc_shutdown (d);

  gomp_mutex_unlock (&acc_device_lock);
}

ialias (acc_shutdown)

static struct gomp_device_descr const *
lazy_init (acc_device_t d)
{
  if (ACC_dev)
    {
      /* Re-initializing the same device, do nothing.  */
      if (d == init_key)
	return ACC_dev;

      _acc_shutdown (init_key);
    }

  assert (!ACC_dev);

  return _acc_init (d);
}

static void
lazy_init_and_open (acc_device_t d)
{
  if (!ACC_dev)
    gomp_init_targets_once ();

  gomp_mutex_lock (&acc_device_lock);

  ACC_dev = lazy_init (d);

  lazy_open (-1);

  gomp_mutex_unlock (&acc_device_lock);
}

int
acc_get_num_devices (acc_device_t d)
{
  int n = 0;
  struct gomp_device_descr const *acc_dev;

  if (d == acc_device_none)
    return 0;

  if (!ACC_dev)
    gomp_init_targets_once ();

  acc_dev = resolve_device (d);
  if (!acc_dev)
    return 0;

  n = acc_dev->device_init_func ();
  if (n < 0)
    n = 0;

  return n;
}

ialias (acc_get_num_devices)

void
acc_set_device_type (acc_device_t d)
{
  lazy_init_and_open (d);
}

ialias (acc_set_device_type)

acc_device_t
acc_get_device_type (void)
{
  acc_device_t res = acc_device_none;
  const struct gomp_device_descr *dev;

  if (ACC_dev)
    res = acc_device_type (ACC_dev->type);
  else
    {
      gomp_init_targets_once ();

      dev = resolve_device (acc_device_default);
      res = acc_device_type (dev->type);
    }

  assert (res != acc_device_default
	  && res != acc_device_not_host);

  return res;
}

ialias (acc_get_device_type)

int
acc_get_device_num (acc_device_t d)
{
  const struct gomp_device_descr *dev;
  int num;

  if (d >= _ACC_device_hwm)
    gomp_fatal ("device %u out of range", (unsigned)d);

  if (!ACC_dev)
    gomp_init_targets_once ();

  dev = resolve_device (d);
  if (!dev)
    gomp_fatal ("no devices of type %u", d);

  /* We might not have called lazy_open for this host thread yet, in which case
     the get_device_num_func hook will return -1.  */
  num = dev->openacc.get_device_num_func ();
  if (num < 0)
    num = goacc_device_num;
  
  return num;
}

ialias (acc_get_device_num)

void
acc_set_device_num (int n, acc_device_t d)
{
  const struct gomp_device_descr *dev;
  int num_devices;

  if (!ACC_dev)
    gomp_init_targets_once ();
  
  if ((int) d == 0)
    {
      int i;
      
      /* A device setting of zero sets all device types on the system to use
         the Nth instance of that device type.  Only attempt it for initialized
	 devices though.  */
      for (i = acc_device_not_host + 1; i < _ACC_device_hwm; i++)
        {
	  dev = resolve_device (d);
	  if (dev && dev->is_initialized)
	    dev->openacc.set_device_num_func (n);
	}

      /* ...and for future calls to acc_init/acc_set_device_type, etc.  */
      goacc_device_num = n;
    }
  else
    {
      gomp_mutex_lock (&acc_device_lock);

      ACC_dev = lazy_init (d);

      num_devices = ACC_dev->get_num_devices_func ();

      if (n >= num_devices)
        gomp_fatal ("device %u out of range", n);

      if (n != handle_num)
	close_handle ();

      lazy_open (n);

      gomp_mutex_unlock (&acc_device_lock);
    }
}

ialias (acc_set_device_num)

int
acc_on_device (acc_device_t dev)
{
  if (ACC_dev && acc_device_type (ACC_dev->type) == acc_device_host_nonshm)
    return dev == acc_device_host_nonshm || dev == acc_device_not_host;
    
  /* Just rely on the compiler builtin.  */
  return __builtin_acc_on_device (dev);
}
ialias (acc_on_device)

attribute_hidden void
ACC_runtime_initialize (void)
{
  gomp_mutex_init (&acc_device_lock);

  ACC_contexts = NULL;
}

/* Compiler helper functions */

static __thread struct gomp_device_descr const *saved_bound_dev;

void
ACC_save_and_set_bind (acc_device_t d)
{
  assert (!saved_bound_dev);

  saved_bound_dev = ACC_dev;
  ACC_dev = dispatchers[d];
}

void
ACC_restore_bind (void)
{
  ACC_dev = saved_bound_dev;
  saved_bound_dev = NULL;
}

/* This is called from any OpenACC support function that may need to implicitly
   initialize the libgomp runtime.  On exit all such initialization will have
   been done, and both the global ACC_dev and the per-host-thread ACC_memmap
   pointers will be valid.  */

void
ACC_lazy_initialize (void)
{
  if (ACC_dev && ACC_memmap)
    return;

  if (!ACC_dev)
    lazy_init_and_open (acc_device_default);
  else
    {
      gomp_mutex_lock (&acc_device_lock);
      lazy_open (-1);
      gomp_mutex_unlock (&acc_device_lock);
    }
}
