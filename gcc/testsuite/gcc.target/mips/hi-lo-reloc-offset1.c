/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler-not "%lo\\(h\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(j\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(k\\+\[1-9\]\[0-9\]?)" } } */

struct __attribute__((packed))
{
  char c;
  short s;
  int i;
} h __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  long long l;
} j __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  float f;
  double d;
} k __attribute__((aligned(1)));

void
foo (void)
{
  h.c = 0;
  h.s = 0;
  h.i = 0;
  j.l = 0;
  k.f = 0;
  k.d = 0;
}
