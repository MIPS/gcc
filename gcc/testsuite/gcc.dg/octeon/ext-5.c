/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "sll|exts\t" } } */

long long f (long long a)
{
  return (int) a;
}
