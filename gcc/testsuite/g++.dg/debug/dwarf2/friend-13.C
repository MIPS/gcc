// { dg-do compile }
// { dg-options "-O -g -dA -Wno-non-template-friend" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

void f(int) {}
void f(long) {}
void f(char) {}
template <typename T> struct foo {
  friend void f(T);
};
foo<int> i;
foo<long> l;
