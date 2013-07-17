/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

int mpx_test (int argc, const char **argv)
{
  void **fa = (void **)__builtin_frame_address (0);
  printf ("%p\n", *fa);
  return 0;
}
