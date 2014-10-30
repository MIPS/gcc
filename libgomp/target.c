/* Copyright (C) 2013-2014 Free Software Foundation, Inc.
   Contributed by Jakub Jelinek <jakub@redhat.com>.

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

/* This file handles the maintainence of threads in response to team
   creation and termination.  */

#include "libgomp.h"
#include "oacc-plugin.h"
#include "gomp-constants.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifdef PLUGIN_SUPPORT
# include <dlfcn.h>
# include <dirent.h>
#endif

static void gomp_target_init (void);

static pthread_once_t gomp_is_initialized = PTHREAD_ONCE_INIT;

#include "splay-tree.h"

/* This structure describes an offload image.
   It contains type of the target, pointer to host table descriptor, and pointer
   to target data.  */
struct offload_image_descr {
  int type;
  void *host_table;
  void *target_data;
};

/* Array of descriptors of offload images.  */
static struct offload_image_descr *offload_images;

/* Total number of offload images.  */
static int num_offload_images;

/* Array of descriptors of all available devices.  */
static struct gomp_device_descr *devices;

/* Total number of available devices.  */
static int num_devices;

/* The comparison function.  */

attribute_hidden int
splay_compare (splay_tree_key x, splay_tree_key y)
{
  if (x->host_start == x->host_end
      && y->host_start == y->host_end)
    return 0;
  if (x->host_end <= y->host_start)
    return -1;
  if (x->host_start >= y->host_end)
    return 1;
  return 0;
}

#include "target.h"
#include "oacc-int.h"

attribute_hidden void
gomp_init_targets_once (void)
{
  (void) pthread_once (&gomp_is_initialized, gomp_target_init);
}

attribute_hidden int
gomp_get_num_devices (void)
{
  gomp_init_targets_once ();
  return num_devices;
}

static struct gomp_device_descr *
resolve_device (int device_id)
{
  if (device_id == -1)
    {
      struct gomp_task_icv *icv = gomp_icv (false);
      device_id = icv->default_device_var;
    }
  if (device_id < 0
      || device_id >= gomp_get_num_devices ())
    return NULL;

  return &devices[device_id];
}

/* Handle the case where splay_tree_lookup found oldn for newn.
   Helper function of gomp_map_vars.  */

static inline void
gomp_map_vars_existing (splay_tree_key oldn, splay_tree_key newn,
			unsigned char kind)
{
  if (oldn->host_start > newn->host_start
      || oldn->host_end < newn->host_end)
    gomp_fatal ("Trying to map into device [%p..%p) object when"
		"[%p..%p) is already mapped",
		(void *) newn->host_start, (void *) newn->host_end,
		(void *) oldn->host_start, (void *) oldn->host_end);
  oldn->refcount++;
}

static int
get_kind (bool is_openacc, void *kinds, int idx)
{
  return is_openacc ? ((unsigned short *) kinds)[idx]
		    : ((unsigned char *) kinds)[idx];
}

