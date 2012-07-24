/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dext\t" } } */

unsigned long long f (unsigned long long a)
{
  return a & 0xffffffffULL;
}
