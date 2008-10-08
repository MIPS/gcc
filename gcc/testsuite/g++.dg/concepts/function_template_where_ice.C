// { dg-do compile }
#include <concepts>
template<int N, typename T> requires std::True<(N >= 0)> T int_pow(T);

void foo()
{
  int_pow<5>(3.14159); // Okay
  int_pow<-1>(2.71828); // { dg-error "no matching function for call to" }
  // { dg-error "candidates are" "" { target *-*-* } 3 }
  // { dg-error "integral constant expression" "" { target *-*-* } 8 }
}
