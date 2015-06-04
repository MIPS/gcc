#include <assert.h>

/* Test condition in worker-single/vector-single mode.  */

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
      if ((arr[j] % 2) != 0)
	arr[j]++;
      else
	arr[j] += 2;
  }

  for (i = 0; i < 32; i++)
    assert (arr[i] == ((i % 2) != 0) ? i + 1 : i + 2);

  return 0;
}
