/* { dg-options "-march=32r6" } */
/* { dg-final { scan-assembler "\t(bltic|bgeic)" } } */
/* { dg-final { scan-assembler-not "\tbc" } } */

#include "branch-helper.h"

void
foo (volatile int *x)
{
  if (*x >= 10)
    OCCUPY_0x7fc;
}
