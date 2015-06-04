#include <assert.h>

/* Test multiple conditional vector-partitioned loops in worker-single
   mode.  */

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
	if ((j % 3) == 0)
	  {
	    #pragma acc loop vector
	    for (k = 0; k < 32; k++)
	      {
		#pragma acc atomic
		arr[j * 32 + k] += 3;
	      }
	  }
	else if ((j % 3) == 1)
	  {
	    #pragma acc loop vector
	    for (k = 0; k < 32; k++)
	      {
		#pragma acc atomic
		arr[j * 32 + k] += 7;
	      }
	  }
      }
  }

  for (i = 0; i < 32 * 32; i++)
    {
      int j = (i / 32) % 3;
      assert (arr[i] == i + ((j == 0) ? 3 : (j == 1) ? 7 : 0));
    }

  return 0;
}
