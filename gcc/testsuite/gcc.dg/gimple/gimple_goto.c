/* { dg-do compile } */

/* Test GIMPLE_GOTO.  */

void yanory()
{
hotdog:
  goto hotdog;
}

/* { dg-final { scan-tree-dump-times "gimpleir: goto hotdog" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: hotdog:" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
