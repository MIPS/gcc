// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<typename T>
  requires C<T>()
    struct Test { };

struct X { };

template struct Test<X>;

int main() { }
