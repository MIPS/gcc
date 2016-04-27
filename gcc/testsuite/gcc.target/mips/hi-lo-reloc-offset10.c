/* { dg-options "-mcode-readable=no -mno-gpopt -mabi=32 -mfpxx isa=2" } */
/* { dg-final { scan-assembler "%lo\\(d\\+4\\)" } } */
/* { dg-final { scan-assembler "%lo\\(f\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_f\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_d\\)" } } */

double d;
double __attribute__((aligned(1))) a_d;
float f;
float __attribute__((aligned(1))) a_f;

void foo (void)
{
 d = 0;
 f = 0;
 a_d = 0;
 a_f = 0;
}

