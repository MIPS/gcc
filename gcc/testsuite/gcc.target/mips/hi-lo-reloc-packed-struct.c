/* { dg-options "-mno-gpopt" } */
/* { dg-final { scan-assembler-not "%lo\\(n\\+\[4-9\])" } } */
/* { dg-final { scan-assembler-not "%lo\\(n\\+\1\[0-9\])" } } */

struct __attribute__((packed))
{
  char c;
  short s;
  int i;
  long long l;
} n;

void
foo (void)
{
  n.c = 0;
  n.s = 0;
  n.i = 0;
  n.l = 0;
}
