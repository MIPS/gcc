// { dg-do compile }
#include <concepts>

concept Iterator<typename Iter> {
  typename value_type;
  value_type operator*(const Iter&);
}

template<typename T>
concept_map Iterator<T*> {
  typedef T value_type;
  T operator*(T* p) { return *p; }
}
