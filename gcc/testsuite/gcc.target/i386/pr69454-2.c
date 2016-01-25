/* { dg-do compile { target { ia32 } } } */
/* { dg-options "-O2 -msse2 -mno-accumulate-outgoing-args -mpreferred-stack-boundary=2" } */

extern void fn2 (void);
long long a, b;
void
fn1 (void)
{
  long long c = a;
  a = b ^ a;
  fn2 ();
  a = c;
}
