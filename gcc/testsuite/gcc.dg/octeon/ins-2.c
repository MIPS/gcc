/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dins\t" } } */
/* { dg-final { scan-assembler-not "sll\t" } } */

struct s
{
  int a:2;
  long long b:34;
};

struct s
f (struct s s, int i, int j)
{
  s.b = (long long) (i/j);
  return s;
}
