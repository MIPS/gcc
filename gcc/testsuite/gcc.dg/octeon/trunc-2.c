/* Same as trunc-1.c but optimized with -O1 to remove
   -fexpensive-optimizations.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O1" } */
/* { dg-final { scan-assembler-times "sll" 1 } } */

int
f (long long a, int b)
{
  int c = a;
  return c + b;
}
