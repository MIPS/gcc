/* No need to 32-bit truncate the result of a seq. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "seq\t" } } */
/* { dg-final { scan-assembler "sne\t" } } */
/* { dg-final { scan-assembler-not "sll" } } */

int
f (long long a, long long b, int c)
{
  int d = a == b;
  return d + c;
}

int
g (long long a, long long b, int c)
{
  int d = a != b;
  return d + c;
}
