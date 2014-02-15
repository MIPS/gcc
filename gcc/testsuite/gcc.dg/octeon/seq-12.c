/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "li\t|addiu\t.*,600" } } */
/* { dg-final { scan-assembler "seq\t|slti?u\t.*,1" } } */

int
f (int i)
{
  return i == -600;
}
