namespace std {
  concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Foo<typename T>
{
  int x; // { dg-error "data member" }
  static float y; // { dg-error "static data member" }
  enum E {}; // { dg-error "cannot be declared in" }
  struct A {}; // { dg-error "cannot be declared in" }

  template<typename U> bool operator==(T, U); // { dg-error "is not permitted" }
  // { dg-error "unsupported" "" { target *-*-* } 16 }

  int* T::operator->() const; // { dg-error "member operator" }
  // { dg-error "write as a non-member" "" { target *-*-* } 19 }

  T operator+(T, T, T); // { dg-error "must take either one or two" }
};
