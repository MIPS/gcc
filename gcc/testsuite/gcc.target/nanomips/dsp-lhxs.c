/* Test MIPS32 DSP LHX instruction */
/* { dg-do compile } */
/* { dg-options "-mgp32 -mdspr3" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

/* { dg-final { scan-assembler "\tlhxs\t" } } */

signed short test (signed short *a, int index)
{
  return a[index];
}
