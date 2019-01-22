/* { dg-options "-m32 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os"} { "" } } */

short g_i16[8];
char g_i8[16];

void
foo (char *a)
{
  __builtin_memcpy(a, &g_i16, sizeof g_i16);
  __builtin_memcpy(a, &g_i8, sizeof g_i8);
}

/* { dg-final { scan-assembler-not "%lo(g_i8)" } } */
/* { dg-final { scan-assembler-not "%lo(g_i16)" } } */
