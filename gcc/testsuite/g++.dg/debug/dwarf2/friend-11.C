// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

template <typename> struct foo {
  struct f {};
};
class bar {
  template <typename T> friend struct foo<T>::f;
};
bar t;
foo<int>::f i;
foo<bar>::f b;
