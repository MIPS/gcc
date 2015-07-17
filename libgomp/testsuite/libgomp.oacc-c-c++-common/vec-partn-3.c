#include <assert.h>

/* Test conditional vector-partitioned loops.  */

int
main (int argc, char *argv[])
{
  int n[32], arr[1024], i;

  for (i = 0; i < 1024; i++)
    arr[i] = 0;

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
	if ((j % 2) == 0)
	  {
	    #pragma acc loop vector
	    for (k = 0; k < 32; k++)
	      arr[j * 32 + k]++;
	  }
	else
	  {
	    #pragma acc loop vector
	    for (k = 0; k < 32; k++)
	      arr[j * 32 + k]--;
	  }
      }

    #pragma acc loop gang(static:*)
    for (j = 0; j < 32; j++)
      n[j]++;
  }

  for (i = 0; i < 32; i++)
    assert (n[i] == 2);

  for (i = 0; i < 1024; i++)
    assert (arr[i] == ((i % 64) < 32) ? 1 : -1);

  return 0;
}
