/* { dg-do compile } */

/* Test GS_COND.  */

int res, a, b, c;

void foo()
{
    res = a > 5 ? b : c;
}

/* { dg-final { scan-tree-dump-times "gimpleir: GS_COND tuple" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