attribute_hidden struct target_mem_desc *
gomp_map_vars (struct gomp_device_descr *devicep, size_t mapnum,
	       void **hostaddrs, void **devaddrs, size_t *sizes, void *kinds,
	       bool is_openacc, bool is_target)
{
  size_t i, tgt_align, tgt_size, not_found_cnt = 0;
  const int rshift = is_openacc ? 8 : 3;
  const int typemask = is_openacc ? 0xff : 0x7;
  struct gomp_memory_mapping *mm = &devicep->mem_map;
  struct splay_tree_key_s cur_node;
  struct target_mem_desc *tgt
    = gomp_malloc (sizeof (*tgt) + sizeof (tgt->list[0]) * mapnum);
  tgt->list_count = mapnum;
  tgt->refcount = 1;
  tgt->device_descr = devicep;
  tgt->mem_map = mm;

  /* From gcc/fortran/trans-types.c  */
  struct descriptor_dimension
    {
      long stride;
      long lbound;
      long ubound;
    };

   struct gfc_array_descriptor
     {
       void *data;
       long offset;
       long dtype;
       struct descriptor_dimension dimension[];
     };

#define GFC_DTYPE_RANK_MASK     0x07
#define GFC_DTYPE_TYPE_MASK     0x38
#define GFC_DTYPE_TYPE_SHIFT    3
#define GFC_DTYPE_SIZE_SHIFT    6

  if (mapnum == 0)
    return tgt;

  tgt_align = sizeof (void *);
  tgt_size = 0;
  if (is_target)
    {
      size_t align = 4 * sizeof (void *);
      tgt_align = align;
      tgt_size = mapnum * sizeof (void *);
    }
  gomp_mutex_lock (&mm->lock);
  for (i = 0; i < mapnum; i++)
    {
      int kind = get_kind (is_openacc, kinds, i);
      if (hostaddrs[i] == NULL)
	{
	  tgt->list[i] = NULL;
	  continue;
	}
      cur_node.host_start = (uintptr_t) hostaddrs[i];
      if (!GOMP_MAP_POINTER_P (kind & typemask))
	cur_node.host_end = cur_node.host_start + sizes[i];
      else
	cur_node.host_end = cur_node.host_start + sizeof (void *);
      splay_tree_key n = splay_tree_lookup (&mm->splay_tree, &cur_node);
      if (n)
	{
	  tgt->list[i] = n;
	  gomp_map_vars_existing (n, &cur_node, kind);
	}
      else
	{
	  tgt->list[i] = NULL;

	  if ((kind & typemask) == GOMP_MAP_TO_PSET)
	    {
	      struct gfc_array_descriptor *gad;
	      size_t rank;
	      int j;
              bool alloc_arrays = true;

	      for (j = i - 1; j >= 0; j--)
		{
		  if (hostaddrs[j] == *(void**)hostaddrs[i])
		    {
		      alloc_arrays = false;
		      break;
		    }
		}

	      gad = (struct gfc_array_descriptor *) cur_node.host_start;
	      rank = gad->dtype & GFC_DTYPE_RANK_MASK;

	      cur_node.host_start = (uintptr_t)gad->data;
	      cur_node.host_end = cur_node.host_start +
				sizeof (struct gfc_array_descriptor) +
				(sizeof (struct descriptor_dimension) * rank);

	      if (alloc_arrays)
                {
                  size_t tsize;

                  tsize = gad->dtype >> GFC_DTYPE_SIZE_SHIFT;

                  for (j = 0; j < rank; j++)
                    {
                      cur_node.host_end += tsize *
                        (gad->dimension[j].ubound -
                         gad->dimension[j].lbound + 1);
                    }
                }
	    }

	  size_t align = (size_t) 1 << (kind >> rshift);
	  not_found_cnt++;
	  if (tgt_align < align)
	    tgt_align = align;
	  tgt_size = (tgt_size + align - 1) & ~(align - 1);
	  tgt_size += cur_node.host_end - cur_node.host_start;
	  if ((kind & typemask) == GOMP_MAP_TO_PSET)
	    {
	      size_t j;
	      for (j = i + 1; j < mapnum; j++)
		if (!GOMP_MAP_POINTER_P (get_kind (is_openacc, kinds, j)
					 & typemask))
		  break;
		else if ((uintptr_t) hostaddrs[j] < cur_node.host_start
			 || ((uintptr_t) hostaddrs[j] + sizeof (void *)
			     > cur_node.host_end))
		  break;
		else
		  {
		    tgt->list[j] = NULL;
		    i++;
		  }
	    }
	}
    }

  if (devaddrs)
    {
      if (mapnum != 1)
        gomp_fatal ("unexpected aggregation");
      tgt->to_free = devaddrs[0];
      tgt->tgt_start = (uintptr_t) tgt->to_free;
      tgt->tgt_end = tgt->tgt_start + sizes[0];
    }
  else if (not_found_cnt || is_target)
    {
      /* Allocate tgt_align aligned tgt_size block of memory.  */
      /* FIXME: Perhaps change interface to allocate properly aligned
	 memory.  */
      tgt->to_free = devicep->device_alloc_func (tgt_size + tgt_align - 1);
      tgt->tgt_start = (uintptr_t) tgt->to_free;
      tgt->tgt_start = (tgt->tgt_start + tgt_align - 1) & ~(tgt_align - 1);
      tgt->tgt_end = tgt->tgt_start + tgt_size;
    }
  else
    {
      tgt->to_free = NULL;
      tgt->tgt_start = 0;
      tgt->tgt_end = 0;
    }

  tgt_size = 0;
  if (is_target)
    tgt_size = mapnum * sizeof (void *);

  tgt->array = NULL;
  if (not_found_cnt)
    {
      tgt->array = gomp_malloc (not_found_cnt * sizeof (*tgt->array));
      splay_tree_node array = tgt->array;
      size_t j;

      for (i = 0; i < mapnum; i++)
	if (tgt->list[i] == NULL)
	  {
	    int kind = get_kind (is_openacc, kinds, i);
	    if (hostaddrs[i] == NULL)
	      continue;
	    splay_tree_key k = &array->key;
	    k->host_start = (uintptr_t) hostaddrs[i];
	    if (!GOMP_MAP_POINTER_P (kind & typemask))
	      k->host_end = k->host_start + sizes[i];
	    else
	      k->host_end = k->host_start + sizeof (void *);
	    splay_tree_key n = splay_tree_lookup (&mm->splay_tree, k);
	    if (n)
	      {
		tgt->list[i] = n;
		gomp_map_vars_existing (n, k, kind);
	      }
	    else
	      {
		size_t align = (size_t) 1 << (kind >> rshift);
		tgt->list[i] = k;
		tgt_size = (tgt_size + align - 1) & ~(align - 1);
		k->tgt = tgt;
		k->tgt_offset = tgt_size;
		tgt_size += k->host_end - k->host_start;
		k->copy_from = GOMP_MAP_COPYFROM_P (kind & typemask)
			       || GOMP_MAP_TOFROM_P (kind & typemask);
		k->refcount = 1;
		k->async_refcount = 0;
		tgt->refcount++;
		array->left = NULL;
		array->right = NULL;

		splay_tree_insert (&mm->splay_tree, array);

		switch (kind & typemask)
		  {
		  case GOMP_MAP_FORCE_ALLOC:
		  case GOMP_MAP_FORCE_FROM:
		    /* FIXME: No special handling (see comment in
		       oacc-parallel.c).  */
		  case GOMP_MAP_ALLOC:
		  case GOMP_MAP_ALLOC_FROM:
		    break;
		  case GOMP_MAP_FORCE_TO:
		  case GOMP_MAP_FORCE_TOFROM:
		    /* FIXME: No special handling, as above.  */
		  case GOMP_MAP_ALLOC_TO:
		  case GOMP_MAP_ALLOC_TOFROM:
		    /* Copy from host to device memory.  */
		    /* FIXME: Perhaps add some smarts, like if copying
		       several adjacent fields from host to target, use some
		       host buffer to avoid sending each var individually.  */
		    devicep->device_host2dev_func
		      ((void *) (tgt->tgt_start + k->tgt_offset),
		       (void *) k->host_start,
		       k->host_end - k->host_start);
		    break;
		  case GOMP_MAP_POINTER:
		    cur_node.host_start
		      = (uintptr_t) *(void **) k->host_start;
		    if (cur_node.host_start == (uintptr_t) NULL)
		      {
			cur_node.tgt_offset = (uintptr_t) NULL;
			/* Copy from host to device memory.  */
			/* FIXME: see above FIXME comment.  */
			devicep->device_host2dev_func
			  ((void *) (tgt->tgt_start + k->tgt_offset),
			   (void *) &cur_node.tgt_offset,
			   sizeof (void *));
			break;
		      }
		    /* Add bias to the pointer value.  */
		    cur_node.host_start += sizes[i];
		    cur_node.host_end = cur_node.host_start + 1;
		    n = splay_tree_lookup (&mm->splay_tree, &cur_node);
		    if (n == NULL)
		      {
			/* Could be possibly zero size array section.  */
			cur_node.host_end--;
			n = splay_tree_lookup (&mm->splay_tree, &cur_node);
			if (n == NULL)
			  {
			    cur_node.host_start--;
			    n = splay_tree_lookup (&mm->splay_tree, &cur_node);
			    cur_node.host_start++;
			  }
		      }
		    if (n == NULL)
		      gomp_fatal ("Pointer target of array section "
				  "wasn't mapped");

		    cur_node.host_start -= n->host_start;
		    cur_node.tgt_offset = n->tgt->tgt_start + n->tgt_offset
					  + cur_node.host_start;
		    /* At this point tgt_offset is target address of the
		       array section.  Now subtract bias to get what we want
		       to initialize the pointer with.  */
		    cur_node.tgt_offset -= sizes[i];
		    /* Copy from host to device memory.  */
		    /* FIXME: see above FIXME comment.  */
		    devicep->device_host2dev_func
		      ((void *) (tgt->tgt_start + k->tgt_offset),
		       (void *) &cur_node.tgt_offset,
		       sizeof (void *));
		    break;
		  case GOMP_MAP_TO_PSET:
		    {
		      /* Copy from host to device memory.  */
		      /* FIXME: see above FIXME comment.  */
		      devicep->device_host2dev_func
				((void *) (tgt->tgt_start + k->tgt_offset),
				(void *) k->host_start,
				(k->host_end - k->host_start));
		      devicep->device_host2dev_func
				((void *) (tgt->tgt_start + k->tgt_offset),
				(void *) &tgt->tgt_start,
				sizeof (void *));

		      for (j = i + 1; j < mapnum; j++)
			if (!GOMP_MAP_POINTER_P (get_kind (is_openacc, kinds, j)
					       & typemask))
			  break;
			else if ((uintptr_t) hostaddrs[j] < k->host_start
			       || ((uintptr_t) hostaddrs[j] + sizeof (void *)
				   > k->host_end))
			  break;
			else
			  {
			    tgt->list[j] = k;
			    k->refcount++;
			    cur_node.host_start
			      = (uintptr_t) *(void **) hostaddrs[j];
			    if (cur_node.host_start == (uintptr_t) NULL)
			      {
			        cur_node.tgt_offset = (uintptr_t) NULL;
			        /* Copy from host to device memory.  */
			        /* FIXME: see above FIXME comment.  */
			        devicep->device_host2dev_func
				  ((void *) (tgt->tgt_start + k->tgt_offset
					   + ((uintptr_t) hostaddrs[j]
					      - k->host_start)),
				   (void *) &cur_node.tgt_offset,
				   sizeof (void *));
			        i++;
			        continue;
			      }
			    /* Add bias to the pointer value.  */
			    cur_node.host_start += sizes[j];
			    cur_node.host_end = cur_node.host_start + 1;
			    n = splay_tree_lookup (&mm->splay_tree, &cur_node);
			    if (n == NULL)
			      {
			        /* Could be possibly zero size array
				   section.  */
			        cur_node.host_end--;
			        n = splay_tree_lookup (&mm->splay_tree,
						     &cur_node);
			        if (n == NULL)
				  {
				    cur_node.host_start--;
				    n = splay_tree_lookup (&mm->splay_tree,
							 &cur_node);
				    cur_node.host_start++;
				  }
			      }
			    if (n == NULL)
				gomp_fatal ("Pointer target of array section "
					"wasn't mapped");
			    cur_node.host_start -= n->host_start;
			    cur_node.tgt_offset = n->tgt->tgt_start
						+ n->tgt_offset
						+ cur_node.host_start;
			    /* At this point tgt_offset is target address of the
			       array section.  Now subtract bias to get what we
			       want to initialize the pointer with.  */
			    cur_node.tgt_offset -= sizes[j];
			    /* Copy from host to device memory.  */
			    /* FIXME: see above FIXME comment.  */

			    devicep->device_host2dev_func
				((void *) (tgt->tgt_start + k->tgt_offset
				       + ((uintptr_t) hostaddrs[j]
					  - k->host_start)),
				(void *) &cur_node.tgt_offset,
				sizeof (void *));
			    i++;
			  }
		      break;
		      }
		    case GOMP_MAP_FORCE_PRESENT:
		      {
		        /* We already looked up the memory region above and it
			   was missing.  */
			size_t size = k->host_end - k->host_start;
			gomp_fatal ("present clause: !acc_is_present (%p, "
				    "%zd (0x%zx))", (void *) k->host_start,
				    size, size);
		      }
		      break;
		    case GOMP_MAP_FORCE_DEVICEPTR:
		      assert (k->host_end - k->host_start == sizeof (void *));
		      
		      devicep->device_host2dev_func
		        ((void *) (tgt->tgt_start + k->tgt_offset),
			 (void *) k->host_start,
			 sizeof (void *));
		      break;
		    case GOMP_MAP_FORCE_PRIVATE:
		      abort ();
		    case GOMP_MAP_FORCE_FIRSTPRIVATE:
		      abort ();
		    default:
		      gomp_fatal ("%s: unhandled kind 0x%.2x", __FUNCTION__,
				  kind);
		  }
		array++;
	      }
	  }
    }

#undef GFC_DTYPE_RANK_MASK
#undef GFC_DTYPE_TYPE_MASK
#undef GFC_DTYPE_TYPE_SHIFT
#undef GFC_DTYPE_SIZE_SHIFT
	
  if (is_target)
    {
      for (i = 0; i < mapnum; i++)
	{
	  if (tgt->list[i] == NULL)
	    cur_node.tgt_offset = (uintptr_t) NULL;
	  else
	    cur_node.tgt_offset = tgt->list[i]->tgt->tgt_start
				  + tgt->list[i]->tgt_offset;
	  /* Copy from host to device memory.  */
	  /* FIXME: see above FIXME comment.  */
	  devicep->device_host2dev_func
	    ((void *) (tgt->tgt_start + i * sizeof (void *)),
	     (void *) &cur_node.tgt_offset,
	     sizeof (void *));
	}
    }

  gomp_mutex_unlock (&mm->lock);
  return tgt;
}

