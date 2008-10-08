concept Foo<typename T> {
  void T::foo(); // { dg-error "unsatisfied requirement" }
}

struct X {
  void bar();
};

concept_map Foo<X> {
  void X::foo() { bar(); } // { dg-error "cannot override" }
} // { dg-error "Foo" }
