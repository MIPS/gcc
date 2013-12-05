/* { dg-options "forbid_cpu=octeon.* -mgp64" } */
/* { dg-final { scan-assembler "\tdmult\t" } } */
/* { dg-final { scan-assembler "\tmflo\t" } } */
/* { dg-final { scan-assembler-not "\tdmul\t" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

long long
f (long long a, long long b)
{
  return a * b;
}
