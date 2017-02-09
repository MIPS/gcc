/* Check that we move DFmode values via memory between FP and GP.  */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-m32 -mhard-float" } */

void bar (void);

double
foo (int x, double *a)
{
  return *a;
}
/* { dg-final { scan-assembler-not "mthc1" } } */
/* { dg-final { scan-assembler-not "mtc1" } } */
/* { dg-final { scan-assembler-times "ldc1" 1 } } */
