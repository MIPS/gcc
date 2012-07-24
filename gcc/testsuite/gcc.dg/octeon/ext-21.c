/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "sll\t" } } */

struct s
{
  long long a:4;
  char b:6;
};

char f (struct s *s)
{
  return s->b + 4;
}
