// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

struct foo {
  template <typename> void f () {}
};
class bar {
  template <typename> friend void foo::f ();
};
bar t;
template void foo::f<int> ();
template void foo::f<bar> ();
