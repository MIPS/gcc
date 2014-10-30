/* OpenACC Runtime Library: acc_device_host, acc_device_host_nonshm.

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

/* Simple implementation of support routines for a shared-memory
   acc_device_host, and a non-shared memory acc_device_host_nonshm, with the
   latter built as a plugin.  */

#include "openacc.h"
#include "config.h"
#include "libgomp.h"
#include "target.h"
#ifdef HOST_NONSHM_PLUGIN
#include "libgomp-plugin.h"
#include "oacc-plugin.h"
#else
#include "oacc-int.h"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef DEBUG

#ifdef HOST_NONSHM_PLUGIN
#define STATIC
#define GOMP(X) GOMP_PLUGIN_##X
#define SELF "host_nonshm plugin: "
#else
#define STATIC static
#define GOMP(X) gomp_##X
#define SELF "host: "
#endif

#ifndef HOST_NONSHM_PLUGIN
static struct gomp_device_descr host_dispatch;
#endif

STATIC const char *
get_name (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

#ifdef HOST_NONSHM_PLUGIN
  return "host_nonshm";
#else
  return "host";
#endif
}

STATIC int
get_type (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

#ifdef HOST_NONSHM_PLUGIN
  return TARGET_TYPE_HOST_NONSHM;
#else
  return TARGET_TYPE_HOST;
#endif
}

STATIC unsigned int
get_caps (void)
{
  unsigned int caps = TARGET_CAP_OPENACC_200 | TARGET_CAP_OPENMP_400
		      | TARGET_CAP_NATIVE_EXEC;

#ifndef HOST_NONSHM_PLUGIN
  caps |= TARGET_CAP_SHARED_MEM;
#endif

#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s: 0x%x\n", __FILE__, __FUNCTION__, caps);
#endif

  return caps;
}

STATIC int
get_num_devices (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

  return 1;
}

STATIC void
offload_register (void *host_table, void *target_data)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p, %p)\n", __FILE__, __FUNCTION__, host_table,
	   target_data);
#endif
}

STATIC int
device_init (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

  return get_num_devices ();
}

STATIC int
device_fini (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

  return 0;
}

STATIC int
device_get_table (struct mapping_table **table)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p)\n", __FILE__, __FUNCTION__, table);
#endif

  return 0;
}

STATIC bool
openacc_avail (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

  return 1;
}

STATIC void *
openacc_open_device (int n)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%u)\n", __FILE__, __FUNCTION__, n);
#endif

  return (void *) (intptr_t) n;
}

STATIC int
openacc_close_device (void *hnd)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p)\n", __FILE__, __FUNCTION__, hnd);
#endif

  return 0;
}

STATIC int
openacc_get_device_num (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

  return 0;
}

STATIC void
openacc_set_device_num (int n)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%u)\n", __FILE__, __FUNCTION__, n);
#endif

  if (n > 0)
    GOMP(fatal) ("device number %u out of range for host execution", n);
}

STATIC void *
device_alloc (size_t s)
{
  void *ptr = GOMP(malloc) (s);

#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%zd): %p\n", __FILE__, __FUNCTION__, s, ptr);
#endif

  return ptr;
}

STATIC void
device_free (void *p)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p)\n", __FILE__, __FUNCTION__, p);
#endif

  free (p);
}

STATIC void *
device_host2dev (void *d, const void *h, size_t s)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p, %p, %zd)\n", __FILE__, __FUNCTION__, d, h,
	   s);
#endif

#ifdef HOST_NONSHM_PLUGIN
  memcpy (d, h, s);
#endif

  return 0;
}

STATIC void *
device_dev2host (void *h, const void *d, size_t s)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p, %p, %zd)\n", __FILE__, __FUNCTION__, h, d,
	   s);
#endif

#ifdef HOST_NONSHM_PLUGIN
  memcpy (h, d, s);
#endif

  return 0;
}

STATIC void
device_run (void *fn_ptr, void *vars)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p, %p)\n", __FILE__, __FUNCTION__, fn_ptr,
	   vars);
#endif

  void (*fn)(void *) = (void (*)(void *)) fn_ptr;

  fn (vars);
}

