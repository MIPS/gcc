/* { dg-options "isa_rev>=2 -mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "\\tli\[^\\n\]+,-1" 4 } } */
/* { dg-final { scan-assembler-times "dins" 4 } } */
/* { dg-final { scan-assembler-times "\\tins" 1 } } */
/* { dg-final { scan-assembler-times "move" 3 } } */
/* { dg-final { scan-assembler-not "and" } } */

unsigned long long f (unsigned long long a)
{
  return a | 0x100000000ull;
}

unsigned long long g (unsigned long long a)
{
  return a | 0x8000000000000000ull;
}

unsigned long long h (unsigned long long a, unsigned long long b)
{
  return gs (a | 0x100000000ull, b | 0x600000000ull);
}

unsigned ff (unsigned a)
{
  return a | 0x000ff000u;
}
