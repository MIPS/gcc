// { dg-do compile }
// { dg-options "-O -g -dA" }
// { dg-final { scan-assembler-times " DW_AT_friend" 3 { xfail { powerpc-ibm-aix* } } } }

template <typename T> struct foo {
  struct bar { struct baz {}; };
};
template <typename T> struct foo<T*> {
  struct bar { struct baz {}; };
};
template <> struct foo<int> {
  struct bar { struct baz {}; };
};
class boo {
  template <typename T> friend struct foo<T>::bar;
};
foo<void>::bar::baz v;
foo<void*>::bar::baz vp;
foo<int>::bar::baz i;
boo m;
