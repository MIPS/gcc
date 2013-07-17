/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

struct S {
  int a;
  int b[100];
  int c;
} S;

int foo (int *i, int k)
{
  printf ("%d\n", i[k]);
  return i[k];
}

int mpx_test (int argc, const char **argv)
{
  struct S s;

  foo(&s.a, 0);
  foo(&s.a, 101);

  return 0;
}
