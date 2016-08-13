// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-not " DW_AT_friend" { xfail { powerpc-ibm-aix* } } } }
// class foo is unused, so we do NOT output the friend tag.

class foo {};
class bar {
  friend class foo;
};
bar t;
// foo l; 
