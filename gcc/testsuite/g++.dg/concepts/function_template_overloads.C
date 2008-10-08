// { dg-do compile }
#include <concepts>
struct positive {};
struct negative {};

template<int N> struct int2type {};

template<int N> requires std::True<(N > 0)> positive classify(int2type<N>);
template<int N> requires std::True<(N < 0)> negative classify(int2type<N>);

void foo()
{
  positive p = classify(int2type<17>());
  negative n = classify(int2type<-1>());
  classify(int2type<0>()); // { dg-error "no matching function for call to" }
  // { dg-error "positive classify" "" { target *-*-* } 8 }
  // { dg-error "negative classify" "" { target *-*-* } 9 }
  // { dg-error "integral constant expression" "" { target *-*-* } 15 }
}
