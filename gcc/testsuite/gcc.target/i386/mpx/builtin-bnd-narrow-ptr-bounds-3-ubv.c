/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fcheck-pointer-bounds -mmpx" } */


#define XFAIL

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  int *p = __bnd_narrow_ptr_bounds (buf + 10, buf, sizeof (int) * 100);
  p[90] = argc;
  return 0;
}
