/* { dg-options "-mcode-readable=no -mno-gpopt -mabi=32" } */
/* { dg-final { scan-assembler-not "%lo\\(a_l\\+4\\)" } } */
/* { dg-final { scan-assembler "%lo\\(l\\+4\\)" } } */

long long l;
double d;

long long __attribute__((aligned(1))) a_l;
double __attribute__((aligned(1))) a_d;

void foo (void)
{
   d = (double) l;
   a_d = (double) a_l;
}
