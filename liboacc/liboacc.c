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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "liboacc.h"
#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int
get_global_id(int id)
{
  fprintf(stderr, "get_global_id is for device only!\n");
  exit(1);
}

/* make sure current device initialized */
void
GOACC_check_cur_dev(void)
{
  OACC_CHECK_INIT
  if(OACC_curr_dev != NULL)
    {
      OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
    }
}

/* get function KERN_NAME form module PROG_NAME */
oacc_kernel
GOACC_get_kernel(const char* prog_name, const char* kern_name)
{
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  return (oacc_kernel)OACC_find_kernel(prog_name, kern_name);
}

/* enqueue kernel KERN for WORKSIZE threads */
void
GOACC_start_kernel(oacc_kernel kern, unsigned worksize,
                  unsigned offset, int groupsize, oacc_event ev,
                  unsigned idx)
{
  OACC_CHECK_KERN_PTR(kern)
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  OACC_enqueue_kernel((struct OACC_kernel_data*)kern, worksize,
         offset, groupsize, (struct OACC_queue_data*)ev, idx);
}

oacc_buffer
GOACC_copyin(void *mem, unsigned size, int check_present, oacc_event ev,
            unsigned idx)
{
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  return (oacc_buffer)OACC_create_or_copy_to_device(mem, size,
    OACC_curr_dev[OACC_curr_num], check_present, 0,
    (struct OACC_queue_data*)ev, idx);
}

void
GOACC_set_kernel_arg(oacc_kernel kern, unsigned idx, oacc_buffer buf)
{
  OACC_CHECK_KERN_PTR(kern)
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  OACC_set_arg_buf((struct OACC_kernel_data*)kern, idx, (OACC_buffer_ptr)buf);
}

oacc_buffer
GOACC_check_present(void *mem)
{
  OACC_buffer_ptr buf = 0;
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  if((buf = OACC_is_present(mem, OACC_curr_dev[OACC_curr_num])) == NULL)
    {
      OACC_fatal("Required data not present\n");
    }
  return buf;
}

oacc_buffer
GOACC_create_on_device(void* mem, unsigned size, int check_present,
                      oacc_event ev, unsigned idx)
{
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  return (oacc_buffer)OACC_create_or_copy_to_device(mem, size,
    OACC_curr_dev[OACC_curr_num], check_present, 1,
    (struct OACC_queue_data*)ev, idx);
}

void
GOACC_copyout(void *mem, unsigned size, int check_present, oacc_event ev,
             unsigned idx)
{
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  OACC_copy_from_device(mem, size, OACC_curr_dev[OACC_curr_num], check_present,
  (struct OACC_queue_data*)ev, idx);
}

oacc_event
GOACC_create_events(const char* src, int lineno)
{
  return (oacc_event)OACC_create_queue(src, lineno);
}

void
GOACC_enqueue_events(oacc_event ev, unsigned n, int k)
{
  OACC_add_event((struct OACC_queue_data*)ev, n, k);
}

void
GOACC_advance_events(oacc_event ev)
{
  struct OACC_queue_data* q = (struct OACC_queue_data*)ev;
  OACC_EQ_ADVANCE(q)
}

void
GOACC_wait_events(oacc_event ev)
{
  OACC_wait_queue((struct OACC_queue_data*)ev);
}

void
GOACC_add_named_async(int cookie, oacc_event ev)
{
  struct OACC_queue_data* q = (struct OACC_queue_data*)ev;
  OACC_CHECK_INIT
  OACC_add_queue_with_name(q, cookie);
}

void
GOACC_add_nameless_async(oacc_event ev)
{
  struct OACC_queue_data* q = (struct OACC_queue_data*)ev;
  OACC_CHECK_INIT
  OACC_add_queue_without_name(q);
}

void
GOACC_wait_named_async(int cookie)
{
  struct OACC_queue_data *q = NULL, *qprev = NULL;
  OACC_CHECK_INIT
  q = OACC_find_queue(cookie, &qprev);
  if(q != NULL)
    {
      OACC_unlink_queue(&OACC_async.named, qprev);
      OACC_wait_queue(q);
    }
}

void
GOACC_wait_all_async()
{
  OACC_CHECK_INIT
  OACC_wait_all_global_queues();
}

void
GOACC_start_profiling(void)
{
  OACC_profiling_enable();
}

#ifdef __cplusplus
}
#endif