static void
gomp_unmap_tgt (struct target_mem_desc *tgt)
{
  /* Deallocate on target the tgt->tgt_start .. tgt->tgt_end region.  */
  if (tgt->tgt_end)
    tgt->device_descr->device_free_func(tgt->to_free);

  free (tgt->array);
  free (tgt);
}

/* Decrease the refcount for a set of mapped variables, and queue asychronous
   copies from the device back to the host after any work that has been issued. 
   Because the regions are still "live", increment an asynchronous reference
   count to indicate that they should not be unmapped from host-side data
   structures until the asynchronous copy has completed.  */

attribute_hidden void
gomp_copy_from_async (struct target_mem_desc *tgt)
{
  struct gomp_device_descr *devicep = tgt->device_descr;
  struct gomp_memory_mapping *mm = tgt->mem_map;
  size_t i;
  
  gomp_mutex_lock (&mm->lock);

  for (i = 0; i < tgt->list_count; i++)
    if (tgt->list[i] == NULL)
      ;
    else if (tgt->list[i]->refcount > 1)
      {
	tgt->list[i]->refcount--;
	tgt->list[i]->async_refcount++;
      }
    else
      {
	splay_tree_key k = tgt->list[i];
	if (k->copy_from)
	  /* Copy from device to host memory.  */
	  devicep->device_dev2host_func
	    ((void *) k->host_start,
	     (void *) (k->tgt->tgt_start + k->tgt_offset),
	     k->host_end - k->host_start);
      }

  gomp_mutex_unlock (&mm->lock);
}

