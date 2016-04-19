/* { dg-final { scan-assembler-not "%lo\\(h\\+6\\)" } } */

struct __attribute__((packed))
{
 short s;
 double d;
} h;

void foo (void)
{
 h.d = 0;
}

