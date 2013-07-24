/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#define XFAIL

#include "mpx-check.h"

int buf[100];
int *p;

int mpx_test (int argc, const char **argv)
{
  int *p1 = __bnd_set_ptr_bounds (buf + 10, sizeof (int) * 10);
  p = buf;
  __bnd_store_ptr_bounds ((void **)&p, p1 - 10);
  p[9] = argc;
  return 0;
}
