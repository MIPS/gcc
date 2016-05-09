/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler "%lo\\(h\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(j\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(k\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(m\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(n\\+\[1-9\]\[0-9\]?)" } } */

struct __attribute__((packed))
{
  char c;
  short s;
} a __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  int i;
} b __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  long long l;
} c __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  float f;
} d __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  double d;
} e __attribute__((aligned(1)));

void
foo (void)
{
  a.s = 0;
  b.i = 0;
  c.l = 0;
  d.f = 0;
  e.d = 0;
}
