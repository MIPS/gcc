/* No need to 32-bit truncate the result of a seqi. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "seqi\t" } } */
/* { dg-final { scan-assembler "snei\t" } } */
/* { dg-final { scan-assembler-not "sll" } } */

int
f (long long a, int b)
{
  int c = a == 20;
  return c + b;
}

int
g (long long a, int b)
{
  int c = a != 20;
  return c + b;
}
