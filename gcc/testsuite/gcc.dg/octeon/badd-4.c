/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */
/* { dg-final { scan-assembler-not "andi|sll" } } */

unsigned long long
baddu1 (unsigned int a, unsigned int b)
{
  unsigned long long c = (unsigned char) (a + b);
  return c + 1;
}
