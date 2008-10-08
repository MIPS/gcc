#include <concepts>

template<typename T, typename U>
struct is_same
{
  static const bool value = false;
};

template<typename T>
struct is_same<T, T>
{
  static const bool value = true;
};

template<typename T, typename U> requires std::SameType<T, U>
bool compare1(T* t, U* u)
{
  return t == u;
}

template<typename T, typename U> requires std::True<is_same<T, U>::value>
bool compare2(T* t, U* u)
{
  return t == u; // { dg-error "no match" }
}

// { dg-error "candidates are" "" { target *-*-* } 24 }
// { dg-error "==" "" { target *-*-* } 24 }
