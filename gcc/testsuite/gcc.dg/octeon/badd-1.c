/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "baddu\t" } } */
/* { dg-final { scan-assembler-not "andi|sll"  } } */

unsigned char
baddu3 (unsigned char a, unsigned char b)
{
  return a + b;
}

