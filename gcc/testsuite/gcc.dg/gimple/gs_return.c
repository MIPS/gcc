/* { dg-do compile } */

/* Test GS_RETURN.  */

int a, b;

int foo()
{
    return a + b;
}

/* { dg-final { scan-tree-dump-times "gimpleir: return" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
