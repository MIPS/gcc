#include <assert.h>

/* Test of gang-private array variable declared on the parallel directive.  */

int
main (int argc, char* argv[])
{
  int x[32], i, arr[32 * 32];

  for (i = 0; i < 32 * 32; i++)
    arr[i] = i;

  #pragma acc parallel private(x) copy(arr) num_gangs(32) num_workers(2) \
		       vector_length(32)
  {
    #pragma acc loop gang
    for (i = 0; i < 32; i++)
      {
        int j;
	for (j = 0; j < 32; j++)
	  x[j] = j * 2;
	
	#pragma acc loop worker
	for (j = 0; j < 32; j++)
	  arr[i * 32 + j] += x[31 - j];
      }
  }

  for (i = 0; i < 32 * 32; i++)
    assert (arr[i] == i + (31 - (i % 32)) * 2);

  return 0;
}
