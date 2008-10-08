#include <concepts>

concept Semigroup<typename T> {
  T operator+(T, T);

  axiom Associative(T x, T y, T z) {
    (x + y) + z == x + (y + z);
  }
};

concept_map Semigroup<int> {};
