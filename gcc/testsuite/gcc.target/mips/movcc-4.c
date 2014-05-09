/* { dg-options " isa>=4" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "movz\t|movn\t" 1 } } */
/* { dg-final { scan-assembler-not "bbit0\t|bbit1\t" } } */
/* { dg-final { scan-assembler-not "xori\t|xor\t" } } */

NOMIPS16 int f(int a, int b, int c)
{
  int d = a&0x1;
  if (d==1)
    return b;
  return c;
}

