// { dg-do link }
#include <concepts>

namespace fake_std {
  concept InputIterator<typename Iter>
  {
    typename difference_type;
  };

  concept RandomAccessIterator<typename Iter> : InputIterator<Iter>
  {
  };

  template<InputIterator Iter> void advance(Iter&, Iter::difference_type);
  template<RandomAccessIterator Iter> void advance(Iter&, Iter::difference_type) {}

  template<InputIterator Iter> void move(Iter& i, Iter::difference_type n)
  {
    advance(i, n);
  }
}

namespace N {
  struct MyIter {};
}

namespace fake_std {
  concept_map RandomAccessIterator<N::MyIter>
  {
    typedef int difference_type;
  };
}

int main()
{
  N::MyIter my_iter;
  fake_std::move(my_iter, 17);
}
