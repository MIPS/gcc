// { dg-do compile }
// { dg-options "-O" }
// Do not scan for the devirtualized call for now (PR 38178)
//  dg-final { scan-assembler "xyzzy" } 

struct S { S(); virtual void xyzzy(); };
inline void foo(S *s) { s->xyzzy(); }
void bar() { S s; foo(&s); }
