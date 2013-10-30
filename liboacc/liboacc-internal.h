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

/* LIBOACC internals */

#ifndef LIBOACC_INTERNAL_H
#define LIBOACC_INTERNAL_H

#if __CYGWIN__ && !_WIN32
#define _WIN32 1
#endif

#include <stdlib.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#define PHTREAD_LOCKS
#endif


#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PTHREAD_LOCKS

#define LOCK_DECLARE(m)			pthread_murex_t m
#define LOCK_DECLARE_EX(m)	extern pthread_murex_t m = PTHREAD_MUTEX_INITIALIZER
#define LOCK_INIT(m)				pthread_mutex_init(&m, NULL)
#define LOCK_AQUIRE(m)			pthread_mutex_lock(&m)
#define LOCK_RELEASE(m)			pthread_mutex_unlock(&m)
#define LOCK_DESTROY(m)			pthread_mutex_destroy(&m)

#else /* single threaded */

#define LOCK_DECLARE(m)
#define LOCK_DECLARE_EX(m)
#define LOCK_INIT(m)
#define LOCK_AQUIRE(m)
#define LOCK_RELEASE(m)
#define LOCK_DESTROY(m)

#endif

enum OACC_queue_status {
  OACC_QS_RUNNING,
  OACC_QS_READY,
  OACC_QS_COMPLETE
};

typedef struct OACC_event_data* OACC_event_ptr;

struct OACC_event_data
{
  int kind;
  cl_uint nevents;
  cl_event* events;
  OACC_event_ptr next;
  OACC_event_ptr prev;
};

struct OACC_queue_data
{
  int cookie;
  const char* source_file;
  int source_line;
  OACC_event_ptr head, tail, curr;
  struct OACC_queue_data* next;
  LOCK_DECLARE(queue_lock);
};

#define OACC_EQ_WAIT_COUNT(q)			((q->curr->prev != NULL) ? \
                                    q->curr->prev->nevents : 0)
#define OACC_EQ_WAIT_PTR(q)				((q->curr->prev != NULL) ? \
                                    q->curr->prev->events : NULL)
#define OACC_EQ_SET_PTR(q, i)			((i < q->curr->nevents) ? \
                                    q->curr->events + i : NULL)
#define OACC_EQ_EVENTS(q,n,w,s,i)	if(q != NULL && q->curr != NULL) \
                                  { \
																		n = OACC_EQ_WAIT_COUNT(q); \
																		w = OACC_EQ_WAIT_PTR(q); \
																		s = OACC_EQ_SET_PTR(q, i); \
																	}
#define OACC_EQ_ADVANCE(q)			if(q != NULL && q->curr != NULL \
                                    && q->curr->next != NULL) \
                                {\
																	q->curr = q->curr->next; \
																}
#define OACC_EQ_FIRE_EVENT(q,i,d)	if(q != NULL && q->curr != NULL \
                                    && i < q->curr->nevents) \
                                {\
																		OACC_fire_event(q, i, d); \
																}

struct OACC_async_data
{
  struct OACC_queue_data *named, *nameless;
};

#define OACC_BF(n)				(1<<n)
#define OACC_BF_PRESENT		OACC_BF(0)
#define OACC_BF_DEV_ONLY	OACC_BF(1)
#define OACC_BF_MAPPED		OACC_BF(2)

/*
argument data represented as OpenCL memory buffer
*/
struct OACC_buffer_data
{
  cl_mem mem_obj;
  unsigned flags;
  void* data_ptr; /* not an owner! DO NOT free */
  size_t data_size;
  struct OACC_buffer_data* next;
};

typedef struct OACC_buffer_data* OACC_buffer_ptr;

/*
element of kernels linked list
*/
struct OACC_kernel_data
{
  cl_kernel kern;
  char* name; /* cached name to avoid reloading */

  size_t nargs;
  OACC_buffer_ptr* args;
  struct OACC_kernel_data* next;
};

/*
element of programs linked list
*/
struct OACC_program_data
{
  cl_program prog;
  char* name; /*cached name to avoid reloading */

  struct OACC_kernel_data *kern_head;
  struct OACC_program_data *next;
};

#define OACC_DF(n)						(1<<n)
#define OACC_DF_OUT_OF_ORDER	OACC_DF(0)
#define OACC_DF_UNIFIED_MEM		OACC_DF(1)
#define OACC_DEF_PROFILING		OACC_DF(2)

enum OACC_vec_type_idx
{
  OACC_IDX_CHAR, OACC_IDX_SHORT, OACC_IDX_INT,
  OACC_IDX_LONG, OACC_IDX_FLOAT, OACC_IDX_DOUBLE,
  OACC_IDX_HALF, OACC_TYPE_LAST
};

enum OACC_vec_kind_idx
{
  OACC_IDX_NATIVE, OACC_IDX_PREF, OACC_KIND_LAST
};

