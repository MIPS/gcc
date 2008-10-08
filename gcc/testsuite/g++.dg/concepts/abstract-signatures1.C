#include "core_concepts.h"

auto concept Addable<typename T> {
  T::T();
  T operator+(T const&, T const&);
}

template<Addable T>
requires std::CopyConstructible<T>
void f()
{
  T t;
  t + t;
  t + T(); // { dg-error "within this context" }
}

class X { 
public:
  X operator+(const X&) const;

private:
  X operator+(X&&) const; // { dg-error "is private" }
};



void g()
{
  f<X>(); // { dg-error "instantiated from here" }
}
