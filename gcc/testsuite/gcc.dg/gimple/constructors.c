/* { dg-do compile } */

/* Test CONSTRUCTORs.  */

void bar (int *);

void foo()
{
    int a[5] = { 1, 2, 3, 4, 5 };
    bar(a);
}

/* { dg-final { scan-tree-dump-times "gimpleir: a\\\[0\\\] = 1" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: a\\\[1\\\] = 2" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: a\\\[2\\\] = 3" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: a\\\[3\\\] = 4" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: a\\\[4\\\] = 5" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: bar \\(&a\\)" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
