/* { dg-xfail-run-if "TODO" { openacc_nvidia_accel_selected } { "*" } { "" } } */

#include <assert.h>

/* Test of gang-private variables declared on loop directive, with broadcasting
   to partitioned workers.  */

int
main (int argc, char* argv[])
{
  int x = 5, i, arr[32 * 32];

  for (i = 0; i < 32 * 32; i++)
    arr[i] = i;

  #pragma acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  {
    #pragma acc loop gang private(x)
    for (i = 0; i < 32; i++)
      {
	x = i * 2;

	#pragma acc loop worker
	for (int j = 0; j < 32; j++)
	  arr[i * 32 + j] += x;
      }
  }

  for (i = 0; i < 32 * 32; i++)
    assert (arr[i] == i + (i / 32) * 2);

  return 0;
}
