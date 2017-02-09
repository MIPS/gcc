/* { dg-options "isa_rev>=7 (-mmicromips)" } */
/* { dg-final { scan-assembler "(bltic|bgeic)" } } */
/* { dg-final { scan-assembler "bc" } } */

#include "branch-helper.h"

MICROMIPS void
foo (volatile int *x)
{
  if (*x >= 10)
    OCCUPY_0x3ffc;
}
