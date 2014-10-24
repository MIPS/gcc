#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "okra.h"

typedef okra_status_t (*okra_get_context_func_t)(okra_context_t**);
typedef okra_status_t (*okra_kernel_create_from_binary_func_t)(okra_context_t *, const char *, size_t , const char *, okra_kernel_t **);
typedef okra_status_t (*okra_push_pointer_func_t)(okra_kernel_t* , void* );
typedef okra_status_t (*okra_execute_kernel_func_t)(okra_context_t*, okra_kernel_t* , okra_range_t* );
typedef okra_status_t (*okra_clear_args_func_t)(okra_kernel_t* );
typedef okra_status_t (*okra_dispose_kernel_func_t)(okra_kernel_t*);

static void *okralib;
static okra_get_context_func_t     _okra_get_context;
static okra_kernel_create_from_binary_func_t   _okra_kernel_create_from_binary;
static okra_push_pointer_func_t    _okra_push_pointer;
static okra_execute_kernel_func_t  _okra_execute_kernel;
static okra_clear_args_func_t      _okra_clear_args;

/* Returns false on error.  */
static bool
loadokra (void)
{
  if (okralib)
    return okralib != (void*) -1;
  okralib = dlopen("libokra_x86_64.so", RTLD_LAZY);
  if (!okralib)
    {
      okralib = (void*) -1;
      fprintf(stderr, "Unable to load libokra_x86_64.so\n");
      return false;
    }
   _okra_get_context         = (okra_get_context_func_t) dlsym(okralib, "okra_get_context");
   _okra_kernel_create_from_binary = (okra_kernel_create_from_binary_func_t)dlsym(okralib, "okra_create_kernel_from_binary");
   _okra_push_pointer        = (okra_push_pointer_func_t) dlsym(okralib, "okra_push_pointer");
   _okra_execute_kernel      = (okra_execute_kernel_func_t) dlsym(okralib, "okra_execute_kernel");
   _okra_clear_args          = (okra_clear_args_func_t) dlsym(okralib, "okra_clear_args");

   if (!_okra_get_context
       || !_okra_kernel_create_from_binary
       || !_okra_push_pointer
       || !_okra_execute_kernel
       || !_okra_clear_args)
     {
       dlclose (okralib);
       okralib = (void*) -1;
       fprintf (stderr, "Cannot find OKRA symbols\n");
       return false;
     }

   return true;
}

typedef union __hsa_kernelarg_
{
  void* addr;             ///< pointer to a buffer
  int32_t s32value;       ///< signed 32 bit value
  uint32_t u32value;      ///< unsigned 32 bit value
  float fvalue;           ///< float value
  double dvalue;          ///< double value
  int64_t s64value;       ///< signed 64 bit value
  uint64_t u64value;      ///< unsigned 64 bit value
} __hsa_kernelarg;

typedef struct __hsa_launch_attrs_
{
  uint64_t flags;
  uint64_t grid[3];
  uint64_t group[3];
} __hsa_launch_attrs;

typedef struct __hsa_kernel_desc_
{
  const char *filename;
  const char *name;
  uint64_t nargs;
  okra_context_t *kernel;
  okra_context_t *context;
} __hsa_kernel_desc;

typedef okra_range_t __hsa_launch_range;

void * __hsa_launch_kernel (__hsa_kernel_desc *, __hsa_launch_range *,
			    __hsa_kernelarg *);

void *
__hsa_launch_kernel (__hsa_kernel_desc * _kd, __hsa_launch_range *range_p,
		     __hsa_kernelarg *args)
{
  okra_status_t status;
  okra_context_t *context;
  okra_kernel_t *kernel;
  okra_range_t range;
  unsigned int i = 0;
  static int debug = 0;

  if (!loadokra ())
    return NULL;

  if (!debug)
    {
      if (getenv ("HSA_DEBUG"))
	debug = 1;
      else
	debug = -1;
    }

  if (_kd->context)
    context = _kd->context ;
  else {
    status = _okra_get_context(&context);
    if (status != OKRA_SUCCESS)
      {
	fprintf (stderr, "Unable to create context\n");
	return NULL;
      }
      _kd->context = context ;
    }
  if (_kd->kernel)
    {
      kernel = _kd->kernel;
    }
  else
    {
      size_t size = 1;
      const char* pfile;
      const char* fileName = _kd->filename;
      if (_kd->filename[0] == 0)
	fileName = "hsakernel.o";
      pfile = (const char *) fopen (fileName, "rb");
      if (!pfile)
	{
	  fprintf (stderr, "Unable to open file %s\n", fileName);
	  return NULL;
	}
      status = _okra_kernel_create_from_binary(context, pfile, size, _kd->name, &kernel);
      fclose((FILE *)pfile);
      if (status != OKRA_SUCCESS)
	{
	  fprintf (stderr, "Unable to create Kernel\n");
	  return NULL;
	}
      _kd->kernel = kernel;
    }

  _okra_clear_args (kernel);
  for (; i < _kd->nargs; i++) {
      void *cur_args = args[i].addr;
      _okra_push_pointer (kernel, cur_args);
  }
  /* set launch dimensions */
  range.dimension = 1;
  range.global_size[0] = 256;
  range.group_size[0] = 16;
  if (!range_p)
    {
      range.dimension = 1;
      range.global_size[0] = 256;
      range.group_size[0] = 16;
      range_p = &range;
    }
  if (debug > 0)
    {
      fprintf (stderr, "HSA: launching kernel %s\n", _kd->name);
      fprintf (stderr, "dim: %u, s0: %u, g0: %u, r: %u\n", range_p->dimension,
	       range_p->global_size[0], range_p->group_size[0],
	       range_p->reserved);
    }
  status = _okra_execute_kernel (context, kernel, range_p);
  if (status != OKRA_SUCCESS)
    {
      fprintf (stderr, "Failed to launch kernel\n");
      return NULL;
    }
  return kernel;
}
