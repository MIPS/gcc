/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */

long long
baddu4 (int a, int b)
{
  return (a + b) & 0xff;
}

