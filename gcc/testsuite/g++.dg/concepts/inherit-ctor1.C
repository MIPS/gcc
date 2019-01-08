<<<<<<< HEAD
// { dg-do compile { target c++17 } }
// { dg-options "-fconcepts" }
=======
// needs port; no tests where parent type is constrained
<<<<<<< HEAD
// { dg-options "-std=c++17 -fconcepts" }
>>>>>>> 594131fbad3... move ported tests; note more issues and needs port after fixes
=======
// { dg-options "-std=c++2a" }
>>>>>>> eda685858ca... move more ported tests

template<typename T>
  concept C = __is_class(T);

template<typename T>
  struct S1 { S1(double) requires C<T> { } };

struct S2 : S1<int> {
  using S1<int>::S1;
};

template<typename T>
  struct S3 : S1<T> {
    using S1<T>::S1;
  };

struct X { };

int main() {
  S3<X> s(0.0);
}
