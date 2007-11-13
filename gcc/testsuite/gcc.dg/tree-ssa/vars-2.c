/* { dg-do compile } */
/* { dg-options "-O -g -fvar-tracking -fdump-tree-final_cleanup-vars -fdump-rtl-vartrack" } */

/* In this test we want to keep the name "j" associated with
   the expression "i * 2" passed to the inlined function "bar"
   even in the presence of RTL splitters.  */

static int bar(int j)
{
  return j;
}
int l;
int foo(int i)
{
  l = bar(i*2);
  return l;
}

/* { dg-final { scan-tree-dump "i \\\* 2 E{ j }" "final_cleanup" } } */
/* { dg-final { scan-rtl-dump "\\\[orig:.* j \\\]" "vartrack" } } */
/* { dg-final { cleanup-tree-dump "final_cleanup" } } */
