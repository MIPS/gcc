/* { dg-options "-mno-gpopt" } */
/* { dg-final { scan-assembler "%lo\\(l\\+4\\)" } } */
/* { dg-final { scan-assembler "%lo\\(d\\+4\\)" } } */
/* { dg-final { scan-assembler-not "%lo\\(a_l\\+4\\)" } } */
/* { dg-final { scan-assembler-not "%lo\\(a_d\\+4\\)" } } */

long long l;
double d;
long long a_l __attribute__((aligned(1)));
double a_d __attribute__((aligned(1)));

void
foo (void)
{
  l = 0;
  d = 0;
  a_l = 0;
  a_d = 0;
}
