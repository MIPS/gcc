/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "cins\t" } } */

unsigned long long
f (unsigned long long i)
{
  return (i & 0x1ffffffffULL) << 4;
}
