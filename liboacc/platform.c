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
/* accounting data for OpenCL platforms / devices */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <malloc.h>
#define alloca _alloca
#endif


size_t OACC_nplatforms = 0;
struct OACC_platform_data* OACC_platforms = NULL;

size_t OACC_nhost_devs = 0, OACC_nacc_devs = 0, OACC_curr_num = 0,
      OACC_host_num = 0, OACC_acc_num = 0;
OACC_device_ptr *OACC_host_devs = NULL, *OACC_acc_devs = NULL,
      *OACC_default_dev = NULL, *OACC_curr_dev = NULL;

/* get OpenCL device type */
static cl_device_type
get_device_type(cl_device_id device)
{
  cl_int err;
  cl_device_type type = 0;

  if((err = clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type),
    &type, NULL)) < 0)
    {
      OACC_fatal("Can't get device type: %d\n", err);
    }
  return type;
}

/* map OpenCL device type to OpenACC device type */
static int
is_host_device(cl_device_type type)
{
  if((type & CL_DEVICE_TYPE_GPU) || (type &  CL_DEVICE_TYPE_ACCELERATOR))
    {
      return 0;
    }
  return 1;
}

static void
read_environment(OACC_device_ptr** ppdev, size_t* pnum)
{
  const char* val = NULL;

  val = getenv("ACC_DEVICE_TYPE");
  if(val != NULL)
    {
      if(strcmp(val, "host") == 0 || strcmp(val, "cpu") == 0)
        {
          *ppdev = OACC_host_devs;
        }
      else if(strcmp(val, "not_host") == 0 || strcmp(val, "gpu") == 0)
        {
          *ppdev = OACC_acc_devs;
        }
    }

  val = getenv("ACC_DEVICE_NUM");
  if(val != NULL)
    {
      size_t num = atoi(val);
      if(num >= 0)
        {
          if(*ppdev == OACC_host_devs && num < OACC_nhost_devs)
            {
              *pnum = num;
            }
          else if(*ppdev == OACC_acc_devs && num < OACC_nacc_devs)
            {
              *pnum = num;
            }
        }
    }
}

/* check if device is available to use */
static int
device_usable(cl_device_id device)
{
  cl_int err;
  cl_bool avail;
  cl_device_exec_capabilities exec_caps;

  /* is device available for OpenCL? */
  if((err = clGetDeviceInfo(device, CL_DEVICE_AVAILABLE, sizeof(cl_bool),
        &avail, NULL)) < 0)
    {
      OACC_warning("Can't get device availability flag: %d\n", err);
      return 0;
    }
  if(avail != CL_TRUE)
    {
      return 0;
    }

  /* can device execute non native kernels? */
  if((err = clGetDeviceInfo(device, CL_DEVICE_EXECUTION_CAPABILITIES,
      sizeof(cl_device_exec_capabilities), &exec_caps, NULL)) < 0)
    {
      OACC_warning("Can't get device execute capabilities: %d\n", err);
      return 0;
    }
  if((exec_caps & CL_EXEC_KERNEL) == 0)
    {
      return 0;
    }

  /* is compiler available? */
  if((err = clGetDeviceInfo(device, CL_DEVICE_COMPILER_AVAILABLE,
      sizeof(cl_bool), &avail, NULL)) < 0)
    {
      OACC_warning("Can't get compiler availablity flag: %d\n", err);
      return 0;
    }
  if(avail != CL_TRUE)
    {
      return 0;
    }

  return 1;
}

