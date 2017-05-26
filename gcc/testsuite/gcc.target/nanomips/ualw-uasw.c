/* { dg-options "-mabi=p32 -mxlp isa_rev>=6 -mnanomips (REQUIRES_STDLIB)" } */
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
