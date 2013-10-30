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
/* runtime support for OpenACC */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* per device init */
void
OACC_dev_init(OACC_device_ptr pdev)
{
  cl_int err;
  cl_command_queue_properties queue_props = 0;

  LOCK_AQUIRE(pdev->dev_lock);
  if(pdev->ctx != NULL && pdev->queue != NULL)
    {
      LOCK_RELEASE(pdev->dev_lock);
      return;
    }
  if(pdev->ctx == NULL)
    {
      pdev->ctx = clCreateContext(NULL, 1, &pdev->id, NULL, NULL, &err);
      if(err < 0)
        {
          OACC_fatal("can't create context: %d\n", err);
        }
    }
  if(pdev->queue == NULL)
    {
      if((pdev->flags & OACC_DF_OUT_OF_ORDER) != 0)
        {
          queue_props |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }
      if(OACC_profiling != 0 && (pdev->flags & OACC_DEF_PROFILING) != 0)
        {
          queue_props |= CL_QUEUE_PROFILING_ENABLE;
        }
      pdev->queue = clCreateCommandQueue(pdev->ctx, pdev->id, queue_props, &err);
      if(err < 0)
        {
          OACC_fatal("Can't create command queue: %d\n", err);
        }
    }
  LOCK_RELEASE(pdev->dev_lock);
}

/* per device cleanup */
void
OACC_dev_fini(OACC_device_ptr pdev)
{
  struct OACC_program_data *prog, *pprog;
  struct OACC_kernel_data *kern, *pkern;
  OACC_buffer_ptr buf, pbuf;

  LOCK_AQUIRE(pdev->dev_lock);

  pprog = pdev->prog_head;
  while(pprog != NULL)
    {
      prog = pprog;
      pprog = prog->next;

      free(prog->name);

      pkern = prog->kern_head;
      while(pkern != NULL)
        {
          kern = pkern;
          pkern = kern->next;

          free(kern->name);
          free(kern->args);
          clReleaseKernel(kern->kern);
          free(kern);
        }
      clReleaseProgram(prog->prog);
      free(prog);
    }

  pbuf = pdev->mem;
  while(pbuf != NULL)
    {
      buf = pbuf;
      pbuf = pbuf->next;

      if(buf->data_ptr != NULL)
        {
          clEnqueueUnmapMemObject(pdev->queue, buf->mem_obj, buf->data_ptr,
                                  0, NULL, NULL);
        }
      clReleaseMemObject(buf->mem_obj);
      free(buf);
    }

  if(pdev->queue != NULL)
    {
      clReleaseCommandQueue(pdev->queue);
      pdev->queue = NULL;
    }
  if(pdev->ctx != NULL)
    {
      clReleaseContext(pdev->ctx);
      pdev->ctx = NULL;
    }
  LOCK_RELEASE(pdev->dev_lock);
}

/* enqueue kernel object */
void
OACC_enqueue_kernel(struct OACC_kernel_data* kern, unsigned worksize,
                    struct OACC_queue_data* queue, unsigned idx)
{
  cl_int err;
  cl_uint nev;
  cl_event *ev_wait = NULL, *ev_set = NULL;

  OACC_EQ_EVENTS(queue, nev, ev_wait, ev_set, idx)

  err = clEnqueueNDRangeKernel(OACC_curr_dev[OACC_curr_num]->queue, kern->kern,
                  1 ,NULL, &worksize, NULL, nev, ev_wait, ev_set);
  if(err < 0)
    {
      OACC_fatal("Can't enqueue kernel: %d\n", err);
    }
}


/* fatal error reporter */
void
OACC_fatal(const char* msg, ...)
{
  va_list args;

  va_start(args, msg);
  vfprintf(stderr, msg, args);
  va_end(args);
  abort();
}

void
OACC_warning(const char* msg, ...)
{
  va_list args;

  va_start(args, msg);
  vfprintf(stderr, msg, args);
  va_end(args);
}

#ifdef __cplusplus
}
#endif
