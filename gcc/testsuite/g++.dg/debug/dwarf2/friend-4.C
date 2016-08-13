// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 1 { xfail { powerpc-ibm-aix* } } } }

struct foo {
  int f();
};
class bar {
  friend int foo::f();
};
int foo::f() {}
bar t;
