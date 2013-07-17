/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-additional-options "-fmpx-narrow-to-innermost-array" } */

#include "mpx-check.h"

struct S {
  int arr[100];
} S;

struct S sa[10];

int rd (int *p, int i)
{
  int res = p[i];
  printf ("%d\n", res);
  return res;
}

int mpx_test (int argc, const char **argv)
{
  rd (&sa[argc].arr[0], 0);
  rd (&sa[argc].arr[0], 99);

  return 0;
}
