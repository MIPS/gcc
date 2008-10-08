#include <concepts>
template<typename T, typename U>
  requires std::SameType<T, U>
  T* select(bool cond, T* t, U* u)
  { return cond? t : u; }

template<typename T>
  requires std::SameType<T, int>
  T* select_int(bool cond, T* t, int* u)
  { return cond? t : u; }

template<typename T>
  requires std::SameType<T, int>
  T* select_int17(bool cond, T* t)
  {
    return cond? t : 0;
  }

template<typename T, typename U> requires std::SameType<T, T>
  T* select(bool cond, T* t, U* u)
  { return cond? t : u; } // { dg-error "distinct pointer types" }
