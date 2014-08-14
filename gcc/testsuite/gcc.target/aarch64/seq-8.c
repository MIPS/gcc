/* { dg-do compile }
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "cset\t" 1 } } */

int
f (long long a, long long b)
{
  return (a == b) + 1;
}
