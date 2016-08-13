// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

template <typename> class foo {};
class bar {
  template <typename> friend class foo;
};
bar t;
foo<int> l;
foo<bar> b;
