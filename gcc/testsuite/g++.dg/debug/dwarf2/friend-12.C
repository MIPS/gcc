// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 4 { xfail { powerpc-ibm-aix* } } } }

template <typename> struct foo {
  template <typename> struct f {};
};
class bar {
  template <typename T> template <typename> friend struct foo<T>::f;
};
bar t;
foo<int>::f<int> i;
foo<bar>::f<bar> b;
foo<int>::f<bar> ib;
foo<bar>::f<int> bi;
