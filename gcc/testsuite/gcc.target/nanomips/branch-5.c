/* { dg-options "-march=32r6" } */
/* { dg-final { scan-assembler "(beqzc|bnezc)" } } */

#include "branch-helper.h"

void
foo (volatile int *x)
{
  if (*x == 0)
    OCCUPY_0xffffc;
}
