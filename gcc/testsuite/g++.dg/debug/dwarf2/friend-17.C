// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 2 { xfail { powerpc-ibm-aix* } } } }

template <typename T> struct foo {
  struct bar { struct baz {}; };
};
template <typename T> struct foo<T*> {
  struct bar { struct baz {}; }; // This baz is not a friend.
};
template <> struct foo<int> {
  struct bar { struct baz {}; }; // Likewise.
};
class boo {
  template <typename T> friend struct foo<T*>::bar::baz;
};
foo<void>::bar::baz v;
foo<void*>::bar::baz vp;
foo<int*>::bar::baz ip;
boo m;
