#include <assert.h>

/* Basic test of firstprivate variable.  */

int
main (int argc, char* argv[])
{
  int x = 5, i, arr[32];

  for (i = 0; i < 32; i++)
    arr[i] = 3;

  #pragma acc parallel firstprivate(x) copy(arr) num_gangs(32) num_workers(8) \
		       vector_length(32)
  {
    #pragma acc loop gang
    for (i = 0; i < 32; i++)
      arr[i] += x;
  }

  for (i = 0; i < 32; i++)
    assert (arr[i] == 8);

  return 0;
}