/* Unmap variables described by TGT.  If DO_COPYFROM is true, copy relevant
   variables back from device to host: if it is false, it is assumed that this
   has been done already, i.e. by gomp_copy_from_async above.  */

attribute_hidden void
gomp_unmap_vars (struct target_mem_desc *tgt, bool do_copyfrom)
{
  struct gomp_device_descr *devicep = tgt->device_descr;
  struct gomp_memory_mapping *mm = tgt->mem_map;

  if (tgt->list_count == 0)
    {
      free (tgt);
      return;
    }

  size_t i;
  gomp_mutex_lock (&mm->lock);
  for (i = 0; i < tgt->list_count; i++)
    if (tgt->list[i] == NULL)
      ;
    else if (tgt->list[i]->refcount > 1)
      tgt->list[i]->refcount--;
    else if (tgt->list[i]->async_refcount > 0)
      tgt->list[i]->async_refcount--;
    else
      {
	splay_tree_key k = tgt->list[i];
	if (k->copy_from && do_copyfrom)
	  /* Copy from device to host memory.  */
	  devicep->device_dev2host_func
	    ((void *) k->host_start,
	     (void *) (k->tgt->tgt_start + k->tgt_offset),
	     k->host_end - k->host_start);
	splay_tree_remove (&mm->splay_tree, k);
	if (k->tgt->refcount > 1)
	  k->tgt->refcount--;
	else
	  gomp_unmap_tgt (k->tgt);
      }

  if (tgt->refcount > 1)
    tgt->refcount--;
  else
    gomp_unmap_tgt (tgt);
  gomp_mutex_unlock (&mm->lock);
}

