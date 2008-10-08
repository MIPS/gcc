// { dg-do "run" }
#include <iterator>
#include <vector>
#include <list>
#include <cassert>


template<std::InputIterator Iter>
requires (std::HasPlus<typename Iter::value_type, typename Iter::value_type>,
       std::CopyAssignable<typename Iter::value_type, typename std::HasPlus<typename Iter::value_type, typename Iter::value_type>::result_type>,
       std::DefaultConstructible<typename Iter::value_type>,
       std::CopyConstructible<typename Iter::value_type>)
typename Iter::value_type foo(Iter first, Iter last)
{
  std::vector<typename Iter::value_type> vec(first, last);

  typename Iter::value_type sum = typename Iter::value_type();
  for (typename Iter::value_type i : vec) // { dg-bogus "cannot iterate|you need" "" { xfail *-*-* } }
    sum = sum + i; // { dg-bogus "not declared|internal" "" { xfail *-*-* } }

  return sum;
}

int main()
{
  std::list<int> values;
  for (int i = 1; i <= 10; ++i)
    values.push_back(i);
  assert(foo(values.begin(), values.end()) == 55);
  return 0;
}
