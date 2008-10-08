#include <concepts>
template<typename T, typename U>
  requires std::SameType<T, U>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T>
  requires std::SameType<T, int>
  const T& select_int(bool cond, const T& t, const int& u)
  { return cond? t : u; }

template<typename T>
  requires std::SameType<T, int>
  T select_int17(bool cond, T& t)
  {
    return cond? t : 17;
  }

concept CopyConstructible<typename T>
{
  T::T(const T&);
};

template<typename T, typename U>
  requires CopyConstructible<T>, CopyConstructible<U>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; } // { dg-error "have different types" }
