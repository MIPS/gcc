/* { dg-additional-options "-fopenacc-dim=-:-" } */
/* This code uses nvptx inline assembly guarded with acc_on_device, which is
   not optimized away at -O0, and then confuses the target assembler.
   { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */
/* { dg-set-target-env-var "GOMP_OPENACC_DIM" "8:16" } */

#include "loop-default.h"

int main ()
{
  return test_1 (8, 16, 32);
}
