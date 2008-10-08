namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Fooable<typename T>
{
  void foo(T);
};

namespace outer_foo_ns {
  concept OuterFooable<typename T>
  {
    void foo(T);
  };

  namespace inner_foo_ns {
    concept InnerFooable<typename T>
    {
      void foo(T);
    };
  }
}


struct X {};
void foo(X);

template<typename T> requires Fooable<T> void use_foo(T);
template<typename T> requires outer_foo_ns::OuterFooable<T> void use_foo(T);

template<typename T> requires outer_foo_ns::inner_foo_ns::InnerFooable<T>
  void use_foo(T);


void check()
{
  use_foo(X()); // { dg-error "no matching" }
  // { dg-error "candidates are" "" { target *-*-* } 32 }
  // { dg-error "no concept map for requirement" "" { target *-*-* } 41 }
  // { dg-error "syntactically matches" "" {target *-*-* } 9 }
  // { dg-error "if the concept semantics are met" "" {target *-*-* } 41 }
  // { dg-error "use_foo" "" { target *-*-* } 33 }
  // { dg-error "syntactically matches" "" {target *-*-* } 15 }
  // { dg-error "namespace outer_foo_ns" "" {target *-*-* } 0 }
  // { dg-error "oncept_map OuterFooable" "" {target *-*-* } 0 }
  // { dg-error "\}" "" {target *-*-* } 0 }
  // { dg-error "use_foo" "" { target *-*-* } 36 }
  // { dg-error "syntactically matches" "" {target *-*-* } 21 }
  // { dg-error "namespace inner_foo_ns" "" {target *-*-* } 0 }
}
