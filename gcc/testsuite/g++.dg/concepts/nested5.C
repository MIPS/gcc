#include <iterator>

template<std::InputIterator Iter, typename T>
requires std::EqualityComparable<Iter::value_type, T>
Iter::difference_type count(Iter first, Iter last, const T& value)
{
  Iter::difference_type n = 0;
  for ( ; first != last; ++first)
    if (*first == value)
      ++n;
  return n;
}
template<std::InputIterator Iter>
void advance(Iter& x, Iter::difference_type n)
{
  while (n--) ++x;
}

template<std::BidirectionalIterator Iter>
void advance(Iter& x, Iter::difference_type n)
{
  if (n > 0) { while (n--) ++x; }
  else { while (n++) --x; }
}

template<std::RandomAccessIterator Iter>
void advance(Iter& x, Iter::difference_type n)
{
  x += n;
}

template<std::RandomAccessIterator Iter>
void honk(Iter x)
{
  advance(x, 2);
}
