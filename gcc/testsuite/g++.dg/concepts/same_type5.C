#include <concepts>
concept CopyConstructible<typename T>
{
  T::T(const T&);
};

template<typename T> struct X {};
template<typename T> struct Y {};

struct C {};
struct D {};

template<typename T, typename U>
  requires std::SameType<T C::*, U D::*> // { dg-error "invalid same-type" }
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T*, U&>, // { dg-error "invalid same-type" }
            CopyConstructible<T>, CopyConstructible<U>
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; } // { dg-error "have different types" }

template<typename T, typename U>
  requires std::SameType<T, int>, std::SameType<U, float>, std::SameType<T, U> // { dg-error "invalid same-type" }
  const T& select(bool cond, const T& t, const U& u)
  { return t; }

template<typename T, typename U>
  requires std::SameType<T[17], U[]> // { dg-error "requires array sizes" }
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<X<T>, Y<U> > // { dg-error "same template" }
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T(int), U(int, float)> // { dg-error "number of arguments" }
  const T& select(bool cond, const T& t, const U& u)
  { return cond? t : u; }

template<typename T, typename U>
  requires std::SameType<T const, U volatile> // { dg-error "incompatible qualifiers" }
  void select(bool cond, const T& t, const U& u)
  {  }
