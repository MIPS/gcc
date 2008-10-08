#include <concepts>

concept C<class T> { T::T (); };

using namespace std;

template <typename T>
struct A { // { dg-error "candidates are" }
  requires (C<T>) A (): t () { } // { dg-error "candidates are" "" { xfail *-*-* } }
  T t;
};

struct B { private: B (); };

int main ()
{
  A<B> a; // { dg-error "no matching function" }
}
