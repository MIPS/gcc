/* Don't generate bbit instead of b{gt,lt,ge,le}z.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "bbit\[01\]\t" } } */

f (long long i)
{
  if (i < 0)
    g ();
}
