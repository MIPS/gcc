#include <concepts>

concept Indexable<typename T> {
  std::CopyConstructible value_type;
  value_type operator[](const T&, int);
}

class E {
  template<Indexable T> 
  static T::value_type 
  index(const T& x, int index) { return x[index]; }
};
