/* { dg-do compile } */

extern int foo (int);
extern int bar (int);

baz (int i)
{
  int a = foo (bar (i));
  return a;
}

/* { dg-final { scan-tree-dump-times "gimpleir: bar.0 = bar \\(i\\)" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: foo.1 = foo \\(bar.0\\)" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
