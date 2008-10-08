#include "core_concepts.h"
template<typename T, typename U>
requires std::SameType<T, U>
void f(const T&, const U&); // { dg-error "candidates are" }

concept Nothing<typename> { };

template<Nothing T>
void g(const T& x, const T& y)
{
  f(x, y);
}

template<Nothing T1, typename T2>
void h(const T1& x, const T2& y)
{
  f(x, y); // { dg-error "no matching function" }
  // { dg-error "same-type constraint" "" { target *-*-* } 17 }
}
