/* { dg-do compile } */
/* { dg-options "-O -fgimple" } */

struct X { int a; };
struct X x;

int __GIMPLE ()
foo (struct X *p, _Complex int q)
{
  int b;
  b = __real q;
  p->a = b;
  x.a = b;
  b = p->a;
  b = x.a;
  return b;
}
