#include <assert.h>

/* Test worker-partitioned/vector-single mode.  */

int
main (int argc, char *argv[])
{
  int arr[32 * 8], i;

  for (i = 0; i < 32 * 8; i++)
    arr[i] = 0;

  #pragma acc parallel copy(arr) num_gangs(8) num_workers(8) vector_length(32)
  {
    int j;
    #pragma acc loop gang
    for (j = 0; j < 32; j++)
      {
	int k;
	#pragma acc loop worker
	for (k = 0; k < 8; k++)
          arr[j * 8 + k] += j * 8 + k;
      }
  }

  for (i = 0; i < 32 * 8; i++)
    assert (arr[i] == i);

  return 0;
}
