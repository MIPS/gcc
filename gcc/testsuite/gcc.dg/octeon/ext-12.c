/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts\t" } } */

long long f (long long a, short b)
{
  short c = (char) a * b;
  return c;
}
