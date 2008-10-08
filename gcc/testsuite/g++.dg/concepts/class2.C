#include <concepts>

template<std::CopyConstructible T1, std::CopyConstructible T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  pair() { } // { dg-error "no matching function for call to" }
  // { dg-error "candidates are" "" { target *-*-* } 598 }

  pair(const T1& a, const T2& b) : first(a), second(b) { }

  first_type first;
  second_type second;
};
