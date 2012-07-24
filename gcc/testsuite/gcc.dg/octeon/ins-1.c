/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dins\t" } } */

struct s
{
  long long a:10;
  unsigned long long b:38;
  long long c:10;
};

struct s f (struct s s, unsigned long long a)
{
  s.b = a;
  return s;
}
