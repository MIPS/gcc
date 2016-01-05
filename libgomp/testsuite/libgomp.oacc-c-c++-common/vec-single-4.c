/* { dg-additional-options "-w" } */

#include <assert.h>

/* Test switch in vector-single mode.  */

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
	  switch (j % 5)
	    {
	    case 0: arr[j] += 1; break;
	    case 1: arr[j] += 2; break;
	    case 2: arr[j] += 3; break;
	    case 3: arr[j] += 4; break;
	    case 4: arr[j] += 5; break;
	    default: arr[j] += 99;
	    }
      }

      for (i = 0; i < 1024; i++)
	assert (arr[i] == (i % 5) + 1);
    }

  return 0;
}