static void
gomp_update (struct gomp_device_descr *devicep, struct gomp_memory_mapping *mm,
	     size_t mapnum, void **hostaddrs, size_t *sizes, void *kinds,
	     bool is_openacc)
{
  size_t i;
  struct splay_tree_key_s cur_node;
  const int typemask = is_openacc ? 0xff : 0x7;

  if (!devicep)
    return;

  if (mapnum == 0)
    return;

  gomp_mutex_lock (&mm->lock);
  for (i = 0; i < mapnum; i++)
    if (sizes[i])
      {
	cur_node.host_start = (uintptr_t) hostaddrs[i];
	cur_node.host_end = cur_node.host_start + sizes[i];
	splay_tree_key n = splay_tree_lookup (&mm->splay_tree,
					      &cur_node);
	if (n)
	  {
	    int kind = get_kind (is_openacc, kinds, i);
	    if (n->host_start > cur_node.host_start
		|| n->host_end < cur_node.host_end)
	      gomp_fatal ("Trying to update [%p..%p) object when"
			  "only [%p..%p) is mapped",
			  (void *) cur_node.host_start,
			  (void *) cur_node.host_end,
			  (void *) n->host_start,
			  (void *) n->host_end);
	    if (GOMP_MAP_COPYTO_P (kind & typemask))
	      /* Copy from host to device memory.  */
	      devicep->device_host2dev_func
		((void *) (n->tgt->tgt_start
			   + n->tgt_offset
			   + cur_node.host_start
			   - n->host_start),
		 (void *) cur_node.host_start,
		 cur_node.host_end - cur_node.host_start);
	    else if (GOMP_MAP_COPYFROM_P (kind & typemask))
	      /* Copy from device to host memory.  */
	      devicep->device_dev2host_func
		((void *) cur_node.host_start,
		 (void *) (n->tgt->tgt_start
			   + n->tgt_offset
			   + cur_node.host_start
			   - n->host_start),
		 cur_node.host_end - cur_node.host_start);
	  }
	else
	  gomp_fatal ("Trying to update [%p..%p) object that is not mapped",
		      (void *) cur_node.host_start,
		      (void *) cur_node.host_end);
      }
  gomp_mutex_unlock (&mm->lock);
}

static void gomp_register_image_for_device (struct gomp_device_descr *device,
					    struct offload_image_descr *image);

/* This function should be called from every offload image.  It gets the
   descriptor of the host func and var tables HOST_TABLE, TYPE of the target,
   and TARGET_DATA needed by target plugin (target tables, etc.)  */
void
GOMP_offload_register (void *host_table, int type, void **target_data)
{
  offload_images = gomp_realloc (offload_images,
				 (num_offload_images + 1)
				 * sizeof (struct offload_image_descr));

  if (offload_images == NULL)
    return;

  offload_images[num_offload_images].type = type;
  offload_images[num_offload_images].host_table = host_table;
  offload_images[num_offload_images].target_data = target_data;

  num_offload_images++;
}

attribute_hidden void
gomp_init_device (struct gomp_device_descr *devicep)
{
  /* Initialize the target device.  */
  devicep->device_init_func ();

  devicep->is_initialized = true;
}

attribute_hidden void
gomp_init_tables (const struct gomp_device_descr *devicep,
		  struct gomp_memory_mapping *mm)
{
  /* Get address mapping table for device.  */
  struct mapping_table *table = NULL;
  int i, num_entries = devicep->device_get_table_func (&table);

  /* Insert host-target address mapping into dev_splay_tree.  */
  for (i = 0; i < num_entries; i++)
    {
      struct target_mem_desc *tgt = gomp_malloc (sizeof (*tgt));
      tgt->refcount = 1;
      tgt->array = gomp_malloc (sizeof (*tgt->array));
      tgt->tgt_start = table[i].tgt_start;
      tgt->tgt_end = table[i].tgt_end;
      tgt->to_free = NULL;
      tgt->list_count = 0;
      tgt->device_descr = (struct gomp_device_descr *) devicep;
      splay_tree_node node = tgt->array;
      splay_tree_key k = &node->key;
      k->host_start = table[i].host_start;
      k->host_end = table[i].host_end;
      k->tgt_offset = 0;
      k->tgt = tgt;
      node->left = NULL;
      node->right = NULL;
      splay_tree_insert (&mm->splay_tree, node);
    }

  free (table);
  
  mm->is_initialized = true;
}

static void
gomp_init_dev_tables (struct gomp_device_descr *devicep)
{
  gomp_init_device (devicep);
  gomp_init_tables (devicep, &devicep->mem_map);
}

