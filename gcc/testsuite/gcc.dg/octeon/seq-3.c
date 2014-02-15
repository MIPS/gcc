/* The result of a 32-bit seqi is already sign-extended to 64 bits. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "seqi\t" } } */
/* { dg-final { scan-assembler "snei\t" } } */
/* { dg-final { scan-assembler-not "andi|sll" } } */

int
f (int i, int j)
{
  int k = i == -512;
  return k;
}

int
g (int i, int j)
{
  int k = i != -512;
  return k;
}
