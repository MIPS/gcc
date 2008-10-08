#include <concepts>

template<std::CopyConstructible T>
struct list {
  requires std::LessThanComparable<T> void sort();
};

