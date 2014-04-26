/* { dg-options "(-mips16) -mgp64 isa_rev>=2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
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