STATIC void
openacc_parallel (void (*fn) (void *), size_t mapnum __attribute__((unused)),
		  void **hostaddrs __attribute__((unused)),
		  void **devaddrs __attribute__((unused)),
		  size_t *sizes __attribute__((unused)),
		  unsigned short *kinds __attribute__((unused)),
		  int num_gangs __attribute__((unused)),
		  int num_workers __attribute__((unused)),
		  int vector_length __attribute__((unused)),
		  int async __attribute__((unused)),
		  void *targ_mem_desc __attribute__((unused)))
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%p, %zu, %p, %p, %p, %d, %d, %d, %d, %p)\n",
	   __FILE__, __FUNCTION__, fn, mapnum, hostaddrs, sizes, kinds,
	   num_gangs, num_workers, vector_length, async, targ_mem_desc);
#endif

#ifdef HOST_NONSHM_PLUGIN
  fn (devaddrs);
#else
  fn (hostaddrs);
#endif
}

STATIC void
openacc_register_async_cleanup (void *targ_mem_desc)
{
#ifdef HOST_NONSHM_PLUGIN
  /* "Asynchronous" launches are executed synchronously on the (non-SHM) host,
     so there's no point in delaying host-side cleanup -- just do it now.  */
  GOMP_PLUGIN_async_unmap_vars (targ_mem_desc);
#endif
}

STATIC void
openacc_async_set_async (int async __attribute__((unused)))
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%d)\n", __FILE__, __FUNCTION__, async);
#endif
}

STATIC int
openacc_async_test (int async __attribute__((unused)))
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%d)\n", __FILE__, __FUNCTION__, async);
#endif

  return 1;
}

STATIC int
openacc_async_test_all (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif

  return 1;
}

STATIC void
openacc_async_wait (int async __attribute__((unused)))
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%d)\n", __FILE__, __FUNCTION__, async);
#endif
}

STATIC void
openacc_async_wait_all (void)
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s\n", __FILE__, __FUNCTION__);
#endif
}

STATIC void
openacc_async_wait_async (int async1 __attribute__((unused)),
                	  int async2 __attribute__((unused)))
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%d, %d)\n", __FILE__, __FUNCTION__, async1,
	   async2);
#endif
}

STATIC void
openacc_async_wait_all_async (int async __attribute__((unused)))
{
#ifdef DEBUG
  fprintf (stderr, SELF "%s:%s (%d)\n", __FILE__, __FUNCTION__, async);
#endif
}

STATIC void *
openacc_create_thread_data (void *targ_data __attribute__((unused)))
{
  return NULL;
}

STATIC void
openacc_destroy_thread_data (void *tls_data __attribute__((unused)))
{
}

#ifndef HOST_NONSHM_PLUGIN
static struct gomp_device_descr host_dispatch =
  {
    .name = "host",

    .type = TARGET_TYPE_HOST,
    .capabilities = TARGET_CAP_OPENACC_200 | TARGET_CAP_NATIVE_EXEC
		    | TARGET_CAP_SHARED_MEM,
    .id = 0,

    .is_initialized = false,
    .offload_regions_registered = false,

    .get_name_func = get_name,
    .get_type_func = get_type,
    .get_caps_func = get_caps,

    .device_init_func = device_init,
    .device_fini_func = device_fini,
    .get_num_devices_func = get_num_devices,
    .offload_register_func = offload_register,
    .device_get_table_func = device_get_table,

    .device_alloc_func = device_alloc,
    .device_free_func = device_free,
    .device_host2dev_func = device_host2dev,
    .device_dev2host_func = device_dev2host,
    
    .device_run_func = device_run,

    .openacc = {
      .open_device_func = openacc_open_device,
      .close_device_func = openacc_close_device,

      .get_device_num_func = openacc_get_device_num,
      .set_device_num_func = openacc_set_device_num,

      /* Device available.  */
      .avail_func = openacc_avail,

      .exec_func = openacc_parallel,

      .register_async_cleanup_func = openacc_register_async_cleanup,

      .async_set_async_func = openacc_async_set_async,
      .async_test_func = openacc_async_test,
      .async_test_all_func = openacc_async_test_all,
      .async_wait_func = openacc_async_wait,
      .async_wait_async_func = openacc_async_wait_async,
      .async_wait_all_func = openacc_async_wait_all,
      .async_wait_all_async_func = openacc_async_wait_all_async,

      .create_thread_data_func = openacc_create_thread_data,
      .destroy_thread_data_func = openacc_destroy_thread_data,

      .cuda = {
	.get_current_device_func = NULL,
	.get_current_context_func = NULL,
	.get_stream_func = NULL,
	.set_stream_func = NULL,
      }
    }
  };

/* Register this device type.  */
static __attribute__ ((constructor))
void ACC_host_init (void)
{
  gomp_mutex_init (&host_dispatch.mem_map.lock);
  ACC_register (&host_dispatch);
}
#endif

