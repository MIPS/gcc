/* { dg-do compile } */

/* Test GIMPLE_BIND.  */

int global;

void foo(int p)
{
  int bar = p, joe;

  joe = bar;
  global = joe;
}

/* { dg-final { scan-tree-dump-times "gimpleir: GIMPLE_BIND tuple" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
