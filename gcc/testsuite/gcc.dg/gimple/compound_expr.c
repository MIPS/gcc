/* { dg-do compile } */

/* Test COMPOUND_EXPRs.  */

int a, b, c;

void foo()
{
    a = (b, c);
}
