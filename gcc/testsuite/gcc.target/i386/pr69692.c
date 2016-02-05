/* { dg-do compile { target { ia32 } } } */
/* { dg-options "-O2 -msse2 -mno-accumulate-outgoing-args -mpreferred-stack-boundary=2" } */

struct bar
{
  int i[16] __attribute__ ((aligned(16)));
};

extern void fn2 (void);
long long a, b;

struct bar
fn1 (struct bar x)
{
  long long c = a;
  a = b ^ a;
  fn2 ();
  a = c;
  return x;
}

/* { dg-final { scan-assembler "pxor\[^\n\]*xmm" } } */
