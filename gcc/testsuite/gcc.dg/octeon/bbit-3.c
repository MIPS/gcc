/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bbit\[01\]\t" } } */
/* { dg-final { scan-assembler-not "andi\t" } } */

int z;
f (int i)
{
  if (i & 1)
    foo ();
}
