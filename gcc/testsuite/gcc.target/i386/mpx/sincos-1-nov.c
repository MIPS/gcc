/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-lm" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"
#include "math.h"

int mpx_test (int argc, const char **argv)
{
  double d1, d2;
  d1 = sin(argc);
  d2 = cos(argc);

  printf ("%f %f\n", d1, d2);

  return 0;
}
