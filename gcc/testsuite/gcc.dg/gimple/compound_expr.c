/* { dg-do compile } */

/* Test COMPOUND_EXPRs.  */

int a, b, c;

void foo()
{
    a = (b, c);
}

/* { dg-final { scan-tree-dump-times "gimpleir: a = c" 1 "gimple"} } */
