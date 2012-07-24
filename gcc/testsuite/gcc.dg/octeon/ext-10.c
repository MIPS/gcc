/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts" } } */

int f (long long a, int b)
{
  int c = (char) a * b;
  return c;
}