attribute_hidden void
gomp_free_memmap (struct gomp_device_descr *devicep)
{
  struct gomp_memory_mapping *mm = &devicep->mem_map;

  while (mm->splay_tree.root)
    {
      struct target_mem_desc *tgt = mm->splay_tree.root->key.tgt;
      
      splay_tree_remove (&mm->splay_tree, &mm->splay_tree.root->key);
      free (tgt->array);
      free (tgt);
    }

  mm->is_initialized = false;
}

attribute_hidden void
gomp_fini_device (struct gomp_device_descr *devicep)
{
  if (devicep->is_initialized)
    devicep->device_fini_func ();

  devicep->is_initialized = false;
}

/* Called when encountering a target directive.  If DEVICE
   is -1, it means use device-var ICV.  If it is -2 (or any other value
   larger than last available hw device, use host fallback.
   FN is address of host code, OPENMP_TARGET contains value of the
   __OPENMP_TARGET__ symbol in the shared library or binary that invokes
   GOMP_target.  HOSTADDRS, SIZES and KINDS are arrays
   with MAPNUM entries, with addresses of the host objects,
   sizes of the host objects (resp. for pointer kind pointer bias
   and assumed sizeof (void *) size) and kinds.  */

void
GOMP_target (int device, void (*fn) (void *), const void *openmp_target,
	     size_t mapnum, void **hostaddrs, size_t *sizes,
	     unsigned char *kinds)
{
  struct gomp_device_descr *devicep = resolve_device (device);
  struct gomp_memory_mapping *mm = &devicep->mem_map;

  if (devicep != NULL && !devicep->is_initialized)
    gomp_init_dev_tables (devicep);

  if (devicep == NULL || !(devicep->capabilities & TARGET_CAP_OPENMP_400))
    {
      /* Host fallback.  */
      struct gomp_thread old_thr, *thr = gomp_thread ();
      old_thr = *thr;
      memset (thr, '\0', sizeof (*thr));
      if (gomp_places_list)
	{
	  thr->place = old_thr.place;
	  thr->ts.place_partition_len = gomp_places_list_len;
	}
      fn (hostaddrs);
      gomp_free_thread (thr);
      *thr = old_thr;
      return;
    }

  struct splay_tree_key_s k;
  k.host_start = (uintptr_t) fn;
  k.host_end = k.host_start + 1;
  gomp_mutex_lock (&mm->lock);
  splay_tree_key tgt_fn = splay_tree_lookup (&devicep->mem_map.splay_tree, &k);
  if (tgt_fn == NULL && !(devicep->capabilities & TARGET_CAP_NATIVE_EXEC))
    gomp_fatal ("Target function wasn't mapped");
  gomp_mutex_unlock (&mm->lock);

  struct target_mem_desc *tgt_vars
    = gomp_map_vars (devicep, mapnum, hostaddrs, NULL, sizes, kinds, false,
		     true);
  struct gomp_thread old_thr, *thr = gomp_thread ();
  old_thr = *thr;
  memset (thr, '\0', sizeof (*thr));
  if (gomp_places_list)
    {
      thr->place = old_thr.place;
      thr->ts.place_partition_len = gomp_places_list_len;
    }
  if (devicep->capabilities & TARGET_CAP_NATIVE_EXEC)
    devicep->device_run_func (fn, (void *) tgt_vars->tgt_start);
  else
    devicep->device_run_func ((void *) tgt_fn->tgt->tgt_start,
			      (void *) tgt_vars->tgt_start);
  gomp_free_thread (thr);
  *thr = old_thr;
  gomp_unmap_vars (tgt_vars, true);
}

void
GOMP_target_data (int device, const void *openmp_target, size_t mapnum,
		  void **hostaddrs, size_t *sizes, unsigned char *kinds)
{
  struct gomp_device_descr *devicep = resolve_device (device);

  if (devicep != NULL && !devicep->is_initialized)
    gomp_init_dev_tables (devicep);

  if (devicep == NULL || !(devicep->capabilities & TARGET_CAP_OPENMP_400))
    {
      /* Host fallback.  */
      struct gomp_task_icv *icv = gomp_icv (false);
      if (icv->target_data)
	{
	  /* Even when doing a host fallback, if there are any active
	     #pragma omp target data constructs, need to remember the
	     new #pragma omp target data, otherwise GOMP_target_end_data
	     would get out of sync.  */
	  struct target_mem_desc *tgt
	    = gomp_map_vars (NULL, 0, NULL, NULL, NULL, NULL, false, false);
	  tgt->prev = icv->target_data;
	  icv->target_data = tgt;
	}
      return;
    }

  struct target_mem_desc *tgt = gomp_map_vars (devicep, mapnum, hostaddrs, NULL,
					       sizes, kinds, false, false);
  struct gomp_task_icv *icv = gomp_icv (true);
  tgt->prev = icv->target_data;
  icv->target_data = tgt;
}

void
GOMP_target_end_data (void)
{
  struct gomp_task_icv *icv = gomp_icv (false);
  if (icv->target_data)
    {
      struct target_mem_desc *tgt = icv->target_data;
      icv->target_data = tgt->prev;
      gomp_unmap_vars (tgt, true);
    }
}

