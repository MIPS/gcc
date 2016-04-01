/* { dg-additional-options "-fopenacc-dim=-:-" } */
/* This code uses nvptx inline assembly guarded with acc_on_device, which is
   not optimized away at -O0, and then confuses the target assembler.
   { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */

#include "loop-default.h"
#include <stdlib.h>

int main ()
{
  setenv ("GOMP_OPENACC_DIM", "8:16", 1);

  return test_1 (8, 16, 32);
}
