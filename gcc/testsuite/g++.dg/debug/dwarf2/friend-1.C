// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 1 { xfail { powerpc-ibm-aix* } } } }

class foo {};
class bar {
  friend class foo;
};
bar t;
foo l;
