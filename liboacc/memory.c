/*
This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */
/* memory management for OpenACC */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* find device buffer object that corresponds to host PTR.
	PPREV can be non NULL, in this case previous pointer returned in it.
*/
static OACC_buffer_ptr
OACC_find_buffer(void *ptr, OACC_device_ptr dev_ptr, OACC_buffer_ptr* pprev)
{
  OACC_buffer_ptr buf = NULL, prev = NULL;

  LOCK_AQUIRE(dev_ptr->dev_lock);
  for(prev = buf = dev_ptr->mem; buf != NULL; prev = buf, buf = buf->next)
    {
      if((ptr >= buf->data_ptr) && (ptr < buf->data_ptr + buf->data_size))
        {
          if(pprev != NULL)
            {
              *pprev = prev;
            }
          break;
        }
    }
  LOCK_RELEASE(dev_ptr->dev_lock);
  return buf;
}

/* add buffer BUF_PTR to linked list on device pointed by DEV_PTR */
static void
OACC_attach_buffer(OACC_device_ptr dev_ptr, OACC_buffer_ptr buf_ptr)
{
  LOCK_AQUIRE(dev_ptr->dev_lock);
  buf_ptr->next = dev_ptr->mem;
  dev_ptr->mem = buf_ptr;
  LOCK_RELEASE(dev_ptr->dev_lock);
}

/* remove buffer that is next to given PREV_PTR from device linked list */
static OACC_buffer_ptr
OACC_detach_buffer(OACC_device_ptr dev_ptr, OACC_buffer_ptr prev_ptr)
{
  OACC_buffer_ptr buf = NULL;

  if(prev_ptr == NULL)
    {
      return NULL;
    }

  LOCK_AQUIRE(dev_ptr->dev_lock);
  buf = prev_ptr->next;
  if(prev_ptr == dev_ptr->mem)
    {
      dev_ptr->mem = buf->next;
    }
  else
    {
      prev_ptr->next = buf->next;
    }
  LOCK_RELEASE(dev_ptr->dev_lock);
  return buf;
}

/* allocate on device DEV_PTR SIZE bytes of memory */
OACC_buffer_ptr
OACC_alloc_on_device(void *host_ptr, size_t size, OACC_device_ptr dev_ptr,
                     int dev_only)
{
  cl_int err;
  cl_mem mem_obj;
  OACC_buffer_ptr buff = NULL;
  cl_mem_flags mem_flags = CL_MEM_READ_WRITE;
  unsigned buf_flags = 0;

  if(host_ptr != NULL && dev_only == 0)
    {
      mem_flags |= ((dev_ptr->flags & OACC_DF_UNIFIED_MEM) != 0) 
        ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
    }
  
  mem_obj = clCreateBuffer(dev_ptr->ctx, mem_flags, size,
                           (dev_only != 0) ? NULL : host_ptr, &err);
  if(err < 0)
    {
      OACC_fatal("Can't create memory buffer: %d\n", err);
    }

  if(host_ptr == NULL && dev_only == 0)
    {
      host_ptr = clEnqueueMapBuffer(dev_ptr->queue, mem_obj, CL_TRUE,
        CL_MAP_READ | CL_MAP_WRITE, 0, size, 0, NULL, NULL, &err);
      if(err < 0)
        {
          OACC_fatal("Can't map memory buffer: %d\n", err);
        }
      buf_flags |= OACC_BF_MAPPED;
    }

  if(dev_only != 0)
    {
      buf_flags |= OACC_BF_DEV_ONLY;
    }

  buff = (OACC_buffer_ptr)calloc(1, sizeof(struct OACC_buffer_data));
  if(buff == NULL)
    {
      OACC_fatal("Not enough memory for buffer\n");
    }

  buff->mem_obj = mem_obj;
  buff->data_size = size;
  buff->data_ptr = host_ptr;
  buff->flags = buf_flags;
  OACC_attach_buffer(dev_ptr, buff);
  return buff;
}

/* free memory on device */
void
OACC_free_device_mem(void* ptr, OACC_device_ptr dev_ptr)
{
  OACC_buffer_ptr buf = NULL, prev = NULL;
  cl_int err;


  buf = OACC_find_buffer(ptr, dev_ptr, &prev);
  if(buf == NULL)
    {
      return;
    }

  if((buf->flags & OACC_BF_MAPPED) != 0)
    {
      if((err = clEnqueueUnmapMemObject(dev_ptr->queue, buf->mem_obj,
          buf->data_ptr, 0, NULL, NULL)) < 0)
        {
          OACC_fatal("Can't unmap memory buffer: %d\n", err);
        }
    }
  clReleaseMemObject(buf->mem_obj);

  OACC_detach_buffer(dev_ptr, prev);
  free(buf);
}

