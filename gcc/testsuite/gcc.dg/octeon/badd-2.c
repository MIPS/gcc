/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */
/* { dg-final { scan-assembler-not "andi|sll" } } */

unsigned char
baddu4 (long long a, long long b)
{
  return a + b;
}

