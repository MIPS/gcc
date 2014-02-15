/* No need to 32-bit truncate the result of a seq when comparing to
   zero. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "slti?u\t" 2 } } */
/* { dg-final { scan-assembler-not "sll" } } */

int
f (long long a, int b)
{
  int c = a == 0;
  return c + b;
}

int
g (long long a, int b)
{
  int c = a != 0;
  return c + b;
}
