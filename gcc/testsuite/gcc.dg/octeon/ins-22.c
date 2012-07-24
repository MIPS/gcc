/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "\\tdins" 1 } } */

unsigned long long f (unsigned long long a)
{
  /* dins */
  return a & 0xffffffffffffcfffull;
}

unsigned long long ff (unsigned long long a)
{
  return a & 0x0000000000ffffffull;
}

unsigned long long gg (unsigned long long a)
{
  return a & 0xfffffefeffffffffull;
}
