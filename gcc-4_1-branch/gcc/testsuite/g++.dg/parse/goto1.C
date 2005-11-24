// PR 24452
// { dg-do compile }

void foo()
{
    x;		// { dg-error "not declared" }
    L: goto L;
}
