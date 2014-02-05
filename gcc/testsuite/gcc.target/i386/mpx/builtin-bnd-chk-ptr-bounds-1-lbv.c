/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fcheck-pointer-bounds -mmpx" } */


#define XFAIL

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  __bnd_chk_ptr_bounds (buf - 1, sizeof (int));
  return 0;
}
