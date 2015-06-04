#include <assert.h>

/* Test worker-single/worker-partitioned transitions.  */

int
main (int argc, char *argv[])
{
  int n[32], arr[32 * 32], i;

  for (i = 0; i < 32 * 32; i++)
    arr[i] = 0;

  for (i = 0; i < 32; i++)
    n[i] = 0;

  #pragma acc parallel copy(n, arr) num_gangs(8) num_workers(16) \
	  vector_length(32)
  {
    int j;
    #pragma acc loop gang
    for (j = 0; j < 32; j++)
      {
	int k;

	n[j]++;

	#pragma acc loop worker
	for (k = 0; k < 32; k++)
          arr[j * 32 + k]++;

	n[j]++;

	#pragma acc loop worker
	for (k = 0; k < 32; k++)
          arr[j * 32 + k]++;

	n[j]++;

	#pragma acc loop worker
	for (k = 0; k < 32; k++)
          arr[j * 32 + k]++;

	n[j]++;
      }
  }

  for (i = 0; i < 32; i++)
    assert (n[i] == 4);

  for (i = 0; i < 32 * 32; i++)
    assert (arr[i] == 3);

  return 0;
}