void
GOMP_target_update (int device, const void *openmp_target, size_t mapnum,
		    void **hostaddrs, size_t *sizes, unsigned char *kinds)
{
  struct gomp_device_descr *devicep = resolve_device (device);

  if (devicep != NULL && !devicep->is_initialized)
    gomp_init_dev_tables (devicep);

  if (devicep == NULL || !(devicep->capabilities & TARGET_CAP_OPENMP_400))
    return;

  gomp_update (devicep, &devicep->mem_map, mapnum, hostaddrs, sizes, kinds,
	       false);
}

void
GOMP_teams (unsigned int num_teams, unsigned int thread_limit)
{
  if (thread_limit)
    {
      struct gomp_task_icv *icv = gomp_icv (true);
      icv->thread_limit_var
	= thread_limit > INT_MAX ? UINT_MAX : thread_limit;
    }
  (void) num_teams;
}

#ifdef PLUGIN_SUPPORT

/* This function checks if the given string FNAME matches
   "libgomp-plugin-*.so.1".  */
static bool
gomp_check_plugin_file_name (const char *fname)
{
  const char *prefix = "libgomp-plugin-";
  const char *suffix = ".so.1";
  if (!fname)
    return false;
  if (strncmp (fname, prefix, strlen (prefix)) != 0)
    return false;
  if (strncmp (fname + strlen (fname) - strlen (suffix), suffix,
	       strlen (suffix)) != 0)
    return false;
  return true;
}

/* This function tries to load plugin for DEVICE.  Name of plugin is passed
   in PLUGIN_NAME.
   Plugin handle and handles of the found functions are stored in the
   corresponding fields of DEVICE.
   The function returns TRUE on success and FALSE otherwise.  */
static bool
gomp_load_plugin_for_device (struct gomp_device_descr *device,
			     const char *plugin_name)
{
  char *err = NULL, *last_missing = NULL;
  int optional_present, optional_total;

  /* Clear any existing error.  */
  dlerror ();

  device->plugin_handle = dlopen (plugin_name, RTLD_LAZY);
  if (!device->plugin_handle)
    {
      err = dlerror ();
      goto out;
    }

  /* Check if all required functions are available in the plugin and store
     their handlers.  */
#define DLSYM(f) \
  do									\
    {									\
      device->f##_func = dlsym (device->plugin_handle, #f);		\
      err = dlerror ();							\
      if (err != NULL)							\
	goto out;							\
    }									\
  while (0)
  /* Similar, but missing functions are not an error.  */
#define DLSYM_OPT(f,n) \
  do									\
    {									\
      char *tmp_err;							\
      device->f##_func = dlsym (device->plugin_handle, #n);		\
      tmp_err = dlerror ();						\
      if (tmp_err == NULL)						\
        optional_present++;						\
      else								\
        last_missing = #n;						\
      optional_total++;							\
    }									\
  while (0)

  DLSYM (get_name);
  DLSYM (get_caps);
  DLSYM (get_type);
  DLSYM (get_num_devices);
  DLSYM (offload_register);
  DLSYM (device_init);
  DLSYM (device_fini);
  DLSYM (device_get_table);
  DLSYM (device_alloc);
  DLSYM (device_free);
  DLSYM (device_dev2host);
  DLSYM (device_host2dev);
  device->capabilities = device->get_caps_func ();
  if (device->capabilities & TARGET_CAP_OPENMP_400)
    DLSYM (device_run);
  if (device->capabilities & TARGET_CAP_OPENACC_200)
    {
      optional_present = optional_total = 0;
      DLSYM_OPT (openacc.exec, openacc_parallel);
      DLSYM_OPT (openacc.open_device, openacc_open_device);
      DLSYM_OPT (openacc.close_device, openacc_close_device);
      DLSYM_OPT (openacc.get_device_num, openacc_get_device_num);
      DLSYM_OPT (openacc.set_device_num, openacc_set_device_num);
      DLSYM_OPT (openacc.avail, openacc_avail);
      DLSYM_OPT (openacc.register_async_cleanup,
		 openacc_register_async_cleanup);
      DLSYM_OPT (openacc.async_test, openacc_async_test);
      DLSYM_OPT (openacc.async_test_all, openacc_async_test_all);
      DLSYM_OPT (openacc.async_wait, openacc_async_wait);
      DLSYM_OPT (openacc.async_wait_async, openacc_async_wait_async);
      DLSYM_OPT (openacc.async_wait_all, openacc_async_wait_all);
      DLSYM_OPT (openacc.async_wait_all_async, openacc_async_wait_all_async);
      DLSYM_OPT (openacc.async_set_async, openacc_async_set_async);
      DLSYM_OPT (openacc.create_thread_data, openacc_create_thread_data);
      DLSYM_OPT (openacc.destroy_thread_data, openacc_destroy_thread_data);
      /* Require all the OpenACC handlers if we have TARGET_CAP_OPENACC_200.  */
      if (optional_present != optional_total)
	{
	  err = "plugin missing OpenACC handler function";
	  goto out;
	}
      optional_present = optional_total = 0;
      DLSYM_OPT (openacc.cuda.get_current_device,
		 openacc_get_current_cuda_device);
      DLSYM_OPT (openacc.cuda.get_current_context,
		 openacc_get_current_cuda_context);
      DLSYM_OPT (openacc.cuda.get_stream, openacc_get_cuda_stream);
      DLSYM_OPT (openacc.cuda.set_stream, openacc_set_cuda_stream);
      /* Make sure all the CUDA functions are there if any of them are.  */
      if (optional_present && optional_present != optional_total)
	{
	  err = "plugin missing OpenACC CUDA handler function";
	  goto out;
	}
    }
#undef DLSYM
#undef DLSYM_OPT

 out:
  if (err != NULL)
    {
      gomp_error ("while loading %s: %s", plugin_name, err);
      if (last_missing)
        gomp_error ("missing function was %s", last_missing);
      if (device->plugin_handle)
	dlclose (device->plugin_handle);
    }
  return err == NULL;
}

