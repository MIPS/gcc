// { dg-do "run" }
#include "core_concepts.h"
#include <cassert>

struct B {};

auto concept DefaultConstructible<typename T> {
  T::T();
}

auto concept Convertible<typename T, typename U> {
  operator U(T);
}

concept_map DefaultConstructible<B> { }

int mangle(B const&, B const&) {return true;}
int mangle(B&&, B&&) {return false;}


auto concept Manglable<typename T>
{
  typename result_type;
  result_type mangle(T const&, T const&);
}

concept_map Manglable<B> { }

template <Manglable T>
requires DefaultConstructible<T> && Convertible<T::result_type, bool>
void mangle_it()
{
  T t1, t2;
  assert(mangle(t1, t2));
  assert(!mangle(T(), T()));
}

int main ()
{
  mangle_it<B>();
  assert(!mangle(B(), B()));
}
