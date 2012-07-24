/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "lw\t" } } */
/* { dg-final { scan-assembler-not "sll" } } */

struct s
{
  long long a;
  int b;
};

int
foo (struct s *x)
{
  return x->a;
}

