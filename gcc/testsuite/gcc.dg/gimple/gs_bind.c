/* { dg-do compile } */

/* Test GS_BIND.  */

int global;

void foo(int p)
{
  int bar = p, joe;

  joe = bar;
  global = joe;
}

/* { dg-final { scan-tree-dump-times "gimpleir: GS_BIND tuple" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
