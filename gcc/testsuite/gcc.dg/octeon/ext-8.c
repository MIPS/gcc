/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler-not "exts\t" } } */

long long f (long long a)
{
  register long long b asm ("$1") = a;
  return (char) b;
}
