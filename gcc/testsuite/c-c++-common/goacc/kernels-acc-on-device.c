/* { dg-additional-options "-O2" } */

#if __cplusplus
extern "C" {
#endif

#if __cplusplus >= 201103
# define __GOACC_NOTHROW noexcept
#elif __cplusplus
# define __GOACC_NOTHROW throw ()
#else /* Not C++ */
# define __GOACC_NOTHROW __attribute__ ((__nothrow__))
#endif

typedef enum acc_device_t { acc_device_X = 123 } acc_device_t;
int acc_on_device (int) __GOACC_NOTHROW;

#if __cplusplus
}
#endif

#define N 32

void
foo (float *a, float *b)
{
#pragma acc kernels copyin(a[0:N]) copyout(b[0:N])
  {
    int ii;

    for (ii = 0; ii < N; ii++)
      {
	if (acc_on_device (acc_device_X))
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
	if (acc_on_device (acc_device_X))
	  b[ii] = a[ii] + 2;
	else
	  b[ii] = a[ii];
      }
  }
}
