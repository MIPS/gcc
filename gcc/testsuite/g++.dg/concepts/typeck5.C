#include <concepts>

auto concept Addable<typename T, typename U = T>
{
  typename result_type = T;
  requires std::CopyConstructible<result_type>;
  result_type operator+(const T&, const U&);
};

template<typename X, typename Y> requires Addable<X, Y>
Addable<X, Y>::result_type add(const X& x, const Y& y)
{
  Addable<X, Y>::result_type res = x + y;
  return res;
}
