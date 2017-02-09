/* { dg-options "isa_rev>=7 (-mmicromips)" } */
/* { dg-final { scan-assembler "(beqzc|bnezc)" } } */

#include "branch-helper.h"

MICROMIPS void
foo (volatile int *x)
{
  if (*x == 0)
    OCCUPY_0xffffc;
}
