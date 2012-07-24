/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* There should only be one sll as the truncation and the shift should have
   been combined. */
/* { dg-final { scan-assembler-times "sll\t" 1 } } */

int f(long long a)
{
  return a<<30;
}

/* This should produce 0. */
int g(long long a)
{
  return a<<32;
}

