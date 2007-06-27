/* { dg-do compile } */

/* Test GS_ASSIGN.  */

int a, b, c;

void foo()
{
    a = b + c;
}

/* { dg-final { scan-tree-dump-times "gimpleir: a.2 = b.0 \\+ c.1" 1 "gimple"} } */
