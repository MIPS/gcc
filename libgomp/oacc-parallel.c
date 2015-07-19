/* Copyright (C) 2013-2015 Free Software Foundation, Inc.

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

/* This file handles OpenACC constructs.  */

#include "openacc.h"
#include "libgomp.h"
#include "libgomp_g.h"
#include "gomp-constants.h"
#include "oacc-int.h"
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>  /* For PRIu64.  */
#endif
#include <string.h>
#include <stdarg.h>
#include <assert.h>

/* Returns the number of mappings associated with the pointer or pset. PSET
   have three mappings, whereas pointer have two.  */

static int
find_pointer (int pos, size_t mapnum, unsigned short *kinds)
{
  if (pos + 1 >= mapnum)
    return 0;

  unsigned char kind = kinds[pos+1] & 0xff;

  if (kind == GOMP_MAP_TO_PSET)
    return 3;
  else if (kind == GOMP_MAP_POINTER)
    return 2;

  return 0;
}

static void *__goacc_host_ganglocal_ptr;

void *
GOACC_get_ganglocal_ptr (void)
{
  return __goacc_host_ganglocal_ptr;
}

static void
alloc_host_shared_mem (size_t shared_size)
{
  if (shared_size > 0)
    __goacc_host_ganglocal_ptr = malloc (shared_size);
}

static void
free_host_shared_mem (void)
{
  if (__goacc_host_ganglocal_ptr)
    {
      free (__goacc_host_ganglocal_ptr);
      __goacc_host_ganglocal_ptr = NULL;
    }
}

static void
alloc_ganglocal_addrs (size_t mapnum, void **hostaddrs, size_t *sizes,
		       unsigned short *kinds)
{
  int i;
  const int typemask = 0xff;
  void *t;

  for (i = 0; i < mapnum; i++)
    {
      if ((kinds[i] & typemask) == GOMP_MAP_FORCE_TO_GANGLOCAL)
	{
	  t = malloc (sizes[i]);
	  memcpy (t, hostaddrs[i], sizes[i]);
	  hostaddrs[i] = t;

	  if (i + 1 < mapnum && GOMP_MAP_POINTER_P (kinds[i+1] & typemask))
	    {
	      size_t *ptr = (size_t *) malloc (sizeof (size_t *));
	      *ptr = (size_t)t;
	      hostaddrs[i+1] = ptr;
	      i++;
	    }
	}
    }
}

static struct oacc_static
{
  void *addr;
  size_t size;
  unsigned short mask;
  bool free;
  struct oacc_static *next;
} *oacc_statics;

static bool alloc_done = false;

void
goacc_allocate_static (acc_device_t d)
{
  struct oacc_static *s;

  if (alloc_done)
    assert (0);

  for (s = oacc_statics; s; s = s->next)
    {
      void *d;

      switch (s->mask)
	{
	case GOMP_MAP_FORCE_ALLOC:
	  break;

	case GOMP_MAP_FORCE_TO:
	  d = acc_deviceptr (s->addr);
	  acc_memcpy_to_device (d, s->addr, s->size);
	  break;

	case GOMP_MAP_FORCE_DEVICEPTR:
	case GOMP_MAP_DEVICE_RESIDENT:
	case GOMP_MAP_LINK:
	  break;

	default:
	  assert (0);
	  break;
	}
    }

  alloc_done = true;
}

void
goacc_deallocate_static (acc_device_t d)
{
  struct oacc_static *s;
  unsigned short mask = GOMP_MAP_FORCE_DEALLOC;

  if (!alloc_done)
    return;

  for (s = oacc_statics; s; s = s->next)
    GOACC_enter_exit_data (d, 1, &s->addr, &s->size, &mask, 0, 0);

  alloc_done = false;
}

static void goacc_wait (int async, int num_waits, va_list ap);

