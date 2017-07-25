/* OpenACC Runtime Library Definitions.

   Copyright (C) 2013-2018 Free Software Foundation, Inc.

   Contributed by Mentor Embedded.

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

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

#include <assert.h>
#include <string.h>
#include "openacc.h"
#include "libgomp.h"
#include "oacc-int.h"

static struct goacc_thread *
get_goacc_thread (void)
{
  struct goacc_thread *thr = goacc_thread ();
  if (!thr || !thr->dev)
    gomp_fatal ("no device active");
  return thr;
}

static struct gomp_device_descr *
get_goacc_thread_device (void)
{
  struct goacc_thread *thr = goacc_thread ();

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  return thr->dev;
}

attribute_hidden struct goacc_asyncqueue *
lookup_goacc_asyncqueue (struct goacc_thread *thr, bool create, int async)
{
  /* The special value acc_async_noval (-1) maps to the thread-specific
     default async stream.  */
  if (async == acc_async_noval)
    async = thr->default_async;

  if (async == acc_async_sync)
    return NULL;

  if (async < 0)
    gomp_fatal ("bad async %d", async);

  struct gomp_device_descr *dev = thr->dev;

  if (!create
      && (async >= dev->openacc.async.nasyncqueue
	  || !dev->openacc.async.asyncqueue[async]))
    return NULL;

  gomp_mutex_lock (&dev->openacc.async.lock);
  if (async >= dev->openacc.async.nasyncqueue)
    {
      int diff = async + 1 - dev->openacc.async.nasyncqueue;
      dev->openacc.async.asyncqueue
	= gomp_realloc (dev->openacc.async.asyncqueue,
			sizeof (goacc_aq) * (async + 1));
      memset (dev->openacc.async.asyncqueue + dev->openacc.async.nasyncqueue,
	      0, sizeof (goacc_aq) * diff);
      dev->openacc.async.nasyncqueue = async + 1;
    }

  if (!dev->openacc.async.asyncqueue[async])
    {
      dev->openacc.async.asyncqueue[async] = dev->openacc.async.construct_func ();

      /* Link new async queue into active list.  */
      goacc_aq_list n = gomp_malloc (sizeof (struct goacc_asyncqueue_list));
      n->aq = dev->openacc.async.asyncqueue[async];
      n->next = dev->openacc.async.active;
      dev->openacc.async.active = n;
    }
  gomp_mutex_unlock (&dev->openacc.async.lock);
  return dev->openacc.async.asyncqueue[async];
}

attribute_hidden struct goacc_asyncqueue *
get_goacc_asyncqueue (int async)
{
  struct goacc_thread *thr = get_goacc_thread ();
  return lookup_goacc_asyncqueue (thr, true, async);
}

int
acc_async_test (int async)
{
  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  goacc_aq aq = lookup_goacc_asyncqueue (thr, true, async);
  int res = thr->dev->openacc.async.test_func (aq);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }

  return res;
}

int
acc_async_test_all (void)
{
  struct goacc_thread *thr = goacc_thread ();
  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);

  int ret = 1;
  gomp_mutex_lock (&thr->dev->openacc.async.lock);
  for (goacc_aq_list l = thr->dev->openacc.async.active; l; l = l->next)
    if (!thr->dev->openacc.async.test_func (l->aq))
      {
	ret = 0;
	break;
      }
  gomp_mutex_unlock (&thr->dev->openacc.async.lock);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
  return ret;
}

