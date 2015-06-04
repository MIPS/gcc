#include <assert.h>

/* Test worker-single/vector-partitioned mode.  */

int
main (int argc, char *argv[])
{
  int arr[32 * 32], i;

  for (i = 0; i < 32 * 32; i++)
    arr[i] = i;

  #pragma acc parallel copy(arr) num_gangs(8) num_workers(8) vector_length(32)
  {
    int j;
    #pragma acc loop gang
    for (j = 0; j < 32; j++)
      {
	int k;
	#pragma acc loop vector
	for (k = 0; k < 32; k++)
	  {
	    #pragma acc atomic
	    arr[j * 32 + k]++;
	  }
      }
  }

  for (i = 0; i < 32 * 32; i++)
    assert (arr[i] == i + 1);

  return 0;
}
