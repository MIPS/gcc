#include <assert.h>

/* Test switch in vector-single mode, initialise array on device.  */

int
main (int argc, char *argv[])
{
  int arr[1024];
  int i;

  for (i = 0; i < 1024; i++)
    arr[i] = 99;

  #pragma acc parallel copy(arr) num_gangs(1024) num_workers(1) \
		       vector_length(32)
  {
    int j;

    /* This loop and the one following must be distributed to available gangs
       in the same way to ensure data dependencies are not violated (hence the
       "static" clauses).  */
    #pragma acc loop gang(static:*)
    for (j = 0; j < 1024; j++)
      arr[j] = 0;
    
    #pragma acc loop gang(static:*)
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

  return 0;
}
