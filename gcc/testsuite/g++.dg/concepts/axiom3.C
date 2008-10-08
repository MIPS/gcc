// { dg-options "-Wall" }
#include <concepts>

concept EqualityComparable<typename T> {
  bool operator==(T, T);

  axiom Reflexivity(T x) {
    x == x;
  }

  axiom Symmetry(T x, T y) {
    if (x == y) y == x;
  }

  axiom Transitivity(T x, T y, T z) {
    if (x == y, y == z) x == z;
  }
}

concept_map EqualityComparable<int> {};
