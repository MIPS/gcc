#include <assert.h>

/* Test switch in worker-partitioned mode.  */

int
main (int argc, char *argv[])
{
  int arr[32 * 32 * 8], i;

  for (i = 0; i < 32 * 32 * 8; i++)
    arr[i] = i;

  #pragma acc parallel copy(arr) num_gangs(8) num_workers(8) vector_length(32)
  {
    int j;
    #pragma acc loop gang
    for (j = 0; j < 32; j++)
      {
	int k;
	#pragma acc loop worker
	for (k = 0; k < 8; k++)
	  {
	    int m;
	    switch ((j * 32 + k) % 3)
	    {
	    case 0:
	      #pragma acc loop vector
	      for (m = 0; m < 32; m++)
		arr[j * 32 * 8 + k * 32 + m]++;
	      break;

	    case 1:
	      #pragma acc loop vector
	      for (m = 0; m < 32; m++)
		arr[j * 32 * 8 + k * 32 + m] += 2;
	      break;

	    case 2:
	      #pragma acc loop vector
	      for (m = 0; m < 32; m++)
		arr[j * 32 * 8 + k * 32 + m] += 3;
	      break;

	    default: ;
	    }
	  }
      }
  }

  for (i = 0; i < 32 * 32 * 8; i++)
    assert (arr[i] == i + ((i / 32) % 3) + 1);

  return 0;
}
