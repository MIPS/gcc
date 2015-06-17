#include <assert.h>

/* Test of gang-private variables declared on loop directive.  */

int
main (int argc, char* argv[])
{
  int x = 5, i, arr[32];

  for (i = 0; i < 32; i++)
    arr[i] = i;

  #pragma acc parallel copy(arr) num_gangs(32) num_workers(8) vector_length(32)
  {
    #pragma acc loop gang private(x)
    for (i = 0; i < 32; i++)
      {
	x = i * 2;
	arr[i] += x;
      }
  }

  for (i = 0; i < 32; i++)
    assert (arr[i] == i * 3);

  return 0;
}
