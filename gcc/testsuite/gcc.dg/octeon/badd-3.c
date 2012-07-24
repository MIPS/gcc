/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */
/* { dg-final { scan-assembler-not "andi|sll" } } */

unsigned long long
baddu4 (unsigned long long a, unsigned long long b)
{
  unsigned long long c = (unsigned char) (a + b);
  return c + 1;
}
