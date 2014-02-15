/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "xori\t" } } */
/* { dg-final { scan-assembler "slti?u\t" } } */

int
f (int i)
{
  return i == 600;
}
