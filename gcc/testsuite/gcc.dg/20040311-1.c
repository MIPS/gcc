/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-options "-march=i686 -O2 -ffast-math -fPIC" { target i?86-*-* } } */

extern double foo (double x);
extern double bar (double x, double y);
extern double baz (double x) __attribute__ ((__const__));

void
test (double x, double y, double z)
{
  double a[7], b, c;
  int i;

  b = -x * x / 3;
  for (i = 0; i < 6; i++)
    a[i] = -x / 3;
  c = bar (baz (y), 1.0 / 3) - b / 3.0 * bar (baz (y), -1.0 / 3);
  if (!(y < 0.001 && y > -0.001))
    a[i] = foo (z / 3.0) * c;
}
