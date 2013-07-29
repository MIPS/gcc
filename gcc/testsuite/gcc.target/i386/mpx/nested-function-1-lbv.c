/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx -mmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#define XFAIL

#include "mpx-check.h"

int mpx_test (int argc, const char **argv)
{
  int a[100];

  void rd (int i)
  {
    printf ("%d\n", a[i]);
  }

  rd (-1);

  return 0;
}
