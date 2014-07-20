#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "okra.h"

typedef okra_status_t (*okra_get_context_func_t)(okra_context_t**);
typedef okra_status_t (*okra_kernel_create_func_t)( okra_context_t* ,const char *, const char *,okra_kernel_t **);
typedef okra_status_t (*okra_push_pointer_func_t)(okra_kernel_t* , void* );
typedef okra_status_t (*okra_execute_kernel_func_t)(okra_context_t*, okra_kernel_t* , okra_range_t* );
typedef okra_status_t (*okra_clear_args_func_t)(okra_kernel_t* );
typedef okra_status_t (*okra_dispose_kernel_func_t)(okra_kernel_t*);

static void *okralib;
static okra_get_context_func_t     _okra_get_context;
static okra_kernel_create_func_t   _okra_kernel_create;
static okra_push_pointer_func_t    _okra_push_pointer;
static okra_execute_kernel_func_t  _okra_execute_kernel;
static okra_clear_args_func_t      _okra_clear_args;

static char *buildStringFromSourceFile (const char* fname)
{
  FILE *fp;
  long filesize = 0;
  char *str;
  fp=fopen(fname, "r");
  if (!fp)
    {
      printf ("Unable to open the HSAIL string file %s\n", fname);
      return NULL;
    }
  fseek(fp,0,SEEK_END); 
  filesize=ftell(fp); 
  rewind(fp);
  str = (char *)malloc(filesize + 1);
  fread( str, filesize , 1 , fp) ;
  str[filesize] = (char)0;  // terminate
  fclose(fp);
  return str;
}

/* Returns false on error.  */
static bool
loadokra (void)
{
  if (okralib)
    return okralib != (void*)-1;
  okralib = dlopen("libokra_x86_64.so", RTLD_LAZY);
  if (!okralib)
    {
      okralib = (void*)-1;
      printf("...unable to load libokra_x86_64.so\n");
      return false;
    }
   _okra_get_context         = (okra_get_context_func_t)dlsym(okralib, "okra_get_context");
   _okra_kernel_create       = (okra_kernel_create_func_t)dlsym(okralib, "okra_create_kernel");
   _okra_push_pointer        = (okra_push_pointer_func_t)dlsym(okralib, "okra_push_pointer");
   _okra_execute_kernel      = (okra_execute_kernel_func_t)dlsym(okralib, "okra_execute_kernel");
   _okra_clear_args          = (okra_clear_args_func_t)dlsym(okralib, "okra_clear_args");

   if (!_okra_get_context
       || !_okra_kernel_create
       || !_okra_push_pointer
       || !_okra_execute_kernel
       || !_okra_clear_args)
     {
       dlclose (okralib);
       okralib = (void*)-1;
       printf ("can't find OKRA symbols\n");
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
  void *kernel;
} __hsa_kernel_desc;

static okra_context_t *okracontext;

void * __hsa_launch_kernel (__hsa_kernel_desc *, __hsa_launch_attrs *attrs,
			    __hsa_kernelarg *args);

void *
__hsa_launch_kernel (__hsa_kernel_desc * _kd, __hsa_launch_attrs *attrs,
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

  if (_kd->kernel)
    {
      kernel = _kd->kernel;
      context = okracontext;
    }
  else
    {
      char* hsailStr = NULL;
      const char* fileName = _kd->filename;
      if (_kd->filename[0] == 0)
	fileName = "hsakernel.hsail"; 
      status = _okra_get_context(&context);
      if (status != OKRA_SUCCESS)
	{
	  printf( "Unable to create context\n"); 
	  return NULL;
	}
      hsailStr= buildStringFromSourceFile(fileName);
      status = _okra_kernel_create(context, hsailStr, _kd->name, &kernel);
      free(hsailStr);
      if (status != OKRA_SUCCESS)
	{
	  printf( "Unable to create Kernel \n"); 
	  return NULL;
	}
      _kd->kernel = kernel;
      okracontext = context;
    }

  _okra_clear_args (kernel);
  for (; i < _kd->nargs; i++) {
      void *cur_args = args[i].addr;
      _okra_push_pointer (kernel, cur_args);
  }
  /* set launch dimensions */
  range.dimension = 1;
  range.global_size[0] = 1;
  range.group_size[0] = 1;
  if (debug > 0)
    printf ("HSA: launching kernel %s\n", _kd->name);
  status = _okra_execute_kernel (context, kernel, &range);
  if (status != OKRA_SUCCESS)
    {
      printf( "Failed to launch kernel \n"); 
      return NULL;
    }
  return kernel; 
}
