/* { dg-options "-mcode-readable=no -mabi=32 -mfpxx isa=2" } */
/* { dg-final { scan-assembler-not "%lo\\(h\\+\[1-9\]\\)" } } */

struct __attribute__((packed))
{
 short s;
 double d;
 float f;
} h;

void foo (void)
{
 h.d = 0;
 h.f = 0;
}

