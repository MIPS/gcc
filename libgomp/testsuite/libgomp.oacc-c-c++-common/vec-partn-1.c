#include <assert.h>

/* Test basic vector-partitioned mode transitions.  */

int
main (int argc, char *argv[])
{
  int n = 0, arr[32], i;

  for (i = 0; i < 32; i++)
    arr[i] = 0;

  #pragma acc parallel copy(n, arr) num_gangs(1) num_workers(1) \
		       vector_length(32)
  {
    int j;
    n++;
    #pragma acc loop vector
    for (j = 0; j < 32; j++)
      arr[j]++;
    n++;
  }

  assert (n == 2);

  for (i = 0; i < 32; i++)
    assert (arr[i] == 1);

  return 0;
}
