#include <assert.h>

/* Test vector-single, gang-partitioned mode.  */

int
main (int argc, char *argv[])
{
  int arr[1024];
  int gangs;

  for (gangs = 1; gangs <= 1024; gangs <<= 1)
    {
      int i;

      for (i = 0; i < 1024; i++)
	arr[i] = 0;

      #pragma acc parallel copy(arr) num_gangs(gangs) num_workers(1) \
			   vector_length(32)
      {
	int j;
	#pragma acc loop gang
	for (j = 0; j < 1024; j++)
	  arr[j]++;
      }

      for (i = 0; i < 1024; i++)
	assert (arr[i] == 1);
    }

  return 0;
}
