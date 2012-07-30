/* { dg-do compile } */
/* { dg-options "-march=octeon -mgp64" } */
/* We should be able to get seq followed by an add instead
   of a conditional move. */
/* { dg-final { scan-assembler "seq\t" } } */

int
f (long long a, long long b)
{
  return (a == b) + 1;
}
