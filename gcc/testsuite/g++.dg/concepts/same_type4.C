#include <concepts>
template<typename T> struct X {};

struct C {};

template<typename T, typename U>
  requires std::SameType<T C::*, U C::*>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T*, U*>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T&, U&>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T[], U[]>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T[17], U[17]>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<X<T>, X<U> >
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T(int), U(int)>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<int(T), int(U)>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T (C::*)(int), U (C::*)(int)>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T const, U const>
  T& select(bool cond, T& t, U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T volatile, U volatile>
  T& select(bool cond, T& t, U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T const volatile, U const volatile>
  T& select(bool cond, T& t, U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T const, U>
  const T& select(bool cond, const T& t, U& u)
  { return cond? t : u; }