/* get various device capabilities */
static void
fill_device_caps(OACC_device_ptr dev)
{
  cl_int err;
  cl_bool flag;
  cl_command_queue_properties qprops;


  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_QUEUE_PROPERTIES,
      sizeof(cl_command_queue_properties), &qprops, NULL)) < 0)
    {
      OACC_warning("Can't get device queue properties: %d\n", err);
    }
  else
    {
      if((qprops & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) != 0)
        {
          dev->flags |= OACC_DF_OUT_OF_ORDER;
        }
      if((qprops & CL_QUEUE_PROFILING_ENABLE) != 0)
        {
          dev->flags |= OACC_DEF_PROFILING;
        }
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_HOST_UNIFIED_MEMORY,
      sizeof(cl_bool), &flag, NULL)) < 0)
    {
      OACC_warning("Can't get unified memory flag: %d\n", err);
    }
  else
    {
      if(flag != CL_FALSE)
        {
          dev->flags |= OACC_DF_UNIFIED_MEM;
        }
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_MAX_COMPUTE_UNITS,
      sizeof(cl_uint), &(dev->max_comp_units), NULL)) < 0)
    {
      OACC_warning("Can't get computational units number: %d\n", err);
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
      sizeof(cl_uint), &(dev->max_wi_dims), NULL)) < 0)
    {
      OACC_warning("Can't get maximal workitems dimension: %d\n", err);
    }
  else
    {
      dev->max_wi_sizes = (size_t*)calloc(dev->max_wi_dims, sizeof(size_t));
      if(dev->max_wi_sizes == NULL)
        {
          OACC_fatal("Can't allocate memory for workitems sizes\n");
        }
      if((err = clGetDeviceInfo(dev->id, CL_DEVICE_MAX_WORK_ITEM_SIZES,
          dev->max_wi_dims * sizeof(size_t), dev->max_wi_sizes, NULL)) < 0)
        {
          OACC_warning("Can't get maximal workitems sizes: %d\n", err);
        }
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
      sizeof(size_t), &(dev->max_wg_size), NULL)) < 0)
    {
      OACC_warning("Can't get maximal workgroup size: %d\n", err);
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_MAX_MEM_ALLOC_SIZE,
      sizeof(cl_ulong), &(dev->max_alloc), NULL)) < 0)
    {
      OACC_warning("Can't get maximal allocation size: %d\n", err);
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_GLOBAL_MEM_SIZE,
      sizeof(cl_ulong), &(dev->glob_mem_size), NULL)) < 0)
    {
      OACC_warning("Can't get global memory size: %d\n", err);
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
      sizeof(cl_ulong), &(dev->const_mem_size), NULL)) < 0)
    {
      OACC_warning("Can't get maximal constant buffer size: %d\n", err);
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_LOCAL_MEM_SIZE,
      sizeof(cl_ulong), &(dev->loc_mem_size), NULL)) < 0)
    {
      OACC_warning("Can't get local memory size: %d\n", err);
    }

  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_CHAR][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for char: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_CHAR][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for char: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_SHORT][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for short: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_SHORT][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for short: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_INT][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for int: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_INT][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for int: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_LONG][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for long: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_LONG][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for long: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_FLOAT][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for float: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_FLOAT][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for float: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_DOUBLE][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for double: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_DOUBLE][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for double: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_HALF][OACC_IDX_NATIVE]), NULL)) < 0)
    {
      OACC_warning("Can't get native vector width for half: %d\n", err);
    }
  if((err = clGetDeviceInfo(dev->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF,
      sizeof(cl_uint),
      &(dev->vec_size[OACC_IDX_HALF][OACC_IDX_PREF]), NULL)) < 0)
    {
      OACC_warning("Can't get preferred vector width for half: %d\n", err);
    }

}

