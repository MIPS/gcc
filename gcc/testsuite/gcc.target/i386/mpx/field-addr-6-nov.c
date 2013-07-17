/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

struct S {
  int a;
  int b[100];
  int c;
} S;

struct S1 {
  int x;
  struct S a[10];
  struct S b;
} S1;

int foo (int *p, int i)
{
  printf ("%d\n", p[i]);
  return p[i];
}

struct S1 s1;
struct S1 *s2 = &s1;

int mpx_test (int argc, const char **argv)
{
  foo (&(s2->a[0].a), 0);
  foo (&(s2->a[9].a), 101);

  return 0;
}
