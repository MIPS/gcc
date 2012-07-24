/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "movz\t|movn\t" 1 } } */
/* { dg-final { scan-assembler-not "bbit0\t|bbit1\t" } } */
/* { dg-final { scan-assembler-not "xori\t|xor\t" } } */

int f(int a, int b, int c)
{
  int d = a&0x1;
  if (d==1)
    return b;
  return c;
}

