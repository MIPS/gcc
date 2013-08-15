/* { dg-do run } */
/* { dg-options "-fcheck-pointers -mmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  int *p = __bnd_set_ptr_bounds (buf + 10, sizeof (int) * 10);
  p[0] = argc;
  p[9] = argc;
  return 0;
}
