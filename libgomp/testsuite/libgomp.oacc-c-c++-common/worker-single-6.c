/* { dg-additional-options "-w" } */

#include <assert.h>
#include <openacc.h>

#define ACTUAL_GANGS 8

/* Test worker-single, vector-partitioned, gang-redundant mode.  */

int
main (int argc, char *argv[])
{
  int n, arr[32], i;
  int ondev;

  for (i = 0; i < 32; i++)
    arr[i] = 0;

  n = 0;

  #pragma acc parallel copy(n, arr) num_gangs(ACTUAL_GANGS) num_workers(8) \
	  vector_length(32) copyout(ondev)
  {
    int j;

    ondev = acc_on_device (acc_device_not_host);

    #pragma acc atomic
    n++;

    #pragma acc loop vector
    for (j = 0; j < 32; j++)
      {
	#pragma acc atomic
	arr[j] += 1;
      }

    #pragma acc atomic
    n++;
  }

  int m = ondev ? ACTUAL_GANGS : 1;
  
  assert (n == m * 2);

  for (i = 0; i < 32; i++)
    assert (arr[i] == m);

  return 0;
}
