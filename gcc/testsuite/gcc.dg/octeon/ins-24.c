/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "\\tins|\\tdins" } } */

unsigned long long f (unsigned long long a)
{
  /* li */
  return a | 0x1000ull;
}

unsigned long long g (unsigned long long a)
{
  /* lui */
  return a | 0x1000000ull;
}

unsigned long long h (unsigned long long a)
{
  /* li - */
  return a | 0xfffffffffffff000ull;
}

unsigned long long ff (unsigned long long a)
{
  /* lui - */
  return a | 0xffffffffff000000ull;
}

unsigned long long gg (unsigned long long a)
{
  return a | 0x100001000ull;
}
