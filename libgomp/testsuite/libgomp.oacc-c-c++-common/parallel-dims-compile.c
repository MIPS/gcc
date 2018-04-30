/* { dg-do "link" } */
/* { dg-additional-options "-foffload-force" } */

#include <limits.h>
#include <openacc.h>

/* TODO: "(int) acc_device_*" casts because of the C++ acc_on_device wrapper
   not behaving as expected for -O0.  */
#pragma acc routine seq
static unsigned int __attribute__ ((optimize ("O2"))) acc_gang ()
{
  if (acc_on_device ((int) acc_device_host))
    return 0;
  else if (acc_on_device ((int) acc_device_nvidia))
    {
      unsigned int r;
      asm volatile ("mov.u32 %0,%%ctaid.x;" : "=r" (r));
      return r;
    }
  else
    __builtin_abort ();
}

#pragma acc routine seq
static unsigned int __attribute__ ((optimize ("O2"))) acc_worker ()
{
  if (acc_on_device ((int) acc_device_host))
    return 0;
  else if (acc_on_device ((int) acc_device_nvidia))
    {
      unsigned int r;
      asm volatile ("mov.u32 %0,%%tid.y;" : "=r" (r));
      return r;
    }
  else
    __builtin_abort ();
}

#pragma acc routine seq
static unsigned int __attribute__ ((optimize ("O2"))) acc_vector ()
{
  if (acc_on_device ((int) acc_device_host))
    return 0;
  else if (acc_on_device ((int) acc_device_nvidia))
    {
      unsigned int r;
      asm volatile ("mov.u32 %0,%%tid.x;" : "=r" (r));
      return r;
    }
  else
    __builtin_abort ();
}


int main ()
{
  acc_init (acc_device_default);

  /* GR, WP, VS.  */
  {
    /* We try with an outrageously large value. */
#define WORKERS 2 << 20
    int workers_actual = WORKERS;
    int gangs_min, gangs_max, workers_min, workers_max, vectors_min, vectors_max;
    gangs_min = workers_min = vectors_min = INT_MAX;
    gangs_max = workers_max = vectors_max = INT_MIN;
#pragma acc parallel copy (workers_actual) /* { dg-warning "using num_workers \\(32\\), ignoring 2097152" "" { target openacc_nvidia_accel_selected } } */ \
  num_workers (WORKERS)
    {
      if (acc_on_device (acc_device_host))
	{
	  /* We're actually executing with num_workers (1).  */
	  workers_actual = 1;
	}
      else if (acc_on_device (acc_device_nvidia))
	{
	  /* The GCC nvptx back end enforces num_workers (32).  */
	  workers_actual = 32;
	}
      else
	__builtin_abort ();
#pragma acc loop worker reduction (min: gangs_min, workers_min, vectors_min) reduction (max: gangs_max, workers_max, vectors_max)
      for (int i = 100 * workers_actual; i > -100 * workers_actual; --i)
	{
	  gangs_min = gangs_max = acc_gang ();
	  workers_min = workers_max = acc_worker ();
	  vectors_min = vectors_max = acc_vector ();
	}
    }
    if (workers_actual < 1)
      __builtin_abort ();
    if (gangs_min != 0 || gangs_max != 0
	|| workers_min != 0 || workers_max != workers_actual - 1
	|| vectors_min != 0 || vectors_max != 0)
      __builtin_abort ();
#undef WORKERS
  }

  return 0;
}
