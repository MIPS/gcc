/* { dg-xfail-run-if "TODO" { c && openacc_nvidia_accel_selected } { "*" } { "" } } */

#include <assert.h>

/* Test of worker-private variables declared on a loop directive, broadcasting
   to vector-partitioned mode.  Back-to-back worker loops.  */

int
main (int argc, char* argv[])
{
  int x = 5, i, arr[32 * 32 * 32];

  for (i = 0; i < 32 * 32 * 32; i++)
    arr[i] = i;

  #pragma acc parallel copy(arr) num_gangs(32) num_workers(32) vector_length(32)
  {
    int j;

    #pragma acc loop gang
    for (i = 0; i < 32; i++)
      {
        #pragma acc loop worker private(x)
	for (j = 0; j < 32; j++)
	  {
	    int k;
	    x = i ^ j * 3;

	    #pragma acc loop vector
	    for (k = 0; k < 32; k++)
	      arr[i * 1024 + j * 32 + k] += x * k;
	  }

	#pragma acc loop worker private(x)
	for (j = 0; j < 32; j++)
	  {
	    int k;
	    x = i | j * 5;
	    
	    #pragma acc loop vector
	    for (k = 0; k < 32; k++)
	      arr[i * 1024 + j * 32 + k] += x * k;
	  }
      }
  }

  for (i = 0; i < 32; i++)
    for (int j = 0; j < 32; j++)
      for (int k = 0; k < 32; k++)
        {
	  int idx = i * 1024 + j * 32 + k;
          assert (arr[idx] == idx + (i ^ j * 3) * k + (i | j * 5) * k);
	}

  return 0;
}