void
GOACC_parallel (int device, void (*fn) (void *),
		size_t mapnum, void **hostaddrs, size_t *sizes,
		unsigned short *kinds,
		int num_gangs, int num_workers, int vector_length,
		size_t shared_size,
		int async, int num_waits, ...)
{
  bool host_fallback = device == GOMP_DEVICE_HOST_FALLBACK;
  va_list ap;
  struct goacc_thread *thr;
  struct gomp_device_descr *acc_dev;
  struct target_mem_desc *tgt;
  void **devaddrs;
  int i;
  struct splay_tree_key_s k;
  splay_tree_key tgt_fn_key;
  void (*tgt_fn);

#ifdef HAVE_INTTYPES_H
  gomp_debug (0, "%s: mapnum=%"PRIu64", hostaddrs=%p, sizes=%p, kinds=%p, "
	      "shared_size=%"PRIu64", async = %d\n",
	      __FUNCTION__, (uint64_t) mapnum, hostaddrs, sizes, kinds,
	      (uint64_t) shared_size, async);
#else
  gomp_debug (0, "%s: mapnum=%lu, hostaddrs=%p, sizes=%p, kinds=%p, "
	      "shared_size=%lu, async=%d\n",
	      __FUNCTION__, (unsigned long) mapnum, hostaddrs, sizes, kinds,
	      (unsigned long) shared_size, async);
#endif

  alloc_ganglocal_addrs (mapnum, hostaddrs, sizes, kinds);

  goacc_lazy_initialize ();

  thr = goacc_thread ();
  acc_dev = thr->dev;

  for (i = 0; i < (signed)(mapnum - 1); i++)
    {
      unsigned short kind1 = kinds[i] & 0xff;
      unsigned short kind2 = kinds[i+1] & 0xff;

      /* Handle Fortran deviceptr clause.  */
      if ((kind1 == GOMP_MAP_FORCE_DEVICEPTR && kind2 == GOMP_MAP_POINTER)
	   && (sizes[i + 1] == 0)
	   && (hostaddrs[i] == *(void **)hostaddrs[i + 1]))
	{
	  kinds[i+1] = kinds[i];
	  sizes[i+1] = sizeof (void *);
	  hostaddrs[i] = NULL;
	}
    }

  /* Host fallback if "if" clause is false or if the current device is set to
     the host.  */
  if (host_fallback)
    {
      goacc_save_and_set_bind (acc_device_host);
      alloc_host_shared_mem (shared_size);
      fn (hostaddrs);
      free_host_shared_mem ();
      goacc_restore_bind ();
      return;
    }
  else if (acc_device_type (acc_dev->type) == acc_device_host)
    {
      alloc_host_shared_mem (shared_size);
      fn (hostaddrs);
      free_host_shared_mem ();
      return;
    }
  
  if (acc_device_type (acc_dev->type) == acc_device_host_nonshm)
    alloc_host_shared_mem (shared_size);

  va_start (ap, num_waits);
  
  if (num_waits > 0)
    goacc_wait (async, num_waits, ap);

  va_end (ap);

  acc_dev->openacc.async_set_async_func (async);

  if (!(acc_dev->capabilities & GOMP_OFFLOAD_CAP_NATIVE_EXEC))
    {
      k.host_start = (uintptr_t) fn;
      k.host_end = k.host_start + 1;
      gomp_mutex_lock (&acc_dev->lock);
      tgt_fn_key = splay_tree_lookup (&acc_dev->mem_map, &k);
      gomp_mutex_unlock (&acc_dev->lock);

      if (tgt_fn_key == NULL)
	gomp_fatal ("target function wasn't mapped");

      tgt_fn = (void (*)) tgt_fn_key->tgt_offset;
    }
  else
    tgt_fn = (void (*)) fn;

  tgt = gomp_map_vars (acc_dev, mapnum, hostaddrs, NULL, sizes, kinds, true,
		       false);

  devaddrs = gomp_alloca (sizeof (void *) * mapnum);
  for (i = 0; i < mapnum; i++)
    {
      if (tgt->list[i] != NULL)
	devaddrs[i] = (void *) (tgt->list[i]->tgt->tgt_start
				+ tgt->list[i]->tgt_offset);
      else
	devaddrs[i] = NULL;
    }

  acc_dev->openacc.exec_func (tgt_fn, mapnum, hostaddrs, devaddrs, sizes, kinds,
			      num_gangs, num_workers, vector_length, async,
			      shared_size, tgt);

  /* If running synchronously, unmap immediately.  */
  if (async < acc_async_noval)
    gomp_unmap_vars (tgt, true);
  else
    {
      gomp_copy_from_async (tgt);
      acc_dev->openacc.register_async_cleanup_func (tgt);
    }

  acc_dev->openacc.async_set_async_func (acc_async_sync);

  if (acc_device_type (acc_dev->type) == acc_device_host_nonshm)
    free_host_shared_mem ();
}

