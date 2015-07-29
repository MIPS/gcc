#include <assert.h>
#include <openacc.h>

/* Test of reduction on parallel directive (with async).  */

#define ACTUAL_GANGS 256

int
main (int argc, char *argv[])
{
  int res = 0, res2 = 0;

  #pragma acc parallel num_gangs(ACTUAL_GANGS) num_workers(32) \
		       vector_length(32) reduction(+:res) copy(res2) async(1)
  {
    res += 5;

    #pragma acc atomic
    res2 += 5;
  }

  acc_wait (1);

  assert (res == res2);

  res = res2 = 1;

  #pragma acc parallel num_gangs(8) num_workers(32) vector_length(32) \
		       reduction(*:res) copy(res2) async(1)
  {
    res *= 5;

    #pragma acc atomic
    res2 *= 5;
  }

  acc_wait (1);

  assert (res == res2);

  return 0;
}
