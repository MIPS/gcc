/* { dg-options "-m32 -march=32r6 -fpeephole2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (int *b, int *c)
{
  *b = 0x7fffffff;
  *c = 0xffffffff;
}

/* { dg-final { scan-assembler "\tli\t\\\$a3,\[0-9\]+" } } */
/* { dg-final { scan-assembler "\tli\t\\\$a3,-1" } } */
