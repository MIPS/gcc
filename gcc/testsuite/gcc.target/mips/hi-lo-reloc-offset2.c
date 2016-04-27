/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler-not "%lo\\(m\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(n\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(o\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(p\\+\[1-9\]\[0-9\]?)" } } */
/* { dg-final { scan-assembler-not "%lo\\(q\\+\[1-9\]\[0-9\]?)" } } */

struct __attribute__((packed))
{
  char c;
  short s;
} m __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  int i;
} n __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  long long l;
} o __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  float f;
} p __attribute__((aligned(1)));

struct __attribute__((packed))
{
  char c;
  double d;
} q __attribute__((aligned(1)));

void
foo (void)
{
  m.s = 0;
  n.i = 0;
  o.l = 0;
  p.f = 0;
  q.d = 0;
}
