/* { dg-options { "-mno-gpopt" } } */
/* { dg-final { scan-assembler-not "%lo\\(l\\+4\\)" } } */

long long __attribute__((aligned(1))) l;
double __attribute__((aligned(1))) d;

void foo (void)
{
   d = (double) l;
}
