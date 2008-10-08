namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Bar<typename T>
{
  T honk(T);
};

concept Foo<typename T>
{
  requires Bar<T>;
};

int honk(int);

concept_map Bar<int> {};
concept_map Foo<int> {};

concept_map Foo<float&> {}; // { dg-error "does not meet the nested requirements" }
// { dg-error "no concept map for requirement" "" { target *-*-* } 24 }

template<typename T> requires Foo<T> void f();

template<typename T>
concept_map Bar<T*> {};

template<typename T>
concept_map Foo<T*> { };

void gptr()
{
  f<int*>();
}

template<typename T>
concept_map Foo<T&> { };

void gref()
{
  f<int&>(); // { dg-error "no matching function for call" }
}
// { dg-error "candidates are" "" { target *-*-* } 27 }
// { dg-error "no concept map for requirement" "" { target *-*-* } 45 }
