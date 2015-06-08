#include "config.h"
#include "libgomp.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include "hsa.h"
#include "hsa_ext_finalize.h"

struct __hsa_launch_attributes
{
  /* Number of dimensions the workload has.  Maximum number is 3.  */
  uint32_t ndim;
  /* Size of the grid in the three respective dimensions.  */
  uint32_t gdims[3];
  /* Size of work-groups in the respective dimensions.  */
  uint32_t wdims[3];
  /* Number of arguments of the kernel.  */
  uint32_t nargs;
};

struct hsa_context_info
{
  bool initialized;
  hsa_agent_t agent;

  uint32_t queue_size;
  hsa_isa_t isa;
  hsa_queue_t* sync_command_q;
  hsa_signal_t sync_signal;
};

struct hsa_program_info
{
  bool created, finalized;
  hsa_ext_program_t handle;
  hsa_executable_t executable;
};

struct hsa_image_desc
{
  hsa_ext_module_t module;
  const char *names;
};

struct hsa_kernel_info
{
  bool initialized;
  const char *name;
  hsa_executable_symbol_t symbol;
  uint64_t object;
  uint32_t kernarg_segment_size;
  uint32_t group_segment_size;
  uint32_t private_segment_size;
  void *kernarg_addr;
};

struct hsa_one_image
{
  struct hsa_one_image *next;
  void ***host_functions;
  struct hsa_image_desc *image_descriptor;
  struct hsa_kernel_info *kernels;
};

struct hsa_image_info
{
  bool initialized;
  struct hsa_one_image *first_image;
};

/* Print to stderr information about what is going on when true.  */

static bool debug;

static struct hsa_context_info hsa_context;
static struct hsa_program_info hsa_program;

/* FIXME: Currently we allow only one HSA image (module, HSA object file).
   Once we move to libgomp, it will be taking care of host function <-> hsa
   kernel mapping and we will automatically have more.  */
static struct hsa_image_info image_info;

/* Callback of hsa_iterate_agents, if AGENT is a GPU device, store it to
   hsa_context.agent.  */

static hsa_status_t
find_gpu_agent (hsa_agent_t agent, void *data __attribute__ ((unused)))
{
  hsa_device_type_t device_type;
  hsa_status_t stat = hsa_agent_get_info (agent, HSA_AGENT_INFO_DEVICE,
					  &device_type);
  if (stat == HSA_STATUS_SUCCESS && device_type == HSA_DEVICE_TYPE_GPU)
    {
      hsa_context.agent = agent;
      return HSA_STATUS_INFO_BREAK;
    }
  return HSA_STATUS_SUCCESS;
}

/* Callback of dispatch queue to report errors.  */

static void
queue_callback(hsa_status_t status, hsa_queue_t* queue, void* data) {
  const char* message;
  hsa_status_string (status, &message);
  fprintf(stderr, "Error at queue %llu: %s", (unsigned long long) queue->id,
	  message);
}


/* Initialize context for running HSA kernels.  */

static void
init_hsa_context (void)
{
  hsa_status_t status;

  if (getenv ("HSA_DEBUG"))
    debug = true;
  else
    debug = false;

  status = hsa_init ();
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("HSA initialization failed");
  if (debug)
    fprintf (stderr, "HSA run-time initialized\n");
  status = hsa_iterate_agents (find_gpu_agent, NULL);
  if (status != HSA_STATUS_INFO_BREAK)
    gomp_fatal ("Error searching for a GPU HSA agent");

  if (debug)
    {
      char name[64];
      memset (&name, 0, sizeof (name));
      status = hsa_agent_get_info (hsa_context.agent, HSA_AGENT_INFO_NAME, name);
      if (status != HSA_STATUS_SUCCESS)
	gomp_fatal ("Error requesting the name of the HSA agent");
      fprintf (stderr, "Selected agent %s\n", name);
    }
  status = hsa_agent_get_info(hsa_context.agent, HSA_AGENT_INFO_QUEUE_MAX_SIZE,
			      &hsa_context.queue_size);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Error requesting maximum queue size of the HSA agent");
  if (debug)
    fprintf (stderr, "The maximum queue length is %u\n",
	     (unsigned int) hsa_context.queue_size);
  status = hsa_agent_get_info(hsa_context.agent, HSA_AGENT_INFO_ISA,
			      &hsa_context.isa);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Error querying the ISA of the agent");

  status = hsa_queue_create (hsa_context.agent, hsa_context.queue_size,
			     HSA_QUEUE_TYPE_SINGLE, queue_callback, NULL,
			     UINT32_MAX, UINT32_MAX,
			     &hsa_context.sync_command_q);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Error creating command queue");

  status = hsa_signal_create (1, 0, NULL, &hsa_context.sync_signal);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Error creating the HSA sync signal");

  if (debug)
    fprintf (stderr, "HSA context initialized, queue has id %llu\n",
	     (long long unsigned) hsa_context.sync_command_q->id);

  hsa_context.initialized = true;
}

