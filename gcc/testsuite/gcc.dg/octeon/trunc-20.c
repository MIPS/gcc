/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "\tsll\t\[^\n\]*,0" } } */

void f (int a)
{
  long long b;
  extern char c;

  a *= 2;
  b = a;
  c = (char) b;
}
