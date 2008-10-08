#include <concepts>

concept Pair<typename P, typename T, typename U>
{
  P::P(const T&, const U&);
};

template<typename P, typename T, typename U>
requires Pair<P, T, U> && std::CopyConstructible<P>
P make_pair(const T& t, const U& u)
{
  return P(t, u);
}

template<typename P, typename T, typename U> 
requires std::SameType<T, T>
P make_pair3(const T& t, const U& u) // { dg-error "candidates are" }
{
  return P(t, u); // { dg-error "no matching function for call to" }
}
