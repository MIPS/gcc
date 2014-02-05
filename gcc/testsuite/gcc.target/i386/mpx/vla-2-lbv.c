/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fcheck-pointer-bounds -mmpx" } */


#define XFAIL

#include "mpx-check.h"

struct Foo {
  int buf[10] __attribute__((bnd_variable_size));
};

int rd (int *p, int i)
{
  int res = p[i];
  printf ("%d\n", res);
  return res;
}

int mpx_test (int argc, const char **argv) {
  struct Foo *foo = (struct Foo *) alloca (20 * sizeof(int));

  rd (foo->buf, -1);

  return 0;
}
