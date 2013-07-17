/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

int buf[100];
int *p = buf;

int foo (int i)
{
  static int **pp = &p;

  return (*pp)[i];
}

int mpx_test (int argc, const char *argv[])
{
  printf ("%d\n", foo (0));
  printf ("%d\n", foo (99));

  return 0;
}
