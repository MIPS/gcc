// { dg-options "-std=c++0x" }
// Contributed by Christopher Eltschka
#include <concepts>

template<typename Char, typename Traits> class X;

auto concept IsCharTraits<typename T, typename Char>
{
  typename char_type = T::char_type;
  requires std::SameType<Char, char_type>;
};

template<typename Char, typename Traits> requires IsCharTraits<Traits,
Char>
 void foo(X<Char, Traits>& o)
{
  o << 1; // { dg-error "no match for" }
}
