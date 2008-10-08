#include <concepts>

auto concept Convertible<typename T, typename U>
{
  operator U(const T&);
};

template<typename X, typename Y> 
requires Convertible<Y, X> && std::CopyConstructible<X>
X implicit_convert(const Y& y)
{
  return y;
}

concept Nothing<typename T> { };

template<typename X, Nothing Y>
requires std::CopyConstructible<X>
X implicit_convert(const Y& y)
{
  return y; // { dg-error "conversion from" }
}
