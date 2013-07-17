/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

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

  rd (foo->buf, 0);
  rd (foo->buf, 19);

  return 0;
}
