/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dmul\\t" } } */
/* { dg-final { scan-assembler-not "dmult\\t" } } */
/* { dg-final { scan-assembler-not "mult\\t" } } */
/* { dg-final { scan-assembler-not "mflo\\t" } } */

long long f(int *a, int *b, int *c)
{
  long long d = (((long long)*a)* *b);
  return d;
}
