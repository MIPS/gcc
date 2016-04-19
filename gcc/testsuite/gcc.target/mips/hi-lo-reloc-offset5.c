/* { dg-options { "-mno-gpopt" } } */
/* { dg-final { scan-assembler-not "%lo\\(d\\+4\\)" } } */

double long long __attribute__((aligned(1))) d;

void foo (void)
{
 d = 0;
}

