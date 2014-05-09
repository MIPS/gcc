/* { dg-options "isa_rev>=2 -mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "ins\t" 2 } } */
/* { dg-final { scan-assembler-not "or\t" } } */
/* { dg-final { scan-assembler-not "cins\t" } } */

#define shift 4
#define mask (0xfffffull<<shift)

int f(int a, int b)
{
  a = (a&~mask) | ((b<<shift)&mask);
  return a;
}

long long fll(long long a, long long b)
{
  a = (a&~mask) | ((b<<shift)&mask);
  return a;
}