/*
data per OpenCL device
*/
struct OACC_device_data
{
  cl_device_id id;
  cl_device_type type; 	/* cached device type */
  unsigned flags;
  cl_uint max_comp_units;
  cl_uint max_wi_dims;
  size_t *max_wi_sizes;
  size_t max_wg_size;
  cl_ulong max_alloc, glob_mem_size, const_mem_size, loc_mem_size;
  cl_uint vec_size[OACC_TYPE_LAST][OACC_KIND_LAST];
  cl_command_queue queue;
  cl_context ctx;				/* one device per context */
  struct OACC_program_data *prog_head;
  OACC_buffer_ptr mem;
  LOCK_DECLARE(dev_lock);
};

typedef struct OACC_device_data* OACC_device_ptr;

/*
data per OpenCL platform
*/
struct OACC_platform_data
{
  cl_platform_id id;

  size_t ndevs;
  struct OACC_device_data* devs;
};

/* global platforms array */
extern size_t OACC_nplatforms;
extern struct OACC_platform_data* OACC_platforms;

extern struct OACC_async_data OACC_async;

/* arrays of pointers to device data in order 
to implement OpenACC get/set device facility */
extern size_t OACC_nhost_devs, OACC_nacc_devs, OACC_curr_num, 
              OACC_host_num, OACC_acc_num;
extern OACC_device_ptr *OACC_host_devs, *OACC_acc_devs,
              *OACC_default_dev, *OACC_curr_dev;

extern long long OACC_read_time, OACC_write_time, OACC_exec_time;
extern int OACC_profiling;

LOCK_DECLARE_EX(OACC_dev_lock);
LOCK_DECLARE_EX(OACC_async_lock);
LOCK_DECLARE_EX(OACC_profile_lock);

/* environment initialization/cleanup */
extern void OACC_env_init(void);
extern void OACC_env_fini(void);
/* device initialization / cleanup */
extern void OACC_dev_init(OACC_device_ptr pdev);
extern void OACC_dev_fini(OACC_device_ptr pdev);

#define OACC_CHECK_INIT								if(OACC_platforms == NULL) \
                                        OACC_env_init();
#define OACC_CHECK_DEV_INIT(dev_ptr)	if((dev_ptr)->ctx == NULL \
                                          || (dev_ptr)->queue == NULL) \
                                        OACC_dev_init(dev_ptr);
#define OACC_CHECK_KERN_PTR(kern)			if(kern == NULL) return;

/* runtime support for OpenACC codegen */
extern struct OACC_kernel_data* OACC_find_kernel(const char* prog_name,
                                                 const char* kern_name);
extern void OACC_enqueue_kernel(struct OACC_kernel_data* kern,
                                unsigned worksize,
                                struct OACC_queue_data* queue,
                                unsigned idx);
extern void OACC_set_arg_buf(struct OACC_kernel_data* kern,
                             unsigned idx, OACC_buffer_ptr buf);

/* memory biffers management */
extern OACC_buffer_ptr OACC_create_or_copy_to_device(void *ptr,
                                                     size_t size,
                                                     OACC_device_ptr dev_ptr,
                                                     int chk_present,
                                                     int dev_only,
                                  struct OACC_queue_data* queue,
                                    unsigned idx);
extern void OACC_copy_from_device(void *ptr, size_t size,
                                  OACC_device_ptr dev_ptr, int chk_present,
                                  struct OACC_queue_data* queue,
                                  unsigned idx);
extern OACC_buffer_ptr OACC_is_present(void *ptr, OACC_device_ptr dev_ptr);

/* memory allocation on device */
extern OACC_buffer_ptr OACC_alloc_on_device(void *host_ptr, size_t size,
                                            OACC_device_ptr dev_ptr,
                                            int dev_only);
extern void OACC_free_device_mem(void* ptr, OACC_device_ptr dev_ptr);

/* async management */
extern struct OACC_queue_data* OACC_create_queue(const char* src, int lineno);
extern void OACC_add_event(struct OACC_queue_data* queue, unsigned n, int kind);
extern void OACC_wait_queue(struct OACC_queue_data* queue);
extern void OACC_fire_event(struct OACC_queue_data* queue, unsigned i,
                            OACC_device_ptr dev_ptr);
extern void OACC_add_queue_with_name(struct OACC_queue_data* queue, int name);
extern void OACC_add_queue_without_name(struct OACC_queue_data* queue);
extern struct OACC_queue_data* OACC_find_queue(int cookie,
                                            struct OACC_queue_data** pprev);
extern void OACC_wait_all_global_queues(void);
extern enum OACC_queue_status OACC_test_queue(struct OACC_queue_data* queue);
extern int OACC_test_all_global_queues(void);
struct OACC_queue_data* OACC_unlink_queue(struct OACC_queue_data** plist,
                                        struct OACC_queue_data* prev);

#define OACC_PF_DATAIN	1
#define OACC_PF_EXEC		2
#define OACC_PF_DATAOUT	3

extern void OACC_profiling_enable(void);

extern void OACC_fatal(const char* msg, ...);
extern void OACC_warning(const char* msg, ...);

#ifdef __cplusplus
}
#endif

#endif
