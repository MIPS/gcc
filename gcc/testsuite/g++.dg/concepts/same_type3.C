#include <concepts>
concept SameU<typename T>
{
  typename U = T;
  requires std::SameType<T, U>;
};

template<typename T>
requires SameU<T>
const T& select(bool cond, const T& t, const SameU<T>::U& u)
  { return cond? t : u; }

concept C1<typename T>
{
  typename assoc1;
};

concept C2<typename T>
{
  typename assoc2;
};

template<typename T>
requires C1<T>, C2<T>, std::SameType<C1<T>::assoc1, C2<T>::assoc2>
const C1<T>::assoc1& select(bool cond, const C1<T>::assoc1& t, const C2<T>::assoc2& u)
  { return cond? t : u; }
