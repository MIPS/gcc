// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<typename T>
  requires C<T>()
    struct Test { };

template struct Test<int>; // { dg-error "constraint" }

int main() { }
