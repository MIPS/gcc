/* { dg-options "-march=32r6" } */
/* { dg-final { scan-assembler "(bltic|bgeic)" } } */
/* { dg-final { scan-assembler "bc" } } */

#include "branch-helper.h"

void
foo (volatile int *x)
{
  if (*x >= 10)
    OCCUPY_0x3ffc;
}
