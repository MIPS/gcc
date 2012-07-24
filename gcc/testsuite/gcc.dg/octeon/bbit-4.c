/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bbit\[01\]\t" } } */
/* { dg-final { scan-assembler-not "sll\t" } } */
/* { dg-final { scan-assembler-not "andi\t" } } */

g (long long i)
{
  if (i % 2)
    foo ();
}