static void
create_hsa_program (void)
{
  hsa_status_t status;
  assert (hsa_context.initialized);
  status = hsa_ext_program_create (HSA_MACHINE_MODEL_LARGE, HSA_PROFILE_FULL,
				   HSA_DEFAULT_FLOAT_ROUNDING_MODE_DEFAULT,
				   NULL, &hsa_program.handle);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not create an HSA program");
  if (debug)
    fprintf (stderr, "Created a finalizer program\n");

  hsa_program.created = true;
}


static void
finalize_hsa_program (void)
{
  hsa_ext_control_directives_t control_directives;
  hsa_code_object_t code_object;
  hsa_status_t status;

  assert (hsa_program.created);
  memset (&control_directives, 0, sizeof (control_directives));
  status = hsa_ext_program_finalize(hsa_program.handle, hsa_context.isa,
				    HSA_EXT_FINALIZER_CALL_CONVENTION_AUTO,
				    control_directives, "",
				    HSA_CODE_OBJECT_TYPE_PROGRAM,
				    &code_object);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Finalization of the HSA program failed");
  if (debug)
    fprintf (stderr, "Finalization done\n");
  hsa_ext_program_destroy(hsa_program.handle);

  status = hsa_executable_create(HSA_PROFILE_FULL, HSA_EXECUTABLE_STATE_UNFROZEN,
				 "", &hsa_program.executable);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not create HSA executable");
  status = hsa_executable_load_code_object(hsa_program.executable,
					   hsa_context.agent, code_object, "");
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not add a code object to the HSA executable");
  status = hsa_executable_freeze(hsa_program.executable, "");
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not add a code object to the HSA executable");

  if (debug)
    fprintf (stderr, "Froze HSA executable with the finalized code object\n");

  hsa_program.finalized = true;
}

static void
init_hsa_image (struct hsa_image_info *ii)
{
  hsa_status_t status;

  if (hsa_program.finalized)
    gomp_fatal ("Sorry, re-finalization not yet supported.");
  if (!hsa_program.created)
    create_hsa_program ();
  struct hsa_one_image *img = ii->first_image;
  while (img)
    {
      const char *p;
      int count = 0;
      struct hsa_kernel_info *kernel;

      status = hsa_ext_program_add_module (hsa_program.handle,
					   img->image_descriptor->module);
      if (status != HSA_STATUS_SUCCESS)
	gomp_fatal ("Could not add a module to the HSA program");
      if (debug)
	fprintf (stderr, "Added a module to the HSA program\n");

      p = img->image_descriptor->names;
      while (*p)
	{
	  count++;
	  do
	    p++;
	  while (*p);
	  p++;
	}

      if (debug)
	fprintf (stderr, "Encountered %d kernels an image\n", count);
      img->kernels = gomp_malloc_cleared (sizeof (struct hsa_kernel_info)
					  * count);
      if (!img->kernels)
	gomp_fatal ("Could not allocate memory for HSA kertnel descriptors");

      p = img->image_descriptor->names;
      kernel = img->kernels;
      while (*p)
	{
	  kernel->name = p;
	  kernel++;
	  do
	    p++;
	  while (*p);
	  p++;
	}
      img = img->next;
    }

  ii->initialized = true;
}

/* Callback of hsa_agent_iterate_regions.  Determines if a memory region can be
   used for kernarg allocations.  */

