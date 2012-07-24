/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dmul\\t" } } */
/* { dg-final { scan-assembler-not "dmult\\t" } } */
/* { dg-final { scan-assembler-not "multu\\t" } } */
/* { dg-final { scan-assembler-not "mflo\\t" } } */

long long f(unsigned *a, unsigned *b, int *c)
{
  long long d = (((unsigned long long)*a)* *b);
  return d;
}
