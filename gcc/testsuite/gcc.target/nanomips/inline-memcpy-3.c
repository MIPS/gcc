/* { dg-options "-m32 -march=32r6 (REQUIRES_STDLIB)" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os"} { "" } } */

#include <string.h>

extern int g_i32[4];
extern char g_i8[16];

void
foo (char *a)
{
  memcpy(a, &g_i32, sizeof g_i32);
  memcpy(a, &g_i8, sizeof g_i8);
}

/* { dg-final { scan-assembler-times "g_i32 " 1 } } */
/* { dg-final { scan-assembler-times "g_i8 " 1 } } */
