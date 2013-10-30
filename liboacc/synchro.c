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
/* synchronization for OpenACC */
#include <stdlib.h>
#include <stdio.h>
#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OACC_async_data OACC_async;

int OACC_profiling = 0;
long long OACC_read_time = 0LL, OACC_write_time = 0LL, OACC_exec_time = 0LL;

/* attach event descriptor EV in the end of QUEUE */
static void
OACC_link_event(struct OACC_queue_data* queue, OACC_event_ptr ev)
{
  LOCK_AQUIRE(queue->queue_lock);
  if(queue->head == NULL)
    {
      queue->head = queue->tail = queue->curr = ev;
    }
  else
    {
      ev->prev = queue->tail;
      queue->tail->next = ev;
      queue->tail = ev;
    }
  LOCK_RELEASE(queue->queue_lock);
}

/* create new synchronization queue,
source filename and line number are remembered for the sake of profiling
*/
struct OACC_queue_data*
OACC_create_queue(const char* src, int lineno)
{
  struct OACC_queue_data *queue = NULL;

  queue = (struct OACC_queue_data*)calloc(1, sizeof(struct OACC_queue_data));
  if(queue == NULL)
    {
      OACC_fatal("Not enough memory for event queue\n");
    }
  queue->source_file = src;
  queue->source_line = lineno;
  LOCK_INIT(queue->queue_lock);

  return queue;
}

/* add OpenCL event array of kind KIND (see OACC_PF_* in liboacc-internel.h) */
void
OACC_add_event(struct OACC_queue_data* queue, unsigned n, int kind)
{
  OACC_event_ptr pev = NULL;

  pev = (OACC_event_ptr)calloc(1, sizeof(struct OACC_event_data));
  pev->kind = kind;
  pev->nevents = n;
  pev->events = (cl_event*)calloc(n, sizeof(cl_event));

  OACC_link_event(queue, pev);
}

