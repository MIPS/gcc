#include <concepts>
concept LessThanComparable<typename T>
{
  bool operator<(const T& x, const T& y);
};

template<typename T> requires LessThanComparable<T>
struct less
{
};

concept Nothing<typename T> { };

template<Nothing T>
bool f(less<T>, const T& x, const T& y)
{
  return x < y;
}

