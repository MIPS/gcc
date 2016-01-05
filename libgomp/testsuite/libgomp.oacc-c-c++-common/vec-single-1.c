/* { dg-additional-options "-w" } */

#include <assert.h>

/* Test trivial operation of vector-single mode.  */

int
main (int argc, char *argv[])
{
  int n = 0;
  #pragma acc parallel copy(n) num_gangs(1) num_workers(1) vector_length(32)
  {
    n++;
  }
  assert (n == 1);
  return 0;
}
