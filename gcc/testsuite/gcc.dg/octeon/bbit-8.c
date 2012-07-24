/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "bbit\[01\]" { xfail *-octeon-* } } } */

f (long long i)
{
  if ((i ^ (1LL << 34)) == 0)
    g ();
}
