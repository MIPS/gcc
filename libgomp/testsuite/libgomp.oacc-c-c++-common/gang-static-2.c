/* { dg-do run { target openacc_nvidia_accel_selected } } */

#include <assert.h>

#define N 100

int
test_static(int *a, int num_gangs, int sarg)
{
  int i, j;

  if (sarg == 0)
    sarg = 1;

  for (i = 0; i < N / sarg; i++)
    for (j = 0; j < sarg; j++)
      assert (a[i*sarg+j] == i % num_gangs);
}

int
test_nonstatic(int *a, int gangs)
{
  int i, j;

  for (i = 0; i < N; i+=gangs)
    for (j = 0; j < gangs; j++)
      assert (a[i+j] == i/gangs);
}

int
main ()
{
  int a[N];
  int i;

#pragma acc parallel loop gang (static:*) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test_nonstatic (a, 10);

#pragma acc parallel loop gang (static:1) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test_static (a, 10, 1);

#pragma acc parallel loop gang (static:2) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test_static (a, 10, 2);

#pragma acc parallel loop gang (static:5) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test_static (a, 10, 5);

#pragma acc parallel loop gang (static:20) num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test_static (a, 10, 20);

  /* Non-static gang.  */
#pragma acc parallel loop gang num_gangs (10)
  for (i = 0; i < 100; i++)
    a[i] = __builtin_GOACC_ctaid (0);

  test_nonstatic (a, 10);

  return 0;
}
