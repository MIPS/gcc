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

#ifndef _TARGET_H
#define _TARGET_H 1

#include <stdarg.h>
#include "splay-tree.h"
#include "gomp-constants.h"

struct target_mem_desc {
  /* Reference count.  */
  uintptr_t refcount;
  /* All the splay nodes allocated together.  */
  splay_tree_node array;
  /* Start of the target region.  */
  uintptr_t tgt_start;
  /* End of the targer region.  */
  uintptr_t tgt_end;
  /* Handle to free.  */
  void *to_free;
  /* Previous target_mem_desc.  */
  struct target_mem_desc *prev;
  /* Number of items in following list.  */
  size_t list_count;

  /* Corresponding target device descriptor.  */
  struct gomp_device_descr *device_descr;
  
  /* Memory mapping info for the thread that created this descriptor.  */
  struct gomp_memory_mapping *mem_map;

  /* List of splay keys to remove (or decrease refcount)
     at the end of region.  */
  splay_tree_key list[];
};

/* Keep in sync with openacc.h:acc_device_t.  */

enum target_type {
  TARGET_TYPE_HOST = GOMP_TARGET_HOST,
  TARGET_TYPE_HOST_NONSHM = GOMP_TARGET_HOST_NONSHM,
  TARGET_TYPE_NVIDIA_PTX = GOMP_TARGET_NVIDIA_PTX,
  TARGET_TYPE_INTEL_MIC = GOMP_TARGET_INTEL_MIC,
};

#define TARGET_CAP_SHARED_MEM	1
#define TARGET_CAP_NATIVE_EXEC	2
#define TARGET_CAP_OPENMP_400	4
#define TARGET_CAP_OPENACC_200	8

/* Information about mapped memory regions (per device/context).  */

struct gomp_memory_mapping
{
  /* Splay tree containing information about mapped memory regions.  */
  struct splay_tree_s splay_tree;

  /* Mutex for operating with the splay tree and other shared structures.  */
  gomp_mutex_t lock;
  
  /* True when tables have been added to this memory map.  */
  bool is_initialized;
};

typedef struct ACC_dispatch_t
{
  /* This is a linked list of data mapped using the
     acc_map_data/acc_unmap_data or "acc enter data"/"acc exit data" pragmas
     (TODO).  Unlike mapped_data in the goacc_thread struct, unmapping can
     happen out-of-order with respect to mapping.  */
  struct target_mem_desc *data_environ;

  /* Open or close a device instance.  */
  void *(*open_device_func) (int n);
  int (*close_device_func) (void *h);

  /* Set or get the device number.  */
  int (*get_device_num_func) (void);
  void (*set_device_num_func) (int);

  /* Execute.  */
  void (*exec_func) (void (*) (void *), size_t, void **, void **, size_t *,
		     unsigned short *, int, int, int, int, void *);

  /* Async cleanup callback registration.  */
  void (*register_async_cleanup_func) (void *);

  /* Asynchronous routines.  */
  int (*async_test_func) (int);
  int (*async_test_all_func) (void);
  void (*async_wait_func) (int);
  void (*async_wait_async_func) (int, int);
  void (*async_wait_all_func) (void);
  void (*async_wait_all_async_func) (int);
  void (*async_set_async_func) (int);

  /* Create/destroy TLS data.  */
  void *(*create_thread_data_func) (void *);
  void (*destroy_thread_data_func) (void *);

  /* NVIDIA target specific routines.  */
  struct {
    void *(*get_current_device_func) (void);
    void *(*get_current_context_func) (void);
    void *(*get_stream_func) (int);
    int (*set_stream_func) (int, void *);
  } cuda;
} ACC_dispatch_t;

struct mapping_table {
  uintptr_t host_start;
  uintptr_t host_end;
  uintptr_t tgt_start;
  uintptr_t tgt_end;
};

/* This structure describes accelerator device.
   It contains name of the corresponding libgomp plugin, function handlers for
   interaction with the device, ID-number of the device, and information about
   mapped memory.  */
struct gomp_device_descr
{
  /* The name of the device.  */
  const char *name;

  /* Capabilities of device (supports OpenACC, OpenMP).  */
  unsigned int capabilities;

  /* This is the ID number of device.  It could be specified in DEVICE-clause of
     TARGET construct.  */
  int id;

  /* The number of the device for this particular device type.  */
  int ord;

  /* This is the TYPE of device.  */
  enum target_type type;

  /* Set to true when device is initialized.  */
  bool is_initialized;
  
  /* True when offload regions have been registered with this device.  */
  bool offload_regions_registered;

  /* Plugin file handler.  */
  void *plugin_handle;

  /* Function handlers.  */
  const char *(*get_name_func) (void);
  unsigned int (*get_caps_func) (void);
  int (*get_type_func) (void);
  int (*get_num_devices_func) (void);
  void (*offload_register_func) (void *, void *);
  int (*device_init_func) (void);
  int (*device_fini_func) (void);
  int (*device_get_table_func) (struct mapping_table **);
  void *(*device_alloc_func) (size_t);
  void (*device_free_func) (void *);
  void *(*device_dev2host_func) (void *, const void *, size_t);
  void *(*device_host2dev_func) (void *, const void *, size_t);
  void (*device_run_func) (void *, void *);

  /* OpenACC-specific functions.  */
  ACC_dispatch_t openacc;
  
  /* Memory-mapping info for this device instance.  */
  struct gomp_memory_mapping mem_map;

  /* Extra information required for a device instance by a given target.  */
  void *target_data;
};

extern struct target_mem_desc *
gomp_map_vars (struct gomp_device_descr *devicep, size_t mapnum,
	       void **hostaddrs, void **devaddrs, size_t *sizes, void *kinds,
	       bool is_openacc, bool is_target);

extern void
gomp_copy_from_async (struct target_mem_desc *tgt);

extern void
gomp_unmap_vars (struct target_mem_desc *tgt, bool);

extern attribute_hidden void
gomp_init_device (struct gomp_device_descr *devicep);

extern attribute_hidden void
gomp_init_tables (const struct gomp_device_descr *devicep,
		  struct gomp_memory_mapping *mm);

extern attribute_hidden void
gomp_fini_device (struct gomp_device_descr *devicep);

extern attribute_hidden void
gomp_free_memmap (struct gomp_device_descr *devicep);

#endif /* _TARGET_H */
