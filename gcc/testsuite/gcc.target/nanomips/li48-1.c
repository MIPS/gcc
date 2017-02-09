/* { dg-options "-m32 -march=32r6 -fpeephole2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int
foo (void)
{
  int b = 0x12345;
  return b;
}

/* { dg-final { scan-assembler "\tli\t\\\$a0,\[0-9\]+" } } */
