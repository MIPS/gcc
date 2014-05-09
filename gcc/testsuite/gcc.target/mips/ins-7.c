/* { dg-options "isa_rev>=2 -mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "ins\t" 3 } } */
/* { dg-final { scan-assembler-not "or\t" } } */
/* { dg-final { scan-assembler-not "sll\t" } } */
/* { dg-final { scan-assembler-not "ext\t" } } */
/* { dg-final { scan-assembler-not "cins\t" } } */

#define shift64 32
#define mask64 (0xffffffffull<<shift64)

#define shift32 24
#define mask32 (0xfffu<<shift32)

int f(int a, int b)
{
  a = (a&~mask32) | ((b<<shift32)&mask32);
  return a;
}

long long fll(long long a, long long b)
{
  a = (a&~mask64) | ((b<<shift64)&mask64);
  return a;
}

long long f1(long long a, long long b)
{
  a = (a&~(long long)mask32) | ((b<<shift32)&(long long)mask32);
  return a;
}
