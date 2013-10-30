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
/* procedures dealing with OpenCL programs/kernels */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <malloc.h>
#define alloca _alloca
#endif
/* read source for PROG_NAME form file and returns allocated buffer
(CALLER must FREE) and size of program
*/
static char*
OACC_get_prog_source(const char* prog_name, size_t* psize)
{
  FILE *fp = NULL;
  char* buf = NULL;
  size_t size;

  if((fp = fopen(prog_name, "r")) == NULL)
    {
      OACC_fatal("Can't open file '%s'\n", prog_name);
      return NULL;
    }
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  buf = (char*)malloc(size + 1);
  if(buf == NULL)
    {
      OACC_fatal("Not enough memory for %d bytes buffer\n", size+1);
      fclose(fp);
      return NULL;
    }
  fread(buf, sizeof(char), size, fp);
  buf[size] = '\0';
  fclose(fp);

  if(psize != NULL)
    {
      *psize = size;
    }
  return buf;
}

/* initialize program object in given context:
  get source, create OpenCL object, build*/
static struct OACC_program_data*
OACC_load_prog(const char* prog_name, cl_context ctx)
{
  char* src = NULL;
  struct OACC_program_data* prog = NULL;
  cl_int err;
  size_t size = 0;

  src = OACC_get_prog_source(prog_name, &size);
  if(src == NULL)
    {
      return NULL;
    }

  prog = (struct OACC_program_data*)calloc(1, sizeof(struct OACC_program_data));
  prog->prog = clCreateProgramWithSource(ctx, 1, (const char**)&src, &size,
                                          &err);
  if(err < 0)
    {
      OACC_fatal("Can't create program: %d\n", err);
      free(prog);
      free(src);
      return NULL;
    }

  err = clBuildProgram(prog->prog, 0, NULL, NULL, NULL, NULL);
  if(err < 0)
    {
      char* the_log = NULL;
      clGetProgramBuildInfo(prog->prog, OACC_curr_dev[OACC_curr_num]->id,
                            CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
      the_log = (char*)malloc(size + 1);
      clGetProgramBuildInfo(prog->prog, OACC_curr_dev[OACC_curr_num]->id,
                              CL_PROGRAM_BUILD_LOG, size+1, the_log, NULL);
      the_log[size] = '\0';
      OACC_fatal("Error building program: %d\n%s\n", err, the_log);
    }

  free(src);
  prog->name = strdup(prog_name);

  return prog;
}

/* find desired program with name PROG_NAME in given context */
static struct OACC_program_data*
OACC_get_program(const char* prog_name, struct OACC_device_data* pdev)
{
  struct OACC_program_data *prog = NULL;

  for(prog = pdev->prog_head; prog != NULL; prog = prog->next)
    {
      if(strcmp(prog_name, prog->name) == 0)
        {
          return prog;
        }
    }

  LOCK_AQUIRE(pdev->dev_lock);
  prog = OACC_load_prog(prog_name, pdev->ctx);
  if(prog != NULL)
    {
      prog->next = pdev->prog_head;
      pdev->prog_head = prog;
    }
  LOCK_RELEASE(pdev->dev_lock);
  return prog;
}

/* initialize kernel data: create, allocated space for arguments */
static struct OACC_kernel_data*
OACC_load_kernel(struct OACC_program_data* prog, const char* kern_name)
{
  cl_int err;
  struct OACC_kernel_data* kern = NULL;

  kern = (struct OACC_kernel_data*)calloc(1, sizeof(struct OACC_kernel_data));
  if(kern == NULL)
    {
      OACC_fatal("Not enough memory for kernels\n");
    }
  kern->kern = clCreateKernel(prog->prog, kern_name, &err);
  if(err < 0)
    {
      OACC_fatal("Can't create kernel: %d\n", err);
    }

  kern->name = strdup(kern_name);
  err = clGetKernelInfo(kern->kern, CL_KERNEL_NUM_ARGS, sizeof(size_t),
                        &kern->nargs, NULL);
  if(err < 0)
    {
      OACC_fatal("Can't get # of kernel args: %d\n", err);
    }
  kern->args = (OACC_buffer_ptr*)calloc(kern->nargs, sizeof(OACC_buffer_ptr));
  if(kern->args == NULL)
    {
      OACC_fatal("Not enough memory for kernel arguments\n");
    }
  return kern;
}
/* find or load kernel */
static struct OACC_kernel_data*
OACC_get_kernel(struct OACC_program_data* prog, const char* kern_name,
                OACC_device_ptr pdev)
{
  struct OACC_kernel_data* kern = NULL;

  for(kern = prog->kern_head; kern != NULL; kern = kern->next)
    {
      if(strcmp(kern->name, kern_name) == 0)
        {
          return kern;
        }
    }

  LOCK_AQUIRE(pdev->dev_lock);
  kern = OACC_load_kernel(prog, kern_name);
  if(kern != NULL)
    {
      kern->next = prog->kern_head;
      prog->kern_head = kern;
    }
  LOCK_RELEASE(pdev->dev_lock);
  return kern;
}

/* get kernel KERN_NAME from program PROG_NAME */
struct OACC_kernel_data*
OACC_find_kernel(const char* prog_name, const char* kern_name)
{
  struct OACC_program_data *prog = NULL;
  OACC_device_ptr dev_ptr;

  if(OACC_curr_dev == NULL)
    {
      return NULL;
    }
  dev_ptr = OACC_curr_dev[OACC_curr_num];
  prog = OACC_get_program(prog_name, dev_ptr);
  if(prog == NULL)
    {
      return NULL;
    }
  return OACC_get_kernel(prog, kern_name, dev_ptr);
}

#ifdef __cplusplus
}
#endif
