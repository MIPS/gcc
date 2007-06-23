/* { dg-do compile } */

/* Test CONSTRUCTORs.  */

void bar (int *);

void foo()
{
    int a[5] = { 1, 2, 3, 4, 5 };
    bar(a);
}