/* wait enqueued events finish, print profiling data if desired */
void
OACC_wait_queue(struct OACC_queue_data* queue)
{
  cl_int err;
  OACC_event_ptr ev, prev;

  if(queue == NULL || queue->tail == NULL)
    {
      return;
    }

  LOCK_AQUIRE(queue->queue_lock);
  err = clWaitForEvents(queue->tail->nevents, queue->tail->events);
  if(err < 0)
    {
      OACC_fatal("Can't wait for events: %d\n", err);
    }

  if(OACC_profiling)
    {


      for(ev = queue->head; ev != NULL; ev = ev->next)
        {
          unsigned i;
          cl_ulong min_start, max_finish;

          min_start = ~0;
          max_finish = 0;
          for(i = 0; i < ev->nevents; ++i)
            {
              cl_ulong start, finish;
              cl_ulong submited;

              err = clGetEventProfilingInfo(ev->events[i],
                CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &submited, NULL);
              if(err < 0)
                {
                  if(err == CL_PROFILING_INFO_NOT_AVAILABLE)
                    {
                      OACC_warning("Profiling: submit info is not available\n");
                      submited = 0;
                    }
                  else
                    {
                      OACC_fatal("Can't get submit profiling: %d\n", err);
                    }
                }

              err = clGetEventProfilingInfo(ev->events[i],
                CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
              if(err < 0)
                {
                  if(err == CL_PROFILING_INFO_NOT_AVAILABLE)
                    {
                      OACC_warning("Profiling: start info is not available\n");
                      start = 0;
                    }
                  else
                    {
                      OACC_fatal("Can't get start profiling: %d\n", err);
                    }
                }
#ifdef MEASURE_START
              if(start < min_start)
                {
                  min_start = start;
                }
#else
              if(submited < min_start)
                {
                  min_start = submited;
                }
#endif
              err = clGetEventProfilingInfo(ev->events[i],
                CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &finish, NULL);
              if(err < 0)
                {
                  if(err == CL_PROFILING_INFO_NOT_AVAILABLE)
                    {
                      OACC_warning("Profiling: finish info is not available\n");
                      finish = 0;
                    }
                  else
                    {
                      OACC_fatal("Can't get finish profiling: %d\n", err);
                    }
                }
              if(finish > max_finish)
                {
                  max_finish = finish;
                }
            }

          LOCK_AQUIRE(OACC_profile_lock);
          switch(ev->kind)
            {
            case OACC_PF_DATAIN:
              OACC_write_time += max_finish - min_start;
              break;
            case OACC_PF_EXEC:
              OACC_exec_time += max_finish - min_start;
              break;
            case OACC_PF_DATAOUT:
              OACC_read_time += max_finish - min_start;
              break;
            }
          LOCK_RELEASE(OACC_profile_lock);
        }

      LOCK_AQUIRE(OACC_profile_lock);
      printf("Profile for region in %s:%d data to device = %lld,"
              " data from device = %lld, execute = %lld\n",
             queue->source_file, queue->source_line, OACC_write_time,
             OACC_read_time, OACC_exec_time);
      LOCK_RELEASE(OACC_profile_lock);
    }

  prev = queue->head;
  while(prev != NULL)
    {
      ev = prev;
      prev = prev->next;

      free(ev->events);
      free(ev);
    }
  LOCK_RELEASE(queue->queue_lock);
  LOCK_DESTROY(queue->queue_lock);
  free(queue);

}

/* simulate OpenCL events complete in the case we bypass it */
void
OACC_fire_event(struct OACC_queue_data* queue, unsigned idx,
                OACC_device_ptr dev_ptr)
{
  cl_int err;
  cl_uint nev;
  cl_event *ev_wait = NULL, *ev_set = NULL;

  LOCK_AQUIRE(queue->queue_lock);
  OACC_EQ_EVENTS(queue, nev, ev_wait, ev_set, idx)

  if(nev > 0 && ev_wait != NULL)
    {
      if((err = clWaitForEvents(nev, ev_wait)) < 0)
        {
          OACC_fatal("Can't wait for events: %d\n", err);
        }
    }

  if(ev_set != NULL)
    {
      *ev_set = clCreateUserEvent(dev_ptr->ctx, &err);
      if(err < 0)
        {
          OACC_fatal("Can't create user event: %d\n", err);
        }
      err = clSetUserEventStatus(*ev_set, CL_COMPLETE);
      if(err < 0)
        {
          OACC_fatal("Can't fire user event: %d\n", err);
        }
    }
  LOCK_RELEASE(queue->queue_lock);
}

/* attach queue to list */
static void
OACC_link_queue_list(struct OACC_queue_data** plist,
                      struct OACC_queue_data* queue)
{
  LOCK_AQUIRE(OACC_async_lock);
  queue->next = *plist;
  *plist = queue;
  LOCK_RELEASE(OACC_async_lock);
}

/* find queue with specified integer name */
struct OACC_queue_data*
OACC_find_queue(int cookie, struct OACC_queue_data** pprev)
{
  struct OACC_queue_data *q = NULL, *qprev = NULL;

  LOCK_AQUIRE(OACC_async_lock);
  for(qprev = q = OACC_async.named; q != NULL; qprev = q, q = q->next)
    {
      if(q->cookie == cookie)
        {
          if(pprev != NULL)
            {
              *pprev = qprev;
            }
          break;
        }
    }
  LOCK_RELEASE(OACC_async_lock);
  return q;
}

struct OACC_queue_data*
OACC_unlink_queue(struct OACC_queue_data** plist, struct OACC_queue_data* prev)
{
  struct OACC_queue_data* queue = NULL;

  if(prev == NULL)
    {
      return NULL;
    }

  LOCK_AQUIRE(OACC_async_lock);
  queue = prev->next;
  if(prev == *plist)
    {
      *plist = queue->next;
    }
  else
    {
      prev->next = queue->next;
    }
  LOCK_RELEASE(OACC_async_lock);
  return queue;
}

/* attach one queue in the end of the other */
static void
OACC_merge_queues(struct OACC_queue_data* dst, struct OACC_queue_data* src)
{
  OACC_event_ptr ev;

  LOCK_AQUIRE(src->queue_lock);
  for(ev = src->head; ev != NULL; ev = ev->next)
    {
      OACC_link_event(dst, ev);
    }
  LOCK_RELEASE(src->queue_lock);
}

/* add queue with given name */
void
OACC_add_queue_with_name(struct OACC_queue_data* queue, int name)
{
  struct OACC_queue_data* q = OACC_find_queue(name, NULL);

  if(q != NULL)
    {
      OACC_merge_queues(q, queue);
      free(queue);
    }
  else
    {
      OACC_link_queue_list(&OACC_async.named, queue);
    }
}

/* add nameless queue */
void
OACC_add_queue_without_name(struct OACC_queue_data* queue)
{
  OACC_link_queue_list(&OACC_async.nameless, queue);
}

/* count still running queues */
static int
OACC_count_unfinished(struct OACC_queue_data** list,
                    struct OACC_queue_data **pwait_list)
{
  int count = 0;
  struct OACC_queue_data *q = NULL, *qprev = NULL;

  for(qprev = q = *list; q != NULL; qprev = q, q = q->next)
    {
      switch(OACC_test_queue(q))
        {
        case OACC_QS_RUNNING:
          ++count;
          break;
        case OACC_QS_READY:
          ++count;
          if(pwait_list != NULL)
            {
              OACC_unlink_queue(list, qprev);
              OACC_link_queue_list(pwait_list, q);
            }
          break;
        }
    }
  return count;
}

/* wait all globally visible queues */
void
OACC_wait_all_global_queues()
{
  int unfinished;

  do
    {
      struct OACC_queue_data *wait_list = NULL;
      unfinished = 0;

      unfinished += OACC_count_unfinished(&OACC_async.named, &wait_list);
      unfinished += OACC_count_unfinished(&OACC_async.nameless, &wait_list);

      if(wait_list != NULL)
        {
          struct OACC_queue_data *q = wait_list, *qnext = NULL;
          while(q != NULL)
            {
              qnext = q->next;
              OACC_wait_queue(q);
              q = qnext;
            }
        }

    }
  while(unfinished > 0);

}

/* check queue status */
enum OACC_queue_status
OACC_test_queue(struct OACC_queue_data* queue)
{
  cl_uint i;

  LOCK_AQUIRE(queue->queue_lock);

  if(queue->curr != queue->tail)
    {
      LOCK_RELEASE(queue->queue_lock);
      return OACC_QS_RUNNING;
    }

  for(i = 0; i < queue->tail->nevents; ++i)
    {
      if(queue->tail->events[i] == (void*)0)
        {
          LOCK_RELEASE(queue->queue_lock);
          return OACC_QS_RUNNING;
        }
    }

  for(i = 0; i < queue->tail->nevents; ++i)
    {
      cl_int err, status;

      err = clGetEventInfo(queue->tail->events[i],
        CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &status, NULL);
      if(err < 0)
        {
          OACC_fatal("Can't get event status: %s\n", err);
        }
      if(status > CL_COMPLETE)
        {
          LOCK_RELEASE(queue->queue_lock);
          return OACC_QS_READY;
        }
    }

  LOCK_RELEASE(queue->queue_lock);
  return OACC_QS_COMPLETE;
}

/* check all globally visible queues */
int
OACC_test_all_global_queues(void)
{
  int unfinished = 0;

  unfinished += OACC_count_unfinished(&OACC_async.named, NULL);
  unfinished += OACC_count_unfinished(&OACC_async.nameless, NULL);
  return unfinished == 0;
}

/* enable profiling */
void
OACC_profiling_enable(void)
{
  LOCK_AQUIRE(OACC_profile_lock);
  OACC_profiling = 1;
  OACC_read_time = 0LL, OACC_write_time = 0LL, OACC_exec_time = 0LL;
  LOCK_RELEASE(OACC_profile_lock);
}

#ifdef __cplusplus
}
#endif
