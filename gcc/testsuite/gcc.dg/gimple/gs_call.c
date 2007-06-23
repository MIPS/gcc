/* { dg-do compile } */

/* Test GS_CALL.  */

void bar();
void tool();

void foo()
{
    bar(), tool();
}
