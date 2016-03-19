/* Test that a user can override the compiler's "avoid offloading"
   decision.  */

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
#pragma acc kernels /* { dg-warning "OpenACC kernels construct will be executed sequentially; will by default avoid offloading to prevent data copy penalty" "" { target openacc_nvidia_accel_selected } } */
  *((volatile int *) &x) = 33, y = acc_on_device (acc_device_host);

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
