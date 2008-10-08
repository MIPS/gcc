// { dg-options "-std=c++0x" }
auto concept LessThanComparable<typename T> {
  bool operator<(T, T);

  bool operator>(T x, T y) { return y < x; }
}

struct X { };
bool operator<(X, X);

concept_map LessThanComparable<float> { }
concept_map LessThanComparable<X> { }
concept_map LessThanComparable<int> { }
