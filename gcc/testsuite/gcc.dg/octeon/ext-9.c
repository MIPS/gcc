/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts\t" } } */
/* { dg-final { scan-assembler-not "sll" } } */
/* { dg-final { scan-assembler-not "seb" } } */
/* { dg-final { scan-assembler-not "dsra" } } */

long long f (long long a)
{
  return (char) a;
}
