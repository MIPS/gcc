/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler "%lo\\(l\\+4\\)" } } */
/* { dg-final { scan-assembler-not "%lo\\(a_l\\+4\\)" } } */

unsigned long long l;
unsigned long long __attribute__((aligned(1))) a_l;

void
foo (void)
{
  l = 0;
  a_l = 0;
}
