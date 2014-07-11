// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<C T> struct S;

struct X { };

// Test explicit specialization
template<> struct S<X> { };

int main() { }