void
GOACC_data_start (int device, size_t mapnum,
		  void **hostaddrs, size_t *sizes, unsigned short *kinds)
{
  bool host_fallback = device == GOMP_DEVICE_HOST_FALLBACK;
  struct target_mem_desc *tgt;
  int i;

#ifdef HAVE_INTTYPES_H
  gomp_debug (0, "%s: mapnum=%"PRIu64", hostaddrs=%p, sizes=%p, kinds=%p\n",
	      __FUNCTION__, (uint64_t) mapnum, hostaddrs, sizes, kinds);
#else
  gomp_debug (0, "%s: mapnum=%lu, hostaddrs=%p, sizes=%p, kinds=%p\n",
	      __FUNCTION__, (unsigned long) mapnum, hostaddrs, sizes, kinds);
#endif

  goacc_lazy_initialize ();

  struct goacc_thread *thr = goacc_thread ();
  struct gomp_device_descr *acc_dev = thr->dev;

  for (i = 0; i < (signed)(mapnum - 1); i++)
    {
      unsigned short kind1 = kinds[i] & 0xff;
      unsigned short kind2 = kinds[i+1] & 0xff;

      /* Handle Fortran deviceptr clause.  */
      if ((kind1 == GOMP_MAP_FORCE_DEVICEPTR && kind2 == GOMP_MAP_POINTER)
	   && (sizes[i + 1] == 0)
	   && (hostaddrs[i] == *(void **)hostaddrs[i + 1]))
	{
	  kinds[i+1] = kinds[i];
	  sizes[i+1] = sizeof (void *);
	  hostaddrs[i] = NULL;
	}
    }

  /* Host fallback or 'do nothing'.  */
  if ((acc_dev->capabilities & GOMP_OFFLOAD_CAP_SHARED_MEM)
      || host_fallback)
    {
      tgt = gomp_map_vars (NULL, 0, NULL, NULL, NULL, NULL, true, false);
      tgt->prev = thr->mapped_data;
      thr->mapped_data = tgt;

      return;
    }

  gomp_debug (0, "  %s: prepare mappings\n", __FUNCTION__);
  tgt = gomp_map_vars (acc_dev, mapnum, hostaddrs, NULL, sizes, kinds, true,
		       false);
  gomp_debug (0, "  %s: mappings prepared\n", __FUNCTION__);
  tgt->prev = thr->mapped_data;
  thr->mapped_data = tgt;
}

void
GOACC_data_end (void)
{
  struct goacc_thread *thr = goacc_thread ();
  struct target_mem_desc *tgt = thr->mapped_data;

  gomp_debug (0, "  %s: restore mappings\n", __FUNCTION__);
  thr->mapped_data = tgt->prev;
  gomp_unmap_vars (tgt, true);
  gomp_debug (0, "  %s: mappings restored\n", __FUNCTION__);
}

