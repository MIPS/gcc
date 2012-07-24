/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */

struct s
{
  int a:2;
  int b:22;
};

void
f (struct s *s, int i)
{
  s->b = i;
}
