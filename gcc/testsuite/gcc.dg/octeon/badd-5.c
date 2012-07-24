/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */
/* { dg-final { scan-assembler-not "andi|sll" } } */

int
baddu2 (unsigned int a, unsigned int b)
{
  unsigned long long c = (unsigned char) (a + b);
  int d = c;
  return d + 1;
}
