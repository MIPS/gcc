/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts\t" } } */

struct s
{
  long long a:4;
  long long b:10;
};

int f (struct s *s, int a)
{
  return (int) s->b + a;
}