void
GOACC_enter_exit_data (int device, size_t mapnum,
		       void **hostaddrs, size_t *sizes, unsigned short *kinds,
		       int async, int num_waits, ...)
{
  struct goacc_thread *thr;
  struct gomp_device_descr *acc_dev;
  bool host_fallback = device == GOMP_DEVICE_HOST_FALLBACK;
  bool data_enter = false;
  size_t i;

  goacc_lazy_initialize ();

  thr = goacc_thread ();
  acc_dev = thr->dev;

  if ((acc_dev->capabilities & GOMP_OFFLOAD_CAP_SHARED_MEM)
      || host_fallback)
    return;

  if (num_waits > 0)
    {
      va_list ap;

      va_start (ap, num_waits);

      goacc_wait (async, num_waits, ap);

      va_end (ap);
    }

  acc_dev->openacc.async_set_async_func (async);

  /* Determine if this is an "acc enter data".  */
  for (i = 0; i < mapnum; ++i)
    {
      unsigned char kind = kinds[i] & 0xff;

      if (kind == GOMP_MAP_POINTER || kind == GOMP_MAP_TO_PSET)
	continue;

      if (kind == GOMP_MAP_FORCE_ALLOC
	  || kind == GOMP_MAP_FORCE_PRESENT
	  || kind == GOMP_MAP_FORCE_TO
	  || kind == GOMP_MAP_TO
	  || kind == GOMP_MAP_ALLOC)
	{
	  data_enter = true;
	  break;
	}

      if (kind == GOMP_MAP_FORCE_DEALLOC
	  || kind == GOMP_MAP_FORCE_FROM)
	break;

      gomp_fatal (">>>> GOACC_enter_exit_data UNHANDLED kind 0x%.2x",
		      kind);
    }

  /* In c, non-pointers and arrays are represented by a single data clause.
     Dynamically allocated arrays and subarrays are represented by a data
     clause followed by an internal GOMP_MAP_POINTER.

     In fortran, scalars and not allocated arrays are represented by a
     single data clause. Allocated arrays and subarrays have three mappings:
     1) the original data clause, 2) a PSET 3) a pointer to the array data.
  */

  if (data_enter)
    {
      for (i = 0; i < mapnum; i++)
	{
	  unsigned char kind = kinds[i] & 0xff;

	  /* Scan for pointers and PSETs.  */
	  int pointer = find_pointer (i, mapnum, kinds);

	  if (!pointer)
	    {
	      switch (kind)
		{
		case GOMP_MAP_ALLOC:
		  acc_present_or_create (hostaddrs[i], sizes[i]);
		  break;
		case GOMP_MAP_FORCE_ALLOC:
		  acc_create (hostaddrs[i], sizes[i]);
		  break;
		case GOMP_MAP_TO:
		  acc_present_or_copyin (hostaddrs[i], sizes[i]);
		  break;
		case GOMP_MAP_FORCE_TO:
		  acc_copyin (hostaddrs[i], sizes[i]);
		  break;
		default:
		  gomp_fatal (">>>> GOACC_enter_exit_data UNHANDLED kind 0x%.2x",
			      kind);
		  break;
		}
	    }
	  else
	    {
	      if (!acc_is_present (hostaddrs[i], sizes[i]))
		{
		  gomp_acc_insert_pointer (pointer, &hostaddrs[i],
					   &sizes[i], &kinds[i]);
		}
	      /* Increment 'i' by two because OpenACC requires fortran
		 arrays to be contiguous, so each PSET is associated with
		 one of MAP_FORCE_ALLOC/MAP_FORCE_PRESET/MAP_FORCE_TO, and
		 one MAP_POINTER.  */
	      i += pointer - 1;
	    }
	}
    }
  else
    for (i = 0; i < mapnum; ++i)
      {
	unsigned char kind = kinds[i] & 0xff;

	int pointer = find_pointer (i, mapnum, kinds);

	if (!pointer)
	  {
	    switch (kind)
	      {
	      case GOMP_MAP_FORCE_DEALLOC:
		if (acc_is_present (hostaddrs[i], sizes[i]))
		  acc_delete (hostaddrs[i], sizes[i]);
		else
		  i++;
		break;
	      case GOMP_MAP_FORCE_FROM:
		acc_copyout (hostaddrs[i], sizes[i]);
		break;
	      default:
		gomp_fatal (">>>> GOACC_enter_exit_data UNHANDLED kind 0x%.2x",
			    kind);
		break;
	      }
	  }
	else
	  {
	    if (acc_is_present (hostaddrs[i], sizes[i]))
	      {
		gomp_acc_remove_pointer (hostaddrs[i], (kinds[i] & 0xff)
					 == GOMP_MAP_FORCE_FROM, async,
					 pointer);
		/* See the above comment.  */
	      }
	    i += pointer - 1;
	  }
      }

  acc_dev->openacc.async_set_async_func (acc_async_sync);
}

static void
goacc_wait (int async, int num_waits, va_list ap)
{
  struct goacc_thread *thr = goacc_thread ();
  struct gomp_device_descr *acc_dev = thr->dev;
  int i;

  assert (num_waits >= 0);

  if (async == acc_async_sync && num_waits == 0)
    {
      acc_wait_all ();
      return;
    }

  if (async == acc_async_sync && num_waits)
    {
      for (i = 0; i < num_waits; i++)
        {
          int qid = va_arg (ap, int);

          if (acc_async_test (qid))
            continue;

          acc_wait (qid);
        }
      return;
    }

  if (async == acc_async_noval && num_waits == 0)
    {
      acc_dev->openacc.async_wait_all_async_func (acc_async_noval);
      return;
    }

  for (i = 0; i < num_waits; i++)
    {
      int qid = va_arg (ap, int);

      if (acc_async_test (qid))
	continue;

      /* If we're waiting on the same asynchronous queue as we're launching on,
         the queue itself will order work as required, so there's no need to
	 wait explicitly.  */
      if (qid != async)
	acc_dev->openacc.async_wait_async_func (qid, async);
    }
}

