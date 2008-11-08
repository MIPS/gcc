// { dg-do compile }
// { dg-options "-O2 -flto-single -fno-strict-aliasing" }
extern int foo();
void bar() { try { int i = foo(); } catch(int) { } }
