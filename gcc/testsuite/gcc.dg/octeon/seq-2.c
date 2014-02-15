/* The result of a 32-bit seq is already sign-extended to 64 bits. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "seq\t" } } */
/* { dg-final { scan-assembler "sne\t" } } */
/* { dg-final { scan-assembler-not "andi|sll" } } */

int
f (int i, int j)
{
  int k = i == j;
  return k;
}

int
g (int i, int j)
{
  int k = i != j;
  return k;
}