/* attach memory obect as IDX argument of kernel KERN */
void
OACC_set_arg_buf(struct OACC_kernel_data* kern, unsigned idx,
                 OACC_buffer_ptr buf)
{
  cl_int err;

  if(idx >= kern->nargs)
    {
      return;
    }

  kern->args[idx] = buf;
  if((err = clSetKernelArg(kern->kern , idx, sizeof(cl_mem),
      &buf->mem_obj)) < 0)
    {
      OACC_fatal("Can't set kernel argument: %d\n", err);
    }
}

/* assure presence of host memory object on device */
OACC_buffer_ptr
OACC_create_or_copy_to_device(void *ptr, size_t size, OACC_device_ptr dev_ptr,
     int chk_present, int dev_only, struct OACC_queue_data* queue, unsigned idx)
{
  OACC_buffer_ptr buf = NULL;
  cl_uint nev;
  cl_event *ev_wait = NULL, *ev_set = NULL;

  OACC_EQ_EVENTS(queue, nev, ev_wait, ev_set, idx)

  if(chk_present && (buf = OACC_is_present(ptr, dev_ptr)) != NULL)
    {
      OACC_EQ_FIRE_EVENT(queue, idx, dev_ptr)
      return buf;
    }

  buf = OACC_find_buffer(ptr, dev_ptr, NULL);
  if(buf == NULL)
    {
      buf = OACC_alloc_on_device(ptr, size, dev_ptr, dev_only);
    }

  if(ptr != NULL && dev_only == 0)
    {
      if((buf->flags & OACC_BF_MAPPED) != 0 && ptr != buf->data_ptr)
        {
          memcpy(buf->data_ptr, ptr, size);
          OACC_EQ_FIRE_EVENT(queue, idx, dev_ptr)
        }
      else
        {
#ifdef UNIFIED_MEM_OPT
          if((dev_ptr->flags & OACC_DF_UNIFIED_MEM) != 0)
            {
              OACC_EQ_FIRE_EVENT(queue, idx, dev_ptr)
            }
          else
            {
#endif
              cl_int err;

              err = clEnqueueWriteBuffer(dev_ptr->queue, buf->mem_obj,
                                 CL_FALSE, 0, size, ptr, nev, ev_wait, ev_set);
              if(err < 0)
                {
                  OACC_fatal("Can't enqueue write buffer: %d\n", err);
                }
#ifdef UNIFIED_MEM_OPT
            }
#endif
        }
      buf->flags |= OACC_BF_PRESENT;
    }
  else if(ptr != NULL && dev_only != 0)
    {
      OACC_EQ_FIRE_EVENT(queue, idx, dev_ptr)
      buf->flags |= OACC_BF_PRESENT;
    }

  return buf;
}

/* copy device memory object back to host */
void
OACC_copy_from_device(void *ptr, size_t size, OACC_device_ptr dev_ptr,
                 int chk_present, struct OACC_queue_data* queue, unsigned idx)
{
  OACC_buffer_ptr buf = NULL;
  cl_uint nev;
  cl_event *ev_wait = NULL, *ev_set = NULL;

  OACC_EQ_EVENTS(queue, nev, ev_wait, ev_set, idx)

  buf = OACC_find_buffer(ptr, dev_ptr, NULL);
  if(buf != NULL)
    {
      if((buf->flags & OACC_BF_MAPPED) != 0 && ptr != buf->data_ptr)
        {
          memcpy(ptr, buf->data_ptr, size);
          OACC_EQ_FIRE_EVENT(queue, idx, dev_ptr)
        }
      else
        {
#ifdef UNIFIED_MEM_OPT
          if((dev_ptr->flags & OACC_DF_UNIFIED_MEM) != 0)
            {
              OACC_EQ_FIRE_EVENT(queue, idx, dev_ptr)
            }
          else
            {
#endif
              cl_int err;

              err = clEnqueueReadBuffer(dev_ptr->queue, buf->mem_obj,
                                        CL_FALSE, 0, size, ptr, nev,
                                        ev_wait, ev_set);
              if(err < 0)
                {
                  OACC_fatal("Can't enqueue read buffer: %d\n", err);
                }
#ifdef UNIFIED_MEM_OPT
            }
#endif
        }
    }
}

/* check memory object presence on device */
OACC_buffer_ptr
OACC_is_present(void *ptr, OACC_device_ptr dev_ptr)
{
  OACC_buffer_ptr buf = OACC_find_buffer(ptr, dev_ptr, NULL);

  if(buf == NULL)
    {
      return NULL;
    }

  return (buf->flags & OACC_BF_PRESENT) ? buf : NULL;
}

#ifdef __cplusplus
}
#endif
