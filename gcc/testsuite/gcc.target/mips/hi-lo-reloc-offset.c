/* { dg-final { scan-assembler-not "%lo\\(h\\+6\\)" } } */

struct __attribute__((packed))
{
 short s;
 unsigned long long l;
} h;

void foo (void)
{
 h.l = 0;
}