void
GOACC_update (int device, size_t mapnum,
	      void **hostaddrs, size_t *sizes, unsigned short *kinds,
	      int async, int num_waits, ...)
{
  bool host_fallback = device == GOMP_DEVICE_HOST_FALLBACK;
  size_t i;

  goacc_lazy_initialize ();

  struct goacc_thread *thr = goacc_thread ();
  struct gomp_device_descr *acc_dev = thr->dev;

  if ((acc_dev->capabilities & GOMP_OFFLOAD_CAP_SHARED_MEM)
      || host_fallback)
    return;

  if (num_waits > 0)
    {
      va_list ap;

      va_start (ap, num_waits);

      goacc_wait (async, num_waits, ap);

      va_end (ap);
    }

  acc_dev->openacc.async_set_async_func (async);

  for (i = 0; i < mapnum; ++i)
    {
      unsigned char kind = kinds[i] & 0xff;

      switch (kind)
	{
	case GOMP_MAP_POINTER:
	case GOMP_MAP_TO_PSET:
	  break;

	case GOMP_MAP_FORCE_TO:
	case GOMP_MAP_FORCE_TO_GANGLOCAL:
	  acc_update_device (hostaddrs[i], sizes[i]);
	  break;

	case GOMP_MAP_FORCE_FROM:
	  acc_update_self (hostaddrs[i], sizes[i]);
	  break;

	default:
	  gomp_fatal (">>>> GOACC_update UNHANDLED kind 0x%.2x", kind);
	  break;
	}
    }

  acc_dev->openacc.async_set_async_func (acc_async_sync);
}

void
GOACC_wait (int async, int num_waits, ...)
{
  va_list ap;

  va_start (ap, num_waits);

  goacc_wait (async, num_waits, ap);

  va_end (ap);
}

int
GOACC_get_num_threads (int gang, int worker, int vector)
{
  return 1;
}

int
GOACC_get_thread_num (int gang, int worker, int vector)
{
  return 0;
}

void
GOACC_register_static (void *addr, int size, unsigned int mask)
{
  struct oacc_static *s;

  s = (struct oacc_static *) malloc (sizeof (struct oacc_static));
  s->addr = addr;
  s->size = (size_t) size;
  s->mask = mask;
  s->free = false;
  s->next = NULL;

  if (oacc_statics)
    s->next = oacc_statics;

   oacc_statics = s;
}

void
GOACC_declare (int device, size_t mapnum,
	       void **hostaddrs, size_t *sizes, unsigned short *kinds)
{
  int i;

  for (i = 0; i < mapnum; i++)
    {
      unsigned char kind = kinds[i] & 0xff;

      if (kind == GOMP_MAP_POINTER || kind == GOMP_MAP_TO_PSET)
	continue;

      switch (kind)
	{
	  case GOMP_MAP_FORCE_ALLOC:
	  case GOMP_MAP_FORCE_DEALLOC:
	  case GOMP_MAP_FORCE_FROM:
	  case GOMP_MAP_FORCE_TO:
	  case GOMP_MAP_POINTER:
	    GOACC_enter_exit_data (device, 1, &hostaddrs[i], &sizes[i],
				   &kinds[i], 0, 0);
	    break;

	  case GOMP_MAP_FORCE_DEVICEPTR:
	    break;

	  case GOMP_MAP_ALLOC:
	    if (!acc_is_present (hostaddrs[i], sizes[i]))
	      {
		GOACC_enter_exit_data (device, 1, &hostaddrs[i], &sizes[i],
				       &kinds[i], 0, 0);
	      }
	    break;

	  case GOMP_MAP_TO:
	    GOACC_enter_exit_data (device, 1, &hostaddrs[i], &sizes[i],
				   &kinds[i], 0, 0);

	    break;

	  case GOMP_MAP_FROM:
	    kinds[i] = GOMP_MAP_FORCE_FROM;
	    GOACC_enter_exit_data (device, 1, &hostaddrs[i], &sizes[i],
				       &kinds[i], 0, 0);
	    break;

	  case GOMP_MAP_FORCE_PRESENT:
	    if (!acc_is_present (hostaddrs[i], sizes[i]))
	      gomp_fatal ("[%p,%zd] is not mapped", hostaddrs[i], sizes[i]);
	    break;

	  default:
	    assert (0);
	    break;
	}
    }
}
