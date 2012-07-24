/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bbit\[01\]" { xfail *-octeon-* } } } */

f (int i)
{
  if ((i ^ (1 << 4)) == 0)
    g ();
}
