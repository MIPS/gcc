// { dg-do compile }

concept Foo<typename T> {
  void foo(const T&);
}

concept_map Foo<int> {
  void foo(int) {}
}
