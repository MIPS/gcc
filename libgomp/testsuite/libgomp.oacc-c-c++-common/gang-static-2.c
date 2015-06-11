/* { dg-do run { target openacc_nvidia_accel_selected } } */

#include <assert.h>

#define N 100

int test(int *a, int sarg)
{
  int i, j, gang;

  if (sarg == 0)
    sarg = 1;

  for (i = 0, gang = 0; i < N; i+=sarg, gang++)
    {
      for (j = 0; j < sarg; j++)
	assert (a[i] == gang % 10);
    }
}

int
main ()
{
  int a[N];
  int i;

#pragma acc parallel loop gang (static:*) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test (a, 0);

#pragma acc parallel loop gang (static:1) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test (a, 1);

#pragma acc parallel loop gang (static:2) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test (a, 2);

#pragma acc parallel loop gang (static:5) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test (a, 5);

#pragma acc parallel loop gang (static:20) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test (a, 20);

  return 0;
}
