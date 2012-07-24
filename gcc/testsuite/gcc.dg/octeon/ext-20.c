/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "sll\t" } } */

struct s
{
  long long a:4;
  short b:14;
};

short f (struct s *s)
{
  return s->b + 4;
}
