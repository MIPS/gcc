/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "cins\t" 3 } } */

unsigned long long
f (unsigned long long i)
{
  return (i & 0xff) << 34;
}

unsigned long long
g (unsigned long long i)
{
  return (i << 4) & 0xff0;
}

unsigned long long
h (unsigned long long i)
{
  return (i << 4) & 0xfff;
}

