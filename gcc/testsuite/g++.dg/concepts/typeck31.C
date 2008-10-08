// { dg-options "-std=c++0x" }
// Contributed by Christopher Eltschka
#include <concepts>

concept has_foo<typename T>
{
  void foo(T&);
};

template<typename F> requires std::Callable0<F>
 concept_map has_foo<F>
{
  void foo(F& f) { f(); }
};
