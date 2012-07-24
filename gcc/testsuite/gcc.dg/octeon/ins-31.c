/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "ins\t" 2 } } */
/* { dg-final { scan-assembler-not "or\t" } } */
/* { dg-final { scan-assembler-not "cins\t" } } */

#define shift 0
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
