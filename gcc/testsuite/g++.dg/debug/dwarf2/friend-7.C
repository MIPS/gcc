// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

template <typename> void f () {}
class bar {
  template <typename> friend void f ();
};
bar t;
template void f<int> ();
template void f<bar> ();