void
acc_wait (int async)
{
  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  goacc_aq aq = lookup_goacc_asyncqueue (thr, true, async);
  thr->dev->openacc.async.synchronize_func (aq);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

/* acc_async_wait is an OpenACC 1.0 compatibility name for acc_wait.  */
#ifdef HAVE_ATTRIBUTE_ALIAS
strong_alias (acc_wait, acc_async_wait)
#else
void
acc_async_wait (int async)
{
  acc_wait (async);
}
#endif

void
acc_wait_async (int async1, int async2)
{
  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async2; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  goacc_aq aq2 = lookup_goacc_asyncqueue (thr, true, async2);
  goacc_aq aq1 = lookup_goacc_asyncqueue (thr, false, async1);
  if (!aq1)
    gomp_fatal ("invalid async 1");
  if (aq1 == aq2)
    gomp_fatal ("identical parameters");

  thr->dev->openacc.async.synchronize_func (aq1);
  thr->dev->openacc.async.serialize_func (aq1, aq2);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

void
acc_wait_all (void)
{
  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  struct gomp_device_descr *dev = get_goacc_thread_device ();

  gomp_mutex_lock (&dev->openacc.async.lock);
  for (goacc_aq_list l = dev->openacc.async.active; l; l = l->next)
    dev->openacc.async.synchronize_func (l->aq);
  gomp_mutex_unlock (&dev->openacc.async.lock);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

/* acc_async_wait_all is an OpenACC 1.0 compatibility name for acc_wait_all.  */
#ifdef HAVE_ATTRIBUTE_ALIAS
strong_alias (acc_wait_all, acc_async_wait_all)
#else
void
acc_async_wait_all (void)
{
  acc_wait_all ();
}
#endif

void
acc_wait_all_async (int async)
{
  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = goacc_thread ();

  acc_prof_info prof_info;
  acc_api_info api_info;
  bool profiling_setup_p
    = __builtin_expect (goacc_profiling_setup_p (thr, &prof_info, &api_info),
			false);
  if (profiling_setup_p)
    {
      prof_info.async = async; //TODO
      /* See <https://github.com/OpenACC/openacc-spec/issues/71>.  */
      prof_info.async_queue = prof_info.async;
    }

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  goacc_aq waiting_queue = lookup_goacc_asyncqueue (thr, true, async);

  gomp_mutex_lock (&thr->dev->openacc.async.lock);
  for (goacc_aq_list l = thr->dev->openacc.async.active; l; l = l->next)
    {
      thr->dev->openacc.async.synchronize_func (l->aq);
      if (waiting_queue)
	thr->dev->openacc.async.serialize_func (l->aq, waiting_queue);
    }
  gomp_mutex_unlock (&thr->dev->openacc.async.lock);

  if (profiling_setup_p)
    {
      thr->prof_info = NULL;
      thr->api_info = NULL;
    }
}

int
acc_get_default_async (void)
{
  /* In the following, no OpenACC Profiling Interface events can possibly be
     generated.  */

  struct goacc_thread *thr = goacc_thread ();

  if (!thr || !thr->dev)
    gomp_fatal ("no device active");

  return thr->default_async;
}

void
acc_set_default_async (int async)
{
  /* In the following, no OpenACC Profiling Interface events can possibly be
     generated.  */

  if (async < acc_async_sync)
    gomp_fatal ("invalid async argument: %d", async);

  struct goacc_thread *thr = get_goacc_thread ();
  thr->default_async = async;
}

static void
goacc_async_unmap_tgt (void *ptr)
{
  struct target_mem_desc *tgt = (struct target_mem_desc *) ptr;

  if (tgt->refcount > 1)
    tgt->refcount--;
  else
    gomp_unmap_tgt (tgt);
}

attribute_hidden void
goacc_async_copyout_unmap_vars (struct target_mem_desc *tgt,
				struct goacc_asyncqueue *aq)
{
  struct gomp_device_descr *devicep = tgt->device_descr;

  /* Increment reference to delay freeing of device memory until callback
     has triggered.  */
  tgt->refcount++;
  gomp_unmap_vars_async (tgt, true, aq);
  devicep->openacc.async.queue_callback_func (aq, goacc_async_unmap_tgt,
					      (void *) tgt);
}

attribute_hidden void
goacc_async_free (struct gomp_device_descr *devicep,
		  struct goacc_asyncqueue *aq, void *ptr)
{
  if (!aq)
    free (ptr);
  else
    devicep->openacc.async.queue_callback_func (aq, free, ptr);
}

attribute_hidden void
goacc_init_asyncqueues (struct gomp_device_descr *devicep)
{
  gomp_mutex_init (&devicep->openacc.async.lock);
  devicep->openacc.async.nasyncqueue = 0;
  devicep->openacc.async.asyncqueue = NULL;
  devicep->openacc.async.active = NULL;
}

attribute_hidden bool
goacc_fini_asyncqueues (struct gomp_device_descr *devicep)
{
  bool ret = true;
  if (devicep->openacc.async.nasyncqueue > 0)
    {
      goacc_aq_list next;
      for (goacc_aq_list l = devicep->openacc.async.active; l; l = next)
	{
	  ret &= devicep->openacc.async.destruct_func (l->aq);
	  next = l->next;
	  free (l);
	}
      free (devicep->openacc.async.asyncqueue);
      devicep->openacc.async.nasyncqueue = 0;
      devicep->openacc.async.asyncqueue = NULL;
      devicep->openacc.async.active = NULL;
    }
  gomp_mutex_destroy (&devicep->openacc.async.lock);
  return ret;
}
