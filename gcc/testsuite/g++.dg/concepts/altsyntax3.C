// { dg-options "-std=c++0x" }
#include <concepts>

namespace N {
  concept Foo<typename T>
  {
  };
}

template<N::Foo X> void f(X);
