#include <concepts>

template <typename TL, typename TR> 
requires std::IntegralLike<TL>, std::IntegralLike<TR> 
std::HasPlus<TL,TR>::result_type
std_add (TL const & lhs,TR const & rhs) {

return lhs + rhs; // { dg-bogus "no match" "" { xfail *-*-* } }

}

// { dg-bogus "operator" "" { xfail *-*-* } 256 }
// { dg-bogus "operator" "" { xfail *-*-* } 221 }
