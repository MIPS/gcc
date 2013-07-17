/* { dg-do run } */
/* { dg-options "-fno-mpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

char buf[100];

int mpx_test (int argc, const char **argv)
{
  assert ((void *)-1 == __bnd_get_ptr_ubound (buf));
  return 0;
}