/* This function adds a compatible offload image IMAGE to an accelerator device
   DEVICE.  */

static void
gomp_register_image_for_device (struct gomp_device_descr *device,
				struct offload_image_descr *image)
{
  if (!device->offload_regions_registered
      && (device->type == image->type || device->type == TARGET_TYPE_HOST))
    {
      device->offload_register_func (image->host_table, image->target_data);
      device->offload_regions_registered = true;
    }
}

/* This functions scans folder, specified in environment variable
   LIBGOMP_PLUGIN_PATH, and loads all suitable libgomp plugins from this folder.
   For a plugin to be suitable, its name should be "libgomp-plugin-*.so.1" and
   it should implement a certain set of functions.
   Result of this function is properly initialized variable NUM_DEVICES and
   array DEVICES, containing all plugins and their callback handles.  */
static void
gomp_find_available_plugins (void)
{
  char *plugin_path = NULL;
  DIR *dir = NULL;
  struct dirent *ent;
  char plugin_name[PATH_MAX];
  int i;

  num_devices = 0;
  devices = NULL;

  plugin_path = getenv ("LIBGOMP_PLUGIN_PATH");
  if (!plugin_path)
    goto out;

  dir = opendir (plugin_path);
  if (!dir)
    goto out;

  while ((ent = readdir (dir)) != NULL)
    {
      struct gomp_device_descr current_device, *devicep;
      unsigned int i;

      if (!gomp_check_plugin_file_name (ent->d_name))
	continue;
      if (strlen (plugin_path) + 1 + strlen (ent->d_name) >= PATH_MAX)
	continue;
      strcpy (plugin_name, plugin_path);
      strcat (plugin_name, "/");
      strcat (plugin_name, ent->d_name);
      if (!gomp_load_plugin_for_device (&current_device, plugin_name))
	continue;
      devices = gomp_realloc (devices, (num_devices + 1)
				  * sizeof (struct gomp_device_descr));
      if (devices == NULL)
	{
	  num_devices = 0;
	  goto out;
	}

      for (i = 0; i < current_device.get_num_devices_func (); i++)
        {
	  devices[num_devices] = current_device;
	  devicep = &devices[num_devices];

	  devicep->is_initialized = false;
	  devicep->offload_regions_registered = false;
	  devicep->mem_map.splay_tree.root = NULL;
	  devicep->mem_map.is_initialized = false;
	  devicep->type = devicep->get_type_func ();
	  devicep->name = devicep->get_name_func ();
	  gomp_mutex_init (&devicep->mem_map.lock);
	  devicep->ord = i;
	  devicep->target_data = NULL;
	  devicep->openacc.data_environ = NULL;
	  devicep->id = ++num_devices;
	}
    }
  /* Prefer a device with TARGET_CAP_OPENMP_400 for ICV default-device-var.  */
  if (num_devices > 1)
    {
      int d = gomp_icv (false)->default_device_var;

      if (!(devices[d].capabilities & TARGET_CAP_OPENMP_400))
	{
	  for (i = 0; i < num_devices; i++)
	    {
	      if (devices[i].capabilities & TARGET_CAP_OPENMP_400)
		{
		  struct gomp_device_descr device_tmp = devices[d];
		  devices[d] = devices[i];
		  devices[d].id = d + 1;
		  devices[i] = device_tmp;
		  devices[i].id = i + 1;

		  break;
		}
	    }
	}
    }

  for (i = 0; i < num_devices; i++)
    {
      int j;

      for (j = 0; j < num_offload_images; j++)
	gomp_register_image_for_device (&devices[i], &offload_images[j]);

      /* The 'devices' array can be moved (by the realloc call) until we have
	 found all the plugins, so registering with the OpenACC runtime (which
	 takes a copy of the pointer argument) must be delayed until now.  */
      if (devices[i].capabilities & TARGET_CAP_OPENACC_200)
	ACC_register (&devices[i]);
    }

 out:
  if (dir)
    closedir (dir);
  free (offload_images);
  offload_images = NULL;
  num_offload_images = 0;
}

/* This function initializes runtime needed for offloading.
   It loads plugins, sets up a connection with devices, etc.  */
static void
gomp_target_init (void)
{
  gomp_find_available_plugins ();
}

#else /* PLUGIN_SUPPORT */
/* If dlfcn.h is unavailable we always fallback to host execution.
   GOMP_target* routines are just stubs for this case.  */
static void
gomp_target_init (void)
{
}
#endif /* PLUGIN_SUPPORT */