static hsa_status_t get_kernarg_memory_region(hsa_region_t region, void* data)
{
  hsa_status_t status;
  hsa_region_segment_t segment;

  status = hsa_region_get_info (region, HSA_REGION_INFO_SEGMENT, &segment);
  if (status != HSA_STATUS_SUCCESS)
    return status;
  if (segment != HSA_REGION_SEGMENT_GLOBAL)
    return HSA_STATUS_SUCCESS;

  uint32_t flags;
  status = hsa_region_get_info (region, HSA_REGION_INFO_GLOBAL_FLAGS, &flags);
  if (status != HSA_STATUS_SUCCESS)
    return status;
  if (flags & HSA_REGION_GLOBAL_FLAG_KERNARG)
    {
      hsa_region_t* ret = (hsa_region_t*) data;
      *ret = region;
      return HSA_STATUS_INFO_BREAK;
    }
  return HSA_STATUS_SUCCESS;
}


static void
init_hsa_kernel (struct hsa_kernel_info *ki, struct hsa_image_info *ii)
{
  hsa_status_t status;

  status = hsa_executable_get_symbol (hsa_program.executable, NULL,
				      ki->name, hsa_context.agent, 0,
				      &ki->symbol);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not find symbol for kernel in the code object");
  if (debug)
    fprintf (stderr, "Located kernel %s\n", ki->name);

  status = hsa_executable_symbol_get_info
    (ki->symbol, HSA_EXECUTABLE_SYMBOL_INFO_KERNEL_OBJECT, &ki->object);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not extract a kernel object from its symbol");
  status = hsa_executable_symbol_get_info
    (ki->symbol, HSA_EXECUTABLE_SYMBOL_INFO_KERNEL_KERNARG_SEGMENT_SIZE,
     &ki->kernarg_segment_size);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not get infor about kernel argument size");
  status = hsa_executable_symbol_get_info
    (ki->symbol, HSA_EXECUTABLE_SYMBOL_INFO_KERNEL_GROUP_SEGMENT_SIZE,
     &ki->group_segment_size);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not get info about kernel group segment size");
  status = hsa_executable_symbol_get_info
    (ki->symbol, HSA_EXECUTABLE_SYMBOL_INFO_KERNEL_PRIVATE_SEGMENT_SIZE,
     &ki->private_segment_size);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not get info about kernel private segment size");

  hsa_region_t kernarg_region;
  kernarg_region.handle = (uint64_t) -1;
  status = hsa_agent_iterate_regions (hsa_context.agent,
				      get_kernarg_memory_region,
				      &kernarg_region);
  if (kernarg_region.handle == (uint64_t) -1)
    gomp_fatal ("Could not find suitable memory region for kernel arguments");

  /* Allocate the kernel argument buffer from the correct region.  */
  status = hsa_memory_allocate (kernarg_region, ki->kernarg_segment_size,
				&ki->kernarg_addr);
  if (status != HSA_STATUS_SUCCESS)
    gomp_fatal ("Could not allocate memory for kernel arguments");

  if (debug)
    {
      fprintf (stderr, "Kernel structure for %s fully initialized\n", ki->name);
      fprintf (stderr, "  group_segment_size: %u\n",
	       (unsigned) ki->group_segment_size);
      fprintf (stderr, "  private_segment_size: %u\n",
	       (unsigned) ki->private_segment_size);
      fprintf (stderr, "  kernarg_segment_size: %u\n",
	       (unsigned) ki->kernarg_segment_size);
      fprintf (stderr, "  kernarg_addr: %p\n", ki->kernarg_addr);
    }
  ki->initialized = true;
}

