// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

template <typename T> class bar {
  friend inline bool operator==(const bar&, const bar&) { return true; }
};
bar<int> i;
bar<void*> vp;
bool f() {
  return i == i && vp == vp;
}
