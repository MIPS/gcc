#include <concepts>

template<std::CopyConstructible T>
struct list {
  requires std::LessThanComparable<T> T silly_min(const T& x, const T& y) 
  { 
    return x < y? x : y; 
  }
};

