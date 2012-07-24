/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */

unsigned char
baddu3 (unsigned char a, unsigned char b)
{
  unsigned char c = a + b;
  return c + 1;
}

