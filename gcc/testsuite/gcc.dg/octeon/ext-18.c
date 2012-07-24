/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts\t" } } */

struct s
{
  long long a:4;
  long long b:1;
};

long long f (struct s *s)
{
  return s->b;
}
