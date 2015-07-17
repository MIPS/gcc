/* { dg-do run } */

#include <stdlib.h>
#include <openacc.h>

#define N 100

int
main()
{
  int a, old_a,  b[N], c, d, i;
  int n = acc_get_device_type () == acc_device_nvidia ? N : 1;

  a = 5;

  for (i = 0; i < n; i++)
    b[i] = -1;

  #pragma acc parallel num_gangs (n) firstprivate (a)
  #pragma acc loop gang
  for (i = 0; i < n; i++)
    {
      a = a + i;
      b[i] = a;
    }

  for (i = 0; i < n; i++)
    if (a + i != b[i])
      abort ();

  #pragma acc data copy (a)
  {
    #pragma acc parallel firstprivate (a) copyout (c)
    {
      a = 10;
      c = a;
    }

    /* This version of 'a' should still be 5.  */
    #pragma acc parallel copyout (d) present (a)
    {
      d = a;
    }

  }

  if (c != 10)
    abort ();

  if (d != 5)
    abort ();

  return 0;
}
