// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 1 { xfail { powerpc-ibm-aix* } } } }

template <typename T, typename U> void f (T, U) {}
template <typename T> void f (T, T) {}
class bar {
  template <typename T> friend void f (T, T);
};
bar t;
template void f<> (int, long);
template void f<> (int, int);
