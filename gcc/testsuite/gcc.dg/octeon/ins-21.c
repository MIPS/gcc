/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "move" 2 } } */
/* { dg-final { scan-assembler-times "dins" 3 } } */
/* { dg-final { scan-assembler-times "\\tins" 1 } } */
/* { dg-final { scan-assembler-not "and" } } */

unsigned long long f (unsigned long long a)
{
  return a & 0xfffffffcffffffffull;
}

unsigned long long g (unsigned long long a)
{
  return a & 0xfffffffffffeffffull;
}

unsigned long long h (unsigned long long a)
{
  return sh (a & 0xfffffffcffffffffull);
}

unsigned ff (unsigned a)
{
  return (a + 1) & 0xfff00fffu;
}
