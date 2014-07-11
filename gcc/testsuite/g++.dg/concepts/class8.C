// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<C T> struct S;

struct X { };

// Not a valid declaration, int does not satisfy C.
template<> struct S<int> { }; // { dg-error "deduction" }

int main() { }
