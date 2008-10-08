#include <concepts>
#include <iterator>

class Foo {
public:
  typedef char value_type;
  typedef int difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef std::random_access_iterator_tag iterator_category;
  value_type operator*() const;
};

template<std::RandomAccessIterator Iter>
int Test() {
  return 0;
}

int main() {
  return Test<Foo>();  // { dg-error "no concept map" "" { xfail *-*-* } }
}
