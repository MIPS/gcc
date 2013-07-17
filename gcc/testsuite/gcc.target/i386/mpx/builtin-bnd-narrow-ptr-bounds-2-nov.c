/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  int *p = __bnd_narrow_ptr_bounds (buf - 10, buf, sizeof (int) * 20);
  p[10] = argc;
  p[19] = argc;
  return 0;
}
