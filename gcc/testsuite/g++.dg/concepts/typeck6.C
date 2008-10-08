#include <concepts>

concept Magma<typename T> : std::CopyConstructible<T>
{
  T operator+(T, T);
};

concept Semigroup<typename T> : Magma<T>
{
};

template<typename T> requires Semigroup<T>
T add3(const T& x, const T& y, const T& z)
{ return x + (y + z); }
