namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Foo<typename T> {};

auto concept Bar<typename T> : Foo<T>
{
  T bar(T);
};

struct X {};
X bar(X);
concept_map Foo<X> {};

struct Y {};
Y bar(Y);

template<typename T> requires Bar<T> void needs_bar(T);

void f()
{
  needs_bar(X());
  needs_bar(Y()); // { dg-error "no matching function for call to" }
  // { dg-error "candidates are" "" { target *-*-* } 23 }
  // { dg-error "no concept map for requirement" "" { target *-*-* } 28 }
}
