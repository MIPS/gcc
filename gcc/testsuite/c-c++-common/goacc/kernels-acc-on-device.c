/* { dg-additional-options "-O2" } */

#include <openacc.h>

#define N 32

void
foo (float *a, float *b)
{
  float exp;
  int i;
  int n;

#pragma acc kernels copyin(a[0:N]) copyout(b[0:N])
  {
    int ii;

    for (ii = 0; ii < N; ii++)
      {
	if (acc_on_device (acc_device_host))
	  b[ii] = a[ii] + 1;
	else
	  b[ii] = a[ii];
      }
  }

#pragma acc kernels copyin(a[0:N]) copyout(b[0:N])
  {
    int ii;

    for (ii = 0; ii < N; ii++)
      {
	if (acc_on_device (acc_device_host))
	  b[ii] = a[ii] + 2;
	else
	  b[ii] = a[ii];
      }
  }
}
