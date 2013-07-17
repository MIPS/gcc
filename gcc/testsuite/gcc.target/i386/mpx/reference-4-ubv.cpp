/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-lstdc++" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#define XFAIL

#include "mpx-check.h"

int buf[100];

int rd (int (&p)[100], int i)
{
  int res = p[i];
  printf ("%d\n", res);
  return res;
}

int (&get_buf ()) [100]
{
  return buf;
}

int mpx_test (int argc, const char **argv)
{
  int *p;

  rd (get_buf (), 100);

  return 0;
}
