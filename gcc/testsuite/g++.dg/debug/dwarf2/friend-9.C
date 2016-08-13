// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

template <typename> struct foo {
  void f () {}
};
class bar {
  template <typename T> friend void foo<T>::f ();
};
bar t;
template void foo<int>::f ();
template void foo<bar>::f ();
