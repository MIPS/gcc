/* { dg-options "isa_rev>=7 (-mmicromips)" } */
/* { dg-final { scan-assembler "\t(bltic|bgeic)" } } */
/* { dg-final { scan-assembler-not "\tbc" } } */

#include "branch-helper.h"

MICROMIPS void
foo (volatile int *x)
{
  if (*x >= 10)
    OCCUPY_0x7fc;
}
