/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "\\tdext" 2 } } */
/* { dg-final { scan-assembler-times "\\tdins" 2 } } */

unsigned long long f (unsigned long long a)
{
  /* li */
  return a & 0x3000ull;
}

unsigned long long g (unsigned long long a)
{
  /* dext/dsll */
  return a & 0x700000ull;
}

unsigned long long ff (unsigned long long a)
{
  /* dins */
  return a & 0xffffffffff000000ull;
}

unsigned long long gg (unsigned long long a)
{
  /* dins */
  return a & 0xfffffffffffff000ull;
}

unsigned long long hh (unsigned long long a)
{
  return a & 0x10300000000ull;
}

unsigned fff (unsigned a)
{
  /* dext/sll */
  return a & 0xff00000u;
}

unsigned ggg (unsigned a)
{
  return a & 0xffu;
}
