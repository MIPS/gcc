/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* We should get a sll, instead of an exts. */
/* { dg-final { scan-assembler-times "sll\t" 1 } } */
/* { dg-final { scan-assembler-not "exts\t" } } */


long long f(long long b)
{
  int c = b;
  return c;
}
