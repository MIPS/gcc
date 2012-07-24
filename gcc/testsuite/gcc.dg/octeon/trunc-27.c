/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* We should not have a truncation here as the memory load should be truncated. */
/* { dg-final { scan-assembler-not "sll\t" } } */

int f(long long *a)
{
  long long b = *a;
  b++;
  return b;
}