void
__hsa_launch_kernel (void *host_fn, struct __hsa_launch_attributes *attrs,
		     uint64_t *args)
{
  struct hsa_kernel_info *ki = NULL;
  hsa_kernel_dispatch_packet_t *packet;
  struct hsa_one_image *img;
  void **hf;
  uint16_t header;
  uint64_t index;
  int i;

  if (!image_info.first_image)
    gomp_fatal ("Call to launch_kernel before register_image");

  if (!hsa_context.initialized)
    init_hsa_context ();
  if (!image_info.initialized)
    init_hsa_image (&image_info);
  if (!hsa_program.finalized)
    finalize_hsa_program ();

  img = image_info.first_image;
  while (img)
    {
      bool found = false;
      i = 0;
      hf = img->host_functions[0];
      while (hf != img->host_functions[1])
	{
	  if (*hf == host_fn)
	    {
	      found = true;
	      break;
	    }
	  hf++;
	  i++;
	}
      if (found)
	{
	  if (debug)
	    fprintf (stderr, "Identified kernel %d\n", i);
	  ki = &img->kernels[i];
	  break;
	}
      else
	img = img->next;
    }
  if (!ki)
    gomp_fatal ("Could not map host function to an HSA kernel");

  if (!ki->initialized)
    init_hsa_kernel (ki, &image_info);

  index  = hsa_queue_add_write_index_relaxed (hsa_context.sync_command_q, 1);
  if (debug)
    fprintf (stderr, "Got AQL index %llu\n", (long long int) index);
  assert ((index & ~(hsa_context.sync_command_q->size - 1)) == 0);
  packet = ((hsa_kernel_dispatch_packet_t*) hsa_context
	    .sync_command_q->base_address) + index;
  hsa_signal_store_relaxed (hsa_context.sync_signal, 1);

  memset (((uint8_t *)packet) + 4, 0, sizeof (*packet) - 4);
  packet->setup  |= (uint16_t) attrs->ndim
    << HSA_KERNEL_DISPATCH_PACKET_SETUP_DIMENSIONS;
  packet->grid_size_x = attrs->gdims[0];
  packet->workgroup_size_x = attrs->wdims[0];
  if (attrs->ndim > 1)
    {
      packet->grid_size_y = attrs->gdims[1];
      packet->workgroup_size_y = attrs->wdims[1];
    }
  else
    {
      packet->grid_size_y = 1;
      packet->workgroup_size_y = 1;
    }
  if (attrs->ndim > 2)
    {
      packet->grid_size_z = attrs->gdims[2];
      packet->workgroup_size_z = attrs->wdims[2];
    }
  else
    {
      packet->grid_size_z = 1;
      packet->workgroup_size_z = 1;
    }

  packet->private_segment_size = ki->private_segment_size;
  packet->group_segment_size = ki->group_segment_size;
  packet->kernel_object = ki->object;
  packet->kernarg_address = ki->kernarg_addr;
  packet->completion_signal = hsa_context.sync_signal;

  if (debug)
    fprintf (stderr, "Copying %u arguments, total %llu bytes, from %p\n",
	     (unsigned) attrs->nargs, 
	     (unsigned long long) sizeof(uint64_t) * attrs->nargs,
	     (void *) args);
  memcpy (ki->kernarg_addr, args, sizeof(uint64_t) * attrs->nargs);

  header = HSA_PACKET_TYPE_KERNEL_DISPATCH << HSA_PACKET_HEADER_TYPE;
  header |= HSA_FENCE_SCOPE_SYSTEM << HSA_PACKET_HEADER_ACQUIRE_FENCE_SCOPE;
  header |= HSA_FENCE_SCOPE_SYSTEM << HSA_PACKET_HEADER_RELEASE_FENCE_SCOPE;

  if (debug)
    fprintf (stderr, "Going to dispatch the kernel\n");

  __atomic_store_n ((uint16_t*)(&packet->header), header, __ATOMIC_RELEASE);
  hsa_signal_store_relaxed (hsa_context.sync_command_q->doorbell_signal, index);

  if (debug)
    fprintf (stderr, "Kernel dispatched, waiting for completion\n");

  hsa_signal_wait_acquire(hsa_context.sync_signal, HSA_SIGNAL_CONDITION_LT, 1,
			  UINT64_MAX, HSA_WAIT_STATE_BLOCKED);
  if (debug)
    fprintf (stderr, "Kernel returned\n");
}

void
__hsa_register_image (void *host_table,
		      /* enum offload_target_type target_type,*/
                      void *target_data)
{
  /*
  fprintf (stderr, "__hsa_register_image %p %p called\n",
           host_table, target_data);
  */
  if (!host_table || !target_data)
    gomp_fatal ("Invalid image registration parameters");

  struct hsa_one_image *img;
  img = gomp_malloc_cleared (sizeof (struct hsa_one_image));
  img->host_functions = host_table;
  img->image_descriptor = target_data;
  img->next = image_info.first_image;
  image_info.first_image = img;
}
