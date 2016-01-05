
/* { dg-additional-options "-O2 -fopenacc-dim=-:-" } */

#include "loop-default.h"
#include <stdlib.h>

int main ()
{
  setenv ("GOMP_OPENACC_DIM", "8:16", 1);

  return test_1 (8, 16, 32);
}
