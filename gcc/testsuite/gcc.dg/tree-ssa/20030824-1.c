/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */

struct A
{
  int a,b;
};

int foo (int x, int y)
{
  int i, j;
  struct A a,b;

  a.a = x;
  b.b = y;
  j = a.a;
  i = b.b;
  return i + j;
}

/* This function should be optimized into 'return y+x'.  */
/* { dg-final { scan-tree-dump-times "return y \\+ x" 1 "optimized"} } */
