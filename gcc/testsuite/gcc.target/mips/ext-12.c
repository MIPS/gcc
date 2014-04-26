/* { dg-options "(-mips16) -mgp64 isa_rev>=2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "ext\\t" 5 } } */
/* { dg-final { scan-assembler-times "dsll" 2 } } */
/* { dg-final { scan-assembler-times "\\tsll" 1 } } */
/* { dg-final { scan-assembler-not "and" } } */

unsigned long long f (unsigned long long a)
{
  return a & 0x300000000ull;
}

unsigned long long g (unsigned long long a)
{
  return a & 0x100000000ull;
}

unsigned long long h (unsigned long long a)
{
  return a & 0xfffffffffull;
}

unsigned long long ff (unsigned long long a)
{
  return (a & 0xfff0000000000ull) >> 40;
}

unsigned hh (unsigned a)
{
  return a & 0xffff000u;
}
