// { dg-do assemble }
// { dg-options "-flto-single" }

namespace ns
{
  template <class> class hash_set { };
}

struct Foo
{
  long long f1, f2, f3;
};

void func(ns::hash_set<int>) {
  Foo foo = { 0, 0, 0 };
}
