/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ext\t" } } */

struct s
{
  long long a:10;
  unsigned b:22;
  long long c:10;
};

unsigned f (struct s *s, unsigned a)
{
  return s->b + a;
}
