/* { dg-options "-m32 -march=32r6 (REQUIRES_STDLIB)" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os"} { "" } } */

#include <string.h>

char c[20];

void
foo (char *a)
{
  memcpy(a, &c, 20);
}

/* { dg-final { scan-assembler-times "\tualw " 5 } } */
/* { dg-final { scan-assembler-times "\tuasw " 5 } } */
