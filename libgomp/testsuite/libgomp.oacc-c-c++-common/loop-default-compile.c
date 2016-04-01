/* { dg-additional-options "-fopenacc-dim=16:16" } */
/* This code uses nvptx inline assembly guarded with acc_on_device, which is
   not optimized away at -O0, and then confuses the target assembler.
   { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */

#include "loop-default.h"
#include <stdlib.h>

int main ()
{
  /* Environment should be ignored.  */
  setenv ("GOMP_OPENACC_DIM", "8:8",  1);
  
  return test_1 (16, 16, 32);
}
