#include <concepts>

auto concept Convertible<typename X, typename Y>
{
};

concept Foo<typename X, typename Y, typename Z>
{
  requires Convertible<X, Y>, Convertible<X, Z>;
};

concept RefinesFoo<typename X, typename Y, typename Z> : Foo<X, Y, Z>
{
  requires std::SameType<Y, Z>;
};

template<typename T1, typename T2, typename T3>
  requires RefinesFoo<T1, T2, T3>
  int& f();

template<typename X1, typename X2, typename X3>
  requires Foo<X1, X2, X3>
  float& f();

concept_map RefinesFoo<int, float, float> {};

void g()
{
  int& x = f<int, float, float>();
}
