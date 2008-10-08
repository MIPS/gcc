#include <concepts>
template<typename T>
  requires std::SameType<T, int>
  int add(T x, T y)
  {
    return x + y;
  }
