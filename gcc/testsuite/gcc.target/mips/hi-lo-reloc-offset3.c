/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler-not "%lo\\(a_d\\+4\\)" } } */
/* { dg-final { scan-assembler-not "%lo\\(a_l\\+4\\)" } } */
/* { dg-final { scan-assembler "%lo\\(l\\+4\\)" } } */
/* { dg-final { scan-assembler "\tsdc1\t\\\$f\[0-9\],%lo\\(d\\)" } } */

long long l;
double d;
long long __attribute__((aligned(1))) a_l;
double __attribute__((aligned(1))) a_d;

void
foo (void)
{
  d = (double) l;
  a_d = (double) a_l;
}
