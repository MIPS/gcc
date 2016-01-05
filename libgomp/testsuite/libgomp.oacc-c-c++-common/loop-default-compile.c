
/* { dg-additional-options "-O2 -fopenacc-dim=16:16" } */

#include "loop-default.h"
#include <stdlib.h>

int main ()
{
  /* Environment should be ignored.  */
  setenv ("GOMP_OPENACC_DIM", "8:8",  1);
  
  return test_1 (16, 16, 32);
}
