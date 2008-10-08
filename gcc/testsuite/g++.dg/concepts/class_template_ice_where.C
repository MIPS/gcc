// { dg-do compile }
#include <concepts>
template<typename T, unsigned N> requires std::True<(N > 0)>, std::True<N>
class array 
{
};

void foo()
{
  array<int, 7> okay;
  array<int, 0> triggers_error; // { dg-error "do not meet the requirements" "incomplete" }
  // { dg-error "note" "" { target *-*-* } 11 }
  // { dg-error "incomplete" "" { target *-*-* } 11 }
}
