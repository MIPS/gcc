/* Check if worker-single variables get broadcastd to vectors.  */

/* { dg-do run } */

#include <assert.h>
#include <math.h>

#define N 32

#pragma acc routine
float
some_val ()
{
  return 2.71;
}

int
main ()
{
  float threads[N], v1 = 3.14;

  for (int i = 0; i < N; i++)
    threads[i] = -1;

#pragma acc parallel num_gangs (1) vector_length (32) copy (v1)
  {
    float val = some_val ();

#pragma acc loop vector
    for (int i = 0; i < N; i++)
      threads[i] = val + v1*i;
  }

  for (int i = 0; i < N; i++)
    assert (fabs (threads[i] - (some_val () + v1*i)) < 0.0001);

  return 0;
}
