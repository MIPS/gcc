/* { dg-do compile } */

extern int foo (int);
extern int bar (int);

baz (int i)
{
  int a = foo (bar (i));
  return a;
}
