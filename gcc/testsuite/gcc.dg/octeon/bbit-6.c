/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bbit\[01\]\t" } } */
/* { dg-final { scan-assembler-not "andi\t" } } */

f (long long i)
{
  if (i & 0x80)
    foo ();
}
