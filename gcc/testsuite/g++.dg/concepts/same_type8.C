// { dg-options "-std=c++0x" }
// Contributed by Christopher Eltschka
#include "core_concepts.h"

concept Foo<typename F> {
  typename reference;
  typename value_type;
  requires std::SameType<reference, value_type&>;

  reference operator*(F);
};

template<Foo F>
F::value_type& foo(F f) {
  return *f; 
}
