/* Testing bneic/beqic branch range - max offset 0x7fc. */
/* { dg-options "-mshared -mabi=p32" } */

#include "branch-helper.h"

void
foo1 (volatile int *x)
{
  if (*x != 1)
    OCCUPY_0x7fc;
}
/* { dg-final { scan-assembler "\tbeqic.*" } } */

void
foo2 (volatile int *x)
{
  if (*x != 1) {
    OCCUPY_0x7fc;
	asm ("nop");
  }
}
/* { dg-final { scan-assembler "\tbneic.*" } } */
/* { dg-final { scan-assembler "\tbc\t\\\$L.*\n" } } */
