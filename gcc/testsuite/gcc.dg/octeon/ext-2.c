/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dext\t" } } */

unsigned long long f (unsigned int a)
{
  return a;
}
