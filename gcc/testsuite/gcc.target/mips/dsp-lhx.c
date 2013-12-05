/* Test MIPS32 DSP LHX instruction */
/* { dg-do compile } */
/* { dg-options "-mgp32 -mdsp" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

/* { dg-final { scan-assembler "\tlhx\t" } } */

NOMIPS16 signed short test (signed short *a, int index)
{
  return a[index];
}
