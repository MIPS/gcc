#include <concepts>

auto concept MutuallyLessThanComparable<typename T, typename U>
{
  bool operator<(const T&, const U&);
  bool operator<(const U&, const T&);
};

concept_map MutuallyLessThanComparable<int, int> { };

template<typename T, typename U>
  requires MutuallyLessThanComparable<T, U>
  void foo(T, U);

void bar()
{
  foo(17, 42);
}
