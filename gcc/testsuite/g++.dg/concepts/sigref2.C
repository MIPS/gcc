// { dg-do compile }
#include <concepts>

concept Foo<typename T> {
  void foo(T);
}

concept_map Foo<int> {
  void foo(const int&) {}
}
