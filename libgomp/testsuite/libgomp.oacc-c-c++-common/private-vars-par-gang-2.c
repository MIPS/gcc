#include <assert.h>

/* Test of gang-private variables declared on the parallel directive.  */

#if defined(ACC_DEVICE_TYPE_host) || defined(ACC_DEVICE_TYPE_host_nonshm)
#define ACTUAL_GANGS 1
#else
#define ACTUAL_GANGS 32
#endif

int
main (int argc, char* argv[])
{
  int x = 5, i, arr[ACTUAL_GANGS];

  for (i = 0; i < ACTUAL_GANGS; i++)
    arr[i] = 3;

  #pragma acc parallel private(x) copy(arr) num_gangs(ACTUAL_GANGS) \
		       num_workers(8) vector_length(32)
  {
    #pragma acc loop gang(static:1)
    for (i = 0; i < ACTUAL_GANGS; i++)
      x = i * 2;

    #pragma acc loop gang(static:1)
    for (i = 0; i < ACTUAL_GANGS; i++)
      arr[i] += x;
  }

  for (i = 0; i < ACTUAL_GANGS; i++)
    assert (arr[i] == 3 + i * 2);

  return 0;
}
