// { dg-do compile }
concept Foo<typename T> {
  void operator++(T&, int);
}
