// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<typename T>
  struct S1 {
    S1() requires C<T>() { }
  };

template<typename T>
  struct S2 : S1<T> {
    using S1<T>::S1;
  };

struct X { };

int main() { 
  S2<X> s;
}

