/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -fno-reorder-blocks" } */
/* { dg-final { scan-assembler-times "bbit\[01\]" 2 } } */

f (int a)
{
  if ((a & 64))
    z ();
  if ((a & 128) == 0)
    g ();
}