/* global platforms array initialization */
void
OACC_env_init(void)
{
  cl_uint np = 0, i;
  cl_int err;
  cl_platform_id* pids;
  size_t hidx = 0, aidx = 0;

  LOCK_AQUIRE(OACC_dev_lock);

  if(OACC_platforms != NULL)
    {
      LOCK_RELEASE(OACC_dev_lock);
      return;
    }

  /* make cleanup implicit */
  atexit(OACC_env_fini);
  if((err = clGetPlatformIDs(0, NULL, &np)) < 0 || np == 0)
    {
      OACC_fatal("OpenCL platforms are not found: %d\n", err);
    }

  pids = (cl_platform_id*)alloca(sizeof(cl_platform_id) *np);
  if((err = clGetPlatformIDs(np, pids, NULL)) < 0)
    {
      OACC_fatal("Can't get platform IDs: %d\n", err);
    }

  OACC_nplatforms = np;
  OACC_platforms = (struct OACC_platform_data*)calloc(OACC_nplatforms,
    sizeof(struct OACC_platform_data));
  if(OACC_platforms == NULL)
    {
      OACC_fatal("Not enough memory for platforms info\n");
    }

  for(i = 0; i < OACC_nplatforms; ++i)
    {
      cl_uint nd, j;
      cl_device_id* dids;

      /* get array of devices in this platform */
      OACC_platforms[i].id = pids[i];
      if((err = clGetDeviceIDs(pids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &nd)) < 0
        || nd == 0)
        {
          OACC_platforms[i].ndevs = 0;
          OACC_fatal("Can't get devices for platform: %d\n", err);
        }
      OACC_platforms[i].ndevs = nd;
      dids = (cl_device_id*)alloca(sizeof(cl_device_id) * OACC_platforms[i].ndevs);
      clGetDeviceIDs(pids[i], CL_DEVICE_TYPE_ALL, nd, dids, NULL);
      OACC_platforms[i].devs =
        (struct OACC_device_data*)calloc(OACC_platforms[i].ndevs,
        sizeof(struct OACC_device_data));
      if(OACC_platforms[i].devs == NULL)
        {
          OACC_fatal("Not enough memory for device info\n");
        }

      /* initialize per devcie data, NOTE: context and command queue
        are initialized later in OACC_dev_ini function */
      for(j = 0; j < OACC_platforms[i].ndevs; ++j)
        {
          cl_device_type type;

          if(!device_usable(dids[j]))
            {
              OACC_platforms[i].devs[j].id = 0;
              continue;
            }

          OACC_platforms[i].devs[j].id = dids[j];
          type = get_device_type(dids[j]);
          OACC_platforms[i].devs[j].type = type;
          LOCK_INIT(OACC_platforms[i].devs[j].dev_lock);

          /* cache device info */
          fill_device_caps(&OACC_platforms[i].devs[j]);

          /* count OpenACC devices per type */
          (is_host_device(type)) ? ++OACC_nhost_devs : ++OACC_nacc_devs;
        }

    }

  if(OACC_nacc_devs == 0 && OACC_nhost_devs == 0)
    {
      OACC_fatal("No OpenCL device found\n");
    }

  /*
  create arrays of pointers to map OpenACC host / not devices to OpenCL devices
  */

  if(OACC_nhost_devs > 0)
    {
      OACC_host_devs = (OACC_device_ptr*)calloc(OACC_nhost_devs,
          sizeof(OACC_device_ptr));
      if(OACC_host_devs == NULL)
        {
          OACC_fatal("Not enough memory for host device table\n");
        }
    }
  if(OACC_nacc_devs > 0)
    {
      OACC_acc_devs = (OACC_device_ptr*)calloc(OACC_nacc_devs,
          sizeof(OACC_device_ptr));
      if(OACC_acc_devs == NULL)
        {
          OACC_fatal("Not enough memory for accelerator device table\n");
        }
    }

  for(i = 0; i < OACC_nplatforms; ++i)
    {
      size_t j;

      for(j = 0; j < OACC_platforms[i].ndevs; ++j)
        {
          if(is_host_device(OACC_platforms[i].devs[j].type))
            {
              OACC_host_devs[hidx++] = OACC_platforms[i].devs + j;
            }
          else
            {
              OACC_acc_devs[aidx++] = OACC_platforms[i].devs + j;
            }
        }
    }

  OACC_curr_dev = OACC_default_dev = (OACC_nacc_devs > 0) 
      ? OACC_acc_devs : OACC_host_devs;
  OACC_acc_num = OACC_host_num = OACC_curr_num = 0;

  read_environment(&OACC_curr_dev, &OACC_curr_num);

  LOCK_RELEASE(OACC_dev_lock);
}

void
OACC_env_fini(void)
{
  size_t i, j;

  LOCK_AQUIRE(OACC_dev_lock);

  if(OACC_nplatforms == 0)
    {
      LOCK_RELEASE(OACC_dev_lock);
      return;
    }

  /* free arrays of pointers mapping */
  if(OACC_nhost_devs > 0)
    {
      free(OACC_host_devs);
    }
  if(OACC_nacc_devs > 0)
    {
      free(OACC_acc_devs);
    }

  /* per platform cleanup*/
  for(i = 0; i < OACC_nplatforms; ++i)
    {

      /* per device cleanup*/
      if(OACC_platforms[i].ndevs > 0)
        {
          struct OACC_device_data* pdev = OACC_platforms[i].devs;
          for(j = 0; j < OACC_platforms[i].ndevs; ++j, ++pdev)
            {
              OACC_dev_fini(pdev);
              free(pdev->max_wi_sizes);
              LOCK_DESTROY(pdev->dev_lock);
            }
          free(OACC_platforms[i].devs);
        }
    }

  free(OACC_platforms);
  LOCK_RELEASE(OACC_dev_lock);

  LOCK_DESTROY(OACC_dev_lock);
  LOCK_DESTROY(OACC_async_lock);
  LOCK_DESTROY(OACC_profile_lock);
}

#ifdef __cplusplus
}
#endif
