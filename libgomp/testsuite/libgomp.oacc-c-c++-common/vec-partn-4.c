#include <assert.h>

/* Test conditions inside vector-partitioned loops.  */

int
main (int argc, char *argv[])
{
  int n[32], arr[1024], i;

  for (i = 0; i < 1024; i++)
    arr[i] = i;

  for (i = 0; i < 32; i++)
    n[i] = 0;

  #pragma acc parallel copy(n, arr) num_gangs(32) num_workers(1) \
		       vector_length(32)
  {
    int j, k;

    #pragma acc loop gang(static:*)
    for (j = 0; j < 32; j++)
      n[j]++;

    #pragma acc loop gang
    for (j = 0; j < 32; j++)
      {
	#pragma acc loop vector
	for (k = 0; k < 32; k++)
	  if ((arr[j * 32 + k] % 2) != 0)
	    arr[j * 32 + k] *= 2;
      }

    #pragma acc loop gang(static:*)
    for (j = 0; j < 32; j++)
      n[j]++;
  }

  for (i = 0; i < 32; i++)
    assert (n[i] == 2);

  for (i = 0; i < 1024; i++)
    assert (arr[i] == ((i % 2) == 0 ? i : i * 2));

  return 0;
}
