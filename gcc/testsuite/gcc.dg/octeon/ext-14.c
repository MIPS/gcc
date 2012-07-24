/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "dext\t" } } */

struct s
{
  unsigned a:4;
  unsigned long long b:34;
};

unsigned long long f (struct s s)
{
  register struct s a asm ("$1") = s;
  return a.b;
}
