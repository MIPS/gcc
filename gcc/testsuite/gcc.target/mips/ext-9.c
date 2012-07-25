/* { dg-options "-O2 -mgp64" } */
/* { dg-final { scan-assembler-not "and" } } */

struct s
{
  long long a:10;
  unsigned long long b:8;
  long long c:10;
};

unsigned int f (struct s s, unsigned int a)
{
  return s.b - a;
}
