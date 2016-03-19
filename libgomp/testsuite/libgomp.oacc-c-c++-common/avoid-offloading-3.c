/* Test that a user can override the compiler's "avoid offloading"
   decision.  */

/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#include <openacc.h>

int main(void)
{
  int x, y;

#pragma acc data copyout(x, y)
#pragma acc kernels
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
