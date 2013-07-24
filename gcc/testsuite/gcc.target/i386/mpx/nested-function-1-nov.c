/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

int mpx_test (int argc, const char **argv)
{
  int a[100];

  void rd (int i)
  {
    printf ("%d\n", a[i]);
  }

  rd (0);
  rd (99);

  return 0;
}
