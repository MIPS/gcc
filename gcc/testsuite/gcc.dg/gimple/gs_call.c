/* { dg-do compile } */

/* Test GS_CALL.  */

void bar();
void tool();

void foo()
{
    bar(), tool();
}

/* { dg-final { scan-tree-dump-times "gimpleir: bar \\(\\)" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: tool \\(\\)" 1 "gimple"} } */
