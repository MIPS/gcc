/* { dg-options { "-mno-gpopt" } } */
/* { dg-final { scan-assembler-not "%lo\\(h\\+4\\)" } } */

unsigned long long __attribute__((aligned(1))) h;

void foo (void)
{
 h = 0;
}

