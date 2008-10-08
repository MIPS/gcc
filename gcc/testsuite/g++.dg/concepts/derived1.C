#include "core_concepts.h"

struct W { };
struct X { };
struct Y : public X { };
struct Z : public virtual Y { };
struct Z2 : private Y { };


template<typename T, typename U>
requires std::DerivedFrom<T, U>
void f(); // { dg-error "candidates are" }

void g()
{
  f<Y, X>(); // okay
  f<Z, X>(); // okay
  f<X, X>(); // { dg-error "no matching|does not derive" }
  f<W, X>(); // { dg-error "no matching|does not derive" }
  f<Z2, X>(); // { dg-error "no matching|does not derive" }
}
