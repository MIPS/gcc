/* Test that a user can override the compiler's "avoid offloading"
   decision.  */

/* { dg-additional-options "-fopt-info-optimized-omp" } */

#include <openacc.h>

int main(void)
{
  /* Override the compiler's "avoid offloading" decision.  */
  acc_device_t d;
#if defined ACC_DEVICE_TYPE_nvidia
  d = acc_device_nvidia;
#elif defined ACC_DEVICE_TYPE_host
  d = acc_device_host;
#else
# error Not ported to this ACC_DEVICE_TYPE
#endif
  acc_init (d);

  int x, y;

#pragma acc data copyout(x, y)
#pragma acc kernels
  *((volatile int *) &x) = 33, y = acc_on_device (acc_device_host); /* { dg-warning "note: beginning .gang-single. region in OpenACC .kernels. construct" } */

  /* The following will trigger "avoid offloading".  */
#pragma acc kernels
  {
#pragma acc loop auto /* { dg-warning "note: forwarded loop nest in OpenACC .kernels. construct to .parloops. for analysis" } */
    /* { dg-warning "OpenACC kernels construct will be executed sequentially; will by default avoid offloading to prevent data copy penalty" "" { target { openacc_nvidia_accel_selected && opt_levels_2_plus } } 30 } */
    /* { dg-warning "note: assigned OpenACC seq loop parallelism" "" { target *-*-* } 30 } */
    for (int i = 0; i < x; ++i)
      if (x == 0)
	x = 1;
  }

  if (x != 33)
    __builtin_abort();
#if defined ACC_DEVICE_TYPE_nvidia
  if (y != 0)
    __builtin_abort();
#else
  if (y != 1)
    __builtin_abort();
#endif

  return 0;
}
