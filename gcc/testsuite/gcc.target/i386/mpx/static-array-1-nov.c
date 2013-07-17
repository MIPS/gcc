/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  printf("%d\n", buf[0]);
  printf("%d\n", buf[99]);
  return 0;
}
