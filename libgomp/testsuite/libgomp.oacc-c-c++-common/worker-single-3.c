#include <assert.h>

/* Test switch in worker-single/vector-single mode.  */

int
main (int argc, char *argv[])
{
  int arr[32], i;

  for (i = 0; i < 32; i++)
    arr[i] = i;

  #pragma acc parallel copy(arr) num_gangs(8) num_workers(8) vector_length(32)
  {
    int j;
    #pragma acc loop gang
    for (j = 0; j < 32; j++)
      switch (arr[j] % 5)
	{
	case 0: arr[j] += 1; break;
	case 1: arr[j] += 2; break;
	case 2: arr[j] += 3; break;
	case 3: arr[j] += 4; break;
	case 4: arr[j] += 5; break;
	default: arr[j] += 99;
	}
  }

  for (i = 0; i < 32; i++)
    assert (arr[i] == i + (i % 5) + 1);

  return 0;
}
