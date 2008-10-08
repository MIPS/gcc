#include <iostream>
#include <iterator>
#include <string>
#include <sstream>

namespace std {
  template<typename T>
  concept_map Range<istream_iterator<T>> {
    typedef istream_iterator<T> iterator;
    iterator begin(iterator& iter) { return iter; }
    iterator end(iterator&)        { return iterator(); }
  }
}

extern "C" {
  void abort();
}

int main()
{
  std::stringstream str("1 2 3 4 5");
  int sum = 0;
  for (int i : std::istream_iterator<int>(str))
    sum += i;

  if (sum != 15)
    abort();
}
