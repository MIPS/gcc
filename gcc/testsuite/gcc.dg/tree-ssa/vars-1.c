/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-final_cleanup-vars" } */

/* In this test we want to keep the name "j" associated with
   the expression "i * k" passed to the inlined function "bar".  */

static int bar(int j)
{
  return j;
}
int l;
int foo(int i, int k)
{
  l = bar(i*k);
  return l;
}

/* { dg-final { scan-tree-dump "k \\\* i E{ j }" "final_cleanup" } } */
/* { dg-final { cleanup-tree-dump "final_cleanup" } } */
