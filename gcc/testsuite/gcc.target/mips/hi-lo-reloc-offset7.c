/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler-not "%lo\\(h\\+\[1-9\]\\)" } } */

struct __attribute__((packed))
{
 char c;
 short s;
 int i;
} h;

void foo (void)
{
 h.c = 0;
 h.s = 0;
 h.i = 0;
}

